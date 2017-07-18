#ifndef __YsFpsControl_h__
#define __YsFpsControl_h__

namespace YskLib {

//====================================================
//! @brief FPS制御クラス
//!
//! @ingroup Etc
//====================================================
class YsFpsControl {
public:

	//! @brief 現在のFPSを取得
	float GetNowFps() { return m_NowFps; }

	//! @brief 設定されている最大FPS値を取得
	DWORD GetMaxFps() { return m_MaxFPS; }

	//! @brief 最大FPS値を設定
	void SetMaxFps(DWORD fps) {
		m_MaxFPS = fps;
		Reset();
	}

	//! @brief 遅延した時間を返す
	int GetLagTime() { return m_LagTime; }

	//! @brief FPS制御待ちを有効/無効にする
	void SetEnableWait(bool enable) { m_EnableWait = enable; }
	//! @brief FPS制御待ち有効/無効状態を取得
	bool GetEnableWait() { return m_EnableWait; }

	//! @brief 時間更新
	bool Update() {
		if (m_Count == 0) { //1フレーム目なら時刻を記憶
			m_StartTime = timeGetTime();
		}
		if (m_Count == m_AverageSampleN) { //60フレーム目なら平均を計算する
			DWORD t = timeGetTime();
			m_NowFps = 1000.f / ((t - m_StartTime) / (float)m_AverageSampleN);
			m_Count = 0;
			m_StartTime = t;
		}
		m_Count++;
		return true;
	}

	//! brief FPS計算をリセットする
	void Reset() {
		m_Count = 0;
	}

	//! brief 規定時間が来るまで待つ
	void Wait(int resetTime = 3000) {
		if (m_Count == 0)return;
		//
		if (m_EnableWait == false)return;

		int tookTime = timeGetTime() - m_StartTime;	//かかった時間
		int waitTime = m_Count * 1000 / m_MaxFPS - tookTime;	//待つべき時間
		if (waitTime > 0) {
			Sleep(waitTime);	//待機
			//			Sleep(1);
			m_LagTime = 0;
		}

		if (waitTime < 0) {
			m_LagTime = -waitTime;
		}

		// 激しく遅れている場合は、リセット
		if (m_LagTime >= resetTime) {
			Reset();
		}
	}


private:
	DWORD	m_StartTime = 0;	//!< 測定開始時刻
	DWORD	m_Count = 0;		//!< カウンタ
	float	m_NowFps = 0;		//!< 現在のfps

	static const DWORD m_AverageSampleN = 30;	//!< 平均を取るサンプル数

	DWORD	m_MaxFPS = 60;			//!< 設定したFPS

	int		m_LagTime = 0;

	bool	m_EnableWait = true;	//!< FPS制御ON/OFF
};

}

#endif
