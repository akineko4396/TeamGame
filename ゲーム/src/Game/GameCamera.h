#ifndef __GameCamera_h__
#define __GameCamera_h__

#include "GameWorld\CollisionEngine\CollisionEngine.h"

//======================================
//
// �Q�[���p�ɂ�����x�@�\�����������J����
//
// INPUT�N���X���g�p���ăJ�������삵�܂�
//
//
//======================================
class GameCamera : public YsCamera {
public:
	YsMatrix	m_LocalMat;		// ���[�J���s��@��{�I�ɂ�Z���ƍ��W�̕��������s�ɂȂ��Ă��邱��
	YsMatrix	m_BaseMat;		// ��ƂȂ�s��@�Q�[���ł͂����ɃL�����̍s�񂪓��銴��

	bool		m_IsFPSMode;	// FPS���[�h
	bool		m_IsTPSMode;	// TPS���[�h

	//
	GameCamera() {
		// �J�����f�[�^������
		m_IsFPSMode = false;
		m_IsTPSMode = false;

		m_LocalMat.CreateMove(0, 0, -15);

		SetPerspectiveFovLH(60, (float)YsDx.GetRezoW() / YsDx.GetRezoH(), 0.01f, 1000);

	}

	// �����ݒ�
	//  Rx			�c �����̃J�����p�xX
	//  Ry			�c �����̃J�����p�xY
	//  CamZoom		�c �����̃J�����Y�[���l(FPS���[�h���͖���)
	//  fpsModeOn	�c FPS���[�hON/OFF
	//	TpsModeOn	�c TPS���[�hON/OFF
	void Init(float Rx, float Ry, float CamZoom, bool fpsModeOn = false, bool tpsModeOn = false) {
		m_LocalMat.CreateRotateX(Rx);
		m_LocalMat.RotateY(Ry);
		m_LocalMat.Move_Local(0, 0, CamZoom);
		m_BaseMat.CreateMove(0, 0, 0);
		m_IsFPSMode = fpsModeOn;
		m_IsTPSMode = tpsModeOn;
	}

	// ����
	void Update(YsVec3 _pos = YsVec3(0, 0, 0)) {
		float ratio = 1.0f;
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
			ratio *= 0.2f;
		}
		else if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
			ratio *= 3.0f;
		}

		//=========================================================
		// �J��������
		//=========================================================
		// FPS���[�h
		if (m_IsFPSMode) {
			// FPS���[�h�̓Y�[���ړ��͖���
			m_LocalMat.SetPos(0, 0, 0);

			if (APP.m_bWndActive) {
				POINT *pt = INPUT.GetMouseMoveValue();

				// �z�C�[���h���b�O
				if (INPUT.MouseCheck(CInput::MOUSE_M)) {
					m_BaseMat.GetPos() -= mCam.GetXAxis() * (float)pt->x * 0.05f*ratio;
					m_BaseMat.GetPos() += mCam.GetYAxis() * (float)pt->y * 0.05f*ratio;
				}
				else {
					// ���_��]
					m_LocalMat.RotateAxis(m_LocalMat.GetXAxis(), pt->y*0.5f);
					m_LocalMat.RotateY(pt->x*0.5f);
				}
			}

			// �E�B���h�E�̃A�N�e�B�u��Ԃɂ��؂�ւ�
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
			// TPS���[�h�̓Y�[���ړ��͖���
			m_LocalMat.SetPos(0, 3, 3);

			if (APP.m_bWndActive){
				POINT *pt = INPUT.GetMouseMoveValue();

				// ���_��]
				m_BaseMat.GetPos() = _pos;
				m_BaseMat.RotateY(pt->x*0.5f);
			}

			// �E�B���h�E�̃A�N�e�B�u��Ԃɂ��؂�ւ�
			if (m_IsTPSMode) {
				if (APP.m_bWndActive) {
					INPUT.SetFPSMode(APP.m_hWnd, true);
				}
				else {
					INPUT.SetFPSMode(APP.m_hWnd, false);
				}
			}
		}
		// �ʏ탂�[�h
		else if (!m_IsFPSMode && !m_IsTPSMode){

			if (APP.m_bWndActive) {
				// �}�E�X�E�{�^��
				if (INPUT.MouseCheck(CInput::MOUSE_R)) {
					POINT *pt = INPUT.GetMouseMoveValue();

					m_LocalMat.RotateAxis(m_LocalMat.GetXAxis(), pt->y*0.5f);
					m_LocalMat.RotateY(pt->x*0.5f);
				}
				// �}�E�X�z�C�[��
				{
					// ���[�J��Z�����Ɉړ�
					m_LocalMat.Move_Local(0, 0, INPUT.GetMouseWheel()*0.002f*ratio);

					// ��������
					float len = m_LocalMat.GetPos().Length();	// ����
					float d = m_LocalMat.GetZAxis().Dot(m_LocalMat.GetPos());	// Z�������ƍ��W�Ƃ̓��ςŁA�ʂ�߂��Ă��܂��Ă��邩����
					if (d > 0)len *= -1;	// �Y�[���������Ēʂ�߂����̂ŁA�����̓}�C�i�X�ɂ���

					if (len < 0.03f) {
						m_LocalMat.Move_Local(0, 0, len - 0.03f);
					}
				}
				// �z�C�[���h���b�O
				if (INPUT.MouseCheck(CInput::MOUSE_M)) {
					POINT *pt = INPUT.GetMouseMoveValue();
					m_BaseMat.GetPos() -= mCam.GetXAxis() * (float)pt->x * 0.05f*ratio;
					m_BaseMat.GetPos() += mCam.GetYAxis() * (float)pt->y * 0.05f*ratio;
				}
			}

			INPUT.SetFPSMode(APP.m_hWnd, false);
		}


		//������쐬
		//VF.Create(mProj, mView, mCam);

	}

	// �Z�b�g
	void SetCamera() {
		//==========================================
		// �J�����ݒ�
		//==========================================
		// �ˉe�s��ݒ�
		SetProj(mProj);

		// �ŏI�I�ȃJ�����s������߂�
		mCam = m_LocalMat * m_BaseMat;

		// �J�����s�񂩂�r���[�s������
		CameraToView();

		
		//������쐬
		VF.Create(mProj, mView, mCam);

	}

	void SetCamera(YsVec3& _Pos) {
		//==========================================
		// �J�����ݒ�
		//==========================================
		// �ˉe�s��ݒ�
		SetProj(mProj);

		//�v���C���[����J�����܂ł̋���
		m_LocalMat.CreateMove(0, 1.7f, -4.0f);

		//�J�����s��̐e�s��̍��W���v���C���[�̍��W�ɂ���
		m_BaseMat.GetPos() = _Pos;

		// �ŏI�I�ȃJ�����s������߂�
		mCam = m_LocalMat * m_BaseMat;

		// �J�����s�񂩂�r���[�s������
		CameraToView();


		//������쐬
		VF.Create(mProj, mView, mCam);
	}
};

#endif
