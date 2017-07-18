#include "main.h"
#include "SampleScene.h"

//============================================================
// 物理エンジン用 衝突検知関数
//  body0とbody1が当たった時に呼ばれる
//============================================================
bool SampleBulletWorld::ContactProcessedCallback(btManifoldPoint& cp, YsBulletRigidBody* body0, YsBulletRigidBody* body1)
{
	Task_CharaBase* task0 = (Task_CharaBase*)body0->GetUserPointer();
	Task_CharaBase* task1 = (Task_CharaBase*)body1->GetUserPointer();

	
	if(task0){
		task0->OnHitPhysics(cp, body0, body1);
	}
	if(task1){
		task1->OnHitPhysics(cp, body1, body0);
	}
	return true;
}

//============================================================
//
// Task_Human
//
//============================================================
void Task_Human::Init(SPtr<YsGameModel> mesh, YsMatrix& mat)
{
	m_Mat = mat;

	m_Mo.SetModel(mesh);
	m_Mo.InitAnimator(m_Anim);
	m_Anim.ChangeAnime(0, true);

}

// 処理関数
void Task_Human::Update()
{
	// 適当にアニメ変更
	if (APP.m_Rnd.GetInt(0, 50) == 0) {
		int no = APP.m_Rnd.GetInt(0, 6);
		switch (no) {
		case 0:
			m_Anim.ChangeAnimeSmooth_FromName("歩き", 0, 20, true);
			break;
		case 1:
			m_Anim.ChangeAnimeSmooth_FromName("魔法２", 0, 20, true);
			break;
		case 2:
			m_Anim.ChangeAnimeSmooth_FromName("攻撃１", 0, 20, true);
			break;
		case 3:
			m_Anim.ChangeAnimeSmooth_FromName("攻撃２", 0, 20, true);
			break;
		case 4:
			m_Anim.ChangeAnimeSmooth_FromName("攻撃３", 0, 20, true);
			break;
		case 5:
			m_Anim.ChangeAnimeSmooth_FromName("ターン", 0, 10, true);
			break;
		default:
			m_Anim.ChangeAnimeSmooth_FromName("立ち", 0, 20, true);
			break;
		}
	}


	// スクリプト関数(ラムダ式)
	auto scriptProc = [&](YsAnimeKey_Script* scr)
	{
		m_Mo.CalcBoneMatrix(&m_Mat);

		Dw_Scroll(2, "[Script]%s", scr->Value.c_str());

		// 文字列をCSVとして解析
		YsCsv csv;
		csv.LoadFromText(scr->Value, false);
		// 行ループ
		for (UINT y = 0; y<csv.m_Tbl.size(); y++) {
			//==========================================
			// 爆発発生
			//==========================================
			if (csv.m_Tbl[y][0] == "Attack") {
				std::string& boneName = csv.m_Tbl[y][1];

				auto bone = m_Mo.SearchBone(boneName);
				if (bone) {
					SPtr<Task_Explode> add = SampleScene::s_pScene->m_EffectTaskMgr.CreateTask<Task_Explode>();
					add->Init(&SampleScene::s_pScene->m_tsExplode, bone->WorldMat.GetPos().x, bone->WorldMat.GetPos().y, bone->WorldMat.GetPos().z, 1);
				}
			}
			//==========================================
			// レーザー発生
			//==========================================
			else if (csv.m_Tbl[y][0] == "Magic") {
				std::string& boneName = csv.m_Tbl[y][1];	// ボーン名

				auto bone = m_Mo.SearchBone(boneName);
				if (bone) {

					// レーザー発射
					for (int i = 0; i<10; i++) {
						YsMatrix m = bone->WorldMat;
						m.RotateZ_Local(APP.m_Rnd.GetFloat(0, 360));
						m.RotateX_Local(APP.m_Rnd.GetFloat(-20, 20));
						//							m.RotateZ_Local(APP.m_Rnd.GetFloat(-20, 20));
						SPtr<Task_Laser> add = SampleScene::s_pScene->m_EffectTaskMgr.CreateTask<Task_Laser>();
						add->Init(&SampleScene::s_pScene->m_tsLaser, 0, 0, 0, &m);
					}
				}
			}
		}
	};

	// アニメーションを進め、全身のボーンのTransMatを更新
	// さらにスクリプトも実行(スクリプトキーの位置にきたらscriptProc()が実行される)
	m_Anim.AnimationAndScript(1.0, scriptProc, &m_Mat);
	// アニメーション実行のみ
//	m_Amin.Animation(1.0);

	// 全身のボーンのWorldMatを更新
//	m_Mo.CalcBoneMatrix(&m_Mat);
	m_Mo.CalcBoneMatrix_SkipRoot(&m_Mat);

}

// 描画関数
void Task_Human::Draw()
{
	// 描画
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
	// 輪郭描画
	ShMgr.m_Samp.DrawModelOutline(m_Mo, &YsVec4(0.2f, 0.2f, 0.2f, 1), &YsVec2(0.0005f, 0.015f));

}

//=======================================================
//
// Task_AirPlane
//
//=======================================================
void Task_AirPlane::Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsVec3& pos, float mass)
{
	m_Mo.SetModel(mesh);
	m_Mat.CreateMove(pos);

	// 物理 メッシュ形状作成
	SPtr<YsBulletShape_Mesh> shapeMesh = std::make_shared<YsBulletShape_Mesh>();
	shapeMesh->CreateFromMesh(*mesh->GetModelTbl()[0]->GetMesh().get());

	// 物理剛体作成・登録
	m_btBody.Create(world, shapeMesh, m_Mat, 0);

}

// 処理関数
void Task_AirPlane::Update()
{
	m_Mat.RotateY_Local(0.3f);

	// 全身のボーンのWorldMatを更新
	m_Mo.CalcBoneMatrix(&m_Mat);

	// キャラの姿勢を元に、物理ワールド内の剛体の行列を更新
	if(m_btBody.GetBody()){
		m_btBody.SetMatrix(m_Mat);
	}

}

// 描画関数
void Task_AirPlane::Draw()
{
	// 描画
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);

	// 剛体HIT時は、わかりやすいようにラインを表示
	if(m_bDebugHit){
		ShMgr.m_Samp.DrawBoxLine(m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMin(),
			m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMax(),
			&YsVec4(1, 1, 0, 1), &m_Mat);

		m_bDebugHit = false;
	}

}

void Task_AirPlane::OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest)
{
	m_bDebugHit = true;
}

//=======================================================
//
// Task_RigidBoxタスク
//
//=======================================================
void Task_RigidBox::Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsMatrix& m, YsVec3& halfsize, float mass)
{
	m_Mo.SetModel(mesh);

	m_Mat = m;

	// 物理形状作成
	SPtr<YsBulletShape_Box> btBox = std::make_shared<YsBulletShape_Box>();
	btBox->Create(
		halfsize	// 立方体のxyzハーフサイズ(半分のサイズ)
	);
	// 物理剛体作成・登録
	m_btBody.Create(
		world,		// 物理世界
		btBox,		// 形状
		m_Mat,		// 行列
		mass		// 質量。 0で不動物体
	);
	m_btBody.SetFriction(0.5f);	// 摩擦設定
	m_btBody.SetNonSleep();		// スリープ状態に入らないようにする
	m_btBody.SetUserPointer(this);	// このタスクをUserPointerに記憶

	m_Cnt = 900;

	m_ExplodeInterval = 0;
}

void Task_RigidBox::Update()
{
	// 行列取得
	if(m_btBody.GetBody()){
		m_OldMat = m_Mat;

		m_btBody.GetMatrix(m_Mat);	// 物理世界の行列を取得
	}

	// 
	m_Mo.CalcBoneMatrix(&m_Mat);

	// 一定時間で消える
	m_Cnt--;
	if(m_Cnt <= 0){
		TaskDelete();
	}

	if(m_ExplodeInterval > 0){
		m_ExplodeInterval--;
	}

}
// 描画関数
void Task_RigidBox::Draw()
{

	// 描画
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);

	// 剛体HIT時は、わかりやすいようにラインを表示
	if(m_bDebugHit){
		ShMgr.m_Samp.DrawBoxLine(m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMin(),
			m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMax(),
			&YsVec4(1, 1, 0, 1), &m_Mat);
		m_bDebugHit = false;
	}

}

void Task_RigidBox::OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest)
{
	m_bDebugHit = true;

	SPtr<SampleScene> ss = SPtr_dynamic_cast<SampleScene>(APP.m_NowScene);
	if(ss == nullptr)return;

	YsVec3 v = m_OldMat.GetPos() - m_Mat.GetPos();

	float speed = v.Length();

	if(speed > 0.2f && m_ExplodeInterval <= 0){
		SPtr<Task_Explode> add = ss->m_EffectTaskMgr.CreateTask<Task_Explode>();
		add->Init(&ss->m_tsExplode, cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y()+0.3f, cp.getPositionWorldOnB().z(), 3);


		m_ExplodeInterval = 2;
	}
}


//============================================================
//
// Task_Explode
//
//============================================================
void Task_Explode::Init(YsTextureSet* tex, float x, float y, float z, float size)
{
	m_BB.SetTex(tex);
	m_BB.SetVertex(-size / 2, -size / 2, size, size);
	m_BB.GetAnime().Init(8, 8, -1);		// UVアニメ設定

	m_Mat.CreateMove(x, y, z);

	m_vMove.x = APP.m_Rnd.GetFloat(-0.01f, 0.01f);
	m_vMove.y = APP.m_Rnd.GetFloat(-0.01f, 0.01f);
	m_vMove.z = APP.m_Rnd.GetFloat(-0.01f, 0.01f);

	// ポイントライト追加
	m_PointLight = ShMgr.m_LightMgr.AddPointLight();
	m_PointLight->SetData(m_Mat.GetPos(), YsVec4(1, 0.5f, 0.2f, 0) * 2, 20);
}

// 処理関数
void Task_Explode::Update()
{
	// 移動
	m_Mat.Move(m_vMove);

	// アニメーション進行
	m_BB.GetAnime().Animation(0.5f, true);

	if(m_BB.GetAnime().IsEnd()){
		TaskDelete();
	}

	// ポイントライト座標更新
	if(m_PointLight){
		m_PointLight->Position = m_Mat.GetPos();
	}

	// 20コマ目以降はライト色減衰
	if(m_BB.GetAnime().GetPos() >= 20){
		m_PointLight->Diffuse *= 0.9f;
	}

}

// 描画関数
void Task_Explode::Draw()
{
	if(m_BB.GetAnime().GetPos() < 20){
		ShMgr.m_bsAdd.SetState();
	}
	else{
		ShMgr.m_bsAlpha.SetState();
	}

	YsMatrix m = m_Mat;
	m.SetBillBoard(YsCamera::LastCam.mCam);

	ShMgr.m_Samp.DrawBillBoard(m_BB, &m);
}


//============================================================
//
// Task_Laser
//
//============================================================
void Task_Laser::Init(YsTextureSet* texset, float x, float y, float z, const YsMatrix* mat)
{
	m_Lz.SetTex(texset);
	m_Lz.Init(0.5f);

	if(mat){
		m_Mat = *mat;
	}
	else{
		m_Mat.CreateMove(x, y, z);
		m_Mat.RotateX_Local(APP.m_Rnd.GetFloat(0, 360));
		m_Mat.RotateZ_Local(APP.m_Rnd.GetFloat(0, 360));
	}

	m_fSpeed = 0.5f;

	m_Cnt = 120;

	m_Lz.AddPoint_Front(m_Mat.GetPos());

	m_fAngPow = APP.m_Rnd.GetFloat(5,8);

	// ポイントライト追加
	m_PointLight = ShMgr.m_LightMgr.AddPointLight();
	m_PointLight->SetData(m_Mat.GetPos(), YsVec4(0.2f, 0.2f, 1.0f, 0) * 3, 3);
}

// 処理関数
void Task_Laser::Update()
{
	// 方向へZ移動
	m_Mat.Move_Local(0, 0, m_fSpeed);

	// X軸回転
	m_Mat.RotateX_Local(m_fAngPow);



	m_Cnt--;
	if(m_Cnt > 0){
		// 現在の座標にポイント追加
		m_Lz.AddPoint_Front(m_Mat.GetPos());

		// ポイントの50個制限
		if(m_Lz.GetPointNum() > 50){
			m_Lz.DelPoint_Back();
		}
	}
	else{
		// レーザーのポイントの個数を減らしていく
		if(m_Lz.GetList().size() > 0){
			m_Lz.DelPoint_Back();
		}
		// 長さが０になったら消す
		else{
			TaskDelete();
		}
	}

	// ポイントライト座標更新
	m_PointLight->Position = m_Mat.GetPos();

}

// 描画関数
void Task_Laser::Draw()
{
	ShMgr.m_bsAdd.SetState();

	// レーザー描画
	ShMgr.m_Samp.DrawLaser(m_Lz, 0);
}