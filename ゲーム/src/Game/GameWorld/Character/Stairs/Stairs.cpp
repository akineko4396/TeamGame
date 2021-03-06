#include "main.h"

#include "Game\GameWorld\GameWorld.h"


void Stairs::Init(YsMatrix &_m)
{
	m_Mat = _m;
	m_pSm = YsDx.GetResStg()->LoadMesh("data/original/Stairs/Stairs.xed");
	m_Mo.SetModel(m_pSm);

	// 設定
	m_hitBump = std::make_shared<HitObj_Mesh>();
	m_hitBump->m_Mass = 0; // 押せない

	// 移動
	m_Mo.CalcBoneMatrix(&m_Mat);

}
void Stairs::Update()
{
	// コリジョンに登録
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	GetStage()->m_ColEng.AddDef(m_hitBump); // 判定される側

	// 移動
	m_Mo.CalcBoneMatrix(&m_Mat);
}
void Stairs::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}