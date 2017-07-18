
#pragma once

class Box : public CharacterBase{
public:
	Box(){
		// クラス識別情報
		m_ID = CharaID::BOX;
	}
	~Box(){}

	// 初期化 など
	void Init(YsMatrix &_m);

	//+++++++++++++++++++++++++++++++++++++++++++
	//	オーバーライド
	//+++++++++++++++++++++++++++++++++++++++++++

	virtual void Update()override;
	
	virtual void Draw()override;

	virtual void SetCamera()override{}

	// ダメージを受けた時に呼びだされる
	bool OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep);


private:

	// メッシュ読み込み用
	SPtr<YsGameModel>	m_pSm;

	// 当り判定オブジェクト
	SPtr<HitObj_Mesh> m_hitBump;

};