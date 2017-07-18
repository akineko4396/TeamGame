#include "main.h"

#include "Game\GameWorld\GameWorld.h"


void Stairs::Init(YsMatrix &_m)
{
	m_Mat = _m;
	m_pSm = YsDx.GetResStg()->LoadMesh("data/original/Stairs/Stairs.xed");
	m_Mo.SetModel(m_pSm);

	// �ݒ�
	m_hitBump = std::make_shared<HitObj_Mesh>();
	m_hitBump->m_Mass = 0; // �����Ȃ�

	// �ړ�
	m_Mo.CalcBoneMatrix(&m_Mat);

}
void Stairs::Update()
{
	// �R���W�����ɓo�^
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	GetStage()->m_ColEng.AddDef(m_hitBump); // ���肳��鑤

	// �ړ�
	m_Mo.CalcBoneMatrix(&m_Mat);
}
void Stairs::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}