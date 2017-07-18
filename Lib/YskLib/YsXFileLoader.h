//===============================================================
//! @file YsXFileLoader.h
//! @brief X�t�@�C���ǂݍ��݌n
//!
//! Direct3D9�̋@�\���g���ǂݍ��݂��s���Ă��܂�
//!
//! @author ���c
//===============================================================

#ifndef __YsXFileLoader_h__
#define __YsXFileLoader_h__

namespace YskLib
{

//=================================================================================
//! @brief �X�L�����b�V��X�t�@�C���ǂݍ��݃N���X
//!
//! @param[in] fileName	: �t�@�C���p�X
//! @param[out] outModelTbl	: ���ʂ������A���b�V���z����w��
//! @param[out] outBoneTree	: ���ʂ������A�{�[���z����w��
//! @param[out] outAnimeTbl	: ���ʂ������A�A�j���f�[�^�z����w��
//!
//! @ingroup Graphics_Model
//=================================================================================
bool YsLoadXFile(
	const std::string& fileName,
	std::vector<SPtr<YsSingleModel>>& outModelTbl,
	std::vector<SPtr<YsGameModel::BoneNode>>& outBoneTree,
	std::vector<SPtr<YsAnimationSet>>& outAnimeTbl
);


}
#endif
