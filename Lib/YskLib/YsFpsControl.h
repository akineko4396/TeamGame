#ifndef __YsFpsControl_h__
#define __YsFpsControl_h__

namespace YskLib {

//====================================================
//! @brief FPS����N���X
//!
//! @ingroup Etc
//====================================================
class YsFpsControl {
public:

	//! @brief ���݂�FPS���擾
	float GetNowFps() { return m_NowFps; }

	//! @brief �ݒ肳��Ă���ő�FPS�l���擾
	DWORD GetMaxFps() { return m_MaxFPS; }

	//! @brief �ő�FPS�l��ݒ�
	void SetMaxFps(DWORD fps) {
		m_MaxFPS = fps;
		Reset();
	}

	//! @brief �x���������Ԃ�Ԃ�
	int GetLagTime() { return m_LagTime; }

	//! @brief FPS����҂���L��/�����ɂ���
	void SetEnableWait(bool enable) { m_EnableWait = enable; }
	//! @brief FPS����҂��L��/������Ԃ��擾
	bool GetEnableWait() { return m_EnableWait; }

	//! @brief ���ԍX�V
	bool Update() {
		if (m_Count == 0) { //1�t���[���ڂȂ玞�����L��
			m_StartTime = timeGetTime();
		}
		if (m_Count == m_AverageSampleN) { //60�t���[���ڂȂ畽�ς��v�Z����
			DWORD t = timeGetTime();
			m_NowFps = 1000.f / ((t - m_StartTime) / (float)m_AverageSampleN);
			m_Count = 0;
			m_StartTime = t;
		}
		m_Count++;
		return true;
	}

	//! brief FPS�v�Z�����Z�b�g����
	void Reset() {
		m_Count = 0;
	}

	//! brief �K�莞�Ԃ�����܂ő҂�
	void Wait(int resetTime = 3000) {
		if (m_Count == 0)return;
		//
		if (m_EnableWait == false)return;

		int tookTime = timeGetTime() - m_StartTime;	//������������
		int waitTime = m_Count * 1000 / m_MaxFPS - tookTime;	//�҂ׂ�����
		if (waitTime > 0) {
			Sleep(waitTime);	//�ҋ@
			//			Sleep(1);
			m_LagTime = 0;
		}

		if (waitTime < 0) {
			m_LagTime = -waitTime;
		}

		// �������x��Ă���ꍇ�́A���Z�b�g
		if (m_LagTime >= resetTime) {
			Reset();
		}
	}


private:
	DWORD	m_StartTime = 0;	//!< ����J�n����
	DWORD	m_Count = 0;		//!< �J�E���^
	float	m_NowFps = 0;		//!< ���݂�fps

	static const DWORD m_AverageSampleN = 30;	//!< ���ς����T���v����

	DWORD	m_MaxFPS = 60;			//!< �ݒ肵��FPS

	int		m_LagTime = 0;

	bool	m_EnableWait = true;	//!< FPS����ON/OFF
};

}

#endif
