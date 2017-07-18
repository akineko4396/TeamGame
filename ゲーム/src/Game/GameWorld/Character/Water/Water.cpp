#include "main.h"

#include "Game\GameWorld\GameWorld.h"



bool Water::OnDamage(SPtr<BaseGameObj> attacker, const AttackParam& atk, DamageReply& rep)
{
	if (damage_Once == false){
		int rnd = APP.m_Rnd.GetInt(4) + 1;

		if (rnd == 1){
			m_ChangedMat.Move_Local(YsVec3::Left);
		}
		else if (rnd == 2){
			m_ChangedMat.Move_Local(YsVec3::Right);
		}
		else if (rnd == 3){
			m_ChangedMat.Move_Local(YsVec3::Front);
		}
		else if (rnd == 4){
			m_ChangedMat.Move_Local(YsVec3::Back);
		}
		else{
			m_ChangedMat.Move_Local(YsVec3::Zero);
		}
		damage_Once = true;
	}

	return true;
}
void Water::Init(YsMatrix &_m, CharaID _ID)
{
	//ID��ݒ�
	m_ID = _ID;

	//�ړ��s��
	m_Mat = _m;
	m_ChangedMat.CreateMove(0, 0, 0);

	//���f��
	m_pSm = YsDx.GetResStg()->LoadMesh("data/original/Water/water.x");
	m_Mo.SetModel(m_pSm);

	//���蔻��ݒ�
	m_hitBump = std::make_shared<HitObj_Mesh>();
	m_hitBump->m_Mass = 0; // �����Ȃ�

	// �ړ�
	m_Mat *= m_ChangedMat;
	m_Mo.CalcBoneMatrix(&m_Mat);

	// �n�ʔ���Obj
	m_hitGround = std::make_shared<HitObj_Ray>();
}
void Water::Update()
{
	// �R���W�����ɓo�^
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	//GetStage()->m_ColEng.AddDef(m_hitBump); // ���肳��鑤
	GetStage()->m_ColEng.AddAtk(m_hitBump); // ���肷�鑤


	// �q�b�g���̏���
	m_hitBump->m_OnHitStay = [this](const SPtr<BaseHitObj>& obj)
	{
		//-------------------------------
		// �U���ڍ׃f�[�^�̐ݒ�
		//-------------------------------
		AttackParam atk;

		// ���������z���̃��[�v
		for (auto& res : obj->m_HitResTbl){

			// HIT��������擾
			SPtr<BaseGameObj> you = res.m_youHitObj->m_wpTask.lock();

			//-------------------------------
			// ����Ƀ_���[�W�ʒm���s��
			//-------------------------------
			DamageReply rep;
			you->OnDamage(YsToSPtr(this), atk, rep);

		}
	};




	//�d��
	m_ChangedMat._42 -= 0.01f;

	//�n�ʔ���
	Ground();

	
	

	// �ړ�
	m_Mat *= m_ChangedMat;
	m_Mo.CalcBoneMatrix(&m_Mat);


	// �ړ��x�N�g�������ɖ߂�
	float vecy = m_ChangedMat._42;
	YsMatrix m;
	m.CreateMove(0, vecy, 0);
	m_ChangedMat = m;
	damage_Once = false;
}
void Water::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}
void Water::SetCamera()
{

}
void Water::Ground()
{

	YsVec3 vPos = m_Mat.GetPos();
	vPos.y += 1;
	// ��{�ݒ�
	m_hitGround->Set(YsToSPtr(this), vPos, vPos + YsVec3(0, -1.5f, 0));
	// �ڍאݒ�
	m_hitGround->m_AtkFilter = HitGroups::_0; // ����O���[�v 0:�Ԃ���
	m_hitGround->m_ShapeFilter = HitShapes::MESH; // ���b�V���̂ݔ���
	// �q�b�g���̏���
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
		// �󒆎�
		if (m_SkyFlag == 1){
			// �߂荞��ł�
			if (dist < 1 && m_ChangedMat._42 <= 0){
				m_Mat._42 += 1 - dist; // �����߂�
				m_ChangedMat._42 = 0; // Y �����̗͂����Z�b�g���Ƃ�
				m_SkyFlag = 0; // �n���
			}
		}
		// �n�㎞
		else if (m_SkyFlag == 0){
			// �n�ʂ���0.3 �ȏ㗣�ꂽ�Ƃ��A�󒆂Ƃ݂Ȃ��B�܂��́A������ɗ͂���������Ƃ����������ɋ󒆂Ƃ݂Ȃ��B
			if (dist >= 1 + 0.3f || m_ChangedMat._42 > 0){
				m_SkyFlag = 1; // �󒆂�
			}
			// �n��ɂ��܂�
			else{
				m_Mat._42 += 1 - dist; // �����߂�
				m_ChangedMat._42 = 0;
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

	GetStage()->m_ColEng.AddDef(m_hitGround); // ���肳��鑤�Ƃ��ēo�^

}







void nagare::Update(Water & Water)
{

}