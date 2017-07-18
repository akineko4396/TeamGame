#include "YskLib.h"

using namespace YskLib;


const YsVec2 YsVec2::Zero(0, 0);	//!< �S�v�f0�x�N�g��
const YsVec2 YsVec2::One(1, 1);		//!< �S�v�f1�x�N�g��

void YsVec2::Homing(const YsVec2& vTargetDir, float MaxAng)
{
	// ����
	YsVec3 vWay(x,y,0);
	vWay.Normalize();
	// �G�ւ̕���
	YsVec3 vTar(vTargetDir.x, vTargetDir.y, 0);
	vTar.Normalize();


	// ���ςŊp�x�����߂�
	float dot = YsVec3::Dot2(vWay, vTar);
	if(dot < 1.0f){
		float Deg;
		YsVec3 crs;
		// �����΂̂Ƃ��͓K���ɋȂ���
		if(dot == -1.0f){
			crs.Set(0,0,1);
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
				crs.z = 1;
			}
		}

		// ��]�s��쐬
		D3DXMATRIX mRota;
		D3DXMatrixRotationAxis(&mRota,&crs,YsToRadian(Deg));

		TransformNormal(mRota);
		
	}
}
