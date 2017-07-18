//===============================================================
//! @file YsCamera.h
//! @brief カメラクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsCamera_h__
#define __YsCamera_h__

namespace YskLib
{

//============================================================
//!
//! @brief カメラクラス
//!
//!  ビュー行列と射影行列を扱う								\n
//!  設定した内容は静的変数LastCamにもコピーされるので、	\n
//!  YsCamera::LastCamでどこからでも設定内容の取得も可能		\n
//!
//! @ingroup Math
//============================================================
class YsCamera{
public:
	YsMatrix mProj;	//!< 射影行列。

	YsMatrix mCam;	//!< カメラ行列。直接操作するやつです。
	YsMatrix mView;	//!< カメラ行列をビュー行列に変換した行列(逆行列)。
					//!< SetTransform関数時に更新されるので直接操作する必要は無し

	//! @brief 最後に使用されたカメラ情報のコピーが入る(YsCameraクラスで操作される)
	static YsCamera		LastCam;

	//! @brief カメラ行列mCamから、ビュー行列mViewを作成。LastCamにもコピーされる。
	void CameraToView();

	//! @brief ビュー行列mViewから、カメラ行列mCamを作成。LastCamにもコピーされる。
	void ViewToCamera();

	//! @brief ビュー行列を直接mViewにセットし、カメラ行列mCamも更新する
	void SetView(const YsMatrix& lpmView);

	//! @brief 射影行列をmProjに作成する。LastCamにもコピーされる。
	void SetPerspectiveFovLH(float Siyakaku, float Aspect, float zNear, float zFar);

	//! @brief 正射影行列をmProjに作成する。LastCamにもコピーされる。
	void SetOrthoLH(float w, float h, float zNear, float zFar);

	//! @brief 射影行列を直接mProjにセットする。LastCamにもコピーされる。
	void SetProj(const YsMatrix& lpmProj);

	//! @brief 現在の射影行列をLastCamへコピー
	void SetProj(){
		LastCam.mProj = mProj;
	}


	//===============================================================================================
	// その他
	//===============================================================================================

	//! @brief 3D→2D変換
	//! 3D座標を2D座標へ変換する(現在のデバイスコンテキストにセットされているビューポート情報を使用)
	//! @param[in]	devContext	… Direct3D11のデバイスコンテキスト
	//! @param[out]	lpvOut		… 結果である2D座標が入ってくる
	//! @param[in]	lpPos		… 処理の基となる3D座標
	void Convert3Dto2D(YsVec3& lpvOut, const YsVec3& lpPos);

	//! @brief 2D→3D変換
	//! 2D座標を3D座標へ変換する(現在のデバイスコンテキストにセットされているビューポート情報を使用)
	//! @param[in]	devContext	… Direct3D11のデバイスコンテキスト
	//! @param[out]	lpvOut		… 結果である3D座標が入ってくる
	//! @param[in]	lpPos		… 処理の基となる2D座標(x,yのみ使用)
	void Convert2Dto3D(YsVec3& lpvOut, const YsVec3& lpvPos);

	// 
	YsCamera();
};

}

#endif
