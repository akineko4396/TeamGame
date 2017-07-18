#include "YskLib.h"

using namespace YskLib;

// ������
bool YsDirectXGraphics::Init(HWND hWnd, int w, int h, YsResourceStorage* resStg, MSAASetting msaa, std::string* errMsg)
{
	m_pResStg = resStg;

	HRESULT hr;

	//=========================================================
	// Direct3D9�f�o�C�X���쐬���Ă���
	//  ��X�t�@�C���ǂݍ��݂ȂǂŎg�p���邽��
	//=========================================================
	if(CreateD3D9Device(hWnd, errMsg) == false){
		Release();
		return false;
	}


	//=====================================================
	// �t�@�N�g���[�쐬(�r�f�I �O���t�B�b�N�̐ݒ�̗񋓂�w��Ɏg�p�����I�u�W�F�N�g)
	//=====================================================
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast< void** >(&m_pGIFactory));
	if ( FAILED( hr ) ){
		if(errMsg)*errMsg = "�t�@�N�g���[�쐬���s";

		Release();
		return false;
	}
	// ALT+Enter�Ńt���X�N���[����������
	if(FAILED(m_pGIFactory->MakeWindowAssociation(hWnd, 0))){
		if(errMsg)*errMsg = "MakeWindowAssociation���s";

		Release();
		return false;
	}

	//=====================================================
	// �A�_�v�^�[�쐬(�A�_�v�^�[ (�r�f�I �J�[�h) ���)
	//=====================================================
	hr = m_pGIFactory->EnumAdapters(0, &m_pGIAdapter);
	if ( FAILED( hr ) ) {
		if(errMsg)*errMsg = "�A�_�v�^�[�擾���s";

		Release();
		return false;
	}

	//=====================================================
	//�f�o�C�X�̐���
	//=====================================================
	UINT creationFlags = 0;

	#ifdef DEBUG_DIRECT3D11
	// Direct3D�̃f�o�b�O��L���ɂ���(�d�����ׂ����G���[���킩��)
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

	// �f�o�C�X�ƂŃf�o�C�X�R���e�L�X�g���쐬
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
		if(errMsg)*errMsg = "DirectX11�f�o�C�X�쐬���s";

		Release();
		return false;
	}

	// �g�p�\��MSAA���擾
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
	// MSAA�p�f�[�^
	DXGI_SAMPLE_DESC sampleDesc;
	sampleDesc.Count = msaa;
	sampleDesc.Quality = 0;
	if(sampleDesc.Count > maxMsaa) {
		sampleDesc.Count = maxMsaa;
	}

	//=====================================================
	// �X���b�v�`�F�C���쐬
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
		if(errMsg)*errMsg = "�X���b�v�`�F�C���쐬���s";

		Release();
		return false;
	}

	// �X���b�v�`�F�C������o�b�N�o�b�t�@�擾
	ID3D11Texture2D* pBackBuffer;
	if(FAILED(m_pGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
		if(errMsg)*errMsg = "�o�b�N�o�b�t�@�擾���s";

		Release();
		return false;
	}
	// �o�b�N�o�b�t�@�p�̃����_�[�^�[�Q�b�g�r���[�쐬
	m_texBackBuffer = SPtr<YsTexture>(new YsTexture());
	if(m_texBackBuffer->CreateFromTexture2D(pBackBuffer) == false){
		if(errMsg)*errMsg = "�o�b�N�o�b�t�@�pView�쐬���s";

		Release();
		return false;
	}
	pBackBuffer->Release();

	//=========================================================
	// Z�o�b�t�@�E�X�e���V���o�b�t�@�쐬
	//=========================================================
	D3D11_TEXTURE2D_DESC descDS;
	ID3D11Texture2D* pDepthBuffer = nullptr;	// Z�o�b�t�@

	// �X���b�v�`�F�[���̐ݒ���擾����
	DXGI_SWAP_CHAIN_DESC chainDesc;
	hr = m_pGISwapChain->GetDesc( &chainDesc );
	if( FAILED( hr ) ){
		if(errMsg)*errMsg = "�X���b�v�`�F�[���̏��擾���s";
		Release();
		return false;
	}
	// �p�����[�^�ݒ�
	ZeroMemory( &descDS, sizeof( D3D11_TEXTURE2D_DESC ) );
	descDS.Width              = chainDesc.BufferDesc.Width;   // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Height             = chainDesc.BufferDesc.Height;  // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.MipLevels          = 1;                            // �~�b�v�}�b�v���쐬���Ȃ�
	descDS.ArraySize          = 1;                            // �e�N�X�`���[�̔z��
	descDS.Format             = DXGI_FORMAT_R24G8_TYPELESS;   // �t�H�[�}�b�g
	descDS.SampleDesc.Count   = chainDesc.SampleDesc.Count;   // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality; // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Usage              = D3D11_USAGE_DEFAULT;          // GPU �ɂ��ǂݎ�肨��я������݃A�N�Z�X��K�v�Ƃ��郊�\�[�X�B
	descDS.BindFlags          = D3D11_BIND_DEPTH_STENCIL |    // �[�x�X�e���V���o�b�t�@�Ƃ��č쐬����
								D3D11_BIND_SHADER_RESOURCE;   // �V�F�[�_�[���\�[�X�r���[�Ƃ��č쐬����
	descDS.CPUAccessFlags     = 0;                            // CPU �A�N�Z�X���s�v�B
	descDS.MiscFlags          = 0;                            // ���̑��̃t���O���ݒ肵�Ȃ��B
	// �[�x�o�b�t�@�p�̃e�N�X�`���[�쐬
	hr = m_pDevice->CreateTexture2D( &descDS, nullptr, &pDepthBuffer );
	if( FAILED( hr ) ){
		if(errMsg)*errMsg = "Z�o�b�t�@�������s";

		Release();
		return false;
	}

	// ���̃e�N�X�`��(Z�o�b�t�@)�p�̐[�x�X�e���V���r���[�쐬
	m_texDepthStencil = SPtr<YsTexture>(new YsTexture());
	if(m_texDepthStencil->CreateFromTexture2D(pDepthBuffer) == false){
		if(errMsg)*errMsg = "�[�x�X�e���V���pView�쐬���s";

		Release();
		return false;
	}
	pDepthBuffer->Release();


	//=========================================================
	// �o�b�N�o�b�t�@�A�[�x�o�b�t�@��RT�ADepth�Ƃ��ăf�o�C�X�R���e�L�X�g�փZ�b�g����
	//=========================================================
	{
		ID3D11RenderTargetView* rtv = m_texBackBuffer->GetRTTex();
		YsDx.GetDevContext()->OMSetRenderTargets(1, &rtv, m_texDepthStencil->GetDepthTex());
	}

	//=========================================================
	// �r���[�|�[�g�̐ݒ�
	//=========================================================
	SetViewport((float)w, (float)h);

	//=========================================================
	// �W���I�ȃX�e�[�g�̐ݒ�
	//=========================================================
	SetDefaultStates();

	//=========================================================
	// �X�v���C�g�o�b�`������
	//=========================================================
	m_Sprite.Init(m_pDeviceContext);

	//=========================================================
	// 1x1 ���e�N�X�`���쐬
	//=========================================================
	{
		// 0xAABBGGRR
		DWORD col = 0xFFFFFFFF;
		m_texWhite = std::make_shared<YsTexture>();
		m_texWhite->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &col);
	}

	//=========================================================
	// 1x1 �f�t�H���g�p�@���}�b�v�쐬
	//=========================================================
	{
		DWORD col = fRGBA(0.5f, 0.5f, 1.0f, 0);//0x00FF8080;
		m_texNormal = std::make_shared<YsTexture>();
		m_texNormal->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &col);
	}

	//=========================================================
	// �f�t�H���g�p �g�D�[���e�N�X�`���ǂݍ���
	//=========================================================
	m_texToon = std::make_shared<YsTexture>();
	m_texToon->LoadTexture("data/Texture/toon.png");

	// 
	ZeroMemory(m_srvReset, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);

	// DrawQuad�p�|���S���쐬
	m_polyQuad.Create(nullptr, 4, YsVertex_Pos_UV::GetVertexTypeData(), true);

	return true;
}

void YsDirectXGraphics::Release()
{
	// ���
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
	// ���X�^���C�U�X�e�[�g�ݒ�
	YsRasterizeState rs;
	rs.SetAll_Standard();
	rs.SetState();

	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	YsDepthStencilState dss;
	dss.SetAll_Standard();
	dss.SetState();

	// �u�����h�X�e�[�g�ݒ�
	YsBlendState bs;
	bs.Set_Alpha(-1);
	bs.SetState();

	// �T���v���X�e�[�g�ݒ�
	YsSamplerState ss;
	ss.SetAll_Standard();
	ss.SetStatePS();
}

bool YsDirectXGraphics::ResizeRezolution(int w, int h)
{
	//===================================================
	// �o�b�t�@���
	//===================================================
	m_texBackBuffer = nullptr;
	m_texDepthStencil = nullptr;

	//===================================================
	// ���T�C�Y
	//===================================================
	m_pGISwapChain->ResizeBuffers(m_DXGISwapChainDesc.BufferCount, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	m_pGISwapChain->GetDesc(&m_DXGISwapChainDesc);

	//===================================================
	// �o�b�t�@�쐬
	//===================================================
	// �X���b�v�`�F�C������o�b�N�o�b�t�@�擾
	ID3D11Texture2D* pBackBuffer;
	if(FAILED(m_pGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
		Release();
		return false;
	}
	// �o�b�N�o�b�t�@�p�̃����_�[�^�[�Q�b�g�r���[�쐬
	m_texBackBuffer = SPtr<YsTexture>(new YsTexture());
	if(m_texBackBuffer->CreateFromTexture2D(pBackBuffer) == false){
		Release();
		return false;
	}
	pBackBuffer->Release();


	//=========================================================
	// Z�o�b�t�@�E�X�e���V���o�b�t�@�쐬
	//=========================================================
	D3D11_TEXTURE2D_DESC descDS;
	ID3D11Texture2D* pDepthBuffer = nullptr;	// Z�o�b�t�@

	// �X���b�v�`�F�[���̐ݒ���擾����
	DXGI_SWAP_CHAIN_DESC chainDesc;
	HRESULT hr = m_pGISwapChain->GetDesc( &chainDesc );
	if( FAILED( hr ) ){
		Release();
		return false;
	}
	// �p�����[�^�ݒ�
	ZeroMemory( &descDS, sizeof( D3D11_TEXTURE2D_DESC ) );
	descDS.Width              = chainDesc.BufferDesc.Width;   // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Height             = chainDesc.BufferDesc.Height;  // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.MipLevels          = 1;                            // �~�b�v�}�b�v���쐬���Ȃ�
	descDS.ArraySize          = 1;                            // �e�N�X�`���[�̔z��
	descDS.Format             = DXGI_FORMAT_R24G8_TYPELESS;   // �t�H�[�}�b�g
	descDS.SampleDesc.Count   = chainDesc.SampleDesc.Count;   // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality; // �o�b�N�o�b�t�@�Ɠ����ɂ���B
	descDS.Usage              = D3D11_USAGE_DEFAULT;          // GPU �ɂ��ǂݎ�肨��я������݃A�N�Z�X��K�v�Ƃ��郊�\�[�X�B
	descDS.BindFlags          = D3D11_BIND_DEPTH_STENCIL |    // �[�x�X�e���V���o�b�t�@�Ƃ��č쐬����
								D3D11_BIND_SHADER_RESOURCE;   // �V�F�[�_�[���\�[�X�r���[�Ƃ��č쐬����
	descDS.CPUAccessFlags     = 0;                            // CPU �A�N�Z�X���s�v�B
	descDS.MiscFlags          = 0;                            // ���̑��̃t���O���ݒ肵�Ȃ��B
	// �[�x�o�b�t�@�p�̃e�N�X�`���[�쐬
	hr = m_pDevice->CreateTexture2D( &descDS, nullptr, &pDepthBuffer );
	if( FAILED( hr ) ){
		Release();
		return false;
	}

	// ���̃e�N�X�`��(Z�o�b�t�@)�p�̐[�x�X�e���V���r���[�쐬
	m_texDepthStencil = SPtr<YsTexture>(new YsTexture());
	if(m_texDepthStencil->CreateFromTexture2D(pDepthBuffer) == false){
		Release();
		return false;
	}
	pDepthBuffer->Release();

	//=========================================================
	// �o�b�N�o�b�t�@�A�[�x�o�b�t�@��RT�ADepth�Ƃ��ăf�o�C�X�R���e�L�X�g�փZ�b�g����
	//=========================================================
	{
		ID3D11RenderTargetView* rtv = m_texBackBuffer->GetRTTex();
		YsDx.GetDevContext()->OMSetRenderTargets(1, &rtv, m_texDepthStencil->GetDepthTex());
	}

	//=========================================================
	// �r���[�|�[�g�̐ݒ�
	//=========================================================
	SetViewport((float)w, (float)h);

	return true;

}

void YsDirectXGraphics::SetViewport(float w, float h)
{
	// �r���[�|�[�g�̐ݒ�
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
	// �s�N�Z�����W�n �� �ˉe���W�n�֕ϊ�
	//==================================================
	float fConvX = (vp.Width * 0.5f);
	float fConvY = (vp.Height * 0.5f);

	float tx = x / fConvX - 1.0f;
	float ty = y / fConvY - 1.0f;

	float tx2 = (x + w) / fConvX - 1.0f;
	float ty2 = (y + h) / fConvY - 1.0f;

	//==================================================
	//�l�p�`�f�[�^��������
	//==================================================
	// ���W�͎ˉe���W�n
	YsVertex_Pos_UV vertices[] = {
		{ YsVec3(tx, -ty2, 0), YsVec2(0, 1) },
		{ YsVec3(tx, -ty, 0), YsVec2(0, 0) },
		{ YsVec3(tx2, -ty2, 0), YsVec2(1, 1) },
		{ YsVec3(tx2, -ty, 0), YsVec2(1, 0) }
	};

	//==================================================
	// ���_��������
	//==================================================
	m_polyQuad.WriteVertexData(vertices, 4);

	//==================================================
	// �`��
	//==================================================
	// �`��p�f�[�^���Z�b�g
	m_polyQuad.SetDrawData();
	// �`��
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
	//  X�t�@�C���̓ǂݍ��݂Ƃ��Ŏg�p���邽�߂Ƀf�o�C�X�쐬���Ƃ�
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//=======================================================
	// D3D�I�u�W�F�N�g�쐬
	//=======================================================
	if(nullptr == (m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION))) {
		if(errMsg)*errMsg = "Direct3D9�쐬���s";

		Release();
		return false;
	}

	//=======================================================
	// ���݂̉�ʃ��[�h���擾
	//=======================================================
	D3DDISPLAYMODE d3ddm;
	if(FAILED(m_pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		Release();
		return false;
	}

	//=======================================================
	// �f�o�C�X�쐬
	//=======================================================
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = 0;
	d3dpp.BackBufferHeight = 0;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	// �X���b�v �G�t�F�N�g
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;// �E�B���h�E �N���C�A���g�̈�𒼂��ɍX�V���܂�
	d3dpp.hDeviceWindow = hWnd;
	// Z �o�b�t�@�̎����쐬
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// 24bitZ�o�b�t�@

	//�f�o�C�X�̍쐬 - T&L HAL
	if(FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, &d3dpp, &m_pD3D9Device))) {
		//���s�����̂�HAL�Ŏ��s
		if(FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, &d3dpp, &m_pD3D9Device))) {
			//���s�����̂�REF�Ŏ��s
			if(FAILED(m_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE, &d3dpp, &m_pD3D9Device))) {
				//���ǎ��s
				if(errMsg)*errMsg = "Direct3D9�f�o�C�X�쐬���s";

				Release();
				return false;
			}
		}
	}

	return true;
}

