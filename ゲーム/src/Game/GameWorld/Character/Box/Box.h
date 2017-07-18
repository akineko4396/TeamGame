
#pragma once

class Box : public CharacterBase{
public:
	Box(){
		// �N���X���ʏ��
		m_ID = CharaID::BOX;
	}
	~Box(){}

	// ������ �Ȃ�
	void Init(YsMatrix &_m);

	//+++++++++++++++++++++++++++++++++++++++++++
	//	�I�[�o�[���C�h
	//+++++++++++++++++++++++++++++++++++++++++++

	virtual void Update()override;
	
	virtual void Draw()override;

	virtual void SetCamera()override{}

	// �_���[�W���󂯂����ɌĂт������
	bool OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep);


private:

	// ���b�V���ǂݍ��ݗp
	SPtr<YsGameModel>	m_pSm;

	// ���蔻��I�u�W�F�N�g
	SPtr<HitObj_Mesh> m_hitBump;

};