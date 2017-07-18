#include "YskLib.h"

using namespace YskLib;


//===================================================
// ブレンドステート
//===================================================
void YsBlendState::SetAll_GetState()
{
	YsSafe_Release(m_pBlendState);

	float oldFactor[4];
	UINT oldMask;
	YsDx.GetDevContext()->OMGetBlendState(&m_pBlendState, oldFactor, &oldMask);

	m_pBlendState->GetDesc(&m_Desc);
}

void YsBlendState::Set_NoAlpha(int RT_No, bool bOverrideAlpha)
{
	YsSafe_Release(m_pBlendState);

	if(RT_No < 0) {
		for(int i = 0; i<8; i++) {
			Set_NoAlpha(i, bOverrideAlpha);
		}
	}
	else {
		m_Desc.RenderTarget[RT_No].BlendEnable = TRUE;

		m_Desc.RenderTarget[RT_No].BlendOp = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[RT_No].SrcBlend = D3D11_BLEND_ONE;
		m_Desc.RenderTarget[RT_No].DestBlend = D3D11_BLEND_ZERO;

		m_Desc.RenderTarget[RT_No].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[RT_No].SrcBlendAlpha = D3D11_BLEND_ONE;
		if(bOverrideAlpha){	// 書き込み側のアルファを上書きする
			m_Desc.RenderTarget[RT_No].DestBlendAlpha = D3D11_BLEND_ZERO;
		}
		else{				// 書き込み側のアルファと合成する
			m_Desc.RenderTarget[RT_No].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		}

//		m_Desc.RenderTarget[RT_No].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}

void YsBlendState::Set_Alpha(int RT_No)
{
	YsSafe_Release(m_pBlendState);

	if(RT_No < 0){
		for(int i=0;i<8;i++){
			Set_Alpha(i);
		}
	}
	else{
		m_Desc.RenderTarget[RT_No].BlendEnable = TRUE;

		m_Desc.RenderTarget[RT_No].BlendOp = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[RT_No].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		m_Desc.RenderTarget[RT_No].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		m_Desc.RenderTarget[RT_No].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[RT_No].SrcBlendAlpha = D3D11_BLEND_ONE;
		m_Desc.RenderTarget[RT_No].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

//		m_Desc.RenderTarget[RT_No].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}
void YsBlendState::Set_Add(int RT_No)
{
	YsSafe_Release(m_pBlendState);

	if(RT_No < 0) {
		for(int i = 0; i<8; i++) {
			Set_Add(i);
		}
	} else {
		m_Desc.RenderTarget[RT_No].BlendEnable = TRUE;

		m_Desc.RenderTarget[RT_No].BlendOp = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[RT_No].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		m_Desc.RenderTarget[RT_No].DestBlend = D3D11_BLEND_ONE;

		m_Desc.RenderTarget[RT_No].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_Desc.RenderTarget[RT_No].SrcBlendAlpha = D3D11_BLEND_ONE;
		m_Desc.RenderTarget[RT_No].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

//		m_Desc.RenderTarget[RT_No].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}

void YsBlendState::Set_WriteMask(int RT_No, bool r, bool g, bool b, bool a)
{
	YsSafe_Release(m_pBlendState);

	if(RT_No < 0) {
		for(int i = 0; i<8; i++) {
			Set_WriteMask(i, r, g, b, a);
		}
	}
	else{
		m_Desc.RenderTarget[RT_No].RenderTargetWriteMask = 0;
		if(r)m_Desc.RenderTarget[RT_No].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
		if(g)m_Desc.RenderTarget[RT_No].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
		if(b)m_Desc.RenderTarget[RT_No].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
		if(a)m_Desc.RenderTarget[RT_No].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
	}
}

bool YsBlendState::Create()
{
	YsSafe_Release(m_pBlendState);

	HRESULT hr = YsDx.GetDev()->CreateBlendState(&m_Desc, &m_pBlendState);
	if(FAILED(hr))return false;
	return true;
}

void YsBlendState::SetState()
{
	// オブジェクトが未作成の場合は、作成する
	if(m_pBlendState == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->OMSetBlendState(m_pBlendState, m_BlendFactor, m_SampleMask);
}

//================================================
// 深度ステンシルステートクラス
//================================================
void YsDepthStencilState::SetAll_GetState()
{
	YsSafe_Release(m_pDepthStencilState);

	UINT StencilRef;
	YsDx.GetDevContext()->OMGetDepthStencilState(&m_pDepthStencilState, &StencilRef);

	m_pDepthStencilState->GetDesc(&m_Desc);
}

void YsDepthStencilState::SetAll_Standard()
{
	YsSafe_Release(m_pDepthStencilState);
	ZeroMemory(&m_Desc, sizeof(m_Desc));

	m_Desc.DepthEnable = TRUE;							// 深度テストを使用する
	m_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
}

// 現在のDescの内容を元に作成
bool YsDepthStencilState::Create()
{
	YsSafe_Release(m_pDepthStencilState);

	HRESULT hr = YsDx.GetDev()->CreateDepthStencilState(&m_Desc, &m_pDepthStencilState);
	if(FAILED(hr)) return false;
	return true;
}

void YsDepthStencilState::SetState()
{
	// オブジェクトが未作成の場合は、作成する
	if(m_pDepthStencilState == nullptr){
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->OMSetDepthStencilState(m_pDepthStencilState, m_StencilRef);
}
#include "YskLib.h"

using namespace YskLib;

//===================================================
// サンプラーステート
//===================================================
void YsSamplerState::SetAll_GetStatePS(UINT StartSlot)
{
	YsSafe_Release(m_pSampler);
	YsDx.GetDevContext()->PSGetSamplers(StartSlot, 1, &m_pSampler);
	m_pSampler->GetDesc(&m_Desc);
}
void YsSamplerState::SetAll_GetStateVS(UINT StartSlot)
{
	YsSafe_Release(m_pSampler);
	YsDx.GetDevContext()->VSGetSamplers(StartSlot, 1, &m_pSampler);
	m_pSampler->GetDesc(&m_Desc);
}
void YsSamplerState::SetAll_GetStateGS(UINT StartSlot)
{
	YsSafe_Release(m_pSampler);
	YsDx.GetDevContext()->GSGetSamplers(StartSlot, 1, &m_pSampler);
	m_pSampler->GetDesc(&m_Desc);
}
void YsSamplerState::SetAll_GetStateCS(UINT StartSlot)
{
	YsSafe_Release(m_pSampler);
	YsDx.GetDevContext()->CSGetSamplers(StartSlot, 1, &m_pSampler);
	m_pSampler->GetDesc(&m_Desc);
}

void YsSamplerState::SetAll_Standard()
{
	YsSafe_Release(m_pSampler);
	ZeroMemory(&m_Desc, sizeof(m_Desc));
	m_Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	m_Desc.MipLODBias = -1.0f;
	m_Desc.MipLODBias = 0;
	m_Desc.MaxAnisotropy = 16;
	m_Desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	m_Desc.BorderColor[0] = m_Desc.BorderColor[1] = m_Desc.BorderColor[2] = m_Desc.BorderColor[3] = 0;
	m_Desc.MinLOD = -D3D11_FLOAT32_MAX;
	m_Desc.MaxLOD = D3D11_FLOAT32_MAX;
}

void YsSamplerState::Set_Wrap() 
{
	YsSafe_Release(m_pSampler);
	m_Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
}

void YsSamplerState::Set_Clamp()
{
	YsSafe_Release(m_pSampler);
	m_Desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_Desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_Desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
}

void YsSamplerState::Set_Filter(D3D11_FILTER filter, int maxAnisotropy)
{
	YsSafe_Release(m_pSampler);
	m_Desc.Filter = filter;
	m_Desc.MaxAnisotropy = maxAnisotropy;
}

void YsSamplerState::Set_Filter_Linear()
{
	YsSafe_Release(m_pSampler);
	m_Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
}

void YsSamplerState::Set_Filter_Point()
{
	YsSafe_Release(m_pSampler);
	m_Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
}

void YsSamplerState::Set_Filter_Anisotropy(UINT MaxAnisotropy)
{
	YsSafe_Release(m_pSampler);
	m_Desc.Filter = D3D11_FILTER_ANISOTROPIC;
	m_Desc.MaxAnisotropy = MaxAnisotropy;
}

void YsSamplerState::Set_ComparisonMode(D3D11_COMPARISON_FUNC compFunc)
{
	YsSafe_Release(m_pSampler);
	m_Desc.ComparisonFunc = compFunc;
	m_Desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
}

bool YsSamplerState::Create()
{
	YsSafe_Release(m_pSampler);

	HRESULT hr = YsDx.GetDev()->CreateSamplerState(&m_Desc, &m_pSampler);
	if(FAILED(hr))return false;
	return true;
}

void YsSamplerState::SetStatePS(UINT StartSlot)
{
	// オブジェクトが未作成の場合は、作成する
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->PSSetSamplers(StartSlot, 1, &m_pSampler);
}

void YsSamplerState::SetStateVS(UINT StartSlot)
{
	// オブジェクトが未作成の場合は、作成する
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->VSSetSamplers(StartSlot, 1, &m_pSampler);
}

void YsSamplerState::SetStateGS(UINT StartSlot)
{
	// オブジェクトが未作成の場合は、作成する
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->GSSetSamplers(StartSlot, 1, &m_pSampler);
}

void YsSamplerState::SetStateCS(UINT StartSlot)
{
	// オブジェクトが未作成の場合は、作成する
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->CSSetSamplers(StartSlot, 1, &m_pSampler);
}

//================================================
// ラスタライザステートクラス
//================================================
void YsRasterizeState::SetAll_GetState() 
{
	YsSafe_Release(m_pRasterize);
	YsDx.GetDevContext()->RSGetState(&m_pRasterize);
	m_pRasterize->GetDesc(&m_Desc);
}

void YsRasterizeState::SetAll_Standard() 
{
	YsSafe_Release(m_pRasterize);
	ZeroMemory(&m_Desc, sizeof(m_Desc));
	m_Desc.FillMode = D3D11_FILL_SOLID;    // ポリゴン面描画
	m_Desc.CullMode = D3D11_CULL_BACK;     // 裏面を描画しない
	m_Desc.FrontCounterClockwise = FALSE;   // 時計回りを表面
	m_Desc.DepthBias = 0;
	m_Desc.DepthBiasClamp = 0;
	m_Desc.SlopeScaledDepthBias = 0;
	m_Desc.DepthClipEnable = TRUE;
	m_Desc.ScissorEnable = FALSE;          // シザー矩形無効

	// スワップチェーンのマルチサンプリングの設定にあわせる
	DXGI_SWAP_CHAIN_DESC swapDesc;
	YsDx.GetSwapChain()->GetDesc(&swapDesc);
	if(swapDesc.SampleDesc.Count > 1) {
		m_Desc.MultisampleEnable = TRUE;
	}
	else {
		m_Desc.MultisampleEnable = FALSE;
	}

	m_Desc.AntialiasedLineEnable = FALSE;

}

// 現在のDescの内容を元にサンプラ作成
bool YsRasterizeState::Create()
{
	YsSafe_Release(m_pRasterize);

	HRESULT hr = YsDx.GetDev()->CreateRasterizerState(&m_Desc, &m_pRasterize);
	if(FAILED(hr))return false;
	return true;
}

void YsRasterizeState::SetState()
{
	// オブジェクトが未作成の場合は、作成する
	if(m_pRasterize == nullptr) {
		if(Create() == false)return;
	}
	YsDx.GetDevContext()->RSSetState(m_pRasterize);
}


//===================================================
// レンダーターゲット、深度ステンシルデータ
//===================================================
void YsRenderTargets::RT(int rtNo, YsTexture& rtTex)
{
	YsSafe_Release(m_pRTs[rtNo]);
	if(rtTex.GetRTTex()) {
		m_pRTs[rtNo] = rtTex.GetRTTex();	// 登録
		if(m_pRTs[rtNo])m_pRTs[rtNo]->AddRef();				// 参照カウンタ増加

		// rtNoが0のやつのビューポートを使用
		if(rtNo == 0) {
			m_VP.Width = (float)rtTex.GetInfo().Width;
			m_VP.Height = (float)rtTex.GetInfo().Height;
			m_VP.MinDepth = 0.0f;
			m_VP.MaxDepth = 1.0f;
			m_VP.TopLeftX = 0;
			m_VP.TopLeftY = 0;
		}
	}
}

void YsRenderTargets::RT(int rtNo, ID3D11RenderTargetView* rtView)	// View指定Ver
{
	YsSafe_Release(m_pRTs[rtNo]);
	if(rtView) {
		m_pRTs[rtNo] = rtView;	// 登録
		if(m_pRTs[rtNo])m_pRTs[rtNo]->AddRef();				// 参照カウンタ増加

		// rtNoが0のやつのビューポートを使用
		if(rtNo == 0) {
			ID3D11Resource* res;
			rtView->GetResource(&res);
			ID3D11Texture2D* tex2D;
			HRESULT hr = res->QueryInterface(IID_ID3D11Texture2D, (void **)&tex2D);
			res->Release();
			if(SUCCEEDED(hr)) {
				D3D11_TEXTURE2D_DESC desc;
				tex2D->GetDesc(&desc);
				m_VP.Width = (float)desc.Width;
				m_VP.Height = (float)desc.Height;
				m_VP.MinDepth = 0.0f;
				m_VP.MaxDepth = 1.0f;
				m_VP.TopLeftX = 0;
				m_VP.TopLeftY = 0;

				tex2D->Release();
			}
		}
	}

}

void YsRenderTargets::RT(int rtNo, YsRenderTargets& rtdata)
{
	RT(rtNo, rtdata.m_pRTs[rtNo]);
}

bool YsRenderTargets::GetRTDesc(int rtNo, D3D11_TEXTURE2D_DESC& outDesc)
{
	if(m_pRTs[rtNo] == nullptr)return false;

	ID3D11Resource* res;
	m_pRTs[rtNo]->GetResource(&res);
	ID3D11Texture2D* tex2D;
	HRESULT hr = res->QueryInterface(IID_ID3D11Texture2D, (void **)&tex2D);
	res->Release();
	if(SUCCEEDED(hr)) {
		tex2D->GetDesc(&outDesc);
		tex2D->Release();
		return true;
	}
	return false;
}

void YsRenderTargets::Depth(YsTexture& depthTex)
{
	YsSafe_Release(m_pDepth);
	if(depthTex.GetDepthTex()) {
		m_pDepth = depthTex.GetDepthTex();	// 登録
		if(m_pDepth)m_pDepth->AddRef();					// 参照カウンタ増加
	}
}

void YsRenderTargets::Depth(ID3D11DepthStencilView* depthView)	// View指定Ver
{
	YsSafe_Release(m_pDepth);
	if(depthView) {
		m_pDepth = depthView;	// 登録
		if(m_pDepth)m_pDepth->AddRef();					// 参照カウンタ増加
	}

}

void YsRenderTargets::Depth(YsRenderTargets& rtdata)
{
	Depth(rtdata.m_pDepth);
}

void YsRenderTargets::GetNowAll() {
	Release();

	YsDx.GetDevContext()->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_pRTs, &m_pDepth);

	UINT pNumVierports = 1;
	YsDx.GetDevContext()->RSGetViewports(&pNumVierports, &m_VP);
}

void YsRenderTargets::GetNowRTs() {
	for(int i = 0; i<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		YsSafe_Release(m_pRTs[i]);
	}

	YsDx.GetDevContext()->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, m_pRTs, NULL);

	UINT pNumVierports = 1;
	YsDx.GetDevContext()->RSGetViewports(&pNumVierports, &m_VP);
}

void YsRenderTargets::GetNowDepth()
{
	YsSafe_Release(m_pDepth);
	YsDx.GetDevContext()->OMGetRenderTargets(0, NULL, &m_pDepth);
}

void YsRenderTargets::SetToDevice(int setRTNum) {
	if (setRTNum < 0)setRTNum = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;

	// RTとDepthをセット
	YsDx.GetDevContext()->OMSetRenderTargets(setRTNum, m_pRTs, m_pDepth);
	// ビューポートも切り替える
	YsDx.GetDevContext()->RSSetViewports(1, &m_VP);
}

void YsRenderTargets::Release() {
	for(int i = 0; i<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		YsSafe_Release(m_pRTs[i]);
	}
	YsSafe_Release(m_pDepth);
}

void YsRenderTargets::operator = (const YsRenderTargets& src)
{
	// 現在の物を解放
	Release();

	// RTの参照をコピーし参照カウンタ増加
	for(int i = 0; i<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		m_pRTs[i] = src.m_pRTs[i];
		if(m_pRTs[i])m_pRTs[i]->AddRef();
	}

	// Depthの参照をコピーし参照カウンタ増加
	m_pDepth = src.m_pDepth;
	if(m_pDepth)m_pDepth->AddRef();

	// ビューポートコピー
	m_VP = src.m_VP;
}
