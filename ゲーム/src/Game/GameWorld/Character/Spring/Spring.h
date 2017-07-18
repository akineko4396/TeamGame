
#pragma once

class Spring : public CharacterBase{
public:
	Spring(){ m_ID = CharaID::UNKOWN;
		m_KillFlg = false;
	}
	~Spring(){}

	void Init(YsMatrix &_m);
	virtual void Update()override;
	virtual void Draw()override;

	virtual void SetCamera()override{}	//‹ó‚Á‚Û

private:

	SPtr<YsGameModel>	m_pSm;
	//GameCamera		m_Cam;

	SPtr<HitObj_Mesh> m_hitBump;
};