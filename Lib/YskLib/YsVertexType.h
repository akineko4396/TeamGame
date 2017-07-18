//===============================================================
//! @file YsVerteType.h
//! @brief 頂点形式データ集
//!
//! @author 鎌田
//===============================================================
#ifndef __YsVertexType_h__
#define __YsVertexType_h__

namespace YskLib
{

//! @addtogroup Graphics_Model
//! @{

//===============================================
//! @brief 頂点レイアウト用データ
//! １つの頂点データの詳細を入れる
//===============================================
struct YsVertexTypeData {
	int								ByteSize;		//!< １頂点のバイト数

	int								Num;			//!< 頂点レイアウトの要素数
	const D3D11_INPUT_ELEMENT_DESC*	pLayout;		//!< 頂点レイアウトデータ(何バイト目に座標があって、何バイト目に法線があって…っというデータ)

	//! @brief 指定セマンティクス存在チェック
	//! @param[in] semanticName … 検索したセマンティクス名
	//! @return	存在しない場合はnullptr　存在する場合はそのデータのアドレスが返る
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
// 頂点データ
//===============================================

//! @brief 頂点データ(座標のみ)
//! これが基本の頂点データクラスになる
struct YsVertex_Pos {
	YsVec3 Pos;	//!< 位置

	//! @brief 頂点データ取得
	//! @return 頂点レイアウトデータ
	static YsVertexTypeData GetVertexTypeData() {
		// 頂点のレイアウト
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// データ
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos),		// 型
			1,							// レイアウトの個数
			layout						// レイアウト
		};
		return vtd;
	}

};


//! @brief 頂点データ(座標,色)
struct YsVertex_Pos_Color : public YsVertex_Pos {
//	YsVec3 Pos;		//!< 位置
	YsVec4 Color;	//!< 色

	//! @brief 頂点データ取得
	//! @return 頂点レイアウトデータ
	static YsVertexTypeData GetVertexTypeData() {
		// 頂点のレイアウト
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		// データ
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_Color),	// 型
			2,							// レイアウトの個数
			layout						// レイアウト
		};
		return vtd;
	}

	YsVertex_Pos_Color() {}

	YsVertex_Pos_Color(const YsVec3& pos, const YsVec4& color) {
		Pos = pos;
		Color = color;
	}

};


//! @brief 頂点データ(座標,UV)
struct YsVertex_Pos_UV : public YsVertex_Pos {
//	YsVec3 Pos;		//!< 位置
	YsVec2 UV;		//!< UV

	//! @brief 頂点データ取得
	//! @return 頂点レイアウトデータ
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		// データ
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

//! @brief 頂点データ(座標,UV,Color)
struct YsVertex_Pos_UV_Color : public YsVertex_Pos {
//	YsVec3 Pos;		//!< 位置
	YsVec2 UV;		//!< UV
	YsVec4 Color;	//!< 色

	//! @brief 頂点データ取得
	//! @return 頂点レイアウトデータ
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		// データ
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


//! @brief 頂点データ(座標,UV,法線)
struct YsVertex_Pos_UV_Normal : public YsVertex_Pos {
//	YsVec3 Pos;			//!< 位置
	YsVec2 UV;			//!< UV
	YsVec3 Normal;		//!< 法線

	//! @brief 頂点データ取得
	//! @return 頂点レイアウトデータ
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// データ
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


//! @brief 頂点データ(座標,UV,法線マップ用法線)
//! YsGameModelクラスで読み込んだモデル(Static mesh)は、この形式です
struct YsVertex_Pos_UV_TBN : public YsVertex_Pos {
//	YsVec3 Pos;			//!< 位置
	YsVec2 UV;			//!< UV
	YsVec3 Tangent;		//!< 接線
	YsVec3 Binormal;	//!< 従法線
	YsVec3 Normal;		//!< 法線

	//! @brief 頂点データ取得
	//! @return 頂点レイアウトデータ
	static YsVertexTypeData GetVertexTypeData() {
		static const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// データ
		static const YsVertexTypeData vtd = {
			sizeof(YsVertex_Pos_UV_TBN),
			5,
			layout
		};
		return vtd;
	}
};


//! @brief 頂点データ(座標,UV,法線マップ用法線,頂点ブレンド情報)
//! YsGameModelクラスで読み込んだモデル(Skin mesh)は、この形式です
struct YsVertex_Pos_UV_TBN_Skin : public YsVertex_Pos_UV_TBN
{
//	YsVec3 Pos;			//!< 位置
//	YsVec2 UV;			//!< UV
//	YsVec3 Tangent;		//!< 接線
//	YsVec3 Binormal;	//!< 従法線
//	YsVec3 Normal;		//!< 法線

	float				BlendWeight[4];		//!< ブレンドの重み
	unsigned short		BlendIndices[4];	//!< ボーン番号

	//! @brief 頂点データ取得
	//! @return 頂点レイアウトデータ
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
		// データ
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
