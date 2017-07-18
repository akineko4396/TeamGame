//===============================================================
//! @file YsSingleModel.h
//! @brief ���b�V�����f���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsSingleModel_h__
#define	__YsSingleModel_h__

namespace YskLib
{

//==========================================================
//! @brief �}�e���A���f�[�^�\����
//!
//!  �P�̍ގ��̏��
//!
//! @ingroup Graphics_Model
//==========================================================
struct YsMaterial {
	//===========================================================
	// �}�e���A���f�[�^
	//===========================================================

	YsVec4			Diffuse;			//!< �g�U�F(��{�F)
	YsVec4			Specular;			//!< ���ːF
	float			Power;				//!< ���˂̋���
	YsVec3			Ambient;			//!< ���F
	YsVec4			Emissive;			//!< ���ȏƖ�

	std::string		TextureFileName;	//!< �e�N�X�`����

	std::string		ExtScript;			//!< �g���X�N���v�g������

	YsVec2			UV_Tiling;			//!< UV�^�C�����O��
	YsVec2			UV_Offset;			//!< UV�I�t�Z�b�g�l

	//------------------------------------------------------------------------
	//! @brief �e�N�X�`���Z�b�g
	//!  �������̃e�N�X�`�����P�̍\���̂ɂ܂Ƃ߂������̂���
	//------------------------------------------------------------------------
	YsTextureSet	TexSet;				//!< �e�N�X�`���Z�b�g

	//------------------------------------------------------------------------
	//! @brief �`��t���O(XED��p)
	//------------------------------------------------------------------------
	char		DrawFlag;
		//! @brief �`��t���O�p�萔
		enum {
			DF_BOTHFACE = 0x01,	//!< ���ʕ`��
		};

	//! @brief ���ʕ`�悪�L�����ǂ���
	bool DF_IsBothFace() { return (DrawFlag & DF_BOTHFACE ? true : false); }

	// 
	YsMaterial(){
		Power = 0;
		DrawFlag = 0;
		UV_Tiling.Set(1, 1);
		UV_Offset.Set(0, 0);
	}
};



//==========================================================
//! @brief �P�ꃁ�b�V�����f���N���X
//!
//!  YsMesh�ƃ}�e���A����g�ݍ��킹�������̂���
//!
//! @ingroup Graphics_Model
//==========================================================
class YsSingleModel{
public:
	//=====================================
	// �ǂݍ��݁E���
	//=====================================

	//! @brief X�t�@�C���ǂݍ���
//	bool LoadXFile(const std::string& filename);

	//------------------------------------------------------------------------
	//! @brief ���
	//------------------------------------------------------------------------
	void Release();

	//------------------------------------------------------------------------
	//! @brief �`��ɕK�v�ȃf�[�^���Z�b�g
	//! �ȉ��̕����f�o�C�X�փZ�b�g����
	//! �E���_�o�b�t�@
	//! �E�C���f�b�N�X�o�b�t�@
	//! �E�v���~�e�B�u�E�g�|���W�[
	//------------------------------------------------------------------------
	void SetDrawData();

	//=====================================
	// �擾
	//=====================================

	//! @brief ���b�V���f�[�^���擾
	SPtr<YsMesh>						GetMesh(){return m_pMesh;}

	//! @brief �}�e���A���f�[�^���擾
	std::vector<YsMaterial>&			GetMaterials(){return m_Materials;}

	//! @brief ���b�V�����擾
	std::string&						GetName(){ return m_Name; }

	//=====================================
	// �ݒ�
	//=====================================

	//------------------------------------------------------------------------
	//! @brief YsMesh���O������Z�b�g����
	//! @param[in]	mesh	�c �Z�b�g����YsMesh�N���X
	//------------------------------------------------------------------------
	void SetMesh(SPtr<YsMesh> mesh) {
		m_pMesh = mesh;
	}

	//------------------------------------------------------------------------
	//! @brief �}�e���A�����X�g���O������Z�b�g����
	//! @param[in]	materials	�c �Z�b�g����}�e���A���z��
	//------------------------------------------------------------------------
	void SetMaterials(const std::vector<YsMaterial>& materials){
		m_Materials.clear();
		m_Materials = materials;
	}

	//------------------------------------------------------------------------
	//! @brief ���b�V�����ݒ�
	//! @param[in]	meshName	�c ���b�V�����ݒ�
	//------------------------------------------------------------------------
	void SetName(const std::string& meshName){ m_Name = meshName; }

	//
	YsSingleModel(){
	}
	~YsSingleModel(){
		Release();
	}

private:
	std::string						m_Name;			//!< ���b�V����

	SPtr<YsMesh>					m_pMesh;		//!< ���b�V���f�[�^
	std::vector<YsMaterial>			m_Materials;	//!< �}�e���A���f�[�^

private:
	// �R�s�[�֎~�p
	YsSingleModel(const YsSingleModel& src) {}
	void operator=(const YsSingleModel& src) {}
};

}

#endif
