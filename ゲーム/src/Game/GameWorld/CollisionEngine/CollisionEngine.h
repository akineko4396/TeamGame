
#pragma once




/*ViewFrustum==============================

������N���X

��������쐬����

=========================================*/
class ViewFrustum {

public:

	//������쐬
	void Create(YsMatrix _Proj, YsMatrix _View, YsMatrix _Cam);

	//�J��������̖@����Ԃ�
	inline YsVec3 GetNormal(int _i) { return m_vN[_i]; }

	//�ۑ����Ă���J�����̍��W��Ԃ�
	inline YsVec3 GetCamPos() { return m_CamPos; }

private:

	//�J�����̍��W�i�[�p
	YsVec3	m_CamPos;

	//	�J��������@��
	YsVec3	m_vN[4];

	//�\����
	struct CLONEVERTEX {
		YsVec3 Pos;
		YsVec3 Normal;
		YsVec2 Tex;
	};

public:

	//--------------------------
	//	�V���O���g���p�^�[������
	//--------------------------
	static ViewFrustum& GetInstance()
	{
		static ViewFrustum Instance;
		return Instance;
	}

};

// �ȒP�ɃA�N�Z�X�ł���悤�ɂ��邽�߂̃}�N��
#define VF	ViewFrustum::GetInstance()

/*===============

template�֐�

===============*/

/*<template>CheckViewFrustum-------------------------------->

������ƃL�������b�V���̃X�t�B�A�Ƃ̔���

_MyData		:	���g�̃f�[�^

return true	:	�������
return false:	������O

<-----------------------------------------------------*/

//template <class T>
//inline BOOL CheckViewFrustum(SPtr<T>& _MyData, SPtr<YsMesh> _Mesh)

class CharacterBase;
BOOL CheckViewFrustum(SPtr<CharacterBase> _MyData, SPtr<YsMesh> _Mesh);














//============================================
// �����蔻�� �萔
//============================================

// ����O���[�v�t�B���^�p
namespace HitGroups {
	enum{
		_0 = 0x00000001,
		_1 = 0x00000002,
		_2 = 0x00000004,
		_3 = 0x00000008,
		_4 = 0x00000010,
		_5 = 0x00000020,
		_6 = 0x00000040,
		_7 = 0x00000080,
		_8 = 0x00000100,
		_9 = 0x00000200,
		_10 = 0x00000400,
		_11 = 0x00000800,
		_12 = 0x00001000,
		_13 = 0x00002000,
		_14 = 0x00004000,
		_15 = 0x00008000,
		// ���Ƃ͏ȗ��B�ő�GP_31�܂ō���
	};
}

// ����`��t�B���^�p
namespace HitShapes {
	enum {
		SPHERE = 0x00000001,	// ��
		RAY = 0x00000002,	// ���C
		MESH = 0x00000004,	// ���b�V��
	};
}

// �q�b�g��ԃt���O
namespace HitStates {
	enum {
		ENTER = 0x00000001,	// HIT����1���
		STAY = 0x00000002,	// HIT��������
		EXIT = 0x00000004,	// HIT���Ȃ��Ȃ���1���
	};
}

class BaseGameObj;
class BaseHitObj;

//============================================
// �q�b�g���ʃf�[�^�\����
//  �����蔻����s�������ʂ̃f�[�^
//============================================
struct HitResData {
	BaseHitObj* m_youHitObj = nullptr;	// �����HitObj

	YsVec3		m_vHitPos;			// �q�b�g�ʒu

	YsVec3		m_vPush;			// (���A���b�V���p)������Ă���x�N�g�� �Ԃ���n�̏����Ɏg�p

	float		m_RayDist = 0;		// (���C�p)������������
	int			m_FaceIdx = 0;		// (���C�p)���肪���b�V���̎��̂݁A�ʔԍ�������
};


//============================================
// ����OBJ��{�N���X
//============================================
class BaseHitObj {
public:
	// �� ���₷���d���őS��public�ɂ��Ă܂��̂ŁA�e���ŃJ�v�Z�������Ă��������B

	//======================================================
	// ����f�[�^
	//======================================================
	// ����t�B���^�֌W
	int					m_AtkFilter = 0xFFFFFFFF;	// ���肷�鑤�̎��̃t�B���^		HitGroups���g�p
	int					m_DefFilter = 0xFFFFFFFF;	// ���肳��鑤�̎��̃t�B���^�@	HitGroups���g�p

	// ����`��t�B���^�֌W
	int					m_ShapeFilter = 0xFFFFFFFF;	// ���肷�鑤�̎��A�ǂ̌`��Ɣ��肷�邩�̃t�B���^	HitShapes���g�p
	int					m_Shape = 0;				// �����̌`��(�����_�E���L���X�g�p)�@HitShapes���g�p

	int					m_HitState = 0;				// �q�b�g��ԃt���O

	WPtr<BaseGameObj>	m_wpTask;					// �Ώۂ̃L����(�^�X�N)�ւ̃A�h���X�B�����L�����͔���𖳎�����p�r�Ŏg�p

	// �ėp���O�`�F�b�N�֐�
	std::function<bool(BaseHitObj*)>	m_BroadPhaseProc;

	// �q�b�g���ʃ��X�g �����蔻�茋�ʂ��i�[����ꏊ
	std::vector<HitResData>				m_HitResTbl;

	//======================================================
	// ����ʒm�֐�
	//======================================================
	// HIT����1��ڂ̂݌Ă΂��
	std::function<void(const SPtr<BaseHitObj>&)>	m_OnHitEnter;
	// HIT���Ă�ԌĂ΂��
	std::function<void(const SPtr<BaseHitObj>&)>	m_OnHitStay;
	// HIT���Ȃ��Ȃ���1��ڂ̂݌Ă΂��
	std::function<void(const SPtr<BaseHitObj>&)>	m_OnHitExit;


	//======================================================
	// ���̑��f�[�^ �D���ɂ��Ă��������B
	//======================================================
	float				m_Mass = 0;			// �Ԃ��莞�Ɏg�p ����0�ŌŒ蕨
	UINT				m_ID = 0;			// ���ʗpID



	//======================================================
	// �����蔻��֐�
	//  obj			: ����̔���I�u�W�F�N�g
	//  resData1	: ���茋�ʂ�����ϐ��@������
	//  resData2	: ���茋�ʂ�����ϐ��@���葤
	//======================================================
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) = 0;

	// ���ʔ�v�Z m_Mass��mass���r���A���̔䗦��Ԃ��B
	float CalcMassRatio(float mass);

	//======================================================
	// �f�o�b�O�\���p Release���ɂ͂���Ȃ��ł�
	//======================================================
	// �f�o�b�O�`��
	virtual void DebugDraw(float alpha){}
	YsVec4		m_Debug_Color = YsVec4(1, 1, 1, 1);

	// 
	virtual ~BaseHitObj(){
	}
};

//============================================
// �������蔻��OBJ
//============================================
class HitObj_Sphere : public BaseHitObj {
public:
	// �f�[�^
	YsVec3		m_vPos;		// ���W
	float		m_Rad;		// ���a

public:
	HitObj_Sphere(){
		m_Shape = HitShapes::SPHERE;
	}

	// �����蔻��v�Z�֐�
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) override;

	// �f�o�b�O�`��
	virtual void DebugDraw(float alpha) override;

	// �ݒ�
	//  task	�c ���̔���f�[�^�̎�����L����
	//  pos		�c ���W
	//  rad		�c ���a
	void Set(SPtr<BaseGameObj> task, const YsVec3& pos, float rad){
		m_wpTask = task;
		m_vPos = pos;
		m_Rad = rad;
	}

};

//============================================
// ���C�����蔻��OBJ
//============================================
class HitObj_Ray : public BaseHitObj {
public:
	// �f�[�^ Start�`End�̍��W�ԂŃ��C���肷�銴��
	YsVec3	m_vPos1;	// ���C���W�P(Start)
	YsVec3	m_vPos2;	// ���C���W�Q(End)
	YsVec3	m_vDir;		// �����@m_vPos2 - m_vPos1������
	float	m_RayLen;	// ���C�̒���

public:
	HitObj_Ray(){
		m_Shape = HitShapes::RAY;
	}

	// �����蔻��v�Z�֐�
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) override;

	// �f�o�b�O�`��
	virtual void DebugDraw(float alpha) override;

	// �ݒ�
	//  task	�c ���̔���f�[�^�̎�����L����
	//  pos1	�c ���C�J�n���W
	//  pos2	�c ���C�I�����W
	void Set(SPtr<BaseGameObj> task, const YsVec3& pos1, const YsVec3& pos2){
		m_wpTask = task;
		m_vPos1 = pos1;
		m_vPos2 = pos2;
		m_vDir = pos2 - pos1;
		m_RayLen = m_vDir.Length();
		m_vDir.Normalize();
	}

};

//============================================
// ���b�V�������蔻��OBJ
//============================================
class HitObj_Mesh : public BaseHitObj {
public:
	SPtr<YsMesh>	m_pMesh;		// ���肷�郁�b�V���̃A�h���X

	YsMatrix		m_Mat;			// �ϊ��s��

public:
	HitObj_Mesh(){
		m_Shape = HitShapes::MESH;
	}

	// �����蔻��v�Z�֐�
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) override;

	// �f�o�b�O�`��
	virtual void DebugDraw(float alpha) override;

	// �ݒ�
	//  task	�c ���̔���f�[�^�̎�����L����
	//  pMesh	�c ���b�V���f�[�^
	//  mat		�c �s��
	void Set(SPtr<BaseGameObj> obj,
		SPtr<YsMesh> pMesh,
		const YsMatrix& mat
		){
		m_wpTask = obj;
		m_pMesh = pMesh;

		m_Mat = mat;
	}

};


//============================================
// �����蔻��}�l�[�W��
//============================================
class CollisionEngine{
public:
	// 
	~CollisionEngine(){
		ClearList();
	}

	// ���肷�鑤�Ƃ��ēo�^
	void AddAtk(const SPtr<BaseHitObj>& obj){
		m_AtkList.push_back(obj);
	}
	// ���肳��鑤�Ƃ��ēo�^
	void AddDef(const SPtr<BaseHitObj>& obj){
		m_DefList.push_back(obj);
	}

	// atkObj vs �Sm_DefList�̔�����s��(�ʒm�͂��Ȃ�)
	void Test(BaseHitObj* atkObj);

	// �����蔻������s���A�eObj�֒ʒm����
	// �Sm_AtkList vs �Sm_DefList�̔�����s���A�Sm_AtkList�֒ʒm����
	void Run();


	// ���X�g���N���A
	void ClearList(){
		m_AtkList.clear();
		m_DefList.clear();
	}

	// �f�o�b�O�`��
	void DebugDraw(float alpha);

private:

	// HitObj���X�g
	//  �������ׁ̈A���肷�鑤�Ƃ���鑤�̂Q�̃��X�g�ŊǗ�
	std::vector<SPtr<BaseHitObj>>	m_AtkList;	// ���肷�鑤
	std::vector<SPtr<BaseHitObj>>	m_DefList;	// ���肳��鑤

};
