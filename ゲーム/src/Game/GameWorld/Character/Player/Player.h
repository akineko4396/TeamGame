
#pragma once




#include "Game\GameWorld\Item\Weapon.h"




class BasePlayerAS;

class Player : public CharacterBase{
public:
	Player(){ m_ID = CharaID::PLAYER; }
	~Player(){}
	
	// ������ �Ȃ�
	void Init(YsMatrix &_m, SPtr<YsGameModel>& _gm);

	//++++++++++++++++++++++++++++++++++++
	// �I�[�o�[���C�h
	//++++++++++++++++++++++++++++++++++++

	virtual void Update()override;

	virtual void Draw()override;

	// �J�����̍X�V����
	//void CamUpdate(){ m_Cam.Update(); }
	
	// ���_�J����
	virtual void SetCamera()override;

	// �_���[�W
	bool OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep);


	//++++++++++++++++++++++++++++++++++++
	// ���[�U�[�֐�
	//++++++++++++++++++++++++++++++++++++

	// �n�ʃ��C���� �Ȃ�
	void Ground();

	// �s����Ԋ֌W

	int		m_SkyFlag = 0;			// 0:�n�� 1:��
	YsVec3	m_vMove;				// �ړ���

//private:

	//�J������x�点��
	static const int MemoNum = 3;
	YsVec3 m_MemoPos[MemoNum];

	GameCamera		m_Cam;
	SPtr<YsGameModel>	m_pSm;
	SPtr<HitObj_Sphere> m_hitBump;
	SPtr<HitObj_Ray>	m_hitGround;// �n�ʂԂ��蔻�背�C
	SPtr<BasePlayerAS> m_ActionState;

	// �����A�C�e��
	WPtr<BaseWeapon>m_Hammer;
	void SetWeapon(SPtr<BaseWeapon> weapon)
	{
		m_Hammer = weapon;
	}


	//�v���C���[�ɒǏ]������̂ɕK�v�ȃx�N�g��
	static YsVec3 vTar;
	static YsVec3 GetTar(){ return vTar; }

	//������ю��ɑ������ޒl������
	YsVec3 v;
};

//======================================
//Player�A�N�V������{�N���X
//======================================
class BasePlayerAS{
public:
	virtual ~BasePlayerAS(){}
	//������s�����
	virtual void Update(Player & Player){}
};

//======================================
//�u�ҋ@�v���
//======================================
class PlayerAS_Wait :public BasePlayerAS
{
public:
	~PlayerAS_Wait(){}
	//������s�����
	virtual void Update(Player & Player)override;
};
//======================================
//�u����v���
//======================================
class PlayerAS_Run :public BasePlayerAS
{
public:
	~PlayerAS_Run(){}
	//������s�����
	virtual void Update(Player& Player) override;
};
//======================================
//�u�U���v���
//======================================
class PlayerAS_Attack :public BasePlayerAS
{
public:
	//������s�����
	virtual void Update(Player& Player) override;
};

//======================================
//�u�W�����v�U���v���
//======================================
class PlayerAS_JumpAttack :public BasePlayerAS
{
public:
	//������s�����
	virtual void Update(Player& Player) override;
};

//======================================
//�u�W�����v(�㏸)�v���
//======================================
class PlayerAS_JumpUp :public BasePlayerAS
{
public:
	//������s�����
	virtual void Update(Player& Player) override;
};

//======================================
//�u�W�����v(�~��)�v���
//======================================
class PlayerAS_JumpDown :public BasePlayerAS
{
public:
	//������s�����
	virtual void Update(Player& Player) override;
};

//======================================
//�u�̂�����v���
//======================================
class PlayerAS_KnockBack :public BasePlayerAS
{
public:
	//������s�����
	virtual void Update(Player& Player) override;
};

//=====================================================
// �ėp�s��
// �A�j���[�V�������I��������u�����v�ɖ߂邾��
//=====================================================
class PlayerAS_Generic : public BasePlayerAS
{
public:
	// ������s�����
	virtual void Update(Player& Player) override;
};


