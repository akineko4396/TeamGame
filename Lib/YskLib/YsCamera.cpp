#include "YskLib.h"
using namespace YskLib;

YsCamera YsCamera::LastCam;			// 最後に使用されたカメラ情報のコピーが入る(YsCameraクラスにより操作)


YsCamera::YsCamera()
{
	// 射影行列
	mProj.CreatePerspectiveFovLH(
		60,				// 視野角
		1.3333f,		// 画面のアスペクト比
		1,				// 最近接距離
		1000);			// 最遠方距離

	LastCam.mProj = mProj;
}

//===============================================================================================
// カメラ行列mCamから、ビュー行列mViewを作成
//===============================================================================================
void YsCamera::CameraToView()
{

	// カメラ行列の逆行列を作成
	mCam.CreateInverse(mView);

	LastCam.mCam = mCam;
	LastCam.mView = mView;
}

//===============================================================================================
// ビュー行列mViewから、カメラ行列mCamを作成
//===============================================================================================
void YsCamera::ViewToCamera()
{

	// ビュー行列の逆行列を作成
	mView.CreateInverse(mCam);

	LastCam.mCam = mCam;
	LastCam.mView = mView;
}

// 直接指定Ver
void YsCamera::SetView(const YsMatrix &lpmView)
{
	mView = lpmView;

	// 逆にカメラ行列を作成
	mView.CreateInverse(mCam);

	LastCam.mCam = mCam;
	LastCam.mView = mView;
}

//===============================================================================================
// 射影行列設定
//===============================================================================================
void YsCamera::SetPerspectiveFovLH(float Siyakaku, float Aspect, float zNear, float zFar)
{
	mProj.CreatePerspectiveFovLH(
		Siyakaku,			// 視野角
		Aspect,				// 画面のアスペクト比
		zNear,				// 最近接距離
		zFar);				// 最遠方距離
	LastCam.mProj = mProj;
}
// 正射影
void YsCamera::SetOrthoLH(float w, float h, float zNear, float zFar)
{
	mProj.CreateOrthoLH(w, h, zNear, zFar);
	LastCam.mProj = mProj;
}
// 直接指定Ver
void YsCamera::SetProj(const YsMatrix& lpmProj)
{
	mProj = lpmProj;
	LastCam.mProj = mProj;
}

void YsCamera::Convert3Dto2D(YsVec3& lpvOut, const YsVec3& lpPos)
{
	UINT Num = 1;
	D3D11_VIEWPORT vp;
	YsDx.GetDevContext()->RSGetViewports(&Num, &vp);

	D3DVIEWPORT9 vp9;
	vp9.Width = (DWORD)vp.Width;
	vp9.Height = (DWORD)vp.Height;
	vp9.X = (DWORD)vp.TopLeftX;
	vp9.Y = (DWORD)vp.TopLeftY;
	vp9.MinZ = vp.MinDepth;
	vp9.MaxZ = vp.MaxDepth;
	YsMatrix mW;

	D3DXVec3Project(&lpvOut, &lpPos, &vp9, &mProj, &mView, &mW);
}

void YsCamera::Convert2Dto3D(YsVec3& lpvOut, const YsVec3& lpvPos)
{
	UINT Num = 1;
	D3D11_VIEWPORT vp;
	YsDx.GetDevContext()->RSGetViewports(&Num, &vp);

	D3DVIEWPORT9 vp9;
	vp9.Width = (DWORD)vp.Width;
	vp9.Height = (DWORD)vp.Height;
	vp9.X = (DWORD)vp.TopLeftX;
	vp9.Y = (DWORD)vp.TopLeftY;
	vp9.MinZ = vp.MinDepth;
	vp9.MaxZ = vp.MaxDepth;
	YsMatrix mW;
	D3DXVec3Unproject(&lpvOut, &lpvPos, &vp9, &mProj, &mView, &mW);
}
