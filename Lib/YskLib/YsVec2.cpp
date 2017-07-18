#include "YskLib.h"

using namespace YskLib;


const YsVec2 YsVec2::Zero(0, 0);	//!< 全要素0ベクトル
const YsVec2 YsVec2::One(1, 1);		//!< 全要素1ベクトル

void YsVec2::Homing(const YsVec2& vTargetDir, float MaxAng)
{
	// 方向
	YsVec3 vWay(x,y,0);
	vWay.Normalize();
	// 敵への方向
	YsVec3 vTar(vTargetDir.x, vTargetDir.y, 0);
	vTar.Normalize();


	// 内積で角度を求める
	float dot = YsVec3::Dot2(vWay, vTar);
	if(dot < 1.0f){
		float Deg;
		YsVec3 crs;
		// 正反対のときは適当に曲げる
		if(dot == -1.0f){
			crs.Set(0,0,1);
			Deg = MaxAng;
		}
		else{
			// 角度制限
			Deg = D3DXToDegree(acos(dot));
			if(Deg > MaxAng)
				Deg = MaxAng;

			// 外積
			YsVec3::Cross(crs, vWay, vTar);
			// 正規化
			crs.Normalize();
			if(crs.Length() == 0){
				crs.z = 1;
			}
		}

		// 回転行列作成
		D3DXMATRIX mRota;
		D3DXMatrixRotationAxis(&mRota,&crs,YsToRadian(Deg));

		TransformNormal(mRota);
		
	}
}
