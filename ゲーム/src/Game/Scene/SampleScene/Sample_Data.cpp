#include "main.h"
#include "SampleScene.h"

//============================================================
// �����G���W���p �Փˌ��m�֐�
//  body0��body1�������������ɌĂ΂��
//============================================================
bool SampleBulletWorld::ContactProcessedCallback(btManifoldPoint& cp, YsBulletRigidBody* body0, YsBulletRigidBody* body1)
{
	Task_CharaBase* task0 = (Task_CharaBase*)body0->GetUserPointer();
	Task_CharaBase* task1 = (Task_CharaBase*)body1->GetUserPointer();

	
	if(task0){
		task0->OnHitPhysics(cp, body0, body1);
	}
	if(task1){
		task1->OnHitPhysics(cp, body1, body0);
	}
	return true;
}

//============================================================
//
// Task_Human
//
//============================================================
void Task_Human::Init(SPtr<YsGameModel> mesh, YsMatrix& mat)
{
	m_Mat = mat;

	m_Mo.SetModel(mesh);
	m_Mo.InitAnimator(m_Anim);
	m_Anim.ChangeAnime(0, true);

}

// �����֐�
void Task_Human::Update()
{
	// �K���ɃA�j���ύX
	if (APP.m_Rnd.GetInt(0, 50) == 0) {
		int no = APP.m_Rnd.GetInt(0, 6);
		switch (no) {
		case 0:
			m_Anim.ChangeAnimeSmooth_FromName("����", 0, 20, true);
			break;
		case 1:
			m_Anim.ChangeAnimeSmooth_FromName("���@�Q", 0, 20, true);
			break;
		case 2:
			m_Anim.ChangeAnimeSmooth_FromName("�U���P", 0, 20, true);
			break;
		case 3:
			m_Anim.ChangeAnimeSmooth_FromName("�U���Q", 0, 20, true);
			break;
		case 4:
			m_Anim.ChangeAnimeSmooth_FromName("�U���R", 0, 20, true);
			break;
		case 5:
			m_Anim.ChangeAnimeSmooth_FromName("�^�[��", 0, 10, true);
			break;
		default:
			m_Anim.ChangeAnimeSmooth_FromName("����", 0, 20, true);
			break;
		}
	}


	// �X�N���v�g�֐�(�����_��)
	auto scriptProc = [&](YsAnimeKey_Script* scr)
	{
		m_Mo.CalcBoneMatrix(&m_Mat);

		Dw_Scroll(2, "[Script]%s", scr->Value.c_str());

		// �������CSV�Ƃ��ĉ��
		YsCsv csv;
		csv.LoadFromText(scr->Value, false);
		// �s���[�v
		for (UINT y = 0; y<csv.m_Tbl.size(); y++) {
			//==========================================
			// ��������
			//==========================================
			if (csv.m_Tbl[y][0] == "Attack") {
				std::string& boneName = csv.m_Tbl[y][1];

				auto bone = m_Mo.SearchBone(boneName);
				if (bone) {
					SPtr<Task_Explode> add = SampleScene::s_pScene->m_EffectTaskMgr.CreateTask<Task_Explode>();
					add->Init(&SampleScene::s_pScene->m_tsExplode, bone->WorldMat.GetPos().x, bone->WorldMat.GetPos().y, bone->WorldMat.GetPos().z, 1);
				}
			}
			//==========================================
			// ���[�U�[����
			//==========================================
			else if (csv.m_Tbl[y][0] == "Magic") {
				std::string& boneName = csv.m_Tbl[y][1];	// �{�[����

				auto bone = m_Mo.SearchBone(boneName);
				if (bone) {

					// ���[�U�[����
					for (int i = 0; i<10; i++) {
						YsMatrix m = bone->WorldMat;
						m.RotateZ_Local(APP.m_Rnd.GetFloat(0, 360));
						m.RotateX_Local(APP.m_Rnd.GetFloat(-20, 20));
						//							m.RotateZ_Local(APP.m_Rnd.GetFloat(-20, 20));
						SPtr<Task_Laser> add = SampleScene::s_pScene->m_EffectTaskMgr.CreateTask<Task_Laser>();
						add->Init(&SampleScene::s_pScene->m_tsLaser, 0, 0, 0, &m);
					}
				}
			}
		}
	};

	// �A�j���[�V������i�߁A�S�g�̃{�[����TransMat���X�V
	// ����ɃX�N���v�g�����s(�X�N���v�g�L�[�̈ʒu�ɂ�����scriptProc()�����s�����)
	m_Anim.AnimationAndScript(1.0, scriptProc, &m_Mat);
	// �A�j���[�V�������s�̂�
//	m_Amin.Animation(1.0);

	// �S�g�̃{�[����WorldMat���X�V
//	m_Mo.CalcBoneMatrix(&m_Mat);
	m_Mo.CalcBoneMatrix_SkipRoot(&m_Mat);

}

// �`��֐�
void Task_Human::Draw()
{
	// �`��
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
	// �֊s�`��
	ShMgr.m_Samp.DrawModelOutline(m_Mo, &YsVec4(0.2f, 0.2f, 0.2f, 1), &YsVec2(0.0005f, 0.015f));

}

//=======================================================
//
// Task_AirPlane
//
//=======================================================
void Task_AirPlane::Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsVec3& pos, float mass)
{
	m_Mo.SetModel(mesh);
	m_Mat.CreateMove(pos);

	// ���� ���b�V���`��쐬
	SPtr<YsBulletShape_Mesh> shapeMesh = std::make_shared<YsBulletShape_Mesh>();
	shapeMesh->CreateFromMesh(*mesh->GetModelTbl()[0]->GetMesh().get());

	// �������̍쐬�E�o�^
	m_btBody.Create(world, shapeMesh, m_Mat, 0);

}

// �����֐�
void Task_AirPlane::Update()
{
	m_Mat.RotateY_Local(0.3f);

	// �S�g�̃{�[����WorldMat���X�V
	m_Mo.CalcBoneMatrix(&m_Mat);

	// �L�����̎p�������ɁA�������[���h���̍��̂̍s����X�V
	if(m_btBody.GetBody()){
		m_btBody.SetMatrix(m_Mat);
	}

}

// �`��֐�
void Task_AirPlane::Draw()
{
	// �`��
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);

	// ����HIT���́A�킩��₷���悤�Ƀ��C����\��
	if(m_bDebugHit){
		ShMgr.m_Samp.DrawBoxLine(m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMin(),
			m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMax(),
			&YsVec4(1, 1, 0, 1), &m_Mat);

		m_bDebugHit = false;
	}

}

void Task_AirPlane::OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest)
{
	m_bDebugHit = true;
}

//=======================================================
//
// Task_RigidBox�^�X�N
//
//=======================================================
void Task_RigidBox::Init(YsBulletWorld& world, SPtr<YsGameModel> mesh, YsMatrix& m, YsVec3& halfsize, float mass)
{
	m_Mo.SetModel(mesh);

	m_Mat = m;

	// �����`��쐬
	SPtr<YsBulletShape_Box> btBox = std::make_shared<YsBulletShape_Box>();
	btBox->Create(
		halfsize	// �����̂�xyz�n�[�t�T�C�Y(�����̃T�C�Y)
	);
	// �������̍쐬�E�o�^
	m_btBody.Create(
		world,		// �������E
		btBox,		// �`��
		m_Mat,		// �s��
		mass		// ���ʁB 0�ŕs������
	);
	m_btBody.SetFriction(0.5f);	// ���C�ݒ�
	m_btBody.SetNonSleep();		// �X���[�v��Ԃɓ���Ȃ��悤�ɂ���
	m_btBody.SetUserPointer(this);	// ���̃^�X�N��UserPointer�ɋL��

	m_Cnt = 900;

	m_ExplodeInterval = 0;
}

void Task_RigidBox::Update()
{
	// �s��擾
	if(m_btBody.GetBody()){
		m_OldMat = m_Mat;

		m_btBody.GetMatrix(m_Mat);	// �������E�̍s����擾
	}

	// 
	m_Mo.CalcBoneMatrix(&m_Mat);

	// ��莞�Ԃŏ�����
	m_Cnt--;
	if(m_Cnt <= 0){
		TaskDelete();
	}

	if(m_ExplodeInterval > 0){
		m_ExplodeInterval--;
	}

}
// �`��֐�
void Task_RigidBox::Draw()
{

	// �`��
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);

	// ����HIT���́A�킩��₷���悤�Ƀ��C����\��
	if(m_bDebugHit){
		ShMgr.m_Samp.DrawBoxLine(m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMin(),
			m_Mo.GetGameModel()->GetModelTbl()[0]->GetMesh()->GetAABBMax(),
			&YsVec4(1, 1, 0, 1), &m_Mat);
		m_bDebugHit = false;
	}

}

void Task_RigidBox::OnHitPhysics(btManifoldPoint&cp, YsBulletRigidBody* my, YsBulletRigidBody* dest)
{
	m_bDebugHit = true;

	SPtr<SampleScene> ss = SPtr_dynamic_cast<SampleScene>(APP.m_NowScene);
	if(ss == nullptr)return;

	YsVec3 v = m_OldMat.GetPos() - m_Mat.GetPos();

	float speed = v.Length();

	if(speed > 0.2f && m_ExplodeInterval <= 0){
		SPtr<Task_Explode> add = ss->m_EffectTaskMgr.CreateTask<Task_Explode>();
		add->Init(&ss->m_tsExplode, cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y()+0.3f, cp.getPositionWorldOnB().z(), 3);


		m_ExplodeInterval = 2;
	}
}


//============================================================
//
// Task_Explode
//
//============================================================
void Task_Explode::Init(YsTextureSet* tex, float x, float y, float z, float size)
{
	m_BB.SetTex(tex);
	m_BB.SetVertex(-size / 2, -size / 2, size, size);
	m_BB.GetAnime().Init(8, 8, -1);		// UV�A�j���ݒ�

	m_Mat.CreateMove(x, y, z);

	m_vMove.x = APP.m_Rnd.GetFloat(-0.01f, 0.01f);
	m_vMove.y = APP.m_Rnd.GetFloat(-0.01f, 0.01f);
	m_vMove.z = APP.m_Rnd.GetFloat(-0.01f, 0.01f);

	// �|�C���g���C�g�ǉ�
	m_PointLight = ShMgr.m_LightMgr.AddPointLight();
	m_PointLight->SetData(m_Mat.GetPos(), YsVec4(1, 0.5f, 0.2f, 0) * 2, 20);
}

// �����֐�
void Task_Explode::Update()
{
	// �ړ�
	m_Mat.Move(m_vMove);

	// �A�j���[�V�����i�s
	m_BB.GetAnime().Animation(0.5f, true);

	if(m_BB.GetAnime().IsEnd()){
		TaskDelete();
	}

	// �|�C���g���C�g���W�X�V
	if(m_PointLight){
		m_PointLight->Position = m_Mat.GetPos();
	}

	// 20�R�}�ڈȍ~�̓��C�g�F����
	if(m_BB.GetAnime().GetPos() >= 20){
		m_PointLight->Diffuse *= 0.9f;
	}

}

// �`��֐�
void Task_Explode::Draw()
{
	if(m_BB.GetAnime().GetPos() < 20){
		ShMgr.m_bsAdd.SetState();
	}
	else{
		ShMgr.m_bsAlpha.SetState();
	}

	YsMatrix m = m_Mat;
	m.SetBillBoard(YsCamera::LastCam.mCam);

	ShMgr.m_Samp.DrawBillBoard(m_BB, &m);
}


//============================================================
//
// Task_Laser
//
//============================================================
void Task_Laser::Init(YsTextureSet* texset, float x, float y, float z, const YsMatrix* mat)
{
	m_Lz.SetTex(texset);
	m_Lz.Init(0.5f);

	if(mat){
		m_Mat = *mat;
	}
	else{
		m_Mat.CreateMove(x, y, z);
		m_Mat.RotateX_Local(APP.m_Rnd.GetFloat(0, 360));
		m_Mat.RotateZ_Local(APP.m_Rnd.GetFloat(0, 360));
	}

	m_fSpeed = 0.5f;

	m_Cnt = 120;

	m_Lz.AddPoint_Front(m_Mat.GetPos());

	m_fAngPow = APP.m_Rnd.GetFloat(5,8);

	// �|�C���g���C�g�ǉ�
	m_PointLight = ShMgr.m_LightMgr.AddPointLight();
	m_PointLight->SetData(m_Mat.GetPos(), YsVec4(0.2f, 0.2f, 1.0f, 0) * 3, 3);
}

// �����֐�
void Task_Laser::Update()
{
	// ������Z�ړ�
	m_Mat.Move_Local(0, 0, m_fSpeed);

	// X����]
	m_Mat.RotateX_Local(m_fAngPow);



	m_Cnt--;
	if(m_Cnt > 0){
		// ���݂̍��W�Ƀ|�C���g�ǉ�
		m_Lz.AddPoint_Front(m_Mat.GetPos());

		// �|�C���g��50����
		if(m_Lz.GetPointNum() > 50){
			m_Lz.DelPoint_Back();
		}
	}
	else{
		// ���[�U�[�̃|�C���g�̌������炵�Ă���
		if(m_Lz.GetList().size() > 0){
			m_Lz.DelPoint_Back();
		}
		// �������O�ɂȂ��������
		else{
			TaskDelete();
		}
	}

	// �|�C���g���C�g���W�X�V
	m_PointLight->Position = m_Mat.GetPos();

}

// �`��֐�
void Task_Laser::Draw()
{
	ShMgr.m_bsAdd.SetState();

	// ���[�U�[�`��
	ShMgr.m_Samp.DrawLaser(m_Lz, 0);
}