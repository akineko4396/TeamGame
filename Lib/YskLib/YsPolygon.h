//===============================================================
//! @file YsPolygon.h
//! @brief ポリゴンクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsPolygon_h__
#define __YsPolygon_h__

namespace YskLib
{

//=============================================================
//! @brief 単純ポリゴンメッシュクラス
//!
//! 頂点バッファのみでインデックスバッファは使用していません。
//!
//! @ingroup Graphics_Model
//=============================================================
class YsPolygon {
public:

	//! @brief バッファの種類
	enum class BufType {
		None,			//!< なし
		Default,		//!< GPUからWrite○ Read○　CPUからWrite×Read×(ただしUpdateSubresource()で更新は可能)　高速
		Dynamic,		//!< GPUからWrite× Read○　CPUからWrite○Read×　頻繁に更新されるようなバッファはこっちの方が効率いいが、Defaultに比べたら少し速度は落ちる
	};

	//! @brief 頂点数取得
	int							GetVertexNum(){ return m_NumVertex; }

	//! @brief 頂点形式情報取得
	const YsVertexTypeData&		GetVertexType(){ return m_VTD; }

	//--------------------------------------------------------------------------
	//! @brief 頂点バッファ作成＆頂点データ書き込み
	//! @param[in] vertices		… 頂点データバッファの先頭アドレス
	//! @param[in] vertexNum	… 頂点数
	//! @param[in] vtd			… 頂点レイアウトデータ
	//! @param[in] isDynamic	… 動的バッファとして作成する(頻繁に更新する場合はこれがいい Map/Unmap可能)
	//--------------------------------------------------------------------------
	void Create(const void* vertices, int vertexNum, const YsVertexTypeData& vtd, bool isDynamic = true);

	//--------------------------------------------------------------------------
	//! @brief 解放
	//--------------------------------------------------------------------------
	void Release();

	//=========================================
	// データ書き込み系
	//=========================================

	//--------------------------------------------------------------------------
	//! @brief 頂点データ書き込み
	//! @param[in]	vertices	… 頂点データバッファの先頭アドレス
	//! @param[in]	vertexNum	… 頂点数
	//--------------------------------------------------------------------------
	void WriteVertexData(const void* vertices, int vertexNum);

	//--------------------------------------------------------------------------
	//! @brief 頂点バッファのアドレスを取得
	//!  ※Create()で作成した時のみ
	//! @param[out]		pOutMappedResource	… リソースをマップした情報が返る
	//--------------------------------------------------------------------------
	bool Map(D3D11_MAPPED_SUBRESOURCE* pOutMappedResource);

	//--------------------------------------------------------------------------
	//! @brief Map()したバッファをUnmap()する
	//--------------------------------------------------------------------------
	void Unmap();

	//=========================================
	// 描画系
	//=========================================

	//--------------------------------------------------------------------------
	//! @brief 描画に必要なデータをデバイスにセット
	//! @param[in]	primitiveTopologyを省略 … Triangle Stripとしてセットする
	//--------------------------------------------------------------------------
	void SetDrawData(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//--------------------------------------------------------------------------
	//! @brief 描画
	//! @param[in] vertexCount … 使用頂点数 0xFFFFFFFF(-1)でCreateしたときのサイズを使用
	//--------------------------------------------------------------------------
	void Draw(UINT vertexCount = 0xFFFFFFFF);

	//! @brief 頂点バッファ取得
	ID3D11Buffer*		GetVB() { return m_VB; }

	// 
	YsPolygon() : m_VB(0), m_TypeFlag(BufType::None) {
	}

	~YsPolygon() {
		Release();
	}

private:
	ID3D11Buffer*		m_VB;			//!< 頂点バッファ
	YsVertexTypeData	m_VTD;			//!< 頂点レイアウトデータ
	int					m_NumVertex;	//!< 頂点数

	BufType				m_TypeFlag;		//!< BufType
};

}
#endif
