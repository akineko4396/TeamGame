#ifndef __CInput_h__
#define __CInput_h__

//===========================================================
//
// キーボードやマウスの入力などを管理する
//
//===========================================================
class CInput
{
public:
	//===============================
	// 共通
	//===============================
	// 定数
	// マウスボタン番号
	enum MOUSEBUTTON{
		MOUSE_L = 0,
		MOUSE_R = 1,
		MOUSE_M = 2
	};
	// キーフラグ
	enum KEYFLAG{
		KEY_Enter	= 1,		// 押した瞬間
		KEY_Stay	= 2,		// 押してる状態
		KEY_Exit	= 4			// 離した瞬間
	};

//===============================
// キーボード
//===============================
public:
	void UpdateKey();		// 仮想キーの状態を取得し、Keyを更新。

	// キーボード判定
	// KeyNo	… 判定するキーコード
	// flag		… どのタイミングで判定するか
	//   ・KEY_Button	… 押している間
	//   ・KEY_Trigger	… 押した最初の1回目のみ
	//   ・KEY_Release	… 離した最初の1回目のみ
	bool KeyCheck(unsigned char KeyNo, KEYFLAG flag = KEY_Stay)
	{
		if(m_Key[KeyNo] & flag)return true;
		return false;
	}

	// キーボード判定(押した最初の1回目のみ)
	// KeyNo	… 判定するキーコード
	bool KeyCheck_Enter(unsigned char KeyNo)
	{
		return KeyCheck(KeyNo, KEY_Enter);
	}

	// キーボード判定(離した最初の1回目のみ)
	// KeyNo	… 判定するキーコード
	bool KeyCheck_Exit(unsigned char KeyNo)
	{
		return KeyCheck(KeyNo, KEY_Exit);
	}
private:
	BYTE	m_KeyBuffer[256];	// キーバッファ
	DWORD	m_Key[256];			// キーボード用フラグ


//===============================
// マウス
//===============================
public:
	// FPSゲーム用マウスモード
	//  マウス非表示、座標固定でm_MouseMoveValue(getMouseMoveValue関数)に移動量が入る
	void SetFPSMode(HWND hWnd, bool enable);

	// マウスデータ更新
	void UpdateMouseData(HWND hWnd);

	// マウスボタン判定
	// button	… 判定するマウスボタン MOUSE_? で指定
	// flag		… どのタイミングで判定するか
	//   ・KEY_Button	… 押している間
	//   ・KEY_Trigger	… 押した最初の1回目のみ
	//   ・KEY_Release	… 離した最初の1回目のみ
	bool MouseCheck(MOUSEBUTTON button, KEYFLAG flag = KEY_Stay)
	{
		BYTE MouseButton[3] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
		return KeyCheck(MouseButton[button], flag);
	}
	bool MouseCheck_Enter(MOUSEBUTTON button)
	{
		return MouseCheck(button, KEY_Enter);
	}
	bool MouseCheck_Exit(MOUSEBUTTON button)
	{
		return MouseCheck(button, KEY_Exit);
	}

	// マウスホイールスクロール値設定
	void	SetMouseWheelValue(int Val){m_MouseWheel = Val;}

	// FPSマウス操作モードのON/OFF取得
	bool	GetMouseFPSMode() { return m_MouseFPSMode; }
	// マウス座標
	POINT*	GetMousePos(){return &m_MousePos;}
	// １つ前のマウス座標
	POINT*	GetMouseOldPos(){return &m_MouseOldPos;}
	// マウスが動いた値(ピクセル)
	POINT*	GetMouseMoveValue(){return &m_MouseMoveValue;}
	// ホイールを動かした値
	int		GetMouseWheel(){return m_MouseWheel;}

private:
	// Mouse
	POINT	m_MousePos;						// マウス座標
	POINT	m_MouseOldPos;					// １つ前のマウス座標
	POINT	m_MouseDown[3];					// マウスを押した位置
	int		m_MouseWheel;					// ホイールを動かした値
	POINT	m_MouseMoveValue;				// マウスが動いた値
	bool	m_MouseFPSMode = false;

//------------------------------------------------------------------
// シングルトン
private:
	CInput();
public:
	static CInput& GetInstance()
	{
		static CInput instance;
		return instance;
	}
//------------------------------------------------------------------
};

#define INPUT CInput::GetInstance()

#endif