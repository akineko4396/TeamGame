

#pragma once

// ���ʗpID
enum CharaID{

	UNKOWN = 0,
	PLAYER,
	ENEMY,
	GROUND,
	BOX,
	LBOX,
	SPRING,
	WATER,
	STAIRS

};


// ���b�V�������L�����̊�{�N���X
//
//
class CharacterBase : public BaseGameObj{
public:
	virtual ~CharacterBase(){}
	CharacterBase(){}

	virtual void Update();
	virtual void Draw();
	virtual void SetCamera(){}

	// ���X�g�Ǘ�

	static void AllUpdate();
	static void AllDraw();
	static void PushObject(SPtr<CharacterBase> _obj){ m_List.push_back(_obj); }
	static std::vector<SPtr<CharacterBase>> GetList(){ return m_List; }
	static void FullClear(){ m_List.clear(); }
	static void CheckKillFlg();

	// �Q�b�^�[

	int GetID(){ return m_ID; }
	int GetKillFlg(){ return m_KillFlg; }
	YsModelObject & GetModelObject(){ return m_Mo; }
	YsAnimator & GetAnime(){ return m_Anim; }
	YsMatrix GetMatrix(){ return m_Mat; }
	YsVec3 GetAngle(){ return m_Angle; }

	YsGameModel & GetGameModel(){ return *m_Mo.GetGameModel(); }
	int GetKeys() { return m_Keys; }
	
	// Template�N���X�Ŏg�p
	YsVec3 GetPos(){ return m_Mat.GetPos(); }
	YsVec3 GetScale(){ return m_Mat.GetScale(); }

	// �Z�b�^�[

	// ���̓R���g���[��(�R���|�[�l���g)���Z�b�g
	inline void SetInput(SPtr<GameInputComponent> p) {
		m_Controller = p;
	}

	void SetKillFlg(){ m_KillFlg = true; }

	//	�w�肳�ꂽ�N���X���C���X�^���X�����ă��X�g�Ɋi�[����
	template <class T>
	inline static SPtr<T> CreateCharaTask()
	{
		SPtr<T> add(new T);
		PushObject(add);
		return add;
	}

protected:

	static std::vector<SPtr<CharacterBase>> m_List;

	int m_ID;
	bool m_KillFlg=false;

	YsModelObject	m_Mo;
	YsAnimator		m_Anim;
	YsVec3 m_Angle;


	// ���݉�������Ă�L�[ ���̃L�[���ɂ��L�����𓮂���(Player��AI��)
	// �e�r�b�g(0�`31)���e�L�[�̏��
	int m_Keys = 0;

	// ���̓R���g���[��
	SPtr<GameInputComponent> m_Controller;
};

