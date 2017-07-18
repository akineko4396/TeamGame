
#pragma once

#include "../StageBase.h"

#include "Data\DataManager.h"

// 配置
#include "Game\Edit\Put.h"

#include "Game\GameAI\NavigationAI.h"

// ver.1
//
// ステージ１(仮) : ステージごとの処理
//
// Stageを継承

// 【メンバ】
//
// ・m_wpCamera...視点になるキャラを入れる
// ・m_wpPlayer...キー処理などをさせるキャラを入れる
//


class GameWorld : public Stage{
public:

	void Init();
	void Release();

	void Update();
	void Draw();

	~GameWorld(){
		Release();
	}
	
	// ポーズカメラ
	GameCamera			m_Cam;

	// 現在カメラ視点になってるキャラへのアドレス
	WPtr<CharacterBase>	m_wpCamera;	

	// 操作するプレイヤーキャラへのアドレス
	WPtr<CharacterBase>	m_wpPlayer;		



	//											//
	//  ↓ 後から変更するかもしれないメンバ ↓	//
	//											//

	Put m_Put;	//プレイヤーが物を配置する


	// デバッグ用 モード切替

	enum{NORMAL,EDIT,CREATE};
	int m_EditFlg = NORMAL;


	// 暗幕データ
	float				m_BlackCurtainPower = 0;	// 暗幕の強さ(フェードイン/アウトで使用)
	float				m_BlackCurtainAdd = 0;		// m_BlackCurtainPowerに毎回加算する値

	//==============================================
	// 物理エンジン
	//==============================================
	YsBulletWorld		m_Bullet;					// 物理エンジン

	//==============================================
	// その他
	//==============================================
	bool				m_PauseFlag = false;		// ポーズフラグ
	bool				m_Debug_DrawHit = false;	// あたり判定デバッグ表示フラグ

};
