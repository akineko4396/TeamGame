#include "main.h"
#include "SampleScene.h"
#include "../TitleScene/TitleScene.h"

SampleScene *SampleScene::s_pScene = nullptr;

//=========================================
// 更新処理
//=========================================
int SampleScene::Update()
{
	// Escapeキー
	if(INPUT.KeyCheck_Enter(VK_ESCAPE)){
		// シーン切り替え
		SPtr<TitleScene> ts(new TitleScene());
		ts->Init();
		APP.m_NowScene = ts;

		return 1;
	}

	//=========================================================
	// Debug文字
	//=========================================================
	Dw_Static(2, "Task数:%d", m_TaskMgr.GetList().size());
	Dw_Static(3, "EffectTask数:%d", m_EffectTaskMgr.GetList().size());


	//==========================================
	// ライト情報更新
	//	※この情報を各シェーダ内で使用します
	//==========================================
	// 平行光源
	m_DirLight->SetData(
		YsVec3(0.5f, -1, 0.2f),		// ライト方向
		YsVec4(m_LightPower, m_LightPower, m_LightPower, 1)			// 基本色(Diffuse)
		);

	// 環境色(Ambient)
	ShMgr.m_LightMgr.m_AmbientLight.Set(0.1f, 0.1f, 0.1f);


	//=========================================================
	// カメラ操作
	//=========================================================
	m_Cam.Update();

	// 移動
	if (INPUT.KeyCheck('W')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetZAxis() * 0.03f);
	}
	if (INPUT.KeyCheck('S')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetZAxis() * -0.03f);
	}
	if (INPUT.KeyCheck('A')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetXAxis() * -0.03f);
	}
	if (INPUT.KeyCheck('D')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetXAxis() * 0.03f);
	}
//	m_Cam.m_BaseMat.Move(m_Cam.mCam.GetYAxis() * (float)INPUT.GetMouseWheel() * 0.002f);


	//=========================================================
	// ポーズ中でない時の処理
	//=========================================================
	if(m_PauseFlag == false){

		//=========================================================
		// マウス左ボタンダウン
		//=========================================================
		if(INPUT.MouseCheck_Enter(CInput::MOUSE_L))
		{
			// サウンド作成テスト
			if(m_sndSE){
				m_sndSE->CreateInstance()->Play();
			}

			// 座標
			YsMatrix mat = YsCamera::LastCam.mCam;

			// 大きさ
			mat.Scale_Local(APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1));

			// 質量
			float mass = (mat.GetXScale() + mat.GetYScale() + mat.GetZScale()) * 10;	// 大きさから適当に決める

			// BOX剛体キャラ作成
			SPtr<Task_RigidBox> add = m_TaskMgr.CreateTask<Task_RigidBox>();
			add->Init(
				m_Bullet,
				m_xBox,
				mat,	// カメラ座標から発射
				YsVec3(1.0f, 1.0f, 1.0f),				// BOXの大きさ
				mass);										// 質量
			// カメラ方向へ力を加える
			YsVec3 vZ = YsCamera::LastCam.mCam.GetZAxis();
			vZ *= 500;
			add->GetRigidBox().ApplyImpulse(vZ);
		}

		//==========================================
		// 一定時間でBOX剛体キャラタスク追加
		//==========================================
		if(APP.m_FrameCnt % 30 == 0)
//		if(0)
		{
			// ハーフサイズ
			YsVec3 halfSize(1, 1, 1);

			// 座標
			YsMatrix mat;
			mat.CreateMove(APP.m_Rnd.GetFloat(-10, 10), 10, 0);

			// 大きさ
			mat.Scale_Local(APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1));

			// 質量
			float mass = (mat.GetXScale() + mat.GetYScale() + mat.GetZScale()) * 10;	// 大きさから適当に決める

			// BOXキャラ作成
			SPtr<Task_RigidBox> add = m_TaskMgr.CreateTask<Task_RigidBox>();
			add->Init(m_Bullet, m_xBox, mat, halfSize, mass);
		}

		//=========================================================
		// BOX剛体に力を加える
		//=========================================================
		if(INPUT.KeyCheck_Enter('F')){
			// 全タスク
			auto it = m_TaskMgr.GetList().begin();
			while(it != m_TaskMgr.GetList().end()){
				// BOX剛体クラスのみ選び出す
				SPtr<Task_RigidBox> pRigidTask = SPtr_dynamic_cast<Task_RigidBox>((*it));
				if(pRigidTask){
					// 上向きに力を加える
					pRigidTask->GetRigidBox().ApplyImpulse(YsVec3(0, 300, 0));
				}

				++it;
			}
		}

		//=========================================================
		// 物理世界を進行させる
		//=========================================================
		Dw_TimeBegin("物理演算");
		m_Bullet.StepSimulation(1.0f / 60, 1);
		Dw_TimeEnd("物理演算", 8);



		//=========================================================
		// 全タスク処理実行
		//=========================================================
		m_TaskMgr.UpdateTask();
		m_EffectTaskMgr.UpdateTask();


		// スポットライト回転
		{
			YsMatrix mR;
			mR.CreateRotateY(3);
			m_SL->Direction.TransformNormal(mR);
		}

	}

	// 明るさ
	if(INPUT.KeyCheck(VK_UP)){
		m_LightPower += 0.02f;
		if(m_LightPower > 1)m_LightPower = 1;
	}
	if(INPUT.KeyCheck(VK_DOWN)){
		m_LightPower -= 0.02f;
		if(m_LightPower < 0)m_LightPower = 0;
	}

	if (INPUT.KeyCheck_Enter('1')) {
		m_ShowPointLoght = !m_ShowPointLoght;
	}

	// ポーズ
	if(INPUT.KeyCheck_Enter('P')){
		m_PauseFlag = !m_PauseFlag;
	}

	return 0;
}

//=========================================
// 描画処理
//=========================================
void SampleScene::Draw()
{
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



	//==========================================
	// クリア
	//==========================================
	// バックバッファをクリアする。
	YsDx.GetBackBuffer()->ClearRT(YsVec4(m_LightPower + 0.1f, m_LightPower + 0.1f, m_LightPower + 0.3f, 1));
	// Zバッファのクリア
	YsDx.GetDepthStencil()->ClearDepth(true, true);


	// 描画などを書く



	//==========================================
	// [2D]背景描画
	//==========================================





	//==========================================
	// [3D]モデル描画
	//==========================================

	// フォグ設定
	ShMgr.m_Samp.SetFog(true, &YsVec3(m_LightPower, m_LightPower, m_LightPower), &YsVec2(0, 500));

	// 全タスク描画実行
	//  (人間、飛行機、箱)
	m_TaskMgr.DrawTask();

	// ステージマップ描画
	ShMgr.m_Samp.DrawModel(m_moMap, &m_mMap);



	//　ポイントライト範囲デバッグ表示
	if(m_ShowPointLoght){
		for (auto& node : ShMgr.m_LightMgr.m_PointLightList) {
			SPtr<PointLight> PL = node.lock();
			if (PL) {
				YsMatrix m;
				m.CreateMove(PL->Position);
				ShMgr.m_Samp.DrawSphereLine(PL->Radius, &YsVec4(1,0,0,1), &m);

				ShMgr.m_Samp.DrawLine(PL->Position + YsVec3(0, -0.2f, 0), PL->Position + YsVec3(0, 0.2f, 0), &YsVec4(1, 1, 0, 1));
				ShMgr.m_Samp.DrawLine(PL->Position + YsVec3(-0.2f, 0, 0), PL->Position + YsVec3(0.2f, 0, 0), &YsVec4(1, 1, 0, 1));
				ShMgr.m_Samp.DrawLine(PL->Position + YsVec3(0, 0, -0.2f), PL->Position + YsVec3(0, 0, 0.2f), &YsVec4(1, 1, 0, 1));
			}
		}
	}

	// エフェクトタスク描画
	YsDx.DS_ZSetting(true, false);			// Zバッファ書き込みOFF
	YsDx.RS_CullMode(D3D11_CULL_NONE);		// 両面描画モード
		m_EffectTaskMgr.DrawTask();
	YsDx.DS_ZSetting(true, true);
	YsDx.RS_CullMode(D3D11_CULL_BACK);

	ShMgr.m_bsAlpha.SetState();

	// 説明文字
	std::string text;
	text = "[サンプルシーン]\n";
	text += "P:ポーズ(更新停止 描画のみ)\n";
	text += "F:物理剛体に力を加える\n";
	text += "左クリック:剛体発射 & サウンド再生\n";
	text += "上下キー:平行光源の明るさ変更\n";
	text += "\n";
	text += "ホイール:上下移動 ドラッグ:平行移動\n";
	text += "WASD:前後左右移動\n";
	text += "\n";
	text += "Escape:タイトルシーンへ戻る\n";
	text += "Alt+Enter:フルスクリーン切り替え\n";
	text += "1:ポイントライトのデバッグ表示\n";


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
void SampleScene::Release()
{
	// サウンド停止
	YsSndMgr.StopAll();

	// タスクマネージャ解放
	m_TaskMgr.Release();

	m_btMap.Release();
	m_Bullet.Release();

	Dw_Scroll(0, "サンプルシーン解放処理");
}
#include <thread>

//============================================================
// 初期設定
//============================================================
void SampleScene::Init()
{
	Dw_Scroll(0, "サンプルシーン初期化");

	// ライト
	m_DirLight = ShMgr.m_LightMgr.AddDirLight();

	s_pScene = this;

	//=================================================================================

	// ロード終了フラグ
	// ※アトミック変数なので、お互いのスレッドから操作・判定しても安全が保障される
	// ※その分、普通の変数に比べたら遅いよ
	std::atomic<bool> endFlag = false;

	// ロードスレッド開始
	// ※別スレッドで稼働
	// ※描画系は行ってはダメ
	std::thread loadThread(
	[&]()
	{

		// 物理世界初期化
		m_Bullet.Init();

		//============================================================
		// データ読み込みなど
		//============================================================


		// サウンド
		m_sndSE = YsDx.GetResStg()->LoadSound("data/Sound/se1.wav", false);


		//==============================================
		// モデル読み込み
		//==============================================
		// 簡易バージョン
		//  ※単純にYsGameModel型のメモリを確保し、モデルを読み込む
		//  ※リソース管理はしていないので、同じファイル名のモデルを読み込んだ場合、重複して読み込んでしまう
		//			m_pSm = std::make_shared<YsGameModel>();
		//			m_pSm->LoadMesh("data/model/Hackadoll_1/model.xed");

		// リソース倉庫使用バージョン(おすすめ)
		//  ※おなじファイル名のモデルを重複して読み込まないように管理している
		m_pSm = YsDx.GetResStg()->LoadMesh("data/model/Hackadoll_1/model.xed");
		m_pSm2 = YsDx.GetResStg()->LoadMesh("data/model/G-chan/model.xed");
		m_xF15 = YsDx.GetResStg()->LoadMesh("data/model/F15J/F15J.x");
		m_xSu27 = YsDx.GetResStg()->LoadMesh("data/model/Su-27/Su-27.x");
		m_xBox = YsDx.GetResStg()->LoadMesh("data/model/box/box.x");
		m_xMap = YsDx.GetResStg()->LoadMesh("data/model/map3/map.x");

		//==============================================
		// テクスチャ読み込み
		//==============================================
		m_texBack.LoadTexture("data/Texture/title_back.png");		// 単純に読み込み方法(同名のファイルでも関係なしに重複して読み込んでしまう)
		m_texBack2 = YsDx.GetResStg()->LoadTexture("data/Texture/title_back.png");	// リソース倉庫を使用して読み込み(同名のファイルは重複読み込みを防げる)
		m_tsLaser.LoadTextureSet("data/Texture/Effect/Laser3.png");
		m_tsExplode.LoadTextureSet("data/Texture/Effect/Explode.png");

		//==============================================
		// キャラ１の作成・設定
		{
			YsMatrix m;

			SPtr<Task_Human> add = m_TaskMgr.CreateTask<Task_Human>();
			add->Init(m_pSm, m);
		}
		// キャラ２の作成・設定
		{
			YsMatrix m;
			m.CreateMove(1, 0, 0);

			SPtr<Task_Human> add = m_TaskMgr.CreateTask<Task_Human>();
			add->Init(m_pSm2, m);
		}

		// キャラ３の作成・設定
		{
			YsMatrix m;
			m.CreateMove(0, 0, 2);
			m.RotateY_Local(135);

			SPtr<Task_Human> add = m_TaskMgr.CreateTask<Task_Human>();
			add->Init(m_pSm, m);
		}

		// ステージマップの設定
		m_moMap.SetModel(m_xMap);
		m_mMap.CreateMove(0, 0, 0);

		// メッシュ形状作成
		SPtr<YsBulletShape_Mesh> shapeMesh = std::make_shared<YsBulletShape_Mesh>();
		shapeMesh->CreateFromMesh(*m_xMap->GetModelTbl()[0]->GetMesh());
		// 剛体作成
		m_btMap.Create(m_Bullet, shapeMesh, m_mMap, 0);
		m_btMap.SetFriction(0.5f);
		m_btMap.SetKinematic(true);


		// 飛行機タスク追加
		{
			SPtr<Task_AirPlane> add = m_TaskMgr.CreateTask<Task_AirPlane>();
			add->Init(m_Bullet, m_xF15, YsVec3(0, 5, 0), 10);

		}
			{
				SPtr<Task_AirPlane> add = m_TaskMgr.CreateTask<Task_AirPlane>();
				add->Init(m_Bullet, m_xF15, YsVec3(15, 1, 0), 10);
			}
			{
				SPtr<Task_AirPlane> add = m_TaskMgr.CreateTask<Task_AirPlane>();
				add->Init(m_Bullet, m_xSu27, YsVec3(-15, 1, 0), 10);
			}

			// 
			endFlag = true;
	}
	);

	// アニメーション描画スレッド開始
	// ※別スレッドで稼働
	// ※できれば描画のみを行ってください(一応デバイスのCreate系はスレッドセーフらしいので、動作的には問題ないっぽい)
	std::thread drawThread(
		[&]() {

		int cnt = 0;

		while(1){
			// アルファ値
			float alpha = sinf(YsToRadian(cnt * 10));
			alpha = alpha*0.5f + 0.5f;

			// バックバッファをクリアする。
			YsDx.GetBackBuffer()->ClearRT(YsVec4(0.0f, 0.0f, 0.0f, 1));

			// 文字表示
			YsMatrix m;
			m.CreateScale(2, 2, 1);
			m.Move(200, 200, 1);
			YsDx.GetSprite().DrawFont("Loading...", m, &YsVec4(1, 1, 1, alpha));


			// バックバッファを画面に表示
			YsDx.Present(0, 0);

			// 
			Sleep(32);

			// ロードスレッドが終了してるなら終わり
			if(endFlag){
				break;
			}

			cnt++;
		}
	}
	);

	// 読み込みスレッドの終了まで待機する
	loadThread.join();
	// 描画スレッドの終了まで待機する
	drawThread.join();

	//=================================================================================

	// カメラデータ初期化
//	m_Cam.Init(20, 130, -5);
	m_Cam.Init(20, 0, 0, true);	// FPSモードON

	m_Cam.m_BaseMat.CreateMove(0,2,-4);
	m_Cam.m_BaseMat.RotateY(160);
//	m_Cam.m_BaseMat.RotateX_Local(90);

	// 
	m_PauseFlag = false;

	m_LightPower = 0.2f;

	// 固定ポイントライト
	m_PL[0] = ShMgr.m_LightMgr.AddPointLight();
	m_PL[0]->SetData(YsVec3(-1, 1, 0), YsVec4(1, 0.1f, 0.1f, 0) * 3, 3);

	m_PL[1] = ShMgr.m_LightMgr.AddPointLight();
	m_PL[1]->SetData(YsVec3(0, 1, 1), YsVec4(0.1f, 0.1f, 1, 0) * 2, 3);

	// スポットライト
	m_SL = ShMgr.m_LightMgr.AddSpotLight();
	m_SL->SetData(YsVec3(0, 0.3f, 3), YsVec3(0, 0, 1), YsVec4(1, 1, 1, 0) * 2, 100, 20, 25);
}
