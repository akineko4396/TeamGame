//===============================================================
//! @file YsMatrix.h
//! @brief �s��N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsMatrix_h__
#define __YsMatrix_h__

namespace YskLib{

//! @brief D3DX�̍s�񂩂�ADirectXTK�̍s��փ|�C���^�^�ϊ�
#define YsToXNAMatrix(v) ((DirectX::SimpleMath::Matrix*)v)

//==========================================================================================================
//! @brief �s�񑀍�N���X
//!
//! @ingroup Math
//==========================================================================================================
class YsMatrix : public D3DXMATRIX{
public:
	//============================================================
	// �����o�֐�
	//============================================================
	//
	YsMatrix() : D3DXMATRIX(1.0f, 0,    0,    0,
							0,    1.0f, 0,    0,
							0,    0,    1.0f, 0,
							0,    0,    0,    1.0f)
	{}

	//
	YsMatrix(const D3DXMATRIX &m){
		_11 = m._11;
		_12 = m._12;
		_13 = m._13;
		_14 = m._14;
		_21 = m._21;
		_22 = m._22;
		_23 = m._23;
		_24 = m._24;
		_31 = m._31;
		_32 = m._32;
		_33 = m._33;
		_34 = m._34;
		_41 = m._41;
		_42 = m._42;
		_43 = m._43;
		_44 = m._44;
	}
	//
	YsMatrix(FLOAT _11, FLOAT _12, FLOAT _13, FLOAT _14,
			FLOAT _21, FLOAT _22, FLOAT _23, FLOAT _24,
			FLOAT _31, FLOAT _32, FLOAT _33, FLOAT _34,
			FLOAT _41, FLOAT _42, FLOAT _43, FLOAT _44 );


	//========================================================================
	//
	// �s��u�����v�n�֐�����s����쐬���A�������Ă���(���ΓI�ω�)
	//
	//========================================================================
	//! @brief �ړ������[���h�����B
	//! @param[in]	x �c x���W
	//! @param[in]	y �c y���W
	//! @param[in]	z �c z���W
	void Move(float x, float y, float z)
	{
		_41 += x;
		_42 += y;
		_43 += z;
	}

	//! @brief �ړ������[���h�����B
	//! @param[in]	v �c �x�N�g��
	void Move(const YsVec3 &v)
	{
		_41 += v.x;
		_42 += v.y;
		_43 += v.z;
	}

	//! @brief �ړ������[�J�������B
	//! @param[in]	x �c x���W
	//! @param[in]	y �c y���W
	//! @param[in]	z �c z���W
	void Move_Local(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixTranslation(&TmpMat,x,y,z);
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}

	//! @brief �ړ������[�J�������B
	//! @param[in]	v �c �x�N�g��
	void Move_Local(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixTranslation(&TmpMat,v.x, v.y, v.z);
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}

	//! @brief X��]�����[���h�����B
	//! @param[in]	Ang �c ��]�p�x(�x)
	void RotateX(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationX(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this, this, &TmpMat);// ����
	}

	//! @brief Y��]�����[���h�����B
	//! @param[in]	Ang �c ��]�p�x(�x)
	void RotateY(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationY(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this, this, &TmpMat);// ����
	}

	//! @brief Z��]�����[���h�����B
	//! @param[in]	Ang �c ��]�p�x(�x)
	void RotateZ(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationZ(&TmpMat, YsToRadian(Ang));
		D3DXMatrixMultiply(this,this, &TmpMat);// ����
	}

	//! @brief �w�莲��]�����[���h�����B
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateAxis(const YsVec3 &vAxis, float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationAxis(&TmpMat, &vAxis, YsToRadian(Ang));
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
	}

	//! @brief X��]�����[���h����(���W�͕ω������Ȃ�)�B
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateX_Normal(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationX(&TmpMat, YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief Y��]�����[���h����(���W�͕ω������Ȃ�)�B
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateY_Normal(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationY(&TmpMat,YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief Z��]�����[���h����(���W�͕ω������Ȃ�)�B
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateZ_Normal(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationZ(&TmpMat,YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this, this, &TmpMat);// ����
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief �w�莲��]�����[���h����(���W�͕ω������Ȃ�)�B
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateAxis_Normal(const YsVec3 &vAxis, float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationAxis(&TmpMat, &vAxis, YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief X��]�����[�J�������B
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateX_Local(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationX(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}

	//! @brief Y��]�����[�J�������B
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateY_Local(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationY(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}

	//! @brief Z��]�����[�J�������B
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateZ_Local(float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationZ(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}

	//! @brief �w�莲��]�����[�J�������B
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void RotateAxis_Local(const YsVec3 &vAxis, float Ang)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixRotationAxis(&TmpMat, &vAxis, YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}

	//! @brief �g������[���h�����B
	//! @param[in]	x		�c x�g�嗦
	//! @param[in]	y		�c y�g�嗦
	//! @param[in]	z		�c z�g�嗦
	void Scale(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixScaling(&TmpMat,x,y,z);
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
	}

	//! @brief �g������[���h�����B
	//! @param[in]	v		�c �g�嗦(x,y,z)
	void Scale(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixScaling(&TmpMat,v.x, v.y, v.z);
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
	}

	//! @brief �g������[�J�������B
	//! @param[in]	x		�c x�g�嗦
	//! @param[in]	y		�c y�g�嗦
	//! @param[in]	z		�c z�g�嗦
	void Scale_Local(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixScaling(&TmpMat,x,y,z);
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}
	//! @brief �g������[�J�������B
	//! @param[in]	v		�c �g�嗦(x,y,z)
	void Scale_Local(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixScaling(&TmpMat,v.x, v.y, v.z);
		D3DXMatrixMultiply(this,&TmpMat,this);// ����
	}

	//! @brief �g������[���h����(���W�͕ω����܂���)�B
	//! @param[in]	x		�c x�g�嗦
	//! @param[in]	y		�c y�g�嗦
	//! @param[in]	z		�c z�g�嗦
	void Scale_Normal(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixScaling(&TmpMat,x,y,z);

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief �g������[���h����(���W�͕ω����܂���)�B
	//! @param[in]	v		�c �g�嗦(x,y,z)
	void Scale_Normal(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixScaling(&TmpMat,v.x, v.y, v.z);

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// ����
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief Look�̕����ɏu���Ɍ�����(���ΓI)�B�������n�����ǁA���_���S�łȂ��A���̏�ŉ�]���܂�
	//! @param[in]	Look	�c ������������
	void RotateLookAtRelative(const YsVec3 &Look);

	//! @brief Look�̕�����MaxAng�̊p�x����������(���ΓI)�B�������n�����ǁA���_���S�łȂ��A���̏�ŉ�]���܂�
	//! @param[in]	Look	�c ������������
	//! @param[in]	MaxAng	�c �ő�p�x
	void RotateLookAtRelativeAngle(	const YsVec3 &Look,
									float MaxAng);

	//! @brief �w�肵���s������[���h����(���ɍ���)
	//! @param[in]	mat		�c ��������s��
	void Multiply(const YsMatrix &mat)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixMultiply(this, this, &mat);// ����
	}

	//! @brief �w�肵���s������[�J������(��O�ɍ���)
	//! @param[in]	mat		�c ��������s��
	void Multiply_Local(const YsMatrix &mat)
	{
		D3DXMATRIX TmpMat;		// ��Ɨp
		D3DXMatrixMultiply(this, &mat, this);// ����
	}
	static void Multiply(YsMatrix &outMat, const YsMatrix &mat1, const YsMatrix &mat2)
	{
		D3DXMatrixMultiply(&outMat, &mat1, &mat2);// ����
	}

	//========================================================================
	//
	// �s��u�쐬�E����v�n�֐�����s��̓��e�𒼐ڑ��삷��֐�(��ΓI�ω�)
	//
	//========================================================================
	//! @brief �P�ʍs��ɂ���B
	void CreateIdentity()
	{
		D3DXMatrixIdentity(this);
	}

	//! @brief ��]�̂ݒP�ʍs��ɂ���B
	void CreateIdentityRotate()
	{
		_11 = 1;_12 = 0;_13=0;_14 = 0;
		_21 = 0;_22 = 1;_23=0;_24 = 0;
		_31 = 0;_32 = 0;_33=1;_34 = 0;
	}

	//! @brief �w��s���P�ʍs��ɂ���
	//! @param[out]	m	�c �P�ʍs��ɂ������s��
	static void CreateIdentity(YsMatrix &m){
		D3DXMatrixIdentity(&m);
	}

	//! @brief �t�s��ɂ���
	void CreateInverse()
	{
//		CreateInverse(this, this);
		YsToXNAMatrix(this)->Invert(*YsToXNAMatrix(this));
	}

	//! @brief ���̍s����t�s��ɂ������̂��AmOut�ɓ����
	//! @param[out]	mOut	�c �t�s��ɂ������̂��i�[����s��
	void CreateInverse(YsMatrix &mOut) const
	{
		CreateInverse(mOut, *this);
	}

	//! @brief �w��s����t�s��ɂ������̂��AmOut�ɓ����
	//! @param[out]	mOut			�c �t�s��ɂ������̂��i�[����ϐ�
	//! @param[out]	pDeterminant	�c �s�񎮂������Ƃ�ϐ�
	//! @param[in]	mIn				�c �t�s��ɂ���s��
	static void CreateInverse(YsMatrix &mOut, const YsMatrix &mIn){
//		D3DXMatrixInverse(mOut,pDeterminant,mIn);
		YsToXNAMatrix(&mIn)->Invert(*YsToXNAMatrix(&mOut));
	}

	//! @brief �ړ��s����쐬
	//! @param[in]	x	�c x���W
	//! @param[in]	y	�c y���W
	//! @param[in]	z	�c z���W
	void CreateMove(float x, float y, float z)
	{
		D3DXMatrixTranslation(this,x,y,z);
	}

	//! @brief �ړ��s����쐬
	//! @param[in]	v	�c ���W(x,y,z)
	void CreateMove(const YsVec3 &v)
	{
		D3DXMatrixTranslation(this,v.x, v.y, v.z);
	}

	//! @brief �ړ��s����쐬��mOut�ɓ����
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	x		�c x���W
	//! @param[in]	y		�c y���W
	//! @param[in]	z		�c z���W
	static void CreateMove(YsMatrix &mOut, float x, float y, float z)
	{
		D3DXMatrixTranslation(&mOut, x, y, z);
	}

	//! @brief �ړ��s����쐬��mOut�ɓ����
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	v		�c ���W(x,y,z)
	static void CreateMove(YsMatrix &mOut, const YsVec3& v)
	{
		D3DXMatrixTranslation(&mOut,v.x, v.y, v.z);
	}

	//! @brief X��]�s����쐬
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void CreateRotateX(float Ang)
	{
		D3DXMatrixRotationX(this,YsToRadian(Ang));
	}

	//! @brief X��]�s����쐬���쐬���AmOut�ɓ����
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	static void CreateRotateX(YsMatrix &mOut, float Ang)
	{
		D3DXMatrixRotationX(&mOut,YsToRadian(Ang));
	}

	//! @brief Y��]�s����쐬
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void CreateRotateY(float Ang)
	{
		D3DXMatrixRotationY(this,YsToRadian(Ang));
	}

	//! @brief Y��]�s����쐬���쐬���AmOut�ɓ����
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	static void CreateRotateY(YsMatrix &mOut, float Ang)
	{
		D3DXMatrixRotationY(&mOut,YsToRadian(Ang));
	}

	//! @brief Z��]�s����쐬
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void CreateRotateZ(float Ang)
	{
		D3DXMatrixRotationZ(this,YsToRadian(Ang));
	}
	//! @brief Z��]�s����쐬���쐬���AmOut�ɓ����
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	static void CreateRotateZ(YsMatrix &mOut, float Ang)
	{
		D3DXMatrixRotationZ(&mOut,YsToRadian(Ang));
	}

	//! @brief �C�ӎ���]�s����쐬
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	void CreateRotateAxis(const YsVec3 &vAxis, float Ang)
	{
		D3DXMatrixRotationAxis(this,&vAxis,YsToRadian(Ang));
	}
	//! @brief �C�ӎ���]�s����쐬���AmOut�ɓ����
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x(�x)
	static void CreateRotateAxis(YsMatrix *mOut, const YsVec3 &vAxis, float Ang)
	{
		D3DXMatrixRotationAxis(mOut,&vAxis,YsToRadian(Ang));
	}

	//! @brief �X�P�[�����O�s����쐬
	//! @param[in]	x	�c x�g�嗦
	//! @param[in]	y	�c y�g�嗦
	//! @param[in]	z	�c z�g�嗦
	void CreateScale(float x, float y, float z)
	{
		D3DXMatrixScaling(this,x,y,z);
	}

	//! @brief �X�P�[�����O�s����쐬
	//! @param[in]	v	�c �g�嗦(x,y,z)
	void CreateScale(const YsVec3 &v)
	{
		D3DXMatrixScaling(this,v.x, v.y, v.z);
	}

	//! @brief �X�P�[�����O�s����쐬
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	x		�c x�g�嗦
	//! @param[in]	y		�c y�g�嗦
	//! @param[in]	z		�c z�g�嗦
	static void CreateScale(YsMatrix &mOut, float x, float y, float z)
	{
		D3DXMatrixScaling(&mOut,x,y,z);
	}

	//! @brief �X�P�[�����O�s����쐬
	//! @param[out]	mOut	�c �󂯎��s��
	//! @param[in]	v		�c �g�嗦(x,y,z)
	static void CreateScale(YsMatrix &mOut, const YsVec3 &v)
	{
		D3DXMatrixScaling(&mOut, v.x, v.y, v.z);
	}

	//! @brief �w�蕽�ʂŃ~���[���]�����邽�߂̍s����쐬
	//! @param[in] planePos		�c ���ʏ�̍��W
	//! @param[in] planeDir		�c ���ʂ̕���
	void CreateRefrect(const YsVec3& planePos, const YsVec3& planeDir){
		D3DXPLANE plane;
		D3DXPlaneFromPointNormal(&plane, &planePos, &planeDir);	// �ʃf�[�^�쐬
		YsMatrix mMirror;
		D3DXMatrixReflect(this, &plane);	// �w��ʂŃ~���[���]�����邽�߂̍s����쐬
	}

	//! @brief �g�听���𐳋K��
	void NormalizeScale() {
		GetXAxis().Normalize();
		GetYAxis().Normalize();
		GetZAxis().Normalize();
	}

	//! @brief �w�������Z�����������s����쐬�B���W�͌��݂̂��̂��g����
	//! @param[in]	LookDir	�c ������������
	//! @param[in]	Up		�c ������ƂȂ�x�N�g��
	void SetLookAt(const YsVec3 &LookDir, const YsVec3 &Up);

	//! @brief �w��ʒu��Z�����������s����쐬�B���W���w�肷��B
	//! @param[in]	MyPos	�c ���W
	//! @param[in]	LookDir	�c ������������
	//! @param[in]	Up		�c ������ƂȂ�x�N�g��
	void SetLookAt(const YsVec3 &MyPos, const YsVec3 &LookDir, const YsVec3 &Up){
		SetPos(MyPos);
		SetLookAt(LookDir, Up);
	}

	//! @brief ���݂̈ʒu����w��ʒu��Z�����������s����쐬�B���W�͕ω������A���݂̂��̂��g����
	//! @param[in]	TargetPos	�c ���������ʒu
	//! @param[in]	Up			�c ������ƂȂ�x�N�g��
	void SetLookAtPos(const YsVec3 &TargetPos, const YsVec3 &Up);

	//! @brief ���݂̈ʒu����w��ʒu��Z�����������s����쐬�B���W�͕ω������A���݂̂��̂��g����
	//! @param[in]	MyPos		�c ���W
	//! @param[in]	TargetPos	�c ���������ʒu
	//! @param[in]	Up			�c ������ƂȂ�x�N�g��
	void SetLookAtPos(const YsVec3 &MyPos, const YsVec3 &TargetPos, const YsVec3 &Up){
		SetPos(MyPos);
		SetLookAtPos(TargetPos, Up);
	}

	//! @brief �J�����̌����Ɠ��������ɂ���B���W�͂��̂܂܁B�g����ێ�
	//! @param[in]	lpCamMat	�c �J�����s��(�r���[�s��ł͂Ȃ�)
	void SetBillBoard(const YsMatrix &lpCamMat)
	{
		float x = GetXScale();
		float y = GetXScale();
		float z = GetXScale();
		
		// ��]�����������R�s�[(�g��͈ێ�)
		_11 = lpCamMat._11*x;
		_12 = lpCamMat._12*x;
		_13 = lpCamMat._13*x;
		_21 = lpCamMat._21*y;
		_22 = lpCamMat._22*y;
		_23 = lpCamMat._23*y;
		_31 = lpCamMat._31*z;
		_32 = lpCamMat._32*z;
		_33 = lpCamMat._33*z;
	}

	//! @brief Y���͎w�肵�AZ�����J�����̕����ɋɗ͌�����悤�ȍs��ɂ���B���[�U�[�̂悤�ȃr���{�[�h���Ɏg���B
	//! @param[in]	vCamPos	�c �J�������W
	//! @param[in]	vY		�c Y���ƂȂ�x�N�g��
	void SetBillBoardAxisY(const YsVec3 &vCamPos, const YsVec3 &vY);

	//! @brief ���`��� m1�`m2�̒��Ԉʒuf�̍s������߂�Bf��0�`1�B
	//! @param[out]	mOut	�c ��Ԍ��ʂ�����s��
	//! @param[in]	m1		�c �J�n�_�̍s��
	//! @param[in]	m2		�c �I���_�̍s��
	//! @param[in]	f		�c ��Ԉʒu(0�`1)
	static void Lerp(YsMatrix& mOut, const YsMatrix& m1, const YsMatrix& m2, float f);

	//! @brief �X�v���C����� m1�`m2�̒��Ԉʒuf�̍s������߂�Bf��0�`1�B
	//!			mPrev��m1�̂P�O�̍s��BmNext��m2�̎��̍s����w�肷��B
	//! @param[out]	mOut	�c ��Ԍ��ʂ�����s��
	//! @param[in]	mPrev	�c �J�n�_���1�O�̍s��
	//! @param[in]	m1		�c �J�n�_�̍s��
	//! @param[in]	m2		�c �I���_�̍s��
	//! @param[in]	mNext	�c �I���_���P��̍s��
	//! @param[in]	f		�c ��Ԉʒu(0�`1)
	static void SplineLerp(YsMatrix& mOut, const YsMatrix& mPrev, const YsMatrix& m1, const YsMatrix& m2, YsMatrix& mNext, float f);

	//! @brief ���W�݂̂��Z�b�g����
	//! @param[in]	fx	�c x���W
	//! @param[in]	fy	�c y���W
	//! @param[in]	fz	�c z���W
	void SetPos(float fx, float fy, float fz){
		_41 = fx;
		_42 = fy;
		_43 = fz;
	}

	//! @brief ���W�݂̂��Z�b�g����
	//! @param[in]	v	�c ���W(x,y,z)
	void SetPos(const YsVec3 &v){
		_41 = v.x;
		_42 = v.y;
		_43 = v.z;
	}

	//! @brief ���W�݂̂��Z�b�g����
	//! @param[in]	lpSrcMat	�c �s��(���W�̂ݎg�p�����)
	void SetPos(const YsMatrix &lpSrcMat){
		_41 = lpSrcMat._41;
		_42 = lpSrcMat._42;
		_43 = lpSrcMat._43;
	}

	//! @brief X���̂݃Z�b�g����
	//! @param[in]	fx	�c �x�N�g����x����
	//! @param[in]	fy	�c �x�N�g����y����
	//! @param[in]	fz	�c �x�N�g����z����
	void SetXAxis(float fx, float fy, float fz){
		_11 = fx;
		_12 = fy;
		_13 = fz;
	}
	//! @brief X���̂݃Z�b�g����
	//! @param[in]	v	�c X���ƂȂ�x�N�g��
	void SetXAxis(const YsVec3 &v){
		_11 = v.x;
		_12 = v.y;
		_13 = v.z;
	}

	//! @brief Y���̂݃Z�b�g����
	//! @param[in]	fx	�c �x�N�g����x����
	//! @param[in]	fy	�c �x�N�g����y����
	//! @param[in]	fz	�c �x�N�g����z����
	void SetYAxis(float fx, float fy, float fz){
		_21 = fx;
		_22 = fy;
		_23 = fz;
	}
	//! @brief Y���̂݃Z�b�g����
	//! @param[in]	v	�c Y���ƂȂ�x�N�g��
	void SetYAxis(const YsVec3 &v){
		_21 = v.x;
		_22 = v.y;
		_23 = v.z;
	}

	//! @brief Z���̂݃Z�b�g����
	//! @param[in]	fx	�c �x�N�g����x����
	//! @param[in]	fy	�c �x�N�g����y����
	//! @param[in]	fz	�c �x�N�g����z����
	void SetZAxis(float fx, float fy, float fz){
		_31 = fx;
		_32 = fy;
		_33 = fz;
	}
	//! @brief Z���̂݃Z�b�g����
	//! @param[in]	v	�c Z���ƂȂ�x�N�g��
	void SetZAxis(const YsVec3 &v){
		_21 = v.x;
		_22 = v.y;
		_23 = v.z;
	}

	//! @brief ��]���������A�w��s�񂩂�R�s�[����
	//! @param[in]	lpSrcMat	�c �R�s�[���̍s��
	void SetRotation(const YsMatrix &lpSrcMat){
		_11 = lpSrcMat._11;
		_12 = lpSrcMat._12;
		_13 = lpSrcMat._13;
		_21 = lpSrcMat._21;
		_22 = lpSrcMat._22;
		_23 = lpSrcMat._23;
		_31 = lpSrcMat._31;
		_32 = lpSrcMat._32;
		_33 = lpSrcMat._33;
	}

	//! @brief �g�嗦�𒼐ڑ��삷��
	//! @param[in]	x	�c x�g�嗦
	//! @param[in]	y	�c y�g�嗦
	//! @param[in]	z	�c z�g�嗦
	void SetScale(float x, float y, float z){
		NormalizeScale();	// ��x�A�g�听�����K��

		_11 *= x;
		_12 *= x;
		_13 *= x;
		_21 *= y;
		_22 *= y;
		_23 *= y;
		_31 *= z;
		_32 *= z;
		_33 *= z;
	}

	// �萔�s��
	static const YsMatrix Identity;	//!< �P�ʍs��

	//! @brief �g�嗦�𒼐ڑ��삷��
	//! @param[in]	size	�c �g�嗦
	void SetScale(const YsVec3 &pSize){
		SetScale(pSize.x, pSize.y, pSize.z);
	}

	//! @brief �������e�s��(�ˉe�s��)�Ƃ��č쐬
	void CreatePerspectiveFovLH(float angle, float aspect, float fNear, float fFar) {
		// �ˉe�s��
		D3DXMatrixPerspectiveFovLH(this,
			YsToRadian(angle),		// ����p
			aspect,					// ��ʂ̃A�X�y�N�g��
			fNear,					// �ŋߐڋ���
			fFar);					// �ŉ�������
	}

	//! @brief ���ˉe�s��(�ˉe�s��)�Ƃ��č쐬
	void CreateOrthoLH(float w, float h, float fNear, float fFar) {
		D3DXMatrixOrthoLH(this, w, h, fNear, fFar);
	}

	//! @brief �r���[�s��Ƃ��č쐬�@�����_���w�肷��Ver
	//! @param[in] pos			�c �J�������W
	//! @param[in] ratgetPos	�c �����_�ƂȂ���W
	//! @param[in] up			�c ��(Y)�ƂȂ����
	void CreateViewTransformPos(const YsVec3 &pos, const YsVec3 &targetPos, const YsVec3 &up = YsVec3(0,1,0)) {
		D3DXMatrixLookAtLH(this, &pos, &targetPos, &up);
	}

	//! @brief �r���[�s��Ƃ��č쐬�@�J�����������w�肷��Ver
	//! @param[in] pos	�c �J�������W
	//! @param[in] dir	�c �J��������
	//! @param[in] up	�c ��(Y)�ƂȂ����
	void CreateViewTransform(const YsVec3 &pos, const YsVec3 &dir, const YsVec3 &up = YsVec3(0, 1, 0)) {
		D3DXMatrixLookAtLH(this, &pos, &(pos + dir), &up);
	}

	//========================================================================
	// �ϊ�
	//========================================================================
	//! @brief �����̍s��̉�]�������N�H�[�^�j�I���֕ϊ�
	//! @param[out]	out	�c �ϊ����ꂽ�N�H�[�^�ɃI�����󂯎��ϐ�
	void ToQuaternion(D3DXQUATERNION &out) const{
		D3DXQuaternionRotationMatrix(&out, this);
	}
	//! @brief �w��s�񂩂��]�������N�H�[�^�j�I���֕ϊ�
	//! @param[out]	out	�c �ϊ����ꂽ�N�H�[�^�ɃI�����󂯎��ϐ�
	//! @param[in]	mIn	�c ���ƂȂ�s��
	static void ToQuaternion(D3DXQUATERNION &out, const YsMatrix &mIn){
		D3DXQuaternionRotationMatrix(&out, &mIn);
	}

	//============================================================================
	// ���擾
	//============================================================================
	//! @brief ���W���x�N�g���^�Ŏ擾
	YsVec3& GetPos(){	return (YsVec3&)_41; }
	const YsVec3& GetPos() const { return (YsVec3&)_41; }

	//! @brief X�����x�N�g���^�Ŏ擾
	YsVec3& GetXAxis() { return (YsVec3&)_11; }
	const YsVec3& GetXAxis() const { return (YsVec3&)_11; }

	//! @brief Y�����x�N�g���^�Ŏ擾
	YsVec3& GetYAxis() { return (YsVec3&)_21; }
	const YsVec3& GetYAxis() const { return (YsVec3&)_21; }

	//! @brief Z�����x�N�g���^�Ŏ擾
	YsVec3& GetZAxis() { return (YsVec3&)_31; }
	const YsVec3& GetZAxis() const { return (YsVec3&)_31; }

	//! @brief X�g�嗦���擾
	float GetXScale() const {
		return D3DXVec3Length(&(YsVec3&)_11);
	}
	//! @brief Y�g�嗦���擾
	float GetYScale() const {
		return D3DXVec3Length(&(YsVec3&)_21);
	}
	//! @brief Z�g�嗦���擾
	float GetZScale() const {
		return D3DXVec3Length(&(YsVec3&)_31);
	}
	//! @brief �g�嗦(x,y,z)���x�N�g���Ŏ擾
	YsVec3 GetScale() const{
		return YsVec3(GetXScale(), GetYScale(), GetZScale());
	}

	float X() const { return _41; }								//!< X���W�擾
	float Y() const { return _42; }								//!< Y���W�擾
	float Z() const { return _43; }								//!< Z���W�擾

	//! @brief �ˉe�s�񂩂�A����p�E�ŋߐڋ����E�ŉ������������߂�
	//! @param[out]	outAng		�c ����p�������Ă���
	//! @param[out]	outNerar	�c �ŋߐڋ����������Ă���
	//! @param[out]	outFar		�c �ŉ��������������Ă���
	void ComputePerspectiveInfo(float& outAng, float& outNear, float& outFar) const;
};

}

#endif
