#include "YskLib.h"

using namespace YskLib;

//==========================================================================================
//
//
// ゲーム用多機能モデルクラス
//
//
//==========================================================================================

YsGameModel::YsGameModel()
{

}

YsGameModel::~YsGameModel()
{
	Release();
}

void YsGameModel::CreateRefMeshTbl()
{
	m_RefModelTbl_SkinOnly.clear();
	m_RefModelTbl_SkinOnly.shrink_to_fit();
	m_RefModelTbl_StaticOnly.clear();
	m_RefModelTbl_StaticOnly.shrink_to_fit();
	for(UINT i=0;i<m_ModelTbl.size();i++){
		if(m_ModelTbl[i]->GetMesh()->IsSkinMesh()){
			m_RefModelTbl_SkinOnly.push_back(m_ModelTbl[i]);
		}
		else{
			m_RefModelTbl_StaticOnly.push_back(m_ModelTbl[i]);
		}
	}
}

bool YsGameModel::LoadMesh(const std::string& fileName, bool bakeCurve)
{
	// ヘッダチェックチェック
	char head[4];
	FILE* fp = fopen(fileName.c_str(), "r");
	if(fp == nullptr)return false;

	fread(head, 3, 1, fp);
	head[3] = '\0';
	fclose(fp);

	// XEDファイル
	if(strcmp(head, "XED") == 0) {
		return LoadXEDFile(fileName, bakeCurve);
	}
	// Xファイル
	else if(strcmp(head, "xof") == 0) {
		return LoadXFile(fileName);
	}
	else{
		return false;
	}
}

//==========================================================================================
// スキンメッシュを読み込む
//==========================================================================================
bool YsGameModel::LoadXFile(const std::string& fileName)
{
	// 解放
	Release();

	// パス、ファイル名抽出
	char szDir[_MAX_PATH];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(fileName.c_str(), nullptr, szDir, szFname, szExt);
	m_FileName = szFname;
	m_FileName += szExt;
	m_FilePath = szDir;

	// XED読み込み
	if (YsLoadXFile(fileName, m_ModelTbl, m_BoneTree, m_AnimeList)) {

		// スキンメッシュとスタティックメッシュの参照を分けたテーブル作成
		CreateRefMeshTbl();

		return true;
	}
	return false;
}

bool YsGameModel::LoadXEDFile(const std::string& fileName, bool bakeCurve)
{
	// 解放
	Release();

	// パス、ファイル名抽出
	char szDir[_MAX_PATH];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(fileName.c_str(), nullptr, szDir, szFname, szExt);
	m_FileName = szFname;
	m_FileName += szExt;
	m_FilePath = szDir;

	// XED読み込み
	if(YsLoadXEDFile(fileName, m_ModelTbl, m_BoneTree, m_AnimeList, m_RigidBodyTbl, bakeCurve)) {

		// スキンメッシュとスタティックメッシュの参照を分けたテーブル作成
		CreateRefMeshTbl();

		//------------------------------------------------------------------------------------------
		// KAMADA SkinMesh Editorでの、マテリアルのテキスト部分を解析　※必要ないなら削除してよい
		// 全メッシュ
		for (auto& model : m_ModelTbl) {
			// 全マテリアル
			for (auto& mate : model->GetMaterials()) {
				// とりあえずCSVとして解析する
				YsCsv csv;
				csv.LoadFromText(mate.ExtScript);
				// 縦ループ
				for(UINT ci = 0;ci < csv.m_Tbl.size();ci++){
					const std::string strType = csv.m_Tbl[ci][0];
					// 
					if(strType == "なにか～"){

					}
				}
			}
		}
		//------------------------------------------------------------------------------------------

		return true;
	}
	return false;
	
}

//==========================================================================================
// スキンメッシュを解放
//==========================================================================================
void YsGameModel::Release()
{
	// ボーン解放
	m_BoneTree.clear();
	m_BoneTree.shrink_to_fit();

	// アニメーションデータ解放
	DeleteAnimation();

	// メッシュ解放
	m_ModelTbl.clear();
	m_ModelTbl.shrink_to_fit();
	m_RefModelTbl_SkinOnly.clear();
	m_RefModelTbl_SkinOnly.shrink_to_fit();
	m_RefModelTbl_StaticOnly.clear();
	m_RefModelTbl_StaticOnly.shrink_to_fit();

	m_FilePath.clear();
	m_FileName.clear();
}
