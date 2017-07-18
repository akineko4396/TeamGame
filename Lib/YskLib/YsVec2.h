//===============================================================
//! @file YsVec2.h
//! @brief 2次元ベクトルクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsVec2_h__
#define __YsVec2_h__

namespace YskLib
{

//! @brief D3DXのベクトルから、DirectXTKのベクトルへポインタ型変換
#define YsToXNAVec2(v) ((DirectX::SimpleMath::Vector2*)v)

//============================================================================
//! @brief ２次元ベクトルクラス
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

	// 各演算子
	YsVec2 operator + () const{ return D3DXVECTOR2::operator+(); }
	YsVec2 operator - () const{ return D3DXVECTOR2::operator-(); }

	YsVec2 operator + ( CONST D3DXVECTOR2& v) const{ return D3DXVECTOR2::operator+(v); }
	YsVec2 operator - ( CONST D3DXVECTOR2& v) const{ return D3DXVECTOR2::operator-(v); }
	YsVec2 operator * ( FLOAT f) const{ return D3DXVECTOR2::operator*(f); }
	YsVec2 operator / ( FLOAT f) const{ return D3DXVECTOR2::operator/(f); }

	// 各要素を乗算する
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

	// 定数ベクトル
	static const YsVec2 Zero;	//!< 全要素0ベクトル
	static const YsVec2 One;	//!< 全要素1ベクトル

	//! @brief ベクトル加算　vOut = v1 + v2
	//! @param[out]	vOut	… 計算結果が入る変数
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	static void Add(YsVec2 &vOut, const YsVec2 &v1, const YsVec2 &v2){	// 静的関数
		D3DXVec2Add(&vOut, &v1, &v2);
	}
	//! @brief ベクトル加算　自分 = 自分 + (fx,fy)
	//! @param[in]	fx		… ベクトルのx成分
	//! @param[in]	fy		… ベクトルのy成分
	void Add(float fx, float fy){
		Add(*this, *this, YsVec2(fx,fy));
	}
	//! @brief ベクトル加算　自分 = 自分 + p
	//! @param[in]	p		… ベクトル
	void Add(const YsVec2 &p){
		Add(*this, *this, p);
	}
	
	//! @brief ベクトル減算　vOut = v1 - v2
	//! @param[out]	vOut	… 計算結果が入る変数
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	static void Sub(YsVec2 &vOut, const YsVec2 &v1, const YsVec2 &v2){	// 静的関数
		D3DXVec2Subtract(&vOut, &v1, &v2);
	}
	//! @brief ベクトル減算　自分 = 自分 - (fx,fy,fz)
	//! @param[in]	fx		… ベクトルのx成分
	//! @param[in]	fy		… ベクトルのy成分
	void Sub(float fx, float fy){
		Sub(*this, *this, YsVec2(fx, fy));
	}
	//! @brief ベクトル減算　自分 = 自分 - p
	//! @param[in]	p		… ベクトル
	void Sub(const YsVec2 &p){
		Sub(*this, *this, p);
	}
	
	//! @brief ベクトル内積
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	//! @return 内積値
	static float Dot(const YsVec2 &v1, const YsVec2 &v2){			// 静的関数
		return D3DXVec2Dot(&v1, &v2);
	}
	//! @brief 内積(結果を-1～1の範囲にする)
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	//! @return 内積値
	static float Dot2(const YsVec2 &v1, const YsVec2 &v2){			// 静的関数 必ず -1<=dot<=1 で返す
		float retdot = D3DXVec2Dot(&v1, &v2);
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
	float Dot(const YsVec2 &v1){
		return Dot(*this, v1);
	}
	//! @brief 内積(結果を-1～1の範囲にする) 自分とv1
	//! @param[in]	v1		… ベクトル１
	//! @return 内積値
	float Dot2(const YsVec2 &v1){
		return Dot2(*this, v1);
	}

	//! @brief スカラー倍
	//! @param[out]	vOut	… 計算結果のベクトルが入る
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	s		… 拡大率
	static void Scale(YsVec2 &vOut, const YsVec2 &vSrc, float s){		// 静的関数
		D3DXVec2Scale(&vOut, &vSrc, s);
	}
	//! @brief スカラー倍
	//! @param[in]	s		… 拡大率
	void Scale(float s){
		Scale(*this, *this, s);
	}

	//! @brief ベクトルの長さを求める
	//! @param[in]	vSrc	… 長さを求めるベクトル
	static float Length(const YsVec2 &vSrc){						// 静的関数
		return D3DXVec2Length(&vSrc);
	}
	//! @brief ベクトルの長さを求める
	float Length(){
		return Length(*this);
	}

	//! @brief ベクトルを正規化
	//! @param[out]	vOut	… 正規化したベクトルが入る
	//! @param[in]	vSrc	… 処理の基となるベクトル
	static void Normalize(YsVec2 &vOut, const YsVec2 &vSrc){			// 静的関数
//		D3DXVec2Normalize(&vOut, &vSrc);
		float len = YsVec2::Length(vSrc);
		if (len > 0) {
			vOut.x = vSrc.x / len;
			vOut.y = vSrc.y / len;
		}
	}
	//! @brief ベクトルを正規化
	YsVec2& Normalize(){
		Normalize(*this,*this);
		return *this;
	}
	//! @brief ベクトルを正規化
	//! @param[out]	vOut	… 正規化したベクトルが入る
	void Normalize(YsVec2 &vOut) const{
		Normalize(vOut, *this);
	}

	//! @brief 線形補間
	static void Lerp(YsVec2 &vOut, const YsVec2 &v1, const YsVec2 &v2, float f)
	{
		D3DXVec2Lerp(&vOut, &v1, &v2, f);
	}
	//! @brief  エルミネートスプライン補間
	static void Hermite(YsVec2 &vOut, const YsVec2 &pV1, const YsVec2 &pT1, const YsVec2 &pV2, const YsVec2 &pT2, float f)
	{
		D3DXVec2Hermite(&vOut, &pV1, &pT1, &pV2, &pT2, f);
	}
	//! @brief  Catmull-Romスプライン補間
	static void CatmullRom(YsVec2 &vOut, const YsVec2 &pV0, const YsVec2 &pV1, const YsVec2 &pV2, const YsVec2 &pV3, float f)
	{
		D3DXVec2CatmullRom(&vOut, &pV0, &pV1, &pV2, &pV3, f);
	}


	//! @brief  行列で変換
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	mat		… 変換行列
	static void Transform(YsVec2 &vOut, const YsVec2 &vSrc, const D3DXMATRIX &mat){	// 静的関数
		D3DXVec2TransformCoord(&vOut, &vSrc, &mat);
	}
	//! @brief  行列で変換
	//! @param[in]	mat		… 変換行列
	void Transform(const D3DXMATRIX &mat){
		Transform(*this, *this, mat);
	}
	//! @brief  行列で変換
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	mat		… 変換行列
	void Transform(YsVec2 &vOut, const D3DXMATRIX &mat) const{
		Transform(vOut, *this, mat);
	}

	//! @brief  行列で変換(回転のみ)
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	mat		… 変換行列
	static void TransformNormal(YsVec2 &vOut, const YsVec2 &vSrc, const D3DXMATRIX &mat){// 静的関数
		D3DXVec2TransformNormal(&vOut, &vSrc, &mat);
	}
	//! @brief  行列で変換(回転のみ)
	//! @param[in]	mat		… 変換行列
	void TransformNormal(const D3DXMATRIX &mat){
		TransformNormal(*this, *this, mat);
	}
	//! @brief  行列で変換(回転のみ)
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	mat		… 変換行列
	void TransformNormal(YsVec2 &vOut, const D3DXMATRIX &mat) const{
		TransformNormal(vOut, *this, mat);
	}

	//! @brief 指定方向に指定角度だけ回転。(正反対でも曲がる)
	//! @param[in]	vTargetDir	… 向きたいの方向
	//! @param[in]	MaxAng		… 最大回転角度　この角度以上は回転しない
	void Homing(const YsVec2& vTargetDir, float MaxAng);

	//! @brief 三角形のAABB求める
	//! @param[in]	v1		… 頂点１の座標
	//! @param[in]	v2		… 頂点２の座標
	//! @param[in]	v3		… 頂点３の座標
	//! @param[out]	outMinP	… AABBの最少座標
	//! @param[out]	outMaxP	… AABBの最大座標
	static void CreateAABB(const YsVec2 &v1, const YsVec2 &v2, const YsVec2 &v3, YsVec2 &outMinP, YsVec2 &outMaxP){
		outMinP = v1;
		outMaxP = v1;
		// 最小点算出
		if(outMinP.x > v2.x)outMinP.x = v2.x;
		if(outMinP.y > v2.y)outMinP.y = v2.y;
		if(outMinP.x > v3.x)outMinP.x = v3.x;
		if(outMinP.y > v3.y)outMinP.y = v3.y;
		// 最大点算出
		if(outMaxP.x < v2.x)outMaxP.x = v2.x;
		if(outMaxP.y < v2.y)outMaxP.y = v2.y;
		if(outMaxP.x < v3.x)outMaxP.x = v3.x;
		if(outMaxP.y < v3.y)outMaxP.y = v3.y;
	}
	//! @brief 線分のAABBを算出
	//! @param[in]	v1		… 座標１
	//! @param[in]	v2		… 座標２
	//! @param[out]	outMinP	… AABBの最少座標
	//! @param[out]	outMaxP	… AABBの最大座標
	static void CreateAABBFromLineSegment(const YsVec2 &v1, const YsVec2 &v2, YsVec2 &outMinP, YsVec2 &outMaxP){

		// 線分のバウンディングボックス算出
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
