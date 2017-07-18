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
// �ʃv���Z�X�Ƀf�o�b�O�\���p�E�B���h�E��ʃX���b�h�ō쐬����
//	�f�o�b�O�R���p�C���������\������܂���
//	Dw_Static�֐�,Dw_Scroll�֐��ŕ������ǉ����邱��
//	(Release�R���p�C�����ɂ́A�S�ďȗ�������)
//
//===========================================================
class YsDebugWnd{
private:
	// Scroll�G�f�B�b�g��
	enum{MAX_SCROLLWINDOW = 6};
	// Static�̈�
	enum{MAX_STATICMSG=100};

	class DebugWndThread;

public:

	// �X���b�h�擾
	DebugWndThread& getThread(){return m_Thread;}

	// �X���b�h�̏�Ԃ��擾
	DWORD getStatus(){
		m_Thread.LockMutex();
		DWORD flag = m_Thread.m_Status;
		m_Thread.UnlockMutex();
		return flag;
	}
	// �X���b�h�̏�Ԃ𔻒�
	bool CheckStatus(DWORD flag){
		if(getStatus() & flag)return true;
		return false;
	}

	// �쐬
	bool Create(HINSTANCE hInst,int ScrollWndNum, void(*cmdProc)(std::string))
	{
		if(CheckStatus(DebugWndThread::ST_OPEN))return false;	// ���łɍ쐬�ς�

		m_hInst = hInst;
		m_CmdProc = cmdProc;

		m_Thread.SetScrollWndNum(ScrollWndNum);
		m_Thread.Begin();

		// �Ƃ肠�����|�[�����O�ō쐬�ł���܂ő҂�
		int timeOut = 0;
		while(!CheckStatus(DebugWndThread::ST_OPEN)){
			Sleep(1);
			// �^�C���A�E�g
			timeOut++;
			if(timeOut > 1000){
				OutputDebugString("***** �f�o�b�O�E�B���h�E�쐬���s *****\n");
				return false;
			}
		}
		OutputDebugString("===== �f�o�b�O�E�B���h�E�쐬 =====\n");
		return true;
	}
	// �I��
	void Close(){
		m_Thread.End();

		if(!CheckStatus(DebugWndThread::ST_OPEN))return;		// �쐬���ĂȂ��Ȃ�A�Ȃɂ����Ȃ�


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

	// �X�^�e�B�b�N�̈�ɕ������ݒ�
	void SetStaticMsg(int idx,const char *format, ...)
	{
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// �쐬���ĂȂ��Ȃ�A�Ȃɂ����Ȃ�
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

	// �X�N���[���̈�ɕ������ݒ�
	void SetScrollMsg(int No,const char *format, ...)
	{
		if(No < 0 && No >= m_Thread.GetScrollWndNum())return;
		No++;
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// �쐬���ĂȂ��Ȃ�A�Ȃɂ����Ȃ�

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

	// �X�^�e�B�b�N�̈�N���A
	void ClearStaticMsg()
	{
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// �쐬���ĂȂ��Ȃ�A�Ȃɂ����Ȃ�

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

	// �X�N���[���̈�N���A
	void ClearScrollMsg(int No)
	{
		if(No < 0 && No >= m_Thread.GetScrollWndNum())return;
		No++;
		if(!CheckStatus(DebugWndThread::ST_OPEN))return;	// �쐬���ĂȂ��Ȃ�A�Ȃɂ����Ȃ�

		m_Thread.LockMutex();

		DebugQ Qnode;
		Qnode.m_Flag	= DebugQ::FLAG_CLEAR_SCROLL;
		Qnode.m_Idx	= No;
		m_Thread.AddMsgQ(Qnode);

		m_Thread.UnlockMutex();
	}

	// ���Ԍv���p
	void TimeBegin(const std::string& pStr){
		m_TimeMutex.lock();

		std::map<std::string,TimeData>::iterator p = m_TimeMap.find(pStr);
		if(p != m_TimeMap.end()){	// ����
			p->second.st = timeGetTime();	// �X�V
		}
		else{// ����
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
		if(p != m_TimeMap.end()){	// ����
			p->second.et = et;	// �X�V
			st = p->second.st;
			str = p->first;
			m_TimeMap.erase(p);	// ����
			bFind = true;
		}
		m_TimeMutex.unlock();

		// �X�^�e�B�b�N�ɕ\��
		if (bFind) {	// ����
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

	// ���Ԍv���p
	class TimeData {
	public:
		DWORD st, et;
	};
	std::map<std::string, TimeData>	m_TimeMap;
	std::mutex						m_TimeMutex;

	char m_TmpStr[4000];			// ��Ɨp

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

	// �m�[�h�N���X
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
	// �f�o�b�O�E�B���h�E�X���b�h�N���X
	//=======================================================
	class DebugWndThread {
	public:
		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP);
		// �J�n
		void Begin();
		// �I��
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
			ST_OPEN = 1,		// �\����
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

		std::mutex		m_mtx;	// �����I�u�W�F�N�g
		std::thread		m_th;
	};

	DebugWndThread	m_Thread;

	// �V���O���g��
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
// �f�o�b�O�R���p�C����
#define Dw_Static DebugWnd.SetStaticMsg					// �X�^�e�B�b�N�̈�ɕ�����ǉ�
#define Dw_Scroll DebugWnd.SetScrollMsg					// �X�N���[���̈�ɕ������ǉ�
#define Dw_StaticClear DebugWnd.ClearStaticMsg			// �X�^�e�B�b�N��������N���A
#define Dw_ScrollClear DebugWnd.ClearScrollMsg			// �X�N���[����������N���A
#define Dw_TimeBegin DebugWnd.TimeBegin					// ���Ԍv���J�n
#define Dw_TimeEnd DebugWnd.TimeEnd						// ���Ԍv���I��
#else
// �����[�X�R���p�C����
#define Dw_Static(x)
#define Dw_Scroll(x)
#define Dw_StaticClear(x)
#define Dw_ScrollClear(x)
#define Dw_TimeBegin(x)
#define Dw_TimeEnd(x)

#endif

}
#endif