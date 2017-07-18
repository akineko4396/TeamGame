
#include "main.h"

#include "Game\GameWorld\GameWorld.h"

#include "EditMode.h"


EditMode* EditMode::s_pInst = nullptr;

std::vector<SPtr<EditObject>> EditMode::m_eList;

EditMode::E_ID EditMode::eidlist[NUM];

void EditMode::Release(){
	m_eList.clear();
	m_Cam = nullptr;
	m_wpNow.reset();
	delete m_PickUp;
	Edit = false;
}

void EditMode::CheckKill()
{
	std::vector<SPtr<EditObject>>::iterator it = m_eList.begin();
	while (it != m_eList.end())
	{

		if ((*it)->GetKillFlg() == true)
		{
			*it = nullptr;
			it = m_eList.erase(it);
		}
		else{
			it++;
		}
	}
}

//Edit
bool EditMode::Edit = false;

void EditMode::EditInit(){
	Edit = true;

	m_Cam = std::make_shared<GameCamera>();
	m_Cam->Init(20, 180, 0);
	m_PickUp = new PickUp();
	m_wpNow.reset();

}
void EditMode::EditUpdate()
{
	if (INPUT.KeyCheck_Enter(VK_LEFT))
	{

		//m_Cnt--;
		//if (m_Cnt < 0){ m_Cnt = MAX - 1; }
		//m_SelectFlg = ID_List[m_Cnt];



		m_Cnt--;
		if (m_Cnt < 0){ m_Cnt = NUM - 1; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;



		m_wpNow.reset();
	}
	if (INPUT.KeyCheck_Enter(VK_RIGHT))
	{

		//m_Cnt++;
		//if (m_Cnt == MAX){ m_Cnt = 0; }
		//m_SelectFlg = ID_List[m_Cnt];


		m_Cnt++;
		if (m_Cnt == NUM){ m_Cnt = 0; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;


		m_wpNow.reset();
	}


	//+++++++++++++++++++++++
	// �}�E�X����
	//+++++++++++++++++++++++

	POINT p;
	GetCursorPos(&p);
	const LONG window_x = 1280;
	const LONG window_y = 720;
	// �E�C���h�E�̘g��
	if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y)
	{

		// �u���b�N�̑傫��
		const float block_size = 1.0f;

		// �l���󂯎�邽�߂̕ϐ�
		YsVec3 HitPos;
		SPtr<CharacterBase>HitObj;
		bool HeadFlg = false;

		// ���C����֐�
		if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg))
		{
			// �u���b�N�̏�ɃJ�[�\��

			// �z�u������
			for each(auto made in m_eList)
			{
				if (made->m_wpObj.expired() == false){
					if (made->m_wpObj.lock() == HitObj){
						//��������̏�ɂ͒u���Ȃ�
						HeadFlg = true;
					}
				}
			}

			// ����`�F�b�N�@&�@��������łȂ�
			if (HeadFlg == false){	//����ɂȂɂ��Ȃ�

				HitPos.y += block_size;

				float x = cell(HitPos.x, block_size);
				float y = cell(HitPos.y, block_size);
				float z = cell(HitPos.z, block_size);

				YsMatrix m;
				m.CreateMove(x, y, z);

				if (EditObject::NowSetting == false){


					//��p���X�g
					SPtr<EditObject>obj(new EditObject(m_SelectFlg, m_Cam,m_Cnt));
					obj->Init(m);
					Push(obj);
					//���݂̕ҏW�L����
					m_wpNow = obj;

				}

				//���݂̕ҏW�L�����Ƀr���{�[�h�̈ʒu��m�点��
				if (m_wpNow.expired() == false){
					m_wpNow.lock()->m_PickUp->SetPos(m);
				}
				else
				{
					//�r���{�[�h
					m_PickUp->SetPos(m);
				}
			}
			else{
				//���݂̕ҏW�L�����ɏ����Ă��炤
				if (m_wpNow.expired() == false){
					if (m_wpNow.lock()->m_wpObj.expired() == true){
						m_wpNow.lock()->SetKill();
						m_wpNow.reset();
						//m_PickUp->SetPos(m);
					}
				}
			}
		}
		else
		{

			//���݂̕ҏW�L�����ɏ����Ă��炤
			if (m_wpNow.expired() == false){
				if (m_wpNow.lock()->m_wpObj.expired() == true){
					m_wpNow.lock()->SetKill();
					m_wpNow.reset();
					//m_PickUp->SetPos(m);
				}
			}

		}

	}


	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_R))
	{
		POINT p;
		GetCursorPos(&p);
		const LONG window_x = 1280;
		const LONG window_y = 720;

		if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y){

			// ������I�u�W�F�N�g
			// �u���b�N�̏�ɃJ�[�\��

			YsVec3 HitPos;
			SPtr<CharacterBase> HitObj;
			bool HeadFlg = false;
			if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg)){

				//������I�u�W�F�N�g�Ȃ������
				for each(auto made in m_eList){
					if (made->m_wpObj.expired() == false){
						if (made->m_wpObj.lock() == HitObj){
							HitObj->SetKillFlg();
							made->SetKill();
						}
					}
				}
			}

		}
	}



	// �ݒu���X�V
	for each(auto obj in m_eList){
		obj->Update();
	}
	// �N���A����
	CheckKill();
}
void EditMode::EditDraw()
{
	// �ݒu���`��
	for each(auto obj in m_eList)
	{
		obj->Draw();
	}

	// �����I�����ĂȂ��Ƃ��̃r���{�[�h
	if (m_wpNow.expired() == true){
		m_PickUp->Draw();
	}
}

//�J���Ҍ���Edit

void EditMode::MapInit(){
	Edit = false;

	m_Cam = std::make_shared<GameCamera>();
	m_Cam->Init(20, 180, 0);
	m_PickUp = new PickUp();
}
void EditMode::MapUpdate()
{
	if (INPUT.KeyCheck_Enter(VK_LEFT))
	{

		//m_Cnt--;
		//if (m_Cnt < 0){ m_Cnt = MAX - 1; }
		//m_SelectFlg = ID_List[m_Cnt];


		m_Cnt--;
		if (m_Cnt < 0){ m_Cnt = NUM - 1; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;

		m_wpNow.reset();
	}
	if (INPUT.KeyCheck_Enter(VK_RIGHT))
	{

		//m_Cnt++;
		//if (m_Cnt == MAX){ m_Cnt = 0; }
		//m_SelectFlg = ID_List[m_Cnt];


		m_Cnt++;
		if (m_Cnt == NUM){ m_Cnt = 0; }
		m_SelectFlg = EditMode::eidlist[m_Cnt].id;

		m_wpNow.reset();
	}

	//+++++++++++++++++++++++
	// �}�E�X����
	//+++++++++++++++++++++++

	POINT p;
	GetCursorPos(&p);
	const LONG window_x = 1280;
	const LONG window_y = 720;
	// �E�C���h�E�̘g��
	if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y)
	{

		// �u���b�N�̑傫��
		const float block_size = 1.0f;

		// �l���󂯎�邽�߂̕ϐ�
		YsVec3 HitPos;
		SPtr<CharacterBase>HitObj;
		bool HeadFlg = false;

		// ���C����֐�
		if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg))
		{

			// �u���b�N�̏�ɃJ�[�\��

			if (HeadFlg == false){	//����ɂȂɂ��Ȃ�

				HitPos.y += block_size;
				float x = cell(HitPos.x, block_size);
				float y = cell(HitPos.y, block_size);
				float z = cell(HitPos.z, block_size);

				YsMatrix m;
				m.CreateMove(x, y, z);

				if (EditObject::NowSetting == false){


					//��p���X�g
					SPtr<EditObject>obj(new EditObject(m_SelectFlg, m_Cam,m_Cnt));
					obj->Init(m);
					Push(obj);
					//���݂̕ҏW�L����
					m_wpNow = obj;


				}

				//���݂̕ҏW�L�����Ƀr���{�[�h�̈ʒu��m�点��
				if (m_wpNow.expired() == false){
					m_wpNow.lock()->m_PickUp->SetPos(m);
				}
				else{
					//�r���{�[�h
					m_PickUp->SetPos(m);
				}
			}

		}
		else
		{

			// �Ȃɂ��������ɃJ�[�\��

			D3DXVECTOR3 MousePoint = Calc2(*m_Cam);
			float x = cell(MousePoint.x, block_size);
			float z = cell(MousePoint.z, block_size);

			// XZ����,Y��0�̍���
			YsMatrix m;
			m.CreateMove(x, 0, z);


			if (EditObject::NowSetting == false){

				//��p���X�g
				SPtr<EditObject>obj(new EditObject(m_SelectFlg, m_Cam,m_Cnt));
				obj->Init(m);
				Push(obj);
				//���݂̕ҏW�L����
				m_wpNow = obj;

			}
			//���݂̕ҏW�L�����Ƀr���{�[�h�̈ʒu��m�点��
			if (m_wpNow.expired() == false){
				m_wpNow.lock()->m_PickUp->SetPos(m);
			}
			else{
				//�r���{�[�h
				m_PickUp->SetPos(m);
			}
		}

	}


	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_R))
	{
		POINT p;
		GetCursorPos(&p);
		const LONG window_x = 1280;
		const LONG window_y = 720;

		if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y){

			// �u���b�N�̏�ɃJ�[�\��

			YsVec3 HitPos;
			SPtr<CharacterBase> HitObj;
			bool HeadFlg = false;
			if (MouceTo3D(*m_Cam, HitPos, HitObj, HeadFlg)){
				if (HitObj){
					// ���㔻��Ȃ�
					HitObj->SetKillFlg();
				}
			}

		}
	}



	// �ݒu���X�V
	for each(auto obj in m_eList){
		obj->Update();
	}

	CheckKill();
}
void EditMode::MapDraw()
{
	// �ݒu���`��
	for each(auto obj in m_eList)
	{
		obj->Draw();
	}

	// �����I�����ĂȂ��Ƃ��̃r���{�[�h
	if (m_wpNow.expired() == true){
		m_PickUp->Draw();
	}
}


//�傫�߂̔�
void LBox::Init(YsMatrix &_m)
{
	m_hitBump = std::make_shared<HitObj_Mesh>();
	// �ڍאݒ�
	m_hitBump->m_Mass = 0;

	m_pSm = APP.m_ResStg.LoadMesh("data/original/largebox.x");
	m_Mo.SetModel(m_pSm);

	m_Mat = _m;
}
void LBox::Update()
{
	auto& models = m_Mo.GetGameModel()->GetModelTbl_Static();
	YsSingleModel* mesh = models[0].get();

	m_hitBump->Set(YsToSPtr(this), mesh->GetMesh(), m_Mat);
	GetStage()->m_ColEng.AddDef(m_hitBump); // ���肳��鑤


	m_Mo.CalcBoneMatrix(&m_Mat);
}
void LBox::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}



// �X�N���[�����W�����[���h���W�ɕϊ�
// 1: D3DXVECTOR3 * pout
// 2: �X�N���[��X���W
// 3: �X�N���[��Y���W
// 4: �ˉe��Ԃł�Z�l�i0�`1�j
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy,
	float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj) {
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return pout;
}

// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
// (�n�� = Y = 0 �̍����̂�)
D3DXVECTOR3* CalcScreenToXZ(D3DXVECTOR3* pout, float Sx, float Sy,
	int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if (ray.y <= 0) {
		// ����_
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}

	return pout;
}


// �X�N���[�����W�����[���h���W�ɕϊ� ver2
// 1: GameCamera
D3DXVECTOR3 Calc2(GameCamera _Camera){

	POINT p;
	YsVec3 Pt;
	GetCursorPos(&p);
	Pt.x = (float)p.x;
	Pt.y = (float)p.y;

	//�N���C�A���g�̑傫���擾�\
	RECT Lect;
	GetWindowRect(APP.m_hWnd, &Lect);
	GetClientRect(APP.m_hWnd, &Lect);


	Pt.x = Pt.x * ((float)1280 / (float)1280);
	Pt.y = Pt.y * ((float)(720 - 30) / (float)720);//�E�C���h�E�̘g�̕�





	D3DXVECTOR3 GroundPoint;
	CalcScreenToXZ(&GroundPoint, Pt.x, Pt.y, 1280, 720, &_Camera.mView, &_Camera.mProj);

	return GroundPoint;
}

// �}�b�v���W(0,0,0)�𒆉��ɃZ����ɁB���S�_��Ԃ��B
// 1: XorYorZ�̍��W
// 2: �Z����̃T�C�Y
float cell(float _f, float _size)
{
	float f = _f;
	if (f >= 0){
		f = _f + _size / 2.0f;
	}
	else{
		f = _f - _size / 2.0f;
	}
	int i = (int)(f / _size);
	float ret = (float)i*_size;
	return ret;
}

// ���C����ƃI�u�W�F�N�g�̈ʒu�Ȃǂ��擾
// �߂�l: �J�[�\���̎w���ʒu�Ƀ��b�V��������?
bool MouceTo3D(GameCamera _Camera, YsVec3 &_Out, SPtr<CharacterBase> &_HitObj, bool &_HeadFlg)
{
	// ���[�J���ϐ�

	std::vector<float> Dist_List;			//�q�b�g��������
	std::vector<SPtr<CharacterBase>> Hit_List;	//�q�b�g�����I�u�W�F�N�g
	POINT pt;								//�}�E�X���W
	bool Hit = false;						//���b�V���Ƃ̔���

	//+++++++++++++++++++

	GetCursorPos(&pt);						//�J�[�\���ʒu�̐�΍��W���擾
	ScreenToClient(APP.m_hWnd, &pt);		//�J�[�\����΍��W����A���̃E�B���h�E���̍��W�ɕ␳
	pt.x = pt.x * ((LONG)1280 / (LONG)1280);//���݂̃T�C�Y�ŕ␳�i�g�T�C�Y���͔�l���j
	pt.y = pt.y * ((LONG)720 / (LONG)720);


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//�@���b�V�����[�v
	//
	for each(auto obj in CharacterBase::GetList())
	{
		D3DXMATRIX mWorldView;
		D3DXMATRIX m;
		D3DXVECTOR3 vRayDir;
		D3DXVECTOR3 vStartPoint;
		D3DXVECTOR3 v;

		//�}�E�X���W���X�N���[���̒��S�����_�ɂȂ�悤�ɕ␳�i���K���j
		v.x = (((2.0f*(float)pt.x) / (float)1280) - 1.0f) / _Camera.mProj._11;
		v.y = -(((2.0f*(float)pt.y) / (float)720) - 1.0f) / _Camera.mProj._22;
		v.z = 1.0f;

		mWorldView = obj->GetMatrix() * _Camera.mView;
		D3DXMatrixInverse(&m, NULL, &mWorldView);

		// vRayDir�̓��C���΂������x�N�g��
		vRayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
		vRayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
		vRayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;

		// vStartPoint�̓X�N���[����̍��W��3D�֕ϊ���������
		vStartPoint.x = m._41;
		vStartPoint.y = m._42;
		vStartPoint.z = m._43;

		//���C��������
		BOOL bHit;
		float fDist;
		for (UINT i = 0; i < obj->GetModelObject().GetGameModel()->GetModelTbl().size(); i++){
			auto & models = obj->GetModelObject().GetGameModel()->GetModelTbl();
			YsSingleModel* mesh = models[i].get();
			bHit = YsCollision::RayToMesh(*mesh->GetMesh().get(), NULL, vStartPoint, (vStartPoint + vRayDir * 100), &fDist, NULL);

			////���������ʒu?
			//YsVec3 Point;
			//if (bHit){
			//	Point = vRayDir*fDist + vStartPoint;
			//}

			if (bHit)			// ���b�V�����������B
			{
				//�q�b�g�t���O
				Hit = true;
				//���胊�X�g�ɓo�^
				Dist_List.push_back(fDist);
				Hit_List.push_back(obj);
			}
		}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	// ��ԋ߂����̕\���ʒu�����
	// ...�`��͍���...
	//

	float min;
	int No, MinNo;
	No = MinNo = 0;
	//�܂����X�g�̍ŏ��̋����ŏ�����
	for each(float dis in Dist_List){
		min = dis;
		break;
	}
	//��ԋ߂������𔻒�
	for each(float dis in Dist_List){
		if (min > dis){
			min = dis;
			MinNo = No;
		}
		No++;
	}

	//��ԋ߂����������I�u�W�F�N�g�܂ŁA���蕨���X�g�����ǂ�
	//	����𔻒肵���I�u�W�F�N�g�ɂ���
	No = 0;
	for each(auto hit_obj in Hit_List)
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//
		if (MinNo == No)	//���X�g�����ǂꂽ�B
		{
			_Out = hit_obj->GetMatrix().GetPos();	//�Q�Ƃ̈����ɒl�n��
			_HitObj = hit_obj;						//�Q�Ƃ̈����ɒl�n��

			//����`�F�b�N
			bool buf = false;
			_HeadFlg = false;
			for each(auto head in CharacterBase::GetList()){
				if (_HitObj == head)continue;
				for (UINT i = 0; i < head->GetModelObject().GetGameModel()->GetModelTbl().size(); i++){
					auto & models = head->GetModelObject().GetGameModel()->GetModelTbl();
					YsSingleModel* mesh = models[i].get();
					const float max_h = 1.5f;			//����F+1�܂�
					buf = YsCollision::RayToMesh(*mesh->GetMesh().get(), &head->GetMatrix(),
						YsVec3(_Out.x, _Out.y + 0.5f, _Out.z), YsVec3(_Out.x, _Out.y + max_h, _Out.z), NULL, NULL);
					if (buf){ _HeadFlg = true; break; }	//����ɂȂɂ�����B
				}
			}
		}
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
		No++;	//���X�g�̃J�E���g�A�b�v
	}

	return Hit;
}



// Edit��p�̃I�u�W�F�N�g
bool EditObject::NowSetting = false;

void EditObject::Init(YsMatrix &_m)
{
	m_Mat = _m;

	//if (m_ID == CharaID::BOX){
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/stand_box.x");
	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//}
	//else if (m_ID == CharaID::LBOX){
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/largebox.x");

	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//	SPtr<EditVec2> top(new EditVec2(0, 1.0f));
	//	m_Shape.push_back(top);
	//	SPtr<EditVec2> bot(new EditVec2(0, -1.0f));
	//	m_Shape.push_back(bot);
	//	SPtr<EditVec2> left(new EditVec2(-1.0f, 0));
	//	m_Shape.push_back(left);
	//	SPtr<EditVec2> right(new EditVec2(1.0f, 0));
	//	m_Shape.push_back(right);
	//}else
	//if (m_ID == CharaID::SPRING){
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/Spring/Spring.xed");
	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//}
	//else{
	//	m_spMo = YsDx.GetResStg()->LoadMesh("data/original/stand_box.x.x");
	//	SPtr<EditVec2> center(new EditVec2(0, 0));
	//	center->Center = true;
	//	m_Shape.push_back(center);
	//}


	m_spMo = YsDx.GetResStg()->LoadMesh(EditMode::eidlist[m_Num].path);
	if (EditMode::eidlist[m_Num].shpe == EditMode::E_ID::BOX1){
		SPtr<EditVec2> center(new EditVec2(0, 0));
		center->Center = true;
		m_Shape.push_back(center);
	}
	else if (EditMode::eidlist[m_Num].shpe == EditMode::E_ID::BOX5){
		SPtr<EditVec2> center(new EditVec2(0, 0));
		center->Center = true;
		m_Shape.push_back(center);
		SPtr<EditVec2> top(new EditVec2(0, 1.0f));
		m_Shape.push_back(top);
		SPtr<EditVec2> bot(new EditVec2(0, -1.0f));
		m_Shape.push_back(bot);
		SPtr<EditVec2> left(new EditVec2(-1.0f, 0));
		m_Shape.push_back(left);
		SPtr<EditVec2> right(new EditVec2(1.0f, 0));
		m_Shape.push_back(right);
	}





	m_Mo.SetModel(m_spMo);

	m_State = std::make_shared<Edit_Set>(m_Mat, m_wpNowCam);


	m_wpObj.reset();
}
void EditObject::Update()
{
	m_State->Update(*this);
	m_Mat = m_State->r_Mat;
	m_Mo.CalcBoneMatrix(&m_Mat);


}
void EditObject::Draw()
{
	//�X�e�[�g�ŕ`��
	m_State->Draw(*this);
}

void Edit_Set::Update(EditObject & E_Obj)
{
	if (INPUT.KeyCheck_Enter(VK_LEFT) || INPUT.KeyCheck_Enter(VK_RIGHT)){
		E_Obj.m_KillFlg = true;
		return;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++

	//�ݒu�X�e�[�g�Ŏ��s

	GameCamera *m_Cam = m_wpCamera.lock().get();

	m_onBlock = false;
	m_onY = false;

	POINT p;
	GetCursorPos(&p);
	const LONG window_x = 1280;
	const LONG window_y = 720;
	if (p.x >= 0 && p.x <= window_x && p.y >= 0 && p.y <= window_y){


		//�P�u���b�N�̃T�C�Y
		const float block_size = 1.0f;


		// ���肵���I�u�W�F�N�g���󂯎��ϐ�

		YsVec3 HitPos;
		SPtr<CharacterBase>HitObj;
		bool HeadFlg = false;


		for each(auto shape in E_Obj.m_Shape){

			//++++++++++++++++++++++++++++++++
			//�_�~�[�J�����쐬
			//++++++++++++++++++++++++++++++++
			GameCamera dumy = *m_Cam;	//���̂��R�s�[
			dumy.m_BaseMat.Move_Local(shape->x, 0, shape->z);
			dumy.SetCamera();


			if (MouceTo3D(dumy, HitPos, HitObj, HeadFlg)){

				// �u���b�N�̏�ɃJ�[�\��

				if (EditMode::Edit){
					// �z�u������
					for each(auto made in EditMode::m_eList)
					{
						if (made->m_wpObj.expired() == false){
							if (made->m_wpObj.lock() == HitObj){
								//��������̏�ɂ͒u���Ȃ�
								HeadFlg = true;
							}
						}
					}

				}


				if (HeadFlg == false){	//����ɂȂɂ��Ȃ�

					HitPos.y += block_size;
					float x = cell(HitPos.x, block_size);
					float y = cell(HitPos.y, block_size);
					float z = cell(HitPos.z, block_size);

					// �ŏI�I�Ȕz�u�ʒu
					YsMatrix m;
					m.CreateMove(x, y, z);


					//�ݒu�\
					m_onBlock = true;
					//����
					if (shape->Center){
						m_Set = m;
					}
				}
				else
				{
					//�ݒu�s��
					m_onBlock = false;
					m_onY = false;

					// Edit�ł�OFF�@MapEdit�ł�ON
					if (EditMode::Edit)return;	//���^�[��

					//break;
				}
			}

			else{

				D3DXVECTOR3 MousePoint = Calc2(dumy);
				float x = cell(MousePoint.x, block_size);
				float z = cell(MousePoint.z, block_size);

				// �ŏI�I�Ȕz�u�ʒu
				YsMatrix m;
				m.CreateMove(x, 0, z);


				//�ݒu�\
				m_onY = true;
				//����
				if (shape->Center){
					m_Set = m;
				}


				// �Ȃɂ��������ɃJ�[�\��
				// Edit�ł�OFF�@MapEdit�ł�ON
				if (EditMode::Edit)return;	//���^�[��
			}
		}
	}

	//
	if (m_onBlock == true && m_onY == true)return;
	if (m_onBlock == false && m_onY == false)return;

	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_L)){
		//�ʒu������E��]�𑀍��
		YsMatrix m = m_Set;
		E_Obj.m_State = nullptr;
		E_Obj.m_State = std::make_shared<Edit_Rot>(m);
	}
}

void Edit_Set::Draw(EditObject & E_Obj)
{

	if (m_onBlock == true && m_onY == true)return;
	if (m_onBlock == false && m_onY == false)return;
	//if (m_onY && EditMode::Edit)return;

	//�������`��Ƃ�

	//ShMgr.m_Samp.DrawModel(E_Obj.m_Mo, &E_Obj.m_Mat);

	for each(auto s in E_Obj.m_Shape){
		YsMatrix copy = E_Obj.m_PickUp->m_Buf;
		copy.Move_Local(s->x, 0, s->z);
		copy.Move_Local(0, 0.01f, 0);
		copy.RotateX_Local(90.0f);

		E_Obj.m_PickUp->Draw(copy);
	}
}

void Edit_Rot::Update(EditObject & E_Obj)
{
	if (INPUT.KeyCheck_Enter(VK_LEFT) || INPUT.KeyCheck_Enter(VK_RIGHT)){
		E_Obj.m_KillFlg = true;
		return;
	}


	//�Z�b�g�I���
	OneFrame++;
	if (INPUT.MouseCheck_Enter(CInput::MOUSEBUTTON::MOUSE_L) && m_PauseFlg == 0 && OneFrame >= 1)
	{
		//(��]�I������) ��������

		//��
		if (E_Obj.m_ID == CharaID::BOX){
			SPtr<Box>box(new Box);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}
		//�\��
		else if (E_Obj.m_ID == CharaID::LBOX)
		{
			SPtr<LBox>box(new LBox);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}
		//�X�v�����O
		else if (E_Obj.m_ID == CharaID::SPRING){
			SPtr<Spring>box(new Spring);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}

		//�����ɐ�����ǉ����Ă���


		else if (E_Obj.m_ID == CharaID::STAIRS){
			SPtr<Stairs>box(new Stairs);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}










		else{
			SPtr<Box>box(new Box);
			box->Init(E_Obj.m_Mat);
			CharacterBase::PushObject(box);
			E_Obj.m_wpObj = box;
		}

		YsMatrix m = r_Mat;//�ۑ�
		E_Obj.m_State = nullptr;
		E_Obj.m_State = std::make_shared<Edit_Put>(m);
		return;
	}


	//+++++++++++++++++++++++++++++
	//�s����󂯎���đ��삷��
	//+++++++++++++++++++++++++++++

	r_Mat = E_Obj.m_Mat;

	//��]�Ȃǂ����
	if (INPUT.GetMouseWheel() > 0 && m_PauseFlg == 0){
		m_PauseFlg = UP;
	}
	if (INPUT.GetMouseWheel() < 0 && m_PauseFlg == 0){
		m_PauseFlg = DOWN;
	}
	switch (m_PauseFlg){
	case UP:
		r_Mat.RotateY_Local(4.5f);
		m_PauseCnt++;
		break;
	case DOWN:
		r_Mat.RotateY_Local(-4.5f);
		m_PauseCnt++;
		break;
	default:
		break;
	}
	if (m_PauseCnt == 20){
		m_PauseCnt = 0;
		m_PauseFlg = 0;
	}
}

void Edit_Rot::Draw(EditObject & E_Obj)
{
	ShMgr.m_Samp.DrawModel(E_Obj.m_Mo, &E_Obj.m_Mat);

	for each(auto s in E_Obj.m_Shape){
		YsMatrix copy = E_Obj.m_PickUp->m_Buf;
		copy.Move_Local(s->x, 0, s->z);
		copy.Move_Local(0, 0.01f, 0);
		copy.RotateX_Local(90.0f);

		E_Obj.m_PickUp->Draw(copy);
	}
}

void Edit_Put::Update(EditObject & E_Obj)
{
	r_Mat = E_Obj.m_Mat;

	//���[���h���̃I�u�W�F�N�g������
	if (E_Obj.m_wpObj.expired() == true){
		//����(Edit����)�̃L���t���O
		E_Obj.m_KillFlg = true;
	}
}

void Edit_Put::Draw(EditObject & E_Obj)
{
	//�w�莞�ԃG�t�F�N�g���o��

	//���̌���ɖڈ�Ƃ���`�悷��
}