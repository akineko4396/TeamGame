//===============================================================
//! @file YsQuat.h
//! @brief クォータニオンクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsQuat_h__
#define __YsQuat_h__

namespace YskLib{

//============================================================================
//! @brief クォータニオンクラス
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

	//! @brief 自分と内積
	//! @param[in]	q	… 対称のクォータにオン
	//! @return 内積値
	float Dot(const YsQuat &q){
		return D3DXQuaternionDot(this, &q);
	}
	//! @brief 内積
	//! @param[in]	q1	… クォータにオン１
	//! @param[in]	q2	… クォータにオン２
	//! @return 内積値
	static float Dot(const YsQuat &q1, const YsQuat &q2){
		return D3DXQuaternionDot(&q1, &q2);
	}


	//! @brief 指数関数を計算する
	//! @param[in]	q	… 処理の基になるクォータニオン
	// このメソッドは、純粋クォータニオンを単位クォータニオンに変換します。
	// つまりD3DXQuaternionExp は、純粋クォータニオンが渡されるものと仮定し、w を無視して計算を行います (w == 0)。
	void Exp(const YsQuat &q){
		D3DXQuaternionExp(this, &q);
	}
	//! @brief 指数関数を計算する
	//! @param[out]	qOut	… 結果が入る変数
	//! @param[in]	q		… 処理の基になるクォータニオン
	static void Exp(YsQuat &qOut, const YsQuat &q){
		D3DXQuaternionExp(&qOut, &q);
	}

	//! @brief 単位クォータニオンにする
	void Identity(){
		D3DXQuaternionIdentity(this);
	}
	//! @brief 単位クォータニオンにする
	//! @param[out]	qOut	… 結果が入る変数
	static void Identity(YsQuat &qOut){
		D3DXQuaternionIdentity(&qOut);
	}

	//! @brief クォータニオンを共役して、再正規化します。
	void Inverse(){
		D3DXQuaternionInverse(this,this);
	}
	//! @brief クォータニオンを共役して、再正規化します。
	//! @param[out]	qOut	… 結果が入る変数
	//! @param[in]	qIn		… 処理の基になるクォータニオン
	static void Inverse(YsQuat &qOut, const YsQuat &qIn){
		D3DXQuaternionInverse(&qOut, &qIn);
	}

	//! @brief 正規化
	void Normalize(){
		D3DXQuaternionNormalize(this,this);
	}
	//! @brief 正規化
	//! @param[out]	qOut	… 結果が入る変数
	//! @param[in]	qIn		… 処理の基になるクォータニオン
	static void Normalize(YsQuat &out, const YsQuat &qIn){
		D3DXQuaternionNormalize(&out, &qIn);
	}

	//! @brief クォータニオンの長さ
	float Length(){
		return D3DXQuaternionLength(this);
	}
	//! @brief クォータニオンの長さ
	//! @param[in]	qIn		… 処理の基になるクォータニオン
	static float Length(const YsQuat &qIn){
		return D3DXQuaternionLength(&qIn);
	}

	//! @brief 自然対数を計算
	void Ln(){
		D3DXQuaternionLn(this,this);
	}
	//! @brief 自然対数を計算
	//! @param[out]	qOut	… 結果が入る変数
	//! @param[in]	qIn		… 処理の基になるクォータニオン
	static void Ln(YsQuat &qOut, const YsQuat &qIn){
		D3DXQuaternionLn(&qOut, &qIn);
	}

	//! @brief 積(後ろから)
	//! @param[in]	qIn		… 処理の基になるクォータニオン
	void Multiply(const YsQuat &qIn){
		D3DXQuaternionMultiply(this,this, &qIn);
	}
	//! @brief 積(手前から)
	//! @param[in]	qIn		… 処理の基になるクォータニオン
	void Multiply_Local(const YsQuat &qIn){
		D3DXQuaternionMultiply(this, &qIn,this);
	}
	//! @brief 積(手前から)
	//! @param[out]	qOut	… 結果が入る変数
	//! @param[in]	qIn1	… クォータニオン１
	//! @param[in]	qIn2	… クォータニオン２
	static void Multiply(YsQuat &qOut, const YsQuat &qIn1, const YsQuat &qIn2){
		D3DXQuaternionMultiply(&qOut, &qIn1, &qIn2);
	}

	//! brief クォータニオンを任意軸ワールド回転
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度
	void RotateAxis(const YsVec3 &vAxis, float Ang){
		YsQuat q;
		q.CreateRotateAxis(vAxis, Ang);
		Multiply( q );
	}
	//! brief クォータニオンを任意軸ローカル回転
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度
	void RotateAxis_Local(const YsVec3 &vAxis, float Ang){
		YsQuat q;
		q.CreateRotateAxis(vAxis, Ang);
		Multiply_Local( q );
	}

	//! brief 任意軸クォータニオンを作成する
	//! @param[in]	vAxis	… 回転軸
	//! @param[in]	Ang		… 回転角度
	void CreateRotateAxis(const YsVec3 &vAxis, float Ang){
		D3DXQuaternionRotationAxis(this, &vAxis,YsToRadian(Ang));
	}

	//! brief 回転行列に変換
	//! @param[out]	out	… 変換された行列を入れる変数
	void ToMatrix(D3DXMATRIX &out) const
	{
		D3DXMatrixRotationQuaternion(&out,this);
	}
	//! brief 指定クォータニオンを回転行列に変換
	//! @param[out]	out		… 変換された行列を入れる変数
	//! @param[in]	pIn		… 処理の基になるクォータニオン
	static void ToMatrix(D3DXMATRIX &out, YsQuat &qIn)
	{
		D3DXMatrixRotationQuaternion(&out, &qIn);
	}

	//! brief 球面線形補間 q1～q2の中間位置のクォータニオンを求める
	//! @param[out]	out		… 補間結果が入る変数
	//! @param[in]	q1		… クォータニオン１
	//! @param[in]	q2		… クォータニオン２
	//! @param[in]	f		… 補間位置(0～1)
	static void Slerp(YsQuat &out, const YsQuat &q1, const YsQuat &q2, float f)
	{
		D3DXQuaternionSlerp(&out, &q1, &q2, f);
	}
	//! brief  球面二次補間 q1～q2の中間位置のクォータニオンを求める
	//! D3DXQuaternionSquad関数参照
	static void Squad(YsQuat &out, const YsQuat &qQ1, const YsQuat &q1, const YsQuat &q2, const YsQuat &q3, float f)
	{
		D3DXQuaternionSquad(&out, &qQ1, &q1, &q2, &q3, f);
	}
	// 球状平方補間の制御点を設定
	//例)
	// z 軸を中心とした回転
	//	YsQuat Q0 = YsQuat(0,  0, 0.707f, -.707f);
	//	YsQuat Q1 = YsQuat(0,  0, 0.000f, 1.000f);
	//	YsQuat Q2 = YsQuat(0,  0, 0.707f, 0.707f);
	//	YsQuat Q3 = YsQuat(0,  0, 1.000f, 0.000f);
	//	YsQuat A, B, C, Qt;
	//	FLOAT time = 0.5f;
	//
	//	D3DXQuaternionSquadSetup(&A, &B, &C, &Q0, &Q1, &Q2, &Q3);
	//	D3DXQuaternionSquad(&Qt, &Q1, &A, &B, &C, time);

	//! brief  球面二次補間の制御ポイント算出
	//! D3DXQuaternionSquadSetup関数参照
	static void SquadSetup(YsQuat &Aout, YsQuat &Bout, YsQuat &Cout, const YsQuat &qQ0, const YsQuat &qQ1, const YsQuat &qQ2, const YsQuat &qQ3){
		D3DXQuaternionSquadSetup(&Aout, &Bout, &Cout, &qQ0, &qQ1, &qQ2, &qQ3);
	}

};

}
#endif
