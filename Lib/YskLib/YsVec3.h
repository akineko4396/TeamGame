//===============================================================
//! @file YsVec3.h
//! @brief 3次元ベクトルクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsVec3_h__
#define __YsVec3_h__

namespace YskLib
{

//! @brief D3DXのベクトルから、DirectXTKのベクトルへポインタ型変換
#define YsToXNAVec3(v) ((DirectX::SimpleMath::Vector3*)v)

//============================================================================
//! @brief ３次元ベクトルクラス
//!
//! D3DXVECTOR3を継承して、便利な関数を付け足したもの
//! 一部、DirectXTKの方の関数も使用しています
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

	// 各演算子
	YsVec3 operator + () const{ return D3DXVECTOR3::operator+(); }
	YsVec3 operator - () const{ return D3DXVECTOR3::operator-(); }

	YsVec3 operator + ( CONST D3DXVECTOR3& v) const{ return D3DXVECTOR3::operator+(v); }
	YsVec3 operator - ( CONST D3DXVECTOR3& v) const{ return D3DXVECTOR3::operator-(v); }
	YsVec3 operator * ( FLOAT f) const{ return D3DXVECTOR3::operator*(f); }
	YsVec3 operator / ( FLOAT f) const{ return D3DXVECTOR3::operator/(f); }


	// 各要素を乗算する
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

	// 定数ベクトル
	static const YsVec3 Zero;	//!< 全要素0ベクトル
	static const YsVec3 One;	//!< 全要素1ベクトル
	static const YsVec3 Up;		//!< 上ベクトル
	static const YsVec3 Down;	//!< 下ベクトル
	static const YsVec3 Left;	//!< 左ベクトル
	static const YsVec3 Right;	//!< 右ベクトル
	static const YsVec3 Front;	//!< 前方ベクトル
	static const YsVec3 Back;	//!< 後方ベクトル

	//! @brief ベクトル加算　vOut = v1 + v2
	//! @param[out]	vOut	… 計算結果が入る変数
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	static void Add(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2){	// 静的関数
		D3DXVec3Add(&vOut, &v1, &v2);
	}
	//! @brief ベクトル加算　自分 = 自分 + (fx,fy,fz)
	//! @param[in]	fx		… ベクトルのx成分
	//! @param[in]	fy		… ベクトルのy成分
	//! @param[in]	fz		… ベクトルのz成分
	void Add(float fx, float fy, float fz){
		Add(*this, *this, YsVec3(fx,fy,fz));
	}
	//! @brief ベクトル加算　自分 = 自分 + p
	//! @param[in]	p		… ベクトル
	void Add(const YsVec3 &p){
		Add(*this, *this, p);
	}
	
	//! @brief ベクトル減算　vOut = v1-v2
	//! @param[out]	vOut	… 計算結果が入る変数
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	static void Sub(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2){	// 静的関数
		D3DXVec3Subtract(&vOut, &v1, &v2);
	}
	//! @brief ベクトル減算　自分 = 自分 - (fx,fy,fz)
	//! @param[in]	fx		… ベクトルのx成分
	//! @param[in]	fy		… ベクトルのy成分
	//! @param[in]	fz		… ベクトルのz成分
	void Sub(float fx, float fy, float fz){
		Sub(*this, *this, YsVec3(fx,fy,fz));
	}
	//! @brief ベクトル減算　自分 = 自分 - p
	//! @param[in]	p		… ベクトル
	void Sub(const YsVec3 &p){
		Sub(*this, *this, p);
	}
	
	//! @brief 内積
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	//! @return 内積値
	static float Dot(const YsVec3 &v1, const YsVec3 &v2){			// 静的関数
		return D3DXVec3Dot(&v1, &v2);
	}
	//! @brief 内積(結果を-1～1の範囲にする)
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	//! @return 内積値
	static float Dot2(const YsVec3 &v1, const YsVec3 &v2){			// 静的関数 必ず -1<=dot<=1 で返す
		float retdot = D3DXVec3Dot(&v1, &v2);
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
	float Dot(const YsVec3 &v1){
		return Dot(*this, v1);
	}
	//! @brief 内積(結果を-1～1の範囲にする) 自分とv1
	//! @param[in]	v1		… ベクトル１
	//! @return 内積値
	float Dot2(const YsVec3 &v1){
		return Dot2(*this, v1);
	}

	//! @brief 外積
	//! @param[out]	vOut	… 計算結果のベクトルが入る
	//! @param[in]	v1		… ベクトル１
	//! @param[in]	v2		… ベクトル２
	static void Cross(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2){// 静的関数
		D3DXVec3Cross(&vOut, &v1, &v2);
	}
	//! @brief 外積 自分 x vBack
	//! @param[out]	vOut	… 計算結果のベクトルが入る
	//! @param[in]	vBack	… 対称のベクトル
	void CrossBack(YsVec3 &vOut, const YsVec3 &vBack) const{
		Cross(vOut, *this, vBack);
	}
	//! @brief 外積 vFront x 自分
	//! @param[out]	vOut	… 計算結果のベクトルが入る
	//! @param[in]	vFront	… 対称のベクトル
	void CrossFront(YsVec3 &vOut, const YsVec3 &vFront) const{
		Cross(vOut,vFront, *this);
	}
	
	//! @brief スカラー倍
	//! @param[out]	vOut	… 計算結果のベクトルが入る
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	s		… 拡大率
	static void Scale(YsVec3 &vOut, const YsVec3 &vSrc, float s){		// 静的関数
		D3DXVec3Scale(&vOut,&vSrc,s);
	}
	//! @brief スカラー倍
	//! @param[in]	s		… 拡大率
	void Scale(float s){
		Scale(*this,*this,s);
	}

	//! @brief ベクトルの長さ
	//! @param[in]	vSrc	… 長さを求めるベクトル
	static float Length(const YsVec3 &vSrc){						// 静的関数
		return D3DXVec3Length(&vSrc);
	}
	//! @brief ベクトルの長さ
	float Length(){
		return Length(*this);
	}

	//! @brief ベクトルを正規化
	//! @param[out]	vOut	… 正規化したベクトルが入る
	//! @param[in]	vSrc	… 処理の基となるベクトル
	static void Normalize(YsVec3 &vOut, const YsVec3 &vSrc){			// 静的関数
//		D3DXVec3Normalize(vOut,vSrc);
		// D3DXVec3Normalizeだと誤差が大きい感じだったので、こっちを使用
		float len = YsVec3::Length(vSrc);
		if (len > 0) {
			vOut.x = vSrc.x / len;
			vOut.y = vSrc.y / len;
			vOut.z = vSrc.z / len;
		}
	}
	//! @brief ベクトルを正規化
	YsVec3& Normalize(){
		Normalize(*this,*this);
		return *this;
	}
	//! @brief ベクトルを正規化
	//! @param[out]	vOut	… 正規化したベクトルが入る
	void Normalize(YsVec3 &vOut) const{
		Normalize(vOut,*this);
	}

	//! @brief 線形補間
	//!
	//! 座標pV1～pV2の中間位置f(0～1)の座標を算出
	//!
	//! @param[out]	vOut		… 補間結果の座標が入る
	//! @param[in]	v1			… 開始座標
	//! @param[in]	v2			… 終了座標
	//! @param[in]	f			… 補間位置(0～1)  例えば0なら開始座標(v1)となり、1なら終了座標(v2)となる　0.5ならv1とv2の中間座標となる
	static void Lerp(YsVec3 &vOut, const YsVec3 &v1, const YsVec3 &v2, float f)
	{
		D3DXVec3Lerp(&vOut,&v1,&v2,f);
	}

	//! @brief エルミネートスプライン補間
	static void Hermite(YsVec3 &vOut, const YsVec3 &pV1, const YsVec3 &pT1, const YsVec3 &pV2, const YsVec3 &pT2, float f)
	{
		D3DXVec3Hermite(&vOut,&pV1,&pT1,&pV2,&pT2,f);
	}

	//! @brief Catmull-Romスプライン補間
	//!
	//! 座標pV1～pV2の中間位置f(0～1)の座標をスプライン補間で算出(曲線的な感じの結果になる)
	//!
	//! @param[out]	vOut		… 補間結果の座標が入る
	//! @param[in]	pV0			… pV1の１つ前の座標
	//! @param[in]	pV1			… 開始座標
	//! @param[in]	pV2			… 終了座標
	//! @param[in]	pV3			… pV2の次の座標
	//! @param[in]	f			… 補間位置(0～1)  例えば0なら開始座標(pV0)となり、1なら終了座標(pV1)となる　0.5ならpV0とpV1の中間座標となる
	static void CatmullRom(YsVec3 &vOut, const YsVec3 &pV0, const YsVec3 &pV1, const YsVec3 &pV2, const YsVec3 &pV3, float f)
	{
		D3DXVec3CatmullRom(&vOut,&pV0,&pV1,&pV2,&pV3,f);
	}

	//! @brief 行列で変換
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	mat		… 変換行列
	static void Transform(YsVec3 &vOut, const YsVec3 &vSrc, const D3DXMATRIX &mat){	// 静的関数
		D3DXVec3TransformCoord(&vOut, &vSrc, &mat);
	}
	//! @brief 行列で変換
	//! @param[in]	mat		… 変換行列
	void Transform(const D3DXMATRIX &mat){
		Transform(*this, *this, mat);
	}
	//! @brief 行列で変換
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	mat		… 変換行列
	void Transform(YsVec3 &vOut, const D3DXMATRIX &mat) const{
		Transform(vOut, *this, mat);
	}

	//! @brief 行列で変換(回転のみ)
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	vSrc	… 処理の基となるベクトル
	//! @param[in]	mat		… 変換行列
	static void TransformNormal(YsVec3 &vOut, const YsVec3 &vSrc, const D3DXMATRIX &mat){// 静的関数
		D3DXVec3TransformNormal(&vOut, &vSrc, &mat);
	}
	//! @brief 行列で変換(回転のみ)
	//! @param[in]	mat		… 変換行列
	void TransformNormal(const D3DXMATRIX &mat){
		TransformNormal(*this, *this, mat);
	}
	//! @brief 行列で変換(回転のみ)
	//! @param[out]	vOut	… 変換後のベクトル
	//! @param[in]	mat		… 変換行列
	void TransformNormal(YsVec3 &vOut, const D3DXMATRIX &mat) const{
		TransformNormal(vOut, *this, mat);
	}

	//! @brief 指定方向に指定角度だけ回転。(正反対でも曲がる)
	//! @param[in]	vTargetDir	… 向きたいの方向
	//! @param[in]	MaxAng		… 最大回転角度　この角度以上は回転しない
	void Homing(const YsVec3& vTargetDir, float MaxAng);

	//! @brief 三角形のAABB求める
	//! @param[in]	v1		… 頂点１の座標
	//! @param[in]	v2		… 頂点２の座標
	//! @param[in]	v3		… 頂点３の座標
	//! @param[out]	outMinP	… AABBの最少座標
	//! @param[out]	outMaxP	… AABBの最大座標
	static void CreateAABB(const YsVec3 &v1, const YsVec3 &v2, const YsVec3 &v3, YsVec3 &outMinP, YsVec3 &outMaxP){
		outMinP = v1;
		outMaxP = v1;
		// 最小点算出
		if(outMinP.x > v2.x)outMinP.x = v2.x;
		if(outMinP.y > v2.y)outMinP.y = v2.y;
		if(outMinP.z > v2.z)outMinP.z = v2.z;
		if(outMinP.x > v3.x)outMinP.x = v3.x;
		if(outMinP.y > v3.y)outMinP.y = v3.y;
		if(outMinP.z > v3.z)outMinP.z = v3.z;
		// 最大点算出
		if(outMaxP.x < v2.x)outMaxP.x = v2.x;
		if(outMaxP.y < v2.y)outMaxP.y = v2.y;
		if(outMaxP.z < v2.z)outMaxP.z = v2.z;
		if(outMaxP.x < v3.x)outMaxP.x = v3.x;
		if(outMaxP.y < v3.y)outMaxP.y = v3.y;
		if(outMaxP.z < v3.z)outMaxP.z = v3.z;
	}
	//! @brief 線分のAABBを算出
	//! @param[in]	v1		… 座標１
	//! @param[in]	v2		… 座標２
	//! @param[out]	outMinP	… AABBの最少座標
	//! @param[out]	outMaxP	… AABBの最大座標
	static void CreateAABBFromLineSegment(const YsVec3 &v1, const YsVec3 &v2, YsVec3 &outMinP, YsVec3 &outMaxP){

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

		if(v1.z < v2.z){
			outMinP.z = v1.z;
			outMaxP.z = v2.z;
		}
		else{
			outMinP.z = v2.z;
			outMaxP.z = v1.z;
		}
	}

	//! @brief YsVec2として返す
	YsVec2& GetVec2(){
		return *(YsVec2*)this;
	}

};

}

#endif
