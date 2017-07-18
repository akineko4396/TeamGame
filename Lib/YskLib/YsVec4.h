//===============================================================
//! @file YsVec4.h
//! @brief 4�����x�N�g���N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsVec4_h__
#define __YsVec4_h__

namespace YskLib
{

//! @brief D3DX�̃x�N�g������ADirectXTK�̃x�N�g���փ|�C���^�^�ϊ�
#define YsToXNAVec4(v) ((DirectX::SimpleMath::Vector4*)v)

//============================================================================
//! @brief �S�����x�N�g���N���X
//!
//! @ingroup Math
//============================================================================
class YsVec4 : public D3DXVECTOR4{
public:

	YsVec4(){
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	YsVec4(float fx, float fy, float fz, float fw){
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}
	YsVec4(const D3DXVECTOR4 &In){
		x = In.x;
		y = In.y;
		z = In.z;
		w = In.w;
	}

	void Set(float fx, float fy, float fz, float fw){
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}
	void Set(const D3DXVECTOR4& v){
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	void Set(const D3DXVECTOR3& v3, float fw){
		x = v3.x;
		y = v3.y;
		z = v3.z;
		w = fw;
	}

	YsVec4& operator=(const D3DXVECTOR4& v){
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	// �e���Z�q
	YsVec4 operator + () const{ return D3DXVECTOR4::operator+(); }
	YsVec4 operator - () const{ return D3DXVECTOR4::operator-(); }

	YsVec4 operator + ( CONST D3DXVECTOR4& v) const{ return D3DXVECTOR4::operator+(v); }
	YsVec4 operator - ( CONST D3DXVECTOR4& v) const{ return D3DXVECTOR4::operator-(v); }
	YsVec4 operator * ( FLOAT f) const{ return D3DXVECTOR4::operator*(f); }
	YsVec4 operator / ( FLOAT f) const{ return D3DXVECTOR4::operator/(f); }

	// �e�v�f����Z����
	YsVec4& operator *= (const D3DXVECTOR4& v){
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	YsVec4 operator * (const D3DXVECTOR4& v) const{
		D3DXVECTOR4 v4;
		v4.x = x * v.x;
		v4.y = y * v.y;
		v4.z = z * v.z;
		v4.w = w * v.w;
		return v4;
	}
	YsVec4& operator *= (float f){ return (YsVec4&)D3DXVECTOR4::operator*=(f); }
	YsVec4& operator /= (float f){ return (YsVec4&)D3DXVECTOR4::operator/=(f); }


	// �萔�x�N�g��
	static const YsVec4 Zero;	//!< �S�v�f0�x�N�g��
	static const YsVec4 One;	//!< �S�v�f1�x�N�g��

	//! @brief �x�N�g�����Z�@vOut = v1 + v2
	//! @param[out]	vOut	�c �v�Z���ʂ�����ϐ�
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	static void Add(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2){	// �ÓI�֐�
		D3DXVec4Add(vOut,v1,v2);
	}
	//! @brief �x�N�g�����Z�@���� = ���� + (fx,fy,fz,fw)
	//! @param[in]	fx		�c �x�N�g����x����
	//! @param[in]	fy		�c �x�N�g����y����
	//! @param[in]	fz		�c �x�N�g����z����
	//! @param[in]	fw		�c �x�N�g����w����
	void Add(float fx, float fy, float fz, float fw){
		Add(this,this,&YsVec4(fx,fy,fz,fw));
	}
	//! @brief �x�N�g�����Z�@���� = ���� + p
	//! @param[in]	p		�c �x�N�g��
	void Add(const YsVec4 *p){
		Add(this,this,p);
	}
	
	//! @brief �x�N�g�����Z�@vOut = v1-v2
	//! @param[out]	vOut	�c �v�Z���ʂ�����ϐ�
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	static void Sub(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2){	// �ÓI�֐�
		D3DXVec4Subtract(vOut,v1,v2);
	}
	//! @brief �x�N�g�����Z�@���� = ���� - (fx,fy,fz,fw)
	//! @param[in]	fx		�c �x�N�g����x����
	//! @param[in]	fy		�c �x�N�g����y����
	//! @param[in]	fz		�c �x�N�g����z����
	//! @param[in]	fw		�c �x�N�g����w����
	void Sub(float fx, float fy, float fz, float fw){
		Sub(this,this,&YsVec4(fx,fy,fz,fw));
	}
	//! @brief �x�N�g�����Z�@���� = ���� - p
	//! @param[in]	p		�c �x�N�g��
	void Sub(const YsVec4 *p){
		Sub(this,this,p);
	}
	
	//! @brief �x�N�g������
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	//! @return ���ϒl
	static float Dot(const YsVec4 *v1, const YsVec4 *v2){			// �ÓI�֐�
		return D3DXVec4Dot(v1,v2);
	}
	//! @brief ����(���ʂ�-1�`1�͈̔͂ɂ���)
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	//! @return ���ϒl
	static float Dot2(const YsVec4 *v1, const YsVec4 *v2){			// �ÓI�֐� �K�� -1<=dot<=1 �ŕԂ�
		float retdot = D3DXVec4Dot(v1,v2);
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
	float Dot(const YsVec4 *v1){
		return Dot(this,v1);
	}
	//! @brief ����(���ʂ�-1�`1�͈̔͂ɂ���) ������v1
	//! @param[in]	v1		�c �x�N�g���P
	//! @return ���ϒl
	float Dot2(const YsVec4 *v1){
		return Dot2(this,v1);
	}

	//! @brief �O��
	//! @param[out]	vOut	�c �v�Z���ʂ����x�N�g��������
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	//! @param[in]	v3		�c �x�N�g���R
	static void Cross(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2, const YsVec4 *v3){// �ÓI�֐�
		D3DXVec4Cross(vOut,v1,v2,v3);
	}
	//! @brief �O�� ���� x v1 x v2
	//! @param[out]	vOut	�c �v�Z���ʂ̃x�N�g��������
	//! @param[in]	v1		�c �x�N�g���P
	//! @param[in]	v2		�c �x�N�g���Q
	void Cross(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2){
		Cross(vOut,this,v1,v2);
	}
	
	//! @brief �X�J���[�{
	//! @param[out]	vOut	�c �v�Z���ʂ̃x�N�g��������
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	s		�c �g�嗦
	static void Scale(YsVec4 *vOut, const YsVec4 *vSrc, float s){		// �ÓI�֐�
		D3DXVec4Scale(vOut,vSrc,s);
	}
	//! @brief �X�J���[�{
	//! @param[in]	s		�c �g�嗦
	void Scale(float s){
		Scale(this,this,s);
	}

	//! @brief �x�N�g���̒���
	//! @param[in]	vSrc	�c ���������߂�x�N�g��
	static float Length(const YsVec4 *vSrc){						// �ÓI�֐�
		return D3DXVec4Length(vSrc);
	}
	//! @brief �x�N�g���̒���
	float Length(){
		return Length(this);
	}
	//! @brief �x�N�g���𐳋K��
	//! @param[out]	vOut	�c ���K�������x�N�g��������
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	static void Normalize(YsVec4 *vOut, const YsVec4 *vSrc){			// �ÓI�֐�
//		D3DXVec4Normalize(vOut,vSrc);
		float len = YsVec4::Length(vSrc);
		if (len > 0) {
			vOut->x = vSrc->x / len;
			vOut->y = vSrc->y / len;
			vOut->z = vSrc->z / len;
			vOut->w = vSrc->w / len;
		}
	}
	//! @brief �x�N�g���𐳋K��
	YsVec4& Normalize(){
		Normalize(this,this);
		return *this;
	}
	//! @brief �x�N�g���𐳋K��
	//! @param[out]	vOut	�c ���K�������x�N�g��������
	void Normalize(YsVec4 *vOut) const{
		Normalize(vOut,this);
	}

	//! @brief ���`���
	static void Lerp(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2, float f)
	{
		D3DXVec4Lerp(vOut,v1,v2,f);
	}
	//! @brief �G���~�l�[�g�X�v���C�����
	static void Hermite(YsVec4 *vOut, const YsVec4 *pV1, const YsVec4 *pT1, const YsVec4 *pV2, const YsVec4 *pT2, float f)
	{
		D3DXVec4Hermite(vOut,pV1,pT1,pV2,pT2,f);
	}
	//! @brief Catmull-Rom�X�v���C�����
	static void CatmullRom(YsVec4 *vOut, const YsVec4 *pV0, const YsVec4 *pV1, const YsVec4 *pV2, const YsVec4 *pV3, float f)
	{
		D3DXVec4CatmullRom(vOut,pV0,pV1,pV2,pV3,f);
	}


	//! @brief �s��ŕϊ�
	//! @param[out]	vOut	�c �ϊ���̃x�N�g��
	//! @param[in]	vSrc	�c �����̊�ƂȂ�x�N�g��
	//! @param[in]	mat		�c �ϊ��s��
	static void Transform(YsVec4 *vOut, const YsVec4 *vSrc, const D3DXMATRIX *mat){	// �ÓI�֐�
		D3DXVec4Transform(vOut, vSrc, mat);
	}
	//! @brief �s��ŕϊ�
	//! @param[in]	mat		�c �ϊ��s��
	void Transform(const D3DXMATRIX *mat){
		Transform(this, this, mat);
	}
	//! @brief �s��ŕϊ�
	//! param[out]	vOut	�c �ϊ���̃x�N�g��
	//! param[in]	mat		�c �ϊ��s��
	void Transform(YsVec4 *vOut, const D3DXMATRIX *mat) const{
		Transform(vOut, this, mat);
	}

	//! @brief  YsVec2�Ƃ��ĕԂ�
	YsVec2& GetVec2(){
		return *(YsVec2*)this;
	}
	//! @brief  YsVec3�Ƃ��ĕԂ�
	YsVec3& GetVec3(){
		return *(YsVec3*)this;
	}
};

}

#endif
