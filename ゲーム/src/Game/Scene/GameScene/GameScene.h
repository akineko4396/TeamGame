#ifndef __GameScene_h__
#define __GameScene_h__

#include "Game/GameCamera.h"

class Stage;

//=========================================================
// ゲームシーンクラス
//=========================================================
class GameScene : public BaseScene{
public:

	// 初期設定関数
	void Init();
	// 解放関数
	void Release();
	// 更新処理
	virtual int Update() override;
	// 描画処理
	virtual void Draw() override;

	// ステージ生成
	static SPtr<Stage>m_Stage;
	static WPtr<Stage>GetStage(){ return m_Stage; }
	int m_StageNo;
	GameScene(int _StageNo) {
		m_StageNo=_StageNo;
	}

	virtual ~GameScene() {
		Release();
	}

public:
	//=========================================================
	// その他データ
	//=========================================================




	
};

#endif