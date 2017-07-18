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
// App�N���X
//  �A�v���P�[�V�����̃O���[�o���I�ȃN���X
//=====================================================
class CApp {
// �����o
public:
	//=================================================
	// �E�B���h�E�֌W
	//=================================================
	HINSTANCE		m_hInst;			// �A�v���P�[�V�����̃C���X�^���X�n���h��
	HWND			m_hWnd;				// �E�B���h�E�n���h��
	DWORD			m_FrameCnt;			// ���݂̃t���[���l
	bool			m_bWndActive;		// �E�B���h�E���A�N�e�B�u���H

	//=================================================
	// FPS����֌W
	//=================================================
	YsFpsControl	m_FpsCtrl;			// FPS����N���X

	//=================================================
	// �`��ݒ�
	//=================================================
	bool			m_b60fpsDraw;		// �`���60fps�ɂ���@false����30fps(���`��̂݁@������60fps�ł�)
	int				m_NonDrawFlag;		// �`������Ȃ��悤�ɂ���t���O 0:�`�悷�� 0�ȊO:�`�悵�Ȃ�(�����Ő؂�ւ��܂�)


	//============================================================
	// ���̑�
	//============================================================
	// ���݂̃V�[��
	SPtr<BaseScene>			m_NowScene;

	// �V�F�[�_�֌W���܂Ƃ߂��N���X
	ShaderManager			m_ShMgr;

	// ����
	YsMTRand				m_Rnd;

	// ���\�[�X�Ǘ��N���X
	//  �e�N�X�`����3D���f����T�E���h�ȂǁA���\�[�X�̓ǂݍ��݂��Ǘ�����
	//  ��������g�p���邱�ƂŁA�����t�@�C�����̃��\�[�X�̏d���ǂݍ��݂��h���܂�
	GameResourceStorage		m_ResStg;














	//============================================================
	// 
	//============================================================
	// �E�B���h�E������
	BOOL InitWindow(HINSTANCE hInstance,int w,int h);	// �E�B���h�E�쐬�ADirect3D�������Ȃ�
	void Release();

	// ���C�����[�v
	int Loop();

	// �E�B���h�E�֐�
	static LRESULT CALLBACK callWindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

	// �Q�[���I��
	void ExitGame(){
		DestroyWindow(m_hWnd);
	}

	// �N���C�A���g�T�C�Y���w��T�C�Y�ɂ���
	void SetClientSize(HWND hWnd, int w, int h);

//============================================================
// �V���O���g��
//============================================================
public:
	static CApp &GetInstance(){
		static CApp Instance;
		return Instance;
	}

private:
	// �R���X�g���N�^
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
