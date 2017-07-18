#include "YskLib.h"

using namespace YskLib;

//=======================================================================
//
// YsSingleModel
//
//=======================================================================
/*
bool YsSingleModel::LoadXFile(const std::string& filename)
{
	// �w�b�_�`�F�b�N�`�F�b�N
	char head[4];
	FILE* fp = fopen(filename, "r");
	fread(head, 3, 1, fp);
	fclose(fp);
	head[3] = '\0';
	if(strcmp(head, "xof") != 0) {
		return false;
	}

	Release();

	// �w�肪�Ȃ��ꍇ�́Acdg��ResStg���g�p����
	if(lpTexStg == nullptr){
		lpTexStg = &YsDx.GetResStg();
	}

	// �p�X
	char szDir  [_MAX_PATH]; 
	char szFname[_MAX_FNAME]; 
	char szExt  [_MAX_EXT]; 
	_splitpath( filename, nullptr, szDir, szFname, szExt);
	m_FilePath = szDir;

	m_pMesh = SPtr<YsMesh>(new YsMesh());
	
	//==============================================
	// DirectX9���g����X�t�@�C���ǂݍ���
	//==============================================
	bool ret = true;
	LPD3DXBUFFER	pD3DXMtrlBuffer = nullptr;
	LPD3DXBUFFER	pAdjacency = nullptr;		// �אڐ��f�[�^
	LPD3DXMESH		pMesh = nullptr;

	DWORD mateNum;
	HRESULT hr;
	hr = D3DXLoadMeshFromX(filename,
		D3DXMESH_MANAGED,
		YsDx.GetD3D9Dev(),
		&pAdjacency,
		&pD3DXMtrlBuffer, // �}�e���A���f�[�^���i�[���邽�߂̕ϐ����w��
		nullptr,
		&mateNum, // �}�e���A�������i�[���邽�߂̕ϐ����w��
		&pMesh);	// ���b�V���f�[�^���i�[���邽�߂̕ϐ����w��
	if(FAILED(hr)) {
		return false;
	}
	//==============================================
	// Direct3D11���֕ϊ�
	//==============================================
	if(!CreateYsMeshFromD3DXMesh(m_pMesh.get(), pMesh)) {
		YsSafe_Release(pD3DXMtrlBuffer);
		YsSafe_Release(pAdjacency);
		YsSafe_Release(pMesh);
		return false;
	}

	//==============================================
	// �}�e���A��
	//==============================================
	{
		std::string strFile;
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		m_Materials.resize(mateNum);
		for(UINT i = 0; i<mateNum; i++) {
			m_Materials[i].Diffuse = (D3DXVECTOR4&)d3dxMaterials[i].MatD3D.Diffuse;
			m_Materials[i].Ambient = (D3DXVECTOR3&)d3dxMaterials[i].MatD3D.Diffuse;	// X�t�@�C���ɂ�Ambient�͂Ȃ��̂ŁADiffuse�����̂܂܎g�p
			m_Materials[i].Specular = (D3DXVECTOR4&)d3dxMaterials[i].MatD3D.Specular;
			m_Materials[i].Power = d3dxMaterials[i].MatD3D.Power;
			m_Materials[i].Emissive = (D3DXVECTOR4&)d3dxMaterials[i].MatD3D.Emissive;

			m_Materials[i].TextureFileName = (d3dxMaterials[i].pTextureFilename == nullptr ? "" : d3dxMaterials[i].pTextureFilename);

			// �e�N�X�`���ǂݍ���
			strFile = m_FilePath + m_Materials[i].TextureFileName;
			m_Materials[i].TexSet.LoadTexture(strFile.c_str(), lpTexStg);
		}
	}

	YsSafe_Release(pD3DXMtrlBuffer);
	YsSafe_Release(pAdjacency);
	YsSafe_Release(pMesh);

	return true;
}
*/

void YsSingleModel::Release()
{
	m_pMesh = nullptr;
	m_Materials.clear();

	m_Name = "";
}

void YsSingleModel::SetDrawData()
{
	if(m_pMesh == nullptr)return;

	m_pMesh->SetDrawData();
}
