
#include "main.h"

#include "GameLib.h"


//----------------------------------------------
// ▼行列からY軸の絶対角取得
// 戻り値 : float
// 引数１ : YsMatrix
//----------------------------------------------
float GetAngleY(YsMatrix &_Mat)
{
	YsMatrix m = _Mat;

	YsVec3 Vec;
	YsVec3 BaseVec(0, 0, 1);

	m._41 = 0;
	m._42 = 0;
	m._43 = 0;
	D3DXVec3TransformCoord(&Vec, &BaseVec, &m);	//キャラの進行方向のベクトルを取得
	Vec.Normalize();

	double Len_A = pow((Vec.x*Vec.x) + (Vec.y*Vec.y) + (Vec.z*Vec.z), 0.5);
	double Len_B = pow((BaseVec.x*BaseVec.x) + (BaseVec.y*BaseVec.y) + (BaseVec.z*BaseVec.z), 0.5);

	double cos_sita = D3DXVec3Dot(&Vec, &BaseVec);

	double sita = acos(cos_sita);

	sita = sita * 180 / D3DX_PI;

	if (Vec.x < 0){
		return 360.0f - sita - 180.0f;
	}
	return sita - 180.0f;
}


//----------------------------------------------
// ▼レイ判定後に当たった位置を取得
// 戻り値 : YsVec3
// 引数１ : レイの開始位置
// 引数２ : レイの終了位置
// 引数３ : 判定で取れた距離
//----------------------------------------------
YsVec3 GetHitPos(YsVec3 vStart, YsVec3 vEnd, float Dis){

	YsVec3 Point;
	YsVec3 Len = vStart - vEnd;
	float l;
	l = D3DXVec3Length(&Len);
	Point = (Len*(Dis / l)) + vStart;

	return Point;
}


//----------------------------------------------
// ▼値を指定した大きさに区切る
// (sizeに1.0fを指定すると0.5(1.0f/2)で切り上げ/切り捨て)
// 引数１ : 区切りたい数 
// 引数２ : 区切る大きさ
//----------------------------------------------
float PointToCell(float _f, float _size)
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