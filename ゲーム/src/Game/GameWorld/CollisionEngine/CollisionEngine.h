
#pragma once




/*ViewFrustum==============================

視錘台クラス

視錘台を作成する

=========================================*/
class ViewFrustum {

public:

	//視錘台作成
	void Create(YsMatrix _Proj, YsMatrix _View, YsMatrix _Cam);

	//カメラからの法線を返す
	inline YsVec3 GetNormal(int _i) { return m_vN[_i]; }

	//保存しているカメラの座標を返す
	inline YsVec3 GetCamPos() { return m_CamPos; }

private:

	//カメラの座標格納用
	YsVec3	m_CamPos;

	//	カメラから法線
	YsVec3	m_vN[4];

	//構造体
	struct CLONEVERTEX {
		YsVec3 Pos;
		YsVec3 Normal;
		YsVec2 Tex;
	};

public:

	//--------------------------
	//	シングルトンパターン生成
	//--------------------------
	static ViewFrustum& GetInstance()
	{
		static ViewFrustum Instance;
		return Instance;
	}

};

// 簡単にアクセスできるようにするためのマクロ
#define VF	ViewFrustum::GetInstance()

/*===============

template関数

===============*/

/*<template>CheckViewFrustum-------------------------------->

視錘台とキャラメッシュのスフィアとの判定

_MyData		:	自身のデータ

return true	:	視錘台内
return false:	視錘台外

<-----------------------------------------------------*/

//template <class T>
//inline BOOL CheckViewFrustum(SPtr<T>& _MyData, SPtr<YsMesh> _Mesh)

class CharacterBase;
BOOL CheckViewFrustum(SPtr<CharacterBase> _MyData, SPtr<YsMesh> _Mesh);














//============================================
// あたり判定 定数
//============================================

// 判定グループフィルタ用
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
		// あとは省略。最大GP_31まで作れる
	};
}

// 判定形状フィルタ用
namespace HitShapes {
	enum {
		SPHERE = 0x00000001,	// 球
		RAY = 0x00000002,	// レイ
		MESH = 0x00000004,	// メッシュ
	};
}

// ヒット状態フラグ
namespace HitStates {
	enum {
		ENTER = 0x00000001,	// HITした1回目
		STAY = 0x00000002,	// HIT中ずっと
		EXIT = 0x00000004,	// HITしなくなった1回目
	};
}

class BaseGameObj;
class BaseHitObj;

//============================================
// ヒット結果データ構造体
//  あたり判定を行った結果のデータ
//============================================
struct HitResData {
	BaseHitObj* m_youHitObj = nullptr;	// 相手のHitObj

	YsVec3		m_vHitPos;			// ヒット位置

	YsVec3		m_vPush;			// (球、メッシュ用)押されているベクトル ぶつかり系の処理に使用

	float		m_RayDist = 0;		// (レイ用)当たった距離
	int			m_FaceIdx = 0;		// (レイ用)相手がメッシュの時のみ、面番号が入る
};


//============================================
// 判定OBJ基本クラス
//============================================
class BaseHitObj {
public:
	// ※ 見やすさ重視で全部publicにしてますので、各自でカプセル化してください。

	//======================================================
	// 判定データ
	//======================================================
	// 判定フィルタ関係
	int					m_AtkFilter = 0xFFFFFFFF;	// 判定する側の時のフィルタ		HitGroupsを使用
	int					m_DefFilter = 0xFFFFFFFF;	// 判定される側の時のフィルタ　	HitGroupsを使用

	// 判定形状フィルタ関係
	int					m_ShapeFilter = 0xFFFFFFFF;	// 判定する側の時、どの形状と判定するかのフィルタ	HitShapesを使用
	int					m_Shape = 0;				// 自分の形状(高速ダウンキャスト用)　HitShapesを使用

	int					m_HitState = 0;				// ヒット状態フラグ

	WPtr<BaseGameObj>	m_wpTask;					// 対象のキャラ(タスク)へのアドレス。同じキャラは判定を無視する用途で使用

	// 汎用事前チェック関数
	std::function<bool(BaseHitObj*)>	m_BroadPhaseProc;

	// ヒット結果リスト あたり判定結果を格納する場所
	std::vector<HitResData>				m_HitResTbl;

	//======================================================
	// 判定通知関数
	//======================================================
	// HITした1回目のみ呼ばれる
	std::function<void(const SPtr<BaseHitObj>&)>	m_OnHitEnter;
	// HITしてる間呼ばれる
	std::function<void(const SPtr<BaseHitObj>&)>	m_OnHitStay;
	// HITしなくなった1回目のみ呼ばれる
	std::function<void(const SPtr<BaseHitObj>&)>	m_OnHitExit;


	//======================================================
	// その他データ 好きにしてください。
	//======================================================
	float				m_Mass = 0;			// ぶつかり時に使用 質量0で固定物
	UINT				m_ID = 0;			// 判別用ID



	//======================================================
	// あたり判定関数
	//  obj			: 相手の判定オブジェクト
	//  resData1	: 判定結果を入れる変数　自分側
	//  resData2	: 判定結果を入れる変数　相手側
	//======================================================
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) = 0;

	// 質量比計算 m_Massとmassを比較し、その比率を返す。
	float CalcMassRatio(float mass);

	//======================================================
	// デバッグ表示用 Release時にはいらないです
	//======================================================
	// デバッグ描画
	virtual void DebugDraw(float alpha){}
	YsVec4		m_Debug_Color = YsVec4(1, 1, 1, 1);

	// 
	virtual ~BaseHitObj(){
	}
};

//============================================
// 球あたり判定OBJ
//============================================
class HitObj_Sphere : public BaseHitObj {
public:
	// データ
	YsVec3		m_vPos;		// 座標
	float		m_Rad;		// 半径

public:
	HitObj_Sphere(){
		m_Shape = HitShapes::SPHERE;
	}

	// あたり判定計算関数
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) override;

	// デバッグ描画
	virtual void DebugDraw(float alpha) override;

	// 設定
	//  task	… この判定データの持ち主キャラ
	//  pos		… 座標
	//  rad		… 半径
	void Set(SPtr<BaseGameObj> task, const YsVec3& pos, float rad){
		m_wpTask = task;
		m_vPos = pos;
		m_Rad = rad;
	}

};

//============================================
// レイあたり判定OBJ
//============================================
class HitObj_Ray : public BaseHitObj {
public:
	// データ Start～Endの座標間でレイ判定する感じ
	YsVec3	m_vPos1;	// レイ座標１(Start)
	YsVec3	m_vPos2;	// レイ座標２(End)
	YsVec3	m_vDir;		// 方向　m_vPos2 - m_vPos1が入る
	float	m_RayLen;	// レイの長さ

public:
	HitObj_Ray(){
		m_Shape = HitShapes::RAY;
	}

	// あたり判定計算関数
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) override;

	// デバッグ描画
	virtual void DebugDraw(float alpha) override;

	// 設定
	//  task	… この判定データの持ち主キャラ
	//  pos1	… レイ開始座標
	//  pos2	… レイ終了座標
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
// メッシュあたり判定OBJ
//============================================
class HitObj_Mesh : public BaseHitObj {
public:
	SPtr<YsMesh>	m_pMesh;		// 判定するメッシュのアドレス

	YsMatrix		m_Mat;			// 変換行列

public:
	HitObj_Mesh(){
		m_Shape = HitShapes::MESH;
	}

	// あたり判定計算関数
	virtual bool HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2) override;

	// デバッグ描画
	virtual void DebugDraw(float alpha) override;

	// 設定
	//  task	… この判定データの持ち主キャラ
	//  pMesh	… メッシュデータ
	//  mat		… 行列
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
// あたり判定マネージャ
//============================================
class CollisionEngine{
public:
	// 
	~CollisionEngine(){
		ClearList();
	}

	// 判定する側として登録
	void AddAtk(const SPtr<BaseHitObj>& obj){
		m_AtkList.push_back(obj);
	}
	// 判定される側として登録
	void AddDef(const SPtr<BaseHitObj>& obj){
		m_DefList.push_back(obj);
	}

	// atkObj vs 全m_DefListの判定を行う(通知はしない)
	void Test(BaseHitObj* atkObj);

	// 当たり判定を実行し、各Objへ通知する
	// 全m_AtkList vs 全m_DefListの判定を行い、全m_AtkListへ通知する
	void Run();


	// リストをクリア
	void ClearList(){
		m_AtkList.clear();
		m_DefList.clear();
	}

	// デバッグ描画
	void DebugDraw(float alpha);

private:

	// HitObjリスト
	//  効率化の為、判定する側とされる側の２つのリストで管理
	std::vector<SPtr<BaseHitObj>>	m_AtkList;	// 判定する側
	std::vector<SPtr<BaseHitObj>>	m_DefList;	// 判定される側

};
