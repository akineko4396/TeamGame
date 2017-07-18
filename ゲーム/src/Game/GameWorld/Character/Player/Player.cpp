
#include "main.h"

#include "Game\GameWorld\GameWorld.h"




YsVec3 Player::vTar;

// 攻撃を受けたときの処理
bool Player::OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep)
{
	v = atk.m_vBlow / 60;

	m_vMove = atk.m_vBlow;

	return true;
}

void Player::Init(YsMatrix &_m, SPtr<YsGameModel>& _gm){
	/*-------------------------------キャラ設定------------------------------------------------*/
	m_Mat = _m;
	m_Mo.SetModel(_gm);
	m_Mo.InitAnimator(m_Anim);
	m_Anim.ChangeAnime(0, true);

	/*------------------------------カメラ設定-------------------------------------------------*/
	m_Cam.Init(20, 180, 0, false, true);
	m_Cam.m_BaseMat.CreateMove(0, 1.6f, 0);	// ベースの位置は、頭くらいの位置
	m_Cam.m_LocalMat.CreateMove(0, 0, -5);	// 少し後ろに引いた位置
	m_Cam.m_LocalMat.RotateX(25);

	for (int i = 0; i < MemoNum; i++){
		m_MemoPos[i] = m_Mat.GetPos();
	}

	/*------------------------------当たり判定設定--------------------------------------------*/
	m_hitBump = std::make_shared<HitObj_Sphere>();
	// 詳細設定
	m_hitBump->m_Mass = 1.0f; // 質量(重さ)
	m_hitBump->m_AtkFilter = HitGroups::_0; // 判定グループ グループ0は「ぶつかり系」にしときます
	m_hitBump->m_DefFilter = HitGroups::_0; // 所属グループ グループ0は「ぶつかり系」にしときます
	m_hitBump->m_ShapeFilter = HitShapes::SPHERE | HitShapes::MESH; // 判定対象の形状

	/*------------------------------モーション初期設定---------------------------------------*/
	m_ActionState = std::make_shared<PlayerAS_Wait>();

	// キー入力コンポーネント生成
	SetInput(std::make_shared<KeyInputComponent>());

	// 地面判定Obj
	m_hitGround = std::make_shared<HitObj_Ray>();
}
void Player::Update(){
	//デバッグキー
	if (GetAsyncKeyState('Z')){
		m_Mat._41 = 0;
		m_Mat._42 = 4;
		m_Mat._43 = 0;
	}

	for (int i = MemoNum - 1; i > 0; i--){
		m_MemoPos[i] = m_MemoPos[i - 1];
	}
	m_MemoPos[0] = m_Mat.GetPos();
	//--------------------------------------------------------------->
	m_vMove -= v;
	if (v.z > 0){ if (m_vMove.z <= 0)m_vMove.z = 0; }
	if (v.z < 0){ if (m_vMove.z >= 0)m_vMove.z = 0; }

	if (v.x > 0){ if (m_vMove.x <= 0)m_vMove.x = 0; }
	if (v.x < 0){ if (m_vMove.x >= 0)m_vMove.x = 0; }
	//-------------------------------------------------------------->

	//入力コンポーネント処理
	if (m_Controller) {
		m_Keys = m_Controller->Update();
	}

	//アクションステートを更新
	if (m_ActionState) {
		auto p = m_ActionState;
		m_ActionState->Update(*this);
	}

	/*-----------------------------------当たり判定設定-------------------------------*/
	YsVec3 vPos = m_Mat.GetPos();
	vPos.y += 0.8f;	// 足元から少し上の位置

	m_hitBump->Set(YsToSPtr(this), vPos, 0.3f);
	GetStage()->m_ColEng.AddAtk(m_hitBump); // 判定する側として登録
	GetStage()->m_ColEng.AddDef(m_hitBump); // 判定される側としても登録

	/*--------------------------当たり判定中に呼ばれる------------------------------*/

	m_hitBump->m_OnHitStay = [this](const SPtr<BaseHitObj>& obj) {
		// 当たった奴等のループ
		for (auto& res : obj->m_HitResTbl){
			// 押し戻す
			float F = obj->CalcMassRatio(res.m_youHitObj->m_Mass);
			m_Mat.Move(res.m_vPush*F);

			if (res.m_youHitObj->m_wpTask.lock()){

			}
		}
	};
	/*---------------ヒットした最初の1回だけ呼ばれる------------------------------*/
	m_hitBump->m_OnHitEnter = [this](const SPtr<BaseHitObj>& obj) {
	};

	/*------------------ヒット判定が終わった最初の1回だけ呼ばれる----------------*/
	m_hitBump->m_OnHitExit = [this](const SPtr<BaseHitObj>& obj) {
	};


	/*---------------------------------地面判定-------------------------------------*/
	Ground();


	/*---------------------------------各種更新------------------------------------*/
	/*アニメ更新*/
	m_Anim.Animation(1.0);
	m_Mo.CalcBoneMatrix(&m_Mat);
	/*カメラ更新*/
	m_Cam.Update(m_Mat.GetPos());


	/*-------------------------------ハンマー更新--------------------------------------*/
	if (m_Hammer.expired() == false){
		m_Hammer.lock()->Update();
	}

	

}
void Player::Draw(){
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);


	if (m_Hammer.expired() == false){
		m_Hammer.lock()->Draw();
	}
}
void Player::SetCamera(){

	m_Cam.SetCamera(m_MemoPos[MemoNum - 1]);
}

void Player::Ground()
{

	/*--------------------------------------->
	地面との当たり判定関数
	<--------------------------------------*/
	YsVec3 vPos = m_Mat.GetPos();
	vPos.y += 1;

	/*--------------------コリジョンエンジンの基本設定---------------------*/
	m_hitGround->Set(YsToSPtr(this), vPos, vPos + YsVec3(0, -1.5f, 0));

	/*-------------------------------HitObj設定-----------------------------*/
	m_hitGround->m_AtkFilter = HitGroups::_0; // 判定グループ 0:ぶつかり
	m_hitGround->m_ShapeFilter = HitShapes::MESH; // メッシュのみ判定

	/*----------------------当たり判定時の処理-----------------------------*/
	m_hitGround->m_OnHitStay = [this](const SPtr<BaseHitObj>& obj)
	{
		// 当たった奴らの中で、一番近い距離を求める
		float dist = FLT_MAX;
		HitResData* pHitRes = nullptr; // レイが当たったやつ
		for (auto& res : obj->m_HitResTbl){
			if (dist > res.m_RayDist){
				pHitRes = &res;
				dist = res.m_RayDist;
			}
		}
		/*-----------------------------------空中時-------------------------------*/
		if (m_SkyFlag == 1){
			// めり込んだら
			if (dist < 1 && m_vMove.y <= 0){
				m_Mat._42 += 1 - dist; // 押し戻す
				m_vMove.y = 0; // Y 方向の力をリセットしとく
				m_SkyFlag = 0; // 地上に
			}
		}
		/*--------------------------------地上時----------------------------------*/
		else if (m_SkyFlag == 0){
			if (dist >= 1 + 0.3f || m_vMove.y > 0){				// 地面から0.3 以上離れたとき、空中とみなす。
				m_SkyFlag = 1; // 空中状態にする						// または、上方向に力が加わったときも無条件に空中とみなす。
			}

			else{/*-----------------地上にいる時-------------------*/
				m_Mat._42 += 1 - dist; // 押し戻す
				m_vMove.y = 0;
				//----------------------------------------------------
				// 地面のキャラの変化に同調して動かす
				//----------------------------------------------------
				// 地面キャラの変化量により、キャラの行列も変化させる
				if (pHitRes){ // 地面キャラが存在
					auto task = pHitRes->m_youHitObj->m_wpTask.lock();
					if (task){
						m_Mat *= task->m_ChangedMat; // 地面キャラの変化量行列を合成(動く床など)
						// ただしY 回転以外はしたくないので、Y 軸が上でZ 軸が水平になるように調整する
						YsVec3 vZ = m_Mat.GetZAxis();
						vZ.y = 0;
						vZ.Normalize();
						m_Mat.SetLookAt(vZ, YsVec3::Up);
					}
				}

			}
		}
		m_Mo.CalcBoneMatrix_SkipRoot(&m_Mat);
	};
	m_hitGround->m_Debug_Color.Set(1, 0, 0, 1); // デバッグ色



	GetStage()->m_ColEng.AddAtk(m_hitGround); // 判定する側として登録
	GetStage()->m_ColEng.AddDef(m_hitGround); // 判定する側として登録


}


//=====================================================
// 「待機」状態
//=====================================================
void PlayerAS_Wait::Update(Player &Player)
{
	// 方向キー
	if (Player.GetKeys() & GK_LUP || Player.GetKeys() & GK_LLEFT || Player.GetKeys() & GK_LRIGHT || Player.GetKeys() & GK_LDOWN) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("走り(武器あり)", 0, 10, true, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_Run> p = std::make_shared<PlayerAS_Run>();
		Player.m_ActionState = p;
		return;
	}
	// マウス左クリック
	if (Player.GetKeys() & GK_X) {
		//アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("攻撃", 0, 10, false, 1);
		// 行動切り替え
		SPtr<PlayerAS_Attack> p = std::make_shared<PlayerAS_Attack>();
		Player.m_ActionState = p;
		return;
	}
	// スペースキー
	if (Player.GetKeys() & GK_A) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("ジャンプ(上昇)", 0, 10, false, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_JumpUp> p = std::make_shared<PlayerAS_JumpUp>();
		Player.m_ActionState = p;
		return;
	}

	//とりあえず十字キー下
	if (Player.GetKeys() & GK_LDOWN) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("のけ反り", 0, 10, false, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_KnockBack> p = std::make_shared<PlayerAS_KnockBack>();
		Player.m_ActionState = p;
		return;
	}

	// 重力
	Player.m_vMove.y -= 0.01f;

	// 摩擦
	/*if (Player.m_SkyFlag == 0) {
	Player.m_vMove *= 0.9f;
	}*/
	// 力移動
	Player.m_Mat.Move(Player.m_vMove);

}

//=====================================================
// 「走り」状態
//=====================================================
void PlayerAS_Run::Update(Player& Player)
{
	// 移動キーを押してる？
	if (!(Player.GetKeys() & GK_LUP || Player.GetKeys() & GK_LLEFT || Player.GetKeys() & GK_LRIGHT || Player.GetKeys() & GK_LDOWN)) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("待機", 0, 10, true, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}

	// ジャンプ攻撃
	if (Player.GetKeys() & GK_X) {
		//アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("ジャンプ攻撃", 0, 2, false, 1);
		// 行動切り替え
		SPtr<PlayerAS_JumpAttack> p = std::make_shared<PlayerAS_JumpAttack>();
		Player.m_ActionState = p;
		return;
	}

	// 移動による視点補正
	float rotaAng = 0;
	/*if (Player.GetKeys() & GK_LLEFT) {
	rotaAng = -1.0f;
	}
	if (Player.GetKeys() & GK_LRIGHT) {
	rotaAng = 1.0f;
	}*/
	if (Player.GetKeys() & GK_LUP) {
		rotaAng *= 0.5f;
	}
	// カメラ回転
	Player.m_Cam.m_BaseMat.RotateY_Local(rotaAng);





	// キーによりキャラの向きを回転
	//YsVec3 vTar;
	if (Player.GetKeys() & GK_LRIGHT) {
		Player.vTar += Player.m_Cam.m_BaseMat.GetXAxis();
	}
	else if (Player.GetKeys() & GK_LLEFT) {
		Player.vTar -= Player.m_Cam.m_BaseMat.GetXAxis();
	}
	if (Player.GetKeys() & GK_LUP) {
		Player.vTar += Player.m_Cam.m_BaseMat.GetZAxis();
	}
	else if (Player.GetKeys() & GK_LDOWN) {
		Player.vTar -= Player.m_Cam.m_BaseMat.GetZAxis();
	}
	Player.vTar.y = 0;
	Player.vTar.Normalize();
	YsVec3 vZ = Player.GetMatrix().GetZAxis();
	vZ.Homing(Player.vTar, 10);
	Player.m_Mat.SetLookAt(vZ, YsVec3::Up);






	// アニメ速度を移動速度にする
	float walkSpeed = (float)Player.GetAnime().GetAnimeSpeed() * 0.3f;
	Player.m_Mat.Move_Local(0, 0, walkSpeed);
	// 重力
	Player.m_vMove.y -= 0.01f;
	// 摩擦
	/*if (Player.m_SkyFlag == 0) {
	Player.m_vMove *= 0.9f;
	}*/
	// 力移動
	Player.m_Mat.Move(Player.m_vMove);
}
//=====================================================
//	「攻撃」状態
//=====================================================
void PlayerAS_Attack::Update(Player& Player)
{
	if (!(Player.GetKeys() & GK_X) && Player.GetAnime().IsAnimationEnd()) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("待機", 0, 10, true, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}



	if (Player.m_Hammer.expired() == false){

		auto weapon = Player.m_Hammer.lock();

		// ダメージ通知書作成
		AttackParam atk;

		int time = 1;

		// ヒット時処理関数
		auto OnHitProc = [this, atk](const SPtr<BaseHitObj>& obj) {
			// ヒット時処理
			// 当たった奴ら全員をループ
			for (auto& res : obj->m_HitResTbl){
				// HITした相手取得
				SPtr<BaseGameObj> you = res.m_youHitObj->m_wpTask.lock();


				//-------------------------------
				// 相手にダメージ通知を行う
				//-------------------------------
				DamageReply rep;
				if (you->OnDamage(nullptr, atk, rep)){
				}
				else{

				}
			}
		};
		// 武器の攻撃判定発生
		weapon->RegisterAttack(
			time,			// 発生持続時間
			OnHitProc		// ヒット時処理関数
			);
	}

}
//=====================================================
//	「ジャンプ攻撃」状態
//=====================================================
void PlayerAS_JumpAttack::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("待機", 0, 10, true, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}

	if (Player.m_Hammer.expired() == false){

		auto weapon = Player.m_Hammer.lock();

		// ダメージ通知書作成
		AttackParam atk;

		int time = 1;

		// ヒット時処理関数
		auto OnHitProc = [this, atk](const SPtr<BaseHitObj>& obj) {
			// ヒット時処理
			// 当たった奴ら全員をループ
			for (auto& res : obj->m_HitResTbl){
				// HITした相手取得
				SPtr<BaseGameObj> you = res.m_youHitObj->m_wpTask.lock();


				//-------------------------------
				// 相手にダメージ通知を行う
				//-------------------------------
				DamageReply rep;
				if (you->OnDamage(nullptr, atk, rep)){
				}
				else{

				}
			}
		};
		// 武器の攻撃判定発生
		weapon->RegisterAttack(
			time,			// 発生持続時間
			OnHitProc		// ヒット時処理関数
			);
	}
}
//=====================================================
//	「ジャンプ(上昇)」状態
//=====================================================
void PlayerAS_JumpUp::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("ジャンプ(降下)", 0, 10, true, 0.01);
		// 行動切り替え
		SPtr<PlayerAS_JumpDown> p = std::make_shared<PlayerAS_JumpDown>();
		Player.m_ActionState = p;
		return;
	}


	//プレイヤー行列保存
	Player.m_Mat = Player.GetMatrix();

	// 力移動
	Player.m_Mat.Move(Player.m_vMove);

}
//=====================================================
//	「ジャンプ(降下)」状態
//=====================================================
void PlayerAS_JumpDown::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("待機", 0, 10, true, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}

	//プレイヤー行列保存
	Player.m_Mat = Player.GetMatrix();

	// 力移動
	Player.m_Mat.Move(Player.m_vMove);

}
//=====================================================
//	「のけ反り」状態
//=====================================================
void PlayerAS_KnockBack::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("待機", 0, 10, true, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}
}
//=====================================================
// 汎用行動
//=====================================================
void PlayerAS_Generic::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// アニメ切り替え
		Player.GetAnime().ChangeAnimeSmooth_FromName("待機", 0, 10, true, 0.5);
		// 行動切り替え
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}


	//プレイヤー行列保存
	Player.m_Mat = Player.GetMatrix();

	// 摩擦
	/*if (Player.m_SkyFlag == 0) {
	Player.m_vMove *= 0.9f;
	}*/
	// 力移動
	Player.m_Mat.Move(Player.m_vMove);
}