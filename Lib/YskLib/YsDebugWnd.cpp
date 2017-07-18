#include "YskLib.h"

using namespace YskLib;

#ifdef DEBUG_WND

// コントロール用ID
#define IDC_EDIT1		4000
#define IDC_SCROLLEDIT1	4001

#define IDC_EDIT_COMMAND		4002

WNDPROC g_EditCommandOldProc;
LRESULT CALLBACK EditCommandProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case WM_KEYDOWN:
		if(wParam == VK_RETURN){
			char tmp[1024];
			GetWindowText(hWnd, tmp, 1024);
			SetWindowText(hWnd, "");

			DebugWnd.AddCommandQueue(tmp);
		}
		else{
			return (CallWindowProc((WNDPROC)g_EditCommandOldProc, hWnd, message, wParam, lParam));
		}
		break;
	default:
		//自分で処理しないものは元のプロシージャにやってもらう
		return (CallWindowProc((WNDPROC)g_EditCommandOldProc, hWnd, message, wParam, lParam));
	}
	return (CallWindowProc((WNDPROC)g_EditCommandOldProc, hWnd, message, wParam, lParam));
}

static LRESULT CALLBACK DebugWndProc( HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
{
	return DebugWnd.getThread().WndProc(hWnd,msg,wP,lP);
}

LRESULT YsDebugWnd::DebugWndThread::WndProc( HWND hWnd, UINT msg, WPARAM wP, LPARAM lP)
{
	switch(msg){
		case WM_SIZE:
			{
				RECT rc;
				GetClientRect( hWnd, &rc );
				rc.bottom -= 20;

				// ウィンドウのフレームサイズを取得する
				int xf = GetSystemMetrics( SM_CXSIZEFRAME );
				int yf = GetSystemMetrics( SM_CYSIZEFRAME );

				int w = rc.right/((int)(m_ScrollWndNum/2.0f+0.5f)+1);
				MoveWindow( m_hEditWnd[0], 0, 0, w, rc.bottom, TRUE );

				int h;
				for(int i=1;i<m_ScrollWndNum+1;i++){
					h = rc.bottom/2;
					if(i == m_ScrollWndNum && m_ScrollWndNum%2 == 1){
						h = rc.bottom;
					}
					MoveWindow(m_hEditWnd[i], w+w*((i-1)/2), ((i-1)%2)*rc.bottom/2, w, h, TRUE );
				}


				MoveWindow(m_hEditCommand, 0, rc.bottom, rc.right, 20, TRUE );
			}
			break;
		case WM_CLOSE:
			OutputDebugString("****************** Close ******************\n");
//			m_th.detach();
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
			/*
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_EDIT_COMMAND){
			}
			break;
			*/
		default:
			return ( DefWindowProc(hWnd,msg,wP,lP) );
	}
	return 0;
}

void YsDebugWnd::DebugWndThread::Begin(){
	m_th = std::thread(
		[&]() {

			WNDCLASS			wc;
			MSG					msg;

			// ウインドウクラスの設定
			wc.lpszClassName = "DebugWindow";
			wc.hInstance = DebugWnd.m_hInst;
			wc.lpfnWndProc = DebugWndProc;
			wc.hCursor = LoadCursor((HINSTANCE)nullptr, IDC_ARROW);
			wc.hIcon = LoadIcon((HINSTANCE)nullptr, IDI_APPLICATION);
			wc.lpszMenuName = nullptr;
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;

			if(RegisterClass(&wc) == 0){
				MessageBox(nullptr, "RegisterClass失敗", "DebugWnd", MB_OK);
				return;
			}

			int w = 200 + m_ScrollWndNum * 100;
			int h = 600;
			int x = (GetSystemMetrics(SM_CXMAXIMIZED) - w) - 10;
			int y = (GetSystemMetrics(SM_CYMAXIMIZED) - h) / 2;

			// ウインドウの生成
			m_hWnd = CreateWindowEx(0,
				"DebugWindow",
				"デバッグウィンドウ",
				WS_OVERLAPPEDWINDOW,
				x,
				y,
				w,
				h,
				nullptr,
				nullptr,
				DebugWnd.m_hInst,
				nullptr);
			// フォント作成
			HFONT hFont = CreateFont(12,		//フォント高さ
				0,						//文字幅
				0,					//テキストの角度
				0,						//ベースラインとｘ軸との角度
				FW_NORMAL,				//フォントの重さ（太さ）
				FALSE,					//イタリック体
				FALSE,					//アンダーライン
				FALSE,					//打ち消し線
				SHIFTJIS_CHARSET,		//文字セット
				OUT_DEFAULT_PRECIS,		//出力精度
				CLIP_DEFAULT_PRECIS,	//クリッピング精度
				PROOF_QUALITY,			//出力品質
				FIXED_PITCH | FF_DONTCARE,//ピッチとファミリー
				"ＭＳ ゴシック");				//書体名

			// エディットボックス作成
			m_hEditWnd[0] = CreateWindow("EDIT",      //エジットコントロール
				"",	//最初に表示される文字列
				WS_CHILD | WS_VISIBLE |
				ES_WANTRETURN | ES_MULTILINE |
				ES_AUTOHSCROLL | ES_READONLY | WS_HSCROLL | WS_BORDER,
				0, 0,//表示位置
				w / 2, h / 2,//幅、高さ
				m_hWnd,//親ウィンドウのハンドル
				(HMENU)IDC_EDIT1,//エジットコントロールのＩＤ
				DebugWnd.m_hInst,//インスタンス・ハンドル
				nullptr);//通常ＮＵＬＬです
			SendMessage(m_hEditWnd[0], WM_SETFONT, (WPARAM)hFont, 0);


			for(int i = 0; i<m_ScrollWndNum; i++){
				m_hEditWnd[i + 1] = CreateWindow("EDIT",      //エジットコントロール
					"",	//最初に表示される文字列
					WS_CHILD | WS_VISIBLE |
					ES_WANTRETURN | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL |
					ES_AUTOHSCROLL | ES_READONLY | WS_HSCROLL | WS_BORDER,
					0, 0,//表示位置
					100, 100,//幅、高さ
					m_hWnd,//親ウィンドウのハンドル
					(HMENU)IDC_SCROLLEDIT1 + i,//エジットコントロールのＩＤ
					DebugWnd.m_hInst,//インスタンス・ハンドル
					nullptr);//通常ＮＵＬＬです
				SendMessage(m_hEditWnd[i + 1], WM_SETFONT, (WPARAM)hFont, 0);
			}

			m_hEditCommand = CreateWindow("EDIT",      //エジットコントロール
				"",	//最初に表示される文字列
				//		WS_CHILD | WS_VISIBLE | ES_WANTRETURN,
				ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 0,//表示位置
				w, 20,//幅、高さ
				m_hWnd,//親ウィンドウのハンドル
				(HMENU)IDC_EDIT_COMMAND,//エジットコントロールのＩＤ
				DebugWnd.m_hInst,//インスタンス・ハンドル
				nullptr);//通常ＮＵＬＬです
			SendMessage(m_hEditCommand, WM_SETFONT, (WPARAM)hFont, 0);
			// サブクラス化
			g_EditCommandOldProc = (WNDPROC)GetWindowLong(m_hEditCommand, GWL_WNDPROC);
			SetWindowLong(m_hEditCommand, GWL_WNDPROC, (LONG)EditCommandProc);

			// 
			ShowWindow(m_hWnd, SW_SHOW);
			UpdateWindow(m_hWnd);

			DebugWnd.setStatus(ST_OPEN);

			while(1){
				if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
					// メッセージ処理
					if(msg.message == WM_QUIT){
						break;
					}
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else{
					// 処理
					bool bStaticUpdate = false;
					DebugQ data;
					while(1){
						if(m_bEndFlag){
							PostMessage(m_hWnd, WM_CLOSE, 0, 0);
						}


						m_mtx.lock();
						{
							if(m_MsgQ.empty()){
								m_mtx.unlock();
								break;
							}
							data = m_MsgQ.front();
							m_MsgQ.pop();
						}
						m_mtx.unlock();

						if(data.m_Flag == DebugQ::FLAG_STATIC){
							m_StaticMsg[data.m_Idx] = data.m_Msg;
							bStaticUpdate = true;
						}
						else if(data.m_Flag == DebugQ::FLAG_SCROLL){
							int No = data.m_Idx;
							int lidx = (int)SendMessage(m_hEditWnd[No], EM_GETLINECOUNT, 0, 0);
							// 300行固定
							if(lidx > 300){
								int idx1 = (int)SendMessage(m_hEditWnd[No], EM_LINEINDEX, 0, 0);
								int idx2 = (int)SendMessage(m_hEditWnd[No], EM_LINEINDEX, 1, 0);
								SendMessage(m_hEditWnd[No], EM_SETSEL, 0, idx2 - idx1);
								SendMessage(m_hEditWnd[No], EM_REPLACESEL, 0, (LPARAM)"");
							}

							lidx = (int)SendMessage(m_hEditWnd[No], EM_GETLINECOUNT, 0, 0);
							int idx = (int)SendMessage(m_hEditWnd[No], EM_LINEINDEX, lidx - 1, 0);
							if(idx >= 0){
								int pos = (int)SendMessage(m_hEditWnd[No], EM_LINELENGTH, idx, 0);

								SendMessage(m_hEditWnd[No], EM_SETSEL, idx + pos, idx + pos);
								SendMessage(m_hEditWnd[No], EM_REPLACESEL, 0, (LPARAM)data.m_Msg.c_str());
							}
						}
						else if(data.m_Flag == DebugQ::FLAG_CLEAR_SCROLL){
							SetWindowText(m_hEditWnd[data.m_Idx], "");
						}

					}

					if(bStaticUpdate){
						std::string str;
						for(int i = 0; i<DebugWnd.MAX_STATICMSG; i++){
							str += std::to_string(i) + ":";
							str += m_StaticMsg[i] + "\r\n";
						}

						SetWindowText(m_hEditWnd[0], str.c_str());
					}

					Sleep(50);
				}
			}

			UnregisterClass("DebugWindow", DebugWnd.m_hInst);
			OutputDebugString("===== デバッグウィンドウ終了 =====\n");
			DebugWnd.delStatus(ST_OPEN);

			DeleteObject(hFont);

			// 初期化
			m_hWnd = nullptr;
			ZeroMemory(m_hEditWnd, sizeof(m_hEditWnd));
			for(int i = 0; i<MAX_STATICMSG; i++){
				m_StaticMsg[i].clear();
			}
			m_Status = 0;
		}
	);
}

#endif