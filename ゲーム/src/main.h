#ifndef __main_h__
#define __main_h__

// YskLib0
#include "YskLib.h"
using namespace YskLib;

#include "GameResource/GameResource.h"

#include "Shader/ShaderManager.h"

#include "Input/CInput.h"
#include "Game/Task/TaskManager.h"
#include "Game/Scene/Scene.h"


#define ShMgr APP.m_ShMgr


//=====================================================
// Appクラス
//  アプリケーションのグローバル的なクラス
//=====================================================
class CApp {
// メンバ
public:
	//=================================================
	// ウィンドウ関係
	//=================================================
	HINSTANCE		m_hInst;			// アプリケーションのインスタンスハンドル
	HWND			m_hWnd;				// ウィンドウハンドル
	DWORD			m_FrameCnt;			// 現在のフレーム値
	bool			m_bWndActive;		// ウィンドウがアクティブか？

	//=================================================
	// FPS制御関係
	//=================================================
	YsFpsControl	m_FpsCtrl;			// FPS制御クラス

	//=================================================
	// 描画設定
	//=================================================
	bool			m_b60fpsDraw;		// 描画を60fpsにする　falseだと30fps(※描画のみ　処理は60fpsです)
	int				m_NonDrawFlag;		// 描画をしないようにするフラグ 0:描画する 0以外:描画しない(自動で切り替わります)


	//============================================================
	// その他
	//============================================================
	// 現在のシーン
	SPtr<BaseScene>			m_NowScene;

	// シェーダ関係をまとめたクラス
	ShaderManager			m_ShMgr;

	// 乱数
	YsMTRand				m_Rnd;

	// リソース管理クラス
	//  テクスチャや3Dモデルやサウンドなど、リソースの読み込みを管理する
	//  ※これを使用することで、同じファイル名のリソースの重複読み込みが防げます
	GameResourceStorage		m_ResStg;














	//============================================================
	// 
	//============================================================
	// ウィンドウ初期化
	BOOL InitWindow(HINSTANCE hInstance,int w,int h);	// ウィンドウ作成、Direct3D初期化など
	void Release();

	// メインループ
	int Loop();

	// ウィンドウ関数
	static LRESULT CALLBACK callWindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

	// ゲーム終了
	void ExitGame(){
		DestroyWindow(m_hWnd);
	}

	// クライアントサイズを指定サイズにする
	void SetClientSize(HWND hWnd, int w, int h);

//============================================================
// シングルトン
//============================================================
public:
	static CApp &GetInstance(){
		static CApp Instance;
		return Instance;
	}

private:
	// コンストラクタ
	CApp(){
		m_hWnd = nullptr;
		m_hInst = nullptr;
		m_bWndActive = true;

		m_FrameCnt = 0;
		m_NonDrawFlag = 0;
		m_b60fpsDraw = false;
	}
};

#define APP CApp::GetInstance()

#endif
