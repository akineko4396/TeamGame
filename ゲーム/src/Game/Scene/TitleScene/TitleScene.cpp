#include "main.h"
#include "TitleScene.h"
#include "../GameScene/GameScene.h"

//=========================================
// 更新処理
//=========================================
int TitleScene::Update()
{
	if(INPUT.KeyCheck_Enter('1')){


		int StageNo = 1;
		// シーン切り替え
		SPtr<GameScene> gs(new GameScene(StageNo));
		gs->Init();
		APP.m_NowScene = gs;

		return 1;
	}
	if (INPUT.KeyCheck_Enter('2')){


		int StageNo = 2;
		// シーン切り替え
		SPtr<GameScene> gs(new GameScene(StageNo));
		gs->Init();
		APP.m_NowScene = gs;

		return 1;
	}

	// Escapeキー
	if(INPUT.KeyCheck_Enter(VK_ESCAPE)){
		APP.ExitGame();
	}

	//==========================================
	// ライト情報更新
	//	※この情報を各シェーダ内で使用します
	//==========================================
	// 平行光源
	m_DirLight->SetData(
		YsVec3(0.5f, -1, -0.2f),		// ライト方向
		YsVec4(0.8f, 0.8f, 0.8f, 1)		// 基本色(Diffuse)
		);

	// 環境色(Ambient)
	ShMgr.m_LightMgr.m_AmbientLight.Set(0.2f, 0.2f, 0.2f);

	
	//=========================================================
	// カメラ操作
	//=========================================================
	m_Cam.Update();

	//=========================================================
	// キャラ処理
	//=========================================================

	return 0;
}

//=========================================
// 描画処理
//=========================================
void TitleScene::Draw()
{
	//==========================================
	// クリア
	//==========================================
	// バックバッファをクリアする。
	YsDx.GetBackBuffer()->ClearRT(YsVec4(0.3f, 0.3f, 0.8f, 1));
	// Zバッファのクリア
	YsDx.GetDepthStencil()->ClearDepth(true, true);

	// 半透明モードに
	ShMgr.m_bsAlpha.SetState();


	//==========================================
	// カメラ設定
	//==========================================
	m_Cam.SetCamera();

	//==========================================
	// 描画直前にシェーダのフレーム単位データを更新する
	//==========================================
	ShMgr.UpdateBeforeDraw();


	// 描画などを書く


	//==========================================
	// [2D]背景描画
	//==========================================
	YsDx.GetSprite().Begin();
	{
		YsDx.GetSprite().Draw(*m_texBack, 0, 0, m_texBack->GetInfo().Width, m_texBack->GetInfo().Height);
	}
	YsDx.GetSprite().End();

	//==========================================
	// [3D]モデル描画
	//==========================================


	// 説明文字
	std::string text;
	text = "[タイトルシーン]\n";
	text += "1:Stage1\n";
	text += "2:Stage2\n";
	text += "Alt+Enter:フルスクリーン切り替え\n";

	// 2D描画
	YsDx.GetSprite().Begin(true);
	{
		YsDx.GetSprite().DrawFont(text, 1, 1, &YsVec4(0, 0, 0, 1));
		YsDx.GetSprite().DrawFont(text, 0, 0, &YsVec4(1, 1, 1, 1));

	}
	YsDx.GetSprite().End();

}

//============================================================
// データ解放
//============================================================
void TitleScene::Release()
{
	// サウンド停止
	YsSndMgr.StopAll();


	Dw_Scroll(0, "タイトルシーン解放処理");
}

//============================================================
// 初期設定
//============================================================
void TitleScene::Init()
{
	Dw_Scroll(0, "タイトルシーン初期化");

	// 平行ライト作成
	m_DirLight = ShMgr.m_LightMgr.AddDirLight();

	// カメラデータ初期化
	m_Cam.Init(20, 130, -3);
	m_Cam.m_BaseMat.SetPos(0, 1.2f, 0);

	//--------------------------------------------------------
	// テクスチャ読み込み
	//--------------------------------------------------------
	m_texBack = APP.m_ResStg.LoadTexture("data/Texture/title_back.png");

	//--------------------------------------------------------
	//
	// モデル読み込み
	//  APP.m_ResStgの代わりに、YsDx.GetResStg()を使用してもいい(どっちも同じ)
	//
	//--------------------------------------------------------
	

}
