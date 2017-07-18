#include "YskLib.h"

using namespace YskLib;

const YsVec3 YsVec3::Zero(0, 0, 0);
const YsVec3 YsVec3::One(1, 1, 1);
const YsVec3 YsVec3::Up(0, 1, 0);
const YsVec3 YsVec3::Down(0, -1, 0);
const YsVec3 YsVec3::Left(-1, 0, 0);
const YsVec3 YsVec3::Right(1, 1, 0);
const YsVec3 YsVec3::Front(0, 0, 1);
const YsVec3 YsVec3::Back(0, 0, -1);

void YsVec3::Homing(const YsVec3& vTargetDir, float MaxAng)
{
	// ����
	YsVec3 vWay(x,y,z);
	vWay.Normalize();
	// �G�ւ̕���
	YsVec3 vTar = vTargetDir;
	vTar.Normalize();

	// ���ςŊp�x�����߂�
	float dot = YsVec3::Dot2(vWay, vTar);
	if(dot < 1.0f){
		float Deg;
		YsVec3 crs;
		// �����΂̂Ƃ��͓K���ɋȂ���
		if(dot == -1.0f){
			crs.Set(0,1,0);
			Deg = MaxAng;
		}
		else{
			// �p�x����
			Deg = D3DXToDegree(acos(dot));
			if(Deg > MaxAng)
				Deg = MaxAng;

			// �O��
			YsVec3::Cross(crs, vWay, vTar);
			// ���K��
			crs.Normalize();
			if(crs.Length() == 0){
				crs.y = 1;
			}
		}

		// ��]�s��쐬
		D3DXMATRIX mRota;
		D3DXMatrixRotationAxis(&mRota,&crs,YsToRadian(Deg));

		TransformNormal(mRota);
		
	}
}
