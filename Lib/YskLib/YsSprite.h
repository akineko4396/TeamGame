//===============================================================
//
//! @file YsSprite.h
//! @brief 2D�`��N���X
//!
//! @author ���c
//
//===============================================================
#ifndef __YsSprite_h__
#define __YsSprite_h__

namespace YskLib{

//======================================================================
//!
//! @brief 2D�`��pSpriteBatch�N���X
//!
//!	DirectX Tool Kit��SpriteBatch�����b�v�����N���X
//!	�܂Ƃ߂ĕ`��ł���̂ō�����2D�`�悪�\
//!
//! @ingroup Graphics_Important
//======================================================================
class YsSprite {
public:
	//--------------------------------------------------------------------------
	//! @brief ������
	//! @param[in]	devContext	�c �f�o�C�X�R���e�L�X�g
	//--------------------------------------------------------------------------
	void Init(ID3D11DeviceContext* devContext);

	//--------------------------------------------------------------------------
	//! @brief ���
	//--------------------------------------------------------------------------
	void Release();

	//--------------------------------------------------------------------------
	//! @brief �o�b�`�`��J�n
	//!
	//! �e�X�e�[�g�͋L������AEnd()�ŕ�������܂��BBegin()�`End()�ԂłȂɂ��X�e�[�g��؂�ւ��Ă������ɂȂ�܂��B	\n
	//! bImmediateMode��false�̏ꍇ�́ADraw()���ɂ����ɂ͕`�悳�ꂸ�AEnd()���ɂ܂Ƃ߂ĕ`�悳���̂Ńp�t�H�[�}���X�������邪�A\n
	//! Begin()�`End()�Ԃ�BlendState�Ȃǂ�؂�ւ��邱�Ƃ��o���Ȃ��B	\n
	//! \n
	//! bImmediateMode��false�̏ꍇ�́ADraw()���ĂԂƑ����ɕ`�悪���s����邪�A��ʂɕ`�悷��ƃp�t�H�[�}���X��������B	\n
	//! ���̂Ԃ�ABegin()�`End()�ԂŃX�e�[�g��؂�ւ��邱�Ƃ��\�B
	//! 
	//! @param[in]	bImmediateMode	�c �����ɕ`�悷�邩�H true:Draw()�֐����ĂԂƂ����ɕ`�悳��� false:End()���ɂ܂Ƃ߂ĕ`�悳���B
	//!	@param[in]	blendState		�c �摜�̍������@�ł���u�����h�X�e�[�g�I�u�W�F�N�g��n���Bnullptr���ƒʏ�̃A���t�@�����ɂȂ�BbImmediateMode��false�̎���End()�Ŏg����B
	//--------------------------------------------------------------------------
	void Begin(bool bImmediateMode = false, YsBlendState* blendState = nullptr);

	//--------------------------------------------------------------------------
	//! @brief �o�b�`�`��I��(���ۂɂ܂Ƃ߂ĕ`�悳���)
	//! �e�X�e�[�g��Begin()���ĂԑO�̏�Ԃɕ������܂�
	//--------------------------------------------------------------------------
	void End();

	//============================================================
	// �`��
	//  �e�`��֐���Begin()�`End()�ԂŌĂԂƌ����ǂ��B
	//  ��Begin()���Ă�ł��Ȃ��ꍇ�ł������Ŏ�����Begin(),End()���܂��������͈����B
	//============================================================
	// �e�N�X�`���`��

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �s������ɕ`��B���s���Z��]�̂݉\�B	\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵�����e�N�X�`��
	//!  @param[in]	mat		�c �ϊ��s��
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, const YsMatrix& mat, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �s������ɕ`��B���s���Z��]�̂݉\�B	\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵�����e�N�X�`��
	//!  @param[in]	mat		�c �ϊ��s��
	//!  @param[in]	sx		�c (�`�挳)X���W
	//!  @param[in]	sy		�c (�`�挳)Y���W
	//!  @param[in]	sw		�c (�`�挳)��
	//!  @param[in]	sh		�c (�`�挳)����
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, const YsMatrix& mat, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �s������ɕ`��B���s���Z��]�̂݉\�B	\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵����YsTexture
	//!  @param[in]	mat		�c �ϊ��s��
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, const YsMatrix& mat, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), mat, Color);
	}

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �s������ɕ`��B���s���Z��]�̂݉\�B	\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵����YsTexture
	//!  @param[in]	mat		�c �ϊ��s��
	//!  @param[in]	sx		�c (�`�挳)X���W
	//!  @param[in]	sy		�c (�`�挳)Y���W
	//!  @param[in]	sw		�c (�`�挳)��
	//!  @param[in]	sh		�c (�`�挳)����
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, const YsMatrix& mat, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), mat, sx, sy, sw, sh, Color);
	}

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �`���x,y,w,h�w��		\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵�����e�N�X�`��
	//!  @param[in]	x		�c X���W
	//!  @param[in]	y		�c Y���W
	//!  @param[in]	w		�c ��
	//!  @param[in]	h		�c ����
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, int x, int y, int w, int h, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �`���x,y,w,h�w��		\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵�����e�N�X�`��
	//!  @param[in]	x		�c (�`���)X���W
	//!  @param[in]	y		�c (�`���)Y���W
	//!  @param[in]	w		�c (�`���)��
	//!  @param[in]	h		�c (�`���)����
	//!  @param[in]	sx		�c (�`�挳)X���W
	//!  @param[in]	sy		�c (�`�挳)Y���W
	//!  @param[in]	sw		�c (�`�挳)��
	//!  @param[in]	sh		�c (�`�挳)����
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, int x, int y, int w, int h, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �`���x,y,w,h�w��		\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵����YsTexture
	//!  @param[in]	x		�c X���W
	//!  @param[in]	y		�c Y���W
	//!  @param[in]	w		�c ��
	//!  @param[in]	h		�c ����
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, int x, int y, int w, int h, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), x, y, w, h, Color);
	}

	//--------------------------------------------------------------------------
	//! @brief �e�N�X�`���`��
	//!
	//! �`���x,y,w,h�w��		\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//!  @param[in]	tex		�c �`�悵����YsTexture
	//!  @param[in]	x		�c (�`���)X���W
	//!  @param[in]	y		�c (�`���)Y���W
	//!  @param[in]	w		�c (�`���)��
	//!  @param[in]	h		�c (�`���)����
	//!  @param[in]	sx		�c (�`�挳)X���W
	//!  @param[in]	sy		�c (�`�挳)Y���W
	//!  @param[in]	sw		�c (�`�挳)��
	//!  @param[in]	sh		�c (�`�挳)����
	//!  @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, int x, int y, int w, int h, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), x, y, w, h, sx, sy, sw, sh, Color);
	}

	//============================================================
	// �t�H���g�`��
	//============================================================

	//--------------------------------------------------------------------------
	//! @brief �t�H���g�`��
	//!
	//! �s������ɕ`��B���s���Z��]�̂݉\�B	\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//! @param[in]	text	�c �`�悷�镶����
	//! @param[in]	mat		�c �ϊ��s��
	//! @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void DrawFont(const std::string& text, const YsMatrix& mat, const YsVec4* Color);

	//--------------------------------------------------------------------------
	//! @brief �t�H���g�`��
	//!
	//! ���W�ƐF�̂ݎw��	\n
	//! ��ʂɌĂԏꍇ��Being()�`End()�ԂŎ��s���邱�ƂŃp�t�H�[�}���X��������܂��B
	//!
	//! @param[in]	text	�c �`�悷�镶����
	//! @param[in]	x		�c X���W
	//! @param[in]	y		�c Y���W
	//! @param[in]	Color	�c �F
	//--------------------------------------------------------------------------
	void DrawFont(const std::string& text, int x, int y, const YsVec4* Color);


	//! @brief �X�v���C�g�o�b�`�I�u�W�F�N�g�擾
	DirectX::SpriteBatch* GetSpriteBatch() { return m_pSB; }


	// 
	YsSprite(){
	}
	// 
	~YsSprite();

private:
	
	DirectX::SpriteBatch*	m_pSB = nullptr;		//!< �X�v���C�g�o�b�`
	DirectX::SpriteFont*	m_pFont = nullptr;		//!< �X�v���C�g�t�H���g
	bool					m_IsBegin = false;		//!< Begin�����H

	//! �e�X�e�[�g�ۑ��p
	struct States {
		YsBlendState bs;
		YsRasterizeState rs;
		YsSamplerState ss;
		YsDepthStencilState ds;
	};
	UPtr<States>	m_SaveStates;			//!< �X�e�[�g�����p�f�[�^

	//! �g�p�X�e�[�g
	YsBlendState m_bsAlpha;

private:
	// �R�s�[�֎~�p
	YsSprite(const YsSprite& src) {}
	void operator=(const YsSprite& src) {}
};

}
#endif
