#include "YskLib.h"

//#include "WICTextureLoader.h"

using namespace YskLib;

bool YsTexture::LoadTexture(const std::string& filename)
{
	Release();
	if(filename.empty())return false;

	/*
	// 画像ファイルを読み込み、シェーダリソースビューを作成する
	if(FAILED(D3DX11CreateShaderResourceViewFromFile(YsDx.GetDev(), filename, nullptr, nullptr, &m_pTexView, nullptr))){
		Release();
		return false;
	}
	// リソース本体も取得しておく
	m_pTexView->GetResource((ID3D11Resource**)&m_pTexture2D);
	// 情報も取得
	m_pTexture2D->GetDesc(&m_Desc);
	*/

	D3DX11_IMAGE_LOAD_INFO info;
	memset(&info, 0, sizeof(D3DX11_IMAGE_LOAD_INFO));
	info.Width = D3DX11_DEFAULT;
	info.Height = D3DX11_DEFAULT;
	info.Depth = D3DX11_DEFAULT;
	info.Usage = D3D11_USAGE_DEFAULT;
	info.CpuAccessFlags = 0;
	info.MiscFlags = 0;
	info.FirstMipLevel = D3DX11_DEFAULT;
	info.MipLevels = 0;						// すべてのミップマップチェーンを作成する
	info.Format = DXGI_FORMAT_FROM_FILE;
	info.Filter = D3DX11_FILTER_POINT;		// テクスチャー読み込み時に使用するフィルター
	info.MipFilter = D3DX11_FILTER_LINEAR;	// ミップマップ作成時に使用するフィルター
	info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	info.pSrcInfo = 0;

	return LoadTextureEx(filename, &info);
}

bool YsTexture::LoadTextureEx(const std::string& filename, UINT MipLevels, DXGI_FORMAT format, UINT bindFlag)
{
	Release();
	if(filename.empty())return false;

	D3DX11_IMAGE_LOAD_INFO info;
	memset(&info, 0, sizeof(D3DX11_IMAGE_LOAD_INFO));
	info.Width = D3DX11_DEFAULT;
	info.Height = D3DX11_DEFAULT;
	info.Depth = D3DX11_DEFAULT;
	info.Usage = D3D11_USAGE_DEFAULT;
	info.CpuAccessFlags = 0;
	info.MiscFlags = 0;
	info.FirstMipLevel = D3DX11_DEFAULT;
	info.MipLevels = MipLevels;
	info.Format = format;
	info.Filter = D3DX11_FILTER_POINT;			// テクスチャー読み込み時に使用するフィルター
	info.MipFilter = D3DX11_FILTER_LINEAR;		// ミップマップ作成時に使用するフィルター
	info.BindFlags = bindFlag;
	info.pSrcInfo = 0;

	return LoadTextureEx(filename, &info);
}

bool YsTexture::LoadTextureEx(const std::string& filename, D3DX11_IMAGE_LOAD_INFO* loadInfo)
{
	Release();
	if(filename.empty())return false;

	/*
	// wchar変換
	int len = strlen(filename);
	WCHAR* wStr = new WCHAR[len + 1];
	wStr[len] = 0;
	size_t wLen;
	mbstowcs_s(&wLen, wStr, len + 1, filename, _TRUNCATE);
	
	if (FAILED(DirectX::CreateWICTextureFromFile(YsDx.GetDev(), wStr, (ID3D11Resource**)&m_pTexture2D, nullptr))) {
		Release();
		delete[] wStr;
		return false;
	}

	delete[] wStr;
	*/

	// 画像ファイルを読み込む
	if (FAILED(D3DX11CreateTextureFromFile(YsDx.GetDev(), filename.c_str(), loadInfo, nullptr, (ID3D11Resource**)&m_pTexture2D, nullptr))) {
		Release();
		return false;
	}

	// m_pTexture2Dから、各ビューを作成する
	if (CreateViewsFromTexture2D() == false) {
		Release();
		return false;
	}

	// ファイル名記憶
	m_strFileName = filename;

	return true;
}


bool YsTexture::Create(int w, int h, DXGI_FORMAT format, const void* fillData)
{
	Release();

	// 2Dテクスチャの設定
	memset( &m_Desc, 0, sizeof( m_Desc ) );
	m_Desc.Usage              = D3D11_USAGE_DEFAULT;
	m_Desc.Format             = format;
	m_Desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	m_Desc.Width              = (UINT)w;
	m_Desc.Height             = (UINT)h;
	m_Desc.CPUAccessFlags     = 0;
	m_Desc.MipLevels          = 1;
	m_Desc.ArraySize          = 1;
	m_Desc.SampleDesc.Count   = 1;
	m_Desc.SampleDesc.Quality = 0;

	// 2Dテクスチャの生成
	HRESULT hr = YsDx.GetDev()->CreateTexture2D(&m_Desc, nullptr, &m_pTexture2D);
	if ( FAILED( hr ) ){
		Release();
		return false;
	}

	// 塗りつぶしデータが指定されているなら、そのデータをテクスチャに流し込む
	if(fillData){
		YsDx.GetDevContext()->UpdateSubresource(m_pTexture2D, 0, nullptr, fillData, 0, 0);
	}

	// 
	m_strFileName = "";

	// m_pTexture2Dから、各ビューを作成する
	if(CreateViewsFromTexture2D() == false) {
		Release();
		return false;
	}

	return true;

}

bool YsTexture::CreateRT(int w, int h, DXGI_FORMAT format, UINT arrayCnt, const void* fillData)
{
	Release();

	// 2Dテクスチャの設定
	memset(&m_Desc, 0, sizeof(m_Desc));
	m_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Desc.Format = format;
	m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_Desc.Width = (UINT)w;
	m_Desc.Height = (UINT)h;
	m_Desc.CPUAccessFlags = 0;
	m_Desc.MipLevels = 1;
	m_Desc.ArraySize = arrayCnt;
//	m_Desc.SampleDesc.Count = 1;
//	m_Desc.SampleDesc.Quality = 0;
	// MSAA(マルチサンプル アンチエイリアシング)は、バックバッファの設定と合わせる
	m_Desc.SampleDesc = YsDx.GetBackBuffer()->GetInfo().SampleDesc;

	// 2Dテクスチャの生成
	HRESULT hr = YsDx.GetDev()->CreateTexture2D(&m_Desc, nullptr, &m_pTexture2D);
	if (FAILED(hr)){
		Release();
		return false;
	}

	// 塗りつぶしデータが指定されているなら、そのデータをテクスチャに流し込む
	if(fillData){
		YsDx.GetDevContext()->UpdateSubresource(m_pTexture2D, 0, nullptr, fillData, 0, 0);
	}

	// 
	m_strFileName = "";

	// m_pTexture2Dから、各ビューを作成する
	if(CreateViewsFromTexture2D() == false) {
		Release();
		return false;
	}

	return true;

}

bool YsTexture::CreateCubeRT(int w, int h, DXGI_FORMAT format)
{
	Release();

	// 2Dテクスチャの設定
	memset(&m_Desc, 0, sizeof(m_Desc));
	m_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Desc.Format = format;
	m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_Desc.Width = (UINT)w;
	m_Desc.Height = (UINT)h;
	m_Desc.CPUAccessFlags = 0;
	m_Desc.MipLevels = 0;
	m_Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;	// キューブマップ
	m_Desc.ArraySize = 6;		// 6面必要なので、テクスチャ配列6枚分
	//	m_Desc.SampleDesc.Count = 1;
	//	m_Desc.SampleDesc.Quality = 0;
	// MSAA(マルチサンプル アンチエイリアシング)は、バックバッファの設定と合わせる
	m_Desc.SampleDesc = YsDx.GetBackBuffer()->GetInfo().SampleDesc;

	// 2Dテクスチャの生成
	HRESULT hr = YsDx.GetDev()->CreateTexture2D(&m_Desc, nullptr, &m_pTexture2D);
	if(FAILED(hr)){
		Release();
		return false;
	}

	// 
	m_strFileName = "";

	// m_pTexture2Dから、各ビューを作成する
	if(CreateViewsFromTexture2D() == false) {
		Release();
		return false;
	}

	return true;
}

bool YsTexture::CreateUAV(int w, int h, DXGI_FORMAT format, UINT arrayCnt, const void* fillData)
{
	Release();

	// 2Dテクスチャの設定
	memset(&m_Desc, 0, sizeof(m_Desc));
	m_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Desc.Format = format;
	m_Desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	m_Desc.Width = (UINT)w;
	m_Desc.Height = (UINT)h;
	m_Desc.CPUAccessFlags = 0;
	m_Desc.MipLevels = 1;
	m_Desc.ArraySize = arrayCnt;
	//m_Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;	// D3D11_RESOURCE_MISC_BUFFER_STRUCTURED
	// m_Desc.StructureByteStride = sizeof(a); // RWStructuredBufferなら構造体のサイズを指定する
	// MSAA(マルチサンプル アンチエイリアシング)は、バックバッファの設定と合わせる
	m_Desc.SampleDesc = YsDx.GetBackBuffer()->GetInfo().SampleDesc;

	// 2Dテクスチャの生成
	HRESULT hr = YsDx.GetDev()->CreateTexture2D(&m_Desc, nullptr, &m_pTexture2D);
	if(FAILED(hr)){
		Release();
		return false;
	}

	// 塗りつぶしデータが指定されているなら、そのデータをテクスチャに流し込む
	if(fillData){
		YsDx.GetDevContext()->UpdateSubresource(m_pTexture2D, 0, nullptr, fillData, 0, 0);
	}

	// 
	m_strFileName = "";

	// m_pTexture2Dから、各ビューを作成する
	if(CreateViewsFromTexture2D() == false) {
		Release();
		return false;
	}


	return true;

}

bool YsTexture::CreateDepth(int w, int h, DXGI_FORMAT format, UINT arrayCnt, const void* fillData)
{
	Release();

	// 2Dテクスチャの設定
	memset(&m_Desc, 0, sizeof(m_Desc));
	m_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Desc.Format = format;
	m_Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	m_Desc.Width = (UINT)w;
	m_Desc.Height = (UINT)h;
	m_Desc.CPUAccessFlags = 0;
	m_Desc.MipLevels = 1;
	m_Desc.ArraySize = arrayCnt;
//	m_Desc.SampleDesc.Count = 1;
//	m_Desc.SampleDesc.Quality = 0;
	// MSAA(マルチサンプル アンチエイリアシング)は、バックバッファの設定と合わせる
	m_Desc.SampleDesc = YsDx.GetDepthStencil()->GetInfo().SampleDesc;

	// 2Dテクスチャの生成
	HRESULT hr = YsDx.GetDev()->CreateTexture2D(&m_Desc, nullptr, &m_pTexture2D);
	if(FAILED(hr)) {
		Release();
		return false;
	}

	// 塗りつぶしデータが指定されているなら、そのデータをテクスチャに流し込む
	if(fillData) {
		YsDx.GetDevContext()->UpdateSubresource(m_pTexture2D, 0, nullptr, fillData, 0, 0);
	}

	// 
	m_strFileName = "";

	// m_pTexture2Dから、各ビューを作成する
	if(CreateViewsFromTexture2D() == false) {
		Release();
		return false;
	}


	return true;

}

bool YsTexture::CreateFromTexture2D(ID3D11Texture2D* pTexture2D)
{
	Release();
	if(pTexture2D == nullptr)return false;

	// 
	m_strFileName = "";

	// 参照カウンタ増加
	pTexture2D->AddRef();
	// アドレスを記憶
	m_pTexture2D = pTexture2D;
	// テクスチャの情報も取得
//	m_pTexture2D->GetDesc(&m_Desc);

	// m_pTexture2Dから、各ビューを作成する
	if(CreateViewsFromTexture2D() == false){
		Release();
		return false;
	}


	return true;
}

bool YsTexture::CreateViewsFromTexture2D()
{
	if(m_pTexture2D == nullptr)return false;

	// テクスチャ本体の情報取得
	m_pTexture2D->GetDesc(&m_Desc);

	//===========================================================
	// RenderTargetViewを作成する
	//===========================================================
	YsSafe_Release(m_pTexRTView);	// 既にあるなら解放
	ClearRTVArray();
	// レンダーターゲットフラグがついてる時
	if(m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET) {
		// 作成するビューの設定
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		rtvDesc.Format = m_Desc.Format;
		// 単品のテクスチャ(通常テクスチャ)の場合
		if(m_Desc.ArraySize == 1){
			if(m_Desc.SampleDesc.Count == 1){
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			}
			else{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			}
		}
		// テクスチャ配列の場合
		else if(m_Desc.ArraySize > 1){
			if(m_Desc.SampleDesc.Count == 1){
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				rtvDesc.Texture2DArray.ArraySize = m_Desc.ArraySize;
				rtvDesc.Texture2DArray.FirstArraySlice = 0;
				rtvDesc.Texture2DArray.MipSlice = 0;
			}
			else{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
				rtvDesc.Texture2DMSArray.ArraySize = m_Desc.ArraySize;
				rtvDesc.Texture2DMSArray.FirstArraySlice = 0;
			}
		}
		// レンダーターゲットビュー作成
		HRESULT hr = YsDx.GetDev()->CreateRenderTargetView(m_pTexture2D, &rtvDesc, &m_pTexRTView);
		if(FAILED(hr)) {
			Release();
			return false;
		}

		// テクスチャ配列なら単品アクセス用にビューも複数つくっとこ(配列の要素１つ１つのビュー　アクセスしやすくするため)
		if(m_Desc.ArraySize > 1){
			m_pTexRTViewArray.resize(m_Desc.ArraySize);
			for(UINT i = 0; i<m_Desc.ArraySize; i++){

				if(m_Desc.SampleDesc.Count == 1){
					rtvDesc.Texture2DArray.ArraySize = 1;			// 単品用なので1
					rtvDesc.Texture2DArray.FirstArraySlice = i;		// ここが配列のインデックスになる
				}
				else{
					rtvDesc.Texture2DMSArray.ArraySize = 1;			// 単品用なので1
					rtvDesc.Texture2DMSArray.FirstArraySlice = i;		// ここが配列のインデックスになる
				}

				HRESULT hr = YsDx.GetDev()->CreateRenderTargetView(m_pTexture2D, &rtvDesc, &m_pTexRTViewArray[i]);
				if(FAILED(hr)) {
					Release();
					return false;
				}
			}
		}
	}

	//===========================================================
	// ShaderResourceViewの情報を作成する
	//===========================================================
	YsSafe_Release(m_pTexView);	// 既にあるなら解放
	ClearSRVArray();
	// シェーダリソースビューフラグがついてる時
	if(m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
		// 作成するビューの設定
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));

		// テクスチャがZバッファの場合は、最適なフォーマットにする
		if(m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL) {
			switch(m_Desc.Format) {
			// 8ビットフォーマットは使用できない？
			case DXGI_FORMAT_R8_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R8_UNORM;
				break;
			// 16ビット
			case DXGI_FORMAT_R16_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R16_UNORM;
				break;
			// 32ビット
			case DXGI_FORMAT_R32_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
			// 24ビット(Zバッファ) + 8ビット(ステンシルバッファ) 
			case DXGI_FORMAT_R24G8_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				break;
			default:
				srvDesc.Format = m_Desc.Format;
				break;
			}
		}
		// Zバッファでない場合は、そのまま同じフォーマットを使用
		else {
			srvDesc.Format = m_Desc.Format;
		}

		// 単品のテクスチャ(通常テクスチャ)の場合
		if(m_Desc.ArraySize == 1){
			if(m_Desc.SampleDesc.Count == 1){
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = m_Desc.MipLevels;
			}
			else{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
				srvDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
			}
		}
		// テクスチャ配列の場合
		else{
			if(m_Desc.SampleDesc.Count == 1){
				if(m_Desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE){
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				}
				else{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				}
				srvDesc.Texture2DArray.MostDetailedMip = 0;
				srvDesc.Texture2DArray.MipLevels = m_Desc.MipLevels;
				srvDesc.Texture2DArray.ArraySize = m_Desc.ArraySize;
				srvDesc.Texture2DArray.FirstArraySlice = 0;
			}
			else{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
				srvDesc.Texture2DMSArray.ArraySize = m_Desc.ArraySize;
				srvDesc.Texture2DMSArray.FirstArraySlice = 0;
			}
		}

		// シェーダリソースビュー作成
		HRESULT hr = YsDx.GetDev()->CreateShaderResourceView(m_pTexture2D, &srvDesc, &m_pTexView);
		if(FAILED(hr)) {
			Release();
			return false;
		}

		// テクスチャ配列なら単品アクセス用にビューも複数つくっとこ(配列の要素１つ１つのビュー　アクセスしやすくするため)
		if(m_Desc.ArraySize > 1){
			m_pTexViewArray.resize(m_Desc.ArraySize);
			for(UINT i = 0; i<m_Desc.ArraySize; i++){

				if(m_Desc.SampleDesc.Count == 1){
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;	// CubeMap時でも単品はこれで。D3D11_SRV_DIMENSION_TEXTURECUBEだとうまくいかない
					srvDesc.Texture2DArray.ArraySize = 1;			// 単品用なので1
					srvDesc.Texture2DArray.FirstArraySlice = i;		// ここが配列のインデックスになる
				}
				else{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					srvDesc.Texture2DMSArray.ArraySize = 1;			// 単品用なので1
					srvDesc.Texture2DMSArray.FirstArraySlice = i;		// ここが配列のインデックスになる
				}

				HRESULT hr = YsDx.GetDev()->CreateShaderResourceView(m_pTexture2D, &srvDesc, &m_pTexViewArray[i]);
				if(FAILED(hr)) {
					Release();
					return false;
				}
			}
		}

	}

	//===========================================================
	// Unordered Access Viewを作成する
	//===========================================================
	YsSafe_Release(m_pTexUAView);
	if(m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) {
		/*
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;// D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = 0;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		*/

		HRESULT hr = YsDx.GetDev()->CreateUnorderedAccessView(m_pTexture2D, nullptr, &m_pTexUAView);
		if(FAILED(hr)) {
			Release();
			return false;
		}
	}

	//===========================================================
	// DepthStencilViewを作成する
	//===========================================================
	YsSafe_Release(m_pTexDepthView);	// 既にあるなら解放
	ClearDSVArray();
	// Zバッファフラグがついてるとき
	if(m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL) {
		// 作成するビューの設定
		CD3D11_DEPTH_STENCIL_VIEW_DESC dpDesc;
		ZeroMemory(&dpDesc, sizeof(dpDesc));

		// テクスチャー作成時に指定したフォーマットと互換性があり、深度ステンシルビューとして指定できるフォーマットを指定する
		switch(m_Desc.Format) {
			// 8ビットフォーマットは使用できない？
			case DXGI_FORMAT_R8_TYPELESS:
				dpDesc.Format = DXGI_FORMAT_R8_UNORM;
				break;
			// 16ビット
			case DXGI_FORMAT_R16_TYPELESS:
				dpDesc.Format = DXGI_FORMAT_D16_UNORM;
				break;
			// 32ビット
			case DXGI_FORMAT_R32_TYPELESS:
				dpDesc.Format = DXGI_FORMAT_D32_FLOAT;
				break;
			// 24ビット(Zバッファ) + 8ビット(ステンシルバッファ) 
			case DXGI_FORMAT_R24G8_TYPELESS:
				dpDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				break;
			default:
				dpDesc.Format = m_Desc.Format;
				break;
		}

		// 単品のテクスチャ(通常テクスチャ)の場合
		if(m_Desc.ArraySize == 1){
			if(m_Desc.SampleDesc.Count == 1){
				dpDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dpDesc.Texture2D.MipSlice = 0;
			}
			else{
				dpDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				dpDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
			}
//			dpDesc.ViewDimension = (m_Desc.SampleDesc.Count == 1 ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DMS);
		}
		// テクスチャ配列の場合
		else{
			if(m_Desc.SampleDesc.Count == 1){
				dpDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				dpDesc.Texture2DArray.ArraySize = m_Desc.ArraySize;
				dpDesc.Texture2DArray.MipSlice = 0;
				dpDesc.Texture2DArray.FirstArraySlice = 0;
			}
			else{
				dpDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
				dpDesc.Texture2DMSArray.ArraySize = m_Desc.ArraySize;
				dpDesc.Texture2DMSArray.FirstArraySlice = 0;
			}
//			dpDesc.ViewDimension = (m_Desc.SampleDesc.Count == 1 ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY);
		}

		// デプスステンシルビュー作成
		HRESULT hr = YsDx.GetDev()->CreateDepthStencilView(m_pTexture2D, &dpDesc, &m_pTexDepthView);
		if(FAILED(hr)) {
			Release();
			return false;
		}

		// テクスチャ配列なら単品アクセス用にビューも複数つくっとこ(配列の要素１つ１つのビュー　アクセスしやすくするため)
		if(m_Desc.ArraySize > 1){
			m_pTexDSViewArray.resize(m_Desc.ArraySize);
			for(UINT i = 0; i<m_Desc.ArraySize; i++){

				if(m_Desc.SampleDesc.Count == 1){
					dpDesc.Texture2DArray.ArraySize = 1;			// 単品用なので1
					dpDesc.Texture2DArray.FirstArraySlice = i;		// ここが配列のインデックスになる
				}
				else{
					dpDesc.Texture2DMSArray.ArraySize = 1;			// 単品用なので1
					dpDesc.Texture2DMSArray.FirstArraySlice = i;		// ここが配列のインデックスになる
				}

				HRESULT hr = YsDx.GetDev()->CreateDepthStencilView(m_pTexture2D, &dpDesc, &m_pTexDSViewArray[i]);
				if(FAILED(hr)) {
					Release();
					return false;
				}
			}
		}
	}

	return true;
}

void YsTexture::SetViewport() const
{
	YsDx.SetViewport((float)m_Desc.Width, (float)m_Desc.Height);
}

void YsTexture::SetTexturePS(UINT SlotNo) const
{
	YsDx.GetDevContext()->PSSetShaderResources(SlotNo, 1, &m_pTexView);
}
void YsTexture::SetTextureVS(UINT SlotNo) const
{
	YsDx.GetDevContext()->VSSetShaderResources(SlotNo, 1, &m_pTexView);
}
void YsTexture::SetTextureGS(UINT SlotNo) const
{
	YsDx.GetDevContext()->GSSetShaderResources(SlotNo, 1, &m_pTexView);
}
void YsTexture::SetTextureCS(UINT SlotNo) const
{
	YsDx.GetDevContext()->CSSetShaderResources(SlotNo, 1, &m_pTexView);
}

void YsTexture::Release()
{
	m_strFileName = "";
	YsSafe_Release(m_pTexture2D);
	YsSafe_Release(m_pTexView);
	YsSafe_Release(m_pTexRTView);
	YsSafe_Release(m_pTexUAView);
	YsSafe_Release(m_pTexDepthView);
	ClearSRVArray();
	ClearRTVArray();
	ClearDSVArray();
	memset(&m_Desc, 0, sizeof(m_Desc));
}

void YsTexture::ClearRT(const YsVec4& ColorRGBA)
{
	if(m_pTexRTView) {
		YsDx.GetDevContext()->ClearRenderTargetView(m_pTexRTView, &ColorRGBA.x);
	}
}

void YsTexture::ClearRTArray(int idx, const YsVec4& ColorRGBA)
{
	if(idx < (int)m_pTexRTViewArray.size()) {
		YsDx.GetDevContext()->ClearRenderTargetView(m_pTexRTViewArray[idx], &ColorRGBA.x);
	}
}

void YsTexture::ClearDepth(bool bDepth, bool bStencil, FLOAT depth, UINT8 stencil) 
{
	if(m_pTexDepthView) {
		UINT flags = 0;
		if(bDepth)flags |= D3D11_CLEAR_DEPTH;
		if(bStencil)flags |= D3D11_CLEAR_STENCIL;

		YsDx.GetDevContext()->ClearDepthStencilView(m_pTexDepthView, flags, depth, stencil);
	}
}

void YsTexture::ClearDepthArray(int idx, bool bDepth, bool bStencil, FLOAT depth, UINT8 stencil)
{
	if (m_pTexDepthView) {
		UINT flags = 0;
		if (bDepth)flags |= D3D11_CLEAR_DEPTH;
		if (bStencil)flags |= D3D11_CLEAR_STENCIL;

		YsDx.GetDevContext()->ClearDepthStencilView(m_pTexDSViewArray[idx], flags, depth, stencil);
	}
}


//=======================================================================
// YsTextureSet
//=======================================================================
void YsTextureSet::LoadTextureSet(const std::string& filename)
{
	YsResourceStorage* pResStg = YsDx.GetResStg();

	// 通常テクスチャ
	MeshTex = pResStg->LoadTexture(filename);

	// 法線マップ
	std::string strFile = YsConvertExtFileName(filename, "normal");
	NormalTex = pResStg->LoadTexture(strFile);

	// その他テクスチャ
	strFile = YsConvertExtFileName(filename, "ext");
	ExtTex = pResStg->LoadTexture(strFile);

	strFile = YsConvertExtFileName(filename, "ext2");
	ExtTex2 = pResStg->LoadTexture(strFile);

	strFile = YsConvertExtFileName(filename, "ext3");
	ExtTex3 = pResStg->LoadTexture(strFile);

	strFile = YsConvertExtFileName(filename, "ext4");
	ExtTex4 = pResStg->LoadTexture(strFile);

	strFile = YsConvertExtFileName(filename, "ext5");
	ExtTex5 = pResStg->LoadTexture(strFile);

	strFile = YsConvertExtFileName(filename, "ext6");
	ExtTex6 = pResStg->LoadTexture(strFile);

	strFile = YsConvertExtFileName(filename, "ext7");
	ExtTex7 = pResStg->LoadTexture(strFile);

	strFile = YsConvertExtFileName(filename, "ext8");
	ExtTex8 = pResStg->LoadTexture(strFile);

}
