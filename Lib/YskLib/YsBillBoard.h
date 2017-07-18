//===============================================================
//! @file YsBillBoard.h
//! @brief 板ポリゴンクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsBillBoard_h__
#define __YsBillBoard_h__

namespace YskLib{

//=======================================================================
//!
//! @brief 四角形ポリゴンオブジェクトクラス
//!
//! 初期では、幅x高:1x1 UV:0～１ 色:白　になる	\n
//!  あとはSet系の関数で変更する				\n
//!　[主な機能]									\n
//!　・テクスチャ								\n
//!　・UVアニメーション							\n
//!　・色										\n
//!
//! @ingroup Graphics_Model
//=======================================================================
class YsBillBoard {
public:

	//=======================================================================
	// 頂点設定
	//=======================================================================
	
	//------------------------------------------------------------------------
	//! @brief 頂点座標設定
	//! X,Y平面に頂点を配置されます。Zは0。	\n
	//! 3D座標系で考えること(Yは上)
	//! @param[in]	x	… X座標
	//! @param[in]	y	… Y座標
	//! @param[in]	w	… 幅
	//! @param[in]	z	… 高さ
	//------------------------------------------------------------------------
	void SetVertex(float x, float y, float w, float h);

	//! @brief 色取得
	YsVec4&		GetColor() { return m_Color; }

	//! @brief 色設定
	//! @param[in]	color	… セットする色
	void SetColor(const YsVec4& color){
		m_Color = color;
	}

	//! @brief テクスチャセットを取得
	YsTextureSet&	GetTex(){ return m_TexSet; }

	//! @brief テクスチャセットをセットする
	//! @param[in]	texSet	… セットするTextureSet
	void SetTex(const YsTextureSet* texSet){
		m_TexSet = *texSet;
	}

	//! @brief 単品のテクスチャをセット
	//! @param[in]	tex	… セットするテクスチャ
	void SetTex(const SPtr<YsTexture> tex){
		m_TexSet.MeshTex = tex;
	}

	//=======================================================================
	// UVアニメーション関係
	//=======================================================================

	//! @brief UVアニメーター取得
	YsUVAnimator&	GetAnime() { return m_Anime; }

	//=======================================================================
	// 描画
	//=======================================================================

	//! @brief 描画
	void Draw();



	//
	YsBillBoard() : m_Color(1, 1, 1, 1){
		m_xCnt = m_yCnt = 1;
		m_AnimeNum = 0;
		m_fAnimePos = 0;

		SetVertex(-0.5f, -0.5f, 1, 1);
		SetColor(YsVec4(1, 1, 1, 1));

		for (int i = 0; i < 4; i++) {
			m_V[i].Color = YsVec4(1,1,1,1);
		}

	}

	~YsBillBoard(){
	}

private:

	YsVertex_Pos_UV_Color	m_V[4];		//!< 頂点データ

	YsVec4				m_Color;		//!< 色

	YsTextureSet		m_TexSet;		//!< テクスチャセット


	// アニメーション用
	int					m_xCnt, m_yCnt;		//!< アニメーションのX,Y分割数
	int					m_AnimeNum;			//!< アニメーションする数
	float				m_fAnimePos;		//!< 現在のアニメの位置
	YsUVAnimator		m_Anime;

	//
	SPtr<YsPolygon>		m_pPoly;

	//! 作業用ポリゴンデータ(全てのYsLaserで使いまわします)
	static WPtr<YsPolygon>	s_wpPolySquare;
};

}
#endif
