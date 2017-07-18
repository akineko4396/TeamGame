#ifndef __SampleScene_h__
#define __SampleScene_h__


#include "Game/GameCamera.h"
#include "Sample_Data.h"

//=========================================================
// タイトルシーンクラス
//=========================================================
class SampleScene : public BaseScene {
public:
	// 初期設定関数
	void Init();
	// 解放関数
	void Release();

	// 更新処理
	virtual int Update() override;
	// 描画処理
	virtual void Draw() override;

	// 
	SampleScene() {
	}
	// 
	virtual ~SampleScene() {
		Release();
	}

public:

	static SampleScene *s_pScene;

	//=========================================================
	// タスク関係
	//=========================================================
	TaskManager		m_TaskMgr;
	TaskManager		m_EffectTaskMgr;

	//=========================================================
	// その他データ
	//=========================================================
	// 平行光源
	SPtr<DirLight>	m_DirLight;

	// カメラ
	GameCamera		m_Cam;

	bool			m_PauseFlag;	// ポーズ
	bool			m_ShowPointLoght = false;	// ポイントライトをデバッグ表示

	//	CDSBuffer		m_sndSE1;
	//	CDSBuffer		m_sndBGM;

	// テクスチャ
	YsTexture			m_texBack;		// YsTextureをそのまま使用できるが、あまりお勧めしません。
	// 利点：使い方が簡単　欠点：他と共有しにくい
	SPtr<YsTexture>		m_texBack2;		// YsTextureをshared_ptrで用意。こちらの方がいろいろと都合がいい。
	// 利点：他と共有しやすい
	YsTextureSet		m_tsLaser;		// SPtr<YsTexture>を複数持ったもの。今後シェーダでの高品質な表現の時に必要。
	YsTextureSet		m_tsExplode;
	// 迷ったらTextureSetつかってればOK。
	// 利点：複数のテクスチャが扱えるし、共有もしやすい

	// モデル
	SPtr<YsGameModel>	m_pSm;
	SPtr<YsGameModel>	m_pSm2;
	SPtr<YsGameModel>	m_xF15;
	SPtr<YsGameModel>	m_xSu27;
	SPtr<YsGameModel>	m_xBox;
	SPtr<YsGameModel>	m_xMap;

	// ステージマップ用
	YsModelObject		m_moMap;
	YsMatrix			m_mMap;
	YsBulletRigidBody	m_btMap;	// ステージマップ用の剛体

	// 物理ワールド
	SampleBulletWorld	m_Bullet;


	// サウンド
	SPtr<YsSoundData>	m_sndSE;


	float				m_LightPower;	// 平行光源の明るさ

	SPtr<PointLight>		m_PL[2];
	SPtr<SpotLight>		m_SL;
};

#endif