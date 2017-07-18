
#pragma once

class PutObj;
class Put_State;

// �Ǘ��N���X

class Put{
public:
	Put(){}
	~Put(){}
	
	void SetPlayer(SPtr<CharacterBase> _player);
	void Update();
	void Draw();

	//�z�u�����X�g
	std::vector<SPtr<PutObj>>m_Obj;

	void Push(SPtr<PutObj> _obj){ m_Obj.push_back(_obj); }
	void PopBack(){ m_Obj.pop_back(); }
	void CheckKill();
	void Clear(){ m_Obj.clear(); }

	// �v���C���[�̃A�h���X
	WPtr<CharacterBase>m_wpPlayer;

};

// �ݒu���̃N���X

class PutObj{
public:
	static bool Setting;
	static CharaID ID;

	void Init(WPtr<CharacterBase> _player);
	void Update();
	void Draw();

	bool GetKillFlg(){ return m_KillFlg; }
	bool m_KillFlg = false;


	// ���b�V���p
	SPtr<YsGameModel>m_spMo;
	YsMatrix		m_Mat;
	YsModelObject	m_Mo;


	// �v���C���[�̃A�h���X
	WPtr<CharacterBase>m_wpPlayer;

	// ���
	SPtr<Put_State>m_State;

	// ������̃L�����A�h���X
	WPtr<CharacterBase>m_wpObj;

};

// �X�e�[�g

class Put_State
{
public:
	~Put_State(){}
	virtual void Update(PutObj & _Obj)=0;
	virtual void Draw(PutObj & _Obj) = 0;
};

class Put_Set : public Put_State
{
public:
	Put_Set(){ PutObj::Setting = true; }
	~Put_Set(){ PutObj::Setting = false; }
	virtual void Update(PutObj & _Obj)override;
	virtual void Draw(PutObj & _Obj)override;

	int one_frame = 0;

	//���C�֌W
	BOOL bHit;
	float fDist;
	YsVec3 StartPos;
	YsVec3 EndPos;
};

class Put_End : public Put_State
{
public:
	Put_End(){}
	~Put_End(){}
	virtual void Update(PutObj & _Obj)override;
	virtual void Draw(PutObj & _Obj)override;
};




