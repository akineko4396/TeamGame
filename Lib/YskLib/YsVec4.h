//===============================================================
//! @file YsVec4.h
//! @brief 4次元ベクトルクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsVec4_h__
#define __YsVec4_h__

namespace YskLib
{

//! @brief D3DXのベクトルから、DirectXTKのベクトルへポインタ型変換
#define YsToXNAVec4(v) ((DirectX::SimpleMath::Vector4*)v)

//============================================================================
//! @brief ４次元ベクトルクラス
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

	// 各演算子
	YsVec4 operator + () const{ return D3DXVECTOR4::operator+(); }
	YsVec4 operator - () const{ return D3DXVECTOR4::operator-(); }

	YsVec4 operator + ( CONST D3DXVECTOR4& v) const{ return D3DXVECTOR4::operator+(v); }
	YsVec4 operator - ( CONST D3DXVECTOR4& v) const{ return D3DXVECTOR4::operator-(v); }
	YsVec4 operator * ( FLOAT f) const{ return D3DXVECTOR4::operator*(f); }
	YsVec4 operator / ( FLOAT f) const{ return D3DXVECTOR4::operator/(f); }

	// 各要素を乗算する
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


	// 定数ベクトル
	static const YsVec4 Zero;	//!< 全要素0ベクトル
	static const YsVec4 One;	//!< 全要素1ベクトル

	//! @brief ベクトル加算　vOut = v1 + v2
	//! @param[out]	vOut	… 計算結果が入る変数
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	static void Add(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2){	// 静的関数
		D3DXVec4Add(vOut,v1,v2);
	}
	//! @brief ベクトル加算　自分 = 自分 + (fx,fy,fz,fw)
	//! @param[in]	fx		… ベクトルのx成分
	//! @param[in]	fy		… ベクトルのy成分
	//! @param[in]	fz		… ベクトルのz成分
	//! @param[in]	fw		… ベクトルのw成分
	void Add(float fx, float fy, float fz, float fw){
		Add(this,this,&YsVec4(fx,fy,fz,fw));
	}
	//! @brief ベクトル加算　自分 = 自分 + p
	//! @param[in]	p		… ベクトル
	void Add(const YsVec4 *p){
		Add(this,this,p);
	}
	
	//! @brief ベクトル減算　vOut = v1-v2
	//! @param[out]	vOut	… 計算結果が入る変数
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	static void Sub(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2){	// 静的関数
		D3DXVec4Subtract(vOut,v1,v2);
	}
	//! @brief ベクトル減算　自分 = 自分 - (fx,fy,fz,fw)
	//! @param[in]	fx		… ベクトルのx成分
	//! @param[in]	fy		… ベクトルのy成分
	//! @param[in]	fz		… ベクトルのz成分
	//! @param[in]	fw		… ベクトルのw成分
	void Sub(float fx, float fy, float fz, float fw){
		Sub(this,this,&YsVec4(fx,fy,fz,fw));
	}
	//! @brief ベクトル減算　自分 = 自分 - p
	//! @param[in]	p		… ベクトル
	void Sub(const YsVec4 *p){
		Sub(this,this,p);
	}
	
	//! @brief ベクトル内積
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	//! @return 内積値
	static float Dot(const YsVec4 *v1, const YsVec4 *v2){			// 静的関数
		return D3DXVec4Dot(v1,v2);
	}
	//! @brief 内積(結果を-1～1の範囲にする)
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	//! @return 内積値
	static float Dot2(const YsVec4 *v1, const YsVec4 *v2){			// 静的関数 必ず -1<=dot<=1 で返す
		float retdot = D3DXVec4Dot(v1,v2);
		if(retdot < -1.0f){
			retdot = -1.0f;
		}
		if(retdot > 1.0f){
			retdot = 1.0f;
		}
		return retdot;
	}
	//! @brief 内積 自分とv1
	//! @param[in]	v1		… ベクトル１
	//! @return 内積値
	float Dot(const YsVec4 *v1){
		return Dot(this,v1);
	}
	//! @brief 内積(結果を-1～1の範囲にする) 自分とv1
	//! @param[in]	v1		… ベクトル１
	//! @return 内積値
	float Dot2(const YsVec4 *v1){
		return Dot2(this,v1);
	}

	//! @brief 外積
	//! @param[out]	vOut	… 計算結果したベクトルが入る
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	//! @param[in]	v3		… ベクトル３
	static void Cross(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2, const YsVec4 *v3){// 静的関数
		D3DXVec4Cross(vOut,v1,v2,v3);
	}
	//! @brief 外積 自分 x v1 x v2
	//! @param[out]	vOut	… 計算結果のベクトルが入る
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	void Cross(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2){
		Cross(vOut,this,v1,v2);
	}
	
	//! @brief スカラー倍
	//! @param[out]	vOut	… 計算結果のベクトルが入る
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	s		… 拡大率
	static void Scale(YsVec4 *vOut, const YsVec4 *vSrc, float s){		// 静的関数
		D3DXVec4Scale(vOut,vSrc,s);
	}
	//! @brief スカラー倍
	//! @param[in]	s		… 拡大率
	void Scale(float s){
		Scale(this,this,s);
	}

	//! @brief ベクトルの長さ
	//! @param[in]	vSrc	… 長さを求めるベクトル
	static float Length(const YsVec4 *vSrc){						// 静的関数
		return D3DXVec4Length(vSrc);
	}
	//! @brief ベクトルの長さ
	float Length(){
		return Length(this);
	}
	//! @brief ベクトルを正規化
	//! @param[out]	vOut	… 正規化したベクトルが入る
	//! @param[in]	vSrc	… 処理の基となるベクトル
	static void Normalize(YsVec4 *vOut, const YsVec4 *vSrc){			// 静的関数
//		D3DXVec4Normalize(vOut,vSrc);
		float len = YsVec4::Length(vSrc);
		if (len > 0) {
			vOut->x = vSrc->x / len;
			vOut->y = vSrc->y / len;
			vOut->z = vSrc->z / len;
			vOut->w = vSrc->w / len;
		}
	}
	//! @brief ベクトルを正規化
	YsVec4& Normalize(){
		Normalize(this,this);
		return *this;
	}
	//! @brief ベクトルを正規化
	//! @param[out]	vOut	… 正規化したベクトルが入る
	void Normalize(YsVec4 *vOut) const{
		Normalize(vOut,this);
	}

	//! @brief 線形補間
	static void Lerp(YsVec4 *vOut, const YsVec4 *v1, const YsVec4 *v2, float f)
	{
		D3DXVec4Lerp(vOut,v1,v2,f);
	}
	//! @brief エルミネートスプライン補間
	static void Hermite(YsVec4 *vOut, const YsVec4 *pV1, const YsVec4 *pT1, const YsVec4 *pV2, const YsVec4 *pT2, float f)
	{
		D3DXVec4Hermite(vOut,pV1,pT1,pV2,pT2,f);
	}
	//! @brief Catmull-Romスプライン補間
	static void CatmullRom(YsVec4 *vOut, const YsVec4 *pV0, const YsVec4 *pV1, const YsVec4 *pV2, const YsVec4 *pV3, float f)
	{
		D3DXVec4CatmullRom(vOut,pV0,pV1,pV2,pV3,f);
	}


	//! @brief 行列で変換
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	mat		… 変換行列
	static void Transform(YsVec4 *vOut, const YsVec4 *vSrc, const D3DXMATRIX *mat){	// 静的関数
		D3DXVec4Transform(vOut, vSrc, mat);
	}
	//! @brief 行列で変換
	//! @param[in]	mat		… 変換行列
	void Transform(const D3DXMATRIX *mat){
		Transform(this, this, mat);
	}
	//! @brief 行列で変換
	//! param[out]	vOut	… 変換後のベクトル
	//! param[in]	mat		… 変換行列
	void Transform(YsVec4 *vOut, const D3DXMATRIX *mat) const{
		Transform(vOut, this, mat);
	}

	//! @brief  YsVec2として返す
	YsVec2& GetVec2(){
		return *(YsVec2*)this;
	}
	//! @brief  YsVec3として返す
	YsVec3& GetVec3(){
		return *(YsVec3*)this;
	}
};

}

#endif
