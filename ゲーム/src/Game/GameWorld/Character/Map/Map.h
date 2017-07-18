
#pragma once

//
// マップの土台(オブジェクト配置に対応できれば作れる)
//

class Map : public CharacterBase{
public:
	Map() { m_ID = CharaID::GROUND; }
	~Map(){}

	void Init(YsMatrix &_m , SPtr<YsGameModel>& _gm);
	virtual void Update()override;
	virtual void Draw()override;
	virtual void SetCamera()override{}	//空っぽ


	SPtr<YsGameModel>	m_pSm;
	SPtr<HitObj_Mesh>	m_hitBump;
};