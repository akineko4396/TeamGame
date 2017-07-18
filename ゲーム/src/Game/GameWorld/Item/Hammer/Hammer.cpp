#include "main.h"

#include "Game\GameWorld\GameWorld.h"


void Weapon_Hammer::Init(SPtr<CharacterBase> owner, SPtr<YsModelObject::BoneNode> bone, const YsMatrix* mat)
{
	// 持ち主記憶
	m_wpChara = owner;

	// 武器モデルを読み込みます　マネしないでね
	SPtr<YsGameModel> model = APP.m_ResStg.LoadMesh("data/original/Hammer/Hammer.x");

	// モデルオブジェクト初期設定
	m_Mo.SetModel(model);
	// 初期行列
	if (mat){
		m_Mat = *mat;
	}
	// 追従するボーン
	m_Bone = bone;


	hitAtk = std::make_shared<HitObj_Sphere>();
}

void Weapon_Hammer::Update()
{
	// ボーンに追従時は、そのボーンの行列を使用
	if (m_Bone.expired() == false){
		m_Mat = m_Bone.lock()->WorldMat;
	}

	m_Mo.CalcBoneMatrix(&m_Mat);


	// (1フレーム遅れて)消える
	if (m_wpChara.expired()){ m_KillFlg = true; }
}

void Weapon_Hammer::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}

void Weapon_Hammer::RegisterAttack(
	int time,
	std::function<void(const SPtr<BaseHitObj>& obj)> onHitProc
	)
{
	auto pChara = m_wpChara.lock();

	YsVec3 Pos = m_Mat.GetPos();
	YsVec3 v1;
	D3DXVec3TransformCoord(&v1, &D3DXVECTOR3(0, 0, 0.8f), &m_Mat);	// 持ちて部分からzに+0.8

	// 攻撃判定登録


	// 基本設定
	hitAtk->Set(pChara, v1, 0.2f);	// 半径0.2

	// 詳細設定
	hitAtk->m_AtkFilter = HitGroups::_5; // 攻撃系グループ
	hitAtk->m_ShapeFilter = HitShapes::SPHERE | HitShapes::MESH;
	// ヒット時に呼ばれる関数
	hitAtk->m_OnHitEnter = [this, onHitProc](const SPtr<BaseHitObj>& obj) {
		if (onHitProc){
			onHitProc(obj);
		}
	};
	hitAtk->m_Debug_Color.Set(0, 0, 1, 1);		// デバッグ色

	pChara->GetStage()->m_ColEng.AddAtk(hitAtk); // 判定する側として登録

}