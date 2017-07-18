#include "main.h"

CInput::CInput()
{
	ZeroMemory(m_KeyBuffer,sizeof(m_KeyBuffer));
	ZeroMemory(m_Key,sizeof(m_Key));

	ZeroMemory(&m_MousePos,sizeof(m_MousePos));
	ZeroMemory(&m_MouseOldPos,sizeof(m_MouseOldPos));
	ZeroMemory(&m_MouseMoveValue,sizeof(m_MouseMoveValue));
	ZeroMemory(m_MouseDown,sizeof(m_MouseDown));
	
	m_MouseWheel = 0;
}

void CInput::UpdateKey()
{
	GetKeyboardState(m_KeyBuffer);		// キー取得
	for(UINT i=0;i<256;i++){
		// 指定のビットはクリア
		m_Key[i] &= ~KEY_Enter;
		m_Key[i] &= ~KEY_Exit;

		// フラグ
		if(m_KeyBuffer[i] & 0x80){
			if(!(m_Key[i] & KEY_Stay)){
				m_Key[i] |= KEY_Enter;
				m_Key[i] |= KEY_Stay;
			}
		}
		else{
			if(m_Key[i] & KEY_Stay){
				m_Key[i] |= KEY_Exit;
				m_Key[i] &= ~KEY_Stay;
			}
		}
	}
}

void CInput::SetFPSMode(HWND hWnd, bool enable)
{
	if (m_MouseFPSMode == enable)return;
	
	m_MouseFPSMode = enable;
	
	if (m_MouseFPSMode) {
		ShowCursor(FALSE);

		RECT rcCli;
		GetClientRect(hWnd, &rcCli);

		m_MouseOldPos.x = rcCli.right / 2;
		m_MouseOldPos.y = rcCli.bottom / 2;

		POINT pos = m_MouseOldPos;
		ClientToScreen(hWnd, &pos);
		SetCursorPos(pos.x, pos.y);
	}
	else {
		ShowCursor(TRUE);
	}
}

void CInput::UpdateMouseData(HWND hWnd)
{
	// マウス処理
	if (m_MouseFPSMode == false) {
		m_MouseOldPos = m_MousePos;	// マウス履歴
	}
	else {
	}
	GetCursorPos(&m_MousePos);
	ScreenToClient(hWnd, &m_MousePos);

	// マウス移動値
	m_MouseMoveValue.x = m_MousePos.x - m_MouseOldPos.x;
	m_MouseMoveValue.y = m_MousePos.y - m_MouseOldPos.y;

	if (m_MouseFPSMode) {
		POINT pos = m_MouseOldPos;
		ClientToScreen(hWnd, &pos);
		SetCursorPos(pos.x, pos.y);
	}

}
