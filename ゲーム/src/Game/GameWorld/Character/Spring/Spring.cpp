#include "main.h"

#include "Game\GameWorld\GameWorld.h"


void Spring::Init(YsMatrix &_m)
{
	m_Mat = _m;
	m_pSm = YsDx.GetResStg()->LoadMesh("data/original/Spring/Spring.xed");
	m_Mo.SetModel(m_pSm);

	// �ݒ�
	m_hitBump = std::make_shared<HitObj_Mesh>();
	m_hitBump->m_Mass = 0; // �����Ȃ�

	// �ړ�
	m_Mo.CalcBoneMatrix(&m_Mat);

}
void Spring::Update()
{
	// �R���W�����ɓo�^
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Skin();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	GetStage()->m_ColEng.AddDef(m_hitBump); // ���肳��鑤
	GetStage()->m_ColEng.AddAtk(m_hitBump); // 






	// �q�b�g���̏���
	m_hitBump->m_OnHitEnter = [this](const SPtr<BaseHitObj>& obj)
	{
		//-------------------------------
		// �U���ڍ׃f�[�^�̐ݒ�
		//-------------------------------
		AttackParam atk;
		YsVec3 Vec;
		YsVec3 PosMemo(m_Mat._41, m_Mat._42, m_Mat._43);
		m_Mat._41 = 0;
		m_Mat._42 = 0;
		m_Mat._43 = 0;
		D3DXVec3TransformCoord(&Vec, &YsVec3(0, 0, 1), &m_Mat);
		Vec.Normalize();
		m_Mat._41 = PosMemo.x;
		m_Mat._42 = PosMemo.y;
		m_Mat._43 = PosMemo.z;

		atk.m_vBlow.y = 0.6f;
		atk.m_vBlow.x = Vec.x / 10;
		atk.m_vBlow.z = Vec.z / 10;


		// ���������z���̃��[�v
		for (auto& res : obj->m_HitResTbl){

			// HIT��������擾
			SPtr<BaseGameObj> you = res.m_youHitObj->m_wpTask.lock();

			//-------------------------------
			// ����Ƀ_���[�W�ʒm���s��
			//-------------------------------
			DamageReply rep;
			you->OnDamage(YsToSPtr(this), atk, rep);

		}
	};






	// �ړ�
	m_Mo.CalcBoneMatrix(&m_Mat);
}
void Spring::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}