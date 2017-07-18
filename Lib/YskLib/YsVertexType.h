//===============================================================
//! @file YsVerteType.h
//! @brief ���_�`���f�[�^�W
//!
//! @author ���c
//===============================================================
#ifndef __YsVertexType_h__
#define __YsVertexType_h__

namespace YskLib
{

//! @addtogroup Graphics_Model
//! @{

//===============================================
//! @brief ���_���C�A�E�g�p�f�[�^
//! �P�̒��_�f�[�^�̏ڍׂ�����
//===============================================
struct YsVertexTypeData {
	int								ByteSize;		//!< �P���_�̃o�C�g��

	int								Num;			//!< ���_���C�A�E�g�̗v�f��
	const D3D11_INPUT_ELEMENT_DESC*	pLayout;		//!< ���_���C�A�E�g�f�[�^(���o�C�g�ڂɍ��W�������āA���o�C�g�ڂɖ@���������āc���Ƃ����f�[�^)

	//! @brief �w��Z�}���e�B�N�X���݃`�F�b�N
	//! @param[in] semanticName �c ���������Z�}���e�B�N�X��
	//! @return	���݂��Ȃ��ꍇ��nullptr�@���݂���ꍇ�͂��̃f�[�^�̃A�h���X���Ԃ�
	const D3D11_INPUT_ELEMENT_DESC* FindSemantics(const std::string& semanticName) const {
		for(int i=0;i<Num;i++){
			if(semanticName == pLayout[i].SemanticName){
				return &pLayout[i];
			}
		}
		return nullptr;

	}
};

//===============================================
// ���_�f�[�^
//===============================================

//! @brief ���_�f�[�^(���W�̂�)
//! ���ꂪ��{�̒��_�f�[�^�N���X�ɂȂ�
struct YsVertex_Pos {
	YsVec3 Pos;	//!< �ʒu

	//! @brief ���_�f�[�^�擾
	//! @return ���_���C�A�E�g�f�[�^
	static YsVertexTypeData GetVertexTypeData() {
		// ���_�̃��C�A�E�g
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// �f�[�^
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos),		// �^
			1,							// ���C�A�E�g�̌�
			layout						// ���C�A�E�g
		};
		return vtd;
	}

};


//! @brief ���_�f�[�^(���W,�F)
struct YsVertex_Pos_Color : public YsVertex_Pos {
//	YsVec3 Pos;		//!< �ʒu
	YsVec4 Color;	//!< �F

	//! @brief ���_�f�[�^�擾
	//! @return ���_���C�A�E�g�f�[�^
	static YsVertexTypeData GetVertexTypeData() {
		// ���_�̃��C�A�E�g
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		// �f�[�^
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_Color),	// �^
			2,							// ���C�A�E�g�̌�
			layout						// ���C�A�E�g
		};
		return vtd;
	}

	YsVertex_Pos_Color() {}

	YsVertex_Pos_Color(const YsVec3& pos, const YsVec4& color) {
		Pos = pos;
		Color = color;
	}

};


//! @brief ���_�f�[�^(���W,UV)
struct YsVertex_Pos_UV : public YsVertex_Pos {
//	YsVec3 Pos;		//!< �ʒu
	YsVec2 UV;		//!< UV

	//! @brief ���_�f�[�^�擾
	//! @return ���_���C�A�E�g�f�[�^
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		// �f�[�^
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_UV),
			2,
			layout
		};
		return vtd;
	}

	YsVertex_Pos_UV() {}

	YsVertex_Pos_UV(const YsVec3& pos, const YsVec2& uv) {
		Pos = pos;
		UV = uv;
	}
};

//! @brief ���_�f�[�^(���W,UV,Color)
struct YsVertex_Pos_UV_Color : public YsVertex_Pos {
//	YsVec3 Pos;		//!< �ʒu
	YsVec2 UV;		//!< UV
	YsVec4 Color;	//!< �F

	//! @brief ���_�f�[�^�擾
	//! @return ���_���C�A�E�g�f�[�^
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		// �f�[�^
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_UV_Color),
			3,
			layout
		};
		return vtd;
	}

	YsVertex_Pos_UV_Color() {}

	YsVertex_Pos_UV_Color(const YsVec3& pos, const YsVec2& uv, const YsVec4& color){
		Pos = pos;
		UV = uv;
		Color = color;
	}
};


//! @brief ���_�f�[�^(���W,UV,�@��)
struct YsVertex_Pos_UV_Normal : public YsVertex_Pos {
//	YsVec3 Pos;			//!< �ʒu
	YsVec2 UV;			//!< UV
	YsVec3 Normal;		//!< �@��

	//! @brief ���_�f�[�^�擾
	//! @return ���_���C�A�E�g�f�[�^
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// �f�[�^
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_UV_Normal),
			3,
			layout
		};
		return vtd;
	}

	YsVertex_Pos_UV_Normal() {}

	YsVertex_Pos_UV_Normal(const YsVec3& pos, const YsVec2& uv, const YsVec3& normal) {
		Pos = pos;
		UV = uv;
		Normal = normal;
	}
};


//! @brief ���_�f�[�^(���W,UV,�@���}�b�v�p�@��)
//! YsGameModel�N���X�œǂݍ��񂾃��f��(Static mesh)�́A���̌`���ł�
struct YsVertex_Pos_UV_TBN : public YsVertex_Pos {
//	YsVec3 Pos;			//!< �ʒu
	YsVec2 UV;			//!< UV
	YsVec3 Tangent;		//!< �ڐ�
	YsVec3 Binormal;	//!< �]�@��
	YsVec3 Normal;		//!< �@��

	//! @brief ���_�f�[�^�擾
	//! @return ���_���C�A�E�g�f�[�^
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// �f�[�^
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_UV_TBN),
			5,
			layout
		};
		return vtd;
	}
};


//! @brief ���_�f�[�^(���W,UV,�@���}�b�v�p�@��,���_�u�����h���)
//! YsGameModel�N���X�œǂݍ��񂾃��f��(Skin mesh)�́A���̌`���ł�
struct YsVertex_Pos_UV_TBN_Skin : public YsVertex_Pos_UV_TBN
{
//	YsVec3 Pos;			//!< �ʒu
//	YsVec2 UV;			//!< UV
//	YsVec3 Tangent;		//!< �ڐ�
//	YsVec3 Binormal;	//!< �]�@��
//	YsVec3 Normal;		//!< �@��

	float				BlendWeight[4];		//!< �u�����h�̏d��
	unsigned short		BlendIndices[4];	//!< �{�[���ԍ�

	//! @brief ���_�f�[�^�擾
	//! @return ���_���C�A�E�g�f�[�^
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R16G16B16A16_UINT,	0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// �f�[�^
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_UV_TBN_Skin),
			7,
			layout
		};
		return vtd;
	}

};

//! @}

}
#endif
