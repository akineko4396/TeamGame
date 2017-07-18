//===============================================================
//! @file YsModelObject.h
//! @brief ���b�V���I�u�W�F�N�g����N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsModelObject_h__
#define __YsModelObject_h__

namespace YskLib{

//===========================================================================================
//!
//! @brief ���f���I�u�W�F�N�g����N���X
//!
//! YsGameModel�N���X���Q�Ƃ��A���̏������Ƀ{�[���𓮍삳����N���X				\n
//! [��ȋ@�\]																		\n
//! �E�n���ꂽYsGameModel�Ɠ����{�[���\�����쐬										\n
//! �E�A�j���[�V�������s�������ꍇ�́AYsAnimator�N���X��ʓr�g�p����΂悢			\n
//! �E�S�{�[���̃��[���h�s������[�J���s������Ɍv�Z								\n
//!
//! @ingroup Graphics_Model_Important
//===========================================================================================
class YsModelObject{
public:
	class BoneNode;

	//=================================================================================
	// ���擾
	//=================================================================================

	//! @brief �o�^����Ă���Q�[�����f���N���X�擾
	SPtr<YsGameModel>					GetGameModel() { return m_pGameModel; }

	//! @brief ���[�g�{�[���擾�@�{�[�����Ȃ��ꍇ��nullptr
	SPtr<BoneNode>						GetRootBone() { return m_BoneTree.size() == 0 ? nullptr : m_BoneTree[0]; }

	//! @brief �{�[���e�[�u���擾
	std::vector<SPtr<BoneNode>>&		GetBoneTree() { return m_BoneTree; }

	//! @brief �A�j���[�V���������݂��邩�H
	bool								IsAnimationExist() { return m_pGameModel->IsAnimationExist(); }

	//=================================================================================
	// �ݒ�E���
	//=================================================================================

	//------------------------------------------------------------------------
	//! @brief ���f����ݒ肷��
	//!
	//! YsGameModel�����Ƀf�[�^���\�z����
	//!
	//! @param[in] pGameMesh	�c ���f���f�[�^�ł���YsGameModel
	//! @param[in] animatorNum	�c �A�j���Ǘ��N���X�������쐬���邩�H
	//------------------------------------------------------------------------
	bool SetModel(SPtr<YsGameModel> pGameMesh, int animatorNum = 5);

	//------------------------------------------------------------------------
	//! @brief ���b�V����ǂݍ���
	//!
	//!  ���ۂ̓ǂݍ��݂�YsGameModel�N���X�ōs���Ă���		\n
	//!  ���̊֐���YsGameModel���쐬���ă��b�V����Ǎ��ASetMesh()�ǂ�ł邾��
	//!
	//! @param[in]	filename	�c �t�@�C����
	//------------------------------------------------------------------------
	bool LoadMesh(const std::string& filename);

	//------------------------------------------------------------------------
	//! @brief ���
	//------------------------------------------------------------------------
	void Release();

	//------------------------------------------------------------------------
	//! @brief ���̃��f���p�ɃA�j���[�^������������
	//! ��Root�{�[���Ɋւ��ẮA�����g���b�N�u�����h�������ɂȂ�܂�
	//! @param[in]	animator	�c ����������A�j���[�^
	//------------------------------------------------------------------------
	void InitAnimator(YsAnimator& animator);

	//=================================================================================
	// �{�[���s�񉉎Z
	//=================================================================================

	//------------------------------------------------------------------------
	//! @brief m_Mat���x�[�X�ɑS�Ẵ{�[����WorldMat���X�V����(�v�Z�ɂ�TransMat���g�p)�B
	//! ������recCalcBoneMatrix�֐����Ă�ł��邾��
	//! @param[in]	baseMat	�c �����̊�ɂȂ�s��B���̍s���擪�ɑS�{�[�����v�Z���Ă����B
	//------------------------------------------------------------------------
	void CalcBoneMatrix(const YsMatrix* baseMat);

	//! @brief m_Mat���x�[�X�ɑS�Ẵ{�[����WorldMat���X�V����(�v�Z�ɂ�TransMat���g�p)�B
	void CalcBoneMatrix_SkipRoot(const YsMatrix* baseMat);

		//! @brief parentMat���x�[�X�Ɏw��{�[���ȉ���WorldMat���X�V����(�v�Z�ɂ�TransMat���g�p)�B
		void recCalcBoneMatrix(BoneNode& node, const YsMatrix& parentMat);

	//------------------------------------------------------------------------
	//! @brief �S�{�[����TransMat���f�t�H���g�ɖ߂�
	//------------------------------------------------------------------------
	void ResetDefaultTransMat();

	//=================================================================================
	//=================================================================================
	
	//------------------------------------------------------------------------
	//! @brief �{�[���e�[�u������w�肵�����O�̃{�[���̏����擾 ���O�����݂��Ȃ��ꍇ��nullptr���Ԃ�
	//! @param[in]	BoneName	�c �����������{�[����
	//! @return ���݂���ꍇ:�{�[���f�[�^�̃A�h���X�@���݂��Ȃ��ꍇ:nullptr
	//------------------------------------------------------------------------
	SPtr<BoneNode> SearchBone(const std::string& BoneName) {
		for(UINT i = 0; i<m_BoneTree.size(); i++) {
			if(m_BoneTree[i]->pSrcBoneNode->BoneName == BoneName) {
				return m_BoneTree[i];
			}
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	//! @brief �{�[���e�[�u������w�肵�����O�̃{�[���̔ԍ����擾 ���O�����݂��Ȃ��ꍇ��-1���Ԃ�
	//! @param[in]	BoneName	�c �����������{�[����
	//! @return ���݂���ꍇ:�{�[���ԍ��@���݂��Ȃ��ꍇ:-1
	//------------------------------------------------------------------------
	int SearchBoneIndex(const std::string& BoneName){
		for(UINT i = 0; i<m_BoneTree.size(); i++) {
			if(m_BoneTree[i]->pSrcBoneNode->BoneName == BoneName) {
				return i;
			}
		}
		return -1;
	}



	// 
	YsModelObject() {
	}
	~YsModelObject() {
		Release();
	}

private:
	//---------------------------------------------------------------------------------
	//! @brief ���ƂȂ�YsGameModel�̃|�C���^
	//---------------------------------------------------------------------------------
	SPtr<YsGameModel>						m_pGameModel = nullptr;

	//---------------------------------------------------------------------------------
	//! @brief �{�[���c���[���{�[���z��
	//! [0]��Root �c���[�\���ł��邪�A1�����z��Ƃ��Ă��A�N�Z�X�\�B
	//---------------------------------------------------------------------------------
	std::vector<SPtr<BoneNode>>					m_BoneTree;

public:
	//=====================================================================
	//! @brief YsModelObject�p�{�[���m�[�h
	//! @ingroup Graphics_Model_Important
	//=====================================================================
	class BoneNode{
	public:
		SPtr<YsGameModel::BoneNode>	pSrcBoneNode;	//!< ���f�[�^���̃{�[���ւ̎Q��

		WPtr<BoneNode>				Mother;			//!< �e�{�[���ւ̃A�h���X
		std::vector<SPtr<BoneNode>>	Child;			//!< �q�{�[���z��

		int							Level;			//!< �K�w Root��0
		YsMatrix					TransMat;		//!< ���[�J���s��(Animation�֐��ōX�V�����)
		YsMatrix					WorldMat;		//!< ���[���h�s��(CalcBoneMatrix�֐��ōX�V�����)

		//-----------------------------------------------------------
		//! @brief �`�掞�ɕK�v��OffsetWorld�s����Z�o����
		//! @param[out] resultOffsetWorldMat	�c �v�Z���ʂ�����
		//-----------------------------------------------------------
		void CalcOffsetWorldMat(YsMatrix& resultOffsetWorldMat) {
			// Offset�s�� * WorldMat
			YsMatrix::Multiply(resultOffsetWorldMat, pSrcBoneNode->OffsetMat, WorldMat);
		}

		//-----------------------------------------------------------
		//! @brief �{�[��Index�擾
		//! @return �{�[��Index  ���s����-1
		//-----------------------------------------------------------
		int GetBoneIndex() {
			if(pSrcBoneNode == nullptr)return -1;
			return pSrcBoneNode->OffsetID;
		}

		// 
		BoneNode() : Level(0), pSrcBoneNode(0){
		}
	};

private:
	// �R�s�[�֎~�p
	YsModelObject(const YsModelObject& src) {}
	void operator=(const YsModelObject& src) {}
};

}
#endif
