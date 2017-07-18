#ifndef __GameCamera_h__
#define __GameCamera_h__

#include "GameWorld\CollisionEngine\CollisionEngine.h"

//======================================
//
// ゲーム用にある程度機能を実装したカメラ
//
// INPUTクラスを使用してカメラ操作します
//
//
//======================================
class GameCamera : public YsCamera {
public:
	YsMatrix	m_LocalMat;		// ローカル行列　基本的にはZ軸と座標の方向が平行になっていること
	YsMatrix	m_BaseMat;		// 基準となる行列　ゲームではここにキャラの行列が入る感じ

	bool		m_IsFPSMode;	// FPSモード
	bool		m_IsTPSMode;	// TPSモード

	//
	GameCamera() {
		// カメラデータ初期化
		m_IsFPSMode = false;
		m_IsTPSMode = false;

		m_LocalMat.CreateMove(0, 0, -15);

		SetPerspectiveFovLH(60, (float)YsDx.GetRezoW() / YsDx.GetRezoH(), 0.01f, 1000);

	}

	// 初期設定
	//  Rx			… 初期のカメラ角度X
	//  Ry			… 初期のカメラ角度Y
	//  CamZoom		… 初期のカメラズーム値(FPSモード時は無効)
	//  fpsModeOn	… FPSモードON/OFF
	//	TpsModeOn	… TPSモードON/OFF
	void Init(float Rx, float Ry, float CamZoom, bool fpsModeOn = false, bool tpsModeOn = false) {
		m_LocalMat.CreateRotateX(Rx);
		m_LocalMat.RotateY(Ry);
		m_LocalMat.Move_Local(0, 0, CamZoom);
		m_BaseMat.CreateMove(0, 0, 0);
		m_IsFPSMode = fpsModeOn;
		m_IsTPSMode = tpsModeOn;
	}

	// 処理
	void Update(YsVec3 _pos = YsVec3(0, 0, 0)) {
		float ratio = 1.0f;
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
			ratio *= 0.2f;
		}
		else if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
			ratio *= 3.0f;
		}

		//=========================================================
		// カメラ操作
		//=========================================================
		// FPSモード
		if (m_IsFPSMode) {
			// FPSモードはズーム移動は無効
			m_LocalMat.SetPos(0, 0, 0);

			if (APP.m_bWndActive) {
				POINT *pt = INPUT.GetMouseMoveValue();

				// ホイールドラッグ
				if (INPUT.MouseCheck(CInput::MOUSE_M)) {
					m_BaseMat.GetPos() -= mCam.GetXAxis() * (float)pt->x * 0.05f*ratio;
					m_BaseMat.GetPos() += mCam.GetYAxis() * (float)pt->y * 0.05f*ratio;
				}
				else {
					// 視点回転
					m_LocalMat.RotateAxis(m_LocalMat.GetXAxis(), pt->y*0.5f);
					m_LocalMat.RotateY(pt->x*0.5f);
				}
			}

			// ウィンドウのアクティブ状態により切り替え
			if (m_IsFPSMode) {
				if (APP.m_bWndActive) {
					INPUT.SetFPSMode(APP.m_hWnd, true);
				}
				else {
					INPUT.SetFPSMode(APP.m_hWnd, false);
				}
			}
		}
		else if (m_IsTPSMode){
			// TPSモードはズーム移動は無効
			m_LocalMat.SetPos(0, 3, 3);

			if (APP.m_bWndActive){
				POINT *pt = INPUT.GetMouseMoveValue();

				// 視点回転
				m_BaseMat.GetPos() = _pos;
				m_BaseMat.RotateY(pt->x*0.5f);
			}

			// ウィンドウのアクティブ状態により切り替え
			if (m_IsTPSMode) {
				if (APP.m_bWndActive) {
					INPUT.SetFPSMode(APP.m_hWnd, true);
				}
				else {
					INPUT.SetFPSMode(APP.m_hWnd, false);
				}
			}
		}
		// 通常モード
		else if (!m_IsFPSMode && !m_IsTPSMode){

			if (APP.m_bWndActive) {
				// マウス右ボタン
				if (INPUT.MouseCheck(CInput::MOUSE_R)) {
					POINT *pt = INPUT.GetMouseMoveValue();

					m_LocalMat.RotateAxis(m_LocalMat.GetXAxis(), pt->y*0.5f);
					m_LocalMat.RotateY(pt->x*0.5f);
				}
				// マウスホイール
				{
					// ローカルZ方向に移動
					m_LocalMat.Move_Local(0, 0, INPUT.GetMouseWheel()*0.002f*ratio);

					// 距離制限
					float len = m_LocalMat.GetPos().Length();	// 距離
					float d = m_LocalMat.GetZAxis().Dot(m_LocalMat.GetPos());	// Z軸方向と座標との内積で、通り過ぎてしまっているか判定
					if (d > 0)len *= -1;	// ズームしすぎて通り過ぎたので、距離はマイナスにする

					if (len < 0.03f) {
						m_LocalMat.Move_Local(0, 0, len - 0.03f);
					}
				}
				// ホイールドラッグ
				if (INPUT.MouseCheck(CInput::MOUSE_M)) {
					POINT *pt = INPUT.GetMouseMoveValue();
					m_BaseMat.GetPos() -= mCam.GetXAxis() * (float)pt->x * 0.05f*ratio;
					m_BaseMat.GetPos() += mCam.GetYAxis() * (float)pt->y * 0.05f*ratio;
				}
			}

			INPUT.SetFPSMode(APP.m_hWnd, false);
		}


		//視錘台作成
		//VF.Create(mProj, mView, mCam);

	}

	// セット
	void SetCamera() {
		//==========================================
		// カメラ設定
		//==========================================
		// 射影行列設定
		SetProj(mProj);

		// 最終的なカメラ行列を求める
		mCam = m_LocalMat * m_BaseMat;

		// カメラ行列からビュー行列を作る
		CameraToView();

		
		//視錘台作成
		VF.Create(mProj, mView, mCam);

	}

	void SetCamera(YsVec3& _Pos) {
		//==========================================
		// カメラ設定
		//==========================================
		// 射影行列設定
		SetProj(mProj);

		//プレイヤーからカメラまでの距離
		m_LocalMat.CreateMove(0, 1.7f, -4.0f);

		//カメラ行列の親行列の座標をプレイヤーの座標にする
		m_BaseMat.GetPos() = _Pos;

		// 最終的なカメラ行列を求める
		mCam = m_LocalMat * m_BaseMat;

		// カメラ行列からビュー行列を作る
		CameraToView();


		//視錘台作成
		VF.Create(mProj, mView, mCam);
	}
};

#endif
