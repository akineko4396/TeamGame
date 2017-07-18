//===============================================================
//! @file YsVec3.h
//! @brief 3�����x�N�g���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsVec3_h__
#define __YsVec3_h__

namespace YskLib
{

//! @brief D3DX�̃x�N�g������ADirectXTK�̃x�N�g���փ|�C���^�^�ϊ�
#define YsToXNAVec3(v) ((DirectX::SimpleMath::Vector3*)v)

//============================================================================
//! @brief �R�����x�N�g���N���X
//!
//! D3DXVECTOR3���p�����āA�֗��Ȋ֐���t������������
//! �ꕔ�ADirectXTK�̕��̊֐����g�p���Ă��܂�
//!
//! @ingroup Math
//============================================================================
class YsVec3 : public D3DXVECTOR3{
public:

	YsVec3(){
		x = 0;
		y = 0;
		z = 0;
	}
	YsVec3(float fx,float fy,float fz){
		x = fx;
		y = fy;
		z = fz;
	}
	YsVec3(const D3DXVECTOR3& In){
		x = In.x;
		y = In.y;
		z = In.z;
	}

	void Set(float fx,float fy,float fz){
		x = fx;
		y = fy;
		z = fz;
	}
	void Set(const D3DXVECTOR3& v){
		x = v.x;
		y = v.y;
		z = v.z;
	}

	YsVec3& operator=(const D3DXVECTOR3& v){
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	// �e���Z�q
	YsVec3 operator + () const{ return D3DXVECTOR3::operator+(); }
	YsVec3 operator - () const{ return D3DXVECTOR3::operator-(); }

	YsVec3 operator + ( CONST D3DXVECTOR3& v) const{ return D3DXVECTOR3::operator+(v); }
	YsVec3 operator - ( CONST D3DXVECTOR3& v) const{ return D3DXVECTOR3::operator-(v); }
	YsVec3 operator * ( FLOAT f) const{ return D3DXVECTOR3::operator*(f); }
	YsVec3 operator / ( FLOAT f) const{ return D3DXVECTOR3::operator/(f); }


	// �e�v�f����Z����
	YsVec3& operator *= (const D3DXVECTOR3& v){
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	YsVec3 operator * (const D3DXVECTOR3& v) const{
		D3DXVECTOR3 v3;
		v3.x = x * v.x;
		v3.y = y * v.y;
		v3.z = z * v.z;
		return v3;
	}
	YsVec3& operator *= (float f){ return (YsVec3&)D3DXVECTOR3::operator*=(f); }
	YsVec3& operator /= (float f){ return (YsVec3&)D3DXVECTOR3::operator/=(f); }

	// �萔�x�N�g��
	static const YsVec3 Zero;	//!< �S�v�f0�x�N�g��
	static const YsVec3 One;	//!< �S�v�f1�x�N�g��
	static const YsVec3 Up;		//!< ��x�N�g��
	static const YsVec3 Down;	//!< ���x�N�g��
	static const YsVec3 Left;	//!< ���x�N�g��
	static const YsVec3 Right;	//!< �E�x�N�g��
	static const YsVec3 Front;	//!< �O���x�N�g��
	static const YsVec3 Back;	//!< ����x�N�g��

	//! @brief �x�N�g�����Z�@vOut = v1 + v2
	//! @param[out]	vOut	�c �v�Z���ʂ�����ϐ�
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	static void Add(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2){	// �ÓI�֐�
		D3DXVec3Add(&vOut, &v1, &v2);
	}
	//! @brief �x�N�g�����Z�@���� = ���� + (fx,fy,fz)
	//! @param[in]	fx		�c �x�N�g����x����
	//! @param[in]	fy		�c �x�N�g����y����
	//! @param[in]	fz		�c �x�N�g����z����
	void Add(float fx, float fy, float fz){
		Add(*this, *this, YsVec3(fx,fy,fz));
	}
	//! @brief �x�N�g�����Z�@���� = ���� + p
	//! @param[in]	p		�c �x�N�g��
	void Add(const YsVec3 &p){
		Add(*this, *this, p);
	}
	
	//! @brief �x�N�g�����Z�@vOut = v1-v2
	//! @param[out]	vOut	�c �v�Z���ʂ�����ϐ�
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	static void Sub(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2){	// �ÓI�֐�
		D3DXVec3Subtract(&vOut, &v1, &v2);
	}
	//! @brief �x�N�g�����Z�@���� = ���� - (fx,fy,fz)
	//! @param[in]	fx		�c �x�N�g����x����
	//! @param[in]	fy		�c �x�N�g����y����
	//! @param[in]	fz		�c �x�N�g����z����
	void Sub(float fx, float fy, float fz){
		Sub(*this, *this, YsVec3(fx,fy,fz));
	}
	//! @brief �x�N�g�����Z�@���� = ���� - p
	//! @param[in]	p		�c �x�N�g��
	void Sub(const YsVec3 &p){
		Sub(*this, *this, p);
	}
	
	//! @brief ����
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	//! @return ���ϒl
	static float Dot(const YsVec3 &v1, const YsVec3 &v2){			// �ÓI�֐�
		return D3DXVec3Dot(&v1, &v2);
	}
	//! @brief ����(���ʂ�-1�`1�͈̔͂ɂ���)
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	//! @return ���ϒl
	static float Dot2(const YsVec3 &v1, const YsVec3 &v2){			// �ÓI�֐� �K�� -1<=dot<=1 �ŕԂ�
		float retdot = D3DXVec3Dot(&v1, &v2);
		if(retdot < -1.0f){
			retdot = -1.0f;
		}
		if(retdot > 1.0f){
			retdot = 1.0f;
		}
		return retdot;
	}
	//! @brief ���� ������v1
	//! @param[in]	v1		�c �x�N�g���P
	//! @return ���ϒl
	float Dot(const YsVec3 &v1){
		return Dot(*this, v1);
	}
	//! @brief ����(���ʂ�-1�`1�͈̔͂ɂ���) ������v1
	//! @param[in]	v1		�c �x�N�g���P
	//! @return ���ϒl
	float Dot2(const YsVec3 &v1){
		return Dot2(*this, v1);
	}

	//! @brief �O��
	//! @param[out]	vOut	�c �v�Z���ʂ̃x�N�g��������
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	static void Cross(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2){// �ÓI�֐�
		D3DXVec3Cross(&vOut, &v1, &v2);
	}
	//! @brief �O�� ���� x vBack
	//! @param[out]	vOut	�c �v�Z���ʂ̃x�N�g��������
	//! @param[in]	vBack	�c �Ώ̂̃x�N�g��
	void CrossBack(YsVec3 &vOut, const YsVec3 &vBack) const{
		Cross(vOut, *this, vBack);
	}
	//! @brief �O�� vFront x ����
	//! @param[out]	vOut	�c �v�Z���ʂ̃x�N�g��������
	//! @param[in]	vFront	�c �Ώ̂̃x�N�g��
	void CrossFront(YsVec3 &vOut, const YsVec3 &vFront) const{
		Cross(vOut,vFront, *this);
	}
	
	//! @brief �X�J���[�{
	//! @param[out]	vOut	�c �v�Z���ʂ̃x�N�g��������
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	s		�c �g�嗦
	static void Scale(YsVec3 &vOut, const YsVec3 &vSrc, float s){		// �ÓI�֐�
		D3DXVec3Scale(&vOut,&vSrc,s);
	}
	//! @brief �X�J���[�{
	//! @param[in]	s		�c �g�嗦
	void Scale(float s){
		Scale(*this,*this,s);
	}

	//! @brief �x�N�g���̒���
	//! @param[in]	vSrc	�c ���������߂�x�N�g��
	static float Length(const YsVec3 &vSrc){						// �ÓI�֐�
		return D3DXVec3Length(&vSrc);
	}
	//! @brief �x�N�g���̒���
	float Length(){
		return Length(*this);
	}

	//! @brief �x�N�g���𐳋K��
	//! @param[out]	vOut	�c ���K�������x�N�g��������
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	static void Normalize(YsVec3 &vOut, const YsVec3 &vSrc){			// �ÓI�֐�
//		D3DXVec3Normalize(vOut,vSrc);
		// D3DXVec3Normalize���ƌ덷���傫�������������̂ŁA���������g�p
		float len = YsVec3::Length(vSrc);
		if (len > 0) {
			vOut.x = vSrc.x / len;
			vOut.y = vSrc.y / len;
			vOut.z = vSrc.z / len;
		}
	}
	//! @brief �x�N�g���𐳋K��
	YsVec3& Normalize(){
		Normalize(*this,*this);
		return *this;
	}
	//! @brief �x�N�g���𐳋K��
	//! @param[out]	vOut	�c ���K�������x�N�g��������
	void Normalize(YsVec3 &vOut) const{
		Normalize(vOut,*this);
	}

	//! @brief ���`���
	//!
	//! ���WpV1�`pV2�̒��Ԉʒuf(0�`1)�̍��W���Z�o
	//!
	//! @param[out]	vOut		�c ��Ԍ��ʂ̍��W������
	//! @param[in]	v1			�c �J�n���W
	//! @param[in]	v2			�c �I�����W
	//! @param[in]	f			�c ��Ԉʒu(0�`1)  �Ⴆ��0�Ȃ�J�n���W(v1)�ƂȂ�A1�Ȃ�I�����W(v2)�ƂȂ�@0.5�Ȃ�v1��v2�̒��ԍ��W�ƂȂ�
	static void Lerp(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2, float f)
	{
		D3DXVec3Lerp(&vOut,&v1,&v2,f);
	}

	//! @brief �G���~�l�[�g�X�v���C�����
	static void Hermite(YsVec3 &vOut, const YsVec3 &pV1, const YsVec3 &pT1, const YsVec3 &pV2, const YsVec3 &pT2, float f)
	{
		D3DXVec3Hermite(&vOut,&pV1,&pT1,&pV2,&pT2,f);
	}

	//! @brief Catmull-Rom�X�v���C�����
	//!
	//! ���WpV1�`pV2�̒��Ԉʒuf(0�`1)�̍��W���X�v���C����ԂŎZ�o(�Ȑ��I�Ȋ����̌��ʂɂȂ�)
	//!
	//! @param[out]	vOut		�c ��Ԍ��ʂ̍��W������
	//! @param[in]	pV0			�c pV1�̂P�O�̍��W
	//! @param[in]	pV1			�c �J�n���W
	//! @param[in]	pV2			�c �I�����W
	//! @param[in]	pV3			�c pV2�̎��̍��W
	//! @param[in]	f			�c ��Ԉʒu(0�`1)  �Ⴆ��0�Ȃ�J�n���W(pV0)�ƂȂ�A1�Ȃ�I�����W(pV1)�ƂȂ�@0.5�Ȃ�pV0��pV1�̒��ԍ��W�ƂȂ�
	static void CatmullRom(YsVec3 &vOut, const YsVec3 &pV0, const YsVec3 &pV1, const YsVec3 &pV2, const YsVec3 &pV3, float f)
	{
		D3DXVec3CatmullRom(&vOut,&pV0,&pV1,&pV2,&pV3,f);
	}

	//! @brief �s��ŕϊ�
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	static void Transform(YsVec3 &vOut, const YsVec3 &vSrc, const D3DXMATRIX &mat){	// �ÓI�֐�
		D3DXVec3TransformCoord(&vOut, &vSrc, &mat);
	}
	//! @brief �s��ŕϊ�
	//! @param[in]	mat		�c �ϊ��s��
	void Transform(const D3DXMATRIX &mat){
		Transform(*this, *this, mat);
	}
	//! @brief �s��ŕϊ�
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	void Transform(YsVec3 &vOut, const D3DXMATRIX &mat) const{
		Transform(vOut, *this, mat);
	}

	//! @brief �s��ŕϊ�(��]�̂�)
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	static void TransformNormal(YsVec3 &vOut, const YsVec3 &vSrc, const D3DXMATRIX &mat){// �ÓI�֐�
		D3DXVec3TransformNormal(&vOut, &vSrc, &mat);
	}
	//! @brief �s��ŕϊ�(��]�̂�)
	//! @param[in]	mat		�c �ϊ��s��
	void TransformNormal(const D3DXMATRIX &mat){
		TransformNormal(*this, *this, mat);
	}
	//! @brief �s��ŕϊ�(��]�̂�)
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	void TransformNormal(YsVec3 &vOut, const D3DXMATRIX &mat) const{
		TransformNormal(vOut, *this, mat);
	}

	//! @brief �w������Ɏw��p�x������]�B(�����΂ł��Ȃ���)
	//! @param[in]	vTargetDir	�c ���������̕���
	//! @param[in]	MaxAng		�c �ő��]�p�x�@���̊p�x�ȏ�͉�]���Ȃ�
	void Homing(const YsVec3& vTargetDir, float MaxAng);

	//! @brief �O�p�`��AABB���߂�
	//! @param[in]	v1		�c ���_�P�̍��W
	//! @param[in]	v2		�c ���_�Q�̍��W
	//! @param[in]	v3		�c ���_�R�̍��W
	//! @param[out]	outMinP	�c AABB�̍ŏ����W
	//! @param[out]	outMaxP	�c AABB�̍ő���W
	static void CreateAABB(const YsVec3 &v1, const YsVec3 &v2, const YsVec3 &v3, YsVec3 &outMinP, YsVec3 &outMaxP){
		outMinP = v1;
		outMaxP = v1;
		// �ŏ��_�Z�o
		if(outMinP.x > v2.x)outMinP.x = v2.x;
		if(outMinP.y > v2.y)outMinP.y = v2.y;
		if(outMinP.z > v2.z)outMinP.z = v2.z;
		if(outMinP.x > v3.x)outMinP.x = v3.x;
		if(outMinP.y > v3.y)outMinP.y = v3.y;
		if(outMinP.z > v3.z)outMinP.z = v3.z;
		// �ő�_�Z�o
		if(outMaxP.x < v2.x)outMaxP.x = v2.x;
		if(outMaxP.y < v2.y)outMaxP.y = v2.y;
		if(outMaxP.z < v2.z)outMaxP.z = v2.z;
		if(outMaxP.x < v3.x)outMaxP.x = v3.x;
		if(outMaxP.y < v3.y)outMaxP.y = v3.y;
		if(outMaxP.z < v3.z)outMaxP.z = v3.z;
	}
	//! @brief ������AABB���Z�o
	//! @param[in]	v1		�c ���W�P
	//! @param[in]	v2		�c ���W�Q
	//! @param[out]	outMinP	�c AABB�̍ŏ����W
	//! @param[out]	outMaxP	�c AABB�̍ő���W
	static void CreateAABBFromLineSegment(const YsVec3 &v1, const YsVec3 &v2, YsVec3 &outMinP, YsVec3 &outMaxP){

		// �����̃o�E���f�B���O�{�b�N�X�Z�o
		if(v1.x < v2.x){
			outMinP.x = v1.x;
			outMaxP.x = v2.x;
		}
		else{
			outMinP.x = v2.x;
			outMaxP.x = v1.x;
		}

		if(v1.y < v2.y){
			outMinP.y = v1.y;
			outMaxP.y = v2.y;
		}
		else{
			outMinP.y = v2.y;
			outMaxP.y = v1.y;
		}

		if(v1.z < v2.z){
			outMinP.z = v1.z;
			outMaxP.z = v2.z;
		}
		else{
			outMinP.z = v2.z;
			outMaxP.z = v1.z;
		}
	}

	//! @brief YsVec2�Ƃ��ĕԂ�
	YsVec2& GetVec2(){
		return *(YsVec2*)this;
	}

};

}

#endif
