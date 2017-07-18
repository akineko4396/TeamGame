//===============================================================
//! @file YsResourceStorage.h
//! @brief ���\�[�X�Ǘ��֌W
//!
//! @author ���c
//===============================================================
#ifndef __YsResourceStorage_h__
#define __YsResourceStorage_h__

#include <map>

namespace YskLib{

class YsTexture;
class YsGameModel;
class YsSoundData;

//===================================================================================
//! @brief ���\�[�X�q��
//!
//!	YsDataWeakStorage<YsIResource>���p��(FlyWeight�p�^�[���I�Ȋ���)						\n
//!		�������YsIResource�N���X���p�������N���X�C���X�^���X���쐬�E�ۊǂł���N���X�B	\n
//!		�@�����ł�map�œo�^���Ă���̂ŁA�d���ǂݍ��݂��h����B							\n
//!		���߂�l�̃f�[�^��shared�|�C���^�Ƃ��ĕԂ�̂ŁAshared�|�C���^�̓�����A		\n
//!		�@��������͏����Ȃ��Ă悢�B�����傪���Ȃ��Ȃ������_�Ŏ����ŉ�������B		\n
//!
//! @ingroup Etc
//===================================================================================
class YsResourceStorage : public YsDataWeakStorage<YsIResource, YsLock>{
public:
	//====================================================================================================
	//! @brief �e�X�N�`����ǂݍ��݂����Ԃ��B���ɓo�^�ς݂Ȃ�ǂݍ��݂͍s�킸�A�����Ԃ��B
	//! �ڍׂȓǂݍ��݂̐ݒ�́A�ϐ�m_LoadTextureInfo���g�p���Ă�̂ŁA���̕ϐ���ύX����Ɠǂݍ��ׂ݂̍��Ȑݒ��ύX�ł��܂�
	//! @param[in]	filename	�c �t�@�C����
	//! @return �e�N�X�`��
	//====================================================================================================
	SPtr<YsTexture> LoadTexture(const std::string& filename);
	D3DX11_IMAGE_LOAD_INFO	m_LoadTextureInfo;		// LoadTexture�֐��ŉ摜�ǂݍ��ݎ��̏ڍאݒ�@�ǂݍ��ׂ݂̍��Ȑݒ�͂�����ύX�����OK

	//====================================================================================================
	//! @brief ���b�V����ǂݍ��݂����Ԃ��B���ɓo�^�ς݂Ȃ�ǂݍ��݂͍s�킸�A�����Ԃ��B
	//!	X�t�@�C����XED�t�@�C���ɑΉ�(�X�L�����b�V���Ή�)
	//! @param[in]	filename	�c �t�@�C����
	//! @param[in]	bakeCurve	�c �Ȑ���Ԍn�̃A�j���L�[�́A�S�Đ��`��ԂƂ��ĕϊ��E�ǉ�����(XED�p �������y���Ȃ�܂�)
	//! @return YsGameModel���f��
	//====================================================================================================
	SPtr<YsGameModel> LoadMesh(const std::string& filename, bool bakeCurve = true);

	//====================================================================================================
	//! @brief �����f�[�^�̂�ǂݍ��݂����Ԃ��B���ɓo�^�ς݂Ȃ�ǂݍ��݂͍s�킸�A�����Ԃ��B
	//! @param[in]	b3D		�c 3D�T�E���h�Ƃ��ēǂݍ��ށH(���m�����`���̉����t�@�C���łȂ���3D�T�E���h�ɂ͂ł��܂���)
	//! @return �T�E���h�f�[�^
	//====================================================================================================
	SPtr<YsSoundData> LoadSound(const std::string& filename, bool b3D);




	//====================================================================================================
	// �S���\�[�X�����擾
	//====================================================================================================
	void DEBUG_GetNowResources(std::vector<std::string>& out){
		auto dataMap = GetDataMap();
		auto it = dataMap.begin();
		while(it != dataMap.end()){
			if((*it).second.expired() == false){
				out.push_back( (*it).second.lock()->GetTypeName() );
			}

			++it;
		}
	}

	// 
	YsResourceStorage();
};

}

#endif
