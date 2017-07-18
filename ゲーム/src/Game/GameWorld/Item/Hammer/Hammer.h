
#pragma once

class Weapon_Hammer : public BaseWeapon{
public:
	~Weapon_Hammer(){
		m_Bone.reset();
		m_wpChara.reset();
	}

	// �����ݒ�
	// owner : ������L����
	// bone : �Ǐ]����{�[���̃A�h���X
	// mat : �����s��
	void Init(SPtr<CharacterBase> owner, SPtr<YsModelObject::BoneNode> bone,
		const YsMatrix* mat);

	virtual void Update()override;
	virtual void Draw()override;

	void Weapon_Hammer::RegisterAttack(
		int time,
		std::function<void(const SPtr<BaseHitObj>& obj)> onHitProc
		);

private:

	YsModelObject					m_Mo;	// ���f���I�u�W�F�N�g

	// �w��{�[���ɒǏ]�����������́A�����Ƀ{�[���̃A�h���X������
	WPtr<YsModelObject::BoneNode>	m_Bone;


	SPtr<HitObj_Sphere> hitAtk;
};
