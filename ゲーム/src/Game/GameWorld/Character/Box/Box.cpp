
#include "main.h"

#include "Game\GameWorld\GameWorld.h"

#include "../CharacterBase.h"

#include "Box.h"

#include "Game\GameWorld\Effect\Effect.h"

//�_���[�W���󂯂����̏���

bool Box::OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep){
	
	//
	//�����𐶐����ď�����
	//��attckPram����_���[�W��^����悤�ɕύX�����Ă�
	//

	SPtr<EF_Explode> add = std::make_shared<EF_Explode>();
	add->Init(m_Mat.GetPos().x, m_Mat.GetPos().y + 1.0f, m_Mat.GetPos().z, 3.0f);
	BaseEffectObj::PushObject(add);

	m_KillFlg = true;
	
	return true;
}

void Box::Init(YsMatrix &_m)
{
	m_Mat = _m;
	m_pSm = YsDx.GetResStg()->LoadMesh("data/original/stand_box.x");
	m_Mo.SetModel(m_pSm);

	// �ݒ�
	m_hitBump = std::make_shared<HitObj_Mesh>();
	m_hitBump->m_Mass = 0; // �����Ȃ�
}
void Box::Update()
{
	// �R���W�����ɓo�^
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	GetStage()->m_ColEng.AddDef(m_hitBump); // ���肳��鑤�Ƃ��ēo�^

	// �ړ�
	m_Mo.CalcBoneMatrix(&m_Mat);
}
void Box::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}