//===============================================================
//! @file YsMesh.h
//! @brief ���b�V���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsMesh_h__
#define __YsMesh_h__

namespace YskLib
{

//==========================================================
//! @brief �T�u�Z�b�g�f�[�^
//!
//! AttribId�Ԗڂ̃}�e���A���́AFaceStart�Ԗڂ̖ʂ���FaceCount���Ԃ�	\n
//!  �`�悷��Ηǂ��A�Ƃ������
//!
//! @ingroup Graphics_Model
//==========================================================
struct YsMeshSubset
{
	DWORD AttribId;			//!< �}�e���A��No

	DWORD FaceStart;		//!< ��Index�@���̃}�e���A���Ŏg�p����Ă���ŏ��̖ʂ�Index
	DWORD FaceCount;		//!< �ʐ��@FaceStart����A�����̖ʂ��g�p����Ă��邩��
};


//=============================================================================
//! @brief ���b�V��
//!
//!	���_�E�ʏ��E�����e�[�u���ȂǁA�����ȃ��b�V�����̂�(�}�e���A���͖���)					\n
//!	m_Vertices��m_Faces��m_ExtFaces�ɑS�Ă̒��_��ʏ�񂪓����Ă�̂ŁA�����蔻��ȂǂɎg�p�\
//!
//! �ECreate�֐��Ń��b�V�����쐬
//! �E�`�撼�O��SetDrawData�֐��Ńf�[�^���Z�b�g���ADrawSubset�֐��ŕ`��
//! �E
//!
//! @ingroup Graphics_Model
//=============================================================================
class YsMesh {
public:
	struct Face;
	struct ExtFace;

	//================================================================
	// �f�[�^�擾�n
	//================================================================

	//! @brief ���_���擾
	unsigned int						GetNumVerts() const { return m_VertexBuf.size() / m_VertexTypeData.ByteSize; }
	
	//! @brief �ʐ��擾
	unsigned int						GetNumFaces() const {return m_Faces.size();}
	
	//! @brief ���_�`�����擾
	const YsVertexTypeData&				GetVertexType() const { return m_VertexTypeData; }
	
	//! @brief �T�u�Z�b�g���擾
	const std::vector<YsMeshSubset>&	GetSubset() const{ return m_Subset;}
	
	//! @brief �T�u�Z�b�g���擾�@�w�肵��mateNo�ƈ�v������̂̂ݕԂ�
	//! @param[in] subsetNo �c �T�u�Z�b�g�ԍ�
	//! @return ���������}�e���A���ԍ��̃T�u�Z�b�g�@�Ȃ��ꍇ��nullptr���Ԃ�
	const YsMeshSubset*					GetSubset(int subsetNo) const;

	//! @brief ���̃��f���̓X�L�����b�V�����ǂ���
	bool								IsSkinMesh(){ return m_bSkinMesh; }

	//======================================================
	// ���E�f�[�^�擾�n
	//======================================================

	//! @brief ���b�V����AABB�̍ŏ��_���擾
	const YsVec3&	GetAABBMin() const { return m_vBBMin; }

	//! @brief ���b�V����AABB�̍ő�_���擾
	const YsVec3&	GetAABBMax() const { return m_vBBMax; }

	//! @brief ���b�V���̋��E���̃Z���^�[���W(���[�J��)���擾
	const YsVec3&	GetBSCenter() const { return m_vBSCenter; }

	//! @brief ���b�V���̋��E���̔��a���擾
	float			GetBSRadius() const { return m_fBSRadius; }

	//======================================================
	// ���_��ʂ̃f�[�^�擾
	//======================================================

	//! @brief �w��ԍ��̖ʂ́A�w��ԍ��̒��_���W���擾
	//! @param[in] faceIdx		�c �ʔԍ�
	//! @param[in] vIdx			�c ���̖ʂ̒��_�ԍ�(0�`2) �����_�o�b�t�@��Index�ł͂���܂���B���̖ʓ��̒��_�ԍ��ł��B
	const YsVec3&	GetVertex_Pos(UINT faceIdx, int vIdx) const {
		return ((YsVertex_Pos&)m_VertexBuf[m_Faces[faceIdx].idx[vIdx] * m_VertexTypeData.ByteSize]).Pos;
	}

	//! @brief �w��ԍ��̒��_�f�[�^���W
	//! @param[in] verIndex		�c ���_�ԍ�(0 �` ���_��-1)
	const YsVec3& GetVertex_Pos(int verIndex) const {
		return ((YsVertex_Pos&)m_VertexBuf[verIndex * m_VertexTypeData.ByteSize]).Pos;
	}

	//! @brief �ʔz����擾
	const std::vector<Face>&			GetFace() const { return m_Faces; }

	//! @brief �ʊg���f�[�^�z����擾
	const std::vector<ExtFace>&			GetExtFace() const { return m_ExtFaces; }

	//================================================================
	// ���̑��A�擾�n
	//================================================================

	//! @brief ���_�o�b�t�@�擾(�Q�ƃJ�E���^�͉��Z����܂���)
	ID3D11Buffer*						GetVB() { return m_VB; }

	//! @brief �C���f�b�N�X�o�b�t�@�擾(�Q�ƃJ�E���^�͉��Z����܂���)
	ID3D11Buffer*						GetIB() { return m_IB; }


	//================================================================
	//
	// �`��֌W
	//
	//================================================================

	//------------------------------------------------------
	//! @brief �`�掞�ɕK�v�ȃf�[�^���f�o�C�X�փZ�b�g
	//! �ȉ��̕����f�o�C�X�փZ�b�g����	\n
	//! �E���_�o�b�t�@					\n
	//! �E�C���f�b�N�X�o�b�t�@			\n
	//! �E�v���~�e�B�u�E�g�|���W�[
	//------------------------------------------------------
	void SetDrawData();

	//------------------------------------------------------
	//! @brief �w��T�u�Z�b�g��`��
	//! @param[in]	subsetNo	�c �`�悷��T�u�Z�b�g�ԍ�(�}�e���A���ԍ�)
	//------------------------------------------------------
	void DrawSubset(int subsetNo);

	//------------------------------------------------------
	//! @brief �w��T�u�Z�b�g��`��(�C���X�^���V���O�`��)
	//! @param[in]	subsetNo	�c �`�悷��T�u�Z�b�g�ԍ�(�}�e���A���ԍ�)
	//! @param[in]	instanceNum	�c �`�悷�郂�f����
	//------------------------------------------------------
	void DrawSubsetInstance(int subsetNo, int instanceNum);

	//======================================================
	//
	// ���b�V���쐬�E���
	//
	//======================================================

	//------------------------------------------------------
	//! @brief ���
	//------------------------------------------------------
	void Release();

	//------------------------------------------------------
	//! @brief ���b�V�����쐬����
	//! @param[in] verArray				�c �R�s�[���̒��_�z��̐擪�A�h���X
	//! @param[in] numVertex			�c �R�s�[���̒��_�z��̐�
	//! @param[in] faceArray			�c �R�s�[���̖ʔz��̐擪�A�h���X
	//! @param[in] numFace				�c �R�s�[���̖ʔz��̐�
	//! @param[in] subsetTbl			�c �R�s�[���T�u�Z�b�g�z��
	//------------------------------------------------------
	template<typename VType>
	bool Create(const VType* verArray, unsigned int numVertex, const Face* faceArray, unsigned int numFace, const std::vector<YsMeshSubset>& subsetTbl);

	//------------------------------------------------------
	//! @brief ���_�z���o�^����
	//! @param[in]	verArray			�c �R�s�[���̒��_�z��̐擪�A�h���X
	//! @param[in]	numVertex			�c �R�s�[���̒��_�z��̐�
	//! @param[in]	bCreateVertexBuffer	�c Direct3D11�̒��_�o�b�t�@���쐬���邩�H
	//! @return ����:true
	//------------------------------------------------------
	template<typename VType>
	bool SetVertexArray(const VType* verArray, unsigned int numVertex, bool bCreateVertexBuffer);

	//------------------------------------------------------
	//! @brief �o�^�������_�z�񂩂�ADirect3D11�̒��_�o�b�t�@���쐬����
	//------------------------------------------------------
	bool CreateVertexBuffer();

	//------------------------------------------------------
	//! @brief �ʔz���o�^����
	//! @param[in] faceArray			�c �R�s�[���̖ʔz��̐擪�A�h���X
	//! @param[in] numFace				�c �R�s�[���̖ʔz��̐�
	//! @param[in] bCreateIndexBuffer	�c Direct3D11�̃C���f�b�N�X�o�b�t�@���쐬���邩�H
	//! @return ����:true
	//------------------------------------------------------
	bool SetFaceArray(const Face* faceArray, unsigned int numFace, bool bCreateIndexBuffer);

	//------------------------------------------------------
	//! @brief �o�^�����ʔz�񂩂�ADirect3D11�̃C���f�b�N�X�o�b�t�@���쐬����
	//------------------------------------------------------
	bool CreateIndexBuffer();

	//------------------------------------------------------
	//! @brief �T�u�Z�b�g���X�g�o�^
	//! @param[in] subsetTbl	�c �R�s�[���T�u�Z�b�g�z��
	//------------------------------------------------------
	void SetSubset(const std::vector<YsMeshSubset>& subsetTbl) {
		m_Subset = subsetTbl;
	}

	//------------------------------------------------------
	//! @brief �����蔻��Ȃǂɕ֗��ȁA�ǉ������쐬����
	//------------------------------------------------------
	bool CreateExtData();


	//======================================================



	//! @brief �ʃf�[�^�\����
	struct Face {
		UINT idx[3];	//!< ���_�P�`�R�ւ̃C���f�b�N�X
	};

	//! @brief �ʊg���f�[�^�\����
	struct ExtFace {
		YsVec3	vN;		//!< �ʂ̕���
		YsVec3	vMin;	//!< �ʂ�AABB�ŏ��_
		YsVec3	vMax;	//!< �ʂ�AABB�ő�_
	};

	//
	YsMesh() {
	}
	//
	~YsMesh() {
	}

private:
	// Direct3D11�p�o�b�t�@
	ID3D11Buffer*				m_VB = nullptr;		//!< ���_�o�b�t�@
	ID3D11Buffer*				m_IB = nullptr;		//!< �C���f�b�N�X�o�b�t�@

	std::vector<YsMeshSubset>	m_Subset;			//!< �T�u�Z�b�g���z��@�ʏ�}�e���A���Ɠ������ɂȂ�


	// ���_�f�[�^
	YsVertexTypeData			m_VertexTypeData;	//!< ���_���C�A�E�g�f�[�^
	std::vector<char>			m_VertexBuf;		//!< ���_�z��(���̂܂ܒ��_�o�b�t�@�Ƃ��č쐬�\�Ȍ`)

	// �ʃf�[�^
	std::vector<Face>			m_Faces;			//!< �ʔz��(���̂܂܃C���f�b�N�X�o�b�t�@�Ƃ��č쐬�\�Ȍ`)

	std::vector<ExtFace>		m_ExtFaces;			//!< �ʊg���f�[�^�z��(��L�̖ʔz��ł͑���Ȃ��f�[�^��⑫�������́B���ɂ����蔻��̍������ȂǂɎg�p)

	bool						m_bSkinMesh = false;	//!< �X�L�����b�V�����H

	// ���E�f�[�^
	YsVec3						m_vBBMin;			//!< ���̃��b�V����AABB�ŏ��_
	YsVec3						m_vBBMax;			//!< ���̃��b�V����AABB�ő�_
	YsVec3						m_vBSCenter;		//!< ���̃��b�V���̋��E�����[�J�����W
	float						m_fBSRadius = 0;	//!< ���̃��b�V���̋��E�����a

private:
	// �R�s�[�֎~�p
	YsMesh(const YsMesh& src) {}
	void operator=(const YsMesh& src) {}

};

// �쐬
template<typename VType>
bool YsMesh::Create(const VType* verArray, unsigned int numVertex, const Face* faceArray, unsigned int numFace, const std::vector<YsMeshSubset>& subsetTbl) {
	// ���_�z��쐬
	if (SetVertexArray(verArray, numVertex, true) == false) {
		Release();
		return false;
	}
	// �ʔz��쐬
	if (SetFaceArray(faceArray, numFace, true) == false) {
		Release();
		return false;
	}

	// Subset�ݒ�
	SetSubset(subsetTbl);

	// �g���f�[�^�쐬
	CreateExtData();

	return true;
}

// ���_�z���o�^����
template<typename VType>
bool YsMesh::SetVertexArray(const VType* verArray, unsigned int numVertex, bool bCreateVertexBuffer)
{
	uint32_t totalSize = sizeof(VType) * numVertex;
	m_VertexBuf.resize(totalSize);
	m_VertexBuf.shrink_to_fit();

	// ���_�z��R�s�[
	memcpy(&m_VertexBuf[0], verArray, totalSize);

	// ���_�^�C�v
	m_VertexTypeData = VType::GetVertexTypeData();

	// ���_�ɃX�L�j���O��񂪂���Ȃ�A�X�L�����b�V���Ƃ���
	if (m_VertexTypeData.FindSemantics("BLENDWEIGHT")) {
		m_bSkinMesh = true;
	}
	else {
		m_bSkinMesh = false;
	}

	// ���_�o�b�t�@�쐬
	if (bCreateVertexBuffer) {
		return CreateVertexBuffer();
	}
	return true;
}


}

#endif
