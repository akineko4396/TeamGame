#include "main.h"
#include "../TitleScene/TitleScene.h"
#include "GameScene.h"

#include "Game/GameWorld/GameWorld.h"

#include "Game\GameWorld\Stage\Stage1\Stage1.h"
#include "Game\GameWorld\Stage\Stage2\Stage2.h"


SPtr<Stage> GameScene::m_Stage;

//============================================================
// 更新処理
//============================================================
int GameScene::Update()
{

	// Escapeキー
	if(INPUT.KeyCheck_Enter(VK_ESCAPE)){
		// シーン切り替え
		SPtr<TitleScene> ts(new TitleScene());
		ts->Init();
		APP.m_NowScene = ts;

		//終了
		//APP.ExitGame();

		return 1;
	}




	// ゲームワールド処理
	m_Stage->Update();




	return 0;
}

//============================================================
// 描画処理
//============================================================
void GameScene::Draw()
{
	//==========================================
	// クリア
	//==========================================
	// バックバッファをクリアする。
	YsDx.GetBackBuffer()->ClearRT(YsVec4(0.3f, 0.3f, 0.3f, 1));
	// Zバッファのクリア
	YsDx.GetDepthStencil()->ClearDepth(true, true);

	// 半透明モードに
	ShMgr.m_bsAlpha.SetState();


	// とりあえずXYZ線だけ描画しとく
	ShMgr.m_Samp.DrawLine(YsVec3(0, 0, 0), YsVec3(1, 0, 0), &YsVec4(1, 0, 0, 1));
	ShMgr.m_Samp.DrawLine(YsVec3(0, 0, 0), YsVec3(0, 1, 0), &YsVec4(0, 1, 0, 1));
	ShMgr.m_Samp.DrawLine(YsVec3(0, 0, 0), YsVec3(0, 0, 1), &YsVec4(0, 0, 1, 1));


	// 描画などを書く



	// ゲームワールド描画
	m_Stage->Draw();





}

//============================================================
// データ解放
//============================================================
void GameScene::Release()
{
	Dw_Scroll(0,"ゲームシーン解放処理");

	m_Stage->Release();
	m_Stage=nullptr;
}

//============================================================
// 初期設定
//============================================================
void GameScene::Init()
{
	Dw_Scroll(0,"ゲームシーン初期化");

	//============================================================
	// データ読み込みなど
	//============================================================

	if (m_StageNo == 1){

		//GameWorld::CreateWorld();
		//GW.Init();

		m_Stage = std::make_shared<GameWorld>();
		m_Stage->Init();

	}
	else if (m_StageNo == 2){

		m_Stage = std::make_shared<Stage2>();
		m_Stage->Init();

	}
	else
	{
		m_Stage = std::make_shared<GameWorld>();
		m_Stage->Init();
	}

}


