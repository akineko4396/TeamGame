#include "main.h"

#include "../../GameWorld.h"

#include "Game\GameWorld\CollisionEngine\CollisionEngine.h"

#include "Stage1.h"

#include "Game\Edit\EditMode.h"
//#include "Game\GameWorld\Effect\Effect.h"
//#include "Game\GameWorld\Item\Weapon.h"



void GameWorld::Init()
{
	// 物理エンジン初期化
	m_Bullet.Init();

	// カメラデータ初期化
	m_Cam.Init(20, 0, -5);

	// フェード
	//m_BlackCurtainAdd = -0.01f;
	//m_BlackCurtainPower = 1.0f;

	// モデル読み込み
	// テクスチャ読み込み
	// ステージ初期化

	//+++++++++++++++++++++++++++++++++++++++++
	// オブジェクトを生成する
	//+++++++++++++++++++++++++++++++++++++++++
	{
		//外部からゲームオブジェクトを読み込み
		UPtr<DataManager> uDM(new DataManager);

		SPtr<CharacterBase> p = uDM->CsvDataLoad("data/txt/Object.csv");

		// カメラ視点キャラとして記憶
		m_wpCamera = p;

		// プレイヤーとして記憶
		m_wpPlayer = p;
		
		// ものを配置するキャラ設定 
		m_Put.SetPlayer(p);


		//武器
		SPtr<Weapon_Hammer>h(new Weapon_Hammer);
		h->Init(p, static_cast<Player*>(p.get())->GetModelObject().SearchBone("右手ダミー"), nullptr);//
		static_cast<Player*>(p.get())->SetWeapon(h);
		BaseWeapon::PushObject(h);

	}
	{
		//箱作成
		/*for (int i = -10; i <= 10; i++){
			for (int j = -10; j <= 10; j++){
				SPtr<Box>box(new Box);
				YsMatrix m;
				m.CreateMove((float)i, 0, (float)j);
				box->Init(m);
				CharacterBase::PushObject(box);
			}
		}*/
	}
	{
		////水
		//for (int i = 0; i < 15; i++){
		//	SPtr<Water>w(new Water);
		//	YsMatrix m;
		//	m.CreateMove(0, 5.0f+(float)i*2.0f, 3.0f);
		//	w->Init(m, CharaID::WATER);
		//	CharacterBase::PushObject(w);
		//}
	}

	//Edit実装
	EditMode::CreateWorld();
	//開発者向けのマップエディタ
	EW.MapInit();

	NAI.Init();
}

void GameWorld::Release()
{
	m_Bullet.Release();
	m_wpCamera.reset();
	m_wpPlayer.reset();

	//外部からゲームオブジェクトを読み込み
	//UPtr<DataManager> uDM(new DataManager);
	//uDM->CsvDataSave("data/txt/Object.csv");

	CharacterBase::FullClear();

	EditMode::DeleteWorld();


	BaseEffectObj::FullClear();
	BaseWeapon::FullClear();
}

void GameWorld::Update()
{
	// ポーズOn/Off
	if (INPUT.KeyCheck_Enter('P')){
		m_PauseFlag = !m_PauseFlag;
	}

	// あたり判定デバッグ表示On/Off
	if (INPUT.KeyCheck_Enter('H')){
		m_Debug_DrawHit = !m_Debug_DrawHit;
	}

	//=========================================================
	// ゲーム処理
	//=========================================================
	if (m_PauseFlag == false){
		// 物理エンジン処理
		const int simuCnt = 1;
		m_Bullet.StepSimulation(1.0f / 60, simuCnt, 1.0f / 60 / simuCnt);

			
		// 当たり判定リストクリア
		m_ColEng.ClearList();

	
		// モード切り替え

		if (INPUT.KeyCheck_Enter('E')){
			m_EditFlg = EDIT;
		}
		if (INPUT.KeyCheck_Enter('M')){
			m_EditFlg = CREATE;
		}
		if (INPUT.KeyCheck_Enter('N')){
			m_EditFlg = NORMAL;
		}

		// Update処理

		if (m_EditFlg == EDIT){
			EditMode::Edit = true;
			EW.CamUpdate();
			EW.EditUpdate();
		}
		else if(m_EditFlg==CREATE){
			EditMode::Edit = false;
			EW.CamUpdate();
			EW.MapUpdate();
		}
		else if(m_EditFlg==NORMAL){
			CharacterBase::AllUpdate();
			BaseEffectObj::AllUpdate();
			
			m_Put.Update();
		}

		// タスクをクリアする処理

		CharacterBase::CheckKillFlg();



		// 当たり判定実行
		Dw_TimeBegin("HitTest"); // 処理時間計測(消しても良い)
		m_ColEng.Run();
		Dw_TimeEnd("HitTest", 2); // 処理時間計測(消しても良い)
	}
	// ポーズ中
	else{
		// カメラ処理
		m_Cam.Update();
	}


	// 暗幕処理
	m_BlackCurtainPower += m_BlackCurtainAdd;
	if (m_BlackCurtainPower > 1)m_BlackCurtainPower = 1;
	if (m_BlackCurtainPower < 0)m_BlackCurtainPower = 0;

}

void GameWorld::Draw()
{
	//==========================================
	// カメラ設定
	//==========================================
	// ポーズ中？
	if (m_PauseFlag == false){


		//----------------視点カメラをセットする----------------
		if (m_EditFlg == NORMAL){

			if (m_wpCamera.expired() == false){
				m_wpCamera.lock()->SetCamera();
			}

		}
		else
		// Editカメラ
		if (m_EditFlg == EDIT || m_EditFlg == CREATE){
			EW.SetCamera();
		}
		//--------------------------------------------------


	}
	// ポーズ中なら、GameWorldのカメラを使用
	else{
		m_Cam.SetCamera();
	}

	//==========================================
	// ライト設定
	//==========================================
	ShMgr.m_Samp.SetLightEnable(false); // ライト無効

	//==========================================
	// 描画直前にシェーダのフレーム単位データを更新する
	//==========================================
	ShMgr.UpdateBeforeDraw();


	// Editで生成したオブジェクトの描画

	if (m_EditFlg != NORMAL){
		EW.EditDraw();
	}

	//------------キャラ描画------------

	//キャラ
	CharacterBase::AllDraw();
	
	//プレイヤーで配置
	m_Put.Draw();

	//エフェクト
	BaseEffectObj::AllDraw();

	if (INPUT.KeyCheck('L')) {
		NAI.DebugWayDraw();
	}

	//==========================
	// あたり判定デバッグ表示
	//==========================
	if (m_Debug_DrawHit){
		ShMgr.m_bsAlpha.SetState();
		//YsDx.DS_ZSetting(false, false);
		m_ColEng.DebugDraw(0.7f);
		//YsDx.DS_ZSetting(true, true);
	}

	// 暗幕描画
	if (m_BlackCurtainPower > 0){
		YsDx.GetSprite().Draw(*YsDx.GetWhiteTex(), 0, 0, YsDx.GetRezoW(), YsDx.GetRezoH(), &YsVec4(0, 0, 0, m_BlackCurtainPower));
	}

	// 説明文字
	std::string text;
	text += "------- システム操作 -------\n";
	text += "P:ポーズ\n";
	text += "H:あたり判定デバッグ表示\n";
	text += "E: エディットモード\n";
	text += "M: マップクリエイト\n";
	text += "N: 通常に戻す\n";
	text += "RETURN: プレイヤーで配置\n";
	text += "\n";

	// 2D描画
	YsDx.GetSprite().Begin(true);
	{
		YsDx.GetSprite().DrawFont(text, 1, 1, &YsVec4(0, 0, 0, 1));
		YsDx.GetSprite().DrawFont(text, 0, 0, &YsVec4(1, 1, 1, 1));
	}
	YsDx.GetSprite().End();

}