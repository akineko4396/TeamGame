
#pragma once

class Weapon_Hammer : public BaseWeapon{
public:
	~Weapon_Hammer(){
		m_Bone.reset();
		m_wpChara.reset();
	}

	// 初期設定
	// owner : 持ち主キャラ
	// bone : 追従するボーンのアドレス
	// mat : 初期行列
	void Init(SPtr<CharacterBase> owner, SPtr<YsModelObject::BoneNode> bone,
		const YsMatrix* mat);

	virtual void Update()override;
	virtual void Draw()override;

	void Weapon_Hammer::RegisterAttack(
		int time,
		std::function<void(const SPtr<BaseHitObj>& obj)> onHitProc
		);

private:

	YsModelObject					m_Mo;	// モデルオブジェクト

	// 指定ボーンに追従させたい時は、ここにボーンのアドレスを入れる
	WPtr<YsModelObject::BoneNode>	m_Bone;


	SPtr<HitObj_Sphere> hitAtk;
};
