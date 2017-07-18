#pragma once

//=================================================
// データを読み書きするクラス
//=================================================

class CharacterBase;

class DataManager
{
public:

	/*DataLoad------------------------------------

	外部ファイルを読み込む
	ゲームに使用するキャラクタやオブジェクトを
	インスタンス化してリストに格納する

	_FileName		:	ファイルパス

	return			:	オブジェクト数を返す

	--------------------------------------------*/
	//int TxtDataLoad(const std::string& _FileName);	//テキストファイルVer.

	SPtr<CharacterBase> CsvDataLoad(const std::string& _FileName);	//csvファイルVer.

	/*DataLoad------------------------------------

	外部ファイルに書きこむ
	ゲームに使用したキャラクタやオブジェクトを
	テキストファイルに書き出す

	_FileName		:	ファイルパス

	return			:	戻り値なし

	--------------------------------------------*/
	//void TxtDataSave(const std::string& _FileName, int _num);

	void CsvDataSave(const std::string& _FileName);

	/*GetSplit--------------------------------->

	指定した文字を切り取って
	データとなる文字列を返す
	戻り値として切り取った文字列を返す

	_Str	:	元となる文字列
	_Split	:	切り取りたい文字列(複数化)

	return	:	vector型

	<----------------------------------------*/
	/*std::vector<std::string> GetSplit(
		std::string _Str,
		std::string _Split
		);
		*/
private:
	//データ取得用
	YsCsv csv;
};