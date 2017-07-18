#include "YskLib.h"

using namespace YskLib;


//===================================================
// �u�����h�X�e�[�g
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
		if(bOverrideAlpha){	// �������ݑ��̃A���t�@���㏑������
			m_Desc.RenderTarget[RT_No].DestBlendAlpha = D3D11_BLEND_ZERO;
		}
		else{				// �������ݑ��̃A���t�@�ƍ�������
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
	// �I�u�W�F�N�g�����쐬�̏ꍇ�́A�쐬����
	if(m_pBlendState == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->OMSetBlendState(m_pBlendState, m_BlendFactor, m_SampleMask);
}

//================================================
// �[�x�X�e���V���X�e�[�g�N���X
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

	m_Desc.DepthEnable = TRUE;							// �[�x�e�X�g���g�p����
	m_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
}

// ���݂�Desc�̓��e�����ɍ쐬
bool YsDepthStencilState::Create()
{
	YsSafe_Release(m_pDepthStencilState);

	HRESULT hr = YsDx.GetDev()->CreateDepthStencilState(&m_Desc, &m_pDepthStencilState);
	if(FAILED(hr)) return false;
	return true;
}

void YsDepthStencilState::SetState()
{
	// �I�u�W�F�N�g�����쐬�̏ꍇ�́A�쐬����
	if(m_pDepthStencilState == nullptr){
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->OMSetDepthStencilState(m_pDepthStencilState, m_StencilRef);
}
#include "YskLib.h"

using namespace YskLib;

//===================================================
// �T���v���[�X�e�[�g
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
	// �I�u�W�F�N�g�����쐬�̏ꍇ�́A�쐬����
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->PSSetSamplers(StartSlot, 1, &m_pSampler);
}

void YsSamplerState::SetStateVS(UINT StartSlot)
{
	// �I�u�W�F�N�g�����쐬�̏ꍇ�́A�쐬����
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->VSSetSamplers(StartSlot, 1, &m_pSampler);
}

void YsSamplerState::SetStateGS(UINT StartSlot)
{
	// �I�u�W�F�N�g�����쐬�̏ꍇ�́A�쐬����
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->GSSetSamplers(StartSlot, 1, &m_pSampler);
}

void YsSamplerState::SetStateCS(UINT StartSlot)
{
	// �I�u�W�F�N�g�����쐬�̏ꍇ�́A�쐬����
	if(m_pSampler == nullptr) {
		if(Create() == false)return;
	}

	YsDx.GetDevContext()->CSSetSamplers(StartSlot, 1, &m_pSampler);
}

//================================================
// ���X�^���C�U�X�e�[�g�N���X
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
	m_Desc.FillMode = D3D11_FILL_SOLID;    // �|���S���ʕ`��
	m_Desc.CullMode = D3D11_CULL_BACK;     // ���ʂ�`�悵�Ȃ�
	m_Desc.FrontCounterClockwise = FALSE;   // ���v����\��
	m_Desc.DepthBias = 0;
	m_Desc.DepthBiasClamp = 0;
	m_Desc.SlopeScaledDepthBias = 0;
	m_Desc.DepthClipEnable = TRUE;
	m_Desc.ScissorEnable = FALSE;          // �V�U�[��`����

	// �X���b�v�`�F�[���̃}���`�T���v�����O�̐ݒ�ɂ��킹��
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

// ���݂�Desc�̓��e�����ɃT���v���쐬
bool YsRasterizeState::Create()
{
	YsSafe_Release(m_pRasterize);

	HRESULT hr = YsDx.GetDev()->CreateRasterizerState(&m_Desc, &m_pRasterize);
	if(FAILED(hr))return false;
	return true;
}

void YsRasterizeState::SetState()
{
	// �I�u�W�F�N�g�����쐬�̏ꍇ�́A�쐬����
	if(m_pRasterize == nullptr) {
		if(Create() == false)return;
	}
	YsDx.GetDevContext()->RSSetState(m_pRasterize);
}


//===================================================
// �����_�[�^�[�Q�b�g�A�[�x�X�e���V���f�[�^
//===================================================
void YsRenderTargets::RT(int rtNo, YsTexture& rtTex)
{
	YsSafe_Release(m_pRTs[rtNo]);
	if(rtTex.GetRTTex()) {
		m_pRTs[rtNo] = rtTex.GetRTTex();	// �o�^
		if(m_pRTs[rtNo])m_pRTs[rtNo]->AddRef();				// �Q�ƃJ�E���^����

		// rtNo��0�̂�̃r���[�|�[�g���g�p
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

void YsRenderTargets::RT(int rtNo, ID3D11RenderTargetView* rtView)	// View�w��Ver
{
	YsSafe_Release(m_pRTs[rtNo]);
	if(rtView) {
		m_pRTs[rtNo] = rtView;	// �o�^
		if(m_pRTs[rtNo])m_pRTs[rtNo]->AddRef();				// �Q�ƃJ�E���^����

		// rtNo��0�̂�̃r���[�|�[�g���g�p
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
		m_pDepth = depthTex.GetDepthTex();	// �o�^
		if(m_pDepth)m_pDepth->AddRef();					// �Q�ƃJ�E���^����
	}
}

void YsRenderTargets::Depth(ID3D11DepthStencilView* depthView)	// View�w��Ver
{
	YsSafe_Release(m_pDepth);
	if(depthView) {
		m_pDepth = depthView;	// �o�^
		if(m_pDepth)m_pDepth->AddRef();					// �Q�ƃJ�E���^����
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

	// RT��Depth���Z�b�g
	YsDx.GetDevContext()->OMSetRenderTargets(setRTNum, m_pRTs, m_pDepth);
	// �r���[�|�[�g���؂�ւ���
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
	// ���݂̕������
	Release();

	// RT�̎Q�Ƃ��R�s�[���Q�ƃJ�E���^����
	for(int i = 0; i<D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		m_pRTs[i] = src.m_pRTs[i];
		if(m_pRTs[i])m_pRTs[i]->AddRef();
	}

	// Depth�̎Q�Ƃ��R�s�[���Q�ƃJ�E���^����
	m_pDepth = src.m_pDepth;
	if(m_pDepth)m_pDepth->AddRef();

	// �r���[�|�[�g�R�s�[
	m_VP = src.m_VP;
}
