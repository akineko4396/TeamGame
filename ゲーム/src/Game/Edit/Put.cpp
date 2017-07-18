#include "main.h"

#include "Game\GameWorld\GameWorld.h"

#include "Put.h"

#include "EditMode.h"

#include "Game\GameAI\NavigationAI.h"

#include "GameLib\GameLib.h"

bool PutObj::Setting = false;
CharaID PutObj::ID = CharaID::UNKOWN;

//------------------------------------------
// �Ǘ��N���X
//------------------------------------------

void Put::SetPlayer(SPtr<CharacterBase> _player){
	m_wpPlayer = _player;
}
void Put::Update()
{
	if (INPUT.KeyCheck_Enter(VK_RETURN) && PutObj::Setting==false){
		SPtr<PutObj>po(new PutObj);
		if (m_wpPlayer.lock()){
			po->Init(m_wpPlayer);
			Push(po);
		}
	}


	// �ݒu�I�u�W�F�N�g�̍X�V
	for each(auto put in m_Obj)
	{
		put->Update();
	}
}
void Put::Draw()
{
	// �ݒu�I�u�W�F�N�g�̕`��
	for each(auto put in m_Obj)
	{
		put->Draw();
	}

}
void Put::CheckKill()
{
	std::vector<SPtr<PutObj>>::iterator it = m_Obj.begin();
	while (it != m_Obj.end())
	{
		if ((*it)->GetKillFlg() == true)
		{
			*it = nullptr;
			it = m_Obj.erase(it);
		}
		else{
			it++;
		}
	}
}


//------------------------------------------
// �z�u���鎞�ɕ`����s��
//------------------------------------------

void PutObj::Init(WPtr<CharacterBase> _player)
{
	// �v���C���[�̃A�h���X
	m_wpPlayer = _player;

	// �g�̃��b�V��
	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/select.x");
	m_Mo.SetModel(m_spMo);

	// ���
	m_State = std::make_shared<Put_Set>();

}
void PutObj::Update()
{	
	if (m_wpPlayer.expired() == true){
		m_KillFlg = true;	
	}
	else{

		m_State->Update(*this);

	}
}
void PutObj::Draw()
{
	if (m_wpPlayer.expired() == true)return;
	m_State->Draw(*this);
}



void Put_Set::Update(PutObj & _Obj)
{
	auto _Player = _Obj.m_wpPlayer.lock();

	// �����̖ڐ��̐�

	_Obj.m_Mat = _Player->GetMatrix();
	_Obj.m_Mat.Move_Local(0, 0, 1.25f);
	YsVec3 vTar = Player::GetTar();
	vTar.y = 0;
	vTar.Normalize();
	YsVec3 vZ = _Player->GetMatrix().GetZAxis();
	vZ.Homing(vTar, 0);
	_Obj.m_Mat.SetLookAt(vZ, YsVec3::Up);


	//�ړ�������
	YsVec3 Pos = _Obj.m_Mat.GetPos();
	Pos.x = cell(Pos.x, 1.0f);
	Pos.y = cell(Pos.y, 1.0f);
	Pos.z = cell(Pos.z, 1.0f);

	Pos -= _Obj.m_Mat.GetPos();	//��������������

	//�p�x������
	float buf = GetAngleY(_Player->GetMatrix());
	float a = cell(buf,90.0f);
	a = a - buf;				//��������������


	//----------�s��ɍ�������------------>
	
	// ��]
	_Obj.m_Mat.RotateY_Local(a);

	// �ړ�
	_Obj.m_Mat.Move(Pos);

	//------------------------------------<



	//���C����


	// �O��

	StartPos = _Player->GetMatrix().GetPos();
	StartPos.y += 0.5f;
	EndPos = _Obj.m_Mat.GetPos();
	EndPos.y += 0.5f;

	for each(auto obj in CharacterBase::GetList()){
	
		if (obj == _Player)continue;

		for (UINT i = 0; i < obj->GetModelObject().GetGameModel()->GetModelTbl().size(); i++){
			auto & models = obj->GetModelObject().GetGameModel()->GetModelTbl();
			YsSingleModel* mesh = models[i].get();
			bHit = YsCollision::RayToMesh(*mesh->GetMesh().get(), &obj->GetMatrix(), StartPos, EndPos, &fDist, NULL);



			////���������ʒu
			YsVec3 Point;
			if (bHit){				
				Point = GetHitPos(StartPos, EndPos, fDist);
				//Dw_Static(6, "%f %f %f", Point.x, Point.y, Point.z);
			}
		}
		if (bHit)break;
	}

	//�ړ����f
	_Obj.m_Mo.CalcBoneMatrix(&_Obj.m_Mat);

	one_frame++;
	if (bHit == false){
		if (INPUT.KeyCheck_Enter(VK_RETURN) && one_frame >= 2){
			_Obj.m_State = nullptr;
			_Obj.m_State = std::make_shared<Put_End>();

			//���L�����̐���
			SPtr<Box>box(new Box);
			box->Init(_Obj.m_Mat);
			CharacterBase::PushObject(box);

			NAI.UpdateWayPoint(box->GetGameModel().GetModelTbl_Static()[0], box->GetPos());

			return;
		}
	}


}
void Put_Set::Draw(PutObj & _Obj)
{

	if (bHit == false){

		// �g�`��
		ShMgr.m_Samp.DrawModel(_Obj.m_Mo, &_Obj.m_Mat);

		// ���C�`��
		ShMgr.m_Samp.DrawLine(StartPos, EndPos, &YsVec4(1, 1, 1, 1));
	}
	else
	{
		// ���łɃI�u�W�F�N�g������Ƃ��̃��C�`��
		ShMgr.m_Samp.DrawLine(StartPos, EndPos, &YsVec4(1, 0, 0, 1));
	}

}

//�z�u�I���
void Put_End::Update(PutObj & _Obj)
{
	// �ݒu�����I�u�W�F���������炱�̃N���X�����
}
void Put_End::Draw(PutObj & _Obj)
{
	// �ڈ�Ȃǂ�`��
}

