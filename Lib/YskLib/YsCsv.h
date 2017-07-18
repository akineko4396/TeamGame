//===============================================================
//! @file YsCsv.h
//! @brief Csv形式のファイル関係
//!
//! @author 鎌田
//===============================================================
#ifndef __YsCsv_h__
#define __YsCsv_h__

namespace YskLib{

//===============================================
//! @brief CSVファイル入出力クラス
//! ・日本語文字がカンマや改行と勘違いしないように対応もしてます
//! @ingroup Etc
//===============================================
class YsCsv{
public:
	//========================================================
	//! brief CSVデータ
	//========================================================
	std::vector<std::vector<std::string>> m_Tbl;

	//========================================================
	//! brief CSVファイルから、m_Tblに読み込む
	//! @param[in]	filename	… CSVファイル名
	//! @param[in]	bDelTitle	… 最初の１行目は省略するか？
	//========================================================
	bool Load(const std::string& filename, bool bDelTitle = false);
	
	//========================================================
	//! brief CSV形式文字列から、m_Tblに読み込む
	//! @param[in]	text		… CSV形式の文字列
	//! @param[in]	bDelTitle	… 最初の１行目は省略するか？
	//========================================================
	bool LoadFromText(const std::string& text, bool bDelTitle = false);

	//========================================================
	//! brief m_Tblをファイルへ保存
	//! @param[in]	filename	… CSVファイル名
	//========================================================
	bool Save(const std::string& filename);

	//========================================================
	//! brief 指定した列に指定文字列があるか、全行を検索する
	//! @param[in]	nCol	… 指定列
	//! @param[in]	str		… 検索文字列
	//! @return 発見：行Index　無い：-1
	//========================================================
	int Search_Col(int nCol, const std::string& str){
		for(int i=0;i<(int)m_Tbl.size();i++){
			if(nCol >= (int)m_Tbl[i].size())continue;

			if(str == m_Tbl[i][nCol]){
				return i;
			}
		}
		return -1;
	}

	//========================================================
	//! brief 指定した行に指定文字列があるか、全列を検索する
	//! @param[in]	nRow	… 指定行
	//! @param[in]	str		… 検索文字列
	//! @return 発見：列Index　無い：-1
	//========================================================
	int Search_Row(int nRow, const std::string& str){
		if(nRow >= (int)m_Tbl.size())return -1;

		for(int i = 0; i<(int)m_Tbl[nRow].size(); i++){
			if(str == m_Tbl[nRow][i]){
				return i;
			}
		}
		return -1;
	}

	//========================================================
	//! brief 0行目をタグ文字として使い、tagと一致する文字列が何列目を求め、nRow行目のtag列目の文字列を返す
	//! @param[in]	nRow	… 指定行
	//! @param[in]	tag		… 列を判定するための文字列
	//! @return nRow行目のtag列目の文字列を返す　tagの列が存在しない場合は、""が返る
	//========================================================
	std::string GetCol(int nRow, const std::string& tag){
		int col = Search_Row(0, tag);
		if(col == -1)return "";

		return m_Tbl[nRow][col];
	}

	//! @brief データをクリアする
	void Clear(){
		m_Tbl.clear();
	}
};

}
#endif