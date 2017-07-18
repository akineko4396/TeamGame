//===============================================================
//! @file YsLaser.h
//! @brief 連続ポリゴンクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsLaser_h__
#define __YsLaser_h__

namespace YskLib{

//===============================================================
//!
//! @brief 曲線レーザー
//!
//! 連続したポリゴンを生成し描画する。	\n
//! 曲線レーザーや剣の軌跡表現に使える
//!
//! @ingroup Graphics_Model
//===============================================================
class YsLaser{
public:
	//====================================================
	// 情報取得
	//====================================================

	//! @brief 座標リスト取得
	std::list<YsVec3>&		GetList(){return m_List;}

	//! @brief 幅
	
	float					GetW(){ return m_W; }
	//! @brief 幅をセット
	void					SetW(float w){ m_W = w; }
	
	//! @brief 色
	YsVec4&					GetColor(){ return m_Color; }
	
	//! @brief 色をセット
	void					SetColor(const YsVec4* color){m_Color = *color;}

	//! @brief レーザーのポイント数取得
	UINT					GetPointNum(){return m_List.size();}

	//! @brief テクスチャセット取得
	YsTextureSet&			GetTex(){ return m_TexSet; }
	//! @brief テクスチャをセット
	void					SetTex(const YsTextureSet* texSet){ if(texSet){m_TexSet = *texSet;} }

	//====================================================
	//! @brief 初期設定
	//! @param[in]	w		… レーザーの幅
	//!	@param[in]	color	… 色
	//====================================================
	void Init(float w, const YsVec4* color = nullptr){
		m_W = w;
		if(color)m_Color = *color;
		m_List.clear();
	}

	//====================================================
	//! @brief リストをクリア
	//====================================================
	void ClearList(){
		m_List.clear();
	}

	//====================================================
	// ポイント追加
	//====================================================

	//--------------------------------------------------------------------------
	//! @brief リストの後ろに追加
	//! @param[in]	pos	… 座標
	//--------------------------------------------------------------------------
	void AddPoint_Back(const YsVec3& pos){
		m_List.push_back(pos);
	}

	//--------------------------------------------------------------------------
	//! @brief リストの手前に追加
	//! @param[in]	pos	… 座標
	//--------------------------------------------------------------------------
	void AddPoint_Front(const YsVec3& pos){
		m_List.push_front(pos);
	}

	//====================================================
	// ポイント削除
	//====================================================

	//! @brief リストの先頭を削除
	void DelPoint_Front(){
		if(m_List.size() > 0){
			m_List.erase(m_List.begin());
		}
	}

	//! @brief リストの後ろを削除
	void DelPoint_Back(){
		if(m_List.size() > 0){
			m_List.erase(--m_List.end());
		}
	}

	//====================================================
	// 描画
	//====================================================
	//! @brief 点の左右に頂点を配置し、曲線レーザーとして描画
	//!
	//! ※違和感をなくすため、カメラからみて左右に配置される		\n
	//! ●―――●―――●―――●―――●							\n
	//! | ＼    | ＼    | ＼    | ＼    |							\n
	//! 0   ＼  1   ＼  2   ＼  3   ＼  4 …　上下の幅はm_Wとなる	\n
	//! |     ＼|     ＼|     ＼|     ＼|							\n
	//! ●―――●―――●―――●―――●							\n
	//! 
	//! @param[in]	vCamPos	… カメラの座標
	void Draw(const YsVec3& vCamPos);

	//! @brief ２つの点を１本の棒とし、その棒同士をポリゴンで結ぶ。
	//!
	//! さらに棒同士の間にスプライン補間で追加の棒を作り滑らかに描画する。	\n
	//! 剣を振った時の軌跡などで使用。										\n
	//! 1―――3―――5―――7―――9										\n
	//! |＼    |＼    |＼    |＼    | 										\n
	//! |  ＼  |  ＼  |  ＼  |  ＼  | …　※必ず偶数で点の数を登録すること！\n
	//! |    ＼|    ＼|    ＼|    ＼| 										\n
	//! 0―――2―――4―――6―――8										\n
	//!
	//! @param[in]	detailNum	… 棒間の分割数 0だと補間なし
	void DrawStrip(int detailNum = 5);

protected:

	std::list<YsVec3>		m_List;		//!< 座標リスト
	float					m_W;		//!< 幅
	YsVec4					m_Color;	//!< 色

	YsTextureSet			m_TexSet;	//!< テクスチャセット

	SPtr<YsPolygon>			m_pPoly;	//!< ポリゴンモデルクラス

	//! 作業用ポリゴンデータ(全てのYsLaserで使いまわします)
	static WPtr<YsPolygon>	s_wpPoly;


public:
	YsLaser() : m_Color(1,1,1,1){
		m_W = 0.1f;
	}

};

}
#endif