#include "main.h"
#include "../TitleScene/TitleScene.h"
#include "GameScene.h"

#include "Game/GameWorld/GameWorld.h"

#include "Game\GameWorld\Stage\Stage1\Stage1.h"
#include "Game\GameWorld\Stage\Stage2\Stage2.h"


SPtr<Stage> GameScene::m_Stage;

//============================================================
// �X�V����
//============================================================
int GameScene::Update()
{

	// Escape�L�[
	if(INPUT.KeyCheck_Enter(VK_ESCAPE)){
		// �V�[���؂�ւ�
		SPtr<TitleScene> ts(new TitleScene());
		ts->Init();
		APP.m_NowScene = ts;

		//�I��
		//APP.ExitGame();

		return 1;
	}




	// �Q�[�����[���h����
	m_Stage->Update();




	return 0;
}

//============================================================
// �`�揈��
//============================================================
void GameScene::Draw()
{
	//==========================================
	// �N���A
	//==========================================
	// �o�b�N�o�b�t�@���N���A����B
	YsDx.GetBackBuffer()->ClearRT(YsVec4(0.3f, 0.3f, 0.3f, 1));
	// Z�o�b�t�@�̃N���A
	YsDx.GetDepthStencil()->ClearDepth(true, true);

	// ���������[�h��
	ShMgr.m_bsAlpha.SetState();


	// �Ƃ肠����XYZ�������`�悵�Ƃ�
	ShMgr.m_Samp.DrawLine(YsVec3(0, 0, 0), YsVec3(1, 0, 0), &YsVec4(1, 0, 0, 1));
	ShMgr.m_Samp.DrawLine(YsVec3(0, 0, 0), YsVec3(0, 1, 0), &YsVec4(0, 1, 0, 1));
	ShMgr.m_Samp.DrawLine(YsVec3(0, 0, 0), YsVec3(0, 0, 1), &YsVec4(0, 0, 1, 1));


	// �`��Ȃǂ�����



	// �Q�[�����[���h�`��
	m_Stage->Draw();





}

//============================================================
// �f�[�^���
//============================================================
void GameScene::Release()
{
	Dw_Scroll(0,"�Q�[���V�[���������");

	m_Stage->Release();
	m_Stage=nullptr;
}

//============================================================
// �����ݒ�
//============================================================
void GameScene::Init()
{
	Dw_Scroll(0,"�Q�[���V�[��������");

	//============================================================
	// �f�[�^�ǂݍ��݂Ȃ�
	//============================================================

	if (m_StageNo == 1){

		//GameWorld::CreateWorld();
		//GW.Init();

		m_Stage = std::make_shared<GameWorld>();
		m_Stage->Init();

	}
	else if (m_StageNo == 2){

		m_Stage = std::make_shared<Stage2>();
		m_Stage->Init();

	}
	else
	{
		m_Stage = std::make_shared<GameWorld>();
		m_Stage->Init();
	}

}


