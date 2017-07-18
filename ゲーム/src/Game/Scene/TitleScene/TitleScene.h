#ifndef __TitleScene_h__
#define __TitleScene_h__

#include "Game/GameCamera.h"

//=========================================================
// タイトルシーンクラス
//=========================================================
class TitleScene : public BaseScene {
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
	TitleScene() {
	}
	// 
	virtual ~TitleScene() {
		Release();
	}

public:
	//=========================================================
	// その他データ
	//=========================================================

	// 平行光源
	SPtr<DirLight>	m_DirLight;

	// カメラ
	GameCamera		m_Cam;			// カメラクラス

	// テクスチャ
	SPtr<YsTexture>		m_texBack;

	// キャラクタに必要なデータをまとめただけの構造体
	struct Chara {
		YsModelObject	m_Mo;		// メッシュオブジェクト　モデルデータ(YsGameModel)を扱える
		YsMatrix		m_Mat;		// 行列

		YsAnimator		m_Anim;		// アニメーター　アニメーションを担当する
		YsAnimator		m_Anim2;	// アニメーターはいつくあっても良い　複数使うとアニメの特殊な合成が可能
	};

};


#endif