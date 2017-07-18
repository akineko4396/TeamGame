
#pragma once

class Stairs : public CharacterBase{
public:
	Stairs(){
		m_ID = CharaID::STAIRS;
		m_KillFlg = false;
	}
	~Stairs(){}

	void Init(YsMatrix &_m);
	virtual void Update()override;
	virtual void Draw()override;

	virtual void SetCamera()override{}	//‹ó‚Á‚Û

private:


	SPtr<YsGameModel>	m_pSm;
	//GameCamera		m_Cam;

	SPtr<HitObj_Mesh> m_hitBump;
};