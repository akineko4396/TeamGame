//===============================================================
//! @file YsGameModel.h
//! @brief �Q�[��3D���f���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsGameModel_h__
#define __YsGameModel_h__

#include "YsKeyframeAnimation.h"

namespace YskLib{

//=======================================================================
// ���̃f�[�^
//=======================================================================
struct YsXEDRigidBody {
	std::string rigidBodyName;				// ���̖�
	int boneIndex;							// �֘A�{�[��Index - �֘A�Ȃ��̏ꍇ��-1
	BYTE rigidBodyGroup;					// �O���[�v
	unsigned short unCollisionGroupFlag;	// ��Փ˃O���[�v�t���O
	BYTE shape;								// �`�� - 0:�� 1:�� 2:�J�v�Z��
	YsVec3 size;							// �T�C�Y(x,y,z)
	YsVec3 position;						// �ʒu(x,y,z)
	YsVec3 rotation;						// ��](x,y,z) -> ���W�A���p
	float mass;								// ����
	float moveAttenuation;					// �ړ�����
	float rotationAttenuation;				// ��]����
	float repulsion;						// ������
	float friction;							// ���C��
	BYTE physicsCalcType;					// ���̂̕������Z - 0:�{�[���Ǐ](static) 1:�������Z(dynamic) 2:�������Z + Bone�ʒu���킹

	YsXEDRigidBody() :
		rigidBodyGroup(0),
		unCollisionGroupFlag(0),
		shape(0),
		size(0.1f, 0.1f, 0.1f),
		position(0, 0, 0),
		rotation(0, 0, 0),
		mass(1), moveAttenuation(0.5f), rotationAttenuation(0.5f), repulsion(0), friction(0.5f), physicsCalcType(0)
	{
	}

	// rotation,position����s���Ԃ�
	YsMatrix GetMatrix(){
		YsMatrix m;
		m.CreateRotateZ(rotation.z);
		m.RotateX(rotation.x);
		m.RotateY(rotation.y);
		m.Move(position);
		return m;
	}
};

//=================================================================================
//! @brief �Q�[���ɓK�������@�\���f���N���X
//! 
//! �����̃��f���f�[�^�E�{�[���K�w�\���E�A�j���[�V�����Ȃǂ�ێ�����N���X	\n	
//! �X�L�����b�V���E�X�^�e�B�b�N���b�V�����������܂�						\n
//!	[��ȋ@�\]																\n
//!   �E�����̃��b�V���f�[�^(������YsSingleModel)							\n
//!   �E�{�[���f�[�^														\n
//!   �@���X�^�e�B�b�N���b�V���ł��Œ�P�̓{�[�����ł���(Root�{�[��)		\n
//!       ���������̃{�[���͓������Ă��S�����_�ɂ͉e�����Ȃ�				\n
//!   �E�A�j���[�V�����f�[�^												\n
//!																			\n
//!  YsModelObject�N���X�ɃZ�b�g���Ďg�p����B								\n
//!
//! @ingroup Graphics_Model_Important
//=================================================================================
class YsGameModel : public YsIResource{
public:
	// YsGameModel�p�{�[���m�[�h�N���X
	class BoneNode;

public:
	//=================================================================================
	// ���擾
	//=================================================================================

	//! @brief �{�[���Q�Ɨp�z��擾
	std::vector<SPtr<BoneNode>>&			GetBoneTree() { return m_BoneTree; }
	
	//! @brief ���[�g�{�[���擾
	SPtr<BoneNode>							GetRoot() { return (m_BoneTree.size() == 0 ? nullptr : m_BoneTree[0]); }
	
	//! @brief ���b�V���z��擾(�S��)
	std::vector<SPtr<YsSingleModel>>&		GetModelTbl() {return m_ModelTbl;}
	
	//! @brief ���b�V���z��擾(�X�L�����b�V���̂�)
	std::vector<SPtr<YsSingleModel>>&		GetModelTbl_Skin(){ return m_RefModelTbl_SkinOnly; }
	
	//! @brief ���b�V���z��擾(�X�^�e�B�b�N���b�V���̂�)
	std::vector<SPtr<YsSingleModel>>&		GetModelTbl_Static(){ return m_RefModelTbl_StaticOnly; }
	
	//! @brief �A�j���[�V�������X�g���擾
	std::vector<SPtr<YsAnimationSet>>&		GetAnimeList() { return m_AnimeList; }

	//! @brief ���̃��X�g���擾
	std::vector< std::vector<YsXEDRigidBody> >&	GetRigidBodyTbl(){ return m_RigidBodyTbl; }
	
	//! @brief �t�@�C���܂ł̃p�X�擾
	const std::string&						GetFilePath() const { return m_FilePath; }

	//! @brief �t�@�C�����擾
	const std::string&						GetFileName() const { return m_FileName; }

	//! @brief �p�X���܂߂��t�@�C�����擾
	std::string								GetFullFileName() const { return m_FilePath + m_FileName; }

	//------------------------------------------------------
	//! @brief ���b�V���ǂݍ���(�X�^�e�B�b�N���b�V���A�X�L�����b�V�����Ή�)
	//!	�Ή��`���FX,Xed
	//! @param[in]	fileName	�c �t�@�C����
	//! @param[in]  bakeCurve	�c �Ȑ���Ԍn�̃A�j���L�[�́A�S�Đ��`��ԂƂ��ĕϊ��E�ǉ�����(XED�p �������y���Ȃ�܂�)
	//------------------------------------------------------
	bool LoadMesh(const std::string& fileName, bool bakeCurve = true);

private:
	//------------------------------------------------------
	//! @brief X�t�@�C���ǂݍ��� ��DirectX SDK��D3DX9���g�p���ēǂݍ��݂��Ă܂�
	//!
	//! "//"����n�܂�A�j�����̃A�j���͖�������܂�
	//!
	//------------------------------------------------------
	bool LoadXFile(const std::string& fileName);

	//------------------------------------------------------
	//! @brief XED(�Ǝ��`��)�ǂݍ���
	//!
	//! XED�t�@�C����KAMADA SkinMesh Editor����o�͂ł��郂�f���`���ł�		\n
	//! "//"����n�܂�A�j�����̃A�j���͖�������܂�	\n
	//!
	//! @param[in]  bakeCurve	�c �Ȑ���Ԍn�̃A�j���L�[�́A�S�Đ��`��ԂƂ��ĕϊ��E�ǉ�����(�������y���Ȃ�܂�)
	//------------------------------------------------------
	bool LoadXEDFile(const std::string& fileName, bool bakeCurve = true);

public:
	//------------------------------------------------------
	//! @brief ���
	//------------------------------------------------------
	void Release();

	//=================================================================================
	// ���̑�
	//=================================================================================

	//------------------------------------------------------
	//! @brief �{�[�������� ������Ȃ��ꍇ��nullptr
	//! @param[in]	Name	�c �{�[����
	//! @return �{�[������:�{�[���f�[�^�̃A�h���X  �{�[������:nullptr
	//------------------------------------------------------
	SPtr<BoneNode> SearchBone(const std::string& Name){
		for(UINT i = 0; i<m_BoneTree.size(); i++) {
			// �����񔻒�
			if(m_BoneTree[i]->BoneName == Name) {
				return m_BoneTree[i];
			}
		}
		return nullptr;
	}

	//------------------------------------------------------
	//! @brief �{�[��������C���f�b�N�X�ԍ����� ������Ȃ��ꍇ��-1
	//! @param[in]	Name	�c �{�[����
	//! @return �{�[������:�{�[���ԍ�  �{�[������:-1
	//------------------------------------------------------
	int SearchBoneIndex(const std::string& Name) const{
		for(int i=0;i<(int)m_BoneTree.size();i++){
			// �����񔻒�
			if(m_BoneTree[i]->BoneName == Name) {
				return i;
			}
		}
		return -1;
	}

	//------------------------------------------------------
	//! @brief �A�j���[�V���������݂��邩�H
	//------------------------------------------------------
	bool IsAnimationExist(){
		return !m_AnimeList.empty();
	}

	//------------------------------------------------------
	//! @brief �A�j���[�V����������Index���擾�B�Ȃ��ꍇ��-1
	//! @param[in]	AnimeName	�c �A�j����
	//! @return �����F�A�j���ԍ��@�A�j�������F-1
	//------------------------------------------------------
	int SearchAnimation(const std::string& AnimeName){
		for(UINT i=0;i<m_AnimeList.size();i++){
			if(m_AnimeList[i]->m_AnimeName == AnimeName){
				return i;
			}
		}
		return -1;
	}

	//------------------------------------------------------
	//! @brief �A�j���[�V������S�폜
	//------------------------------------------------------
	void DeleteAnimation(){
		m_AnimeList.clear();
		m_AnimeList.shrink_to_fit();
	}

	//------------------------------------------------------
	//! @brief ���\�[�X���
	//------------------------------------------------------
	virtual std::string GetTypeName() const override {
		std::string str = "YsGameModel : ";
		str += GetFullFileName();
		return str;
	}

	// 
	YsGameModel();
	// 
	~YsGameModel();


private:
	std::string						m_FilePath;						//!< �t�@�C���܂ł̃p�X

	std::string						m_FileName;						//!< �t�@�C����


	//---------------------------------------------------------------------------------
	// ���b�V���e�[�u��
	//---------------------------------------------------------------------------------
	std::vector<SPtr<YsSingleModel>>	m_ModelTbl;						//!< �S�Ẵ��b�V���ւ̎Q�Ƃ������Ă���z��
	
	std::vector<SPtr<YsSingleModel>>	m_RefModelTbl_SkinOnly;			//!< m_ModelTbl�̒��ŁA�X�L�����b�V���݂̂̃��b�V���ւ̎Q�Ƃ������Ă���z��@CreateRefMeshTbl�֐��ō쐬�����
	std::vector<SPtr<YsSingleModel>>	m_RefModelTbl_StaticOnly;		//!< m_ModelTbl�̒��ŁA�X�^�e�B�b�N���b�V���݂̂̃��b�V���ւ̎Q�Ƃ������Ă���z��@CreateRefMeshTbl�֐��ō쐬�����
		void CreateRefMeshTbl();		//!< m_ModelTbl�̓��e����A�X�L�����b�V���ƃX�^�e�B�b�N���b�V���̎Q�Ƃ𕪂����e�[�u���쐬

	//---------------------------------------------------------------------------------
	//! @brief �{�[���Q�Ɨp�z��(�{�[���z��)
	//! [0]��Root �c���[�\���ł��邪�A1�����z��Ƃ��Ă��A�N�Z�X�\�B
	//---------------------------------------------------------------------------------
	std::vector<SPtr<BoneNode>>			m_BoneTree;

	//---------------------------------------------------------------------------------
	// �A�j���[�V�������X�g
	//---------------------------------------------------------------------------------
	std::vector<SPtr<YsAnimationSet>>	m_AnimeList;					//!< �A�j���[�V�������X�g

	//---------------------------------------------------------------------------------
	// ���̃��X�g
	//---------------------------------------------------------------------------------
	std::vector< std::vector<YsXEDRigidBody> >		m_RigidBodyTbl;	// �񎟌��z��ł� KAMADA SkinMesh Editor�ł�0�`9��10�o�^�ł���̂ŁAm_RigidBodyTbl[0�`9][����Index]�ƂȂ�

private:
	// �R�s�[�֎~�p
	YsGameModel(const YsGameModel& src) {}
	void operator=(const YsGameModel& src) {}

public:
	//=================================================================================
	//! @brief YsGameModel�p�{�[���m�[�h
	//! @ingroup Graphics_Model
	//=================================================================================
	class BoneNode {
	public:
		//=========================
		// �{�[���f�[�^
		//=========================
		std::string					BoneName;		//!< �{�[����
		int							Level;			//!< �K�w Root��0
		YsMatrix					OffsetMat;		//!< �I�t�Z�b�g�s��
		DWORD						OffsetID;		//!< �s��e�[�u���̃C���f�b�N�X�ԍ�

		YsMatrix					DefTransMat;	//!< �f�t�H���g�p���ł̕ϊ��s��@���ʂȂ��Ƃ��Ȃ�����g�p���Ȃ�
		YsMatrix					DefWorldMat;	//!< �f�t�H���g�p���ł̃��[���h�s��@���ʂȂ��Ƃ��Ȃ�����g�p���Ȃ�

		WPtr<BoneNode>				pMother;		//!< �e�{�[���̃A�h���X
		std::vector<SPtr<BoneNode>>	Child;			//!< �q�{�[���z��

		//=========================
		// XED�g���f�[�^
		//=========================
#pragma region XED�g���f�[�^
		//! @brief �{�[���\�����C���[(Xed�p)
		int						BoneLayers;

		//! @brief �{�[���t���O(Xed�p)
		int						BoneFlag;

		//! @brief �{�[���t���O�p�萔(Xed�p)
		enum {
			BF_ROTATE = 0x00000002,		//!< ��]��
			BF_MOVE = 0x00000004,		//!< �ړ���
			BF_DISPBONE = 0x00000008,	//!< �\��
			BF_OPERATE = 0x00000010,	//!< �����
			BF_IK = 0x00000020,			//!< IK�L��

			BF_PROVIDEROTA = 0x00000100,	//!< ��]�t�^
			BF_PROVIDEMOVE = 0x00000200,	//!< �ړ��t�^

			BF_DISPBONELINE = 0x00010000,	//!< �e����̃{�[�����C���\��ON
		};

		//! @brief �{�[���t���O�F��]�L���H(Xed�p)
		bool BF_IsRotate(){ return (BoneFlag & BF_ROTATE ? true : false); }
		//! @brief �{�[���t���O�F�ړ��L���H(Xed�p)
		bool BF_IsMove(){ return (BoneFlag & BF_MOVE ? true : false); }
		//! @brief �{�[���t���O�F�{�[���\���L���H(Xed�p)
		bool BF_IsDispBone(){ return (BoneFlag & BF_DISPBONE ? true : false); }
		//! @brief �{�[���t���O�FIK�L���H(Xed�p)
		bool BF_IsIK(){ return (BoneFlag & BF_IK ? true : false); }
		//! @brief �{�[���t���O�F����\�H(Xed�p)
		bool BF_IsOperate(){ return (BoneFlag & BF_OPERATE ? true : false); }
		//! @brief �{�[���t���O�F�e����̃{�[�����C���\���L���H(Xed�p)
		bool BF_IsDispBoneLine(){ return (BoneFlag & BF_DISPBONELINE ? true : false); }
		//! @brief �{�[���t���O�F��]�t�^�L���H(Xed�p)
		bool BF_IsProvideRotate(){ return (BoneFlag & BF_PROVIDEROTA ? true : false); }
		//! @brief �{�[���t���O�F�ړ��t�^�L���H(Xed�p)
		bool BF_IsProvideMove(){ return (BoneFlag & BF_PROVIDEMOVE ? true : false); }

		// IK�f�[�^(Xed�p)
		struct IKData {
			int		boneIdx;			// �^�[�Q�b�g�{�[��Index
			int		LoopCnt;			// �v�Z���[�v�J�E���g
			float	ikLimitedAng;		// �P�ʊp
			BYTE	flags;				// ���g�p

			struct LinkData {
				int			boneIdx;		// �����N�{�[��Index
				bool		bLimitAng;		// �p�x�����L���H
				YsVec3		minLimitAng;	// �p�x�����F�Œ�p�x
				YsVec3		maxLimitAng;	// �p�x�����F�ő�p�x

				LinkData() : boneIdx(-1), bLimitAng(false), minLimitAng(0, 0, 0), maxLimitAng(0, 0, 0) {}
			};

			std::vector<LinkData>	LinkList;

			IKData() : boneIdx(-1), LoopCnt(0), ikLimitedAng(0), flags(0) {}
		};
		IKData					IK;	//!< IK�f�[�^(Xed�p)

#pragma endregion

		// 
		BoneNode() : OffsetID(0), Level(-1), BoneLayers(0), BoneFlag(0)
		{
		}
	};
};


}

#include "YsXFileLoader.h"
#include "YsXEDLoader.h"


#endif
