
#pragma once

class BaseWaterAS;

class Water : public CharacterBase{
public:
	~Water(){}
	Water(){ m_ID = CharaID::WATER; }

	void Init(YsMatrix &_m,CharaID _ID);
	virtual void Update()override;
	virtual void Draw()override;
	virtual void SetCamera()override;

	bool OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep);

private:
	GameCamera		m_Cam;			// 視点用カメラ
	SPtr<YsGameModel>	m_pSm;		// メッシュ読み込み
	SPtr<HitObj_Mesh> m_hitBump;	// 当り判定オブジェ
	SPtr<HitObj_Ray>	m_hitGround;// 地面判定レイ
	SPtr<BaseWaterAS> m_ActionState;// 状態遷移


	void Ground();
	int	m_SkyFlag = 0;			// 0:地上 1:空中
	bool damage_Once=false;
};

// 状態遷移

class BaseWaterAS{
public:
	virtual ~BaseWaterAS(){}
	virtual void Update(Water & Water)=0;
};

class nagare : public BaseWaterAS{
public:
	~nagare(){}
	virtual void Update(Water & Water)override;
};

