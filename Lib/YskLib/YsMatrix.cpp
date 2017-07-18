#include "YskLib.h"

using namespace YskLib;

const YsMatrix YsMatrix::Identity;


YsMatrix::YsMatrix(	FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
					FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
					FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
					FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44 )
{
	this->_11 = _11;
	this->_12 = _12;
	this->_13 = _13;
	this->_14 = _14;
	this->_21 = _21;
	this->_22 = _22;
	this->_23 = _23;
	this->_24 = _24;
	this->_31 = _31;
	this->_32 = _32;
	this->_33 = _33;
	this->_34 = _34;
	this->_41 = _41;
	this->_42 = _42;
	this->_43 = _43;
	this->_44 = _44;
}


// �s���Z������LookWay�̕����Ɍ�����B
// �@LoockWay ��� �������������x�N�g���̃A�h���X
// �@Up ��� ������̃x�N�g���̃A�h���X
void YsMatrix::SetLookAt(const YsVec3 &LookDir,const YsVec3 &Up)
{
	YsVec3 scale = GetScale();
	YsVec3 v,vX,vY,vZ;
	YsVec3 Look;

	YsVec3::Normalize(Look, LookDir);


	// �O��
	YsVec3::Cross(v, Up, Look);
	v.Normalize();
	if(v.Length() == 0)
		v.x = 1;
	// �O�ς�Y�x�N�g���Z�o
	YsVec3::Cross(vY, Look, v);
	vY.Normalize();
	if(vY.Length() == 0){
		vY.y = 1;
	}

	// �s��̉�]������ݒ�
	_11 = v.x * scale.x;
	_12 = v.y * scale.y;
	_13 = v.z * scale.z;

	_21 = vY.x * scale.x;
	_22 = vY.y * scale.y;
	_23 = vY.z * scale.z;

	_31 = Look.x * scale.x;
	_32 = Look.y * scale.y;
	_33 = Look.z * scale.z;
}

// �s���Z������TargetPos�̈ʒu������悤�ɁA������B
// �@TargetPos ��� ���������W
// �@Up ��� ������̃x�N�g���̃A�h���X
void YsMatrix::SetLookAtPos(const YsVec3 &TargetPos,const YsVec3 &Up)
{
	YsVec3 scale = GetScale();
	YsVec3 v, vX, vY, vZ;
	YsVec3 Look;

	// �����x�N�g��
	Look.x = TargetPos.x - _41;
	Look.y = TargetPos.y - _42;
	Look.z = TargetPos.z - _43;
	Look.Normalize();

	// �O��
	YsVec3::Cross(v, Up, Look);
	v.Normalize();
	// �O�ς�Y�x�N�g���Z�o
	YsVec3::Cross(vY, Look, v);
	vY.Normalize();
	if(vY.Length() == 0){
		vY.y = 1;
	}

	// �s��̉�]������ݒ�
	_11 = v.x * scale.x;
	_12 = v.y * scale.y;
	_13 = v.z * scale.z;

	_21 = vY.x * scale.x;
	_22 = vY.y * scale.y;
	_23 = vY.z * scale.z;

	_31 = Look.x * scale.x;
	_32 = Look.y * scale.y;
	_33 = Look.z * scale.z;
}

void YsMatrix::RotateLookAtRelative(const YsVec3 &Look)
{
	//===================
	// �ǔ��̂��߂̉�]
	//===================

	// ����
	YsVec3 vWay;
	vWay.x = _31;
	vWay.y = _32;
	vWay.z = _33;
	vWay.Normalize();
	// �G�ւ̕���
	YsVec3 vTar;
	YsVec3::Normalize(vTar, Look);

	// ���ςŊp�x�����߂�
	float dot = YsVec3::Dot2(vWay, vTar);
	if(dot >= -1.0f && dot < 1.0f){
		float rAng = acos(dot);

		// �O��
		YsVec3 crs;
		YsVec3::Cross(crs, vWay, vTar);
		// ���K��
		crs.Normalize();
		if(crs.Length() == 0){
			crs.y = 1;
			rAng = 180;
		}

		YsMatrix mRota;
		D3DXMatrixRotationAxis(&mRota,&crs,rAng);

		// ���̏��]�ׁ̈A����������W��0�ɂ��Ă���A��]���������A���̌���W��߂�
		YsVec3 vPos;
		vPos.x = _41;
		vPos.y = _42;
		vPos.z = _43;
		_41 = _42 = _43 = 0;
		(*this) *= mRota;
		_41 = vPos.x;
		_42 = vPos.y;
		_43 = vPos.z;
	}
}

void YsMatrix::RotateLookAtRelativeAngle(const YsVec3 &Look,float MaxAng)
{
	//===================
	// �ǔ��̂��߂̉�]
	//===================

	// ����
	YsVec3 vWay;
	vWay.x = _31;
	vWay.y = _32;
	vWay.z = _33;
	vWay.Normalize();
	// �G�ւ̕���
	YsVec3 vTar;
	YsVec3::Normalize(vTar, Look);


	// ���ςŊp�x�����߂�
	float dot = YsVec3::Dot2(vWay, vTar);
	if(dot < 1.0f && dot > -1.0f){
		// �p�x����
		float Deg = D3DXToDegree(acos(dot));
		if(Deg > MaxAng)
			Deg = MaxAng;

		// �O��
		YsVec3 crs;
		YsVec3::Cross(crs, vWay, vTar);
		if(crs.Length() == 0)return;
		// ���K��
		crs.Normalize();
		if(crs.Length() == 0){
			crs.y = 1;
		}

		YsMatrix mRota;
		D3DXMatrixRotationAxis(&mRota,&crs,YsToRadian(Deg));


		// ���̏��]�ׁ̈A����������W��0�ɂ��Ă���A��]���������A���̌���W��߂�
		YsVec3 vPos;
		vPos.x = _41;
		vPos.y = _42;
		vPos.z = _43;
		_41 = _42 = _43 = 0;
		(*this) *= mRota;
		_41 = vPos.x;
		_42 = vPos.y;
		_43 = vPos.z;
		
	}
}

void YsMatrix::SetBillBoardAxisY(const YsVec3 &vCamPos,const YsVec3 &vY)
{
	YsVec3 vX,vZ,vCam,vW;
	YsVec3::Normalize(vW, vY);
	YsVec3::Sub(vCam, GetPos(), vCamPos);
	vCam.Normalize();
	YsVec3::Cross(vX, vW, vCam);
	vX.Normalize();
	YsVec3::Cross(vZ, vX, vW);
	vZ.Normalize();

	_11 = vX.x;
	_12 = vX.y;
	_13 = vX.z;

	_21 = vW.x;
	_22 = vW.y;
	_23 = vW.z;

	_31 = vZ.x;
	_32 = vZ.y;
	_33 = vZ.z;
}

void YsMatrix::Lerp(YsMatrix& mOut, const YsMatrix& m1, const YsMatrix& m2, float f)
{

	// ��]���������ʐ��`���
	D3DXQUATERNION q, q1, q2;
	D3DXQuaternionRotationMatrix(&q1, &m1);
	D3DXQuaternionRotationMatrix(&q2, &m2);
	D3DXQuaternionSlerp(&q, &q1, &q2, f);
	D3DXQuaternionNormalize(&q, &q);
	// ���W����`���
	YsVec3 pos;
	D3DXVec3Lerp(&pos, (YsVec3*)&m1._41, (YsVec3*)&m2._41, f);
	// �g�����`���
	YsVec3 vScale1 = ((YsMatrix&)m1).GetScale();
	YsVec3 vScale2 = ((YsMatrix&)m2).GetScale();
	YsVec3 scale;
	D3DXVec3Lerp(&scale, &vScale1, &vScale2, f);

	// �l���Z�b�g
	YsMatrix& outM = (YsMatrix&)mOut;
	D3DXMatrixRotationQuaternion(&outM, &q);
	outM.Scale_Local(scale);
	outM.SetPos(pos);
}

void YsMatrix::SplineLerp(YsMatrix& mOut, const YsMatrix& mPrev, const YsMatrix& m1, const YsMatrix& m2, YsMatrix& mNext, float f)
{
	D3DXQUATERNION q, q0, q1, q2, q3;
	D3DXQuaternionRotationMatrix(&q0, &mPrev);
	D3DXQuaternionRotationMatrix(&q1, &m1);
	D3DXQuaternionRotationMatrix(&q2, &m2);
	D3DXQuaternionRotationMatrix(&q3, &mNext);
	D3DXQUATERNION a, b, c;
	D3DXQuaternionSquadSetup(&a, &b, &c, &q0, &q1, &q2, &q3);
	D3DXQuaternionSquad(&q, &q1, &a, &b, &c, f);

	YsVec3 pos;
	D3DXVec3CatmullRom(&pos, (YsVec3*)&mPrev._41, (YsVec3*)&m1._41, (YsVec3*)&m2._41, (YsVec3*)&mNext._41, f);

	D3DXMatrixRotationQuaternion(&mOut, &q);
	mOut._41 = pos.x;
	mOut._42 = pos.y;
	mOut._43 = pos.z;

}


void YsMatrix::ComputePerspectiveInfo(float& outAng, float& outNear, float& outFar) const
{
	YsVec3 v[3];
	v[0].Set(0, 1, 0);	// Near�̏�
	v[1].Set(0, -1, 0);	// Near�̉�
	v[2].Set(0, 0, 1);	// Far�p
	YsMatrix mInvProj = *this;
	mInvProj.CreateInverse();
	v[0].Transform(mInvProj);
	v[1].Transform(mInvProj);
	v[2].Transform(mInvProj);

	outNear = v[0].z;
	outFar	= v[2].z;

	v[0].Normalize();
	v[1].Normalize();

	outAng = YsToDegree(acosf(YsVec3::Dot(v[0], v[1])));	// ����p

}
