//============================================================
//
//
// �T���v���V�[���Ŏg�p���Ă����s�@�┠��G�t�F�N�g�Ȃǂ�
// �I�u�W�F�N�g�N���X���܂Ƃ߂ď����Ă܂�
//
//
//============================================================
#ifndef __Sample_Data_h__
#define __Sample_Data_h__

//============================================================
// �����G���W�����[���h
//
//============================================================
class SampleBulletWorld : public YsBulletWorld {
public:
	// ���̓��m���Փ˂����Ƃ��A���ꂪ�Ă΂��
	virtual bool ContactProcessedCallback(btManifoldPoint& cp, YsBulletRigidBody* body0, YsBulletRigidBody* body1) override;
};


//============================================================
// �L������{�^�X�N
//============================================================
class Task_CharaBase : public BaseTask {
public:
	// ���̃q�b�g���ɌĂ΂��֐�
	virtual void OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest) {}

protected:
	Task_CharaBase(){}
};

//============================================================
// �l�ԃL�����^�X�N
//============================================================
class Task_Human : public Task_CharaBase {
private:
	YsModelObject		m_Mo;			// ���f������I�u�W�F�N�g
	YsAnimator			m_Anim;			// �A�j���[�V�����Ǘ�

	YsMatrix			m_Mat;			// �s��

public:
	// �����ݒ�
	void Init(SPtr<YsGameModel> mesh, YsMatrix& mat);
	// �����֐�
	virtual void Update() override;
	// �`��֐�
	virtual void Draw() override;
};

//============================================================
// ��s�@�L�����^�X�N
//============================================================
class Task_AirPlane : public Task_CharaBase {
private:
	YsBulletRigidBody		m_btBody;	// ���� ����

	YsModelObject		m_Mo;			// ���f������I�u�W�F�N�g
	YsMatrix			m_Mat;			// �s��

	bool				m_bDebugHit = false;	// ���̃q�b�g�t���O(�f�o�b�O�\���p)

public:

	Task_AirPlane()
	{
	}

	// �����ݒ�
	void Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsVec3& pos, float mass);
	// �����֐�
	virtual void Update() override;
	// �`��֐�
	virtual void Draw() override;
	// �������̓��m���q�b�g������
	virtual void OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest) override;
};

//============================================================
// BOX���̃L�����^�X�N�N���X
//============================================================
class Task_RigidBox : public Task_CharaBase {
private:
	YsModelObject	m_Mo;			// ���f������I�u�W�F�N�g
	YsMatrix		m_Mat;			// �s��
	int				m_Cnt;			// �����J�E���g

	YsBulletRigidBody	m_btBody;	// ���� ����

	bool			m_bDebugHit;	// ���̃q�b�g�t���O(�f�o�b�O�\���p)


	YsMatrix		m_OldMat;		// �O��̍s��

	int				m_ExplodeInterval;

public:
	YsBulletRigidBody& GetRigidBox(){ return m_btBody; }

public:
	Task_RigidBox() : m_bDebugHit(false){
	}

	~Task_RigidBox(){
		// ���̍폜(�ʂɌĂ΂Ȃ��Ă��f�X�g���N�^�Ŏ����ŌĂ΂�܂�)
//		m_RigidBox.Release();
	}

	// �����ݒ�
	void Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsMatrix& m, YsVec3& halfsize, float mass);
	// �����֐�
	virtual void Update() override;
	// �`��֐�
	virtual void Draw() override;


	// �������̓��m���q�b�g������
	virtual void OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest) override;
};

//============================================================
// �����G�t�F�N�g
//============================================================
class Task_Explode : public BaseTask {
public:
	void Init(YsTextureSet* tex, float x, float y, float z, float size);
	// �����֐�
	virtual void Update() override;
	// �`��֐�
	virtual void Draw() override;

private:

	YsBillBoard		m_BB;
	YsMatrix		m_Mat;
	YsVec3			m_vMove;

	SPtr<PointLight>	m_PointLight;	// �|�C���g���C�g�f�[�^
};


//============================================================
// ���[�U�[�G�t�F�N�g
//============================================================
class Task_Laser : public BaseTask {
public:

	void Init(YsTextureSet* texset, float x, float y, float z, const YsMatrix* mat);

	// �����֐�
	virtual void Update() override;

	// �`��֐�
	virtual void Draw() override;

private:

	YsLaser		m_Lz;		// �Ȑ����[�U�[�N���X
	YsMatrix	m_Mat;		// �s��

	float		m_fSpeed;	// �ړ����x
	int			m_Cnt;		// �����J�E���g

	float		m_fAngPow;	// ��]���x

	SPtr<PointLight>	m_PointLight;	// �|�C���g���C�g�f�[�^

};

#endif
