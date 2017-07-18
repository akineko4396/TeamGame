//===============================================================
//! @file YsMesh.h
//! @brief メッシュクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsMesh_h__
#define __YsMesh_h__

namespace YskLib
{

//==========================================================
//! @brief サブセットデータ
//!
//! AttribId番目のマテリアルは、FaceStart番目の面からFaceCount枚ぶん	\n
//!  描画すれば良い、という情報
//!
//! @ingroup Graphics_Model
//==========================================================
struct YsMeshSubset
{
	DWORD AttribId;			//!< マテリアルNo

	DWORD FaceStart;		//!< 面Index　このマテリアルで使用されている最初の面のIndex
	DWORD FaceCount;		//!< 面数　FaceStartから、何枚の面が使用されているかの
};


//=============================================================================
//! @brief メッシュ
//!
//!	頂点・面情報・属性テーブルなど、純粋なメッシュ情報のみ(マテリアルは無し)					\n
//!	m_Verticesやm_Facesやm_ExtFacesに全ての頂点や面情報が入ってるので、当たり判定などに使用可能
//!
//! ・Create関数でメッシュを作成
//! ・描画直前でSetDrawData関数でデータをセットし、DrawSubset関数で描画
//! ・
//!
//! @ingroup Graphics_Model
//=============================================================================
class YsMesh {
public:
	struct Face;
	struct ExtFace;

	//================================================================
	// データ取得系
	//================================================================

	//! @brief 頂点数取得
	unsigned int						GetNumVerts() const { return m_VertexBuf.size() / m_VertexTypeData.ByteSize; }
	
	//! @brief 面数取得
	unsigned int						GetNumFaces() const {return m_Faces.size();}
	
	//! @brief 頂点形式情報取得
	const YsVertexTypeData&				GetVertexType() const { return m_VertexTypeData; }
	
	//! @brief サブセット情報取得
	const std::vector<YsMeshSubset>&	GetSubset() const{ return m_Subset;}
	
	//! @brief サブセット情報取得　指定したmateNoと一致するもののみ返す
	//! @param[in] subsetNo … サブセット番号
	//! @return 見つかったマテリアル番号のサブセット　ない場合はnullptrが返る
	const YsMeshSubset*					GetSubset(int subsetNo) const;

	//! @brief このモデルはスキンメッシュかどうか
	bool								IsSkinMesh(){ return m_bSkinMesh; }

	//======================================================
	// 境界データ取得系
	//======================================================

	//! @brief メッシュのAABBの最小点を取得
	const YsVec3&	GetAABBMin() const { return m_vBBMin; }

	//! @brief メッシュのAABBの最大点を取得
	const YsVec3&	GetAABBMax() const { return m_vBBMax; }

	//! @brief メッシュの境界球のセンター座標(ローカル)を取得
	const YsVec3&	GetBSCenter() const { return m_vBSCenter; }

	//! @brief メッシュの境界球の半径を取得
	float			GetBSRadius() const { return m_fBSRadius; }

	//======================================================
	// 頂点や面のデータ取得
	//======================================================

	//! @brief 指定番号の面の、指定番号の頂点座標を取得
	//! @param[in] faceIdx		… 面番号
	//! @param[in] vIdx			… その面の頂点番号(0～2) ※頂点バッファのIndexではありません。この面内の頂点番号です。
	const YsVec3&	GetVertex_Pos(UINT faceIdx, int vIdx) const {
		return ((YsVertex_Pos&)m_VertexBuf[m_Faces[faceIdx].idx[vIdx] * m_VertexTypeData.ByteSize]).Pos;
	}

	//! @brief 指定番号の頂点データ座標
	//! @param[in] verIndex		… 頂点番号(0 ～ 頂点数-1)
	const YsVec3& GetVertex_Pos(int verIndex) const {
		return ((YsVertex_Pos&)m_VertexBuf[verIndex * m_VertexTypeData.ByteSize]).Pos;
	}

	//! @brief 面配列を取得
	const std::vector<Face>&			GetFace() const { return m_Faces; }

	//! @brief 面拡張データ配列を取得
	const std::vector<ExtFace>&			GetExtFace() const { return m_ExtFaces; }

	//================================================================
	// その他、取得系
	//================================================================

	//! @brief 頂点バッファ取得(参照カウンタは加算されません)
	ID3D11Buffer*						GetVB() { return m_VB; }

	//! @brief インデックスバッファ取得(参照カウンタは加算されません)
	ID3D11Buffer*						GetIB() { return m_IB; }


	//================================================================
	//
	// 描画関係
	//
	//================================================================

	//------------------------------------------------------
	//! @brief 描画時に必要なデータをデバイスへセット
	//! 以下の物をデバイスへセットする	\n
	//! ・頂点バッファ					\n
	//! ・インデックスバッファ			\n
	//! ・プリミティブ・トポロジー
	//------------------------------------------------------
	void SetDrawData();

	//------------------------------------------------------
	//! @brief 指定サブセットを描画
	//! @param[in]	subsetNo	… 描画するサブセット番号(マテリアル番号)
	//------------------------------------------------------
	void DrawSubset(int subsetNo);

	//------------------------------------------------------
	//! @brief 指定サブセットを描画(インスタンシング描画)
	//! @param[in]	subsetNo	… 描画するサブセット番号(マテリアル番号)
	//! @param[in]	instanceNum	… 描画するモデル数
	//------------------------------------------------------
	void DrawSubsetInstance(int subsetNo, int instanceNum);

	//======================================================
	//
	// メッシュ作成・解放
	//
	//======================================================

	//------------------------------------------------------
	//! @brief 解放
	//------------------------------------------------------
	void Release();

	//------------------------------------------------------
	//! @brief メッシュを作成する
	//! @param[in] verArray				… コピー元の頂点配列の先頭アドレス
	//! @param[in] numVertex			… コピー元の頂点配列の数
	//! @param[in] faceArray			… コピー元の面配列の先頭アドレス
	//! @param[in] numFace				… コピー元の面配列の数
	//! @param[in] subsetTbl			… コピー元サブセット配列
	//------------------------------------------------------
	template<typename VType>
	bool Create(const VType* verArray, unsigned int numVertex, const Face* faceArray, unsigned int numFace, const std::vector<YsMeshSubset>& subsetTbl);

	//------------------------------------------------------
	//! @brief 頂点配列を登録する
	//! @param[in]	verArray			… コピー元の頂点配列の先頭アドレス
	//! @param[in]	numVertex			… コピー元の頂点配列の数
	//! @param[in]	bCreateVertexBuffer	… Direct3D11の頂点バッファを作成するか？
	//! @return 成功:true
	//------------------------------------------------------
	template<typename VType>
	bool SetVertexArray(const VType* verArray, unsigned int numVertex, bool bCreateVertexBuffer);

	//------------------------------------------------------
	//! @brief 登録した頂点配列から、Direct3D11の頂点バッファを作成する
	//------------------------------------------------------
	bool CreateVertexBuffer();

	//------------------------------------------------------
	//! @brief 面配列を登録する
	//! @param[in] faceArray			… コピー元の面配列の先頭アドレス
	//! @param[in] numFace				… コピー元の面配列の数
	//! @param[in] bCreateIndexBuffer	… Direct3D11のインデックスバッファを作成するか？
	//! @return 成功:true
	//------------------------------------------------------
	bool SetFaceArray(const Face* faceArray, unsigned int numFace, bool bCreateIndexBuffer);

	//------------------------------------------------------
	//! @brief 登録した面配列から、Direct3D11のインデックスバッファを作成する
	//------------------------------------------------------
	bool CreateIndexBuffer();

	//------------------------------------------------------
	//! @brief サブセットリスト登録
	//! @param[in] subsetTbl	… コピー元サブセット配列
	//------------------------------------------------------
	void SetSubset(const std::vector<YsMeshSubset>& subsetTbl) {
		m_Subset = subsetTbl;
	}

	//------------------------------------------------------
	//! @brief あたり判定などに便利な、追加情報を作成する
	//------------------------------------------------------
	bool CreateExtData();


	//======================================================



	//! @brief 面データ構造体
	struct Face {
		UINT idx[3];	//!< 頂点１～３へのインデックス
	};

	//! @brief 面拡張データ構造体
	struct ExtFace {
		YsVec3	vN;		//!< 面の方向
		YsVec3	vMin;	//!< 面のAABB最小点
		YsVec3	vMax;	//!< 面のAABB最大点
	};

	//
	YsMesh() {
	}
	//
	~YsMesh() {
	}

private:
	// Direct3D11用バッファ
	ID3D11Buffer*				m_VB = nullptr;		//!< 頂点バッファ
	ID3D11Buffer*				m_IB = nullptr;		//!< インデックスバッファ

	std::vector<YsMeshSubset>	m_Subset;			//!< サブセット情報配列　通常マテリアルと同じ数になる


	// 頂点データ
	YsVertexTypeData			m_VertexTypeData;	//!< 頂点レイアウトデータ
	std::vector<char>			m_VertexBuf;		//!< 頂点配列(そのまま頂点バッファとして作成可能な形)

	// 面データ
	std::vector<Face>			m_Faces;			//!< 面配列(そのままインデックスバッファとして作成可能な形)

	std::vector<ExtFace>		m_ExtFaces;			//!< 面拡張データ配列(上記の面配列では足りないデータを補足したもの。特にあたり判定の高速化などに使用)

	bool						m_bSkinMesh = false;	//!< スキンメッシュか？

	// 境界データ
	YsVec3						m_vBBMin;			//!< このメッシュのAABB最小点
	YsVec3						m_vBBMax;			//!< このメッシュのAABB最大点
	YsVec3						m_vBSCenter;		//!< このメッシュの境界球ローカル座標
	float						m_fBSRadius = 0;	//!< このメッシュの境界球半径

private:
	// コピー禁止用
	YsMesh(const YsMesh& src) {}
	void operator=(const YsMesh& src) {}

};

// 作成
template<typename VType>
bool YsMesh::Create(const VType* verArray, unsigned int numVertex, const Face* faceArray, unsigned int numFace, const std::vector<YsMeshSubset>& subsetTbl) {
	// 頂点配列作成
	if (SetVertexArray(verArray, numVertex, true) == false) {
		Release();
		return false;
	}
	// 面配列作成
	if (SetFaceArray(faceArray, numFace, true) == false) {
		Release();
		return false;
	}

	// Subset設定
	SetSubset(subsetTbl);

	// 拡張データ作成
	CreateExtData();

	return true;
}

// 頂点配列を登録する
template<typename VType>
bool YsMesh::SetVertexArray(const VType* verArray, unsigned int numVertex, bool bCreateVertexBuffer)
{
	uint32_t totalSize = sizeof(VType) * numVertex;
	m_VertexBuf.resize(totalSize);
	m_VertexBuf.shrink_to_fit();

	// 頂点配列コピー
	memcpy(&m_VertexBuf[0], verArray, totalSize);

	// 頂点タイプ
	m_VertexTypeData = VType::GetVertexTypeData();

	// 頂点にスキニング情報があるなら、スキンメッシュとする
	if (m_VertexTypeData.FindSemantics("BLENDWEIGHT")) {
		m_bSkinMesh = true;
	}
	else {
		m_bSkinMesh = false;
	}

	// 頂点バッファ作成
	if (bCreateVertexBuffer) {
		return CreateVertexBuffer();
	}
	return true;
}


}

#endif
