//===============================================================
//! @file YsSingleModel.h
//! @brief メッシュモデルクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsSingleModel_h__
#define	__YsSingleModel_h__

namespace YskLib
{

//==========================================================
//! @brief マテリアルデータ構造体
//!
//!  １つの材質の情報
//!
//! @ingroup Graphics_Model
//==========================================================
struct YsMaterial {
	//===========================================================
	// マテリアルデータ
	//===========================================================

	YsVec4			Diffuse;			//!< 拡散色(基本色)
	YsVec4			Specular;			//!< 反射色
	float			Power;				//!< 反射の強さ
	YsVec3			Ambient;			//!< 環境色
	YsVec4			Emissive;			//!< 自己照明

	std::string		TextureFileName;	//!< テクスチャ名

	std::string		ExtScript;			//!< 拡張スクリプト文字列

	YsVec2			UV_Tiling;			//!< UVタイリング率
	YsVec2			UV_Offset;			//!< UVオフセット値

	//------------------------------------------------------------------------
	//! @brief テクスチャセット
	//!  複数名のテクスチャを１つの構造体にまとめただけのもの
	//------------------------------------------------------------------------
	YsTextureSet	TexSet;				//!< テクスチャセット

	//------------------------------------------------------------------------
	//! @brief 描画フラグ(XED専用)
	//------------------------------------------------------------------------
	char		DrawFlag;
		//! @brief 描画フラグ用定数
		enum {
			DF_BOTHFACE = 0x01,	//!< 両面描画
		};

	//! @brief 両面描画が有効かどうか
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
//! @brief 単一メッシュモデルクラス
//!
//!  YsMeshとマテリアルを組み合わせただけのもの
//!
//! @ingroup Graphics_Model
//==========================================================
class YsSingleModel{
public:
	//=====================================
	// 読み込み・解放
	//=====================================

	//! @brief Xファイル読み込み
//	bool LoadXFile(const std::string& filename);

	//------------------------------------------------------------------------
	//! @brief 解放
	//------------------------------------------------------------------------
	void Release();

	//------------------------------------------------------------------------
	//! @brief 描画に必要なデータをセット
	//! 以下の物をデバイスへセットする
	//! ・頂点バッファ
	//! ・インデックスバッファ
	//! ・プリミティブ・トポロジー
	//------------------------------------------------------------------------
	void SetDrawData();

	//=====================================
	// 取得
	//=====================================

	//! @brief メッシュデータを取得
	SPtr<YsMesh>						GetMesh(){return m_pMesh;}

	//! @brief マテリアルデータを取得
	std::vector<YsMaterial>&			GetMaterials(){return m_Materials;}

	//! @brief メッシュ名取得
	std::string&						GetName(){ return m_Name; }

	//=====================================
	// 設定
	//=====================================

	//------------------------------------------------------------------------
	//! @brief YsMeshを外部からセットする
	//! @param[in]	mesh	… セットするYsMeshクラス
	//------------------------------------------------------------------------
	void SetMesh(SPtr<YsMesh> mesh) {
		m_pMesh = mesh;
	}

	//------------------------------------------------------------------------
	//! @brief マテリアルリストを外部からセットする
	//! @param[in]	materials	… セットするマテリアル配列
	//------------------------------------------------------------------------
	void SetMaterials(const std::vector<YsMaterial>& materials){
		m_Materials.clear();
		m_Materials = materials;
	}

	//------------------------------------------------------------------------
	//! @brief メッシュ名設定
	//! @param[in]	meshName	… メッシュ名設定
	//------------------------------------------------------------------------
	void SetName(const std::string& meshName){ m_Name = meshName; }

	//
	YsSingleModel(){
	}
	~YsSingleModel(){
		Release();
	}

private:
	std::string						m_Name;			//!< メッシュ名

	SPtr<YsMesh>					m_pMesh;		//!< メッシュデータ
	std::vector<YsMaterial>			m_Materials;	//!< マテリアルデータ

private:
	// コピー禁止用
	YsSingleModel(const YsSingleModel& src) {}
	void operator=(const YsSingleModel& src) {}
};

}

#endif
