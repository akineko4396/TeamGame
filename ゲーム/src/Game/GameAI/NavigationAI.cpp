#include "main.h"
#include "NavigationAI.h"

void NavigationAI::Init()
{
	m_gmWay = YsDx.GetResStg()->LoadMesh("data/model/WayPoint/WayPoint.x");
	m_moWay.SetModel(m_gmWay);
}

void NavigationAI::Navigate(YsVec3 _MyPos, YsVec3 _TargetPos)
{

}

void NavigationAI::CreateWayPoints(SPtr<YsSingleModel>& _sm, YsVec3& _pos)
{
	//�n�`�̃T�C�Y�����߂�---------------------------------
	YsVec3 vMin = _sm->GetMesh()->GetAABBMin();
	YsVec3 vMax = _sm->GetMesh()->GetAABBMax();
	YsVec2 vSize = YsVec2(vMax.x - vMin.x, vMax.z - vMin.z);		//X...����,Y...���s

	//���C����󂩂�n�ʂ֔�΂�-------------------------
	//���C�̔�΂����̍��W�̏����l
	YsVec3 vRayPos = _pos;
	vRayPos.x = (vRayPos.x - vSize.x / 2) + 0.5f;
	vRayPos.z = (vRayPos.z - vSize.y / 2) + 0.5f;
	vRayPos.y = 10;
	float move = 1;		//���C�̂�����
	float fDist = 0;	//���������_�܂ł̒���

	for (; vRayPos.z < vSize.y; vRayPos.x += move)
	{
		if (vRayPos.x > vSize.x) {
			vRayPos.x = _pos.x - vSize.x / 2 + 0.5f;
			vRayPos.z += move;
		}

		//���C�ƃ��b�V������
		BOOL bHit = YsCollision::RayToMesh(*_sm->GetMesh(), NULL, vRayPos, YsVec3(vRayPos.x, vRayPos.y - 20, vRayPos.z), &fDist, NULL);

		//���C������������
		if (bHit) {
			m_WayPointList.push_back(YsVec3(vRayPos.x, vRayPos.y - fDist, vRayPos.z));
		}
	}
}

void NavigationAI::UpdateWayPoint(SPtr<YsSingleModel>& _sm, YsVec3& _pos, int _num) 
{
	YsVec3 vMin = _sm->GetMesh()->GetAABBMin();
	YsVec3 vMax = _sm->GetMesh()->GetAABBMax();
	float fDist = 0; //���������_�܂ł̒���

	for (auto vPos : m_WayPointList) {
		if (vMin.x < vPos.x && vMin.y < vPos.y && vMin.z < vPos.z &&
			vMax.x > vPos.x && vMax.y > vPos.y && vMax.z > vPos.z) {
			//�I�u�W�F�N�g�̒��ɂ͏�ɃE�F�C�|�C���g��u���Ȃ����̂�����
			//if(_num!= CharacterBase::ID::ARM){}
			//���C�ƃ��b�V������
			BOOL bHit = YsCollision::RayToMesh(*_sm->GetMesh(), NULL, YsVec3(vPos.x, vPos.y + 10, vPos.z), vPos, &fDist, NULL);
			if (bHit) {
				vPos = YsVec3(vPos.x, vPos.y + 10 - fDist, vPos.z);
			}
		}
	}
}

void NavigationAI::DebugWayDraw()
{
	ShMgr.m_Samp.DrawModel(m_moMap, &m_mMap);
	for (auto vPos : m_WayPointList) {
		m_mWay.CreateMove(YsVec3(vPos.x, vPos.y + 0.1f, vPos.z));
		ShMgr.m_Samp.DrawModel(m_moWay, &m_mWay);
		//ShMgr.m_Samp.DrawSphereLine(0.3f, &YsVec4(1, 1, 1, 1), &m_mWay);
	}
}