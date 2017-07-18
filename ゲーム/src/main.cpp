//======================================================================================
//
// �Q�[���p��{�t���[��
//
// [�r���h�\��]
//  Debug    �c �f�o�b�O�r���h(���s���x�]���̒��f�o�b�O�D��)
//              <����> �EDirect3D�̃f�o�b�OON�B����Ă̓_���Ȃ��Ƃ��u�o�́v�E�B���h�E�ɕ\�����ċ����Ă����B�������`�悪�{���炢�d���Ȃ�B
//                     �E�V�F�[�_�R���p�C�����ɍœK���t���O��OFF�ɂ��Ă���̂ŁA�r���h������s�܂ł��f�����ł���B
//                     �E�Ƃɂ��������ƕ`�悪�x���̂ŁA�ق�ƂɃo�O��������f�o�b�O�p�B
//
//  Debug2   �c �������[�X + ���f�o�b�O(���s���x�ƃf�o�b�O�@�\�̗���)
//              <����> �EDirect3D�̃f�o�b�OOFF�B�`�悪�����B
//                     �E�V�F�[�_�R���p�C�����ɍœK���t���O��OFF�ɂ��Ă���̂ŁA�r���h������s�܂ł��f�����ł���B
//                     �E_DEBUG�}�N���͓���̂ŁASTL�g�p���̃o�O�Ƃ��͌����₷���Ȃ�܂��B���̂Ԃ���s���x�͂��Ȃ藎����B
//                     �@�����s���x�𑬂��������ꍇ�́APch.h�� #undef _DEBUG �̃R�����g���O���B
//                     �E�u���[�N�|�C���g�ŃX�e�b�v���s�\�B
//                     �EDebug�r���h�قǍׂ��ȃf�o�b�O�͂ł��Ȃ��B
//
//  Release  �c �����[�X�r���h(�ŏI�����[�X�����̒����s���x�D��)
//              <����> �EDirect3D�̃f�o�b�OOFF�B�`�悪�����B
//                     �E�V�F�[�_�R���p�C���̍œK�����ő�ɂ��Ă���̂ŕ`��p�t�H�[�}���X�͏オ�邪�A���s����܂ł��x���B
//                     �@�����̏ꍇ�A�V�F�[�_�͂��炩���߃R���p�C�����ăt�@�C���ɂ��Ă����ׂ��B
//                     �E�u���s���x�D��v�̃r���h�ݒ�ɂ��Ă���̂ŁA�ő�̃p�t�H�[�}���X�����҂ł���B
//
// ��Debug2��Release�ɂ�Direct3D�̃f�o�b�O��ON�ɂ������ꍇ�́APch.h�� #define DEBUG_DIRECT3D11 �̃R�����g���͂����B
// ���f�o�b�O�E�B���h�E�����������ꍇ�́APch.h�� #define DEBUG_WND ���R�����g�A�E�g����B
//======================================================================================
#include "main.h"
#include "Game/Scene/TitleScene/TitleScene.h"
#include "Game\Scene\GameScene\GameScene.h"

static void DebugCommand(std::string cmdText);

//======================================================================================
//���C��
//======================================================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpszArgs,int nWinMode)
{
	// COM������
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// ���������[�N��m�点��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �����ݒ�
	BOOL bRet = APP.InitWindow(hInstance, 1280, 720);

	// ���[�v
	if(bRet){
		APP.Loop();
	}

	// ���
	APP.Release();

	// COM���
	CoUninitialize();

	return 0;
}

//======================================================================================
// �����ݒ�
//======================================================================================
BOOL CApp::InitWindow(HINSTANCE hInstance,int w,int h)
{
	//===============================
	// �f�o�b�O�E�B���h�E�쐬
	//===============================
	#ifdef DEBUG_WND
	DebugWnd.Create(hInstance, 4, DebugCommand);
	#endif

	Dw_Scroll(0, "�E�B���h�E�쐬");
	//===============================
	// ���C���E�B���h�E�쐬
	//===============================
	//�E�B���h�E�N���X�̒�`
	WNDCLASSEX wc;											// �E�B���h�E�N���X�̒�`�p
	wc.cbSize=sizeof(WNDCLASSEX);							// �\���̂̃T�C�Y
	wc.style = 0;											// �X�^�C��
	wc.lpfnWndProc = &CApp::callWindowProc;					// �E�C���h�E�֐�
	wc.cbClsExtra = 0;										// �G�L�X�g���N���X��� 
	wc.cbWndExtra = 0;										// �G�L�X�g���E�B���h�E���
	wc.hInstance = m_hInst;									// �C���X�^���X�n���h��
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);		// ���[�W�A�C�R��
	wc.hIconSm=LoadIcon(hInstance, IDI_APPLICATION);		// �X���[���A�C�R�� 
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);				// �}�E�X�J�[�\��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// �w�i�F 
	wc.lpszMenuName = nullptr;								// ���C�����j���[��
	wc.lpszClassName = "Window";							// �E�B���h�E�N���X��

	//�E�B���h�E�N���X�̓o�^
	if(!RegisterClassEx(&wc)){
		return FALSE;
	}

	//�E�B���h�E�̍쐬
	m_hWnd = CreateWindow(
		"Window",											//�쐬����E�B���h�E��
		"Game",									//�E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW-WS_MAXIMIZEBOX-WS_THICKFRAME,	//�E�B���h�E�^�C�v	
		0,													//�E�B���h�E�̈ʒu�i�w���W�j
		0,													//�E�B���h�E�̈ʒu�i�x���W�j						
		w,													//�E�B���h�E�̕�
		h,													//�E�B���h�E�̍���			
		nullptr,											//�e�E�B���h�E�̃n���h��
		nullptr,											//���j���[�̃n���h��
		m_hInst,											//�C���X�^���X�n���h�� 
		nullptr);											//�ǉ���� 

	// �N���C�A���g�̃T�C�Y��ݒ�
	SetClientSize(m_hWnd,w,h);

	// timeGetTime�֐��̐��x��1ms�ɂ���
	timeBeginPeriod(1);

	//===================================================================
	// Direct3D������
	//===================================================================
	Dw_Scroll(0, "Direct3D�������J�n");
	std::string errMsg;
	if( !YsDx.Init(m_hWnd, w, h, &m_ResStg, YsDx.MSAA_None, &errMsg) ){
		MessageBox(m_hWnd, errMsg.c_str(), "�G���[", MB_OK);
		return FALSE;
	}

	// �V�F�[�_�N���X������
	Dw_Scroll(0, "�V�F�[�_�����J�n");
	ShMgr.Init();

	//===================================================================
	// �T�E���h
	//===================================================================
	Dw_Scroll(0, "�T�E���h�������J�n");
	YsSndMgr.Init(m_hWnd);



	// �f�o�b�O�E�B���h�E������O�ɕ\��
	#ifdef DEBUG_WND
	SetForegroundWindow(m_hWnd);
	#endif

	// ����������
	m_Rnd.InitSeed(timeGetTime());

	// �t���X�N���[��
	//if(IDYES == MessageBox(m_hWnd, "�t���X�N���[���\���ɂ��܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)) {
	//	ShowWindow(m_hWnd, SW_SHOW);
	//	YsDx.SetFullScreen(TRUE);
	//}
	//else{
		ShowWindow(m_hWnd, SW_SHOW);
	//}


	// �����V�[��
	{
		SPtr<TitleScene> ts(new TitleScene());
		ts->Init();
		m_NowScene = ts;
	}

	// 
	Dw_Scroll(1, "==========================");
	Dw_Scroll(1, "help�Ńf�o�b�O�R�}���h�\��");
	Dw_Scroll(1, "==========================");
	DebugCommand("help");

	return TRUE;
}

void CApp::Release()
{
	// �V�[�����
	m_NowScene = nullptr;

	// ���\�[�X���
	m_ResStg.Release();
	// �V�F�[�_���
	ShMgr.Release();
	// Direct3D���
	YsDx.Release();
	// �T�E���h���
	YsSndMgr.Release();

	// �f�o�b�O�E�B���h�E�I��
#ifdef DEBUG_WND
	DebugWnd.Close();
#endif

}

// ���C�����[�v
int CApp::Loop()
{
	MSG msg;			//���b�Z�[�W�\����

	//���b�Z�[�W���[�v
	while(1){
		// ���b�Z�[�W�擾
		if(PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
		{
			// �I�����b�Z�[�W��������A���[�v�I��
			if(msg.message == WM_QUIT) {
				break;
			}

			//=========================================
			// ���b�Z�[�W����
			//=========================================
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// �Q�[������
		else
		{
			//=========================================
			// FPS����
			//=========================================
			// �҂�
			m_FpsCtrl.Wait();
			// FPS�p�̎��ԍX�V
			m_FpsCtrl.Update();
			// FPS�f�o�b�O�\��
			Dw_Static(0, "FPS:%f", m_FpsCtrl.GetNowFps());

			//=========================================
			// �L�[�E�}�E�X���X�V
			//=========================================
			INPUT.UpdateKey();
			INPUT.UpdateMouseData(m_hWnd);

			//=========================================
			// �T�E���h����
			//=========================================
			YsSndMgr.Run();

			//=========================================
			//
			// �V�[���������s
			//
			//=========================================
			{
				//--------------------------------------------
				// �����y���̂��߁A�`�悪30fps�Ŏ��s(������60fps�̂܂�)
				//--------------------------------------------
				m_NonDrawFlag = 0;
				if (m_b60fpsDraw == false) {
					// 2��Ɉ�񂾂��`�悷��
					if (m_FrameCnt % 2 == 0) {
						if (m_NonDrawFlag == 0) {
							m_NonDrawFlag = 1;
						}
					}
				}

				//--------------------------------------------
				// �V�[���X�V����
				//--------------------------------------------
				int ret = m_NowScene->Update();

				//--------------------------------------------
				// �V�[���`�揈��
				//--------------------------------------------
				if(ret == 0 && m_NonDrawFlag == 0) {
					// �V�[���`����s
					m_NowScene->Draw();

					//--------------------------------------------
					// ��ʂɕ\��
					//--------------------------------------------
					YsDx.Present(0, 0);
				}

			}

			//=========================================
			// �f�o�b�O�E�B���h�E
			//=========================================
			#ifdef DEBUG_WND
			DebugWnd.Update();
			#endif

			Dw_Static(40, "Load�ς݃��\�[�X��:%d", YsDx.GetResStg()->GetNodeNum());

			// 
			INPUT.SetMouseWheelValue(0);

			m_FrameCnt++;
		}

	}

	// �v���O�����I��
	return 0;
}

//�E�B���h�E�֐�
LRESULT CALLBACK CApp::callWindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	return APP.WindowProc(hWnd,message,wParam,lParam);
}

LRESULT CApp::WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	//���b�Z�[�W�ɂ���ď�����I��
	switch(message){
		case WM_ACTIVATE:
			{
				// ��A�N�e�B�u
				if(LOWORD(wParam) == WA_INACTIVE){
					m_bWndActive = false;
				}
				// �A�N�e�B�u
				else{
					m_bWndActive = true;
				}
			}
			break;
		// �z�C�[���X�N���[����
		case WM_MOUSEWHEEL:
			{
				INPUT.SetMouseWheelValue((short)HIWORD(wParam));
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			//�s�v�ȃ��b�Z�[�W�̌�n��
			return DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 0;
}

// �E�B���h�E�̃N���C�A���g�̃T�C�Y���w��T�C�Y�ɂ���B
void CApp::SetClientSize(HWND hWnd,int w,int h)
{
	RECT rcWnd,rcCli;

	GetWindowRect(hWnd,&rcWnd); // �E�B���h�E��RECT�擾
	GetClientRect(hWnd,&rcCli); // �N���C�A���g�̈��RECT�擾

	// �E�B���h�E�̗]�����l���āA�N���C�A���g�̃T�C�Y���w��T�C�Y�ɂ���B
	MoveWindow(hWnd,
			rcWnd.left,// X���W
			rcWnd.top,// Y���W
			w + (rcWnd.right-rcWnd.left)-(rcCli.right-rcCli.left),
			h + (rcWnd.bottom-rcWnd.top)-(rcCli.bottom-rcCli.top),
			TRUE);
}

#ifdef DEBUG_WND

// �f�o�b�O�E�B���h�E�p �R�}���h����
//  �f�o�b�O�E�B���h�E���ɂ�����̓{�b�N�X�œ��͂��ꂽ���e�������ɗ���
//  �f�o�b�O��p�̏����Ȃǂɗ��p���Ă�������
static void DebugCommand(std::string cmdText)
{
	// ��)exit�Ɠ��͂���Enter�������ƏI��
	if(cmdText == "exit"){
		APP.ExitGame();
	}
	else if(cmdText == "reload shader"){
		ShMgr.Release();
		ShMgr.Init();
		Dw_Scroll(1, "�V�F�[�_�ēǍ� ����");
	}
	else if(cmdText == "save shader"){
		ShMgr.SaveCsoToFile();
		MessageBox(APP.m_hWnd, "hlsl�t�@�C���Ɠ����ꏊ��cso�t�@�C����ۑ����܂���", "�V�F�[�_�ۑ�", MB_OK);
		MessageBox(APP.m_hWnd, "�����Ӂ�\ncso�����݂���ꍇ�Ahlsl�t�@�C���͓ǂݍ��܂�Ȃ��Ȃ�\ncso���ǂݍ��܂��悤�ɂȂ�܂��B\n�ēxhlsl��ǂݍ��݂����ꍇ��cso�t�@�C�����폜���Ă�������", "�V�F�[�_�ۑ�", MB_OK);
	}
	else if(cmdText == "resource"){
		Dw_Scroll(1, "<<�Ǎ��ς݃��\�[�X�ꗗ>>");
		std::vector<std::string> res;
		APP.m_ResStg.DEBUG_GetNowResources(res);
		for(auto& str : res){
			Dw_Scroll(1, "%s", str.c_str());
		}
	}
	else if (cmdText.compare(0, 14, "change maxfps ") == 0) {
		char tmp[1024];
		char tmp2[1024];
		int fps;
		sscanf(cmdText.c_str(), "%s %s %d", tmp, tmp2, &fps);

		if (fps > 0) {
			APP.m_FpsCtrl.SetMaxFps(fps);
			Dw_Scroll(1, "FPS�ύX(Max Fps = %d)", fps);
		}
		else {
			Dw_Scroll(1, "������FPS�l�ł�");
		}
	}
	else if(cmdText == "fps limit on"){
		Dw_Scroll(1, "FPS����ON");
		APP.m_FpsCtrl.SetEnableWait( true );
	}
	else if(cmdText == "fps limit off"){
		Dw_Scroll(1, "FPS����OFF");
		APP.m_FpsCtrl.SetEnableWait( false );
	}
	else if(cmdText == "draw 60fps"){
		Dw_Scroll(1, "60FPS�`�惂�[�h");
		APP.m_b60fpsDraw = true;
	}
	else if(cmdText == "draw 30fps"){
		Dw_Scroll(1, "30FPS�`�惂�[�h");
		APP.m_b60fpsDraw = false;
	}
	else if(cmdText == "help"){
		Dw_Scroll(1, "<<�R�}���h�ꗗ>>");
		Dw_Scroll(1, "help �c ���̐�������\��");
		Dw_Scroll(1, "exit �c �I��");
		Dw_Scroll(1, "reload shader �c �V�F�[�_�ēǍ�");
		Dw_Scroll(1, "save shader �c �R���p�C���ς݂̃V�F�[�_(cso)���t�@�C���ɕۑ�");
		Dw_Scroll(1, "resource �c �Ǎ��ς݃��\�[�X�ꗗ");
		Dw_Scroll(1, "change maxfps ���l �c �ő�FPS�l��ύX");
		Dw_Scroll(1, "fps limit on �c FPS����ON");
		Dw_Scroll(1, "fps limit off �c FPS����OFF");
		Dw_Scroll(1, "draw 60fps �c 60fps�`�惂�[�h");
		Dw_Scroll(1, "draw 30fps �c 30fps�`�惂�[�h");
	}
}


#endif
