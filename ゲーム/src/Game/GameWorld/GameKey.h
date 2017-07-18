#pragma once

//======================================================
// ゲーム専用キー定数
//======================================================
enum {
	GK_LEFT = 0x00000001,
	GK_UP = 0x00000002,
	GK_RIGHT = 0x00000004,
	GK_DOWN = 0x00000008,
	GK_A = 0x00000010,
	GK_B = 0x00000020,
	GK_X = 0x00000040,
	GK_Y = 0x00000080,
	GK_L1 = 0x00000100,
	GK_L2 = 0x00000200,
	GK_R1 = 0x00000400,
	GK_R2 = 0x00000800,
	GK_LLEFT = 0x00001000,
	GK_LUP = 0x00002000,
	GK_LRIGHT = 0x00004000,
	GK_LDOWN = 0x00008000,
	GK_START = 0x00010000,
};
//======================================================
// ゲーム入力基本クラス
//======================================================
class GameInputComponent {
public:
	virtual ~GameInputComponent() {}
	//======================================================
	// 処理
	//======================================================
	// 戻り値：押下キーの情報
	virtual int Update() { return 0; }
	GameInputComponent() {
		m_bEnable = true;
	}
	bool m_bEnable; // 入力有効
};
//======================================================
// キーボード入力コンポーネント
//======================================================
class KeyInputComponent : public GameInputComponent {
public:
	// キーボードの状態からゲームキーを返す
	virtual int Update() {
		if (!m_bEnable)return 0;
		int keys = 0;
		// キーボード → ゲームキー作成
		if (INPUT.KeyCheck(VK_LEFT)) keys |= GK_LEFT;
		if (INPUT.KeyCheck(VK_UP)) keys |= GK_UP;
		if (INPUT.KeyCheck(VK_RIGHT)) keys |= GK_RIGHT;
		if (INPUT.KeyCheck(VK_DOWN)) keys |= GK_DOWN;
		if (INPUT.KeyCheck(VK_SPACE)) keys |= GK_A;
		if (INPUT.KeyCheck('X')) keys |= GK_B;
		if (INPUT.MouseCheck(INPUT.MOUSE_L)) keys |= GK_X;
		if (INPUT.KeyCheck('V')) keys |= GK_Y;
		if (INPUT.KeyCheck('A')) keys |= GK_L1;
		if (INPUT.KeyCheck('S')) keys |= GK_L2;
		if (INPUT.KeyCheck('D')) keys |= GK_R1;
		if (INPUT.KeyCheck('A')) keys |= GK_LLEFT;
		if (INPUT.KeyCheck('W')) keys |= GK_LUP;
		if (INPUT.KeyCheck('D')) keys |= GK_LRIGHT;
		if (INPUT.KeyCheck('S')) keys |= GK_LDOWN;
		return keys;
	}
};