#include "main.h"
#include "TitleScene.h"
#include "../GameScene/GameScene.h"

//=========================================
// �X�V����
//=========================================
int TitleScene::Update()
{
	if(INPUT.KeyCheck_Enter('1')){


		int StageNo = 1;
		// �V�[���؂�ւ�
		SPtr<GameScene> gs(new GameScene(StageNo));
		gs->Init();
		APP.m_NowScene = gs;

		return 1;
	}
	if (INPUT.KeyCheck_Enter('2')){


		int StageNo = 2;
		// �V�[���؂�ւ�
		SPtr<GameScene> gs(new GameScene(StageNo));
		gs->Init();
		APP.m_NowScene = gs;

		return 1;
	}

	// Escape�L�[
	if(INPUT.KeyCheck_Enter(VK_ESCAPE)){
		APP.ExitGame();
	}

	//==========================================
	// ���C�g���X�V
	//	�����̏����e�V�F�[�_���Ŏg�p���܂�
	//==========================================
	// ���s����
	m_DirLight->SetData(
		YsVec3(0.5f, -1, -0.2f),		// ���C�g����
		YsVec4(0.8f, 0.8f, 0.8f, 1)		// ��{�F(Diffuse)
		);

	// ���F(Ambient)
	ShMgr.m_LightMgr.m_AmbientLight.Set(0.2f, 0.2f, 0.2f);

	
	//=========================================================
	// �J��������
	//=========================================================
	m_Cam.Update();

	//=========================================================
	// �L��������
	//=========================================================

	return 0;
}

//=========================================
// �`�揈��
//=========================================
void TitleScene::Draw()
{
	//==========================================
	// �N���A
	//==========================================
	// �o�b�N�o�b�t�@���N���A����B
	YsDx.GetBackBuffer()->ClearRT(YsVec4(0.3f, 0.3f, 0.8f, 1));
	// Z�o�b�t�@�̃N���A
	YsDx.GetDepthStencil()->ClearDepth(true, true);

	// ���������[�h��
	ShMgr.m_bsAlpha.SetState();


	//==========================================
	// �J�����ݒ�
	//==========================================
	m_Cam.SetCamera();

	//==========================================
	// �`�撼�O�ɃV�F�[�_�̃t���[���P�ʃf�[�^���X�V����
	//==========================================
	ShMgr.UpdateBeforeDraw();


	// �`��Ȃǂ�����


	//==========================================
	// [2D]�w�i�`��
	//==========================================
	YsDx.GetSprite().Begin();
	{
		YsDx.GetSprite().Draw(*m_texBack, 0, 0, m_texBack->GetInfo().Width, m_texBack->GetInfo().Height);
	}
	YsDx.GetSprite().End();

	//==========================================
	// [3D]���f���`��
	//==========================================


	// ��������
	std::string text;
	text = "[�^�C�g���V�[��]\n";
	text += "1:Stage1\n";
	text += "2:Stage2\n";
	text += "Alt+Enter:�t���X�N���[���؂�ւ�\n";

	// 2D�`��
	YsDx.GetSprite().Begin(true);
	{
		YsDx.GetSprite().DrawFont(text, 1, 1, &YsVec4(0, 0, 0, 1));
		YsDx.GetSprite().DrawFont(text, 0, 0, &YsVec4(1, 1, 1, 1));

	}
	YsDx.GetSprite().End();

}

//============================================================
// �f�[�^���
//============================================================
void TitleScene::Release()
{
	// �T�E���h��~
	YsSndMgr.StopAll();


	Dw_Scroll(0, "�^�C�g���V�[���������");
}

//============================================================
// �����ݒ�
//============================================================
void TitleScene::Init()
{
	Dw_Scroll(0, "�^�C�g���V�[��������");

	// ���s���C�g�쐬
	m_DirLight = ShMgr.m_LightMgr.AddDirLight();

	// �J�����f�[�^������
	m_Cam.Init(20, 130, -3);
	m_Cam.m_BaseMat.SetPos(0, 1.2f, 0);

	//--------------------------------------------------------
	// �e�N�X�`���ǂݍ���
	//--------------------------------------------------------
	m_texBack = APP.m_ResStg.LoadTexture("data/Texture/title_back.png");

	//--------------------------------------------------------
	//
	// ���f���ǂݍ���
	//  APP.m_ResStg�̑���ɁAYsDx.GetResStg()���g�p���Ă�����(�ǂ���������)
	//
	//--------------------------------------------------------
	

}
