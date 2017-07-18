//===============================================================
//! @file YsDirectXGraphics.h
//! @brief Direct3D�֌W�̃N���X
//!
//! @author ���c
//===============================================================

#ifndef __YsDirectXGraphics_h__
#define __YsDirectXGraphics_h__

namespace YskLib
{

//================================================
//! @brief Direct3D�N���X
//!
//! Direct3D11�̊�{�I�ȋ@�\���܂Ƃ߂��N���X
//!
//! @ingroup Graphics_Important
//================================================
class YsDirectXGraphics {
public:

	//==============================================================
	// �擾
	//==============================================================
	//! @brief �f�o�C�X�擾�B���\�[�X�̍쐬����уf�B�X�v���C �A�_�v�^�[�̋@�\�̗񋓂ȂǂŎg�p�B
	//! ���Q�ƃJ�E���^�͉��Z���Ă��܂���
	ID3D11Device*			GetDev() { return m_pDevice; }

	//! @brief �f�o�C�X�R���e�L�X�g�擾�B�`���X�e�[�g�̕ύX�ȂǂŎg�p�B
	//! ���Q�ƃJ�E���^�͉��Z���Ă��܂���
	ID3D11DeviceContext*	GetDevContext(){return m_pDeviceContext;}

	//! @brief �X���b�v�`�F�C���擾�B
	//! ���Q�ƃJ�E���^�͉��Z���Ă��܂���
	IDXGISwapChain*			GetSwapChain(){return m_pGISwapChain;}

	//! @brief X�𑜓x�擾
	int						GetRezoW(){return (int)m_DXGISwapChainDesc.BufferDesc.Width;}

	//! @brief Y�𑜓x�擾
	int						GetRezoH(){return (int)m_DXGISwapChainDesc.BufferDesc.Height;}

	//! @brief �o�b�N�o�b�t�@�擾�B
	SPtr<YsTexture>			GetBackBuffer() { return m_texBackBuffer; }

	//! @brief Z�o�b�t�@�擾�B
	SPtr<YsTexture>		GetDepthStencil() { return m_texDepthStencil; }

	//! @brief �X�v���C�g�N���X�擾
	YsSprite&				GetSprite() { return m_Sprite; }


	//! @brief 1x1 ���e�N�X�`���擾
	SPtr<YsTexture>			GetWhiteTex() { return m_texWhite; }

	//! @brief 1x1 Z�����@���}�b�v�擾
	SPtr<YsTexture>			GetNormalTex() { return m_texNormal; }

	//! @brief �ʏ�g�D�[���e�N�X�`���擾
	SPtr<YsTexture>			GetToonTex() { return m_texToon; }
	
	//! @brief ���\�[�X�q�Ɏ擾
	YsResourceStorage*		GetResStg() { return m_pResStg; }


	//! @brief DirectX9�̃f�o�C�X���擾�B
	//! ���Q�ƃJ�E���^�͉��Z���Ă��܂���
	LPDIRECT3DDEVICE9		GetD3D9Dev(){ return m_pD3D9Device; }

	//! @brief �A���`�G�C���A�X�p�萔
	enum MSAASetting{
		MSAA_None = 1,
		MSAA_x2 = 2,
		MSAA_x4 = 4,
		MSAA_x8 = 8,
	};

	//==============================================================
	// �������E���
	//==============================================================

	//--------------------------------------------------------------------------
	//! @brief �����ݒ�
	//! Direct3D11������������
	//! @param[in]	hWnd		�c �E�B���h�E�̃n���h��
	//! @param[in]	w			�c �𑜓x X
	//! @param[in]	h			�c �𑜓x Y
	//! @param[in]	resStg		�c �o�^���郊�\�[�X�Ǘ��N���X�@YskLib���Ńe�N�X�`���Ȃǂ̃��\�[�X��ǂݍ��ލۂɂ́A�������g�p�����
	//! @param[in]	msaa		�c �A���`�G�C���A�X���g�����@�e�N�X�`���̌`���Ƃ����낢��ʓ|�ɂȂ�̂Ŏg�p���Ȃ����Ɛ����@AA�͕ʓr�V�F�[�_�ł��ق��������Ǝv��
	//! @param[out]	errMsg		�c �G���[���b�Z�[�W���Ԃ�
	//! @return ���� �c true
	//--------------------------------------------------------------------------
	bool Init(HWND hWnd, int w, int h, YsResourceStorage* resStg, MSAASetting msaa = MSAA_None, std::string* errMsg = nullptr);

	//--------------------------------------------------------------------------
	//! @brief ���
	//--------------------------------------------------------------------------
	void Release();

	//--------------------------------------------------------------------------
	//! @brief �W���I�ȃX�e�[�g�ݒ������
	//--------------------------------------------------------------------------
	void SetDefaultStates();

	//--------------------------------------------------------------------------
	//! @brief �t���X�N���[���؂�ւ�
	//! @param[in]	bFullScreen	�c true:�t���X�N���[���@false:�E�B���h�E
	//--------------------------------------------------------------------------
	void SetFullScreen(bool bFullScreen){
		m_pGISwapChain->SetFullscreenState((BOOL)bFullScreen, 0);
	}

	//--------------------------------------------------------------------------
	//! @brief �𑜓x�ύX
	//! @param[in]	w		�c �𑜓x X
	//! @param[in]	h		�c �𑜓x Y
	//--------------------------------------------------------------------------
	bool ResizeRezolution(int w, int h);

	//--------------------------------------------------------------------------
	//! @brief �r���[�|�[�g�ݒ�
	//!
	//! w,h��0���w�肷��ƁA�o�b�N�o�b�t�@�̑傫���Őݒ肳���B
	//!
	//! @param[in]	w �c ��
	//! @param[in]	h �c ����
	//--------------------------------------------------------------------------
	void SetViewport(float w = 0, float h = 0);

	//==============================================================
	// ���X�^���C�U�X�e�[�g�֌W
	//==============================================================

	//! @brief �J�����O���[�h�̕ύX
	//! �����X�^���C�U�X�e�[�g�̃J�����O�ȊO�̍��ڂ͊�{�I�Ȃ��̂ɐݒ肳��܂�
	//! @param[in]	mode �c �J�����O���[�h
	//!						- D3D11_CULL_NONE �c ���ʕ`��
	//!						- D3D11_CULL_FRONT �c ���ʂ̂ݕ\��
	//!						- D3D11_CULL_BACK �c �\�ʂ̂ݕ\��
	void RS_CullMode(D3D11_CULL_MODE mode);

	//==============================================================
	// �[�x�X�e���V���X�e�[�g�֌W
	//  �ڍׂ�YsDepthStencilState�N���X���g�p���Ă�������
	//==============================================================

	//! @brief Z�����Z�������݃��[�h��ON/OFF
	//! ���[�x�X�e���V���X�e�[�g�̂��̑��̍��ڂ͊�{�I�Ȃ��̂ɐݒ肳��܂�
	void DS_ZSetting(bool ZEnable, bool ZWriteEnable);

	//==============================================================
	// �`��֌W
	//==============================================================
	//! @brief 2D�l�p�`�`��
	// x,y,w,h�̓s�N�Z�����W�n�Ŏw��
	void DrawQuad(int x, int y, int w, int h);

	
	void RemoveTextureVS(int slotNo = -1);		//!< ���_�V�F�[�_�̎w��X���b�g�̃e�N�X�`�������� -1�őS�ẴX���b�g����
	void RemoveTexturePS(int slotNo = -1);		//!< �s�N�Z���V�F�[�_�̎w��X���b�g�̃e�N�X�`�������� -1�őS�ẴX���b�g����
	void RemoveTextureGS(int slotNo = -1);		//!< �W�I���g���V�F�[�_�̎w��X���b�g�̃e�N�X�`�������� -1�őS�ẴX���b�g����
	void RemoveTextureCS(int slotNo = -1);		//!< �R���s���[�g�V�F�[�_�̎w��X���b�g�̃e�N�X�`�������� -1�őS�ẴX���b�g����

	//! @brief �o�b�N�o�b�t�@����ʂɕ\��
	HRESULT Present(UINT SyncInterval, UINT Flags);

private:

	ID3D11Device*			m_pDevice;			//!< DirectX11�̒��S�ɂȂ�N���X�B�S�̂̊Ǘ��ƃo�b�t�@�A�V�F�[�_�A�e�N�X�`���Ȃǂ̃��\�[�X�쐬�Ȃǂ��s���BDX9�Ƃ͈���āA���̃N���X�͕`��֌W�̃����o�֐��������Ȃ��B
	ID3D11DeviceContext*	m_pDeviceContext;	//!< �`�揈�����s���N���X�B
												//!< �����I�ɂ́A�����_�����O�R�}���h�ƌĂ΂��o�C�i���f�[�^���쐬���AGPU�ɑ���BGPU�������_�����O�R�}���h����͂��邱�ƂŎ��ۂ̕`�揈�����s����B
												//!< �����쐬���邱�Ƃ��ł��邽�߁A�����_�����O�R�}���h�����O�ɍ쐬������A�X���b�h�ł̕��񏈗����\�H

	IDXGIAdapter*			m_pGIAdapter;		//!< �f�B�X�v���C �T�u�V�X�e��(1�܂��͕�����GPU�ADAC�A����уr�f�I �������[)�B
	IDXGIFactory*			m_pGIFactory;		//!< �t���X�N���[���؂�ւ��ȂǂŎg�p�B
	IDXGISwapChain*			m_pGISwapChain;		//!< �E�C���h�E�ւ̕\����_�u���o�b�t�@�����O�Ȃǂ��s���N���X�B�}���`�T���v�����O�A���t���b�V�����[�g�Ȃǂ̐ݒ���ł���݂����B
	DXGI_SWAP_CHAIN_DESC	m_DXGISwapChainDesc;//!< �X���b�v�`�F�[���̐ݒ�f�[�^

	SPtr<YsTexture>			m_texBackBuffer;	//!< �o�b�N�o�b�t�@
	SPtr<YsTexture>			m_texDepthStencil;	//!< Z�o�b�t�@

	YsSprite				m_Sprite;			//!< 2D�X�v���C�g�`��֌W

	SPtr<YsTexture>			m_texWhite;			//!< 1x1 ���e�N�X�`�� �}�e���A���̃e�N�X�`�����������p
	SPtr<YsTexture>			m_texNormal;		//!< 1x1 Z�����@���}�b�v �@���}�b�v���������p
	SPtr<YsTexture>			m_texToon;			//!< data\Texture\toon.png

	YsResourceStorage*		m_pResStg;			//!< ���\�[�X�q�ɂւ̎Q��

	YsSamplerState			m_ssLinear_Wrap;	//!< Linear��� Wrap 
	YsSamplerState			m_ssLinear_Clamp;	//!< Linear��� Clamp 


	ID3D11ShaderResourceView* m_srvReset[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];	// �S��nullptr�@Shader Resource View�̃��Z�b�g�p

	// DrawQuad�p
	YsPolygon				m_polyQuad;

	//=============================================
	// DirectX9(X�t�@�C���̓ǂݍ��݂ȂǂŎg�p)
	//=============================================
	LPDIRECT3D9				m_pD3D9;			//!< Direct3D9�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3D9Device;		//!< Direct3D9�f�o�C�X

	bool CreateD3D9Device(HWND hWnd, std::string* errMsg = nullptr);

//==============================================================
// �V���O���g��
private:
	YsDirectXGraphics() : m_pDevice(0), m_pDeviceContext(0), m_pGIAdapter(0), m_pGIFactory(0), m_pGISwapChain(0), m_pResStg(0),
							m_pD3D9(0), m_pD3D9Device(0)
	{
		ZeroMemory(m_srvReset, sizeof(m_srvReset));
	}

	~YsDirectXGraphics() {
		Release();
	}

public:
	static YsDirectXGraphics& GetInstance() {
		static YsDirectXGraphics Instance;
		return Instance;
	}


//==============================================================
};


}
#endif
