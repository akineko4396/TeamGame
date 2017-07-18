//===============================================================
//! @file YsQuat.h
//! @brief �N�H�[�^�j�I���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsQuat_h__
#define __YsQuat_h__

namespace YskLib{

//============================================================================
//! @brief �N�H�[�^�j�I���N���X
//!
//! @ingroup Math
//============================================================================
class YsQuat : public D3DXQUATERNION{
public:

	YsQuat() : D3DXQUATERNION(0, 0, 0, 1){}

	YsQuat(const D3DXQUATERNION &q){
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}

	YsQuat( const float *f){
		x = f[0];
		y = f[1];
		z = f[2];
		z = f[3];
	}

	YsQuat( float X, float Y, float Z, float W ){
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	//! @brief �����Ɠ���
	//! @param[in]	q	�c �Ώ̂̃N�H�[�^�ɃI��
	//! @return ���ϒl
	float Dot(const YsQuat &q){
		return D3DXQuaternionDot(this, &q);
	}
	//! @brief ����
	//! @param[in]	q1	�c �N�H�[�^�ɃI���P
	//! @param[in]	q2	�c �N�H�[�^�ɃI���Q
	//! @return ���ϒl
	static float Dot(const YsQuat &q1, const YsQuat &q2){
		return D3DXQuaternionDot(&q1, &q2);
	}


	//! @brief �w���֐����v�Z����
	//! @param[in]	q	�c �����̊�ɂȂ�N�H�[�^�j�I��
	// ���̃��\�b�h�́A�����N�H�[�^�j�I����P�ʃN�H�[�^�j�I���ɕϊ����܂��B
	// �܂�D3DXQuaternionExp �́A�����N�H�[�^�j�I�����n�������̂Ɖ��肵�Aw �𖳎����Čv�Z���s���܂� (w == 0)�B
	void Exp(const YsQuat &q){
		D3DXQuaternionExp(this, &q);
	}
	//! @brief �w���֐����v�Z����
	//! @param[out]	qOut	�c ���ʂ�����ϐ�
	//! @param[in]	q		�c �����̊�ɂȂ�N�H�[�^�j�I��
	static void Exp(YsQuat &qOut, const YsQuat &q){
		D3DXQuaternionExp(&qOut, &q);
	}

	//! @brief �P�ʃN�H�[�^�j�I���ɂ���
	void Identity(){
		D3DXQuaternionIdentity(this);
	}
	//! @brief �P�ʃN�H�[�^�j�I���ɂ���
	//! @param[out]	qOut	�c ���ʂ�����ϐ�
	static void Identity(YsQuat &qOut){
		D3DXQuaternionIdentity(&qOut);
	}

	//! @brief �N�H�[�^�j�I�����������āA�Đ��K�����܂��B
	void Inverse(){
		D3DXQuaternionInverse(this,this);
	}
	//! @brief �N�H�[�^�j�I�����������āA�Đ��K�����܂��B
	//! @param[out]	qOut	�c ���ʂ�����ϐ�
	//! @param[in]	qIn		�c �����̊�ɂȂ�N�H�[�^�j�I��
	static void Inverse(YsQuat &qOut, const YsQuat &qIn){
		D3DXQuaternionInverse(&qOut, &qIn);
	}

	//! @brief ���K��
	void Normalize(){
		D3DXQuaternionNormalize(this,this);
	}
	//! @brief ���K��
	//! @param[out]	qOut	�c ���ʂ�����ϐ�
	//! @param[in]	qIn		�c �����̊�ɂȂ�N�H�[�^�j�I��
	static void Normalize(YsQuat &out, const YsQuat &qIn){
		D3DXQuaternionNormalize(&out, &qIn);
	}

	//! @brief �N�H�[�^�j�I���̒���
	float Length(){
		return D3DXQuaternionLength(this);
	}
	//! @brief �N�H�[�^�j�I���̒���
	//! @param[in]	qIn		�c �����̊�ɂȂ�N�H�[�^�j�I��
	static float Length(const YsQuat &qIn){
		return D3DXQuaternionLength(&qIn);
	}

	//! @brief ���R�ΐ����v�Z
	void Ln(){
		D3DXQuaternionLn(this,this);
	}
	//! @brief ���R�ΐ����v�Z
	//! @param[out]	qOut	�c ���ʂ�����ϐ�
	//! @param[in]	qIn		�c �����̊�ɂȂ�N�H�[�^�j�I��
	static void Ln(YsQuat &qOut, const YsQuat &qIn){
		D3DXQuaternionLn(&qOut, &qIn);
	}

	//! @brief ��(��납��)
	//! @param[in]	qIn		�c �����̊�ɂȂ�N�H�[�^�j�I��
	void Multiply(const YsQuat &qIn){
		D3DXQuaternionMultiply(this,this, &qIn);
	}
	//! @brief ��(��O����)
	//! @param[in]	qIn		�c �����̊�ɂȂ�N�H�[�^�j�I��
	void Multiply_Local(const YsQuat &qIn){
		D3DXQuaternionMultiply(this, &qIn,this);
	}
	//! @brief ��(��O����)
	//! @param[out]	qOut	�c ���ʂ�����ϐ�
	//! @param[in]	qIn1	�c �N�H�[�^�j�I���P
	//! @param[in]	qIn2	�c �N�H�[�^�j�I���Q
	static void Multiply(YsQuat &qOut, const YsQuat &qIn1, const YsQuat &qIn2){
		D3DXQuaternionMultiply(&qOut, &qIn1, &qIn2);
	}

	//! brief �N�H�[�^�j�I����C�ӎ����[���h��]
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x
	void RotateAxis(const YsVec3 &vAxis, float Ang){
		YsQuat q;
		q.CreateRotateAxis(vAxis, Ang);
		Multiply( q );
	}
	//! brief �N�H�[�^�j�I����C�ӎ����[�J����]
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x
	void RotateAxis_Local(const YsVec3 &vAxis, float Ang){
		YsQuat q;
		q.CreateRotateAxis(vAxis, Ang);
		Multiply_Local( q );
	}

	//! brief �C�ӎ��N�H�[�^�j�I�����쐬����
	//! @param[in]	vAxis	�c ��]��
	//! @param[in]	Ang		�c ��]�p�x
	void CreateRotateAxis(const YsVec3 &vAxis, float Ang){
		D3DXQuaternionRotationAxis(this, &vAxis,YsToRadian(Ang));
	}

	//! brief ��]�s��ɕϊ�
	//! @param[out]	out	�c �ϊ����ꂽ�s�������ϐ�
	void ToMatrix(D3DXMATRIX &out) const
	{
		D3DXMatrixRotationQuaternion(&out,this);
	}
	//! brief �w��N�H�[�^�j�I������]�s��ɕϊ�
	//! @param[out]	out		�c �ϊ����ꂽ�s�������ϐ�
	//! @param[in]	pIn		�c �����̊�ɂȂ�N�H�[�^�j�I��
	static void ToMatrix(D3DXMATRIX &out, YsQuat &qIn)
	{
		D3DXMatrixRotationQuaternion(&out, &qIn);
	}

	//! brief ���ʐ��`��� q1�`q2�̒��Ԉʒu�̃N�H�[�^�j�I�������߂�
	//! @param[out]	out		�c ��Ԍ��ʂ�����ϐ�
	//! @param[in]	q1		�c �N�H�[�^�j�I���P
	//! @param[in]	q2		�c �N�H�[�^�j�I���Q
	//! @param[in]	f		�c ��Ԉʒu(0�`1)
	static void Slerp(YsQuat &out, const YsQuat &q1, const YsQuat &q2, float f)
	{
		D3DXQuaternionSlerp(&out, &q1, &q2, f);
	}
	//! brief  ���ʓ񎟕�� q1�`q2�̒��Ԉʒu�̃N�H�[�^�j�I�������߂�
	//! D3DXQuaternionSquad�֐��Q��
	static void Squad(YsQuat &out, const YsQuat &qQ1, const YsQuat &q1, const YsQuat &q2, const YsQuat &q3, float f)
	{
		D3DXQuaternionSquad(&out, &qQ1, &q1, &q2, &q3, f);
	}
	// ���󕽕���Ԃ̐���_��ݒ�
	//��)
	// z ���𒆐S�Ƃ�����]
	//	YsQuat Q0 = YsQuat(0,  0, 0.707f, -.707f);
	//	YsQuat Q1 = YsQuat(0,  0, 0.000f, 1.000f);
	//	YsQuat Q2 = YsQuat(0,  0, 0.707f, 0.707f);
	//	YsQuat Q3 = YsQuat(0,  0, 1.000f, 0.000f);
	//	YsQuat A, B, C, Qt;
	//	FLOAT time = 0.5f;
	//
	//	D3DXQuaternionSquadSetup(&A, &B, &C, &Q0, &Q1, &Q2, &Q3);
	//	D3DXQuaternionSquad(&Qt, &Q1, &A, &B, &C, time);

	//! brief  ���ʓ񎟕�Ԃ̐���|�C���g�Z�o
	//! D3DXQuaternionSquadSetup�֐��Q��
	static void SquadSetup(YsQuat &Aout, YsQuat &Bout, YsQuat &Cout, const YsQuat &qQ0, const YsQuat &qQ1, const YsQuat &qQ2, const YsQuat &qQ3){
		D3DXQuaternionSquadSetup(&Aout, &Bout, &Cout, &qQ0, &qQ1, &qQ2, &qQ3);
	}

};

}
#endif
