//===============================================================
//! @file YsMTRand.h
//! @brief 乱数クラス
//!
//! @author 鎌田
//===============================================================

#ifndef __YsMTRand_h__
#define __YsMTRand_h__

namespace YskLib {

//=================================================================
//!
//! @brief メルセンヌ･ツイスター乱数クラス
//!
//! @ingroup Etc
//=================================================================
class YsMTRand {
public:
	//! @brief 乱数シードを設定する(つまりシャッフル)
	//! @param[in]	seed	… 乱数シード
	void InitSeed(ULONG seed) {
		m_Rnd.seed(seed);
	}

	//! @brief 整数の乱数取得
	UINT GetInt() { return m_Rnd(); }

	//! @brief 整数の乱数取得
	//! @param[in]	num	… 0 ～ num-1 までの乱数になる
	int GetInt(int num) {
		return m_Rnd() % num;
	}

	//! @brief  st ～ ed間の整数乱数を発生
	//! @param[in]	st	… 最少値
	//! @param[in]	ed	… 最大値
	int GetInt(int st, int ed) {
		if(ed < st)std::swap(ed, st);
		return (int)GetInt(ed - st + 1) + st;
	}

	//! @brief st ～ ed間の浮動小数点乱数を発生
	//! @param[in]	st	… 最少値
	//! @param[in]	ed	… 最大値
	float GetFloat(float st, float ed) {
		if(ed < st)std::swap(ed, st);
		return (GetInt() / 4294967295.0f)*(ed - st) + st;
	}

	//
	YsMTRand() {}
	YsMTRand(ULONG seed) : m_Rnd(seed) {
	}

private:
	std::mt19937	m_Rnd;		//!< メルセンヌ・ツイスター乱数
};

}

#endif
