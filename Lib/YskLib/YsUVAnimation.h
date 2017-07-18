//===============================================================
//! @file YsUVAnimation.h
//! @brief キーフレームアニメーション系のクラス群
//!
//! @author 鎌田
//===============================================================
#ifndef __YsUVAnimation_h__
#define __YsUVAnimation_h__

namespace YskLib
{

//=======================================================================
//!
//! @brief UVアニメーション管理クラス
//!
//! 1枚の画像を、縦と横に等間隔分割アニメーションするときに使用する		\n
//!																		\n
//! ・Init関数で、縦と横の分割数や最大アニメ数を設定する。				\n
//! ・Animation関数で、アニメーション位置を進める。						\n
//! ・GetUV関数で、現在のアニメ位置から、UV座標を算出してくれる。
//!
//! @ingroup Animation
//=======================================================================
class YsUVAnimator {
public:
	//------------------------------------------------------------------------
	//! @brief アニメーション情報設定
	//! @param[in]	xCount		… 横 分割数
	//! @param[in]	yCount		… 縦 分割数
	//! @param[in]	nAnimeNum	… 最大アニメ数　-1で xCount*yCount になる 
	//------------------------------------------------------------------------
	void Init(int xCount = 1, int yCount = 1, int animeMaxNum = -1) {
		m_xCnt = xCount;
		m_yCnt = yCount;
		if (animeMaxNum < 0) {
			m_AnimeMaxNum = m_xCnt * m_yCnt;
		}
		else {
			m_AnimeMaxNum = animeMaxNum;
		}

		m_fAnimePos = 0;
	}

	//------------------------------------------------------------------------
	//! @brief アニメーション進行
	//! @param[in]	fSpeed	… アニメ速度
	//! @param[in]	bLoop	… ループ再生する？
	//------------------------------------------------------------------------
	void Animation(float fSpeed, bool bLoop);

	//------------------------------------------------------------------------
	//! @brief 現在のアニメ位置から、UVを取得
	//! @param[out]	uvMin	… 矩形の左上座標(UV座標系 0～1)が格納される
	//! @param[out]	uvMax	… 矩形の右下座標(UV座標系 0～1)が格納される
	//------------------------------------------------------------------------
	void GetUV(YsVec2* uvMin, YsVec2* uvMax) {
		int No = (int)m_fAnimePos;
		int x = No % m_xCnt;
		int y = No / m_xCnt;
		float xAddr = 1.0f / m_xCnt;
		float yAddr = 1.0f / m_yCnt;

		if (uvMin) {
			uvMin->x = xAddr*x;
			uvMin->y = yAddr*y;
		}
		if (uvMax) {
			uvMax->x = xAddr*(x + 1);
			uvMax->y = yAddr*(y + 1);
		}
	}

	//------------------------------------------------------------------------
	//! @brief アニメ位置変更
	//! @param[in]	idx	… アニメのコマ番号
	//------------------------------------------------------------------------
	void SetPos(int idx) {
		m_fAnimePos = (float)idx;
	}

	//------------------------------------------------------------------------
	//! @brief 現在のアニメ位置を取得
	//------------------------------------------------------------------------
	float GetPos() { return m_fAnimePos; }

	//------------------------------------------------------------------------
	//! @brief アニメ終了した？
	//! ※ループ再生時はtrueが返ってきません
	//! @return true:再生終了
	//------------------------------------------------------------------------
	bool IsEnd() {
		if ((int)m_fAnimePos >= m_AnimeMaxNum - 1)return true;
		return false;
	}


	//------------------------------------------------------------------------
	//! @brief アニメ総数を取得
	//------------------------------------------------------------------------
	int GetMaxNum() { return m_AnimeMaxNum; }

	// 
	YsUVAnimator() : m_xCnt(1), m_yCnt(1), m_AnimeMaxNum(1), m_fAnimePos(0){
	}
	
private:
	int					m_xCnt, m_yCnt;		//!< アニメーションのX,Y分割数
	int					m_AnimeMaxNum;		//!< アニメーションする数
	float				m_fAnimePos;		//!< 現在のアニメの位置

};


}

#endif
