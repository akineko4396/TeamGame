//============================================================
//
//
// サンプルシーンで使用している飛行機や箱やエフェクトなどの
// オブジェクトクラスをまとめて書いてます
//
//
//============================================================
#ifndef __Sample_Data_h__
#define __Sample_Data_h__

//============================================================
// 物理エンジンワールド
//
//============================================================
class SampleBulletWorld : public YsBulletWorld {
public:
	// 物体同士が衝突したとき、これが呼ばれる
	virtual bool ContactProcessedCallback(btManifoldPoint& cp, YsBulletRigidBody* body0, YsBulletRigidBody* body1) override;
};


//============================================================
// キャラ基本タスク
//============================================================
class Task_CharaBase : public BaseTask {
public:
	// 剛体ヒット時に呼ばれる関数
	virtual void OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest) {}

protected:
	Task_CharaBase(){}
};

//============================================================
// 人間キャラタスク
//============================================================
class Task_Human : public Task_CharaBase {
private:
	YsModelObject		m_Mo;			// モデル操作オブジェクト
	YsAnimator			m_Anim;			// アニメーション管理

	YsMatrix			m_Mat;			// 行列

public:
	// 初期設定
	void Init(SPtr<YsGameModel> mesh, YsMatrix& mat);
	// 処理関数
	virtual void Update() override;
	// 描画関数
	virtual void Draw() override;
};

//============================================================
// 飛行機キャラタスク
//============================================================
class Task_AirPlane : public Task_CharaBase {
private:
	YsBulletRigidBody		m_btBody;	// 物理 剛体

	YsModelObject		m_Mo;			// モデル操作オブジェクト
	YsMatrix			m_Mat;			// 行列

	bool				m_bDebugHit = false;	// 剛体ヒットフラグ(デバッグ表示用)

public:

	Task_AirPlane()
	{
	}

	// 初期設定
	void Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsVec3& pos, float mass);
	// 処理関数
	virtual void Update() override;
	// 描画関数
	virtual void Draw() override;
	// 物理剛体同士がヒットした時
	virtual void OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest) override;
};

//============================================================
// BOX剛体キャラタスククラス
//============================================================
class Task_RigidBox : public Task_CharaBase {
private:
	YsModelObject	m_Mo;			// モデル操作オブジェクト
	YsMatrix		m_Mat;			// 行列
	int				m_Cnt;			// 寿命カウント

	YsBulletRigidBody	m_btBody;	// 物理 剛体

	bool			m_bDebugHit;	// 剛体ヒットフラグ(デバッグ表示用)


	YsMatrix		m_OldMat;		// 前回の行列

	int				m_ExplodeInterval;

public:
	YsBulletRigidBody& GetRigidBox(){ return m_btBody; }

public:
	Task_RigidBox() : m_bDebugHit(false){
	}

	~Task_RigidBox(){
		// 剛体削除(別に呼ばなくてもデストラクタで自動で呼ばれます)
//		m_RigidBox.Release();
	}

	// 初期設定
	void Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsMatrix& m, YsVec3& halfsize, float mass);
	// 処理関数
	virtual void Update() override;
	// 描画関数
	virtual void Draw() override;


	// 物理剛体同士がヒットした時
	virtual void OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest) override;
};

//============================================================
// 爆発エフェクト
//============================================================
class Task_Explode : public BaseTask {
public:
	void Init(YsTextureSet* tex, float x, float y, float z, float size);
	// 処理関数
	virtual void Update() override;
	// 描画関数
	virtual void Draw() override;

private:

	YsBillBoard		m_BB;
	YsMatrix		m_Mat;
	YsVec3			m_vMove;

	SPtr<PointLight>	m_PointLight;	// ポイントライトデータ
};


//============================================================
// レーザーエフェクト
//============================================================
class Task_Laser : public BaseTask {
public:

	void Init(YsTextureSet* texset, float x, float y, float z, const YsMatrix* mat);

	// 処理関数
	virtual void Update() override;

	// 描画関数
	virtual void Draw() override;

private:

	YsLaser		m_Lz;		// 曲線レーザークラス
	YsMatrix	m_Mat;		// 行列

	float		m_fSpeed;	// 移動速度
	int			m_Cnt;		// 寿命カウント

	float		m_fAngPow;	// 回転速度

	SPtr<PointLight>	m_PointLight;	// ポイントライトデータ

};

#endif
