//===============================================================
//! @file YsCamera.h
//! @brief �J�����N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsCamera_h__
#define __YsCamera_h__

namespace YskLib
{

//============================================================
//!
//! @brief �J�����N���X
//!
//!  �r���[�s��Ǝˉe�s�������								\n
//!  �ݒ肵�����e�͐ÓI�ϐ�LastCam�ɂ��R�s�[�����̂ŁA	\n
//!  YsCamera::LastCam�łǂ�����ł��ݒ���e�̎擾���\		\n
//!
//! @ingroup Math
//============================================================
class YsCamera{
public:
	YsMatrix mProj;	//!< �ˉe�s��B

	YsMatrix mCam;	//!< �J�����s��B���ڑ��삷���ł��B
	YsMatrix mView;	//!< �J�����s����r���[�s��ɕϊ������s��(�t�s��)�B
					//!< SetTransform�֐����ɍX�V�����̂Œ��ڑ��삷��K�v�͖���

	//! @brief �Ō�Ɏg�p���ꂽ�J�������̃R�s�[������(YsCamera�N���X�ő��삳���)
	static YsCamera		LastCam;

	//! @brief �J�����s��mCam����A�r���[�s��mView���쐬�BLastCam�ɂ��R�s�[�����B
	void CameraToView();

	//! @brief �r���[�s��mView����A�J�����s��mCam���쐬�BLastCam�ɂ��R�s�[�����B
	void ViewToCamera();

	//! @brief �r���[�s��𒼐�mView�ɃZ�b�g���A�J�����s��mCam���X�V����
	void SetView(const YsMatrix& lpmView);

	//! @brief �ˉe�s���mProj�ɍ쐬����BLastCam�ɂ��R�s�[�����B
	void SetPerspectiveFovLH(float Siyakaku, float Aspect, float zNear, float zFar);

	//! @brief ���ˉe�s���mProj�ɍ쐬����BLastCam�ɂ��R�s�[�����B
	void SetOrthoLH(float w, float h, float zNear, float zFar);

	//! @brief �ˉe�s��𒼐�mProj�ɃZ�b�g����BLastCam�ɂ��R�s�[�����B
	void SetProj(const YsMatrix& lpmProj);

	//! @brief ���݂̎ˉe�s���LastCam�փR�s�[
	void SetProj(){
		LastCam.mProj = mProj;
	}


	//===============================================================================================
	// ���̑�
	//===============================================================================================

	//! @brief 3D��2D�ϊ�
	//! 3D���W��2D���W�֕ϊ�����(���݂̃f�o�C�X�R���e�L�X�g�ɃZ�b�g����Ă���r���[�|�[�g�����g�p)
	//! @param[in]	devContext	�c Direct3D11�̃f�o�C�X�R���e�L�X�g
	//! @param[out]	lpvOut		�c ���ʂł���2D���W�������Ă���
	//! @param[in]	lpPos		�c �����̊�ƂȂ�3D���W
	void Convert3Dto2D(YsVec3& lpvOut, const YsVec3& lpPos);

	//! @brief 2D��3D�ϊ�
	//! 2D���W��3D���W�֕ϊ�����(���݂̃f�o�C�X�R���e�L�X�g�ɃZ�b�g����Ă���r���[�|�[�g�����g�p)
	//! @param[in]	devContext	�c Direct3D11�̃f�o�C�X�R���e�L�X�g
	//! @param[out]	lpvOut		�c ���ʂł���3D���W�������Ă���
	//! @param[in]	lpPos		�c �����̊�ƂȂ�2D���W(x,y�̂ݎg�p)
	void Convert2Dto3D(YsVec3& lpvOut, const YsVec3& lpvPos);

	// 
	YsCamera();
};

}

#endif
