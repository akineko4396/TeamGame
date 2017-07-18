#pragma once

//--------------------------------------------
//
// EditMode�N���X ver.1.0
// 
// �J���Ҍ����G�f�B�^
// �G�f�B�b�g���[�h
//
//
// �y�Ăяo�����ԁz
// CreateWorld
// Init
//
// CamUpdate
// Update
// SetCamera
// Draw
//
// DeleteWorld
//
//--------------------------------------------

//--------------------------------------------
// 
// EditObject
// 
// �E�I�u�W�F�N�g��z�u������ҏW����N���X
// �E�ҏW�ς݂ɂȂ�����{�҂̃L�����̃C���X�^���X��������
//		�ҏW���EditMode�N���X�̕ҏW�ς݃��X�g�Ɏc��
// �E������L�����̃C���X�^���X�����ł����玩�g��������
//
//--------------------------------------------

// ���L�����N���X
class LBox : public CharacterBase{
public:

	~LBox(){}

	//��{�����o�֐�

	void Init(YsMatrix &_m);
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetCamera()override{}

private:
	SPtr<YsGameModel>	m_pSm;

	SPtr<HitObj_Mesh> m_hitBump;
};


class EditObject;
class Edit_State;
class PickUp;





// �S�̂̊Ǘ��N���X
class EditMode{
public:

	~EditMode(){
		Release();
	}

	//MapEdit

	void MapInit();
	void MapUpdate();
	void MapDraw();

	//Edit

	static bool Edit;
	void EditInit();
	void EditUpdate();
	void EditDraw();

	//���
	void Release();


	void CamUpdate(){ m_Cam->Update(); }

	void SetCamera(){ m_Cam->SetCamera(); }



	
	//���L�J����
	SPtr<GameCamera> m_Cam;

	//�ݒu���������X�g
	static std::vector<SPtr<EditObject>>m_eList;
	void Push(SPtr<EditObject> _obj){ m_eList.push_back(_obj); }
	void PopBack(){ m_eList.pop_back(); }
	void CheckKill();

	//�����\��
	PickUp * m_PickUp;

	//���ݕҏW���̃L����
	WPtr<EditObject>m_wpNow;



	//---------------------
	//�L�������ʗp
	//---------------------

	int m_Cnt = 0;			//���݂̔z��ԍ�
	CharaID m_SelectFlg = CharaID::BOX;	//�Ƃ肠���������l

	//�L�������Ƃ̏��
#define NUM 4											// �P:�z��(�ς���)
	struct E_ID{
		CharaID id;
		std::string path;
		enum SHAPE{ BOX1, BOX5 };//�z�u���镨�̌`��
		SHAPE shpe = BOX1;
	};
	static E_ID eidlist[NUM];
														// �Q:�����֒ǉ�����	�R:�������鏈����cpp
	void Syoki(){
		// 1��
		eidlist[0].id = CharaID::BOX;
		eidlist[0].path = "data/original/stand_box.x";
		eidlist[0].shpe = E_ID::SHAPE::BOX1;
		// 2��
		eidlist[1].id = CharaID::LBOX;
		eidlist[1].path = "data/original/largebox.x";
		eidlist[1].shpe = E_ID::SHAPE::BOX5;
		// 3��
		eidlist[2].id = CharaID::SPRING;
		eidlist[2].path = "data/original/Spring/Spring.xed";
		eidlist[2].shpe = E_ID::SHAPE::BOX1;
		// 4
		eidlist[3].id = CharaID::STAIRS;
		eidlist[3].path = "data/original/Stairs/Stairs.xed";
		eidlist[3].shpe = E_ID::SHAPE::BOX1;
	}
	/////////////////////////////////





	//�V���O���g��
private:
	static EditMode*	s_pInst;
	EditMode() {
		Syoki();
	}
public:
	static void DeleteWorld() { YsSafe_Delete(s_pInst);
	s_pInst = nullptr;
	}
	static void CreateWorld() {
		DeleteWorld();
		s_pInst = new EditMode();
	}
	static EditMode& GetWorld() { return *s_pInst; }
};

#define EW EditMode::GetWorld()

// �����\���p
class PickUp{
public:
	PickUp(){
		m_Tex = SPtr<YsTexture>(new YsTexture);
		m_Tex = APP.m_ResStg.LoadTexture("data/original/Yellow.png");
		m_Board.SetVertex(-0.5f, -0.5f, 1.0f, 1.0f);
		m_Board.SetTex(m_Tex);
	}
	~PickUp(){ 
		//m_Tex->Release();
		m_Tex = nullptr;
	}
	void SetPos(YsMatrix _m){
		m_Buf = _m;
		m_BMat = _m;
		m_BMat.Move_Local(0, 0.01f, 0);
		m_BMat.RotateX_Local(90.0f);
		m_bflg = true;
	}
	void Draw(){
		if (m_bflg){
			ShMgr.m_Samp.DrawBillBoard(m_Board, &m_BMat);
		}
	}
	void Draw(const YsMatrix &_m){
		if (m_bflg){
			ShMgr.m_Samp.DrawBillBoard(m_Board, &_m);
		}
	}

	YsBillBoard m_Board;
	SPtr<YsTexture> m_Tex;
	YsMatrix m_BMat;
	YsMatrix m_Buf;
	bool m_bflg = false;
};

// �`��p
class EditVec2{
public:
	float x = 0;
	float z = 0;
	bool Center = false;
	EditVec2(){}
	EditVec2(float _x, float _z){ x = _x; z = _z; }
};

// �z�u��
class EditObject{
public:
	EditObject(int _id, SPtr<GameCamera> _Cam, int _num){
		// ID���Z�b�g
		m_ID = _id;
		// �e�N�X�`������
		m_PickUp = new PickUp;
		// �J�����̃|�C���^�擾
		m_wpNowCam = _Cam;


		m_Num = _num;

	};
	~EditObject(){
		m_State = nullptr;
		m_Shape.clear();
		m_wpNowCam.reset();
		m_wpObj.reset();
		m_Shape.clear();
		m_State = nullptr;
		delete m_PickUp;
	}

	void Init(YsMatrix &_m);
	void Update();
	void Draw();

	//�G�f�B�b�g�J���������L���Ă��炤
	WPtr<GameCamera>m_wpNowCam;

	//�����\��
	PickUp * m_PickUp;

	//������̃L�����A�h���X
	WPtr<CharacterBase>m_wpObj;

	//�L���t���O
	void SetKill(){
		m_KillFlg = true;
	}
	bool GetKillFlg(){ return m_KillFlg; }
	bool m_KillFlg = false;

	//�`����w�肷��z��
	std::vector<SPtr<EditVec2>>m_Shape;

	//�ݒu�t���O
	static bool NowSetting;

	//�X�e�[�g
	SPtr<Edit_State>m_State;

	//�I�u�W�F�N�g���ʗp
	int m_ID,m_Num;


	//���b�V���p
	SPtr<YsGameModel>m_spMo;
	YsMatrix m_Mat;
	YsModelObject m_Mo;

	//�ݒu�G�t�F�N�g�p
	YsBillBoard m_Board;
};
class Edit_State{
public:

	virtual ~Edit_State(){}
	virtual void Update(EditObject & E_Obj) = 0;
	virtual void Draw(EditObject & E_Obj) = 0;
	//�Ԃ�l�p�̍s��
	YsMatrix r_Mat;
};
class Edit_Set : public Edit_State{
public:
	Edit_Set(YsMatrix &_m, WPtr<GameCamera> _Cam){
		EditObject::NowSetting = true;
		r_Mat = _m;
		m_wpCamera = _Cam;
	}
	~Edit_Set(){ EditObject::NowSetting = false; }
	virtual void Update(EditObject & E_Obj)override;
	virtual void Draw(EditObject & E_Obj)override;

	bool m_onBlock = false;
	bool m_onY = false;
	YsMatrix m_Set;
	WPtr<GameCamera>m_wpCamera;

};
class Edit_Rot :public Edit_State{
public:
	Edit_Rot(YsMatrix &_m){
		r_Mat = _m;
		m_PauseCnt = 0;
		m_PauseFlg = 0;
		EditObject::NowSetting = true;
	}
	~Edit_Rot(){ EditObject::NowSetting = false; }
	virtual void Update(EditObject & E_Obj)override;
	virtual void Draw(EditObject & E_Obj)override;

private:
	int m_PauseCnt = 0;
	int m_PauseFlg = 0;
	enum{ UP = 1, DOWN };

	int OneFrame = 0;
};
class Edit_Put :public Edit_State{
public:
	Edit_Put(YsMatrix &_m){ r_Mat = _m; }
	~Edit_Put(){}
	virtual void Update(EditObject & E_Obj)override;
	virtual void Draw(EditObject & E_Obj)override;

};


//+++++++++++++++
// �֐�
//+++++++++++++++

// �}�E�X���W3D�ϊ������Ԏ�O�œ��������L�����ƁA
//		���̏�ɉ�������Ă��邩��ʂ����Ă����
bool MouceTo3D(GameCamera _Camera, YsVec3 &_Out, SPtr<CharacterBase> &_HitObj, bool &_HeadFlg);

// XZ���ʂ�Y�̍�����0�̃}�E�X���W���擾
D3DXVECTOR3 Calc2(GameCamera _Camera);

// �Z����ɋ�؂����l��Ԃ�
float cell(float _f, float _size);

