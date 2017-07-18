//===============================================================
//! @file YsPolygon.h
//! @brief �|���S���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsPolygon_h__
#define __YsPolygon_h__

namespace YskLib
{

//=============================================================
//! @brief �P���|���S�����b�V���N���X
//!
//! ���_�o�b�t�@�݂̂ŃC���f�b�N�X�o�b�t�@�͎g�p���Ă��܂���B
//!
//! @ingroup Graphics_Model
//=============================================================
class YsPolygon {
public:

	//! @brief �o�b�t�@�̎��
	enum class BufType {
		None,			//!< �Ȃ�
		Default,		//!< GPU����Write�� Read���@CPU����Write�~Read�~(������UpdateSubresource()�ōX�V�͉\)�@����
		Dynamic,		//!< GPU����Write�~ Read���@CPU����Write��Read�~�@�p�ɂɍX�V�����悤�ȃo�b�t�@�͂������̕��������������ADefault�ɔ�ׂ��班�����x�͗�����
	};

	//! @brief ���_���擾
	int							GetVertexNum(){ return m_NumVertex; }

	//! @brief ���_�`�����擾
	const YsVertexTypeData&		GetVertexType(){ return m_VTD; }

	//--------------------------------------------------------------------------
	//! @brief ���_�o�b�t�@�쐬�����_�f�[�^��������
	//! @param[in] vertices		�c ���_�f�[�^�o�b�t�@�̐擪�A�h���X
	//! @param[in] vertexNum	�c ���_��
	//! @param[in] vtd			�c ���_���C�A�E�g�f�[�^
	//! @param[in] isDynamic	�c ���I�o�b�t�@�Ƃ��č쐬����(�p�ɂɍX�V����ꍇ�͂��ꂪ���� Map/Unmap�\)
	//--------------------------------------------------------------------------
	void Create(const void* vertices, int vertexNum, const YsVertexTypeData& vtd, bool isDynamic = true);

	//--------------------------------------------------------------------------
	//! @brief ���
	//--------------------------------------------------------------------------
	void Release();

	//=========================================
	// �f�[�^�������݌n
	//=========================================

	//--------------------------------------------------------------------------
	//! @brief ���_�f�[�^��������
	//! @param[in]	vertices	�c ���_�f�[�^�o�b�t�@�̐擪�A�h���X
	//! @param[in]	vertexNum	�c ���_��
	//--------------------------------------------------------------------------
	void WriteVertexData(const void* vertices, int vertexNum);

	//--------------------------------------------------------------------------
	//! @brief ���_�o�b�t�@�̃A�h���X���擾
	//!  ��Create()�ō쐬�������̂�
	//! @param[out]		pOutMappedResource	�c ���\�[�X���}�b�v������񂪕Ԃ�
	//--------------------------------------------------------------------------
	bool Map(D3D11_MAPPED_SUBRESOURCE* pOutMappedResource);

	//--------------------------------------------------------------------------
	//! @brief Map()�����o�b�t�@��Unmap()����
	//--------------------------------------------------------------------------
	void Unmap();

	//=========================================
	// �`��n
	//=========================================

	//--------------------------------------------------------------------------
	//! @brief �`��ɕK�v�ȃf�[�^���f�o�C�X�ɃZ�b�g
	//! @param[in]	primitiveTopology���ȗ� �c Triangle Strip�Ƃ��ăZ�b�g����
	//--------------------------------------------------------------------------
	void SetDrawData(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//--------------------------------------------------------------------------
	//! @brief �`��
	//! @param[in] vertexCount �c �g�p���_�� 0xFFFFFFFF(-1)��Create�����Ƃ��̃T�C�Y���g�p
	//--------------------------------------------------------------------------
	void Draw(UINT vertexCount = 0xFFFFFFFF);

	//! @brief ���_�o�b�t�@�擾
	ID3D11Buffer*		GetVB() { return m_VB; }

	// 
	YsPolygon() : m_VB(0), m_TypeFlag(BufType::None) {
	}

	~YsPolygon() {
		Release();
	}

private:
	ID3D11Buffer*		m_VB;			//!< ���_�o�b�t�@
	YsVertexTypeData	m_VTD;			//!< ���_���C�A�E�g�f�[�^
	int					m_NumVertex;	//!< ���_��

	BufType				m_TypeFlag;		//!< BufType
};

}
#endif
