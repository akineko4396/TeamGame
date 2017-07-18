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
	// ヘッダチェックチェック
	char head[4];
	FILE* fp = fopen(filename, "r");
	fread(head, 3, 1, fp);
	fclose(fp);
	head[3] = '\0';
	if(strcmp(head, "xof") != 0) {
		return false;
	}

	Release();

	// 指定がない場合は、cdgのResStgを使用する
	if(lpTexStg == nullptr){
		lpTexStg = &YsDx.GetResStg();
	}

	// パス
	char szDir  [_MAX_PATH]; 
	char szFname[_MAX_FNAME]; 
	char szExt  [_MAX_EXT]; 
	_splitpath( filename, nullptr, szDir, szFname, szExt);
	m_FilePath = szDir;

	m_pMesh = SPtr<YsMesh>(new YsMesh());
	
	//==============================================
	// DirectX9を使ってXファイル読み込み
	//==============================================
	bool ret = true;
	LPD3DXBUFFER	pD3DXMtrlBuffer = nullptr;
	LPD3DXBUFFER	pAdjacency = nullptr;		// 隣接性データ
	LPD3DXMESH		pMesh = nullptr;

	DWORD mateNum;
	HRESULT hr;
	hr = D3DXLoadMeshFromX(filename,
		D3DXMESH_MANAGED,
		YsDx.GetD3D9Dev(),
		&pAdjacency,
		&pD3DXMtrlBuffer, // マテリアルデータを格納するための変数を指定
		nullptr,
		&mateNum, // マテリアル数を格納するための変数を指定
		&pMesh);	// メッシュデータを格納するための変数を指定
	if(FAILED(hr)) {
		return false;
	}
	//==============================================
	// Direct3D11側へ変換
	//==============================================
	if(!CreateYsMeshFromD3DXMesh(m_pMesh.get(), pMesh)) {
		YsSafe_Release(pD3DXMtrlBuffer);
		YsSafe_Release(pAdjacency);
		YsSafe_Release(pMesh);
		return false;
	}

	//==============================================
	// マテリアル
	//==============================================
	{
		std::string strFile;
		D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
		m_Materials.resize(mateNum);
		for(UINT i = 0; i<mateNum; i++) {
			m_Materials[i].Diffuse = (D3DXVECTOR4&)d3dxMaterials[i].MatD3D.Diffuse;
			m_Materials[i].Ambient = (D3DXVECTOR3&)d3dxMaterials[i].MatD3D.Diffuse;	// XファイルにはAmbientはないので、Diffuseをそのまま使用
			m_Materials[i].Specular = (D3DXVECTOR4&)d3dxMaterials[i].MatD3D.Specular;
			m_Materials[i].Power = d3dxMaterials[i].MatD3D.Power;
			m_Materials[i].Emissive = (D3DXVECTOR4&)d3dxMaterials[i].MatD3D.Emissive;

			m_Materials[i].TextureFileName = (d3dxMaterials[i].pTextureFilename == nullptr ? "" : d3dxMaterials[i].pTextureFilename);

			// テクスチャ読み込み
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
