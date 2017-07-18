//===============================================================
//! @file YsCollision.h
//! @brief �����蔻��N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsCollision_h__
#define __YsCollision_h__

namespace YskLib{

class YsMesh;

//================================================================
//! @brief �����蔻��N���X
//!
//! �֐��͂��ׂ�static�֐��ł�
//!
//! @ingroup Collision
//================================================================
class YsCollision{
public:
	//=======================================================================
	//! @brief �_�Ɨ�����(��]�Ȃ�)
	//! @param[in] vMin		�c BOX1�̍���̍��W
	//! @param[in] vMax		�c BOX1�̉E��̍��W
	//! @param[in] pnt		�c �_�̍��W
	//! @return true���ƃq�b�g����
	//=======================================================================
	static bool PointToBox(const YsVec3& vMin, const YsVec3& vMax, const YsVec3& pnt)
	{
		if (pnt.x < vMin.x)return false;
		if (pnt.y < vMin.y)return false;
		if (pnt.z < vMin.z)return false;

		if (pnt.x > vMax.x)return false;
		if (pnt.y > vMax.y)return false;
		if (pnt.z > vMax.z)return false;

		return true;
	}

	//=======================================================================
	//! @brief 2D�l�p�`�̓��m����(��]�Ȃ�)
	//! @param[in] min1		�c BOX1�̍���̍��W
	//! @param[in] max1		�c BOX1�̉E��̍��W
	//! @param[in] min2		�c BOX2�̍���̍��W
	//! @param[in] max2		�c BOX2�̉E��̍��W
	//! @return true���ƃq�b�g����
	//=======================================================================
	static bool BoxToBox2D(const YsVec2 &min1, const YsVec2 &max1, const YsVec2 &min2, const YsVec2 &max2)
	{
		if( (max1.x >= min2.x) && (min1.x <= max2.x) &&
			(max1.y >= min2.y) && (min1.y <= max2.y))
		{
			return true;
		}
		return false;
	}
	//=======================================================================
	//! @brief 3D�l�p�`�̓��m����(��]�Ȃ�)
	//! @param[in] min1		�c BOX1�̍ŏ����W
	//! @param[in] max1		�c BOX1�̍ő���W
	//! @param[in] min2		�c BOX2�̍ŏ����W
	//! @param[in] max2		�c BOX2�̍ő���W
	//! @return true���ƃq�b�g����
	//=======================================================================
	static bool BoxToBox3D(const YsVec3 &min1, const YsVec3 &max1, const YsVec3 &min2, const YsVec3 &max2)
	{
		if( (max1.x >= min2.x) && (min1.x <= max2.x) &&
			(max1.y >= min2.y) && (min1.y <= max2.y) &&
			(max1.z >= min2.z) && (min1.z <= max2.z) )
		{
			return true;
		}
		return false;
	}

	//=======================================================================
	//! @brief �� vs ��
	//! @param[in]	vQpos1	�c ���P�̍��W
	//! @param[in]	r1		�c ���P�̔��a
	//! @param[in]	vQpos2	�c ���Q�̍��W
	//! @param[in]	r2		�c ���Q�̔��a
	//! @param[out] outMove	�c ���Q���狅�P�ւ̃x�N�g��������
	//! @return �߂荞��ł��鋗���B�܂�A�{�l���Ƃg�h�s�A�|�l���Ɠ������Ă��Ȃ��B
	//=======================================================================
	static float SphereToSphere(const YsVec3& vQpos1, float r1, const YsVec3& vQpos2, float r2, YsVec3& outMove)
	{
		outMove = vQpos1 - vQpos2;
		float len = YsVec3::Length(outMove);

		return r1 + r2 - len;
	}
	//=======================================================================
	//! @brief �� vs ��
	//! @param[in] vQpos1	�c ���P�̍��W
	//! @param[in] r1		�c ���P�̔��a
	//! @param[in] vQpos2	�c ���Q�̍��W
	//! @param[in] r2		�c ���Q�̔��a
	//! @return true���ƃq�b�g����
	//=======================================================================
	static bool SphereToSphere(const YsVec3& vQpos1, float r1, const YsVec3& vQpos2, float r2)
	{
		YsVec3 vOut = vQpos1 - vQpos2;
		float lenPow2 = YsVec3::Dot(vOut, vOut);

		return lenPow2 < (r1 + r2);
	}

	//=======================================================================
	//! @brief �� vs ���C
	//! @param[in]	vQpos		�c ���̍��W
	//! @param[in]	Qr			�c ���̔��a
	//! @param[in]	rayPos		�c ���C�̍��p
	//! @param[in]	rayDir		�c ���C�̕���(���K�����Ă�������)
	//! @param[out]	dist		�c ���C���q�b�g��������������
	//! @param[out] outPush		�c ���������߂������ꍇ�Ɏg�p�ł���x�N�g��������
	//! @return true���ƃq�b�g
	//=======================================================================
	static bool SphereToRay(const YsVec3& vQpos, float Qr, const YsVec3& rayPos, const YsVec3& rayDir, float* dist, YsVec3* outPush);

	//=======================================================================
	//! @brief ���C vs �O�p�`
	//!  ������D3DXIntersectTri()�Q��
	//=======================================================================
	static BOOL RayToTriangle(const YsVec3& p1, const YsVec3& p2, const YsVec3& p3, const YsVec3& rayPos, const YsVec3& rayDir, float* pU, float* pV, float* dist)
	{
		return D3DXIntersectTri(&p1,&p2,&p3,&rayPos,&rayDir,pU,pV,dist);
	}


	//=======================================================================
	//! @brief ���b�V���ƃ��C�Ƃ̔���(���C�̎w��͕����ł͂Ȃ����W�P�ƍ��W�Q�ł̎w��Ȃ̂Œ���)
	//!
	//! �������̂���meshData�̓����ɂ���AABB�f�[�^���g���ĊȈՔ�����s���A���炩��HIT���Ȃ����̂͏��O����			\n
	//! ����ɂ�背�C�̕������w��ł͂Ȃ��A���C�̊J�n���W�`�I�����W(�����̂��郌�C)���w�肷��`�ɂȂ��Ă��܂��B	\n
	//! �Ώۃ��b�V���̕ϊ��s����w��ł���̂ŁA�����Ă��镨�̂Ƃ�����\�B
	//!
	//! @param[in]	mesh			�c ���肷�郁�b�V��
	//! @param[in]	lpMat			�c ���b�V���̕ϊ��s��@nullptr���Ǝw��Ȃ�
	//! @param[in]	rayPosStart		�c ���C�̊J�n���W
	//! @param[in]	rayPosEnd		�c ���C�̏I�����W
	//! @param[out]	outDist			�c ���C���q�b�g��������������
	//! @param[out]	outHitFaceIndex	�c ���C���q�b�g�����ʂ̔ԍ�������
	//! @return true���ƃq�b�g
	//=======================================================================
	static bool RayToMesh(const YsMesh& mesh, const YsMatrix* lpMat, const YsVec3& rayPosStart, const YsVec3& rayPosEnd, float* outDist, UINT* outHitFaceIndex);


	// 
	struct MTS_HitData{
		int		faceNo;
		YsVec3	vHitPos;

		MTS_HitData(int no, const YsVec3& hitPos) : faceNo(no), vHitPos(hitPos){}
	};

	//=======================================================================
	//! @brief ���ƃ��b�V��(YsMesh�̒��ɂ��钸�_��ʏ����g�p)
	//! @param[in]	meshData	�c ���肷�郁�b�V��
	//! @param[in]	lpMat		�c �ϊ��s�� nullptr���Ǝw��Ȃ�
	//! @param[in]	lpQPos		�c ���̈ʒu
	//! @param[in]	Qr			�c ���̔��a
	//! @param[out] outMove		�c �߂荞�񂾕��̃x�N�g�����Ԃ�(�ʁ����̕���)
	//! @param[in] startFaceNo	�c ���肷��ʂ̊J�n�ԍ��@0�ōŏ�����
	//! @param[in] faceNum		�c ���肷��ʂ̐��@-1�őS�Ă̖ʐ�
	//! @param[out] outHitDataTbl �c �q�b�g�������W���Ԃ�
	//! @return true���ƃq�b�g
	//=======================================================================
	static bool MeshToSphere(const YsMesh& mesh, const YsMatrix* lpMat, const YsVec3& vQPos, float Qr, YsVec3* outMove, int startFaceNo = 0, int faceNum = -1, std::vector<MTS_HitData>* outHitDataTbl = nullptr);

	//=======================================================================
	//! @brief ���ƃ��b�V��(YsMesh�̒��ɂ��钸�_��ʏ����g�p �w��}�e���A���̖ʂ̂�)
	//! meshData�̎w�肵���}�e���A��No�̖ʂƂ�������
	//! @param[in]	mateNo	�c ���肵�����}�e���A���ԍ�
	//=======================================================================
	static bool MeshToSphere(const YsMesh& mesh, int mateNo, const YsMatrix* lpMat, const YsVec3& vQPos, float Qr, YsVec3* outMove);
};

}
#endif