//===============================================================
//! @file YsBillBoard.h
//! @brief �|���S���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsBillBoard_h__
#define __YsBillBoard_h__

namespace YskLib{

//=======================================================================
//!
//! @brief �l�p�`�|���S���I�u�W�F�N�g�N���X
//!
//! �����ł́A��x��:1x1 UV:0�`�P �F:���@�ɂȂ�	\n
//!  ���Ƃ�Set�n�̊֐��ŕύX����				\n
//!�@[��ȋ@�\]									\n
//!�@�E�e�N�X�`��								\n
//!�@�EUV�A�j���[�V����							\n
//!�@�E�F										\n
//!
//! @ingroup Graphics_Model
//=======================================================================
class YsBillBoard {
public:

	//=======================================================================
	// ���_�ݒ�
	//=======================================================================
	
	//------------------------------------------------------------------------
	//! @brief ���_���W�ݒ�
	//! X,Y���ʂɒ��_��z�u����܂��BZ��0�B	\n
	//! 3D���W�n�ōl���邱��(Y�͏�)
	//! @param[in]	x	�c X���W
	//! @param[in]	y	�c Y���W
	//! @param[in]	w	�c ��
	//! @param[in]	z	�c ����
	//------------------------------------------------------------------------
	void SetVertex(float x, float y, float w, float h);

	//! @brief �F�擾
	YsVec4&		GetColor() { return m_Color; }

	//! @brief �F�ݒ�
	//! @param[in]	color	�c �Z�b�g����F
	void SetColor(const YsVec4& color){
		m_Color = color;
	}

	//! @brief �e�N�X�`���Z�b�g���擾
	YsTextureSet&	GetTex(){ return m_TexSet; }

	//! @brief �e�N�X�`���Z�b�g���Z�b�g����
	//! @param[in]	texSet	�c �Z�b�g����TextureSet
	void SetTex(const YsTextureSet* texSet){
		m_TexSet = *texSet;
	}

	//! @brief �P�i�̃e�N�X�`�����Z�b�g
	//! @param[in]	tex	�c �Z�b�g����e�N�X�`��
	void SetTex(const SPtr<YsTexture> tex){
		m_TexSet.MeshTex = tex;
	}

	//=======================================================================
	// UV�A�j���[�V�����֌W
	//=======================================================================

	//! @brief UV�A�j���[�^�[�擾
	YsUVAnimator&	GetAnime() { return m_Anime; }

	//=======================================================================
	// �`��
	//=======================================================================

	//! @brief �`��
	void Draw();



	//
	YsBillBoard() : m_Color(1, 1, 1, 1){
		m_xCnt = m_yCnt = 1;
		m_AnimeNum = 0;
		m_fAnimePos = 0;

		SetVertex(-0.5f, -0.5f, 1, 1);
		SetColor(YsVec4(1, 1, 1, 1));

		for (int i = 0; i < 4; i++) {
			m_V[i].Color = YsVec4(1,1,1,1);
		}

	}

	~YsBillBoard(){
	}

private:

	YsVertex_Pos_UV_Color	m_V[4];		//!< ���_�f�[�^

	YsVec4				m_Color;		//!< �F

	YsTextureSet		m_TexSet;		//!< �e�N�X�`���Z�b�g


	// �A�j���[�V�����p
	int					m_xCnt, m_yCnt;		//!< �A�j���[�V������X,Y������
	int					m_AnimeNum;			//!< �A�j���[�V�������鐔
	float				m_fAnimePos;		//!< ���݂̃A�j���̈ʒu
	YsUVAnimator		m_Anime;

	//
	SPtr<YsPolygon>		m_pPoly;

	//! ��Ɨp�|���S���f�[�^(�S�Ă�YsLaser�Ŏg���܂킵�܂�)
	static WPtr<YsPolygon>	s_wpPolySquare;
};

}
#endif
