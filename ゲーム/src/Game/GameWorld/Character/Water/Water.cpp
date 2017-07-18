#include "main.h"

#include "Game\GameWorld\GameWorld.h"



bool Water::OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep)
{
	if (damage_Once == false){
		int rnd = APP.m_Rnd.GetInt(4) + 1;

		if (rnd == 1){
			m_ChangedMat.Move_Local(YsVec3::Left);
		}
		else if (rnd == 2){
			m_ChangedMat.Move_Local(YsVec3::Right);
		}
		else if (rnd == 3){
			m_ChangedMat.Move_Local(YsVec3::Front);
		}
		else if (rnd == 4){
			m_ChangedMat.Move_Local(YsVec3::Back);
		}
		else{
			m_ChangedMat.Move_Local(YsVec3::Zero);
		}
		damage_Once = true;
	}

	return true;
}
void Water::Init(YsMatrix &_m, CharaID _ID)
{
	//IDを設定
	m_ID = _ID;

	//移動行列
	m_Mat = _m;
	m_ChangedMat.CreateMove(0, 0, 0);

	//モデル
	m_pSm = YsDx.GetResStg()->LoadMesh("data/original/Water/water.x");
	m_Mo.SetModel(m_pSm);

	//当り判定設定
	m_hitBump = std::make_shared<HitObj_Mesh>();
	m_hitBump->m_Mass = 0; // 押せない

	// 移動
	m_Mat *= m_ChangedMat;
	m_Mo.CalcBoneMatrix(&m_Mat);

	// 地面判定Obj
	m_hitGround = std::make_shared<HitObj_Ray>();
}
void Water::Update()
{
	// コリジョンに登録
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	//GetStage()->m_ColEng.AddDef(m_hitBump); // 判定される側
	GetStage()->m_ColEng.AddAtk(m_hitBump); // 判定する側


	// ヒット時の処理
	m_hitBump->m_OnHitStay = [this](const SPtr<BaseHitObj>& obj)
	{
		//-------------------------------
		// 攻撃詳細データの設定
		//-------------------------------
		AttackParam atk;

		// 当たった奴等のループ
		for (auto& res : obj->m_HitResTbl){

			// HITした相手取得
			SPtr<BaseGameObj> you = res.m_youHitObj->m_wpTask.lock();

			//-------------------------------
			// 相手にダメージ通知を行う
			//-------------------------------
			DamageReply rep;
			you->OnDamage(YsToSPtr(this), atk, rep);

		}
	};




	//重力
	m_ChangedMat._42 -= 0.01f;

	//地面判定
	Ground();

	
	

	// 移動
	m_Mat *= m_ChangedMat;
	m_Mo.CalcBoneMatrix(&m_Mat);


	// 移動ベクトルを元に戻す
	float vecy = m_ChangedMat._42;
	YsMatrix m;
	m.CreateMove(0, vecy, 0);
	m_ChangedMat = m;
	damage_Once = false;
}
void Water::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}
void Water::SetCamera()
{

}
void Water::Ground()
{

	YsVec3 vPos = m_Mat.GetPos();
	vPos.y += 1;
	// 基本設定
	m_hitGround->Set(YsToSPtr(this), vPos, vPos + YsVec3(0, -1.5f, 0));
	// 詳細設定
	m_hitGround->m_AtkFilter = HitGroups::_0; // 判定グループ 0:ぶつかり
	m_hitGround->m_ShapeFilter = HitShapes::MESH; // メッシュのみ判定
	// ヒット時の処理
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
		// 空中時
		if (m_SkyFlag == 1){
			// めり込んでる
			if (dist < 1 && m_ChangedMat._42 <= 0){
				m_Mat._42 += 1 - dist; // 押し戻す
				m_ChangedMat._42 = 0; // Y 方向の力をリセットしとく
				m_SkyFlag = 0; // 地上に
			}
		}
		// 地上時
		else if (m_SkyFlag == 0){
			// 地面から0.3 以上離れたとき、空中とみなす。または、上方向に力が加わったときも無条件に空中とみなす。
			if (dist >= 1 + 0.3f || m_ChangedMat._42 > 0){
				m_SkyFlag = 1; // 空中に
			}
			// 地上にいます
			else{
				m_Mat._42 += 1 - dist; // 押し戻す
				m_ChangedMat._42 = 0;
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

	GetStage()->m_ColEng.AddDef(m_hitGround); // 判定される側として登録

}







void nagare::Update(Water & Water)
{

}