//===============================================================
//! @file YsXEDLoader.h
//! @brief XEDファイル読み込む
//!
//! XEDファイルは、KAMADA SkinMesh Editorで作成できるモデル形式です
//!
//! @author 鎌田
//===============================================================

#ifndef __YsXEDLoader_h__
#define __YsXEDLoader_h__

namespace YskLib
{

//=================================================================================
//! @brief XEDファイル読み込みクラス
//!
//! @param[in] fileName	… ファイルパス
//! @param[out] outModelTbl	… 結果をいれる、メッシュ配列を指定
//! @param[out] outBoneTree	… 結果をいれる、ボーン配列を指定
//! @param[out] outAnimeTbl	… 結果をいれる、アニメデータ配列を指定
//! @param[in]  bakeCurve	… 曲線補間系のアニメキーは、全て線形補間として変換・追加する(処理が軽くなります)
//!
//! @ingroup Graphics_Model
//=================================================================================
bool YsLoadXEDFile(
	const std::string& fileName,
	std::vector<SPtr<YsSingleModel>>& outModelTbl,
	std::vector<SPtr<YsGameModel::BoneNode>>& outBoneTree,
	std::vector<SPtr<YsAnimationSet>>& outAnimeTbl,
	std::vector< std::vector<YsXEDRigidBody> >&	outRigidBodyTbl,
	bool bakeCurve
);


}

#endif
