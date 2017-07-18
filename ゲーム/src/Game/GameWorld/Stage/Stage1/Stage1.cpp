#include "main.h"

#include "../../GameWorld.h"

#include "Game\GameWorld\CollisionEngine\CollisionEngine.h"

#include "Stage1.h"

#include "Game\Edit\EditMode.h"
//#include "Game\GameWorld\Effect\Effect.h"
//#include "Game\GameWorld\Item\Weapon.h"



void GameWorld::Init()
{
	// �����G���W��������
	m_Bullet.Init();

	// �J�����f�[�^������
	m_Cam.Init(20, 0, -5);

	// �t�F�[�h
	//m_BlackCurtainAdd = -0.01f;
	//m_BlackCurtainPower = 1.0f;

	// ���f���ǂݍ���
	// �e�N�X�`���ǂݍ���
	// �X�e�[�W������

	//+++++++++++++++++++++++++++++++++++++++++
	// �I�u�W�F�N�g�𐶐�����
	//+++++++++++++++++++++++++++++++++++++++++
	{
		//�O������Q�[���I�u�W�F�N�g��ǂݍ���
		UPtr<DataManager> uDM(new DataManager);

		SPtr<CharacterBase> p = uDM->CsvDataLoad("data/txt/Object.csv");

		// �J�������_�L�����Ƃ��ċL��
		m_wpCamera = p;

		// �v���C���[�Ƃ��ċL��
		m_wpPlayer = p;
		
		// ���̂�z�u����L�����ݒ� 
		m_Put.SetPlayer(p);


		//����
		SPtr<Weapon_Hammer>h(new Weapon_Hammer);
		h->Init(p, static_cast<Player*>(p.get())->GetModelObject().SearchBone("�E��_�~�["), nullptr);//
		static_cast<Player*>(p.get())->SetWeapon(h);
		BaseWeapon::PushObject(h);

	}
	{
		//���쐬
		/*for (int i = -10; i <= 10; i++){
			for (int j = -10; j <= 10; j++){
				SPtr<Box>box(new Box);
				YsMatrix m;
				m.CreateMove((float)i, 0, (float)j);
				box->Init(m);
				CharacterBase::PushObject(box);
			}
		}*/
	}
	{
		////��
		//for (int i = 0; i < 15; i++){
		//	SPtr<Water>w(new Water);
		//	YsMatrix m;
		//	m.CreateMove(0, 5.0f+(float)i*2.0f, 3.0f);
		//	w->Init(m, CharaID::WATER);
		//	CharacterBase::PushObject(w);
		//}
	}

	//Edit����
	EditMode::CreateWorld();
	//�J���Ҍ����̃}�b�v�G�f�B�^
	EW.MapInit();

	NAI.Init();
}

void GameWorld::Release()
{
	m_Bullet.Release();
	m_wpCamera.reset();
	m_wpPlayer.reset();

	//�O������Q�[���I�u�W�F�N�g��ǂݍ���
	//UPtr<DataManager> uDM(new DataManager);
	//uDM->CsvDataSave("data/txt/Object.csv");

	CharacterBase::FullClear();

	EditMode::DeleteWorld();


	BaseEffectObj::FullClear();
	BaseWeapon::FullClear();
}

void GameWorld::Update()
{
	// �|�[�YOn/Off
	if (INPUT.KeyCheck_Enter('P')){
		m_PauseFlag = !m_PauseFlag;
	}

	// �����蔻��f�o�b�O�\��On/Off
	if (INPUT.KeyCheck_Enter('H')){
		m_Debug_DrawHit = !m_Debug_DrawHit;
	}

	//=========================================================
	// �Q�[������
	//=========================================================
	if (m_PauseFlag == false){
		// �����G���W������
		const int simuCnt = 1;
		m_Bullet.StepSimulation(1.0f / 60, simuCnt, 1.0f / 60 / simuCnt);

			
		// �����蔻�胊�X�g�N���A
		m_ColEng.ClearList();

	
		// ���[�h�؂�ւ�

		if (INPUT.KeyCheck_Enter('E')){
			m_EditFlg = EDIT;
		}
		if (INPUT.KeyCheck_Enter('M')){
			m_EditFlg = CREATE;
		}
		if (INPUT.KeyCheck_Enter('N')){
			m_EditFlg = NORMAL;
		}

		// Update����

		if (m_EditFlg == EDIT){
			EditMode::Edit = true;
			EW.CamUpdate();
			EW.EditUpdate();
		}
		else if(m_EditFlg==CREATE){
			EditMode::Edit = false;
			EW.CamUpdate();
			EW.MapUpdate();
		}
		else if(m_EditFlg==NORMAL){
			CharacterBase::AllUpdate();
			BaseEffectObj::AllUpdate();
			
			m_Put.Update();
		}

		// �^�X�N���N���A���鏈��

		CharacterBase::CheckKillFlg();



		// �����蔻����s
		Dw_TimeBegin("HitTest"); // �������Ԍv��(�����Ă��ǂ�)
		m_ColEng.Run();
		Dw_TimeEnd("HitTest", 2); // �������Ԍv��(�����Ă��ǂ�)
	}
	// �|�[�Y��
	else{
		// �J��������
		m_Cam.Update();
	}


	// �Ö�����
	m_BlackCurtainPower += m_BlackCurtainAdd;
	if (m_BlackCurtainPower > 1)m_BlackCurtainPower = 1;
	if (m_BlackCurtainPower < 0)m_BlackCurtainPower = 0;

}

void GameWorld::Draw()
{
	//==========================================
	// �J�����ݒ�
	//==========================================
	// �|�[�Y���H
	if (m_PauseFlag == false){


		//----------------���_�J�������Z�b�g����----------------
		if (m_EditFlg == NORMAL){

			if (m_wpCamera.expired() == false){
				m_wpCamera.lock()->SetCamera();
			}

		}
		else
		// Edit�J����
		if (m_EditFlg == EDIT || m_EditFlg == CREATE){
			EW.SetCamera();
		}
		//--------------------------------------------------


	}
	// �|�[�Y���Ȃ�AGameWorld�̃J�������g�p
	else{
		m_Cam.SetCamera();
	}

	//==========================================
	// ���C�g�ݒ�
	//==========================================
	ShMgr.m_Samp.SetLightEnable(false); // ���C�g����

	//==========================================
	// �`�撼�O�ɃV�F�[�_�̃t���[���P�ʃf�[�^���X�V����
	//==========================================
	ShMgr.UpdateBeforeDraw();


	// Edit�Ő��������I�u�W�F�N�g�̕`��

	if (m_EditFlg != NORMAL){
		EW.EditDraw();
	}

	//------------�L�����`��------------

	//�L����
	CharacterBase::AllDraw();
	
	//�v���C���[�Ŕz�u
	m_Put.Draw();

	//�G�t�F�N�g
	BaseEffectObj::AllDraw();

	if (INPUT.KeyCheck('L')) {
		NAI.DebugWayDraw();
	}

	//==========================
	// �����蔻��f�o�b�O�\��
	//==========================
	if (m_Debug_DrawHit){
		ShMgr.m_bsAlpha.SetState();
		//YsDx.DS_ZSetting(false, false);
		m_ColEng.DebugDraw(0.7f);
		//YsDx.DS_ZSetting(true, true);
	}

	// �Ö��`��
	if (m_BlackCurtainPower > 0){
		YsDx.GetSprite().Draw(*YsDx.GetWhiteTex(), 0, 0, YsDx.GetRezoW(), YsDx.GetRezoH(), &YsVec4(0, 0, 0, m_BlackCurtainPower));
	}

	// ��������
	std::string text;
	text += "------- �V�X�e������ -------\n";
	text += "P:�|�[�Y\n";
	text += "H:�����蔻��f�o�b�O�\��\n";
	text += "E: �G�f�B�b�g���[�h\n";
	text += "M: �}�b�v�N���G�C�g\n";
	text += "N: �ʏ�ɖ߂�\n";
	text += "RETURN: �v���C���[�Ŕz�u\n";
	text += "\n";

	// 2D�`��
	YsDx.GetSprite().Begin(true);
	{
		YsDx.GetSprite().DrawFont(text, 1, 1, &YsVec4(0, 0, 0, 1));
		YsDx.GetSprite().DrawFont(text, 0, 0, &YsVec4(1, 1, 1, 1));
	}
	YsDx.GetSprite().End();

}