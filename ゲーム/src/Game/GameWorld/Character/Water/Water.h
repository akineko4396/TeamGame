
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
	GameCamera		m_Cam;			// ���_�p�J����
	SPtr<YsGameModel>	m_pSm;		// ���b�V���ǂݍ���
	SPtr<HitObj_Mesh> m_hitBump;	// ���蔻��I�u�W�F
	SPtr<HitObj_Ray>	m_hitGround;// �n�ʔ��背�C
	SPtr<BaseWaterAS> m_ActionState;// ��ԑJ��


	void Ground();
	int	m_SkyFlag = 0;			// 0:�n�� 1:��
	bool damage_Once=false;
};

// ��ԑJ��

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

