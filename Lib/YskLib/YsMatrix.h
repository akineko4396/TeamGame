//===============================================================
//! @file YsMatrix.h
//! @brief 行列クラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsMatrix_h__
#define __YsMatrix_h__

namespace YskLib{

//! @brief D3DXの行列から、DirectXTKの行列へポインタ型変換
#define YsToXNAMatrix(v) ((DirectX::SimpleMath::Matrix*)v)

//==========================================================================================================
//! @brief 行列操作クラス
//!
//! @ingroup Math
//==========================================================================================================
class YsMatrix : public D3DXMATRIX{
public:
	//============================================================
	// メンバ関数
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
	// 行列「合成」系関数･･･行列を作成し、合成していく(相対的変化)
	//
	//========================================================================
	//! @brief 移動をワールド合成。
	//! @param[in]	x … x座標
	//! @param[in]	y … y座標
	//! @param[in]	z … z座標
	void Move(float x, float y, float z)
	{
		_41 += x;
		_42 += y;
		_43 += z;
	}

	//! @brief 移動をワールド合成。
	//! @param[in]	v … ベクトル
	void Move(const YsVec3 &v)
	{
		_41 += v.x;
		_42 += v.y;
		_43 += v.z;
	}

	//! @brief 移動をローカル合成。
	//! @param[in]	x … x座標
	//! @param[in]	y … y座標
	//! @param[in]	z … z座標
	void Move_Local(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixTranslation(&TmpMat,x,y,z);
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}

	//! @brief 移動をローカル合成。
	//! @param[in]	v … ベクトル
	void Move_Local(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixTranslation(&TmpMat,v.x, v.y, v.z);
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}

	//! @brief X回転をワールド合成。
	//! @param[in]	Ang … 回転角度(度)
	void RotateX(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationX(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this, this, &TmpMat);// 合成
	}

	//! @brief Y回転をワールド合成。
	//! @param[in]	Ang … 回転角度(度)
	void RotateY(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationY(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this, this, &TmpMat);// 合成
	}

	//! @brief Z回転をワールド合成。
	//! @param[in]	Ang … 回転角度(度)
	void RotateZ(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationZ(&TmpMat, YsToRadian(Ang));
		D3DXMatrixMultiply(this,this, &TmpMat);// 合成
	}

	//! @brief 指定軸回転をワールド合成。
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度(度)
	void RotateAxis(const YsVec3 &vAxis, float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationAxis(&TmpMat, &vAxis, YsToRadian(Ang));
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
	}

	//! @brief X回転をワールド合成(座標は変化させない)。
	//! @param[in]	Ang		… 回転角度(度)
	void RotateX_Normal(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationX(&TmpMat, YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief Y回転をワールド合成(座標は変化させない)。
	//! @param[in]	Ang		… 回転角度(度)
	void RotateY_Normal(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationY(&TmpMat,YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief Z回転をワールド合成(座標は変化させない)。
	//! @param[in]	Ang		… 回転角度(度)
	void RotateZ_Normal(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationZ(&TmpMat,YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this, this, &TmpMat);// 合成
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief 指定軸回転をワールド合成(座標は変化させない)。
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度(度)
	void RotateAxis_Normal(const YsVec3 &vAxis, float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationAxis(&TmpMat, &vAxis, YsToRadian(Ang));

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief X回転をローカル合成。
	//! @param[in]	Ang		… 回転角度(度)
	void RotateX_Local(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationX(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}

	//! @brief Y回転をローカル合成。
	//! @param[in]	Ang		… 回転角度(度)
	void RotateY_Local(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationY(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}

	//! @brief Z回転をローカル合成。
	//! @param[in]	Ang		… 回転角度(度)
	void RotateZ_Local(float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationZ(&TmpMat,YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}

	//! @brief 指定軸回転をローカル合成。
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度(度)
	void RotateAxis_Local(const YsVec3 &vAxis, float Ang)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixRotationAxis(&TmpMat, &vAxis, YsToRadian(Ang));
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}

	//! @brief 拡大をワールド合成。
	//! @param[in]	x		… x拡大率
	//! @param[in]	y		… y拡大率
	//! @param[in]	z		… z拡大率
	void Scale(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixScaling(&TmpMat,x,y,z);
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
	}

	//! @brief 拡大をワールド合成。
	//! @param[in]	v		… 拡大率(x,y,z)
	void Scale(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixScaling(&TmpMat,v.x, v.y, v.z);
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
	}

	//! @brief 拡大をローカル合成。
	//! @param[in]	x		… x拡大率
	//! @param[in]	y		… y拡大率
	//! @param[in]	z		… z拡大率
	void Scale_Local(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixScaling(&TmpMat,x,y,z);
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}
	//! @brief 拡大をローカル合成。
	//! @param[in]	v		… 拡大率(x,y,z)
	void Scale_Local(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixScaling(&TmpMat,v.x, v.y, v.z);
		D3DXMatrixMultiply(this,&TmpMat,this);// 合成
	}

	//! @brief 拡大をワールド合成(座標は変化しません)。
	//! @param[in]	x		… x拡大率
	//! @param[in]	y		… y拡大率
	//! @param[in]	z		… z拡大率
	void Scale_Normal(float x, float y, float z)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixScaling(&TmpMat,x,y,z);

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief 拡大をワールド合成(座標は変化しません)。
	//! @param[in]	v		… 拡大率(x,y,z)
	void Scale_Normal(const YsVec3 &v)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixScaling(&TmpMat,v.x, v.y, v.z);

		YsVec3 vTmp;
		vTmp.x = _41;
		vTmp.y = _42;
		vTmp.z = _43;
		_41 = _42 = _43 = 0;
		D3DXMatrixMultiply(this,this,&TmpMat);// 合成
		_41 = vTmp.x;
		_42 = vTmp.y;
		_43 = vTmp.z;
	}

	//! @brief Lookの方向に瞬時に向ける(相対的)。※合成系だけど、原点中心でなく、その場で回転します
	//! @param[in]	Look	… 向きたい方向
	void RotateLookAtRelative(const YsVec3 &Look);

	//! @brief Lookの方向にMaxAngの角度だけ向ける(相対的)。※合成系だけど、原点中心でなく、その場で回転します
	//! @param[in]	Look	… 向きたい方向
	//! @param[in]	MaxAng	… 最大角度
	void RotateLookAtRelativeAngle(	const YsVec3 &Look,
									float MaxAng);

	//! @brief 指定した行列をワールド合成(後ろに合成)
	//! @param[in]	mat		… 合成する行列
	void Multiply(const YsMatrix &mat)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixMultiply(this, this, &mat);// 合成
	}

	//! @brief 指定した行列をローカル合成(手前に合成)
	//! @param[in]	mat		… 合成する行列
	void Multiply_Local(const YsMatrix &mat)
	{
		D3DXMATRIX TmpMat;		// 作業用
		D3DXMatrixMultiply(this, &mat, this);// 合成
	}
	static void Multiply(YsMatrix &outMat, const YsMatrix &mat1, const YsMatrix &mat2)
	{
		D3DXMatrixMultiply(&outMat, &mat1, &mat2);// 合成
	}

	//========================================================================
	//
	// 行列「作成・操作」系関数･･･行列の内容を直接操作する関数(絶対的変化)
	//
	//========================================================================
	//! @brief 単位行列にする。
	void CreateIdentity()
	{
		D3DXMatrixIdentity(this);
	}

	//! @brief 回転のみ単位行列にする。
	void CreateIdentityRotate()
	{
		_11 = 1;_12 = 0;_13=0;_14 = 0;
		_21 = 0;_22 = 1;_23=0;_24 = 0;
		_31 = 0;_32 = 0;_33=1;_34 = 0;
	}

	//! @brief 指定行列を単位行列にする
	//! @param[out]	m	… 単位行列にしたい行列
	static void CreateIdentity(YsMatrix &m){
		D3DXMatrixIdentity(&m);
	}

	//! @brief 逆行列にする
	void CreateInverse()
	{
//		CreateInverse(this, this);
		YsToXNAMatrix(this)->Invert(*YsToXNAMatrix(this));
	}

	//! @brief この行列を逆行列にしたものを、mOutに入れる
	//! @param[out]	mOut	… 逆行列にしたものを格納する行列
	void CreateInverse(YsMatrix &mOut) const
	{
		CreateInverse(mOut, *this);
	}

	//! @brief 指定行列を逆行列にしたものを、mOutに入れる
	//! @param[out]	mOut			… 逆行列にしたものを格納する変数
	//! @param[out]	pDeterminant	… 行列式をうけとる変数
	//! @param[in]	mIn				… 逆行列にする行列
	static void CreateInverse(YsMatrix &mOut, const YsMatrix &mIn){
//		D3DXMatrixInverse(mOut,pDeterminant,mIn);
		YsToXNAMatrix(&mIn)->Invert(*YsToXNAMatrix(&mOut));
	}

	//! @brief 移動行列を作成
	//! @param[in]	x	… x座標
	//! @param[in]	y	… y座標
	//! @param[in]	z	… z座標
	void CreateMove(float x, float y, float z)
	{
		D3DXMatrixTranslation(this,x,y,z);
	}

	//! @brief 移動行列を作成
	//! @param[in]	v	… 座標(x,y,z)
	void CreateMove(const YsVec3 &v)
	{
		D3DXMatrixTranslation(this,v.x, v.y, v.z);
	}

	//! @brief 移動行列を作成しmOutに入れる
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	x		… x座標
	//! @param[in]	y		… y座標
	//! @param[in]	z		… z座標
	static void CreateMove(YsMatrix &mOut, float x, float y, float z)
	{
		D3DXMatrixTranslation(&mOut, x, y, z);
	}

	//! @brief 移動行列を作成しmOutに入れる
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	v		… 座標(x,y,z)
	static void CreateMove(YsMatrix &mOut, const YsVec3& v)
	{
		D3DXMatrixTranslation(&mOut,v.x, v.y, v.z);
	}

	//! @brief X回転行列を作成
	//! @param[in]	Ang		… 回転角度(度)
	void CreateRotateX(float Ang)
	{
		D3DXMatrixRotationX(this,YsToRadian(Ang));
	}

	//! @brief X回転行列を作成を作成し、mOutに入れる
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	Ang		… 回転角度(度)
	static void CreateRotateX(YsMatrix &mOut, float Ang)
	{
		D3DXMatrixRotationX(&mOut,YsToRadian(Ang));
	}

	//! @brief Y回転行列を作成
	//! @param[in]	Ang		… 回転角度(度)
	void CreateRotateY(float Ang)
	{
		D3DXMatrixRotationY(this,YsToRadian(Ang));
	}

	//! @brief Y回転行列を作成を作成し、mOutに入れる
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	Ang		… 回転角度(度)
	static void CreateRotateY(YsMatrix &mOut, float Ang)
	{
		D3DXMatrixRotationY(&mOut,YsToRadian(Ang));
	}

	//! @brief Z回転行列を作成
	//! @param[in]	Ang		… 回転角度(度)
	void CreateRotateZ(float Ang)
	{
		D3DXMatrixRotationZ(this,YsToRadian(Ang));
	}
	//! @brief Z回転行列を作成を作成し、mOutに入れる
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	Ang		… 回転角度(度)
	static void CreateRotateZ(YsMatrix &mOut, float Ang)
	{
		D3DXMatrixRotationZ(&mOut,YsToRadian(Ang));
	}

	//! @brief 任意軸回転行列を作成
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度(度)
	void CreateRotateAxis(const YsVec3 &vAxis, float Ang)
	{
		D3DXMatrixRotationAxis(this,&vAxis,YsToRadian(Ang));
	}
	//! @brief 任意軸回転行列を作成し、mOutに入れる
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度(度)
	static void CreateRotateAxis(YsMatrix *mOut, const YsVec3 &vAxis, float Ang)
	{
		D3DXMatrixRotationAxis(mOut,&vAxis,YsToRadian(Ang));
	}

	//! @brief スケーリング行列を作成
	//! @param[in]	x	… x拡大率
	//! @param[in]	y	… y拡大率
	//! @param[in]	z	… z拡大率
	void CreateScale(float x, float y, float z)
	{
		D3DXMatrixScaling(this,x,y,z);
	}

	//! @brief スケーリング行列を作成
	//! @param[in]	v	… 拡大率(x,y,z)
	void CreateScale(const YsVec3 &v)
	{
		D3DXMatrixScaling(this,v.x, v.y, v.z);
	}

	//! @brief スケーリング行列を作成
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	x		… x拡大率
	//! @param[in]	y		… y拡大率
	//! @param[in]	z		… z拡大率
	static void CreateScale(YsMatrix &mOut, float x, float y, float z)
	{
		D3DXMatrixScaling(&mOut,x,y,z);
	}

	//! @brief スケーリング行列を作成
	//! @param[out]	mOut	… 受け取る行列
	//! @param[in]	v		… 拡大率(x,y,z)
	static void CreateScale(YsMatrix &mOut, const YsVec3 &v)
	{
		D3DXMatrixScaling(&mOut, v.x, v.y, v.z);
	}

	//! @brief 指定平面でミラー反転させるための行列を作成
	//! @param[in] planePos		… 平面上の座標
	//! @param[in] planeDir		… 平面の方向
	void CreateRefrect(const YsVec3& planePos, const YsVec3& planeDir){
		D3DXPLANE plane;
		D3DXPlaneFromPointNormal(&plane, &planePos, &planeDir);	// 面データ作成
		YsMatrix mMirror;
		D3DXMatrixReflect(this, &plane);	// 指定面でミラー反転させるための行列を作成
	}

	//! @brief 拡大成分を正規化
	void NormalizeScale() {
		GetXAxis().Normalize();
		GetYAxis().Normalize();
		GetZAxis().Normalize();
	}

	//! @brief 指定方向にZ軸を向けた行列を作成。座標は現在のものが使われる
	//! @param[in]	LookDir	… 向きたい方向
	//! @param[in]	Up		… 上方向となるベクトル
	void SetLookAt(const YsVec3 &LookDir, const YsVec3 &Up);

	//! @brief 指定位置にZ軸を向けた行列を作成。座標も指定する。
	//! @param[in]	MyPos	… 座標
	//! @param[in]	LookDir	… 向きたい方向
	//! @param[in]	Up		… 上方向となるベクトル
	void SetLookAt(const YsVec3 &MyPos, const YsVec3 &LookDir, const YsVec3 &Up){
		SetPos(MyPos);
		SetLookAt(LookDir, Up);
	}

	//! @brief 現在の位置から指定位置にZ軸を向けた行列を作成。座標は変化せず、現在のものが使われる
	//! @param[in]	TargetPos	… 向きたい位置
	//! @param[in]	Up			… 上方向となるベクトル
	void SetLookAtPos(const YsVec3 &TargetPos, const YsVec3 &Up);

	//! @brief 現在の位置から指定位置にZ軸を向けた行列を作成。座標は変化せず、現在のものが使われる
	//! @param[in]	MyPos		… 座標
	//! @param[in]	TargetPos	… 向きたい位置
	//! @param[in]	Up			… 上方向となるベクトル
	void SetLookAtPos(const YsVec3 &MyPos, const YsVec3 &TargetPos, const YsVec3 &Up){
		SetPos(MyPos);
		SetLookAtPos(TargetPos, Up);
	}

	//! @brief カメラの向きと同じ向きにする。座標はそのまま。拡大も維持
	//! @param[in]	lpCamMat	… カメラ行列(ビュー行列ではない)
	void SetBillBoard(const YsMatrix &lpCamMat)
	{
		float x = GetXScale();
		float y = GetXScale();
		float z = GetXScale();
		
		// 回転部分だけをコピー(拡大は維持)
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

	//! @brief Y軸は指定し、Z軸をカメラの方向に極力向けるような行列にする。レーザーのようなビルボード時に使う。
	//! @param[in]	vCamPos	… カメラ座標
	//! @param[in]	vY		… Y軸となるベクトル
	void SetBillBoardAxisY(const YsVec3 &vCamPos, const YsVec3 &vY);

	//! @brief 線形補間 m1～m2の中間位置fの行列を求める。fは0～1。
	//! @param[out]	mOut	… 補間結果が入る行列
	//! @param[in]	m1		… 開始点の行列
	//! @param[in]	m2		… 終了点の行列
	//! @param[in]	f		… 補間位置(0～1)
	static void Lerp(YsMatrix& mOut, const YsMatrix& m1, const YsMatrix& m2, float f);

	//! @brief スプライン補間 m1～m2の中間位置fの行列を求める。fは0～1。
	//!			mPrevはm1の１つ前の行列。mNextはm2の次の行列を指定する。
	//! @param[out]	mOut	… 補間結果が入る行列
	//! @param[in]	mPrev	… 開始点より1つ前の行列
	//! @param[in]	m1		… 開始点の行列
	//! @param[in]	m2		… 終了点の行列
	//! @param[in]	mNext	… 終了点より１つ後の行列
	//! @param[in]	f		… 補間位置(0～1)
	static void SplineLerp(YsMatrix& mOut, const YsMatrix& mPrev, const YsMatrix& m1, const YsMatrix& m2, YsMatrix& mNext, float f);

	//! @brief 座標のみをセットする
	//! @param[in]	fx	… x座標
	//! @param[in]	fy	… y座標
	//! @param[in]	fz	… z座標
	void SetPos(float fx, float fy, float fz){
		_41 = fx;
		_42 = fy;
		_43 = fz;
	}

	//! @brief 座標のみをセットする
	//! @param[in]	v	… 座標(x,y,z)
	void SetPos(const YsVec3 &v){
		_41 = v.x;
		_42 = v.y;
		_43 = v.z;
	}

	//! @brief 座標のみをセットする
	//! @param[in]	lpSrcMat	… 行列(座標のみ使用される)
	void SetPos(const YsMatrix &lpSrcMat){
		_41 = lpSrcMat._41;
		_42 = lpSrcMat._42;
		_43 = lpSrcMat._43;
	}

	//! @brief X軸のみセットする
	//! @param[in]	fx	… ベクトルのx成分
	//! @param[in]	fy	… ベクトルのy成分
	//! @param[in]	fz	… ベクトルのz成分
	void SetXAxis(float fx, float fy, float fz){
		_11 = fx;
		_12 = fy;
		_13 = fz;
	}
	//! @brief X軸のみセットする
	//! @param[in]	v	… X軸となるベクトル
	void SetXAxis(const YsVec3 &v){
		_11 = v.x;
		_12 = v.y;
		_13 = v.z;
	}

	//! @brief Y軸のみセットする
	//! @param[in]	fx	… ベクトルのx成分
	//! @param[in]	fy	… ベクトルのy成分
	//! @param[in]	fz	… ベクトルのz成分
	void SetYAxis(float fx, float fy, float fz){
		_21 = fx;
		_22 = fy;
		_23 = fz;
	}
	//! @brief Y軸のみセットする
	//! @param[in]	v	… Y軸となるベクトル
	void SetYAxis(const YsVec3 &v){
		_21 = v.x;
		_22 = v.y;
		_23 = v.z;
	}

	//! @brief Z軸のみセットする
	//! @param[in]	fx	… ベクトルのx成分
	//! @param[in]	fy	… ベクトルのy成分
	//! @param[in]	fz	… ベクトルのz成分
	void SetZAxis(float fx, float fy, float fz){
		_31 = fx;
		_32 = fy;
		_33 = fz;
	}
	//! @brief Z軸のみセットする
	//! @param[in]	v	… Z軸となるベクトル
	void SetZAxis(const YsVec3 &v){
		_21 = v.x;
		_22 = v.y;
		_23 = v.z;
	}

	//! @brief 回転部分だけ、指定行列からコピーする
	//! @param[in]	lpSrcMat	… コピー元の行列
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

	//! @brief 拡大率を直接操作する
	//! @param[in]	x	… x拡大率
	//! @param[in]	y	… y拡大率
	//! @param[in]	z	… z拡大率
	void SetScale(float x, float y, float z){
		NormalizeScale();	// 一度、拡大成分正規化

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

	// 定数行列
	static const YsMatrix Identity;	//!< 単位行列

	//! @brief 拡大率を直接操作する
	//! @param[in]	size	… 拡大率
	void SetScale(const YsVec3 &pSize){
		SetScale(pSize.x, pSize.y, pSize.z);
	}

	//! @brief 透視投影行列(射影行列)として作成
	void CreatePerspectiveFovLH(float angle, float aspect, float fNear, float fFar) {
		// 射影行列
		D3DXMatrixPerspectiveFovLH(this,
			YsToRadian(angle),		// 視野角
			aspect,					// 画面のアスペクト比
			fNear,					// 最近接距離
			fFar);					// 最遠方距離
	}

	//! @brief 正射影行列(射影行列)として作成
	void CreateOrthoLH(float w, float h, float fNear, float fFar) {
		D3DXMatrixOrthoLH(this, w, h, fNear, fFar);
	}

	//! @brief ビュー行列として作成　注視点を指定するVer
	//! @param[in] pos			… カメラ座標
	//! @param[in] ratgetPos	… 注視点となる座標
	//! @param[in] up			… 上(Y)となる方向
	void CreateViewTransformPos(const YsVec3 &pos, const YsVec3 &targetPos, const YsVec3 &up = YsVec3(0,1,0)) {
		D3DXMatrixLookAtLH(this, &pos, &targetPos, &up);
	}

	//! @brief ビュー行列として作成　カメラ方向を指定するVer
	//! @param[in] pos	… カメラ座標
	//! @param[in] dir	… カメラ方向
	//! @param[in] up	… 上(Y)となる方向
	void CreateViewTransform(const YsVec3 &pos, const YsVec3 &dir, const YsVec3 &up = YsVec3(0, 1, 0)) {
		D3DXMatrixLookAtLH(this, &pos, &(pos + dir), &up);
	}

	//========================================================================
	// 変換
	//========================================================================
	//! @brief 自分の行列の回転部分をクォータニオンへ変換
	//! @param[out]	out	… 変換されたクォータにオンを受け取る変数
	void ToQuaternion(D3DXQUATERNION &out) const{
		D3DXQuaternionRotationMatrix(&out, this);
	}
	//! @brief 指定行列から回転部分をクォータニオンへ変換
	//! @param[out]	out	… 変換されたクォータにオンを受け取る変数
	//! @param[in]	mIn	… 元となる行列
	static void ToQuaternion(D3DXQUATERNION &out, const YsMatrix &mIn){
		D3DXQuaternionRotationMatrix(&out, &mIn);
	}

	//============================================================================
	// 情報取得
	//============================================================================
	//! @brief 座標をベクトル型で取得
	YsVec3& GetPos(){	return (YsVec3&)_41; }
	const YsVec3& GetPos() const { return (YsVec3&)_41; }

	//! @brief X軸をベクトル型で取得
	YsVec3& GetXAxis() { return (YsVec3&)_11; }
	const YsVec3& GetXAxis() const { return (YsVec3&)_11; }

	//! @brief Y軸をベクトル型で取得
	YsVec3& GetYAxis() { return (YsVec3&)_21; }
	const YsVec3& GetYAxis() const { return (YsVec3&)_21; }

	//! @brief Z軸をベクトル型で取得
	YsVec3& GetZAxis() { return (YsVec3&)_31; }
	const YsVec3& GetZAxis() const { return (YsVec3&)_31; }

	//! @brief X拡大率を取得
	float GetXScale() const {
		return D3DXVec3Length(&(YsVec3&)_11);
	}
	//! @brief Y拡大率を取得
	float GetYScale() const {
		return D3DXVec3Length(&(YsVec3&)_21);
	}
	//! @brief Z拡大率を取得
	float GetZScale() const {
		return D3DXVec3Length(&(YsVec3&)_31);
	}
	//! @brief 拡大率(x,y,z)をベクトルで取得
	YsVec3 GetScale() const{
		return YsVec3(GetXScale(), GetYScale(), GetZScale());
	}

	float X() const { return _41; }								//!< X座標取得
	float Y() const { return _42; }								//!< Y座標取得
	float Z() const { return _43; }								//!< Z座標取得

	//! @brief 射影行列から、視野角・最近接距離・最遠方距離を求める
	//! @param[out]	outAng		… 視野角が入ってくる
	//! @param[out]	outNerar	… 最近接距離が入ってくる
	//! @param[out]	outFar		… 最遠方距離が入ってくる
	void ComputePerspectiveInfo(float& outAng, float& outNear, float& outFar) const;
};

}

#endif
