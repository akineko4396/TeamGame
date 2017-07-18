//===============================================================
//! @file YsLaser.h
//! @brief �A���|���S���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsLaser_h__
#define __YsLaser_h__

namespace YskLib{

//===============================================================
//!
//! @brief �Ȑ����[�U�[
//!
//! �A�������|���S���𐶐����`�悷��B	\n
//! �Ȑ����[�U�[�⌕�̋O�Օ\���Ɏg����
//!
//! @ingroup Graphics_Model
//===============================================================
class YsLaser{
public:
	//====================================================
	// ���擾
	//====================================================

	//! @brief ���W���X�g�擾
	std::list<YsVec3>&		GetList(){return m_List;}

	//! @brief ��
	
	float					GetW(){ return m_W; }
	//! @brief �����Z�b�g
	void					SetW(float w){ m_W = w; }
	
	//! @brief �F
	YsVec4&					GetColor(){ return m_Color; }
	
	//! @brief �F���Z�b�g
	void					SetColor(const YsVec4* color){m_Color = *color;}

	//! @brief ���[�U�[�̃|�C���g���擾
	UINT					GetPointNum(){return m_List.size();}

	//! @brief �e�N�X�`���Z�b�g�擾
	YsTextureSet&			GetTex(){ return m_TexSet; }
	//! @brief �e�N�X�`�����Z�b�g
	void					SetTex(const YsTextureSet* texSet){ if(texSet){m_TexSet = *texSet;} }

	//====================================================
	//! @brief �����ݒ�
	//! @param[in]	w		�c ���[�U�[�̕�
	//!	@param[in]	color	�c �F
	//====================================================
	void Init(float w, const YsVec4* color = nullptr){
		m_W = w;
		if(color)m_Color = *color;
		m_List.clear();
	}

	//====================================================
	//! @brief ���X�g���N���A
	//====================================================
	void ClearList(){
		m_List.clear();
	}

	//====================================================
	// �|�C���g�ǉ�
	//====================================================

	//--------------------------------------------------------------------------
	//! @brief ���X�g�̌��ɒǉ�
	//! @param[in]	pos	�c ���W
	//--------------------------------------------------------------------------
	void AddPoint_Back(const YsVec3& pos){
		m_List.push_back(pos);
	}

	//--------------------------------------------------------------------------
	//! @brief ���X�g�̎�O�ɒǉ�
	//! @param[in]	pos	�c ���W
	//--------------------------------------------------------------------------
	void AddPoint_Front(const YsVec3& pos){
		m_List.push_front(pos);
	}

	//====================================================
	// �|�C���g�폜
	//====================================================

	//! @brief ���X�g�̐擪���폜
	void DelPoint_Front(){
		if(m_List.size() > 0){
			m_List.erase(m_List.begin());
		}
	}

	//! @brief ���X�g�̌����폜
	void DelPoint_Back(){
		if(m_List.size() > 0){
			m_List.erase(--m_List.end());
		}
	}

	//====================================================
	// �`��
	//====================================================
	//! @brief �_�̍��E�ɒ��_��z�u���A�Ȑ����[�U�[�Ƃ��ĕ`��
	//!
	//! ����a�����Ȃ������߁A�J��������݂č��E�ɔz�u�����		\n
	//! ���\�\�\���\�\�\���\�\�\���\�\�\��							\n
	//! | �_    | �_    | �_    | �_    |							\n
	//! 0   �_  1   �_  2   �_  3   �_  4 �c�@�㉺�̕���m_W�ƂȂ�	\n
	//! |     �_|     �_|     �_|     �_|							\n
	//! ���\�\�\���\�\�\���\�\�\���\�\�\��							\n
	//! 
	//! @param[in]	vCamPos	�c �J�����̍��W
	void Draw(const YsVec3& vCamPos);

	//! @brief �Q�̓_���P�{�̖_�Ƃ��A���̖_���m���|���S���Ō��ԁB
	//!
	//! ����ɖ_���m�̊ԂɃX�v���C����ԂŒǉ��̖_����芊�炩�ɕ`�悷��B	\n
	//! ����U�������̋O�ՂȂǂŎg�p�B										\n
	//! 1�\�\�\3�\�\�\5�\�\�\7�\�\�\9										\n
	//! |�_    |�_    |�_    |�_    | 										\n
	//! |  �_  |  �_  |  �_  |  �_  | �c�@���K�������œ_�̐���o�^���邱�ƁI\n
	//! |    �_|    �_|    �_|    �_| 										\n
	//! 0�\�\�\2�\�\�\4�\�\�\6�\�\�\8										\n
	//!
	//! @param[in]	detailNum	�c �_�Ԃ̕����� 0���ƕ�ԂȂ�
	void DrawStrip(int detailNum = 5);

protected:

	std::list<YsVec3>		m_List;		//!< ���W���X�g
	float					m_W;		//!< ��
	YsVec4					m_Color;	//!< �F

	YsTextureSet			m_TexSet;	//!< �e�N�X�`���Z�b�g

	SPtr<YsPolygon>			m_pPoly;	//!< �|���S�����f���N���X

	//! ��Ɨp�|���S���f�[�^(�S�Ă�YsLaser�Ŏg���܂킵�܂�)
	static WPtr<YsPolygon>	s_wpPoly;


public:
	YsLaser() : m_Color(1,1,1,1){
		m_W = 0.1f;
	}

};

}
#endif