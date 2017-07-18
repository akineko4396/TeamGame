#include "main.h"
#include "SampleScene.h"
#include "../TitleScene/TitleScene.h"

SampleScene *SampleScene::s_pScene = nullptr;

//=========================================
// �X�V����
//=========================================
int SampleScene::Update()
{
	// Escape�L�[
	if(INPUT.KeyCheck_Enter(VK_ESCAPE)){
		// �V�[���؂�ւ�
		SPtr<TitleScene> ts(new TitleScene());
		ts->Init();
		APP.m_NowScene = ts;

		return 1;
	}

	//=========================================================
	// Debug����
	//=========================================================
	Dw_Static(2, "Task��:%d", m_TaskMgr.GetList().size());
	Dw_Static(3, "EffectTask��:%d", m_EffectTaskMgr.GetList().size());


	//==========================================
	// ���C�g���X�V
	//	�����̏����e�V�F�[�_���Ŏg�p���܂�
	//==========================================
	// ���s����
	m_DirLight->SetData(
		YsVec3(0.5f, -1, 0.2f),		// ���C�g����
		YsVec4(m_LightPower, m_LightPower, m_LightPower, 1)			// ��{�F(Diffuse)
		);

	// ���F(Ambient)
	ShMgr.m_LightMgr.m_AmbientLight.Set(0.1f, 0.1f, 0.1f);


	//=========================================================
	// �J��������
	//=========================================================
	m_Cam.Update();

	// �ړ�
	if (INPUT.KeyCheck('W')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetZAxis() * 0.03f);
	}
	if (INPUT.KeyCheck('S')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetZAxis() * -0.03f);
	}
	if (INPUT.KeyCheck('A')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetXAxis() * -0.03f);
	}
	if (INPUT.KeyCheck('D')) {
		m_Cam.m_BaseMat.Move(m_Cam.mCam.GetXAxis() * 0.03f);
	}
//	m_Cam.m_BaseMat.Move(m_Cam.mCam.GetYAxis() * (float)INPUT.GetMouseWheel() * 0.002f);


	//=========================================================
	// �|�[�Y���łȂ����̏���
	//=========================================================
	if(m_PauseFlag == false){

		//=========================================================
		// �}�E�X���{�^���_�E��
		//=========================================================
		if(INPUT.MouseCheck_Enter(CInput::MOUSE_L))
		{
			// �T�E���h�쐬�e�X�g
			if(m_sndSE){
				m_sndSE->CreateInstance()->Play();
			}

			// ���W
			YsMatrix mat = YsCamera::LastCam.mCam;

			// �傫��
			mat.Scale_Local(APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1));

			// ����
			float mass = (mat.GetXScale() + mat.GetYScale() + mat.GetZScale()) * 10;	// �傫������K���Ɍ��߂�

			// BOX���̃L�����쐬
			SPtr<Task_RigidBox> add = m_TaskMgr.CreateTask<Task_RigidBox>();
			add->Init(
				m_Bullet,
				m_xBox,
				mat,	// �J�������W���甭��
				YsVec3(1.0f, 1.0f, 1.0f),				// BOX�̑傫��
				mass);										// ����
			// �J���������֗͂�������
			YsVec3 vZ = YsCamera::LastCam.mCam.GetZAxis();
			vZ *= 500;
			add->GetRigidBox().ApplyImpulse(vZ);
		}

		//==========================================
		// ��莞�Ԃ�BOX���̃L�����^�X�N�ǉ�
		//==========================================
		if(APP.m_FrameCnt % 30 == 0)
//		if(0)
		{
			// �n�[�t�T�C�Y
			YsVec3 halfSize(1, 1, 1);

			// ���W
			YsMatrix mat;
			mat.CreateMove(APP.m_Rnd.GetFloat(-10, 10), 10, 0);

			// �傫��
			mat.Scale_Local(APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1), APP.m_Rnd.GetFloat(0.1f, 1));

			// ����
			float mass = (mat.GetXScale() + mat.GetYScale() + mat.GetZScale()) * 10;	// �傫������K���Ɍ��߂�

			// BOX�L�����쐬
			SPtr<Task_RigidBox> add = m_TaskMgr.CreateTask<Task_RigidBox>();
			add->Init(m_Bullet, m_xBox, mat, halfSize, mass);
		}

		//=========================================================
		// BOX���̂ɗ͂�������
		//=========================================================
		if(INPUT.KeyCheck_Enter('F')){
			// �S�^�X�N
			auto it = m_TaskMgr.GetList().begin();
			while(it != m_TaskMgr.GetList().end()){
				// BOX���̃N���X�̂ݑI�яo��
				SPtr<Task_RigidBox> pRigidTask = SPtr_dynamic_cast<Task_RigidBox>((*it));
				if(pRigidTask){
					// ������ɗ͂�������
					pRigidTask->GetRigidBox().ApplyImpulse(YsVec3(0, 300, 0));
				}

				++it;
			}
		}

		//=========================================================
		// �������E��i�s������
		//=========================================================
		Dw_TimeBegin("�������Z");
		m_Bullet.StepSimulation(1.0f / 60, 1);
		Dw_TimeEnd("�������Z", 8);



		//=========================================================
		// �S�^�X�N�������s
		//=========================================================
		m_TaskMgr.UpdateTask();
		m_EffectTaskMgr.UpdateTask();


		// �X�|�b�g���C�g��]
		{
			YsMatrix mR;
			mR.CreateRotateY(3);
			m_SL->Direction.TransformNormal(mR);
		}

	}

	// ���邳
	if(INPUT.KeyCheck(VK_UP)){
		m_LightPower += 0.02f;
		if(m_LightPower > 1)m_LightPower = 1;
	}
	if(INPUT.KeyCheck(VK_DOWN)){
		m_LightPower -= 0.02f;
		if(m_LightPower < 0)m_LightPower = 0;
	}

	if (INPUT.KeyCheck_Enter('1')) {
		m_ShowPointLoght = !m_ShowPointLoght;
	}

	// �|�[�Y
	if(INPUT.KeyCheck_Enter('P')){
		m_PauseFlag = !m_PauseFlag;
	}

	return 0;
}

//=========================================
// �`�揈��
//=========================================
void SampleScene::Draw()
{
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



	//==========================================
	// �N���A
	//==========================================
	// �o�b�N�o�b�t�@���N���A����B
	YsDx.GetBackBuffer()->ClearRT(YsVec4(m_LightPower + 0.1f, m_LightPower + 0.1f, m_LightPower + 0.3f, 1));
	// Z�o�b�t�@�̃N���A
	YsDx.GetDepthStencil()->ClearDepth(true, true);


	// �`��Ȃǂ�����



	//==========================================
	// [2D]�w�i�`��
	//==========================================





	//==========================================
	// [3D]���f���`��
	//==========================================

	// �t�H�O�ݒ�
	ShMgr.m_Samp.SetFog(true, &YsVec3(m_LightPower, m_LightPower, m_LightPower), &YsVec2(0, 500));

	// �S�^�X�N�`����s
	//  (�l�ԁA��s�@�A��)
	m_TaskMgr.DrawTask();

	// �X�e�[�W�}�b�v�`��
	ShMgr.m_Samp.DrawModel(m_moMap, &m_mMap);



	//�@�|�C���g���C�g�͈̓f�o�b�O�\��
	if(m_ShowPointLoght){
		for (auto& node : ShMgr.m_LightMgr.m_PointLightList) {
			SPtr<PointLight> PL = node.lock();
			if (PL) {
				YsMatrix m;
				m.CreateMove(PL->Position);
				ShMgr.m_Samp.DrawSphereLine(PL->Radius, &YsVec4(1,0,0,1), &m);

				ShMgr.m_Samp.DrawLine(PL->Position + YsVec3(0, -0.2f, 0), PL->Position + YsVec3(0, 0.2f, 0), &YsVec4(1, 1, 0, 1));
				ShMgr.m_Samp.DrawLine(PL->Position + YsVec3(-0.2f, 0, 0), PL->Position + YsVec3(0.2f, 0, 0), &YsVec4(1, 1, 0, 1));
				ShMgr.m_Samp.DrawLine(PL->Position + YsVec3(0, 0, -0.2f), PL->Position + YsVec3(0, 0, 0.2f), &YsVec4(1, 1, 0, 1));
			}
		}
	}

	// �G�t�F�N�g�^�X�N�`��
	YsDx.DS_ZSetting(true, false);			// Z�o�b�t�@��������OFF
	YsDx.RS_CullMode(D3D11_CULL_NONE);		// ���ʕ`�惂�[�h
		m_EffectTaskMgr.DrawTask();
	YsDx.DS_ZSetting(true, true);
	YsDx.RS_CullMode(D3D11_CULL_BACK);

	ShMgr.m_bsAlpha.SetState();

	// ��������
	std::string text;
	text = "[�T���v���V�[��]\n";
	text += "P:�|�[�Y(�X�V��~ �`��̂�)\n";
	text += "F:�������̂ɗ͂�������\n";
	text += "���N���b�N:���̔��� & �T�E���h�Đ�\n";
	text += "�㉺�L�[:���s�����̖��邳�ύX\n";
	text += "\n";
	text += "�z�C�[��:�㉺�ړ� �h���b�O:���s�ړ�\n";
	text += "WASD:�O�㍶�E�ړ�\n";
	text += "\n";
	text += "Escape:�^�C�g���V�[���֖߂�\n";
	text += "Alt+Enter:�t���X�N���[���؂�ւ�\n";
	text += "1:�|�C���g���C�g�̃f�o�b�O�\��\n";


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
void SampleScene::Release()
{
	// �T�E���h��~
	YsSndMgr.StopAll();

	// �^�X�N�}�l�[�W�����
	m_TaskMgr.Release();

	m_btMap.Release();
	m_Bullet.Release();

	Dw_Scroll(0, "�T���v���V�[���������");
}
#include <thread>

//============================================================
// �����ݒ�
//============================================================
void SampleScene::Init()
{
	Dw_Scroll(0, "�T���v���V�[��������");

	// ���C�g
	m_DirLight = ShMgr.m_LightMgr.AddDirLight();

	s_pScene = this;

	//=================================================================================

	// ���[�h�I���t���O
	// ���A�g�~�b�N�ϐ��Ȃ̂ŁA���݂��̃X���b�h���瑀��E���肵�Ă����S���ۏႳ���
	// �����̕��A���ʂ̕ϐ��ɔ�ׂ���x����
	std::atomic<bool> endFlag = false;

	// ���[�h�X���b�h�J�n
	// ���ʃX���b�h�ŉғ�
	// ���`��n�͍s���Ă̓_��
	std::thread loadThread(
	[&]()
	{

		// �������E������
		m_Bullet.Init();

		//============================================================
		// �f�[�^�ǂݍ��݂Ȃ�
		//============================================================


		// �T�E���h
		m_sndSE = YsDx.GetResStg()->LoadSound("data/Sound/se1.wav", false);


		//==============================================
		// ���f���ǂݍ���
		//==============================================
		// �ȈՃo�[�W����
		//  ���P����YsGameModel�^�̃��������m�ۂ��A���f����ǂݍ���
		//  �����\�[�X�Ǘ��͂��Ă��Ȃ��̂ŁA�����t�@�C�����̃��f����ǂݍ��񂾏ꍇ�A�d�����ēǂݍ���ł��܂�
		//			m_pSm = std::make_shared<YsGameModel>();
		//			m_pSm->LoadMesh("data/model/Hackadoll_1/model.xed");

		// ���\�[�X�q�Ɏg�p�o�[�W����(��������)
		//  �����Ȃ��t�@�C�����̃��f�����d�����ēǂݍ��܂Ȃ��悤�ɊǗ����Ă���
		m_pSm = YsDx.GetResStg()->LoadMesh("data/model/Hackadoll_1/model.xed");
		m_pSm2 = YsDx.GetResStg()->LoadMesh("data/model/G-chan/model.xed");
		m_xF15 = YsDx.GetResStg()->LoadMesh("data/model/F15J/F15J.x");
		m_xSu27 = YsDx.GetResStg()->LoadMesh("data/model/Su-27/Su-27.x");
		m_xBox = YsDx.GetResStg()->LoadMesh("data/model/box/box.x");
		m_xMap = YsDx.GetResStg()->LoadMesh("data/model/map3/map.x");

		//==============================================
		// �e�N�X�`���ǂݍ���
		//==============================================
		m_texBack.LoadTexture("data/Texture/title_back.png");		// �P���ɓǂݍ��ݕ��@(�����̃t�@�C���ł��֌W�Ȃ��ɏd�����ēǂݍ���ł��܂�)
		m_texBack2 = YsDx.GetResStg()->LoadTexture("data/Texture/title_back.png");	// ���\�[�X�q�ɂ��g�p���ēǂݍ���(�����̃t�@�C���͏d���ǂݍ��݂�h����)
		m_tsLaser.LoadTextureSet("data/Texture/Effect/Laser3.png");
		m_tsExplode.LoadTextureSet("data/Texture/Effect/Explode.png");

		//==============================================
		// �L�����P�̍쐬�E�ݒ�
		{
			YsMatrix m;

			SPtr<Task_Human> add = m_TaskMgr.CreateTask<Task_Human>();
			add->Init(m_pSm, m);
		}
		// �L�����Q�̍쐬�E�ݒ�
		{
			YsMatrix m;
			m.CreateMove(1, 0, 0);

			SPtr<Task_Human> add = m_TaskMgr.CreateTask<Task_Human>();
			add->Init(m_pSm2, m);
		}

		// �L�����R�̍쐬�E�ݒ�
		{
			YsMatrix m;
			m.CreateMove(0, 0, 2);
			m.RotateY_Local(135);

			SPtr<Task_Human> add = m_TaskMgr.CreateTask<Task_Human>();
			add->Init(m_pSm, m);
		}

		// �X�e�[�W�}�b�v�̐ݒ�
		m_moMap.SetModel(m_xMap);
		m_mMap.CreateMove(0, 0, 0);

		// ���b�V���`��쐬
		SPtr<YsBulletShape_Mesh> shapeMesh = std::make_shared<YsBulletShape_Mesh>();
		shapeMesh->CreateFromMesh(*m_xMap->GetModelTbl()[0]->GetMesh());
		// ���̍쐬
		m_btMap.Create(m_Bullet, shapeMesh, m_mMap, 0);
		m_btMap.SetFriction(0.5f);
		m_btMap.SetKinematic(true);


		// ��s�@�^�X�N�ǉ�
		{
			SPtr<Task_AirPlane> add = m_TaskMgr.CreateTask<Task_AirPlane>();
			add->Init(m_Bullet, m_xF15, YsVec3(0, 5, 0), 10);

		}
			{
				SPtr<Task_AirPlane> add = m_TaskMgr.CreateTask<Task_AirPlane>();
				add->Init(m_Bullet, m_xF15, YsVec3(15, 1, 0), 10);
			}
			{
				SPtr<Task_AirPlane> add = m_TaskMgr.CreateTask<Task_AirPlane>();
				add->Init(m_Bullet, m_xSu27, YsVec3(-15, 1, 0), 10);
			}

			// 
			endFlag = true;
	}
	);

	// �A�j���[�V�����`��X���b�h�J�n
	// ���ʃX���b�h�ŉғ�
	// ���ł���Ε`��݂̂��s���Ă�������(�ꉞ�f�o�C�X��Create�n�̓X���b�h�Z�[�t�炵���̂ŁA����I�ɂ͖��Ȃ����ۂ�)
	std::thread drawThread(
		[&]() {

		int cnt = 0;

		while(1){
			// �A���t�@�l
			float alpha = sinf(YsToRadian(cnt * 10));
			alpha = alpha*0.5f + 0.5f;

			// �o�b�N�o�b�t�@���N���A����B
			YsDx.GetBackBuffer()->ClearRT(YsVec4(0.0f, 0.0f, 0.0f, 1));

			// �����\��
			YsMatrix m;
			m.CreateScale(2, 2, 1);
			m.Move(200, 200, 1);
			YsDx.GetSprite().DrawFont("Loading...", m, &YsVec4(1, 1, 1, alpha));


			// �o�b�N�o�b�t�@����ʂɕ\��
			YsDx.Present(0, 0);

			// 
			Sleep(32);

			// ���[�h�X���b�h���I�����Ă�Ȃ�I���
			if(endFlag){
				break;
			}

			cnt++;
		}
	}
	);

	// �ǂݍ��݃X���b�h�̏I���܂őҋ@����
	loadThread.join();
	// �`��X���b�h�̏I���܂őҋ@����
	drawThread.join();

	//=================================================================================

	// �J�����f�[�^������
//	m_Cam.Init(20, 130, -5);
	m_Cam.Init(20, 0, 0, true);	// FPS���[�hON

	m_Cam.m_BaseMat.CreateMove(0,2,-4);
	m_Cam.m_BaseMat.RotateY(160);
//	m_Cam.m_BaseMat.RotateX_Local(90);

	// 
	m_PauseFlag = false;

	m_LightPower = 0.2f;

	// �Œ�|�C���g���C�g
	m_PL[0] = ShMgr.m_LightMgr.AddPointLight();
	m_PL[0]->SetData(YsVec3(-1, 1, 0), YsVec4(1, 0.1f, 0.1f, 0) * 3, 3);

	m_PL[1] = ShMgr.m_LightMgr.AddPointLight();
	m_PL[1]->SetData(YsVec3(0, 1, 1), YsVec4(0.1f, 0.1f, 1, 0) * 2, 3);

	// �X�|�b�g���C�g
	m_SL = ShMgr.m_LightMgr.AddSpotLight();
	m_SL->SetData(YsVec3(0, 0.3f, 3), YsVec3(0, 0, 1), YsVec4(1, 1, 1, 0) * 2, 100, 20, 25);
}
