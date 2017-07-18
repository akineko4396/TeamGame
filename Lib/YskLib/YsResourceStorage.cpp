#include "YskLib.h"

using namespace YskLib;

YsResourceStorage::YsResourceStorage()
{
	// LoadTexture関数時の、読み込み詳細設定
	memset(&m_LoadTextureInfo, 0, sizeof(D3DX11_IMAGE_LOAD_INFO));
	m_LoadTextureInfo.Width = D3DX11_DEFAULT;				// 幅は画像から取得
	m_LoadTextureInfo.Height = D3DX11_DEFAULT;				// 高さは画像から取得
	m_LoadTextureInfo.Depth = D3DX11_DEFAULT;
	m_LoadTextureInfo.Usage = D3D11_USAGE_DEFAULT;
	m_LoadTextureInfo.CpuAccessFlags = 0;
	m_LoadTextureInfo.MiscFlags = 0;
	m_LoadTextureInfo.FirstMipLevel = D3DX11_DEFAULT;
	m_LoadTextureInfo.MipLevels = 0;						// すべてのミップマップチェーンを作成する
	m_LoadTextureInfo.Format = DXGI_FORMAT_FROM_FILE;		// フォーマットは画像から取得したものを使用
	m_LoadTextureInfo.Filter = D3DX11_FILTER_POINT;			// テクスチャー読み込み時に使用するフィルター
	m_LoadTextureInfo.MipFilter = D3DX11_FILTER_LINEAR;		// ミップマップ作成時に使用するフィルター
	m_LoadTextureInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_LoadTextureInfo.pSrcInfo = 0;

}


SPtr<YsTexture> YsResourceStorage::LoadTexture(const std::string& filename)
{
	// データ追加
	bool bCreate;
	SPtr<YsTexture> pAdd = AddData_Type<YsTexture>(filename, &bCreate);

	// 新規生成時は読み込み
	if(bCreate){
		if(pAdd->LoadTextureEx(filename, &m_LoadTextureInfo) == false){
			// 失敗時は空を返す
			pAdd = nullptr;
			Optimize();
			return AddData_Type<YsTexture>("Empty YsTexture");
		}
	}

	return pAdd;
}

SPtr<YsGameModel> YsResourceStorage::LoadMesh(const std::string& filename, bool bakeCurve)
{
	// データ追加
	bool bCreate;
	SPtr<YsGameModel> pAdd = AddData_Type<YsGameModel>(filename, &bCreate);

	// 新規生成時は読み込み
	if(bCreate){
		// 読み込み
		if(pAdd->LoadMesh(filename, bakeCurve) == false){
			// 失敗時は空を返す
			pAdd = nullptr;
			Optimize();
			return AddData_Type<YsGameModel>("Empty YsGameModel");
		}
	}

	return pAdd;

}


SPtr<YsSoundData> YsResourceStorage::LoadSound(const std::string& filename, bool b3D)
{
	// データ追加
	bool bCreate;
	SPtr<YsSoundData> pAdd = AddData_Type<YsSoundData>(filename, &bCreate);

	// 新規生成時は読み込み
	if(bCreate){
		if(pAdd->LoadWaveFile(filename, b3D) == false){
			// 失敗時は空を返す
			pAdd = nullptr;
			Optimize();
			return AddData_Type<YsSoundData>("Empty YsSoundData");
		}
	}

	return pAdd;
}
