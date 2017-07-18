

#include "main.h"

#include "Game\GameWorld\GameWorld.h"

#include "CollisionEngine.h"



BOOL CheckViewFrustum(SPtr<CharacterBase> _MyData, SPtr<YsMesh> _Mesh)
{

	//���g����J�����܂ł̋����i�[�p
	YsVec3 vTargetVec = _MyData->GetPos();

	//���������g�̍��W�[�J�����̍��W
	vTargetVec = vTargetVec - VF.GetCamPos();

	for (int i = 0; i < 4; i++)
	{
		//���ς����߂�
		float Len = 0;

		//���ς�Ԃ�
		Len = D3DXVec3Dot(&vTargetVec, &VF.GetNormal(i));

		//���ςŋ��߂����� > ���g�̃o�E���f�B���O�X�t�B�A�̔��a
		if (Len > _Mesh->GetBSRadius()* _MyData->GetScale().x)
		{
			//�`�悵�Ȃ�
			return false;
		}
	}

	//�`�悷��
	return true;
}

void ViewFrustum::Create(YsMatrix _Proj, YsMatrix _View, YsMatrix _Cam)
{

	//�ˉe�s��ƃr���[�s�������
	YsMatrix mVP = _View*_Proj;

	//�J�����̃��[���h���W�擾
	YsMatrix mInvV = _View;

	//�t�s��쐬
	mInvV.CreateInverse();

	//�J�������W�擾
	m_CamPos = _Cam.GetPos();

	//�t�s��ɂ���
	mVP.CreateInverse();

	//�����䒸�_��
	enum VERTEX
	{
		LeftTop = 0,
		RightTop = 1,
		LeftBottom = 2,
		RightBottom = 3,
		MAX,
	};

	//�����䒸�_�i�[�p
	YsVec3 vFrustumVertex[VERTEX::MAX] = {
		YsVec3(-1, 1, 1),	//LeftTop
		YsVec3(1, 1, 1),	//RightTop
		YsVec3(-1, -1, 1),//LeftBottom
		YsVec3(1, -1, 1)	//RightBottom
	};

	//���_�܂ł̋����i�[�p
	YsVec3 VertexLength[VERTEX::MAX] = {
		YsVec3(0, 0, 0),	//LeftTop�܂ł̋���
		YsVec3(0, 0, 0),	//RigthTop�܂ł̋���
		YsVec3(0, 0, 0),	//LeftBottom�܂ł̋���
		YsVec3(0, 0, 0)	//RightBottom�܂ł̋���
	};

	//���_�������[�v
	for (int i = 0; i < VERTEX::MAX; i++)
	{

		//���������s��Ǝ�����̒��_���������邽�߂̎ˉe�s��
		YsMatrix mPVertex;

		//���_�s�񉻗p
		YsMatrix mTransVertex;

		//2D���_���s��
		D3DXMatrixTranslation(&mTransVertex, vFrustumVertex[i].x, vFrustumVertex[i].y, vFrustumVertex[i].z);

		//3D��ԂƂ��ĕۊǂ���
		D3DXVec3TransformCoord(&vFrustumVertex[i], &vFrustumVertex[i], &mVP);

		//�J�����Ƃ̋������i�[
		VertexLength[i] = vFrustumVertex[i] - m_CamPos;
	}

	//���ꂼ��̒��_���m�����т��ꂼ��̖ʂɑ΂��Ă̊O�ς����߂�

	enum NVERTEX
	{
		NLeft = 0,
		NTop = 1,
		NRight = 2,
		NBottom = 3,
		NMAX
	};

	YsVec3 Cross[NVERTEX::NMAX];

	//NLeft
	D3DXVec3Cross(&m_vN[NVERTEX::NLeft], &vFrustumVertex[VERTEX::LeftBottom], &VertexLength[VERTEX::LeftTop]);

	//NTop
	D3DXVec3Cross(&m_vN[NVERTEX::NTop], &vFrustumVertex[VERTEX::LeftTop], &VertexLength[VERTEX::RightTop]);

	//NRight
	D3DXVec3Cross(&m_vN[NVERTEX::NRight], &vFrustumVertex[VERTEX::RightTop], &VertexLength[VERTEX::RightBottom]);

	//NButtom
	D3DXVec3Cross(&m_vN[NVERTEX::NBottom], &vFrustumVertex[VERTEX::RightBottom], &VertexLength[VERTEX::LeftBottom]);

	//���K��
	for (int i = 0; i < NVERTEX::NMAX; i++)
	{
		D3DXVec3Normalize(&m_vN[i], &m_vN[i]);
	}
}













float BaseHitObj::CalcMassRatio(float mass)
{
	// �����̎��ʂ�0�Ȃ�A�����͕s��
	if (m_Mass == 0)return 0.0f;
	// ����̎��ʂ�0�Ȃ�A�����͓���
	if (mass == 0)return 1.0f;

	// �������ʂ���
	return mass * (1.0f / (m_Mass + mass));
}

// �� vs ���̑�
bool HitObj_Sphere::HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2)
{

	// vs ��
	if (obj->m_Shape == HitShapes::SPHERE)	// �^����
	{
		HitObj_Sphere* sph = static_cast<HitObj_Sphere*>(obj);	// �����_�E���L���X�g

		// ������
		YsVec3 vDir = sph->m_vPos - m_vPos;
		float dist = vDir.Length();

		// �������ĂȂ��Ȃ�I��
		if (dist >= m_Rad + sph->m_Rad)return false;

		// �������Ă�Ȃ猋�ʂ�o�^
		vDir.Normalize();
		float f = (m_Rad + sph->m_Rad) - dist;	// �߂荞��ł��鋗�������߂�
		YsVec3 vPush = (-vDir * f);				// �����߂����߂̃x�N�g�������߂�

		// �����Ɍ��ʂ��L������
		if (resData1){
			// ���ʃf�[�^�ǉ�
			resData1->m_youHitObj = obj;
			resData1->m_vHitPos = m_vPos + vDir*(dist*0.5f);
			resData1->m_vPush = vPush;
		}

		// ����Ɍ��ʂ��L������
		if (resData2){
			// ���ʃf�[�^�ǉ�
			resData2->m_youHitObj = this;
			resData2->m_vHitPos = sph->m_vPos + -vDir*(dist*0.5f);
			resData2->m_vPush = -vPush;
		}

		return true;
	}
	// vs Ray
	else if (obj->m_Shape == HitShapes::RAY){
		HitObj_Ray* ray = static_cast<HitObj_Ray*>(obj);	// �����_�E���L���X�g

		YsVec3 vQPush;		// ���������Ă���x�N�g��
		float dist;				// ���C��������������

		// �� vs ���C����
		if (YsCollision::SphereToRay(m_vPos, m_Rad, ray->m_vPos1, ray->m_vDir, &dist, &vQPush) == false){
			// �������ĂȂ��Ȃ�I��
			return false;
		}

		// �����`�F�b�N
		if (dist > ray->m_RayLen)return false;

		// �������Ă�Ȃ�A���ʂ�o�^

		// �����Ɍ��ʂ��L������
		if (resData1){
			// ���ʃf�[�^�ǉ�
			resData1->m_youHitObj = obj;
			resData1->m_vPush = vQPush;
			resData1->m_vHitPos = ray->m_vPos1 + ray->m_vDir*dist;
		}
		// ����Ɍ��ʂ��L������
		if (resData2){
			// ���ʃf�[�^�ǉ�
			resData2->m_youHitObj = this;
			resData2->m_RayDist = dist;
			resData2->m_FaceIdx = -1;
			resData2->m_vHitPos = ray->m_vPos1 + ray->m_vDir*dist;
		}

		return true;

	}
	// vs ���b�V��
	else if (obj->m_Shape == HitShapes::MESH){
		HitObj_Mesh* mesh = static_cast<HitObj_Mesh*>(obj);

		//==================================================
		// �� vs ���b�V������
		//==================================================
		YsVec3 vOut;
		std::vector<YsCollision::MTS_HitData>	hitData;	// �q�b�g���ʂ̃f�[�^�z��
		if (YsCollision::MeshToSphere(*mesh->m_pMesh, &mesh->m_Mat, m_vPos, m_Rad, &vOut, 0, -1, &hitData) == false){
			// �������ĂȂ��Ȃ�I��
			return false;
		}

		// �����Ɍ��ʂ��L������
		if (resData1){
			// ���ʃf�[�^�ǉ�
			resData1->m_youHitObj = obj;
			resData1->m_vHitPos = hitData[0].vHitPos;
			resData1->m_vPush = vOut;
		}
		// ����Ɍ��ʂ��L������
		if (resData2){
			// ���ʃf�[�^�ǉ�
			resData2->m_youHitObj = this;
			resData2->m_vHitPos = hitData[0].vHitPos;
			resData2->m_vPush = -vOut;
		}

		return true;

	}

	return false;
}

void HitObj_Sphere::DebugDraw(float alpha)
{
	YsVec4 mulCol(1, 1, 1, alpha);

	// �����C���`��
	YsMatrix m;
	m.SetPos(m_vPos);
	ShMgr.m_Samp.DrawSphereLine(m_Rad, &(m_Debug_Color * mulCol), &m);

	// �����������̂ւ̃��C���`��
	for (auto& res : m_HitResTbl){
		ShMgr.m_Samp.DrawLine(m_vPos, res.m_vHitPos, &YsVec4(1, 1, 0, alpha));
	}

}


// ���C vs ���̑�
bool HitObj_Ray::HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2)
{
	// vs ��
	if (obj->m_Shape == HitShapes::SPHERE){
		// �����̊֐����Ă�
		return obj->HitTest(this, resData2, resData1);
	}
	// vs Ray
	else if (obj->m_Shape == HitShapes::RAY){
		// ���C vs ���C�͔��肵�Ȃ�
		return false;
	}
	// vs ���b�V��
	else if (obj->m_Shape == HitShapes::MESH){
		HitObj_Mesh* mesh = static_cast<HitObj_Mesh*>(obj);

		//==================================================
		// ���C vs ���b�V������
		//==================================================
		float dist;
		UINT faceIdx;
		if (YsCollision::RayToMesh(*mesh->m_pMesh, &mesh->m_Mat, m_vPos1, m_vPos2, &dist, &faceIdx) == false)
		{
			// �������ĂȂ��Ȃ�I��
			return false;
		}

		// ��������
		if (dist > m_RayLen)return false;

		// �����Ɍ��ʂ��L������
		if (resData1){
			// ���ʃf�[�^�ǉ�
			resData1->m_youHitObj = obj;
			resData1->m_vHitPos = m_vPos1 + m_vDir*dist;
			resData1->m_RayDist = dist;
			resData1->m_FaceIdx = faceIdx;
		}
		// ����Ɍ��ʂ��L������
		if (resData2){
			// ���ʃf�[�^�ǉ�
			resData2->m_youHitObj = this;
			resData2->m_vHitPos = m_vPos1 + m_vDir*dist;
		}

		return true;

	}
	return false;
}

// �f�o�b�O�`��
void HitObj_Ray::DebugDraw(float alpha)
{
	YsVec4 mulCol(1, 1, 1, alpha);

	// ���C���`��
	ShMgr.m_Samp.DrawLine(m_vPos1, m_vPos2, &(m_Debug_Color*mulCol));

	// ���������ꏊ�ɉ~��`��
	for (auto& res : m_HitResTbl){
		YsMatrix m;
		m.SetPos(m_vPos1 + m_vDir*res.m_RayDist);
		m.SetLookAt(m_vDir, YsVec3(0, 1, 0));
		ShMgr.m_Samp.DrawCircleLine(0.1f, &YsVec4(1, 1, 0, alpha), &m);
	}
}

// ���b�V�� vs ���̑�
bool HitObj_Mesh::HitTest(BaseHitObj* obj, HitResData* resData1, HitResData* resData2)
{

	// vs ��
	if (obj->m_Shape == HitShapes::SPHERE){
		// �����̊֐����Ă�
		return obj->HitTest(this, resData2, resData1);
	}
	// vs Ray
	else if (obj->m_Shape == HitShapes::RAY){
		// ���C���̊֐����Ă�
		return obj->HitTest(this, resData2, resData1);
	}
	// vs ���b�V��
	else if (obj->m_Shape == HitShapes::MESH){
		// ���b�V�����m�̔���͂��Ȃ�
		return false;
	}
	return false;

}

void HitObj_Mesh::DebugDraw(float alpha)
{
	YsVec4 mulCol(1, 1, 1, alpha);

	// ���C���[�t���[���Ń��b�V����`��
	YsRasterizeState save;
	save.SetAll_GetState();

	YsRasterizeState rs;
	rs.SetAll_Standard();
	rs.Set_FillMode_Wireframe();
	rs.SetState();

	ShMgr.m_Samp.SetLightEnable(false);
	ShMgr.m_Samp.DrawMeshOnly(*m_pMesh, &(m_Debug_Color*mulCol), &m_Mat);
	ShMgr.m_Samp.SetLightEnable(true);

	save.SetState();
}


//======================================================
//
// CollisionEngine
//
//======================================================

void CollisionEngine::Test(BaseHitObj* atkObj)
{
	// atkObj vs �SDefObj

	// ���茋�ʃf�[�^���N���A����
	atkObj->m_HitResTbl.clear();

	HitResData res;
	BaseHitObj* defObj;

	//-----------------------------------------------
	// �SDefList�Ɣ�����s
	//-----------------------------------------------
	for (UINT di = 0; di<m_DefList.size(); di++){
		defObj = m_DefList[di].get();

		// �ėp���O�`�F�b�N�֐�
		if (atkObj->m_BroadPhaseProc){
			if (atkObj->m_BroadPhaseProc(defObj) == false)continue;
		}

		// �������g�͏��O
		if (atkObj == defObj)continue;

		// ���肵�ėǂ��`��H
		if (!(atkObj->m_ShapeFilter & defObj->m_Shape)){
			continue;
		}

		// ����������̕��͏��O
		// lock()�͒x������g��Ȃ��B�@������3000x3000���肵���Ƃ��A[lock():300ms] vs [expired() + _Get():7ms]�ƃ_���`
		if (WPtr_Get(atkObj->m_wpTask) == WPtr_Get(defObj->m_wpTask)){
			continue;
		}
		// lock()�o�[�W�����͒x������A�����ł͎g��Ȃ��悤��
		//		if(atkObj->m_wpTask.lock() == defObj->m_wpTask.lock())continue;

		// ����[�U�O���[�v] vs ����[�h�O���[�v]
		if (!(atkObj->m_AtkFilter & defObj->m_DefFilter))continue;

		// ��L�̃`�F�b�N�����ׂăN���A�������̓��m�����A������s
		if (atkObj->HitTest(defObj, &res, nullptr)){
			// ���ʂ�o�^
			atkObj->m_HitResTbl.push_back(res);
		}
	}

	//-----------------------------------------------
	// �q�b�g��ԃt���O����
	//-----------------------------------------------

	// ����HIT�t���O��HIT�I���t���O��OFF
	atkObj->m_HitState &= ~HitStates::ENTER;	// OFF
	atkObj->m_HitState &= ~HitStates::EXIT;		// OFF


	// ����HIT�Ȃ�HIT�t���O�n��ON
	if (atkObj->m_HitResTbl.size() > 0){
		if (!(atkObj->m_HitState & HitStates::STAY)){	// �O���HIT���ł͂Ȃ�
			atkObj->m_HitState |= HitStates::ENTER;		// ����HIT ON
			atkObj->m_HitState |= HitStates::STAY;		// HIT�� ON
		}
	}
	// ����HIT���Ă��Ȃ��Ƃ�
	else{
		// HIT���Ȃ�HIT���t���OOFF���āAHIT�I���t���OON
		if (atkObj->m_HitState & HitStates::STAY){
			atkObj->m_HitState |= HitStates::EXIT;	// ON
			atkObj->m_HitState &= ~HitStates::STAY;	// OFF
		}
	}
}

void CollisionEngine::Run()
{

	/*
	//=================================
	// �}���`�X���b�h�ŕ��񔻒���s
	//=================================
	std::atomic<int> nowIdx = 0;	// �A�g�~�b�N�ϐ�
	auto testThread = [this, &nowIdx]() {
	while(1){
	int idx = nowIdx.fetch_add(1);		// nowIdx�����Z���A���Z�u�O�v�̒l��Ԃ�
	if(idx >= m_AtkList.size())break;

	// atkObj vs �Sm_DefList
	Test(m_AtkList[idx].get());
	}
	};

	// CPU�� �擾
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	std::vector<std::future<void>> fut(sys.dwNumberOfProcessors);

	// �X���b�h�N��
	for(DWORD i = 0;i<sys.dwNumberOfProcessors;i++){
	fut[i] = std::async(std::launch::async | std::launch::deferred, testThread);
	}
	// �X���b�h�I���ҋ@
	for(DWORD i = 0; i<sys.dwNumberOfProcessors; i++){
	fut[i].wait();
	}
	*/

	//=================================
	// �V���O���X���b�h�ł̔���
	//=================================
	// �����蔻����s
	// �S�Ă̍U�^�C�v���h�^�C�v�ɑ΂��Ĕ�����s��
	for (UINT ai = 0; ai<m_AtkList.size(); ai++){	// ���肷�鑤

		// atkObj vs �Sm_DefList
		Test(m_AtkList[ai].get());

	}

	// �q�b�g�������̂́A�ʒm�֐������s����
	for (UINT ai = 0; ai<m_AtkList.size(); ai++){
		BaseHitObj* atkObj = m_AtkList[ai].get();

		// �P�ł��q�b�g���Ă���Βʒm�֐������s����
		if (atkObj->m_HitResTbl.size() > 0){

			// �q�b�g�����ŏ��̈��ڂ̂�
			if (atkObj->m_HitState & HitStates::ENTER && atkObj->m_OnHitEnter){
				atkObj->m_OnHitEnter(m_AtkList[ai]);
			}
			// �q�b�g���Ă�Ԃ�����
			if (atkObj->m_HitState & HitStates::STAY && atkObj->m_OnHitStay){
				atkObj->m_OnHitStay(m_AtkList[ai]);
			}
		}

		// �q�b�g���Ȃ��Ȃ����ŏ��̈��ڂ̂�
		if (atkObj->m_HitState & HitStates::EXIT && atkObj->m_OnHitExit){
			atkObj->m_OnHitExit(m_AtkList[ai]);
		}
	}
}

void CollisionEngine::DebugDraw(float alpha)
{
	// �SObject���f�o�b�O�`��

	for (UINT ai = 0; ai<m_AtkList.size(); ai++){
		m_AtkList[ai]->DebugDraw(alpha);
	}
	for (UINT di = 0; di<m_DefList.size(); di++){
		m_DefList[di]->DebugDraw(alpha);
	}
}