#pragma once

//--------------------------------------------
//
// EditModeクラス ver.1.0
// 
// 開発者向けエディタ
// エディットモード
//
//
// 【呼び出し順番】
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
// ・オブジェクトを配置したり編集するクラス
// ・編集済みになったら本編のキャラのインスタンス生成する
//		編集後もEditModeクラスの編集済みリストに残る
// ・作ったキャラのインスタンスが消滅したら自身も消える
//
//--------------------------------------------

// 箱キャラクラス
class LBox : public CharacterBase{
public:

	~LBox(){}

	//基本メンバ関数

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





// 全体の管理クラス
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

	//解放
	void Release();


	void CamUpdate(){ m_Cam->Update(); }

	void SetCamera(){ m_Cam->SetCamera(); }



	
	//共有カメラ
	SPtr<GameCamera> m_Cam;

	//設置した物リスト
	static std::vector<SPtr<EditObject>>m_eList;
	void Push(SPtr<EditObject> _obj){ m_eList.push_back(_obj); }
	void PopBack(){ m_eList.pop_back(); }
	void CheckKill();

	//足元表示
	PickUp * m_PickUp;

	//現在編集中のキャラ
	WPtr<EditObject>m_wpNow;



	//---------------------
	//キャラ識別用
	//---------------------

	int m_Cnt = 0;			//現在の配列番号
	CharaID m_SelectFlg = CharaID::BOX;	//とりあえず初期値

	//キャラごとの情報
#define NUM 4											// １:配列数(変えて)
	struct E_ID{
		CharaID id;
		std::string path;
		enum SHAPE{ BOX1, BOX5 };//配置する物の形状
		SHAPE shpe = BOX1;
	};
	static E_ID eidlist[NUM];
														// ２:ここへ追加する	３:生成する処理はcpp
	void Syoki(){
		// 1つ目
		eidlist[0].id = CharaID::BOX;
		eidlist[0].path = "data/original/stand_box.x";
		eidlist[0].shpe = E_ID::SHAPE::BOX1;
		// 2つ目
		eidlist[1].id = CharaID::LBOX;
		eidlist[1].path = "data/original/largebox.x";
		eidlist[1].shpe = E_ID::SHAPE::BOX5;
		// 3つ目
		eidlist[2].id = CharaID::SPRING;
		eidlist[2].path = "data/original/Spring/Spring.xed";
		eidlist[2].shpe = E_ID::SHAPE::BOX1;
		// 4
		eidlist[3].id = CharaID::STAIRS;
		eidlist[3].path = "data/original/Stairs/Stairs.xed";
		eidlist[3].shpe = E_ID::SHAPE::BOX1;
	}
	/////////////////////////////////





	//シングルトン
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

// 足元表示用
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

// 形状用
class EditVec2{
public:
	float x = 0;
	float z = 0;
	bool Center = false;
	EditVec2(){}
	EditVec2(float _x, float _z){ x = _x; z = _z; }
};

// 配置物
class EditObject{
public:
	EditObject(int _id, SPtr<GameCamera> _Cam, int _num){
		// IDをセット
		m_ID = _id;
		// テクスチャ生成
		m_PickUp = new PickUp;
		// カメラのポインタ取得
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

	//エディットカメラを共有してもらう
	WPtr<GameCamera>m_wpNowCam;

	//足元表示
	PickUp * m_PickUp;

	//生成後のキャラアドレス
	WPtr<CharacterBase>m_wpObj;

	//キルフラグ
	void SetKill(){
		m_KillFlg = true;
	}
	bool GetKillFlg(){ return m_KillFlg; }
	bool m_KillFlg = false;

	//形状を指定する配列
	std::vector<SPtr<EditVec2>>m_Shape;

	//設置フラグ
	static bool NowSetting;

	//ステート
	SPtr<Edit_State>m_State;

	//オブジェクト識別用
	int m_ID,m_Num;


	//メッシュ用
	SPtr<YsGameModel>m_spMo;
	YsMatrix m_Mat;
	YsModelObject m_Mo;

	//設置エフェクト用
	YsBillBoard m_Board;
};
class Edit_State{
public:

	virtual ~Edit_State(){}
	virtual void Update(EditObject & E_Obj) = 0;
	virtual void Draw(EditObject & E_Obj) = 0;
	//返り値用の行列
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
// 関数
//+++++++++++++++

// マウス座標3D変換から一番手前で当たったキャラと、
//		その上に何か乗っているか一通りやってくれる
bool MouceTo3D(GameCamera _Camera, YsVec3 &_Out, SPtr<CharacterBase> &_HitObj, bool &_HeadFlg);

// XZ平面でYの高さが0のマウス座標を取得
D3DXVECTOR3 Calc2(GameCamera _Camera);

// セル状に区切った値を返す
float cell(float _f, float _size);

