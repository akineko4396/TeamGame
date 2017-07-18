#ifndef __YsDebugWnd_h__
#define __YsDebugWnd_h__

#include <queue>

//#ifdef _DEBUG
//#define DEBUG_WND
//#endif

#pragma warning(disable:4996)

namespace YskLib
{
#ifdef DEBUG_WND

//===========================================================
//
// 別プロセスにデバッグ表示用ウィンドウを別スレッドで作成する
//	デバッグコンパイル時しか表示されません
//	Dw_Static関数,Dw_Scroll関数で文字列を追加すること
//	(Releaseコンパイル時には、全て省略される為)
//
//===========================================================
class YsDebugWnd{
private:
	// Scrollエディット数
	enum{MAX_SCROLLWINDOW = 6};
	// Static領域
	enum{MAX_STATICMSG=100};

	class DebugWndThread;

public:

	// スレッド取得
	DebugWndThread& getThread(){return m_Thread;}

	// スレッドの状態を取得
	DWORD getStatus(){
		m_Thread.LockMutex();
		DWORD flag = m_Thread.m_Status;
		m_Thread.UnlockMutex();
		return flag;
	}
	// スレッドの状態を判定
	bool CheckStatus(DWORD flag){
		if(getStatus() & flag)return true;
		return false;
	}

	// 作成
	bool Create(HINSTANCE hInst,int ScrollWndNum, void(*cmdProc)(std::string))
	{
		if(CheckStatus(DebugWndThread::ST_OPEN))return false;	// すでに作成済み

		m_hInst = hInst;
		m_CmdProc = cmdProc;

		m_Thread.SetScrollWndNum(ScrollWndNum);
		m_Thread.Begin();

		// とりあえずポーリングで作成できるまで待つ
		int timeOut = 0;
		while(!CheckStatus(DebugWndThread::ST_OPEN)){
			Sleep(1);
			// タイムアウト
			timeOut++;
			if(timeOut > 1000){
				OutputDebugString("***** デバッグウィンドウ作成失敗 *****\n");
				return false;
			}
		}
		OutputDebugString("===== デバッグウィンドウ作成 =====\n");
		return true;
	}
	// 終了
	void Close(){
		m_Thread.End();

		if(!CheckStatus(DebugWndThread::ST_OPEN))return;		// 作成してないなら、なにもしない


		m_hInst = nullptr;
	}

	void Update(){
		m_Thread.LockMutex();
		std::queue<std::string> tmpQ = m_CmdQueue;
		m_CmdQueue = std::queue<std::string>();
		m_Thread.UnlockMutex();

		while(!tmpQ.empty()){
			if(m_CmdProc){
				m_CmdProc(tmpQ.front());
			}
			tmpQ.pop();
		}
	}

	void AddCommandQueue(const std::string& str){
		m_Thread.LockMutex();
		m_CmdQueue.push(str);
		m_Thread.UnlockMutex();
	}

	// スタティック領域に文字列を設定
	void SetStaticMsg(int idx,const char *format, ...)
	{
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// 作成してないなら、なにもしない
		if(idx >= MAX_STATICMSG)return;

		m_Thread.LockMutex();
		va_list argptr;
		va_start(argptr, format);
		vsprintf(m_TmpStr,format, argptr);
		va_end(argptr);

		DebugQ Qnode;
		Qnode.m_Flag	= DebugQ::FLAG_STATIC;
		Qnode.m_Idx	= idx;
		Qnode.m_Msg	= m_TmpStr;
		m_Thread.AddMsgQ(Qnode);

		m_Thread.UnlockMutex();
	}

	// スクロール領域に文字列を設定
	void SetScrollMsg(int No,const char *format, ...)
	{
		if(No < 0 && No >= m_Thread.GetScrollWndNum())return;
		No++;
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// 作成してないなら、なにもしない

		m_Thread.LockMutex();
		va_list argptr;
		va_start(argptr, format);
		vsprintf(m_TmpStr,format, argptr);
		strcat(m_TmpStr,"\r\n");
		va_end(argptr);

		DebugQ Qnode;
		Qnode.m_Flag	= DebugQ::FLAG_SCROLL;
		Qnode.m_Idx	= No;
		Qnode.m_Msg	= m_TmpStr;
		m_Thread.AddMsgQ(Qnode);

		m_Thread.UnlockMutex();
	}

	// スタティック領域クリア
	void ClearStaticMsg()
	{
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// 作成してないなら、なにもしない

		m_Thread.LockMutex();

		for(int i=0;i<MAX_STATICMSG;i++){
			DebugQ Qnode;
			Qnode.m_Flag	= DebugQ::FLAG_STATIC;
			Qnode.m_Idx	= i;
			Qnode.m_Msg	= "";
			m_Thread.AddMsgQ(Qnode);
		}

		m_Thread.UnlockMutex();
	}

	// スクロール領域クリア
	void ClearScrollMsg(int No)
	{
		if(No < 0 && No >= m_Thread.GetScrollWndNum())return;
		No++;
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// 作成してないなら、なにもしない

		m_Thread.LockMutex();

		DebugQ Qnode;
		Qnode.m_Flag	= DebugQ::FLAG_CLEAR_SCROLL;
		Qnode.m_Idx	= No;
		m_Thread.AddMsgQ(Qnode);

		m_Thread.UnlockMutex();
	}

	// 時間計測用
	void TimeBegin(const std::string& pStr){
		m_TimeMutex.lock();

		std::map<std::string,TimeData>::iterator p = m_TimeMap.find(pStr);
		if(p != m_TimeMap.end()){	// 存在
			p->second.st = timeGetTime();	// 更新
		}
		else{// 無い
			TimeData time;
			time.st = timeGetTime();
			m_TimeMap.insert(std::pair<std::string,TimeData>(pStr,time));
		}

		m_TimeMutex.unlock();
	}
	DWORD TimeEnd(const std::string& pStr,int StaticLineNo){
		DWORD et = timeGetTime();
		DWORD st;
		std::string str;

		m_TimeMutex.lock();

		bool bFind = false;
		std::map<std::string,TimeData>::iterator p = m_TimeMap.find(pStr);
		if(p != m_TimeMap.end()){	// 存在
			p->second.et = et;	// 更新
			st = p->second.st;
			str = p->first;
			m_TimeMap.erase(p);	// 消去
			bFind = true;
		}
		m_TimeMutex.unlock();

		// スタティックに表示
		if (bFind) {	// 存在
			if (StaticLineNo >= 0) {
				SetStaticMsg(StaticLineNo, "%s:%03d min", str.c_str(), et - st);
			}
		}

		return et-st;
	}

private:
	HINSTANCE		m_hInst;

	std::queue<std::string>	m_CmdQueue;
	std::function<void(std::string)>	m_CmdProc;

	// 時間計測用
	class TimeData {
	public:
		DWORD st, et;
	};
	std::map<std::string, TimeData>	m_TimeMap;
	std::mutex						m_TimeMutex;

	char m_TmpStr[4000];			// 作業用

	void setStatus(DWORD flag){
		m_Thread.LockMutex();
		m_Thread.m_Status = flag;
		m_Thread.UnlockMutex();
	}
	void addStatus(DWORD flag){
		m_Thread.LockMutex();
		m_Thread.m_Status |= flag;
		m_Thread.UnlockMutex();
	}
	void delStatus(DWORD flag){
		m_Thread.LockMutex();
		m_Thread.m_Status &= ~flag;
		m_Thread.UnlockMutex();
	}

	// ノードクラス
	class DebugQ {
	public:
		enum FLAGS {
			FLAG_STATIC = 1,
			FLAG_SCROLL = 2,
			FLAG_CLEAR_SCROLL = 3,
		};
		FLAGS		m_Flag;
		int			m_Idx;
		std::string	m_Msg;
	};

	//=======================================================
	// デバッグウィンドウスレッドクラス
	//=======================================================
	class DebugWndThread {
	public:
		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP);
		// 開始
		void Begin();
		// 終了
		void End(){
			m_bEndFlag = true;
			m_th.join();
		}

		void LockMutex(){
			m_mtx.lock();
		}
		void UnlockMutex(){
			m_mtx.unlock();
		}

		void SetScrollWndNum(int num){
			m_ScrollWndNum = num;
			if(m_ScrollWndNum > MAX_SCROLLWINDOW)m_ScrollWndNum = MAX_SCROLLWINDOW;
		}
		int GetScrollWndNum(){ return m_ScrollWndNum; }

		void AddMsgQ(DebugQ& data){
			m_MsgQ.push(data);
		}

		// Status
		enum STATUS {
			ST_OPEN = 1,		// 表示中
		};
		DWORD			m_Status;

		DebugWndThread() :
			m_hWnd(0),
			m_Status(0),
			m_ScrollWndNum(0),
			m_bEndFlag(false)
		{
			ZeroMemory(m_hEditWnd, sizeof(m_hEditWnd));
			m_hEditCommand = nullptr;
		}

	private:
		bool			m_bEndFlag;

		HWND			m_hWnd;
		HWND			m_hEditWnd[MAX_SCROLLWINDOW + 1];
		HWND			m_hEditCommand;
		std::string		m_StaticMsg[MAX_STATICMSG];
		int				m_ScrollWndNum;
		std::queue<DebugQ> m_MsgQ;

		std::mutex		m_mtx;	// 同期オブジェクト
		std::thread		m_th;
	};

	DebugWndThread	m_Thread;

	// シングルトン
private:
	YsDebugWnd() :
		m_hInst(0)
	{
	}
public:
	static YsDebugWnd& getInstance()
	{
		static YsDebugWnd Instance;
		return Instance;
	}

};

#define DebugWnd YsDebugWnd::getInstance()
#endif

#ifdef DEBUG_WND
// デバッグコンパイル時
#define Dw_Static DebugWnd.SetStaticMsg					// スタティック領域に文字列追加
#define Dw_Scroll DebugWnd.SetScrollMsg					// スクロール領域に文字列を追加
#define Dw_StaticClear DebugWnd.ClearStaticMsg			// スタティック文字列をクリア
#define Dw_ScrollClear DebugWnd.ClearScrollMsg			// スクロール文字列をクリア
#define Dw_TimeBegin DebugWnd.TimeBegin					// 時間計測開始
#define Dw_TimeEnd DebugWnd.TimeEnd						// 時間計測終了
#else
// リリースコンパイル時
#define Dw_Static(x)
#define Dw_Scroll(x)
#define Dw_StaticClear(x)
#define Dw_ScrollClear(x)
#define Dw_TimeBegin(x)
#define Dw_TimeEnd(x)

#endif

}
#endif