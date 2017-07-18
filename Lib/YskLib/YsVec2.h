//===============================================================
//! @file YsVec2.h
//! @brief 2�����x�N�g���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsVec2_h__
#define __YsVec2_h__

namespace YskLib
{

//! @brief D3DX�̃x�N�g������ADirectXTK�̃x�N�g���փ|�C���^�^�ϊ�
#define YsToXNAVec2(v) ((DirectX::SimpleMath::Vector2*)v)

//============================================================================
//! @brief �Q�����x�N�g���N���X
//!
//! @ingroup Math
//============================================================================
class YsVec2 : public D3DXVECTOR2{
public:

	YsVec2(){
		x = 0;
		y = 0;
	}
	YsVec2(float fx, float fy){
		x = fx;
		y = fy;
	}
	YsVec2(const D3DXVECTOR2 &In){
		x = In.x;
		y = In.y;
	}

	void Set(float fx,float fy){
		x = fx;
		y = fy;
	}
	void Set(const D3DXVECTOR2& v){
		x = v.x;
		y = v.y;
	}

	YsVec2& operator=(const D3DXVECTOR2& v){
		x = v.x;
		y = v.y;
		return *this;
	}

	// �e���Z�q
	YsVec2 operator + () const{ return D3DXVECTOR2::operator+(); }
	YsVec2 operator - () const{ return D3DXVECTOR2::operator-(); }

	YsVec2 operator + ( CONST D3DXVECTOR2& v) const{ return D3DXVECTOR2::operator+(v); }
	YsVec2 operator - ( CONST D3DXVECTOR2& v) const{ return D3DXVECTOR2::operator-(v); }
	YsVec2 operator * ( FLOAT f) const{ return D3DXVECTOR2::operator*(f); }
	YsVec2 operator / ( FLOAT f) const{ return D3DXVECTOR2::operator/(f); }

	// �e�v�f����Z����
	YsVec2& operator *= (const D3DXVECTOR2& v){
		x *= v.x;
		y *= v.y;
		return *this;
	}
	YsVec2 operator * (const D3DXVECTOR2& v) const{
		D3DXVECTOR2 v2;
		v2.x = x * v.x;
		v2.y = y * v.y;
		return v2;
	}
	YsVec2& operator *= (float f){ return (YsVec2&)D3DXVECTOR2::operator*=(f); }
	YsVec2& operator /= (float f){ return (YsVec2&)D3DXVECTOR2::operator/=(f); }

	// �萔�x�N�g��
	static const YsVec2 Zero;	//!< �S�v�f0�x�N�g��
	static const YsVec2 One;	//!< �S�v�f1�x�N�g��

	//! @brief �x�N�g�����Z�@vOut = v1 + v2
	//! @param[out]	vOut	�c �v�Z���ʂ�����ϐ�
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	static void Add(YsVec2 &vOut, const YsVec2 &v1, const YsVec2 &v2){	// �ÓI�֐�
		D3DXVec2Add(&vOut, &v1, &v2);
	}
	//! @brief �x�N�g�����Z�@���� = ���� + (fx,fy)
	//! @param[in]	fx		�c �x�N�g����x����
	//! @param[in]	fy		�c �x�N�g����y����
	void Add(float fx, float fy){
		Add(*this, *this, YsVec2(fx,fy));
	}
	//! @brief �x�N�g�����Z�@���� = ���� + p
	//! @param[in]	p		�c �x�N�g��
	void Add(const YsVec2 &p){
		Add(*this, *this, p);
	}
	
	//! @brief �x�N�g�����Z�@vOut = v1 - v2
	//! @param[out]	vOut	�c �v�Z���ʂ�����ϐ�
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	static void Sub(YsVec2 &vOut, const YsVec2 &v1, const YsVec2 &v2){	// �ÓI�֐�
		D3DXVec2Subtract(&vOut, &v1, &v2);
	}
	//! @brief �x�N�g�����Z�@���� = ���� - (fx,fy,fz)
	//! @param[in]	fx		�c �x�N�g����x����
	//! @param[in]	fy		�c �x�N�g����y����
	void Sub(float fx, float fy){
		Sub(*this, *this, YsVec2(fx, fy));
	}
	//! @brief �x�N�g�����Z�@���� = ���� - p
	//! @param[in]	p		�c �x�N�g��
	void Sub(const YsVec2 &p){
		Sub(*this, *this, p);
	}
	
	//! @brief �x�N�g������
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	//! @return ���ϒl
	static float Dot(const YsVec2 &v1, const YsVec2 &v2){			// �ÓI�֐�
		return D3DXVec2Dot(&v1, &v2);
	}
	//! @brief ����(���ʂ�-1�`1�͈̔͂ɂ���)
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	//! @return ���ϒl
	static float Dot2(const YsVec2 &v1, const YsVec2 &v2){			// �ÓI�֐� �K�� -1<=dot<=1 �ŕԂ�
		float retdot = D3DXVec2Dot(&v1, &v2);
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
	float Dot(const YsVec2 &v1){
		return Dot(*this, v1);
	}
	//! @brief ����(���ʂ�-1�`1�͈̔͂ɂ���) ������v1
	//! @param[in]	v1		�c �x�N�g���P
	//! @return ���ϒl
	float Dot2(const YsVec2 &v1){
		return Dot2(*this, v1);
	}

	//! @brief �X�J���[�{
	//! @param[out]	vOut	�c �v�Z���ʂ̃x�N�g��������
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	s		�c �g�嗦
	static void Scale(YsVec2 &vOut, const YsVec2 &vSrc, float s){		// �ÓI�֐�
		D3DXVec2Scale(&vOut, &vSrc, s);
	}
	//! @brief �X�J���[�{
	//! @param[in]	s		�c �g�嗦
	void Scale(float s){
		Scale(*this, *this, s);
	}

	//! @brief �x�N�g���̒��������߂�
	//! @param[in]	vSrc	�c ���������߂�x�N�g��
	static float Length(const YsVec2 &vSrc){						// �ÓI�֐�
		return D3DXVec2Length(&vSrc);
	}
	//! @brief �x�N�g���̒��������߂�
	float Length(){
		return Length(*this);
	}

	//! @brief �x�N�g���𐳋K��
	//! @param[out]	vOut	�c ���K�������x�N�g��������
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	static void Normalize(YsVec2 &vOut, const YsVec2 &vSrc){			// �ÓI�֐�
//		D3DXVec2Normalize(&vOut, &vSrc);
		float len = YsVec2::Length(vSrc);
		if (len > 0) {
			vOut.x = vSrc.x / len;
			vOut.y = vSrc.y / len;
		}
	}
	//! @brief �x�N�g���𐳋K��
	YsVec2& Normalize(){
		Normalize(*this,*this);
		return *this;
	}
	//! @brief �x�N�g���𐳋K��
	//! @param[out]	vOut	�c ���K�������x�N�g��������
	void Normalize(YsVec2 &vOut) const{
		Normalize(vOut, *this);
	}

	//! @brief ���`���
	static void Lerp(YsVec2 &vOut, const YsVec2 &v1, const YsVec2 &v2, float f)
	{
		D3DXVec2Lerp(&vOut, &v1, &v2, f);
	}
	//! @brief  �G���~�l�[�g�X�v���C�����
	static void Hermite(YsVec2 &vOut, const YsVec2 &pV1, const YsVec2 &pT1, const YsVec2 &pV2, const YsVec2 &pT2, float f)
	{
		D3DXVec2Hermite(&vOut, &pV1, &pT1, &pV2, &pT2, f);
	}
	//! @brief  Catmull-Rom�X�v���C�����
	static void CatmullRom(YsVec2 &vOut, const YsVec2 &pV0, const YsVec2 &pV1, const YsVec2 &pV2, const YsVec2 &pV3, float f)
	{
		D3DXVec2CatmullRom(&vOut, &pV0, &pV1, &pV2, &pV3, f);
	}


	//! @brief  �s��ŕϊ�
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	static void Transform(YsVec2 &vOut, const YsVec2 &vSrc, const D3DXMATRIX &mat){	// �ÓI�֐�
		D3DXVec2TransformCoord(&vOut, &vSrc, &mat);
	}
	//! @brief  �s��ŕϊ�
	//! @param[in]	mat		�c �ϊ��s��
	void Transform(const D3DXMATRIX &mat){
		Transform(*this, *this, mat);
	}
	//! @brief  �s��ŕϊ�
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	void Transform(YsVec2 &vOut, const D3DXMATRIX &mat) const{
		Transform(vOut, *this, mat);
	}

	//! @brief  �s��ŕϊ�(��]�̂�)
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	static void TransformNormal(YsVec2 &vOut, const YsVec2 &vSrc, const D3DXMATRIX &mat){// �ÓI�֐�
		D3DXVec2TransformNormal(&vOut, &vSrc, &mat);
	}
	//! @brief  �s��ŕϊ�(��]�̂�)
	//! @param[in]	mat		�c �ϊ��s��
	void TransformNormal(const D3DXMATRIX &mat){
		TransformNormal(*this, *this, mat);
	}
	//! @brief  �s��ŕϊ�(��]�̂�)
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	void TransformNormal(YsVec2 &vOut, const D3DXMATRIX &mat) const{
		TransformNormal(vOut, *this, mat);
	}

	//! @brief �w������Ɏw��p�x������]�B(�����΂ł��Ȃ���)
	//! @param[in]	vTargetDir	�c ���������̕���
	//! @param[in]	MaxAng		�c �ő��]�p�x�@���̊p�x�ȏ�͉�]���Ȃ�
	void Homing(const YsVec2& vTargetDir, float MaxAng);

	//! @brief �O�p�`��AABB���߂�
	//! @param[in]	v1		�c ���_�P�̍��W
	//! @param[in]	v2		�c ���_�Q�̍��W
	//! @param[in]	v3		�c ���_�R�̍��W
	//! @param[out]	outMinP	�c AABB�̍ŏ����W
	//! @param[out]	outMaxP	�c AABB�̍ő���W
	static void CreateAABB(const YsVec2 &v1, const YsVec2 &v2, const YsVec2 &v3, YsVec2 &outMinP, YsVec2 &outMaxP){
		outMinP = v1;
		outMaxP = v1;
		// �ŏ��_�Z�o
		if(outMinP.x > v2.x)outMinP.x = v2.x;
		if(outMinP.y > v2.y)outMinP.y = v2.y;
		if(outMinP.x > v3.x)outMinP.x = v3.x;
		if(outMinP.y > v3.y)outMinP.y = v3.y;
		// �ő�_�Z�o
		if(outMaxP.x < v2.x)outMaxP.x = v2.x;
		if(outMaxP.y < v2.y)outMaxP.y = v2.y;
		if(outMaxP.x < v3.x)outMaxP.x = v3.x;
		if(outMaxP.y < v3.y)outMaxP.y = v3.y;
	}
	//! @brief ������AABB���Z�o
	//! @param[in]	v1		�c ���W�P
	//! @param[in]	v2		�c ���W�Q
	//! @param[out]	outMinP	�c AABB�̍ŏ����W
	//! @param[out]	outMaxP	�c AABB�̍ő���W
	static void CreateAABBFromLineSegment(const YsVec2 &v1, const YsVec2 &v2, YsVec2 &outMinP, YsVec2 &outMaxP){

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

	}

};

}

#endif
