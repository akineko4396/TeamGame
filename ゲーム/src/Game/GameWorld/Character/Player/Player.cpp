
#include "main.h"

#include "Game\GameWorld\GameWorld.h"




YsVec3 Player::vTar;

// �U�����󂯂��Ƃ��̏���
bool Player::OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep)
{
	v = atk.m_vBlow / 60;

	m_vMove = atk.m_vBlow;

	return true;
}

void Player::Init(YsMatrix &_m, SPtr<YsGameModel>& _gm){
	/*-------------------------------�L�����ݒ�------------------------------------------------*/
	m_Mat = _m;
	m_Mo.SetModel(_gm);
	m_Mo.InitAnimator(m_Anim);
	m_Anim.ChangeAnime(0, true);

	/*------------------------------�J�����ݒ�-------------------------------------------------*/
	m_Cam.Init(20, 180, 0, false, true);
	m_Cam.m_BaseMat.CreateMove(0, 1.6f, 0);	// �x�[�X�̈ʒu�́A�����炢�̈ʒu
	m_Cam.m_LocalMat.CreateMove(0, 0, -5);	// �������Ɉ������ʒu
	m_Cam.m_LocalMat.RotateX(25);

	for (int i = 0; i < MemoNum; i++){
		m_MemoPos[i] = m_Mat.GetPos();
	}

	/*------------------------------�����蔻��ݒ�--------------------------------------------*/
	m_hitBump = std::make_shared<HitObj_Sphere>();
	// �ڍאݒ�
	m_hitBump->m_Mass = 1.0f; // ����(�d��)
	m_hitBump->m_AtkFilter = HitGroups::_0; // ����O���[�v �O���[�v0�́u�Ԃ���n�v�ɂ��Ƃ��܂�
	m_hitBump->m_DefFilter = HitGroups::_0; // �����O���[�v �O���[�v0�́u�Ԃ���n�v�ɂ��Ƃ��܂�
	m_hitBump->m_ShapeFilter = HitShapes::SPHERE | HitShapes::MESH; // ����Ώۂ̌`��

	/*------------------------------���[�V���������ݒ�---------------------------------------*/
	m_ActionState = std::make_shared<PlayerAS_Wait>();

	// �L�[���̓R���|�[�l���g����
	SetInput(std::make_shared<KeyInputComponent>());

	// �n�ʔ���Obj
	m_hitGround = std::make_shared<HitObj_Ray>();
}
void Player::Update(){
	//�f�o�b�O�L�[
	if (GetAsyncKeyState('Z')){
		m_Mat._41 = 0;
		m_Mat._42 = 4;
		m_Mat._43 = 0;
	}

	for (int i = MemoNum - 1; i > 0; i--){
		m_MemoPos[i] = m_MemoPos[i - 1];
	}
	m_MemoPos[0] = m_Mat.GetPos();
	//--------------------------------------------------------------->
	m_vMove -= v;
	if (v.z > 0){ if (m_vMove.z <= 0)m_vMove.z = 0; }
	if (v.z < 0){ if (m_vMove.z >= 0)m_vMove.z = 0; }

	if (v.x > 0){ if (m_vMove.x <= 0)m_vMove.x = 0; }
	if (v.x < 0){ if (m_vMove.x >= 0)m_vMove.x = 0; }
	//-------------------------------------------------------------->

	//���̓R���|�[�l���g����
	if (m_Controller) {
		m_Keys = m_Controller->Update();
	}

	//�A�N�V�����X�e�[�g���X�V
	if (m_ActionState) {
		auto p = m_ActionState;
		m_ActionState->Update(*this);
	}

	/*-----------------------------------�����蔻��ݒ�-------------------------------*/
	YsVec3 vPos = m_Mat.GetPos();
	vPos.y += 0.8f;	// �������班����̈ʒu

	m_hitBump->Set(YsToSPtr(this), vPos, 0.3f);
	GetStage()->m_ColEng.AddAtk(m_hitBump); // ���肷�鑤�Ƃ��ēo�^
	GetStage()->m_ColEng.AddDef(m_hitBump); // ���肳��鑤�Ƃ��Ă��o�^

	/*--------------------------�����蔻�蒆�ɌĂ΂��------------------------------*/

	m_hitBump->m_OnHitStay = [this](const SPtr<BaseHitObj>& obj) {
		// ���������z���̃��[�v
		for (auto& res : obj->m_HitResTbl){
			// �����߂�
			float F = obj->CalcMassRatio(res.m_youHitObj->m_Mass);
			m_Mat.Move(res.m_vPush*F);

			if (res.m_youHitObj->m_wpTask.lock()){

			}
		}
	};
	/*---------------�q�b�g�����ŏ���1�񂾂��Ă΂��------------------------------*/
	m_hitBump->m_OnHitEnter = [this](const SPtr<BaseHitObj>& obj) {
	};

	/*------------------�q�b�g���肪�I������ŏ���1�񂾂��Ă΂��----------------*/
	m_hitBump->m_OnHitExit = [this](const SPtr<BaseHitObj>& obj) {
	};


	/*---------------------------------�n�ʔ���-------------------------------------*/
	Ground();


	/*---------------------------------�e��X�V------------------------------------*/
	/*�A�j���X�V*/
	m_Anim.Animation(1.0);
	m_Mo.CalcBoneMatrix(&m_Mat);
	/*�J�����X�V*/
	m_Cam.Update(m_Mat.GetPos());


	/*-------------------------------�n���}�[�X�V--------------------------------------*/
	if (m_Hammer.expired() == false){
		m_Hammer.lock()->Update();
	}

	

}
void Player::Draw(){
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);


	if (m_Hammer.expired() == false){
		m_Hammer.lock()->Draw();
	}
}
void Player::SetCamera(){

	m_Cam.SetCamera(m_MemoPos[MemoNum - 1]);
}

void Player::Ground()
{

	/*--------------------------------------->
	�n�ʂƂ̓����蔻��֐�
	<--------------------------------------*/
	YsVec3 vPos = m_Mat.GetPos();
	vPos.y += 1;

	/*--------------------�R���W�����G���W���̊�{�ݒ�---------------------*/
	m_hitGround->Set(YsToSPtr(this), vPos, vPos + YsVec3(0, -1.5f, 0));

	/*-------------------------------HitObj�ݒ�-----------------------------*/
	m_hitGround->m_AtkFilter = HitGroups::_0; // ����O���[�v 0:�Ԃ���
	m_hitGround->m_ShapeFilter = HitShapes::MESH; // ���b�V���̂ݔ���

	/*----------------------�����蔻�莞�̏���-----------------------------*/
	m_hitGround->m_OnHitStay = [this](const SPtr<BaseHitObj>& obj)
	{
		// ���������z��̒��ŁA��ԋ߂����������߂�
		float dist = FLT_MAX;
		HitResData* pHitRes = nullptr; // ���C�������������
		for (auto& res : obj->m_HitResTbl){
			if (dist > res.m_RayDist){
				pHitRes = &res;
				dist = res.m_RayDist;
			}
		}
		/*-----------------------------------�󒆎�-------------------------------*/
		if (m_SkyFlag == 1){
			// �߂荞�񂾂�
			if (dist < 1 && m_vMove.y <= 0){
				m_Mat._42 += 1 - dist; // �����߂�
				m_vMove.y = 0; // Y �����̗͂����Z�b�g���Ƃ�
				m_SkyFlag = 0; // �n���
			}
		}
		/*--------------------------------�n�㎞----------------------------------*/
		else if (m_SkyFlag == 0){
			if (dist >= 1 + 0.3f || m_vMove.y > 0){				// �n�ʂ���0.3 �ȏ㗣�ꂽ�Ƃ��A�󒆂Ƃ݂Ȃ��B
				m_SkyFlag = 1; // �󒆏�Ԃɂ���						// �܂��́A������ɗ͂���������Ƃ����������ɋ󒆂Ƃ݂Ȃ��B
			}

			else{/*-----------------�n��ɂ��鎞-------------------*/
				m_Mat._42 += 1 - dist; // �����߂�
				m_vMove.y = 0;
				//----------------------------------------------------
				// �n�ʂ̃L�����̕ω��ɓ������ē�����
				//----------------------------------------------------
				// �n�ʃL�����̕ω��ʂɂ��A�L�����̍s����ω�������
				if (pHitRes){ // �n�ʃL����������
					auto task = pHitRes->m_youHitObj->m_wpTask.lock();
					if (task){
						m_Mat *= task->m_ChangedMat; // �n�ʃL�����̕ω��ʍs�������(�������Ȃ�)
						// ������Y ��]�ȊO�͂������Ȃ��̂ŁAY �������Z ���������ɂȂ�悤�ɒ�������
						YsVec3 vZ = m_Mat.GetZAxis();
						vZ.y = 0;
						vZ.Normalize();
						m_Mat.SetLookAt(vZ, YsVec3::Up);
					}
				}

			}
		}
		m_Mo.CalcBoneMatrix_SkipRoot(&m_Mat);
	};
	m_hitGround->m_Debug_Color.Set(1, 0, 0, 1); // �f�o�b�O�F



	GetStage()->m_ColEng.AddAtk(m_hitGround); // ���肷�鑤�Ƃ��ēo�^
	GetStage()->m_ColEng.AddDef(m_hitGround); // ���肷�鑤�Ƃ��ēo�^


}


//=====================================================
// �u�ҋ@�v���
//=====================================================
void PlayerAS_Wait::Update(Player &Player)
{
	// �����L�[
	if (Player.GetKeys() & GK_LUP || Player.GetKeys() & GK_LLEFT || Player.GetKeys() & GK_LRIGHT || Player.GetKeys() & GK_LDOWN) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("����(���킠��)", 0, 10, true, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_Run> p = std::make_shared<PlayerAS_Run>();
		Player.m_ActionState = p;
		return;
	}
	// �}�E�X���N���b�N
	if (Player.GetKeys() & GK_X) {
		//�A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�U��", 0, 10, false, 1);
		// �s���؂�ւ�
		SPtr<PlayerAS_Attack> p = std::make_shared<PlayerAS_Attack>();
		Player.m_ActionState = p;
		return;
	}
	// �X�y�[�X�L�[
	if (Player.GetKeys() & GK_A) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�W�����v(�㏸)", 0, 10, false, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_JumpUp> p = std::make_shared<PlayerAS_JumpUp>();
		Player.m_ActionState = p;
		return;
	}

	//�Ƃ肠�����\���L�[��
	if (Player.GetKeys() & GK_LDOWN) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�̂�����", 0, 10, false, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_KnockBack> p = std::make_shared<PlayerAS_KnockBack>();
		Player.m_ActionState = p;
		return;
	}

	// �d��
	Player.m_vMove.y -= 0.01f;

	// ���C
	/*if (Player.m_SkyFlag == 0) {
	Player.m_vMove *= 0.9f;
	}*/
	// �͈ړ�
	Player.m_Mat.Move(Player.m_vMove);

}

//=====================================================
// �u����v���
//=====================================================
void PlayerAS_Run::Update(Player& Player)
{
	// �ړ��L�[�������Ă�H
	if (!(Player.GetKeys() & GK_LUP || Player.GetKeys() & GK_LLEFT || Player.GetKeys() & GK_LRIGHT || Player.GetKeys() & GK_LDOWN)) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�ҋ@", 0, 10, true, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}

	// �W�����v�U��
	if (Player.GetKeys() & GK_X) {
		//�A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�W�����v�U��", 0, 2, false, 1);
		// �s���؂�ւ�
		SPtr<PlayerAS_JumpAttack> p = std::make_shared<PlayerAS_JumpAttack>();
		Player.m_ActionState = p;
		return;
	}

	// �ړ��ɂ�鎋�_�␳
	float rotaAng = 0;
	/*if (Player.GetKeys() & GK_LLEFT) {
	rotaAng = -1.0f;
	}
	if (Player.GetKeys() & GK_LRIGHT) {
	rotaAng = 1.0f;
	}*/
	if (Player.GetKeys() & GK_LUP) {
		rotaAng *= 0.5f;
	}
	// �J������]
	Player.m_Cam.m_BaseMat.RotateY_Local(rotaAng);





	// �L�[�ɂ��L�����̌�������]
	//YsVec3 vTar;
	if (Player.GetKeys() & GK_LRIGHT) {
		Player.vTar += Player.m_Cam.m_BaseMat.GetXAxis();
	}
	else if (Player.GetKeys() & GK_LLEFT) {
		Player.vTar -= Player.m_Cam.m_BaseMat.GetXAxis();
	}
	if (Player.GetKeys() & GK_LUP) {
		Player.vTar += Player.m_Cam.m_BaseMat.GetZAxis();
	}
	else if (Player.GetKeys() & GK_LDOWN) {
		Player.vTar -= Player.m_Cam.m_BaseMat.GetZAxis();
	}
	Player.vTar.y = 0;
	Player.vTar.Normalize();
	YsVec3 vZ = Player.GetMatrix().GetZAxis();
	vZ.Homing(Player.vTar, 10);
	Player.m_Mat.SetLookAt(vZ, YsVec3::Up);






	// �A�j�����x���ړ����x�ɂ���
	float walkSpeed = (float)Player.GetAnime().GetAnimeSpeed() * 0.3f;
	Player.m_Mat.Move_Local(0, 0, walkSpeed);
	// �d��
	Player.m_vMove.y -= 0.01f;
	// ���C
	/*if (Player.m_SkyFlag == 0) {
	Player.m_vMove *= 0.9f;
	}*/
	// �͈ړ�
	Player.m_Mat.Move(Player.m_vMove);
}
//=====================================================
//	�u�U���v���
//=====================================================
void PlayerAS_Attack::Update(Player& Player)
{
	if (!(Player.GetKeys() & GK_X) && Player.GetAnime().IsAnimationEnd()) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�ҋ@", 0, 10, true, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}



	if (Player.m_Hammer.expired() == false){

		auto weapon = Player.m_Hammer.lock();

		// �_���[�W�ʒm���쐬
		AttackParam atk;

		int time = 1;

		// �q�b�g�������֐�
		auto OnHitProc = [this, atk](const SPtr<BaseHitObj>& obj) {
			// �q�b�g������
			// ���������z��S�������[�v
			for (auto& res : obj->m_HitResTbl){
				// HIT��������擾
				SPtr<BaseGameObj> you = res.m_youHitObj->m_wpTask.lock();


				//-------------------------------
				// ����Ƀ_���[�W�ʒm���s��
				//-------------------------------
				DamageReply rep;
				if (you->OnDamage(nullptr, atk, rep)){
				}
				else{

				}
			}
		};
		// ����̍U�����蔭��
		weapon->RegisterAttack(
			time,			// ������������
			OnHitProc		// �q�b�g�������֐�
			);
	}

}
//=====================================================
//	�u�W�����v�U���v���
//=====================================================
void PlayerAS_JumpAttack::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�ҋ@", 0, 10, true, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}

	if (Player.m_Hammer.expired() == false){

		auto weapon = Player.m_Hammer.lock();

		// �_���[�W�ʒm���쐬
		AttackParam atk;

		int time = 1;

		// �q�b�g�������֐�
		auto OnHitProc = [this, atk](const SPtr<BaseHitObj>& obj) {
			// �q�b�g������
			// ���������z��S�������[�v
			for (auto& res : obj->m_HitResTbl){
				// HIT��������擾
				SPtr<BaseGameObj> you = res.m_youHitObj->m_wpTask.lock();


				//-------------------------------
				// ����Ƀ_���[�W�ʒm���s��
				//-------------------------------
				DamageReply rep;
				if (you->OnDamage(nullptr, atk, rep)){
				}
				else{

				}
			}
		};
		// ����̍U�����蔭��
		weapon->RegisterAttack(
			time,			// ������������
			OnHitProc		// �q�b�g�������֐�
			);
	}
}
//=====================================================
//	�u�W�����v(�㏸)�v���
//=====================================================
void PlayerAS_JumpUp::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�W�����v(�~��)", 0, 10, true, 0.01);
		// �s���؂�ւ�
		SPtr<PlayerAS_JumpDown> p = std::make_shared<PlayerAS_JumpDown>();
		Player.m_ActionState = p;
		return;
	}


	//�v���C���[�s��ۑ�
	Player.m_Mat = Player.GetMatrix();

	// �͈ړ�
	Player.m_Mat.Move(Player.m_vMove);

}
//=====================================================
//	�u�W�����v(�~��)�v���
//=====================================================
void PlayerAS_JumpDown::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�ҋ@", 0, 10, true, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}

	//�v���C���[�s��ۑ�
	Player.m_Mat = Player.GetMatrix();

	// �͈ړ�
	Player.m_Mat.Move(Player.m_vMove);

}
//=====================================================
//	�u�̂�����v���
//=====================================================
void PlayerAS_KnockBack::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�ҋ@", 0, 10, true, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}
}
//=====================================================
// �ėp�s��
//=====================================================
void PlayerAS_Generic::Update(Player& Player)
{
	if (Player.GetAnime().IsAnimationEnd()) {
		// �A�j���؂�ւ�
		Player.GetAnime().ChangeAnimeSmooth_FromName("�ҋ@", 0, 10, true, 0.5);
		// �s���؂�ւ�
		SPtr<PlayerAS_Wait> p = std::make_shared<PlayerAS_Wait>();
		Player.m_ActionState = p;
		return;
	}


	//�v���C���[�s��ۑ�
	Player.m_Mat = Player.GetMatrix();

	// ���C
	/*if (Player.m_SkyFlag == 0) {
	Player.m_vMove *= 0.9f;
	}*/
	// �͈ړ�
	Player.m_Mat.Move(Player.m_vMove);
}