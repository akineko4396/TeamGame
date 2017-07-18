#include "main.h"

#include "Game\GameWorld\GameWorld.h"


void Weapon_Hammer::Init(SPtr<CharacterBase> owner, SPtr<YsModelObject::BoneNode> bone, const YsMatrix* mat)
{
	// ������L��
	m_wpChara = owner;

	// ���탂�f����ǂݍ��݂܂��@�}�l���Ȃ��ł�
	SPtr<YsGameModel> model = APP.m_ResStg.LoadMesh("data/original/Hammer/Hammer.x");

	// ���f���I�u�W�F�N�g�����ݒ�
	m_Mo.SetModel(model);
	// �����s��
	if (mat){
		m_Mat = *mat;
	}
	// �Ǐ]����{�[��
	m_Bone = bone;


	hitAtk = std::make_shared<HitObj_Sphere>();
}

void Weapon_Hammer::Update()
{
	// �{�[���ɒǏ]���́A���̃{�[���̍s����g�p
	if (m_Bone.expired() == false){
		m_Mat = m_Bone.lock()->WorldMat;
	}

	m_Mo.CalcBoneMatrix(&m_Mat);


	// (1�t���[���x���)������
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
	D3DXVec3TransformCoord(&v1, &D3DXVECTOR3(0, 0, 0.8f), &m_Mat);	// �����ĕ�������z��+0.8

	// �U������o�^


	// ��{�ݒ�
	hitAtk->Set(pChara, v1, 0.2f);	// ���a0.2

	// �ڍאݒ�
	hitAtk->m_AtkFilter = HitGroups::_5; // �U���n�O���[�v
	hitAtk->m_ShapeFilter = HitShapes::SPHERE | HitShapes::MESH;
	// �q�b�g���ɌĂ΂��֐�
	hitAtk->m_OnHitEnter = [this, onHitProc](const SPtr<BaseHitObj>& obj) {
		if (onHitProc){
			onHitProc(obj);
		}
	};
	hitAtk->m_Debug_Color.Set(0, 0, 1, 1);		// �f�o�b�O�F

	pChara->GetStage()->m_ColEng.AddAtk(hitAtk); // ���肷�鑤�Ƃ��ēo�^

}