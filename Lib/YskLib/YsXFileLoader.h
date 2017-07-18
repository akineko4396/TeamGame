//===============================================================
//! @file YsXFileLoader.h
//! @brief Xファイル読み込み系
//!
//! Direct3D9の機能を使い読み込みを行っています
//!
//! @author 鎌田
//===============================================================

#ifndef __YsXFileLoader_h__
#define __YsXFileLoader_h__

namespace YskLib
{

//=================================================================================
//! @brief スキンメッシュXファイル読み込みクラス
//!
//! @param[in] fileName	: ファイルパス
//! @param[out] outModelTbl	: 結果をいれる、メッシュ配列を指定
//! @param[out] outBoneTree	: 結果をいれる、ボーン配列を指定
//! @param[out] outAnimeTbl	: 結果をいれる、アニメデータ配列を指定
//!
//! @ingroup Graphics_Model
//=================================================================================
bool YsLoadXFile(
	const std::string& fileName,
	std::vector<SPtr<YsSingleModel>>& outModelTbl,
	std::vector<SPtr<YsGameModel::BoneNode>>& outBoneTree,
	std::vector<SPtr<YsAnimationSet>>& outAnimeTbl
);


}
#endif
