#ifndef __ShaderManager_h__
#define __ShaderManager_h__

#include "LightManager.h"

#include "EasyShader.h"
#include "SampleShader.h"
#include "SpriteShader.h"






//=======================================================================
//
// シェーダ管理クラス
//　各シェーダ描画関係のもの管理をするクラス
// 
//
// ★授業では定数バッファは以下のように使う予定です
//  cbuffer(b0～b8)		… 自由 シェーダごとに変わります
//  cbuffer(b9)			… (固定)ライトデータとして使用予定　b12と同じ感じになるけど、勉強のため自分で作りましょう
//  cbuffer(b10)		… (固定)シャドウマップで使用予定
//  cbuffer(b11)		… 未使用　自由に使ってね
//  cbuffer(b12)		… (固定)SampleShaderでのライトデータで使ってます
//  cbuffer(b13)		… (固定)デバッグ用で使用予定
//  ※このあたりの仕様は、自分でやりやすいようにカスタマイズしてみてください。
//
// ★授業ではサンプラステートは以下のように使う予定です	
//  s0					… Linear補間のWrapサンプラ
//  s1					… Linear補間のClampサンプラ
//  s2～9				… 自由　好きなように使ってください
//  s10					… ShadowMap用 Comparisonサンプラ
//  s11～s15			… 自由　好きなように使ってください
//
//=======================================================================
class ShaderManager {
public:
	//================================================
	// ライト
	//================================================
	LightManager		m_LightMgr;			// ライト管理クラス

	//================================================
	// ステート
	//================================================
	YsBlendState		m_bsAlpha;			// 半透明合成モード
	YsBlendState		m_bsAdd;			// 加算合成モード
	YsBlendState		m_bsNoAlpha;		// アルファ無効モード

	//================================================
	//
	// サンプラ
	//
	//================================================
	YsSamplerState		m_smp0_Linear_Wrap;		// (s0) Linear補間 & Wrapモード用サンプラ
	YsSamplerState		m_smp1_Linear_Clamp;	// (s1) Linear補間 & Clampモード用サンプラ


	//================================================
	//
	// シェーダクラス
	//
	//================================================
	EasyShader		m_Es;				// 勉強用の単純なシェーダ
	SampleShader	m_Samp;				// いろいろな機能を入れたサンプル用シェーダクラス
	SpriteShader	m_Sprite;			// 2D描画用シェーダクラス







	//================================================
	// その他
	//================================================

	YsTexture		m_texCube;			// キューブマップテクスチャ(環境マッピング用)



	//================================================
	// 最初に一回実行
	//================================================
	void Init() {
		// シェーダクラス初期化
		m_Es.Init();
		m_Samp.Init();
		m_Sprite.Init();







		//-------------------------------
		// よく使うブレンドステートを作成
		//-------------------------------
		// 半透明合成用ステート
		m_bsAlpha.Set_Alpha(-1);
//		m_bsAlpha.Set_AlphaToCoverageEnable(true);
		m_bsAlpha.Create();
		// 加算合成用ステート
		m_bsAdd.Set_Add(-1);
//		m_bsAlpha.Set_AlphaToCoverageEnable(true);
		m_bsAdd.Create();
		// アルファ値無効モード
		m_bsNoAlpha.Set_NoAlpha(-1, true);
		m_bsNoAlpha.Create();

		//-------------------------------
		// よく使うサンプラ作成 & セット
		//-------------------------------
		m_smp0_Linear_Wrap.SetAll_Standard();
		m_smp0_Linear_Wrap.Set_Wrap();
		m_smp0_Linear_Wrap.Create();
		m_smp0_Linear_Wrap.SetStateVS(0);
		m_smp0_Linear_Wrap.SetStatePS(0);

		m_smp1_Linear_Clamp.SetAll_Standard();
		m_smp1_Linear_Clamp.Set_Clamp();
		m_smp1_Linear_Clamp.Create();
		m_smp1_Linear_Clamp.SetStateVS(1);
		m_smp1_Linear_Clamp.SetStatePS(1);




		// ライト管理クラス
		m_LightMgr.Init();

		// キューブマップ
		m_texCube.LoadTexture("data/Texture/CubeMap.dds");
		m_texCube.SetTexturePS(20);
	}

	//================================================
	// 最後に一回実行される
	//================================================
	void Release() {
		// シェーダクラス解放
		m_Es.Release();
		m_Samp.Release();
		m_Sprite.Release();







		// 
		m_bsAdd.Release();
		m_bsAlpha.Release();

		// 
		m_smp0_Linear_Wrap.Release();
		m_smp1_Linear_Clamp.Release();

		m_LightMgr.Release();
	}

	//================================================
	// 描画直前に更新するデータ
	//================================================
	void UpdateBeforeDraw() {
		// 各シェーダのフレーム単位でのデータ更新
		m_Es.WritePerFrameData();		// 定数バッファへ必要なデータを書き込んだりする
		m_Samp.WritePerFrameData();		// 定数バッファへ必要なデータを書き込んだりする




		// ライト情報
		m_LightMgr.Update();
	}

	//================================================
	// 各シェーダのコンパイル済みのシェーダ(cso)をファイルに保存
	//================================================
	void SaveCsoToFile(){
		m_Es.SaveCsoToFile();
		m_Samp.SaveCsoToFile();
		m_Sprite.SaveCsoToFile();





	}

};

#endif
