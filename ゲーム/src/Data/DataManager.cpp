#include "main.h"
#include "Game/GameWorld/GameWorld.h"
#include "DataManager.h"
#include "FilePath.h"


//int DataManager::TxtDataLoad(const std::string& _FileName)
//{
//	std::ifstream ifs;
//
//	//	読み込み
//	ifs.open(_FileName.c_str());
//
//	//	読み込み失敗
//	if (!ifs)
//	{
//		//	メッセージをだす
//		MessageBox(NULL, "テキストファイルが読み込めていません", "警告", MB_OK);
//
//		//	何もせず返す
//		return 0;
//	}
//
//	int num_cnt = 0;
//
//	for (;;)
//	{
//		std::string line;
//
//		//	一行読み込む
//		getline(ifs, line);
//
//		//	読み込み終了
//		if (ifs.fail())
//		{
//			break;
//		}
//
//
//		/*---------------------------------------------------------------------
//
//		区切りたい文字を設定、vector配列に格納(切り取りたい文字は複数指定可)
//
//		例
//		string sp = "abc";
//
//		sp = GetSplit(line, "ac");
//
//		spには"abc"のうちaとcが切り取られたbのみが帰ってくる
//
//		---------------------------------------------------------------------*/
//		std::vector<std::string> sp = GetSplit(line, "(),");
//
//
//		//	デフォルトステータス
//		YsVec3 vPos = YsVec3(0.0f, 0.0f, 0.0f);
//		YsVec3 vScale = YsVec3(1.0f, 1.0f, 1.0f);
//		YsVec3 vAngle = YsVec3(0.0f, 0.0f, 0.0f);
//		INT		 Id = -1;
//
//
//		//	最初のアドレスをイテレータに格納
//		auto FileIt = sp.begin();
//
//
//		/*-----------------------------------------------------
//
//		指定されたタグが見つかった場合、そのタグ内の数値を
//		用意されたステータス用の変数に代入
//		数値は+2の位置に指定
//
//
//		タグ
//		{
//		Pos				:	座標ベクトル
//		Scale			:	拡大ベクトル
//		Angle			:	角度ベクトル
//		Id			:	識別番号
//		}
//
//		-----------------------------------------------------*/
//
//		while (FileIt != sp.end())
//		{
//			if ((*FileIt) == "Pos")
//			{
//				FileIt += 2;
//				vPos.x = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vPos.y = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vPos.z = static_cast<float>(atof((*FileIt).c_str()));
//			}
//			else if ((*FileIt) == "Scale")
//			{
//				FileIt += 2;
//				vScale.x = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vScale.y = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vScale.z = static_cast<float>(atof((*FileIt).c_str()));
//			}
//			else if ((*FileIt) == "Angle")
//			{
//				FileIt += 2;
//				vAngle.x = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vAngle.y = static_cast<float>(atof((*FileIt).c_str()));
//
//				FileIt += 2;
//				vAngle.z = static_cast<float>(atof((*FileIt).c_str()));
//			}
//			/*else if ((*FileIt) == "Id")
//			{
//				FileIt += 2;
//				Id = atoi((*FileIt).c_str());
//			}*/
//
//
//			++FileIt;
//		}
//
//		//-----------------------------------------------------
//		//
//		//	Classタグの中を確認して、指定したClass名があれば
//		//	インスタンス化してObjectManagerにプッシュする
//		//	ステータスは先に設定値を代入する
//		//
//		//-----------------------------------------------------
//		FileIt = sp.begin();
//
//		while (FileIt != sp.end())
//		{
//			//====================
//			// キャラクタ
//			//====================
//			//プレイヤー
//			{
//				if ((*FileIt) == "Player")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(PLAYER_PATH);
//
//					CreateGameObject<Player>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::PLAYER);
//					num_cnt++;
//				}
//			}
//
//			//====================
//			//オブジェクト
//			//====================
//			//ステージ
//			{
//				if ((*FileIt) == "Ground")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(GROUND_PATH);
//
//					CreateGameObject<Ground>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::GROUND);
//					num_cnt++;
//				}
//			}
//
//			//拠点
//			{
//				if ((*FileIt) == "Stronghold")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Stronghold>(num_cnt, vPos, vScale, vAngle, drawModel,OBJECT_LIST::ID::STRONGHOLD);
//					num_cnt++;
//				}
//			}
//
//			//壁
//			{
//				if ((*FileIt) == "Wall")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Wall>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::WALL);
//					num_cnt++;
//				}
//			}
//
//			//投石器
//			{
//				if ((*FileIt) == "Sling")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Sling>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::SLING);
//					num_cnt++;
//				}
//			}
//
//			//大砲
//			{
//				if ((*FileIt) == "Cannon")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Cannon>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::CANNON);
//					num_cnt++;
//				}
//			}
//
//			//火炎放射器
//			{
//				if ((*FileIt) == "Flamethrower")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Flamethrower>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::FLAMETHROWER);
//					num_cnt++;
//				}
//			}
//
//			//プレス機
//			{
//				if ((*FileIt) == "Press")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh("");
//
//					CreateGameObject<Press>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::PRESS);
//					num_cnt++;
//				}
//			}
//
//			//アーム
//			{
//				if ((*FileIt) == "Arm")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(ARM_PATH);
//
//					CreateGameObject<Arm>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::ARM);
//					num_cnt++;
//				}
//			}
//
//			//ベルトコンベア
//			{
//				if ((*FileIt) == "Belt")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(BELT_PATH);
//
//					CreateGameObject<BeltConveyor>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::BELTCONVEYOR);
//					num_cnt++;
//				}
//			}
//
//			//ブリッジ
//			{
//				if ((*FileIt) == "Bridge")
//				{
//					// 描画用メッシュ読み込み
//					SPtr<YsGameModel> drawModel = YsDx.GetResStg()->LoadMesh(BRIDGE_PATH);
//
//					CreateGameObject<Bridge>(num_cnt, vPos, vScale, vAngle, drawModel, OBJECT_LIST::ID::BRIDGE);
//					num_cnt++;
//				}
//			}
//
//			++FileIt;
//
//		}
//	}
//
//	return num_cnt;
//}
//
//void DataManager::TxtDataSave(const std::string& _FileName, int _num)
//{
//
//	std::ofstream ofs;
//
//	// 読み込み
//	ofs.open(_FileName.c_str());
//
//	//	読み込み失敗
//	if (!ofs)
//	{
//		//	メッセージをだす
//		MessageBox(NULL, "テキストファイルが読み込めていません", "警告", MB_OK);
//
//		//	何もせず返す
//		return;
//	}
//
//	// テキスト内のデータを消去
//	ofs.trunc;
//
//	//作業用変数
//	static int num_cnt;
//	YsVec3 Pos = YsVec3(0.0f, 0.0f, 0.0f);
//	YsVec3 Scale = YsVec3(1.0f, 1.0f, 1.0f);
//	YsVec3 Angle = YsVec3(0.0f, 0.0f, 0.0f);
//
//	//リスト内をすべて回す
//	for (num_cnt = 0; num_cnt < _num; num_cnt++) {
//
//		//仮データを格納
//		Pos = HELP.GetObject_All(num_cnt)->GetPos();
//		Scale = HELP.GetObject_All(num_cnt)->GetScale();
//		if (HELP.GetObject_All(num_cnt)->GetObjId() == OBJECT_LIST::ID::PLAYER) {
//			Angle = HELP.GetObject_All(num_cnt)->GetAxis();
//		}
//		else {
//			Angle = HELP.GetObject_All(num_cnt)->GetAngle();
//		}
//
//
//		//クラス情報保存
//		switch (HELP.GetObject_All(num_cnt)->GetObjId()) {
//			//プレイヤー
//		case OBJECT_LIST::ID::PLAYER:
//			ofs << "ClassName(Player)" << TXT_DATA_SAVE << std::endl; break;
//			//エネミー
//		case OBJECT_LIST::ID::ENEMY:
//			ofs << "ClassName(Enemy)" << TXT_DATA_SAVE << std::endl; break;
//			//ステージ
//		case OBJECT_LIST::ID::GROUND:
//			ofs << "ClassName(Ground)" << TXT_DATA_SAVE << std::endl; break;
//			//拠点
//		case OBJECT_LIST::ID::STRONGHOLD:
//			ofs << "ClassName(Stronghold)" << TXT_DATA_SAVE << std::endl; break;
//			//壁
//		case OBJECT_LIST::ID::WALL:
//			ofs << "ClassName(Wall)" << TXT_DATA_SAVE << std::endl; break;
//			//投石器
//		case OBJECT_LIST::ID::SLING:
//			ofs << "ClassName(Sling)" << TXT_DATA_SAVE << std::endl; break;
//			//大砲
//		case OBJECT_LIST::ID::CANNON:
//			ofs << "ClassName(Cannon)" << TXT_DATA_SAVE << std::endl; break;
//			//火炎放射器
//		case OBJECT_LIST::ID::FLAMETHROWER:
//			ofs << "ClassName(Flamethrower)" << TXT_DATA_SAVE << std::endl; break;
//			//プレス機
//		case OBJECT_LIST::ID::PRESS:
//			ofs << "ClassName(Press)" << TXT_DATA_SAVE << std::endl; break;
//			//アーム
//		case OBJECT_LIST::ID::ARM:
//			ofs << "ClassName(Arm)" << TXT_DATA_SAVE << std::endl; break;
//			//ベルトコンベア
//		case OBJECT_LIST::ID::BELTCONVEYOR:
//			ofs << "ClassName(Belt)" << TXT_DATA_SAVE << std::endl; break;
//			//ブリッジ
//		case OBJECT_LIST::ID::BRIDGE:
//			ofs << "ClassName(Bridge)" << TXT_DATA_SAVE << std::endl; break;
//		}
//	}
//
//	//ファイルクローズ
//	ofs.close();
//}
//
///*trim------------------------------------------->
//
//指定された文字列を切り取り
//切り取った文字列の前後のいらない部分を削り取る
//
//_Str				: 切り取りたい文字列の元
//_TrimCharacterList	: いらない文字列
//
//return : string型
//
//<-----------------------------------------------*/
//static std::string trim(const std::string& _Str, const char* _TrimCharacterList = " \t\v\r\n")
//{
//	std::string result;
//
//	// 左側からトリムする文字以外が見つかる位置を検索します。
//	std::string::size_type left = _Str.find_first_not_of(_TrimCharacterList);
//
//	if (left != std::string::npos)
//	{
//		// 左側からトリムする文字以外が見つかった場合は、同じように右側からも検索します。
//		std::string::size_type right = _Str.find_last_not_of(_TrimCharacterList);
//
//		// 戻り値を決定します。ここでは右側から検索しても、トリムする文字以外が必ず存在するので判定不要です。
//		result = _Str.substr(left, right - left + 1);
//	}
//
//	return result;
//}
//
//
//std::vector<std::string> DataManager::GetSplit(
//	std::string _Str,
//	std::string _Split)
//{
//	//	文字を格納するための変数
//	std::vector<std::string> vecRet;
//
//
//	//	検索したい候補のうちのいずれかがヒットすれば
//	while (_Str.find_first_of(_Split) != std::string::npos)
//	{
//
//		//	切り取りたい文字列を設定
//		_Str = trim(_Str, " ");
//		_Str = trim(_Str, "\t\v\r\n");
//
//		//	切り取りたい文字数を取得
//		int Cp = _Str.find_first_of(_Split);
//
//		//	文字数が一つのみだった場合
//		if (Cp <= 0)
//		{
//			Cp += 1;
//		}
//
//		//	先頭空文字が格納されないように
//		if (Cp > 0)
//		{
//
//			//	格納
//			vecRet.push_back(_Str.substr(0, Cp));
//
//		}
//
//		//	区切り文字が二文字以上に対応
//		_Str = _Str.substr(Cp);
//	}
//
//	//	リストを返す
//	return vecRet;
//}







SPtr<CharacterBase> DataManager::CsvDataLoad(const std::string& _FileName)
{

	//===========================
	// CSVデータ読み込み
	//===========================
	csv.Load(_FileName);

	SPtr<CharacterBase> p;

	//読み込み・配置(０行目はタイトル行なので、１行目から)
	for (UINT i = 1; i < csv.m_Tbl.size(); i++) {
		// データ取得
		YsVec3 pos;
		pos.x = std::stof(csv.GetCol(i, "X座標"));
		pos.y = std::stof(csv.GetCol(i, "Y座標"));
		pos.z = std::stof(csv.GetCol(i, "Z座標"));
		float size = std::stof(csv.GetCol(i, "サイズ"));
		YsVec3 rotate;
		rotate.x = std::stof(csv.GetCol(i, "角度X"));
		rotate.y = std::stof(csv.GetCol(i, "角度Y"));
		rotate.z = std::stof(csv.GetCol(i, "角度Z"));
		std::string className = csv.GetCol(i, "クラス");

		YsVec3 scale = YsVec3(size, size, size);

		//モデル用
		std::string modelName;
		SPtr<YsGameModel> drawModel;

		//仮行列
		YsMatrix mat;

		//　座標をセットする
		mat.CreateMove(pos);

		//　拡大サイズをセットする
		mat.Scale_Local(size, size, size);

		//　回転量をセットする
		mat.RotateY_Local(rotate.y);
		mat.RotateX_Local(rotate.y);
		mat.RotateZ_Local(rotate.y);

		//====================
		// キャラクタ
		//====================
		//プレイヤー
		{
			if (className == "Player")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Player> add = CharacterBase::CreateCharaTask<Player>();

				//メッシュ読み込み
				modelName = PLAYER_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);

				//プレイやーの情報を保存
				p = add;
			}
		}

		//ボス
		{
			if (className == "Boss")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Boss> add = CharacterBase::CreateCharaTask<Boss>();

				//メッシュ読み込み
				modelName = BOSS_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//スケルトン
		{
			if (className == "Skeleton")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Skeleton> add = CharacterBase::CreateCharaTask<Skeleton>();

				//メッシュ読み込み
				modelName = SKELETON_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//デーモン
		{
			if (className == "Demon")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Demon> add = CharacterBase::CreateCharaTask<Demon>();

				//メッシュ読み込み
				modelName = DEMON_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//====================
		//オブジェクト
		//====================
		//ステージ
		{
			if (className == "Ground")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Ground> add = CharacterBase::CreateCharaTask<Ground>();

				//メッシュ読み込み
				modelName = GROUND_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//拠点
		{
			if (className == "Stronghold")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Stronghold> add = CharacterBase::CreateCharaTask<Stronghold>();

				//メッシュ読み込み
				modelName = STRONGHOLD_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//壁
		{
			if (className == "Wall")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Wall> add = CharacterBase::CreateCharaTask<Wall>();

				//メッシュ読み込み
				modelName = WALL_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//投石器
		{
			if (className == "Sling")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Sling> add = CharacterBase::CreateCharaTask<Sling>();

				//メッシュ読み込み
				modelName = SLING_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//大砲
		{
			if (className == "Cannon")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Cannon> add = CharacterBase::CreateCharaTask<Cannon>();

				//メッシュ読み込み
				modelName = CANNON_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//火炎放射器
		{
			if (className == "Flamethrower")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Flamethrower> add = CharacterBase::CreateCharaTask<Flamethrower>();

				//メッシュ読み込み
				modelName = FLAMETHROWER_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//プレス機
		{
			if (className == "Press")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Press> add = CharacterBase::CreateCharaTask<Press>();

				//メッシュ読み込み
				modelName = PRESS_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//アーム
		{
			if (className == "Arm")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Arm> add = CharacterBase::CreateCharaTask<Arm>();

				//メッシュ読み込み
				modelName = ARM_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//ベルトコンベア
		{
			if (className == "BeltConveyor")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<BeltConveyor> add = CharacterBase::CreateCharaTask<BeltConveyor>();

				//メッシュ読み込み
				modelName = BELTCONVEYOR_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}

		//ブリッジ
		{
			if (className == "Bridge")
			{
				//　リストにインスタンス化したオブジェクトをプッシュする
				SPtr<Bridge> add = CharacterBase::CreateCharaTask<Bridge>();

				//メッシュ読み込み
				modelName = BRIDGE_PATH;
				drawModel = YsDx.GetResStg()->LoadMesh(modelName);

				//回転量セット（オブジェクト用）
				add->SetAngle(rotate);

				//　オブジェクトの初期化をする
				add->Init(mat, drawModel);
			}
		}
	}

	return p;
}

void DataManager::CsvDataSave(const std::string& _FileName)
{
	std::ofstream ofs;

	// 読み込み
	ofs.open(_FileName.c_str());

	//	読み込み失敗
	if (!ofs)
	{
		//	メッセージをだす
		MessageBox(NULL, "テキストファイルが読み込めていません", "警告", MB_OK);

		//	何もせず返す
		return;
	}

	// テキスト内のデータを消去
	ofs.trunc;

	//作業用変数
	YsVec3 Pos = YsVec3(0.0f, 0.0f, 0.0f);
	float size = 1.0f;
	YsVec3 Angle = YsVec3(0.0f, 0.0f, 0.0f);

	std::vector<SPtr<CharacterBase>> cList = CharacterBase::GetList();

	//種類名を先に保存
	ofs << "X座標," << "Y座標," << "Z座標," << "サイズ," << "角度X," << "角度Y," << "角度Z," << "クラス" << std::endl;

	//リスト内をすべて回す
	for (UINT i = 0; i < cList.size(); i++) {

		//仮データを格納
		Pos = cList[i]->GetPos();
		size = cList[i]->GetScale().x;
		if (cList[i]->GetObjId() == OBJECT_LIST::ID::PLAYER) {
			Angle = cList[i]->GetAxis();
		}
		else {
			Angle = cList[i]->GetAngle();
		}


		//クラス情報保存
		switch (cList[i]->GetObjId()) {
			//プレイヤー
		case OBJECT_LIST::ID::PLAYER:
			ofs << CSV_DATA_SAVE << "Player" << std::endl; break;
			//ボス
		case OBJECT_LIST::ID::BOSS:
			ofs << CSV_DATA_SAVE << "Boss" << std::endl; break;
			//スケルトン
		case OBJECT_LIST::ID::SKELETON:
			ofs << CSV_DATA_SAVE << "Skeleton" << std::endl; break;
			//デーモン
		case OBJECT_LIST::ID::DEMON:
			ofs << CSV_DATA_SAVE << "Demon" << std::endl; break;
			//ステージ
		case OBJECT_LIST::ID::GROUND:
			ofs << CSV_DATA_SAVE << "Ground" << std::endl; break;
			//拠点
		case OBJECT_LIST::ID::STRONGHOLD:
			ofs << CSV_DATA_SAVE << "Stronghold" << std::endl; break;
			//壁
		case OBJECT_LIST::ID::WALL:
			ofs << CSV_DATA_SAVE << "Wall" << std::endl; break;
			//投石器
		case OBJECT_LIST::ID::SLING:
			ofs << CSV_DATA_SAVE << "Sling" << std::endl; break;
			//大砲
		case OBJECT_LIST::ID::CANNON:
			ofs << CSV_DATA_SAVE << "Cannon" << std::endl; break;
			//火炎放射器
		case OBJECT_LIST::ID::FLAMETHROWER:
			ofs << CSV_DATA_SAVE << "Flamethrower" << std::endl; break;
			//プレス機
		case OBJECT_LIST::ID::PRESS:
			ofs << CSV_DATA_SAVE << "Press" << std::endl; break;
			//アーム
		case OBJECT_LIST::ID::ARM:
			ofs << CSV_DATA_SAVE << "Arm" << std::endl; break;
			//ベルトコンベア
		case OBJECT_LIST::ID::BELTCONVEYOR:
			ofs << CSV_DATA_SAVE << "BeltConveyor" << std::endl; break;
			//ブリッジ
		case OBJECT_LIST::ID::BRIDGE:
			ofs << CSV_DATA_SAVE << "Bridge" << std::endl; break;
		}
	}

	cList.clear();

	//ファイルクローズ
	ofs.close();
}