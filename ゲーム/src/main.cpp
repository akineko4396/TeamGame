//======================================================================================
//
// ゲーム用基本フレーム
//
// [ビルド構成]
//  Debug    … デバッグビルド(実行速度犠牲の超デバッグ優先)
//              <特徴> ・Direct3DのデバッグON。やってはダメなことを「出力」ウィンドウに表示して教えてくれる。ただし描画が倍くらい重くなる。
//                     ・シェーダコンパイル時に最適化フラグをOFFにしているので、ビルドから実行までが素早くできる。
//                     ・とにかく処理と描画が遅いので、ほんとにバグを見つけるデバッグ用。
//
//  Debug2   … 半リリース + 半デバッグ(実行速度とデバッグ機能の両立)
//              <特徴> ・Direct3DのデバッグOFF。描画が高速。
//                     ・シェーダコンパイル時に最適化フラグをOFFにしているので、ビルドから実行までが素早くできる。
//                     ・_DEBUGマクロは入るので、STL使用時のバグとかは見つけやすくなります。そのぶん実行速度はかなり落ちる。
//                     　※実行速度を速くしたい場合は、Pch.hの #undef _DEBUG のコメントを外す。
//                     ・ブレークポイントでステップ実行可能。
//                     ・Debugビルドほど細かなデバッグはできない。
//
//  Release  … リリースビルド(最終リリース向きの超実行速度優先)
//              <特徴> ・Direct3DのデバッグOFF。描画が高速。
//                     ・シェーダコンパイルの最適化を最大にしているので描画パフォーマンスは上がるが、実行するまでが遅い。
//                     　※この場合、シェーダはあらかじめコンパイルしてファイルにしておくべき。
//                     ・「実行速度優先」のビルド設定にしているので、最大のパフォーマンスが期待できる。
//
// ※Debug2やReleaseにもDirect3DのデバッグをONにしたい場合は、Pch.hの #define DEBUG_DIRECT3D11 のコメントをはずす。
// ※デバッグウィンドウを消したい場合は、Pch.hの #define DEBUG_WND をコメントアウトする。
//======================================================================================
#include "main.h"
#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game\Scene\GameScene\GameScene.h"

static void DebugCommand(std::string cmdText);

//======================================================================================
//メイン
//======================================================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpszArgs,int nWinMode)
{
	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// 初期設定
	BOOL bRet = APP.InitWindow(hInstance, 1280, 720);

	// ループ
	if(bRet){
		APP.Loop();
	}

	// 解放
	APP.Release();

	// COM解放
	CoUninitialize();

	return 0;
}

//======================================================================================
// 初期設定
//======================================================================================
BOOL CApp::InitWindow(HINSTANCE hInstance,int w,int h)
{
	//===============================
	// デバッグウィンドウ作成
	//===============================
	#ifdef DEBUG_WND
	DebugWnd.Create(hInstance, 4, DebugCommand);
	#endif

	Dw_Scroll(0, "ウィンドウ作成");
	//===============================
	// メインウィンドウ作成
	//===============================
	//ウィンドウクラスの定義
	WNDCLASSEX wc;											// ウィンドウクラスの定義用
	wc.cbSize=sizeof(WNDCLASSEX);							// 構造体のサイズ
	wc.style = 0;											// スタイル
	wc.lpfnWndProc = &CApp::callWindowProc;					// ウインドウ関数
	wc.cbClsExtra = 0;										// エキストラクラス情報 
	wc.cbWndExtra = 0;										// エキストラウィンドウ情報
	wc.hInstance = m_hInst;									// インスタンスハンドル
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);		// ラージアイコン
	wc.hIconSm=LoadIcon(hInstance, IDI_APPLICATION);		// スモールアイコン 
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);				// マウスカーソル
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 背景色 
	wc.lpszMenuName = nullptr;								// メインメニュー名
	wc.lpszClassName = "Window";							// ウィンドウクラス名

	//ウィンドウクラスの登録
	if(!RegisterClassEx(&wc)){
		return FALSE;
	}

	//ウィンドウの作成
	m_hWnd = CreateWindow(
		"Window",											//作成するウィンドウ名
		"Game",									//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW-WS_MAXIMIZEBOX-WS_THICKFRAME,	//ウィンドウタイプ	
		0,													//ウィンドウの位置（Ｘ座標）
		0,													//ウィンドウの位置（Ｙ座標）						
		w,													//ウィンドウの幅
		h,													//ウィンドウの高さ			
		nullptr,											//親ウィンドウのハンドル
		nullptr,											//メニューのハンドル
		m_hInst,											//インスタンスハンドル 
		nullptr);											//追加情報 

	// クライアントのサイズを設定
	SetClientSize(m_hWnd,w,h);

	// timeGetTime関数の精度を1msにする
	timeBeginPeriod(1);

	//===================================================================
	// Direct3D初期化
	//===================================================================
	Dw_Scroll(0, "Direct3D初期化開始");
	std::string errMsg;
	if( !YsDx.Init(m_hWnd, w, h, &m_ResStg, YsDx.MSAA_None, &errMsg) ){
		MessageBox(m_hWnd, errMsg.c_str(), "エラー", MB_OK);
		return FALSE;
	}

	// シェーダクラス初期化
	Dw_Scroll(0, "シェーダ準備開始");
	ShMgr.Init();

	//===================================================================
	// サウンド
	//===================================================================
	Dw_Scroll(0, "サウンド初期化開始");
	YsSndMgr.Init(m_hWnd);



	// デバッグウィンドウよりも手前に表示
	#ifdef DEBUG_WND
	SetForegroundWindow(m_hWnd);
	#endif

	// 乱数初期化
	m_Rnd.InitSeed(timeGetTime());

	// フルスクリーン
	//if(IDYES == MessageBox(m_hWnd, "フルスクリーン表示にしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)) {
	//	ShowWindow(m_hWnd, SW_SHOW);
	//	YsDx.SetFullScreen(TRUE);
	//}
	//else{
		ShowWindow(m_hWnd, SW_SHOW);
	//}


	// 初期シーン
	{
		SPtr<TitleScene> ts(new TitleScene());
		ts->Init();
		m_NowScene = ts;
	}

	// 
	Dw_Scroll(1, "==========================");
	Dw_Scroll(1, "helpでデバッグコマンド表示");
	Dw_Scroll(1, "==========================");
	DebugCommand("help");

	return TRUE;
}

void CApp::Release()
{
	// シーン解放
	m_NowScene = nullptr;

	// リソース解放
	m_ResStg.Release();
	// シェーダ解放
	ShMgr.Release();
	// Direct3D解放
	YsDx.Release();
	// サウンド解放
	YsSndMgr.Release();

	// デバッグウィンドウ終了
#ifdef DEBUG_WND
	DebugWnd.Close();
#endif

}

// メインループ
int CApp::Loop()
{
	MSG msg;			//メッセージ構造体

	//メッセージループ
	while(1){
		// メッセージ取得
		if(PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
		{
			// 終了メッセージがきたら、ループ終了
			if(msg.message == WM_QUIT) {
				break;
			}

			//=========================================
			// メッセージ処理
			//=========================================
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ゲーム処理
		else
		{
			//=========================================
			// FPS制御
			//=========================================
			// 待つ
			m_FpsCtrl.Wait();
			// FPS用の時間更新
			m_FpsCtrl.Update();
			// FPSデバッグ表示
			Dw_Static(0, "FPS:%f", m_FpsCtrl.GetNowFps());

			//=========================================
			// キー・マウス情報更新
			//=========================================
			INPUT.UpdateKey();
			INPUT.UpdateMouseData(m_hWnd);

			//=========================================
			// サウンド処理
			//=========================================
			YsSndMgr.Run();

			//=========================================
			//
			// シーン処理実行
			//
			//=========================================
			{
				//--------------------------------------------
				// 処理軽減のため、描画が30fpsで実行(処理は60fpsのまま)
				//--------------------------------------------
				m_NonDrawFlag = 0;
				if (m_b60fpsDraw == false) {
					// 2回に一回だけ描画する
					if (m_FrameCnt % 2 == 0) {
						if (m_NonDrawFlag == 0) {
							m_NonDrawFlag = 1;
						}
					}
				}

				//--------------------------------------------
				// シーン更新処理
				//--------------------------------------------
				int ret = m_NowScene->Update();

				//--------------------------------------------
				// シーン描画処理
				//--------------------------------------------
				if(ret == 0 && m_NonDrawFlag == 0) {
					// シーン描画実行
					m_NowScene->Draw();

					//--------------------------------------------
					// 画面に表示
					//--------------------------------------------
					YsDx.Present(0, 0);
				}

			}

			//=========================================
			// デバッグウィンドウ
			//=========================================
			#ifdef DEBUG_WND
			DebugWnd.Update();
			#endif

			Dw_Static(40, "Load済みリソース数:%d", YsDx.GetResStg()->GetNodeNum());

			// 
			INPUT.SetMouseWheelValue(0);

			m_FrameCnt++;
		}

	}

	// プログラム終了
	return 0;
}

//ウィンドウ関数
LRESULT CALLBACK CApp::callWindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	return APP.WindowProc(hWnd,message,wParam,lParam);
}

LRESULT CApp::WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	//メッセージによって処理を選択
	switch(message){
		case WM_ACTIVATE:
			{
				// 非アクティブ
				if(LOWORD(wParam) == WA_INACTIVE){
					m_bWndActive = false;
				}
				// アクティブ
				else{
					m_bWndActive = true;
				}
			}
			break;
		// ホイールスクロール時
		case WM_MOUSEWHEEL:
			{
				INPUT.SetMouseWheelValue((short)HIWORD(wParam));
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			//不要なメッセージの後始末
			return DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 0;
}

// ウィンドウのクライアントのサイズを指定サイズにする。
void CApp::SetClientSize(HWND hWnd,int w,int h)
{
	RECT rcWnd,rcCli;

	GetWindowRect(hWnd,&rcWnd); // ウィンドウのRECT取得
	GetClientRect(hWnd,&rcCli); // クライアント領域のRECT取得

	// ウィンドウの余白を考えて、クライアントのサイズを指定サイズにする。
	MoveWindow(hWnd,
			rcWnd.left,// X座標
			rcWnd.top,// Y座標
			w + (rcWnd.right-rcWnd.left)-(rcCli.right-rcCli.left),
			h + (rcWnd.bottom-rcWnd.top)-(rcCli.bottom-rcCli.top),
			TRUE);
}

#ifdef DEBUG_WND

// デバッグウィンドウ用 コマンド入力
//  デバッグウィンドウ下にある入力ボックスで入力された内容がここに来る
//  デバッグ専用の処理などに利用してください
static void DebugCommand(std::string cmdText)
{
	// 例)exitと入力してEnterを押すと終了
	if(cmdText == "exit"){
		APP.ExitGame();
	}
	else if(cmdText == "reload shader"){
		ShMgr.Release();
		ShMgr.Init();
		Dw_Scroll(1, "シェーダ再読込 完了");
	}
	else if(cmdText == "save shader"){
		ShMgr.SaveCsoToFile();
		MessageBox(APP.m_hWnd, "hlslファイルと同じ場所にcsoファイルを保存しました", "シェーダ保存", MB_OK);
		MessageBox(APP.m_hWnd, "※注意※\ncsoが存在する場合、hlslファイルは読み込まれなくなり\ncsoが読み込まれるようになります。\n再度hlslを読み込みたい場合はcsoファイルを削除してください", "シェーダ保存", MB_OK);
	}
	else if(cmdText == "resource"){
		Dw_Scroll(1, "<<読込済みリソース一覧>>");
		std::vector<std::string> res;
		APP.m_ResStg.DEBUG_GetNowResources(res);
		for(auto& str : res){
			Dw_Scroll(1, "%s", str.c_str());
		}
	}
	else if (cmdText.compare(0, 14, "change maxfps ") == 0) {
		char tmp[1024];
		char tmp2[1024];
		int fps;
		sscanf(cmdText.c_str(), "%s %s %d", tmp, tmp2, &fps);

		if (fps > 0) {
			APP.m_FpsCtrl.SetMaxFps(fps);
			Dw_Scroll(1, "FPS変更(Max Fps = %d)", fps);
		}
		else {
			Dw_Scroll(1, "無効なFPS値です");
		}
	}
	else if(cmdText == "fps limit on"){
		Dw_Scroll(1, "FPS制御ON");
		APP.m_FpsCtrl.SetEnableWait( true );
	}
	else if(cmdText == "fps limit off"){
		Dw_Scroll(1, "FPS制御OFF");
		APP.m_FpsCtrl.SetEnableWait( false );
	}
	else if(cmdText == "draw 60fps"){
		Dw_Scroll(1, "60FPS描画モード");
		APP.m_b60fpsDraw = true;
	}
	else if(cmdText == "draw 30fps"){
		Dw_Scroll(1, "30FPS描画モード");
		APP.m_b60fpsDraw = false;
	}
	else if(cmdText == "help"){
		Dw_Scroll(1, "<<コマンド一覧>>");
		Dw_Scroll(1, "help … この説明文を表示");
		Dw_Scroll(1, "exit … 終了");
		Dw_Scroll(1, "reload shader … シェーダ再読込");
		Dw_Scroll(1, "save shader … コンパイル済みのシェーダ(cso)をファイルに保存");
		Dw_Scroll(1, "resource … 読込済みリソース一覧");
		Dw_Scroll(1, "change maxfps 数値 … 最大FPS値を変更");
		Dw_Scroll(1, "fps limit on … FPS制御ON");
		Dw_Scroll(1, "fps limit off … FPS制御OFF");
		Dw_Scroll(1, "draw 60fps … 60fps描画モード");
		Dw_Scroll(1, "draw 30fps … 30fps描画モード");
	}
}


#endif
