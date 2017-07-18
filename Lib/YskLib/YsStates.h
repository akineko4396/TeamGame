//===============================================================
//! @file YsStates.h
//! @brief �X�e�[�g�N���X�Q�A�����_�[�^�[�Q�b�g�؂�ւ��N���X
//!
//! �EYsBlendState�N���X		�c ����������Z�����Ȃǂ�؂�ւ��鎞�Ɏg��		\n
//! �EYsDepthStencilState�N���X	�c Z�o�b�t�@��X�e���V���o�b�t�@�̓���ݒ�		\n
//! �EYsSamplerState�N���X		�c �e�N�X�`���̏ڍׂȎg�p�ݒ�					\n
//! �EYsRasterizeState�N���X	�c 3D����2D�ւ̃����_�����O���̓���ݒ�			\n
//! �EYsRenderTargets�N���X		�c MRT(�}���`�����_�[�^�[�Q�b�g)���l������RT�ADepth������s��	\n
//!
//! @author ���c
//===============================================================
#ifndef __YsStates_h__
#define __YsStates_h__

namespace YskLib {

//! @defgroup Graphics_States �O���t�B�b�N�֌W(�X�e�[�g�A�����_�[�^�[�Q�b�g�n)
//! @{

//================================================
//! @brief �u�����h�X�e�[�g�N���X
//!
//!	����������Z�����Ȃǂ�؂�ւ��鎞�Ɏg��
//================================================
class YsBlendState {
public:
	//===============================================
	// �p�����[�^�ݒ�n
	//  ���܂��f�o�C�X�ւ̓Z�b�g����܂���BSetState()�ŃZ�b�g����܂��B
	//  ���܂��A�����̊֐��̓X�e�[�g�I�u�W�F�N�g��������܂�(�č쐬���K�v�Ȃ���)
	//===============================================
	//! @brief ���݂̃p�C�v���C������擾���������g��
	void SetAll_GetState();

	//! @brief  ������ No �c -1�őSRT(0�`7)�ɃZ�b�g����
	//! @param[in]	RT_No�@			�c �ݒ肷�郌���_�[�^�[�Q�b�g�ԍ��B-1�őSRT(0�`7)�ɃZ�b�g����B
	//! @param[in]	bOverrideAlpha�@�c �`���̃A���t�@�l���㏑�����邩�H true�ɂ���ƕ`���̃A���t�@�ɕ`�挳�̃A���t�@���㏑������(RT���A���t�@�l�������Ă�e�N�X�`���̂ݗL��)�B
	void Set_NoAlpha(int RT_No = 0, bool bOverrideAlpha = false);
	
	//! @brief  ���������� No �c -1�őSRT(0�`7)�ɃZ�b�g����
	void Set_Alpha(int RT_No = 0);
	
	//! @brief  ���Z���� No �c -1�őSRT(0�`7)�ɃZ�b�g����
	void Set_Add(int RT_No = 0);

	//! @brief  �e�`�����l���̏������݃}�X�N��ݒ�
	//! @param[in]	RT_No�@	�c �ݒ肷�郌���_�[�^�[�Q�b�g�ԍ��B-1�őSRT(0�`7)�ɃZ�b�g����B
	//! @param[in]	r		�c �Ԑ������������ނ��ǂ���
	//! @param[in]	g		�c �ΐ������������ނ��ǂ���
	//! @param[in]	b		�c �������������ނ��ǂ���
	//! @param[in]	a		�c �A���t�@�������������ނ��ǂ���
	void Set_WriteMask(int RT_No = 0, bool r = true, bool g = true, bool b = true, bool a = true);

	//! @brief �A���t�@�g�D�J�o���b�W�̐ݒ�B���e�X�g�݂����Ȃ��́B
	void Set_AlphaToCoverageEnable(bool bEnable){
		YsSafe_Release(m_pBlendState);
		m_Desc.AlphaToCoverageEnable = bEnable ? TRUE : FALSE;
	}

	//! @brief MRT�Ŋe�����_�[�^�[�Q�b�g�̃u�����h�X�e�[�g�̐ݒ���ʂɐݒ�ł���
	//! @param[in] bEnable	�c	false:0�Ԗڂ݂̂��g�p�����
	//!							true:0�`7�ʂŎg�p�����
	void Set_IndependentBlendEnable(bool bEnable) {
		YsSafe_Release(m_pBlendState);
		m_Desc.IndependentBlendEnable = bEnable ? TRUE : FALSE;
	}

	//! @brief �u�����f�B���O�W���ݒ�BRGBA �̐����B
	//!�@��D3D11_BLEND_BLEND_FACTOR�w�莞�̂ݎg�p�B
	void Set_BlendFactor(const float factor[4]){
		YsSafe_Release(m_pBlendState);
		for(int i = 0; i<4; i++){
			m_BlendFactor[i] = factor[i];
		}
	}

	//! @brief ���ׂẴA�N�e�B�u�ȃ����_�[�^�[�Q�b�g�łǂ̃T���v�����X�V����邩�����ʂ����}�X�N��ݒ�
	void Set_SampleMask(UINT mask){
		YsSafe_Release(m_pBlendState);
		m_SampleMask = mask;
	}

	//! @brief desc�̓��e���R�s�[����
	void Set_FromDesc(const D3D11_BLEND_DESC& desc){
		YsSafe_Release(m_pBlendState);
		m_Desc = desc;
	}

	//===============================================
	// �쐬�E�Z�b�g
	//===============================================
	//! @brief ���݂̐ݒ���e�����ɁA�X�e�[�g�I�u�W�F�N�g���쐬�̂ݍs��
	//! @return �쐬����:true
	bool Create();

	//! @brief �u�����h�X�e�[�g���f�o�C�X�փZ�b�g����
	//! �X�e�[�g�I�u�W�F�N�g���쐬����Ă��Ȃ��ꍇ�́A�쐬����
	void SetState();

	//! @brief ���
	void Release(){
		YsSafe_Release(m_pBlendState);
	}

	//===============================================
	// �ݒ���擾
	//===============================================
	//! @brief �ݒ���擾
	const D3D11_BLEND_DESC&	GetDesc() { return m_Desc; }
	//! @brief �X�e�[�g�I�u�W�F�N�g�擾
	ID3D11BlendState*		GetState() { return m_pBlendState; }
	//! @brief BlendFactor�擾
	float*					GetBlendFactor() { return m_BlendFactor; }
	//! @brief SampleMasl�擾
	UINT					GetSampleMask() { return m_SampleMask; }

	//===============================================
	YsBlendState() : m_pBlendState(0) {
		ZeroMemory(&m_Desc, sizeof(D3D11_BLEND_DESC));
		Set_Alpha(0);
		ZeroMemory(m_BlendFactor, sizeof(m_BlendFactor));
		m_SampleMask = 0xFFFFFFFF;
		Set_WriteMask(-1);
	}
	~YsBlendState() {
		Release();
	}

private:
	// �u�����h�X�e�[�g
	D3D11_BLEND_DESC	m_Desc;				//!< �ݒ���
	ID3D11BlendState*	m_pBlendState;		//!< �u�����h�X�e�[�g�I�u�W�F�N�g

	float				m_BlendFactor[4];	//!< BlendFactor
	UINT				m_SampleMask;		//!< SampleMask

private:
	// �R�s�[�֎~�p
	YsBlendState(const YsBlendState& src) {}
	void operator=(const YsBlendState& src) {}
};


//================================================
//! @brief �[�x�X�e���V���X�e�[�g�N���X
//!
//!�@Z�o�b�t�@��X�e���V���o�b�t�@�̓���ݒ�
//================================================
class YsDepthStencilState {
public:
	//===============================================
	// �쐬�E�Z�b�g
	//  ���܂��f�o�C�X�ւ̓Z�b�g����܂���BSetState()�ŃZ�b�g����܂��B
	//  ���܂��A�����̊֐��̓X�e�[�g�I�u�W�F�N�g��������܂�(�č쐬���K�v�Ȃ���)
	//===============================================

	//----------------------------------------------------------
	//! @brief ��{�I�Ȑݒ���Z�b�g
	//----------------------------------------------------------
	void SetAll_Standard();

	//----------------------------------------------------------
	//! @brief ���݂̃p�C�v���C������擾���������g��
	//! ������������ł́A���ׂĂ̍��ڂ��擾�ł��Ȃ����Ƃ�����܂�
	//----------------------------------------------------------
	void SetAll_GetState();

	//----------------------------------------------------------
	//! @brief Z�����ON/OFF
	void Set_ZEnable(bool enable){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.DepthEnable = enable ? TRUE : FALSE;
	}
	//! @brief Z�������݂�ON/OFF
	void Set_ZWriteEnable(bool enable){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.DepthWriteMask = enable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	}
	//! @brief Z�����r���@�̐ݒ� �ʏ��D3D11_COMPARISON_LESS_EQUAL��OK
	void Set_ZFunc(D3D11_COMPARISON_FUNC funcFlag = D3D11_COMPARISON_LESS_EQUAL){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.DepthFunc = funcFlag;
	}

	//! @brief �X�e���V���e�X�g��ON/OFF
	void Set_StencilEnable(bool enable){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.StencilEnable = enable ? TRUE : FALSE;
	}
	//! @brief �X�e���V���o�b�t�@��ǂݎ�镔���̃}�X�N�ݒ�
	void Set_StencilReadMask(unsigned char mask){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.StencilReadMask = mask;
	}
	//! @brief �X�e���V���o�b�t�@���������ޕ����̃}�X�N�ݒ�
	void Set_StencilWriteMask(unsigned char mask){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.StencilWriteMask = mask;
	}
	//! @brief �[�x�X�e���V�� �e�X�g�̎��s���Ɋ�Ƃ��Ďg�p�����Q�ƒl�ݒ�
	//! @param[in]	stencilRef	�c �X�e���V���l
	void Set_StencilRef(UINT stencilRef) {
		YsSafe_Release(m_pDepthStencilState);
		m_StencilRef = stencilRef;
	}

	//! @brief desc�̓��e���R�s�[����
	void Set_FromDesc(const D3D11_DEPTH_STENCIL_DESC& desc){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc = desc;
	}
	//----------------------------------------------------------

	//===============================================
	// �쐬�E�Z�b�g
	//===============================================
	//! @brief ���݂�Desc�̓��e�����ɃX�e�[�g�I�u�W�F�N�g���쐬�̂ݍs��
	//! @return �쐬����:true
	bool Create();

	//! @brief �[�x�X�e���V���X�e�[�g���f�o�C�X�փZ�b�g
	//!	�X�e�[�g�I�u�W�F�N�g���쐬����Ă��Ȃ��ꍇ�́A�쐬����
	void SetState();

	//! @brief ���
	void Release(){
		YsSafe_Release(m_pDepthStencilState);
	}

	//===============================================
	// �ݒ���擾
	//===============================================
	//! @brief �ݒ���擾
	const D3D11_DEPTH_STENCIL_DESC&	GetDesc() { return m_Desc; }
	//! @brief �[�x�X�e���V���X�e�[�g�I�u�W�F�N�g�擾
	ID3D11DepthStencilState*		GetState() { return m_pDepthStencilState; }
	//! @brief �V�e���V���l�擾
	UINT							GetStencilRef() { return m_StencilRef; }

	//===============================================
	YsDepthStencilState() : m_pDepthStencilState(0), m_StencilRef(0) {
	}

	~YsDepthStencilState() {
		Release();
	}

private:
	D3D11_DEPTH_STENCIL_DESC	m_Desc;					//!< �ݒ���
	ID3D11DepthStencilState*	m_pDepthStencilState;	//!< �X�e�[�g�I�u�W�F�N�g
	UINT						m_StencilRef;			//!< �X�e���V���l

private:
	// �R�s�[�֎~�p
	YsDepthStencilState(const YsDepthStencilState& src) {}
	void operator=(const YsDepthStencilState& src) {}
};

//================================================
//! @brief �T���v���[�X�e�[�g�N���X
//!
//!�@�e�N�X�`���̏ڍׂȎg�p�ݒ�
//================================================
class YsSamplerState {
public:
	//===============================================
	// �쐬�E�Z�b�g
	//  ���܂��f�o�C�X�ւ̓Z�b�g����܂���BSetState()�ŃZ�b�g����܂��B
	//  ���܂��A�����̊֐��̓X�e�[�g�I�u�W�F�N�g��������܂�(�č쐬���K�v�Ȃ���)
	//===============================================

	//----------------------------------------------------------
	//! @brief �W���I�ȃT���v���쐬
	//----------------------------------------------------------
	void SetAll_Standard();

	//----------------------------------------------------------
	//! @brief �s�N�Z���V�F�[�_�p�C�v���C�����猻�݂̐ݒ�����擾
	//! @param[in]	StartSlot	�c �ݒ���擾�������X���b�g�ԍ�
	void SetAll_GetStatePS(UINT StartSlot);
	//! @brief ���_�V�F�[�_�p�C�v���C�����猻�݂̐ݒ�����擾
	//! @param[in]	StartSlot	�c �ݒ���擾�������X���b�g�ԍ�
	void SetAll_GetStateVS(UINT StartSlot);
	//! @brief �W�I���g���V�F�[�_�p�C�v���C�����猻�݂̐ݒ�����擾
	//! @param[in]	StartSlot	�c �ݒ���擾�������X���b�g�ԍ�
	void SetAll_GetStateGS(UINT StartSlot);
	//! @brief �R���s���[�^�V�F�[�_�p�C�v���C�����猻�݂̐ݒ�����擾
	//! @param[in]	StartSlot	�c �ݒ���擾�������X���b�g�ԍ�
	void SetAll_GetStateCS(UINT StartSlot);
	//----------------------------------------------------------

	//----------------------------------------------------------
	//! @brief �e�N�X�`�� �A�h���b�V���O���[�h��WRAP�ɐݒ�
	void Set_Wrap();

	//! @brief �e�N�X�`�� �A�h���b�V���O���[�h��CLAMP�ɐݒ�
	void Set_Clamp();
	//----------------------------------------------------------

	//----------------------------------------------------------
	//! @brief �t�B���^�ݒ�
	//! @param[in]	filter	�c �t�B���^
	void Set_Filter(D3D11_FILTER filter, int maxAnisotropy = 1);
	void Set_Filter_Linear();		//!< MIN MAG MIP ��S��Linear�Ƃ��Đݒ�
	void Set_Filter_Point();		//!< MIN MAG MIP ��S��Point�Ƃ��Đݒ�
	void Set_Filter_Anisotropy(UINT MaxAnisotropy = 16);	//!< �ٕ����t�B���^�ݒ�
	//----------------------------------------------------------

	//! @brief ��r���[�h�ݒ�
	//! Percent Closer Filter(PCF)�̃\�t�g�V���h�E�\�����\�ɂȂ�B\n
	//! ��HLSL���̃T���v����SamplerComparisonState�ŗp�ӂ��ATexture2D��Sample()�ł͂Ȃ��ASampleCmp()��SampleCmpLevelZero()���g�����ƁB
	//! @param[in]	compFunc	�c D3D11_COMPARISON�n�̃t���O���w��
	void Set_ComparisonMode(D3D11_COMPARISON_FUNC compFunc = D3D11_COMPARISON_LESS_EQUAL);

	//! @brief �~�b�v�}�b�v���x���̃o�C�A�X��ݒ肷��
	//! �~�b�v�}�b�v����肩����Ȃ��悤�ɂ������ꍇ�̓}�C�i�X�̒l���w�肷��B��苭�����������ꍇ�̓v���X�l���w�肷��B
	//! @param[in]	bias �c �v�Z���ꂽ�~�b�v�}�b�v ���x������̃I�t�Z�b�g
	void Set_MipLODBias(float bias){
		m_Desc.MipLODBias = bias;
	}
	//! @brief �~�b�v�}�b�v���x���͈̔͂�ݒ�
	//! �Ⴆ�΃~�b�v�}�b�v�����������Ȃ��ꍇ�́AfMin=0 fMax=0)�Ƃ��ɂ���B
	//! @param[in]	fMin	�c �~�b�v�}�b�v�͈͂̉���
	//! @param[in]	fMax	�c �~�b�v�}�b�v�͈͂̏��
	void Set_MipLOD(float fMin, float fMax){
		m_Desc.MinLOD = fMin;
		m_Desc.MaxLOD = fMax;
	}

	//! @brief desc�̓��e���R�s�[����
	void Set_FromDesc(const D3D11_SAMPLER_DESC& desc) {
		YsSafe_Release(m_pSampler);
		m_Desc = desc;
	}


	//===============================================
	// �쐬�E�Z�b�g
	//===============================================
	//! @brief ���݂�Desc�̓��e�����ɃT���v���쐬
	//! @return �쐬����:true
	bool Create();

	//! @brief �T���v�����s�N�Z���V�F�[�_�[�̃p�C�v���C�� �X�e�[�W�ɃZ�b�g����B
	//! �X�e�[�g�I�u�W�F�N�g���쐬����Ă��Ȃ��ꍇ�́A�쐬����
	//! @param[in]	StartSlot	�c �Z�b�g����X���b�g�ԍ�
	void SetStatePS(UINT StartSlot = 0);

	//! @brief �T���v���𒸓_�V�F�[�_�[�̃p�C�v���C�� �X�e�[�W�ɃZ�b�g����B
	//! �X�e�[�g�I�u�W�F�N�g���쐬����Ă��Ȃ��ꍇ�́A�쐬����
	//! @param[in]	StartSlot	�c �Z�b�g����X���b�g�ԍ�
	void SetStateVS(UINT StartSlot = 0);
	
	//! @brief �T���v�����W�I���g���V�F�[�_�[�̃p�C�v���C�� �X�e�[�W�ɃZ�b�g����B
	//! �X�e�[�g�I�u�W�F�N�g���쐬����Ă��Ȃ��ꍇ�́A�쐬����
	//! @param[in]	StartSlot	�c �Z�b�g����X���b�g�ԍ�
	void SetStateGS(UINT StartSlot = 0);
	
	//! @brief �T���v�����R���s���[�g�V�F�[�_�[�̃p�C�v���C�� �X�e�[�W�ɃZ�b�g����B
	//! �X�e�[�g�I�u�W�F�N�g���쐬����Ă��Ȃ��ꍇ�́A�쐬����
	//! @param[in]	StartSlot	�c �Z�b�g����X���b�g�ԍ�
	void SetStateCS(UINT StartSlot = 0);

	//===============================================
	//! @brief ���
	//===============================================
	void Release(){
		YsSafe_Release(m_pSampler);
	}

	//===============================================
	// �ݒ���擾
	//===============================================
	//! @brief �ݒ���擾
	const D3D11_SAMPLER_DESC&	GetDesc() { return m_Desc; }
	//! @brief �X�e�[�g�I�u�W�F�N�g�擾
	ID3D11SamplerState*			GetState() { return m_pSampler; }

	//===============================================
	YsSamplerState() : m_pSampler(0) {
	}
	~YsSamplerState() {
		Release();
	}

private:
	D3D11_SAMPLER_DESC		m_Desc;			//!< �ݒ���
	ID3D11SamplerState*		m_pSampler;		//!< �X�e�[�g�I�u�W�F�N�g

private:
	// �R�s�[�֎~�p
	YsSamplerState(const YsSamplerState& src) {}
	void operator=(const YsSamplerState& src) {}
};

//================================================
//! @brief ���X�^���C�U�X�e�[�g�N���X
//!
//!�@3D����2D�ւ̃����_�����O���̓���ݒ�
//================================================
class YsRasterizeState {
public:
	//===============================================
	// �쐬�E�Z�b�g
	//  ���܂��f�o�C�X�ւ̓Z�b�g����܂���BSetState()�ŃZ�b�g����܂��B
	//  ���܂��A�����̊֐��̓X�e�[�g�I�u�W�F�N�g��������܂�(�č쐬���K�v�Ȃ���)
	//===============================================

	//----------------------------------------------------------
	//! @brief �W���I�ȏ��쐬
	//----------------------------------------------------------
	void SetAll_Standard();

	//----------------------------------------------------------
	//! @brief ���݂̃p�C�v���C������擾���������g��
	//----------------------------------------------------------
	void SetAll_GetState();

	//----------------------------------------------------------
	//! @brief �J�����O���[�h�ݒ�F�\�ʂ̂ݕ`��
	void Set_CullMode_Back() {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = D3D11_CULL_BACK;
	}
	//! @brief �J�����O���[�h�ݒ�F���ʂ̂ݕ`��
	void Set_CullMode_Front() {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = D3D11_CULL_FRONT;
	}
	//! @brief �J�����O���[�h�ݒ�F���ʕ`��
	void Set_CullMode_None() {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = D3D11_CULL_NONE;
	}
	//! @brief �J�����O���[�h�ݒ�
	void Set_CullMode(D3D11_CULL_MODE mode) {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = mode;
	}

	//! @brief �ʏ�
	void Set_FillMode_Solid() {
		YsSafe_Release(m_pRasterize);
		m_Desc.FillMode = D3D11_FILL_SOLID;
	}
	//! @brief ���C���[�t���[��
	void Set_FillMode_Wireframe() {
		YsSafe_Release(m_pRasterize);
		m_Desc.FillMode = D3D11_FILL_WIREFRAME;
	}

	//! @brief desc�̓��e���R�s�[����
	void Set_FromDesc(const D3D11_RASTERIZER_DESC& desc){
		YsSafe_Release(m_pRasterize);
		m_Desc = desc;
	}
	//----------------------------------------------------------

	//===============================================
	// �쐬�E�Z�b�g
	//===============================================
	//! @brief ���݂�Desc�̓��e�����ɃT���v���쐬
	//! @return �쐬����:true
	bool Create();

	//! @brief ���X�^���C�U�[�X�e�[�g���f�o�C�X�փZ�b�g
	//!  �X�e�[�g�I�u�W�F�N�g���쐬����Ă��Ȃ��ꍇ�́A�쐬����
	void SetState();

	//! @brief ���
	void Release(){
		YsSafe_Release(m_pRasterize);
	}

	//===============================================
	// �ݒ���擾
	//===============================================
	//! @brief �ݒ���擾
	const D3D11_RASTERIZER_DESC&	GetDesc() { return m_Desc; }
	//! @brief ���X�^���C�U�[�X�e�[�g�I�u�W�F�N�g�擾
	ID3D11RasterizerState*			GetState() { return m_pRasterize; }

	//===============================================
	YsRasterizeState() : m_pRasterize(0) {
	}

	~YsRasterizeState() {
		Release();
	}

private:
	D3D11_RASTERIZER_DESC	m_Desc;			//!< �ݒ���
	ID3D11RasterizerState*	m_pRasterize;	//!< �X�e�[�g�I�u�W�F�N�g

private:
	// �R�s�[�֎~�p
	YsRasterizeState(const YsRasterizeState& src) {}
	void operator=(const YsRasterizeState& src) {}
};

//================================================================
//! @brief �����_�[�^�[�Q�b�g�AZ�o�b�t�@�̐؂�ւ�����N���X
//!
//!	MRT(�}���`�����_�[�^�[�Q�b�g)���l������RT�ADepth������s��
//!
//================================================================
class YsRenderTargets {
public:
	//====================================================
	// �ݒ�
	//====================================================

	//----------------------------------------------------------
	// �o�^�n

	//! @brief �w��No�̃����_�[�^�[�Q�b�g�Ƃ��ēo�^(�f�o�C�X�ɂ͂܂��Z�b�g���Ȃ�)
	//! @param[in]	rtNo	�c �Z�b�g���郌���_�[�^�[�Q�b�g�ԍ�
	//! @param[in]	rtTex	�c ���̃e�N�X�`������RenderTargerView��o�^����
	void RT(int rtNo, YsTexture& rtTex);

	//! @brief �w��No�̃����_�[�^�[�Q�b�g�Ƃ��ēo�^(�f�o�C�X�ɂ͂܂��Z�b�g���Ȃ�)
	//! @param[in]	rtNo	�c �Z�b�g���郌���_�[�^�[�Q�b�g�ԍ�
	//! @param[in]	rtView	�c ����RenderTargerView��o�^����
	void RT(int rtNo, ID3D11RenderTargetView* rtView);	// View�w��Ver

	//! @brief rtdata�ɓo�^����Ă���A�w��No�̃����_�[�^�[�Q�b�g���擾���o�^(�f�o�C�X�ɂ͂܂��Z�b�g���Ȃ�)
	void RT(int rtNo, YsRenderTargets& rtdata);

	//! @brief �[�x�X�e���V����o�^(�f�o�C�X�ɂ͂܂��Z�b�g���Ȃ�)
	//! @param[in]	depthTex	�c ���̃e�N�X�`������DepthStencilView��o�^����
	void Depth(YsTexture& depthTex);

	//! @brief �[�x�X�e���V����o�^(�f�o�C�X�ɂ͂܂��Z�b�g���Ȃ�)
	//! @param[in]	depthView	�c ����DepthStencilView��o�^����
	void Depth(ID3D11DepthStencilView* depthView);	// View�w��Ver

	//! @brief �[�x�X�e���V����o�^(�f�o�C�X�ɂ͂܂��Z�b�g���Ȃ�)
	//! @param[in]	rtdata	�c ����YsRenderTargets�ɓo�^����Ă���Depth��o�^
	void Depth(YsRenderTargets& rtdata);
	//----------------------------------------------------------

	//----------------------------------------------------------
	// �f�o�C�X����擾�n

	//! @brief ���݃f�o�C�X�ɃZ�b�g����Ă���S�Ă�RT��DepthStencil���A���̃N���X�Ɏ擾����
	void GetNowAll();

	//! @brief ���݃f�o�C�X�ɃZ�b�g����Ă���S�Ă�RT���A���̃N���X�Ɏ擾����
	void GetNowRTs();

	//! @brief ���݃f�o�C�X�ɃZ�b�g����Ă���DepthStencil���A���̃N���X�Ɏ擾����
	void GetNowDepth();

	//----------------------------------------------------------

	//! @brief �o�^����Ă������S�ĉ���
	void Release();


	//====================================================
	// �f�o�C�X�փZ�b�g
	//====================================================

	//! @brief ���̃N���X�ɓo�^����Ă���RT�Depth���A���ۂɃf�o�C�X�փZ�b�g����(RT�ADepth�����ۂɐ؂�ւ��邱��)
	//! ��0�Ԗڂ�RT�̃T�C�Y�Ńr���[�|�[�g���ύX���Ă��܂��B
	//! @param[in] setRTNum		: �Z�b�g����RT�̐��B-1�ł��ׂăZ�b�g(8��)�B�Ⴆ�� 2 ���ƁA0��1��2�����Z�b�g�����B
	void SetToDevice(int setRTNum = -1);

	//====================================================
	// �擾
	//====================================================

	//! @brief ���݂��̃N���X�ɓo�^����Ă���RT���擾(�f�o�C�X��RT�擾�ł͂���܂���)
	//! ���߂�l�̃r���[�́A�Q�ƃJ�E���^��AddRef���܂���
	//! @param[in] rtNo	�c RT�ԍ�(0�`7)
	//! @return ���̃N���X�ɓo�^����Ă���rtNo�Ԗڂ̃����_�[�^�[�Q�b�g�r���[
	ID3D11RenderTargetView* GetRT(int rtNo) { return m_pRTs[rtNo]; }

	//! @brief ���݂��̃N���X�ɓo�^����Ă���Depth���擾(�f�o�C�X��Depth�擾�ł͂���܂���)
	//! ���߂�l�̃r���[�́A�Q�ƃJ�E���^��AddRef���܂���
	//! @return ���̃N���X�ɓo�^����Ă���idx�Ԗڂ̃f�v�X�X�e���V���r���[
	ID3D11DepthStencilView* GetDepth() { return m_pDepth; }

	//! @brief �w��RT�̏��擾
	//! @param[in]	rtNo	�c �����擾������RT�ԍ�
	//! @param[out]	outDesc	�c �������邽�߂̕ϐ�
	//! @return true:����Ɏ擾�ł���
	bool GetRTDesc(int rtNo, D3D11_TEXTURE2D_DESC& outDesc);


	//====================================================

	// 
	YsRenderTargets() {
		ZeroMemory(m_pRTs, sizeof(m_pRTs));
		m_pDepth = nullptr;
	}
	// 
	~YsRenderTargets() {
		Release();
	}

	// �R�s�[
	YsRenderTargets(const YsRenderTargets& src) : YsRenderTargets(){
		*this = src;
	}
	void operator=(const YsRenderTargets& src);

private:
	ID3D11RenderTargetView*		m_pRTs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];	//!< RT�ݒ���L���p
	ID3D11DepthStencilView*		m_pDepth;										//!< Depth�ݒ���L���p
	D3D11_VIEWPORT				m_VP;											//!< �r���[�|�[�g�擾�p

};


//! @}

}

#endif
