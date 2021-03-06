#include "main.h"

#include "Game\GameWorld\GameWorld.h"

void Map::Init(YsMatrix &_m, SPtr<YsGameModel>& _gm)
{
	m_Mat = _m;
	m_Mo.SetModel(_gm);

	m_hitBump = std::make_shared<HitObj_Mesh>();
	// 詳細設定
	m_hitBump->m_Mass = 0; // 質量(重さ)
}
void Map::Update()
{

	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	GetStage()->m_ColEng.AddDef(m_hitBump); // 判定される側としても登録


	m_Mo.CalcBoneMatrix(&m_Mat);
}
void Map::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}