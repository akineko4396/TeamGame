//===============================================================
//! @file YsUVAnimation.h
//! @brief �L�[�t���[���A�j���[�V�����n�̃N���X�Q
//!
//! @author ���c
//===============================================================
#ifndef __YsUVAnimation_h__
#define __YsUVAnimation_h__

namespace YskLib
{

//=======================================================================
//!
//! @brief UV�A�j���[�V�����Ǘ��N���X
//!
//! 1���̉摜���A�c�Ɖ��ɓ��Ԋu�����A�j���[�V��������Ƃ��Ɏg�p����		\n
//!																		\n
//! �EInit�֐��ŁA�c�Ɖ��̕�������ő�A�j������ݒ肷��B				\n
//! �EAnimation�֐��ŁA�A�j���[�V�����ʒu��i�߂�B						\n
//! �EGetUV�֐��ŁA���݂̃A�j���ʒu����AUV���W���Z�o���Ă����B
//!
//! @ingroup Animation
//=======================================================================
class YsUVAnimator {
public:
	//------------------------------------------------------------------------
	//! @brief �A�j���[�V�������ݒ�
	//! @param[in]	xCount		�c �� ������
	//! @param[in]	yCount		�c �c ������
	//! @param[in]	nAnimeNum	�c �ő�A�j�����@-1�� xCount*yCount �ɂȂ� 
	//------------------------------------------------------------------------
	void Init(int xCount = 1, int yCount = 1, int animeMaxNum = -1) {
		m_xCnt = xCount;
		m_yCnt = yCount;
		if (animeMaxNum < 0) {
			m_AnimeMaxNum = m_xCnt * m_yCnt;
		}
		else {
			m_AnimeMaxNum = animeMaxNum;
		}

		m_fAnimePos = 0;
	}

	//------------------------------------------------------------------------
	//! @brief �A�j���[�V�����i�s
	//! @param[in]	fSpeed	�c �A�j�����x
	//! @param[in]	bLoop	�c ���[�v�Đ�����H
	//------------------------------------------------------------------------
	void Animation(float fSpeed, bool bLoop);

	//------------------------------------------------------------------------
	//! @brief ���݂̃A�j���ʒu����AUV���擾
	//! @param[out]	uvMin	�c ��`�̍�����W(UV���W�n 0�`1)���i�[�����
	//! @param[out]	uvMax	�c ��`�̉E�����W(UV���W�n 0�`1)���i�[�����
	//------------------------------------------------------------------------
	void GetUV(YsVec2* uvMin, YsVec2* uvMax) {
		int No = (int)m_fAnimePos;
		int x = No % m_xCnt;
		int y = No / m_xCnt;
		float xAddr = 1.0f / m_xCnt;
		float yAddr = 1.0f / m_yCnt;

		if (uvMin) {
			uvMin->x = xAddr*x;
			uvMin->y = yAddr*y;
		}
		if (uvMax) {
			uvMax->x = xAddr*(x + 1);
			uvMax->y = yAddr*(y + 1);
		}
	}

	//------------------------------------------------------------------------
	//! @brief �A�j���ʒu�ύX
	//! @param[in]	idx	�c �A�j���̃R�}�ԍ�
	//------------------------------------------------------------------------
	void SetPos(int idx) {
		m_fAnimePos = (float)idx;
	}

	//------------------------------------------------------------------------
	//! @brief ���݂̃A�j���ʒu���擾
	//------------------------------------------------------------------------
	float GetPos() { return m_fAnimePos; }

	//------------------------------------------------------------------------
	//! @brief �A�j���I�������H
	//! �����[�v�Đ�����true���Ԃ��Ă��܂���
	//! @return true:�Đ��I��
	//------------------------------------------------------------------------
	bool IsEnd() {
		if ((int)m_fAnimePos >= m_AnimeMaxNum - 1)return true;
		return false;
	}


	//------------------------------------------------------------------------
	//! @brief �A�j���������擾
	//------------------------------------------------------------------------
	int GetMaxNum() { return m_AnimeMaxNum; }

	// 
	YsUVAnimator() : m_xCnt(1), m_yCnt(1), m_AnimeMaxNum(1), m_fAnimePos(0){
	}
	
private:
	int					m_xCnt, m_yCnt;		//!< �A�j���[�V������X,Y������
	int					m_AnimeMaxNum;		//!< �A�j���[�V�������鐔
	float				m_fAnimePos;		//!< ���݂̃A�j���̈ʒu

};


}

#endif
