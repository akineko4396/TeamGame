//===============================================================
//! @file YsXEDLoader.h
//! @brief XED�t�@�C���ǂݍ���
//!
//! XED�t�@�C���́AKAMADA SkinMesh Editor�ō쐬�ł��郂�f���`���ł�
//!
//! @author ���c
//===============================================================

#ifndef __YsXEDLoader_h__
#define __YsXEDLoader_h__

namespace YskLib
{

//=================================================================================
//! @brief XED�t�@�C���ǂݍ��݃N���X
//!
//! @param[in] fileName	�c �t�@�C���p�X
//! @param[out] outModelTbl	�c ���ʂ������A���b�V���z����w��
//! @param[out] outBoneTree	�c ���ʂ������A�{�[���z����w��
//! @param[out] outAnimeTbl	�c ���ʂ������A�A�j���f�[�^�z����w��
//! @param[in]  bakeCurve	�c �Ȑ���Ԍn�̃A�j���L�[�́A�S�Đ��`��ԂƂ��ĕϊ��E�ǉ�����(�������y���Ȃ�܂�)
//!
//! @ingroup Graphics_Model
//=================================================================================
bool YsLoadXEDFile(
	const std::string& fileName,
	std::vector<SPtr<YsSingleModel>>& outModelTbl,
	std::vector<SPtr<YsGameModel::BoneNode>>& outBoneTree,
	std::vector<SPtr<YsAnimationSet>>& outAnimeTbl,
	std::vector< std::vector<YsXEDRigidBody> >&	outRigidBodyTbl,
	bool bakeCurve
);


}

#endif
