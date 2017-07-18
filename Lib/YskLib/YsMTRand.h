//===============================================================
//! @file YsMTRand.h
//! @brief �����N���X
//!
//! @author ���c
//===============================================================

#ifndef __YsMTRand_h__
#define __YsMTRand_h__

namespace YskLib {

//=================================================================
//!
//! @brief �����Z���k��c�C�X�^�[�����N���X
//!
//! @ingroup Etc
//=================================================================
class YsMTRand {
public:
	//! @brief �����V�[�h��ݒ肷��(�܂�V���b�t��)
	//! @param[in]	seed	�c �����V�[�h
	void InitSeed(ULONG seed) {
		m_Rnd.seed(seed);
	}

	//! @brief �����̗����擾
	UINT GetInt() { return m_Rnd(); }

	//! @brief �����̗����擾
	//! @param[in]	num	�c 0 �` num-1 �܂ł̗����ɂȂ�
	int GetInt(int num) {
		return m_Rnd() % num;
	}

	//! @brief  st �` ed�Ԃ̐��������𔭐�
	//! @param[in]	st	�c �ŏ��l
	//! @param[in]	ed	�c �ő�l
	int GetInt(int st, int ed) {
		if(ed < st)std::swap(ed, st);
		return (int)GetInt(ed - st + 1) + st;
	}

	//! @brief st �` ed�Ԃ̕��������_�����𔭐�
	//! @param[in]	st	�c �ŏ��l
	//! @param[in]	ed	�c �ő�l
	float GetFloat(float st, float ed) {
		if(ed < st)std::swap(ed, st);
		return (GetInt() / 4294967295.0f)*(ed - st) + st;
	}

	//
	YsMTRand() {}
	YsMTRand(ULONG seed) : m_Rnd(seed) {
	}

private:
	std::mt19937	m_Rnd;		//!< �����Z���k�E�c�C�X�^�[����
};

}

#endif
