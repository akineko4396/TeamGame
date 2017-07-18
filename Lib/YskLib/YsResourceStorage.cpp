#include "YskLib.h"

using namespace YskLib;

YsResourceStorage::YsResourceStorage()
{
	// LoadTexture�֐����́A�ǂݍ��ݏڍאݒ�
	memset(&m_LoadTextureInfo, 0, sizeof(D3DX11_IMAGE_LOAD_INFO));
	m_LoadTextureInfo.Width = D3DX11_DEFAULT;				// ���͉摜����擾
	m_LoadTextureInfo.Height = D3DX11_DEFAULT;				// �����͉摜����擾
	m_LoadTextureInfo.Depth = D3DX11_DEFAULT;
	m_LoadTextureInfo.Usage = D3D11_USAGE_DEFAULT;
	m_LoadTextureInfo.CpuAccessFlags = 0;
	m_LoadTextureInfo.MiscFlags = 0;
	m_LoadTextureInfo.FirstMipLevel = D3DX11_DEFAULT;
	m_LoadTextureInfo.MipLevels = 0;						// ���ׂẴ~�b�v�}�b�v�`�F�[�����쐬����
	m_LoadTextureInfo.Format = DXGI_FORMAT_FROM_FILE;		// �t�H�[�}�b�g�͉摜����擾�������̂��g�p
	m_LoadTextureInfo.Filter = D3DX11_FILTER_POINT;			// �e�N�X�`���[�ǂݍ��ݎ��Ɏg�p����t�B���^�[
	m_LoadTextureInfo.MipFilter = D3DX11_FILTER_LINEAR;		// �~�b�v�}�b�v�쐬���Ɏg�p����t�B���^�[
	m_LoadTextureInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_LoadTextureInfo.pSrcInfo = 0;

}


SPtr<YsTexture> YsResourceStorage::LoadTexture(const std::string& filename)
{
	// �f�[�^�ǉ�
	bool bCreate;
	SPtr<YsTexture> pAdd = AddData_Type<YsTexture>(filename, &bCreate);

	// �V�K�������͓ǂݍ���
	if(bCreate){
		if(pAdd->LoadTextureEx(filename, &m_LoadTextureInfo) == false){
			// ���s���͋��Ԃ�
			pAdd = nullptr;
			Optimize();
			return AddData_Type<YsTexture>("Empty YsTexture");
		}
	}

	return pAdd;
}

SPtr<YsGameModel> YsResourceStorage::LoadMesh(const std::string& filename, bool bakeCurve)
{
	// �f�[�^�ǉ�
	bool bCreate;
	SPtr<YsGameModel> pAdd = AddData_Type<YsGameModel>(filename, &bCreate);

	// �V�K�������͓ǂݍ���
	if(bCreate){
		// �ǂݍ���
		if(pAdd->LoadMesh(filename, bakeCurve) == false){
			// ���s���͋��Ԃ�
			pAdd = nullptr;
			Optimize();
			return AddData_Type<YsGameModel>("Empty YsGameModel");
		}
	}

	return pAdd;

}


SPtr<YsSoundData> YsResourceStorage::LoadSound(const std::string& filename, bool b3D)
{
	// �f�[�^�ǉ�
	bool bCreate;
	SPtr<YsSoundData> pAdd = AddData_Type<YsSoundData>(filename, &bCreate);

	// �V�K�������͓ǂݍ���
	if(bCreate){
		if(pAdd->LoadWaveFile(filename, b3D) == false){
			// ���s���͋��Ԃ�
			pAdd = nullptr;
			Optimize();
			return AddData_Type<YsSoundData>("Empty YsSoundData");
		}
	}

	return pAdd;
}
