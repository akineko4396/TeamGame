//===============================================================
//! @file YsTexture.h
//! @brief �e�N�X�`���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsTexture_h__
#define __YsTexture_h__

namespace YskLib
{

//===========================================================================
//!
//! @brief �e�N�X�`�����\�[�X�N���X
//!
//!  ���ȉ��̂��̂���舵����
//!   - �V�F�[�_���\�[�X(�ʏ�̃e�N�X�`��)
//!   - �����_�[�^�[�Q�b�g(�`���Ƃ��Đݒ�\��RenderTarget�e�N�X�`��)
//!   - Z�o�b�t�@(Z�o�b�t�@�ƃX�e���V���o�b�t�@)
//!   - �A���I�[�_�[�h�A�N�Z�X�e�N�X�`��
//!   .
//!  �����̑�
//!   - MSAA(�}���`�T���v�� �A���`�G�C���A�V���O)�̐ݒ�́A�o�b�N�o�b�t�@�̐ݒ���g�p���܂�
//!
//!  ��Direct3D11�Ȃǂł́A�e�N�X�`���z��Ƃ������̂��g�p�ł���
//!    �ꌾ�ł����ƁA�P�̃��\�[�X�f�[�^�̒��ɕ����̉摜�����銴��(Direct3D9�ł����L���[�u�}�b�v�̂悤�Ȃ���)
//!
//! @ingroup Graphics_Important
//===========================================================================
class YsTexture : public YsIResource {
public:
	//===================================================================
	// �ǂݍ��݁E�쐬
	//===================================================================

	//--------------------------------------------------------------------------
	//! @brief �摜�t�@�C���ǂݍ���
	//! ��{�I�Ȑݒ�œǂݍ��ށAShaderResourceView�݂̂��쐬���܂�
	//! @param[in] filename		�c �ǂݍ��މ摜�t�@�C����
	//! @return  true:����
	//--------------------------------------------------------------------------
	bool LoadTexture(const std::string& filename);

	//--------------------------------------------------------------------------
	//! @brief �摜�t�@�C���ǂݍ���
	//! �ڍאݒ�œǂݍ��ށAShaderResourceView�݂̂��쐬���܂�
	//! @param[in] filename		�c �ǂݍ��މ摜�t�@�C����
	//! @param[in] MipLevels	�c �쐬����~�b�v�}�b�v���B0�ł��ׂẴ~�b�v�}�b�v���쐬����B1����1���̂�(�~�b�v�}�b�v�쐬���Ȃ�)�B
	//! @param[in] format		�c �ǂݍ��񂾃e�N�X�`���̌`�����w�肷��BDXGI_FORMAT_FROM_FILE�Ńt�@�C������`�����擾�B
	//! @param[in] bindFlag		�c �e�N�X�`���[���o�C���h�ł���悤�ɂ���p�C�v���C�� �X�e�[�W
	//! @return  true:����
	//--------------------------------------------------------------------------
	bool LoadTextureEx(const std::string& filename, UINT MipLevels = 0, DXGI_FORMAT format = DXGI_FORMAT_FROM_FILE, UINT bindFlag = D3D11_BIND_SHADER_RESOURCE);

	//--------------------------------------------------------------------------
	//! @brief �摜�t�@�C���ǂݍ���
	//! �ڍאݒ�œǂݍ��ށAShaderResourceView�݂̂��쐬���܂�
	//! @param[in] filename		�c �ǂݍ��މ摜�t�@�C����
	//! @param[in] loadInfo		�c �ǂݍ��ݏڍאݒ�
	//! @return  true:����
	//--------------------------------------------------------------------------
	bool LoadTextureEx(const std::string& filename, D3DX11_IMAGE_LOAD_INFO* loadInfo);

	//--------------------------------------------------------------------------
	//! @brief �ʏ�e�N�X�`���쐬(ShaderResourceView�݂̂̍쐬)
	//! �e�N�X�`�����\�[�X���쐬���AShaderResourceView�݂̂��쐬���܂�
	//! @param[in] w			�c �摜�̕�(�s�N�Z��)
	//! @param[in] h			�c �摜�̕�(�s�N�Z��)
	//! @param[in] format		�c �摜�̌`��
	//! @param[in] fillData		�c �o�b�t�@�ɏ������ރf�[�^�Bw*h*format�̃T�C�Y�Ŏ����Ă������ƁBnullptr���Ə������݂Ȃ��B
	//! @return true:����
	//--------------------------------------------------------------------------
	bool Create(int w, int h, DXGI_FORMAT format, const void* fillData = nullptr);

	//--------------------------------------------------------------------------
	//! @brief �����_�[�^�[�Q�b�g�e�N�X�`���Ƃ��č쐬(RenderTargetView��ShaderResourceView���쐬����)
	//! �e�N�X�`�����\�[�X���쐬���ARenderTargetView��ShaderResourceView��2��ނ��쐬���܂�
	//! @param[in] w			�c �摜�̕�(�s�N�Z��)
	//! @param[in] h			�c �摜�̕�(�s�N�Z��)
	//! @param[in] format		�c �摜�̌`��
	//! @param[in] arrayCnt		�c �e�N�X�`���z����g�p����ꍇ�A���̐��B1�Œʏ��1���e�N�X�`��
	//! @param[in] fillData		�c �o�b�t�@�ɏ������ރf�[�^�Bw*h*format�̃T�C�Y�Ŏ����Ă������ƁBnullptr���Ə������݂Ȃ��B
	//! @return true:����
	//--------------------------------------------------------------------------
	bool CreateRT(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const void* fillData = nullptr);

	//--------------------------------------------------------------------------
	//! @brief �����_�[�^�[�Q�b�g�e�N�X�`���Ƃ��č쐬(�L���[�u�}�b�v�p)(RenderTargetView��ShaderResourceView���쐬����)
	//!
	//! �L���[�u�}�b�v�́A�e�N�X�`���z��6���Ɠ����BShaderResourceView��D3D11_SRV_DIMENSION_TEXTURECUBE�̃t���O���t��
	//!
	//! @param[in] w			�c �摜�̕�(�s�N�Z��)
	//! @param[in] h			�c �摜�̕�(�s�N�Z��)
	//! @param[in] format		�c �摜�̌`��
	//! @return true:����
	//--------------------------------------------------------------------------
	bool CreateCubeRT(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	//===================================================================
	//! @brief �A���I�[�_�[�h�A�N�Z�X�e�N�X�`���Ƃ��č쐬(UnorderedAccessView��ShaderResourceView���쐬����)
	//! @param[in] w			�c �摜�̕�(�s�N�Z��)
	//! @param[in] h			�c �摜�̕�(�s�N�Z��)
	//! @param[in] format		�c �摜�̌`��
	//! @param[in] arrayCnt		�c �e�N�X�`���z����g�p����ꍇ�A���̐��B1�Œʏ��1���e�N�X�`��
	//! @param[in] fillData		�c �o�b�t�@�ɏ������ރf�[�^�Bw*h*format�̃T�C�Y�Ŏ����Ă������ƁBnullptr���Ə������݂Ȃ��B
	//! return true:����
	//--------------------------------------------------------------------------
	bool CreateUAV(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const void* fillData = nullptr);

	//===================================================================

	//--------------------------------------------------------------------------
	//! @brief Z�o�b�t�@�e�N�X�`���Ƃ��č쐬(DepthStencilView��ShaderResourceView���쐬����)
	//! �e�N�X�`�����\�[�X���쐬���ADepthStencilView��ShaderResourceView��2��ނ��쐬���܂�
	//! @param[in] w			�c �摜�̕�(�s�N�Z��)
	//! @param[in] h			�c �摜�̕�(�s�N�Z��)
	//! @param[in] format		�c �摜�̌`��
	//! @param[in] arrayCnt	�c �e�N�X�`���z����g�p����ꍇ�A���̐��B1�Œʏ��1���e�N�X�`��
	//! @param[in] fillData	�c �o�b�t�@�ɏ������ރf�[�^�Bw*h*format�̃T�C�Y�Ŏ����Ă������ƁBnullptr���Ə������݂Ȃ��B
	//! @return true:����
	//--------------------------------------------------------------------------
	bool CreateDepth(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS, UINT arrayCnt = 1, const void* fillData = nullptr);

	//--------------------------------------------------------------------------
	//! @brief ����Texture2D���\�[�X���Z�b�g
	//! �����œn�����e�N�X�`�����\�[�X�ɍœK�ȃr���[���쐬����
	//! @param[in]  pTexture2D	�c �Z�b�g�������e�N�X�`��
	//! @return true:����
	//--------------------------------------------------------------------------
	bool CreateFromTexture2D(ID3D11Texture2D* pTexture2D);

	private:
	//! @brief m_pTexture2D�̐ݒ�����ɁA�e�r���[���쐬����
	bool CreateViewsFromTexture2D();
	public:

	//--------------------------------------------------------------------------
	//! @brief ���
	//--------------------------------------------------------------------------
	void Release();

	//===================================================================
	// �N���A�֌W
	//===================================================================

	//--------------------------------------------------------------------------
	//! @brief RenderTarget���N���A
	//! ��RenderTargetView���쐬����K�v������܂�
	//! ���e�N�X�`���z��̏ꍇ�́A�S���̃e�N�X�`�����N���A�����
	//! @param[in]	ColorRGBA	�c �N���A����F
	//--------------------------------------------------------------------------
	void ClearRT(const YsVec4& ColorRGBA);

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���z��̕���RenderTarget�̂����A�P�������N���A
	//! �e�N�X�`���z��ō쐬����RenderTarget�́A�w��Ԗڂ̂��̂������N���A�ł���B
	//! �e�N�X�`���z��Ƃ��č쐬���Ă��Ȃ���΂��̊֐��͈Ӗ��Ȃ��ł��B
	//! @param[in]	idx			�c �e�N�X�`���z��̔ԍ�
	//! @param[in]	ColorRGBA	�c �N���A����F
	//--------------------------------------------------------------------------
	void ClearRTArray(int idx, const YsVec4& ColorRGBA);

	//--------------------------------------------------------------------------
	//! @brief DepthStencil���N���A
	//! ��DepthStencilView���쐬����K�v������܂�
	//! ���e�N�X�`���z��̏ꍇ�́A�S���̃e�N�X�`�����N���A�����
	//! @param[in] bDepth		�c Z�o�b�t�@�N���A����H
	//! @param[in] bStencil		�c �X�e���V���o�b�t�@�N���A����H
	//! @param[in] depth		�c �N���A����Z�o�b�t�@�̒l
	//! @param[in] stencil		�c �N���A����X�e���V���o�b�t�@�̒l
	//--------------------------------------------------------------------------
	void ClearDepth(bool bDepth = true, bool bStencil = true, FLOAT depth = 1.0f, UINT8 stencil = 0);

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���z��̕���DepthStencil�̂����A�P�������N���A
	//! �e�N�X�`���z��ō쐬����DepthStencil�́A�w��Ԗڂ̂��̂������N���A�ł���B
	//! �e�N�X�`���z��Ƃ��č쐬���Ă��Ȃ���΂��̊֐��͈Ӗ��Ȃ��ł��B
	//! ��DepthStencilView���쐬����K�v������܂�
	//! @param[in] bDepth		�c Z�o�b�t�@�N���A����H
	//! @param[in] bStencil		�c �X�e���V���o�b�t�@�N���A����H
	//! @param[in] depth		�c �N���A����Z�o�b�t�@�̒l
	//! @param[in] stencil		�c �N���A����X�e���V���o�b�t�@�̒l
	//--------------------------------------------------------------------------
	void ClearDepthArray(int idx, bool bDepth = true, bool bStencil = true, FLOAT depth = 1.0f, UINT8 stencil = 0);

	//===================================================================
	// �擾�E�ݒ�
	//===================================================================

	//! @brief ���ݓǂݍ���ł���t�@�C�����擾
	const std::string&			GetFileName() { return m_strFileName; }

	//! @brief �e�N�X�`�����\�[�X�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���܂���)
	ID3D11Texture2D*			GetTexture2D(){ return m_pTexture2D; }

	//! @brief ShaderResource�r���[�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���܂���)
	ID3D11ShaderResourceView*	GetTex() { return m_pTexView; }

	//! @brief �e�N�X�`���z��p ShaderResource�r���[�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���܂���)
	//! ���e�N�X�`���z��Ƃ��č쐬���Ȃ��ꍇ�̂ݎg�p�\
	ID3D11ShaderResourceView*	GetTexArray(int no) { return m_pTexViewArray[no]; }



	//! @brief RenderTarget�r���[�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���Ă��܂���)
	ID3D11RenderTargetView*		GetRTTex(){ return m_pTexRTView; }

	//! @brief �e�N�X�`���z��p RenderTarget�r���[�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���܂���)
	//! ���e�N�X�`���z��Ƃ��č쐬���Ȃ��ꍇ�̂ݎg�p�\
	ID3D11RenderTargetView*		GetRTTexArray(int no) { return m_pTexRTViewArray[no]; }



	//! @brief UnorderedAccessView�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���܂���)
	ID3D11UnorderedAccessView*	GetUATex(){ return m_pTexUAView; }



	//! @brief DepthStencil�r���[�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���܂���)
	ID3D11DepthStencilView*		GetDepthTex(){ return m_pTexDepthView; }	
	
	//! @brief �e�N�X�`���z��p DepthStencil�r���[�擾(���߂�l�̃r���[�̎Q�ƃJ�E���^��AddRef���܂���)
	//! ���e�N�X�`���z��Ƃ��č쐬���Ȃ��ꍇ�̂ݎg�p�\
	ID3D11DepthStencilView*		GetDepthTexArray(int no) { return m_pTexDSViewArray[no]; }



	//! @brief �摜���擾
	const D3D11_TEXTURE2D_DESC&	GetInfo() const{ return m_Desc; }		// ���

	//! @brief �摜�̕��E���̃r���[�|�[�g�ɕύX����
	void SetViewport() const;
	
	//===================================================================
	// �e�V�F�[�_�[�X�e�[�W�Ƀe�N�X�`�����Z�b�g
	//  SlotNo	�c HLSL����register(t0)�̔ԍ��ɂ�����@�͈͂�0�`127
	//===================================================================

	//! @brief �s�N�Z���V�F�[�_�X�e�[�W�Ƀe�N�X�`�����Z�b�g����
	void SetTexturePS(UINT SlotNo) const;
	
	//! @brief ���_�V�F�[�_�X�e�[�W�Ƀe�N�X�`�����Z�b�g����
	void SetTextureVS(UINT SlotNo) const;
	
	//! @brief �W�I���g���V�F�[�_�X�e�[�W�Ƀe�N�X�`�����Z�b�g����
	void SetTextureGS(UINT SlotNo) const;
	
	//! @brief �R���s���[�g�V�F�[�_�X�e�[�W�Ƀe�N�X�`�����Z�b�g����
	void SetTextureCS(UINT SlotNo) const;

	//===================================================================
	// ���̑�
	//===================================================================

	//! @brief ���\�[�X���
	virtual std::string GetTypeName() const override {
		std::string str = "YsTexture : ";
		str += m_strFileName;
		return str;
	}

	// 
	YsTexture() : m_pTexture2D(0), m_pTexView(0), m_pTexRTView(0), m_pTexUAView(0), m_pTexDepthView(0)
	{
		ZeroMemory( &m_Desc, sizeof( m_Desc ) );
	}

	~YsTexture(){
		Release();
	}


private:

	std::string					m_strFileName;		//!< �t�@�C����(LoadTexture()�����ꍇ�̂�)

	ID3D11Texture2D*			m_pTexture2D;		//!< �e�N�X�`�����\�[�X(�摜�f�[�^�{��)

	ID3D11ShaderResourceView*	m_pTexView;			//!< �V�F�[�_���\�[�X�r���[(m_pTexture2D����F��ǂݍ��ނ��߂̂���)
	ID3D11RenderTargetView*		m_pTexRTView;		//!< �����_�[�^�[�Q�b�g�r���[(m_pTexture2D�֏������݂��s�����߂̂���)
	ID3D11UnorderedAccessView*	m_pTexUAView;		//!< �A���I�[�_�[�h�A�N�Z�X�r���[
	ID3D11DepthStencilView*		m_pTexDepthView;	//!< �[�x�X�e���V���r���[(m_pTexture2D��Z�o�b�t�@�Ƃ��ď������ނ��߂̂���)

	//! SRV�e�N�X�`���z��p
	std::vector<ID3D11ShaderResourceView*>	m_pTexViewArray;
	//! SRV�e�N�X�`���z����
	void ClearSRVArray(){
		for(auto& val : m_pTexViewArray){
			YsSafe_Release(val);
		}
		m_pTexViewArray.clear();
		m_pTexViewArray.shrink_to_fit();
	}

	//! RTV�e�N�X�`���z��p
	std::vector<ID3D11RenderTargetView*>	m_pTexRTViewArray;
	//! RTV�e�N�X�`���z����
	void ClearRTVArray(){
		for(auto& val : m_pTexRTViewArray){
			YsSafe_Release(val);
		}
		m_pTexRTViewArray.clear();
		m_pTexRTViewArray.shrink_to_fit();
	}

	//! DSV�e�N�X�`���z��p
	std::vector<ID3D11DepthStencilView*>	m_pTexDSViewArray;
	//! DSV�e�N�X�`���z����
	void ClearDSVArray(){
		for(auto& val : m_pTexDSViewArray){
			YsSafe_Release(val);
		}
		m_pTexDSViewArray.clear();
		m_pTexDSViewArray.shrink_to_fit();
	}

	D3D11_TEXTURE2D_DESC		m_Desc;				//!< ���

private:
	// �R�s�[�֎~�p
	YsTexture(const YsTexture& src) {}
	void operator=(const YsTexture& src) {}
};

// format����
// _FLOAT �c ���������_�^�Bfloat�݂����ȁB
// _SINT  �c �����t�������^�Bint�݂����ȁB
// _UINT  �c �����Ȃ������^�Bunsigned int�݂����ȁB
// _SNORM �c �����t�����K�������^�B�܂�-1.0�`1.0�ň�����B
// _UNORM �c �����Ȃ����K�������^�B�܂�0.0�`1.0�ň�����B
// _TYPELESS �c �^�Ȃ��f�[�^�^�BRT�Ƃ��ɂ͎g���Ȃ����ۂ��BDepth�͂��ꂶ��Ȃ��Ƃ����Ȃ��ۂ��B



//================================================================================================
//! @brief �e�N�X�`���Z�b�g�N���X
//! 
//! �P���ɕ����̃e�N�X�`��(YsTexture)���P�̍\���̂ɂ܂Ƃ߂����� \n
//! LoadTexture�֐��ɂ��A�t�@�C�������L�[�Ɉ�C�ɓǂݍ��܂�� \n
//! �V�F�[�_�ō��i���ȕ\�����s���̂ɕ֗�						\n
//!																\n
//! ��)LoadTexture("aaa.png");		\n
//!			��
//!�@�@�EMeshTex   �c aaa.png���ǂݍ��܂��\n
//!�@�@�ENormalTex �c aaa.normal.png���ǂݍ��܂��\n
//!�@�@�EExtTex    �c aaa.ext.png���ǂݍ��܂��\n
//!�@�@�EExtTex2   �c aaa.ext2.png���ǂݍ��܂��\n
//!�@�@�EExtTex3   �c aaa.ext3.png���ǂݍ��܂��\n
//!�@�@�EExtTex4   �c aaa.ext4.png���ǂݍ��܂��\n
//!�@�@�EExtTex5   �c aaa.ext5.png���ǂݍ��܂��\n
//!�@�@�EExtTex6   �c aaa.ext6.png���ǂݍ��܂��\n
//!�@�@�EExtTex7   �c aaa.ext7.png���ǂݍ��܂��\n
//!�@�@�EExtTex8   �c aaa.ext8.png���ǂݍ��܂��\n
//!�@���t�@�C�����Ɗg���q�̊ԂɁA��L�̂悤�Ȗ��O��}�������t�@�C����ǂݍ��ނ悤�ɂȂ�\n
//!
//! @ingroup Graphics_Important
//================================================================================================
struct YsTextureSet {
	SPtr<YsTexture>	MeshTex;	//!< �f�J�[���e�N�X�`��
	SPtr<YsTexture>	NormalTex;	//!< �@���e�N�X�`��
	SPtr<YsTexture>	ExtTex;		//!< ���̑��g���e�N�X�`��
	SPtr<YsTexture>	ExtTex2;	// �c
	SPtr<YsTexture>	ExtTex3;
	SPtr<YsTexture>	ExtTex4;
	SPtr<YsTexture>	ExtTex5;
	SPtr<YsTexture>	ExtTex6;
	SPtr<YsTexture>	ExtTex7;
	SPtr<YsTexture>	ExtTex8;

	//! @brief �e�N�X�`���Z�b�g�ǂݍ���(�@���}�b�v��g���e�N�X�`���Ȃǂ��ǂݍ���)
	//! �e�N�X�`���̓ǂݍ��݂ɂ́AYsDirectXGraphics�ɓo�^����Ă���ResourceStorage���g���܂�
	//! @param[in] filename �c �t�@�C����
	void LoadTextureSet(const std::string& filename);

	//! @brief ����Bshared_ptr�Ȃ̂�nullptr�����Ă邾���B
	void Release(){
		MeshTex = nullptr;
		NormalTex = nullptr;
		ExtTex = nullptr;
		ExtTex2 = nullptr;
		ExtTex3 = nullptr;
		ExtTex4 = nullptr;
		ExtTex5 = nullptr;
		ExtTex6 = nullptr;
		ExtTex7 = nullptr;
		ExtTex8 = nullptr;
	}

	// 
	YsTextureSet(){
	}
	YsTextureSet(SPtr<YsTexture> src){
		*this = src;
	}

	//! @brief YsTexture��shared_ptr����R�s�[���Ă���Bsrc��MeshTex�Ɋi�[�����B
	//! MeshTex�ȊO�̃����o�́A���YsTexture�ɂȂ�
	void operator=(SPtr<YsTexture> src){
		if(src != nullptr){
			MeshTex = src;
		}
		else{
			MeshTex = std::make_shared<YsTexture>();
		}
		NormalTex = std::make_shared<YsTexture>();
		ExtTex = std::make_shared<YsTexture>();
		ExtTex2 = ExtTex;
		ExtTex3 = ExtTex;
		ExtTex4 = ExtTex;
		ExtTex5 = ExtTex;
		ExtTex6 = ExtTex;
		ExtTex7 = ExtTex;
		ExtTex8 = ExtTex;
	}

	void operator=(const YsTextureSet& src){
		if(&src == nullptr){
			*this = nullptr;
		}
		else{
			MeshTex = src.MeshTex;
			NormalTex = src.NormalTex;
			ExtTex = src.ExtTex;
			ExtTex2 = src.ExtTex2;
			ExtTex3 = src.ExtTex3;
			ExtTex4 = src.ExtTex4;
			ExtTex5 = src.ExtTex5;
			ExtTex6 = src.ExtTex6;
			ExtTex7 = src.ExtTex7;
			ExtTex8 = src.ExtTex8;
		}
	}
};

}

#endif
