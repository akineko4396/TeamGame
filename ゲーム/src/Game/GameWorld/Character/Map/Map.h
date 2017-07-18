
#pragma once

//
// �}�b�v�̓y��(�I�u�W�F�N�g�z�u�ɑΉ��ł���΍���)
//

class Map : public CharacterBase{
public:
	Map() { m_ID = CharaID::GROUND; }
	~Map(){}

	void Init(YsMatrix &_m , SPtr<YsGameModel>& _gm);
	virtual void Update()override;
	virtual void Draw()override;
	virtual void SetCamera()override{}	//�����


	SPtr<YsGameModel>	m_pSm;
	SPtr<HitObj_Mesh>	m_hitBump;
};