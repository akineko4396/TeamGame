
#include "main.h"

#include "GameLib.h"


//----------------------------------------------
// ���s�񂩂�Y���̐�Ίp�擾
// �߂�l : float
// �����P : YsMatrix
//----------------------------------------------
float GetAngleY(YsMatrix &_Mat)
{
	YsMatrix m = _Mat;

	YsVec3 Vec;
	YsVec3 BaseVec(0, 0, 1);

	m._41 = 0;
	m._42 = 0;
	m._43 = 0;
	D3DXVec3TransformCoord(&Vec, &BaseVec, &m);	//�L�����̐i�s�����̃x�N�g�����擾
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
// �����C�����ɓ��������ʒu���擾
// �߂�l : YsVec3
// �����P : ���C�̊J�n�ʒu
// �����Q : ���C�̏I���ʒu
// �����R : ����Ŏ�ꂽ����
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
// ���l���w�肵���傫���ɋ�؂�
// (size��1.0f���w�肷���0.5(1.0f/2)�Ő؂�グ/�؂�̂�)
// �����P : ��؂肽���� 
// �����Q : ��؂�傫��
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