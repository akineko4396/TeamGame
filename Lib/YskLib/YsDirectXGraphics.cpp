#include "YskLib.h"

using namespace YskLib;

// 初期化
bool YsDirectXGraphics::Init(HWND hWnd, int w, int h, YsResourceStorage* resStg, MSAASetting msaa, std::string* errMsg)
{
	m_pResStg = resStg;

	HRESULT hr;

	//=========================================================
	// Direct3D9デバイスも作成しておく
	//  ※Xファイル読み込みなどで使用するため
	//=========================================================
	if(CreateD3D9Device(hWnd, errMsg) == false){
		Release();
		return false;
	}


	//=====================================================
	// ファクトリー作成(ビデオ グラフィックの設定の列挙や指定に使用されるオブジェクト)
	//=====================================================
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast< void** >(&m_pGIFactory));
	if ( FAILED( hr ) ){
		if(errMsg)*errMsg = "ファクトリー作成失敗";

		Release();
		return false;
	}
	// ALT+Enterでフルスクリーンを許可する
	if(FAILED(m_pGIFactory->MakeWindowAssociation(hWnd, 0))){
		if(errMsg)*errMsg = "MakeWindowAssociation失敗";

		Release();
		return false;
	}

	//=====================================================
	// アダプター作成(アダプター (ビデオ カード) を列挙)
	//=====================================================
	hr = m_pGIFactory->EnumAdapters(0, &m_pGIAdapter);
	if ( FAILED( hr ) ) {
		if(errMsg)*errMsg = "アダプター取得失敗";

		Release();
		return false;
	}

	//=====================================================
	//デバイスの生成
	//=====================================================
	UINT creationFlags = 0;

	#ifdef DEBUG_DIRECT3D11
	// Direct3Dのデバッグを有効にする(重いが細かいエラーがわかる)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};
	const int featureNum = 6;

	// デバイスとでデバイスコンテキストを作成
	hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				creationFlags,
				featureLevels,
				featureNum,
				D3D11_SDK_VERSION,
				&m_pDevice,
				nullptr,
				&m_pDeviceContext);
	if FAILED( hr )
	{
		if(errMsg)*errMsg = "DirectX11デバイス作成失敗";

		Release();
		return false;
	}

	// 使用可能なMSAAを取得
	UINT maxMsaa = 1;
	for(UINT i = 0; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i++) {
		UINT Quality;
		if(SUCCEEDED(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality))) {
			if(0 < Quality) {
				maxMsaa = i;
//				sampleDesc.Count = i;
//				sampleDesc.Quality = Quality - 1;
			}
		}
	}
	// MSAA用データ
	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = msaa;
	sampleDesc.Quality = 0;
	if(sampleDesc.Count > maxMsaa) {
		sampleDesc.Count = maxMsaa;
	}

	//=====================================================
	// スワップチェイン作成
	//=====================================================
	m_DXGISwapChainDesc.BufferDesc.Width = w;
	m_DXGISwapChainDesc.BufferDesc.Height = h;
	m_DXGISwapChainDesc.BufferDesc.RefreshRate.Numerator  = 0;
	m_DXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_DXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_DXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_DXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;// DXGI_MODE_SCALING_CENTERED;	// DXGI_MODE_SCALING_UNSPECIFIED
//	m_DXGISwapChainDesc.SampleDesc.Count = 1;
//	m_DXGISwapChainDesc.SampleDesc.Quality = 0;
	m_DXGISwapChainDesc.SampleDesc = sampleDesc;
	m_DXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	m_DXGISwapChainDesc.BufferCount = 2;
	m_DXGISwapChainDesc.OutputWindow = hWnd;
	m_DXGISwapChainDesc.Windowed = TRUE;
	m_DXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_DXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if(FAILED(m_pGIFactory->CreateSwapChain(m_pDevice, &m_DXGISwapChainDesc, &m_pGISwapChain))){
		if(errMsg)*errMsg = "スワップチェイン作成失敗";

		Release();
		return false;
	}

	// スワップチェインからバックバッファ取得
	ID3D11Texture2D* pBackBuffer;
	if(FAILED(m_pGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
		if(errMsg)*errMsg = "バックバッファ取得失敗";

		Release();
		return false;
	}
	// バックバッファ用のレンダーターゲットビュー作成
	m_texBackBuffer = SPtr<YsTexture>(new YsTexture());
	if(m_texBackBuffer->CreateFromTexture2D(pBackBuffer) == false){
		if(errMsg)*errMsg = "バックバッファ用View作成失敗";

		Release();
		return false;
	}
	pBackBuffer->Release();

	//=========================================================
	// Zバッファ・ステンシルバッファ作成
	//=========================================================
	D3D11_TEXTURE2D_DESC descDS;
	ID3D11Texture2D* pDepthBuffer = nullptr;	// Zバッファ

	// スワップチェーンの設定を取得する
	DXGI_SWAP_CHAIN_DESC chainDesc;
	hr = m_pGISwapChain->GetDesc( &chainDesc );
	if( FAILED( hr ) ){
		if(errMsg)*errMsg = "スワップチェーンの情報取得失敗";
		Release();
		return false;
	}
	// パラメータ設定
	ZeroMemory( &descDS, sizeof( D3D11_TEXTURE2D_DESC ) );
	descDS.Width              = chainDesc.BufferDesc.Width;   // バックバッファと同じにする。
	descDS.Height             = chainDesc.BufferDesc.Height;  // バックバッファと同じにする。
	descDS.MipLevels          = 1;                            // ミップマップを作成しない
	descDS.ArraySize          = 1;                            // テクスチャーの配列数
	descDS.Format             = DXGI_FORMAT_R24G8_TYPELESS;   // フォーマット
	descDS.SampleDesc.Count   = chainDesc.SampleDesc.Count;   // バックバッファと同じにする。
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality; // バックバッファと同じにする。
	descDS.Usage              = D3D11_USAGE_DEFAULT;          // GPU による読み取りおよび書き込みアクセスを必要とするリソース。
	descDS.BindFlags          = D3D11_BIND_DEPTH_STENCIL |    // 深度ステンシルバッファとして作成する
								D3D11_BIND_SHADER_RESOURCE;   // シェーダーリソースビューとして作成する
	descDS.CPUAccessFlags     = 0;                            // CPU アクセスが不要。
	descDS.MiscFlags          = 0;                            // その他のフラグも設定しない。
	// 深度バッファ用のテクスチャー作成
	hr = m_pDevice->CreateTexture2D( &descDS, nullptr, &pDepthBuffer );
	if( FAILED( hr ) ){
		if(errMsg)*errMsg = "Zバッファ生成失敗";

		Release();
		return false;
	}

	// そのテクスチャ(Zバッファ)用の深度ステンシルビュー作成
	m_texDepthStencil = SPtr<YsTexture>(new YsTexture());
	if(m_texDepthStencil->CreateFromTexture2D(pDepthBuffer) == false){
		if(errMsg)*errMsg = "深度ステンシル用View作成失敗";

		Release();
		return false;
	}
	pDepthBuffer->Release();


	//=========================================================
	// バックバッファ、深度バッファをRT、Depthとしてデバイスコンテキストへセットする
	//=========================================================
	{
		ID3D11RenderTargetView* rtv = m_texBackBuffer->GetRTTex();
		YsDx.GetDevContext()->OMSetRenderTargets(1, &rtv, m_texDepthStencil->GetDepthTex());
	}

	//=========================================================
	// ビューポートの設定
	//=========================================================
	SetViewport((float)w, (float)h);

	//=========================================================
	// 標準的なステートの設定
	//=========================================================
	SetDefaultStates();

	//=========================================================
	// スプライトバッチ初期化
	//=========================================================
	m_Sprite.Init(m_pDeviceContext);

	//=========================================================
	// 1x1 白テクスチャ作成
	//=========================================================
	{
		// 0xAABBGGRR
		DWORD col = 0xFFFFFFFF;
		m_texWhite = std::make_shared<YsTexture>();
		m_texWhite->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &col);
	}

	//=========================================================
	// 1x1 デフォルト用法線マップ作成
	//=========================================================
	{
		DWORD col = fRGBA(0.5f, 0.5f, 1.0f, 0);//0x00FF8080;
		m_texNormal = std::make_shared<YsTexture>();
		m_texNormal->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &col);
	}

	//=========================================================
	// デフォルト用 トゥーンテクスチャ読み込み
	//=========================================================
	m_texToon = std::make_shared<YsTexture>();
	m_texToon->LoadTexture("data/Texture/toon.png");

	// 
	ZeroMemory(m_srvReset, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);

	// DrawQuad用ポリゴン作成
	m_polyQuad.Create(nullptr, 4, YsVertex_Pos_UV::GetVertexTypeData(), true);

	return true;
}

void YsDirectXGraphics::Release()
{
	// 解放
	m_polyQuad.Release();
	m_pResStg = nullptr;
	m_texWhite = nullptr;
	m_texNormal = nullptr;
	m_texToon = nullptr;
	m_Sprite.Release();
	m_texBackBuffer = nullptr;
	m_texDepthStencil = nullptr;
	YsSafe_Release(m_pGISwapChain);
	YsSafe_Release(m_pDeviceContext);
	YsSafe_Release(m_pDevice);
	YsSafe_Release(m_pGIAdapter);
	YsSafe_Release(m_pGIFactory);

	YsSafe_Release(m_pD3D9Device);
	YsSafe_Release(m_pD3D9);
}

void YsDirectXGraphics::SetDefaultStates()
{
	// ラスタライザステート設定
	YsRasterizeState rs;
	rs.SetAll_Standard();
	rs.SetState();

	// デプスステンシルステート設定
	YsDepthStencilState dss;
	dss.SetAll_Standard();
	dss.SetState();

	// ブレンドステート設定
	YsBlendState bs;
	bs.Set_Alpha(-1);
	bs.SetState();

	// サンプラステート設定
	YsSamplerState ss;
	ss.SetAll_Standard();
	ss.SetStatePS();
}

bool YsDirectXGraphics::ResizeRezolution(int w, int h)
{
	//===================================================
	// バッファ解放
	//===================================================
	m_texBackBuffer = nullptr;
	m_texDepthStencil = nullptr;

	//===================================================
	// リサイズ
	//===================================================
	m_pGISwapChain->ResizeBuffers(m_DXGISwapChainDesc.BufferCount, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	m_pGISwapChain->GetDesc(&m_DXGISwapChainDesc);

	//===================================================
	// バッファ作成
	//===================================================
	// スワップチェインからバックバッファ取得
	ID3D11Texture2D* pBackBuffer;
	if(FAILED(m_pGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
		Release();
		return false;
	}
	// バックバッファ用のレンダーターゲットビュー作成
	m_texBackBuffer = SPtr<YsTexture>(new YsTexture());
	if(m_texBackBuffer->CreateFromTexture2D(pBackBuffer) == false){
		Release();
		return false;
	}
	pBackBuffer->Release();


	//=========================================================
	// Zバッファ・ステンシルバッファ作成
	//=========================================================
	D3D11_TEXTURE2D_DESC descDS;
	ID3D11Texture2D* pDepthBuffer = nullptr;	// Zバッファ

	// スワップチェーンの設定を取得する
	DXGI_SWAP_CHAIN_DESC chainDesc;
	HRESULT hr = m_pGISwapChain->GetDesc( &chainDesc );
	if( FAILED( hr ) ){
		Release();
		return false;
	}
	// パラメータ設定
	ZeroMemory( &descDS, sizeof( D3D11_TEXTURE2D_DESC ) );
	descDS.Width              = chainDesc.BufferDesc.Width;   // バックバッファと同じにする。
	descDS.Height             = chainDesc.BufferDesc.Height;  // バックバッファと同じにする。
	descDS.MipLevels          = 1;                            // ミップマップを作成しない
	descDS.ArraySize          = 1;                            // テクスチャーの配列数
	descDS.Format             = DXGI_FORMAT_R24G8_TYPELESS;   // フォーマット
	descDS.SampleDesc.Count   = chainDesc.SampleDesc.Count;   // バックバッファと同じにする。
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality; // バックバッファと同じにする。
	descDS.Usage              = D3D11_USAGE_DEFAULT;          // GPU による読み取りおよび書き込みアクセスを必要とするリソース。
	descDS.BindFlags          = D3D11_BIND_DEPTH_STENCIL |    // 深度ステンシルバッファとして作成する
								D3D11_BIND_SHADER_RESOURCE;   // シェーダーリソースビューとして作成する
	descDS.CPUAccessFlags     = 0;                            // CPU アクセスが不要。
	descDS.MiscFlags          = 0;                            // その他のフラグも設定しない。
	// 深度バッファ用のテクスチャー作成
	hr = m_pDevice->CreateTexture2D( &descDS, nullptr, &pDepthBuffer );
	if( FAILED( hr ) ){
		Release();
		return false;
	}

	// そのテクスチャ(Zバッファ)用の深度ステンシルビュー作成
	m_texDepthStencil = SPtr<YsTexture>(new YsTexture());
	if(m_texDepthStencil->CreateFromTexture2D(pDepthBuffer) == false){
		Release();
		return false;
	}
	pDepthBuffer->Release();

	//=========================================================
	// バックバッファ、深度バッファをRT、Depthとしてデバイスコンテキストへセットする
	//=========================================================
	{
		ID3D11RenderTargetView* rtv = m_texBackBuffer->GetRTTex();
		YsDx.GetDevContext()->OMSetRenderTargets(1, &rtv, m_texDepthStencil->GetDepthTex());
	}

	//=========================================================
	// ビューポートの設定
	//=========================================================
	SetViewport((float)w, (float)h);

	return true;

}

void YsDirectXGraphics::SetViewport(float w, float h)
{
	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = w != 0 ? w : m_texBackBuffer->GetInfo().Width;
	vp.Height = h != 0 ? h : m_texBackBuffer->GetInfo().Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
}

void YsDirectXGraphics::RS_CullMode(D3D11_CULL_MODE mode)
{
	YsRasterizeState rs;
	rs.SetAll_Standard();
	rs.Set_CullMode(mode);
	rs.SetState();
}

void YsDirectXGraphics::DS_ZSetting(bool ZEnable, bool ZWriteEnable)
{
	YsDepthStencilState ds;
	ds.SetAll_GetState();
	ds.Set_ZEnable(ZEnable);
	ds.Set_ZWriteEnable(ZWriteEnable);
	ds.SetState();
}

void YsDirectXGraphics::DrawQuad(int x, int y, int w, int h)
{
	UINT pNumVierports = 1;
	D3D11_VIEWPORT vp;
	YsDx.GetDevContext()->RSGetViewports(&pNumVierports, &vp);

	//==================================================
	// ピクセル座標系 → 射影座標系へ変換
	//==================================================
	float fConvX = (vp.Width * 0.5f);
	float fConvY = (vp.Height * 0.5f);

	float tx = x / fConvX - 1.0f;
	float ty = y / fConvY - 1.0f;

	float tx2 = (x + w) / fConvX - 1.0f;
	float ty2 = (y + h) / fConvY - 1.0f;

	//==================================================
	//四角形データ書き込み
	//==================================================
	// 座標は射影座標系
	YsVertex_Pos_UV vertices[] = {
		{ YsVec3(tx, -ty2, 0), YsVec2(0, 1) },
		{ YsVec3(tx, -ty, 0), YsVec2(0, 0) },
		{ YsVec3(tx2, -ty2, 0), YsVec2(1, 1) },
		{ YsVec3(tx2, -ty, 0), YsVec2(1, 0) }
	};

	//==================================================
	// 頂点書き込み
	//==================================================
	m_polyQuad.WriteVertexData(vertices, 4);

	//==================================================
	// 描画
	//==================================================
	// 描画用データをセット
	m_polyQuad.SetDrawData();
	// 描画
	m_polyQuad.Draw();
}

void YsDirectXGraphics::RemoveTextureVS(int slotNo)
{
	if(slotNo >= 0){
		ID3D11ShaderResourceView* srv = nullptr;
		m_pDeviceContext->VSSetShaderResources(slotNo, 1, &srv);
	}
	else{
		m_pDeviceContext->VSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, m_srvReset);
	}
}

void YsDirectXGraphics::RemoveTexturePS(int slotNo)
{
	if(slotNo >= 0){
		ID3D11ShaderResourceView* srv = nullptr;
		m_pDeviceContext->PSSetShaderResources(slotNo, 1, &srv);
	}
	else{
		m_pDeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, m_srvReset);
	}
}

void YsDirectXGraphics::RemoveTextureGS(int slotNo)
{
	if(slotNo >= 0){
		ID3D11ShaderResourceView* srv = nullptr;
		m_pDeviceContext->GSSetShaderResources(slotNo, 1, &srv);
	}
	else{
		m_pDeviceContext->GSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, m_srvReset);
	}
}

void YsDirectXGraphics::RemoveTextureCS(int slotNo)
{
	if(slotNo >= 0){
		ID3D11ShaderResourceView* srv = nullptr;
		m_pDeviceContext->CSSetShaderResources(slotNo, 1, &srv);
	}
	else{
		m_pDeviceContext->CSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, m_srvReset);
	}
}

HRESULT YsDirectXGraphics::Present(UINT SyncInterval, UINT Flags)
{
	return m_pGISwapChain->Present(SyncInterval, Flags);
}



bool YsDirectXGraphics::CreateD3D9Device(HWND hWnd, std::string* errMsg)
{
	YsSafe_Release(m_pD3D9);
	YsSafe_Release(m_pD3D9Device);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	// DirectX9
	//  Xファイルの読み込みとかで使用するためにデバイス作成しとく
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//=======================================================
	// D3Dオブジェクト作成
	//=======================================================
	if(nullptr == (m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION))) {
		if(errMsg)*errMsg = "Direct3D9作成失敗";

		Release();
		return false;
	}

	//=======================================================
	// 現在の画面モードを取得
	//=======================================================
	D3DDISPLAYMODE d3ddm;
	if(FAILED(m_pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		Release();
		return false;
	}

	//=======================================================
	// デバイス作成
	//=======================================================
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = 0;
	d3dpp.BackBufferHeight = 0;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	// スワップ エフェクト
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;// ウィンドウ クライアント領域を直ちに更新します
	d3dpp.hDeviceWindow = hWnd;
	// Z バッファの自動作成
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// 24bitZバッファ

	//デバイスの作成 - T&L HAL
	if(FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, &d3dpp, &m_pD3D9Device))) {
		//失敗したのでHALで試行
		if(FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, &d3dpp, &m_pD3D9Device))) {
			//失敗したのでREFで試行
			if(FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, &d3dpp, &m_pD3D9Device))) {
				//結局失敗
				if(errMsg)*errMsg = "Direct3D9デバイス作成失敗";

				Release();
				return false;
			}
		}
	}

	return true;
}

