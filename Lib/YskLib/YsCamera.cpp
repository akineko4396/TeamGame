#include "YskLib.h"
using namespace YskLib;

YsCamera YsCamera::LastCam;			// �Ō�Ɏg�p���ꂽ�J�������̃R�s�[������(YsCamera�N���X�ɂ�葀��)


YsCamera::YsCamera()
{
	// �ˉe�s��
	mProj.CreatePerspectiveFovLH(
		60,				// ����p
		1.3333f,		// ��ʂ̃A�X�y�N�g��
		1,				// �ŋߐڋ���
		1000);			// �ŉ�������

	LastCam.mProj = mProj;
}

//===============================================================================================
// �J�����s��mCam����A�r���[�s��mView���쐬
//===============================================================================================
void YsCamera::CameraToView()
{

	// �J�����s��̋t�s����쐬
	mCam.CreateInverse(mView);

	LastCam.mCam = mCam;
	LastCam.mView = mView;
}

//===============================================================================================
// �r���[�s��mView����A�J�����s��mCam���쐬
//===============================================================================================
void YsCamera::ViewToCamera()
{

	// �r���[�s��̋t�s����쐬
	mView.CreateInverse(mCam);

	LastCam.mCam = mCam;
	LastCam.mView = mView;
}

// ���ڎw��Ver
void YsCamera::SetView(const YsMatrix &lpmView)
{
	mView = lpmView;

	// �t�ɃJ�����s����쐬
	mView.CreateInverse(mCam);

	LastCam.mCam = mCam;
	LastCam.mView = mView;
}

//===============================================================================================
// �ˉe�s��ݒ�
//===============================================================================================
void YsCamera::SetPerspectiveFovLH(float Siyakaku, float Aspect, float zNear, float zFar)
{
	mProj.CreatePerspectiveFovLH(
		Siyakaku,			// ����p
		Aspect,				// ��ʂ̃A�X�y�N�g��
		zNear,				// �ŋߐڋ���
		zFar);				// �ŉ�������
	LastCam.mProj = mProj;
}
// ���ˉe
void YsCamera::SetOrthoLH(float w, float h, float zNear, float zFar)
{
	mProj.CreateOrthoLH(w, h, zNear, zFar);
	LastCam.mProj = mProj;
}
// ���ڎw��Ver
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
