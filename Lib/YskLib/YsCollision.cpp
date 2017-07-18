#include "YskLib.h"

using namespace YskLib;

// �� vs ���C
bool YsCollision::SphereToRay(const YsVec3& vQpos, float Qr, const YsVec3& rayPos, const YsVec3& rayDir, float* dist, YsVec3* outPush)
{
	bool bHit = false;	// HIT�������ǂ���������

	YsVec3 v = vQpos - rayPos;
	float l = YsVec3::Dot(v, rayDir);

	if(l < 0){	// ���ς��}�C�i�X���ƁA�G��Ă��Ȃ��\���������̂ŕʔ���
		// ���ƃ��C���W(�_)�Ƃ̔���
		l = v.Length();
		if(l <= Qr){	// HIT�m��
			bHit = true;
			if(dist)*dist = 0;

			// �_�̏ꍇ�́A�_�Ƌ������ԕ����ɉ����߂����߂̃x�N�g����Ԃ�
			if(outPush){
				*outPush = v;
				((YsVec3*)outPush)->Normalize();
				*outPush *= Qr - l;
			}
		}
	}
	// ���ς��v���X�̎��́A���C�̐���ɂ���
	else{
		YsVec3 pos = rayPos + rayDir * l;
		YsVec3 vQ = vQpos - pos;
		float L = vQ.Length();
		if(L  < Qr){
			bHit = true;
			if(dist)*dist = l - sqrtf(Qr*Qr - L*L);

			// ����̏ꍇ�́A�����̕����ɉ����߂����߂̃x�N�g����Ԃ�
			if(outPush){
				*outPush = vQ;
				((YsVec3*)outPush)->Normalize();
				*outPush *= Qr - L;
			}
		}
	}

	return bHit;
}

bool YsCollision::RayToMesh(const YsMesh& mesh, const YsMatrix* lpMat, const YsVec3& rayPosStart, const YsVec3& rayPosEnd, float* outDist, UINT* outHitFaceIndex)
{
	if (mesh.GetExtFace().size() == 0)return false;

	// ���C����Ώۂ̍��W�n�֕ϊ�
	YsVec3 vRayPos1, vRayPos2;
	YsVec3 vDir;
	float rayLen = 0;
	if (lpMat) {
		YsMatrix invMat;
		lpMat->CreateInverse(invMat);
		// ���C�����w�肵���s��̋t�s��ŕϊ����A���[�J�����W�n�֕ϊ�����B
		YsVec3::Transform(vRayPos1, rayPosStart, invMat);
		YsVec3::Transform(vRayPos2, rayPosEnd, invMat);

		vDir = rayPosEnd - rayPosStart;
		rayLen = vDir.Length();
		vDir /= rayLen;
//		vDir.Normalize();
		YsVec3::TransformNormal(vDir, vDir, invMat);

	}
	else {
		vRayPos1 = rayPosStart;
		vRayPos2 = rayPosEnd;

		vDir = vRayPos2 - vRayPos1;
		rayLen = vDir.Length();
		vDir /= rayLen;
	}

	//---------------------------------------------
	// �������ׁ̈AAABB�ŊȈՔ�����s��
	//---------------------------------------------
	// ���C��AABB
	YsVec3 vMin, vMax;
	YsVec3::CreateAABBFromLineSegment(vRayPos1, vRayPos2, vMin, vMax);
	if (BoxToBox3D(vMin, vMax, mesh.GetAABBMin(), mesh.GetAABBMax()) == false)return false;


	//---------------------------------------------
	// ����
	//---------------------------------------------

	// �ԐM�p
	float fDist = FLT_MAX;
	UINT hitFaceIdx;

	// ��Ɨp
	float d = 0;
	bool bHit;
	bool RetBl = false;

	const UINT FaceTblNum = (UINT)mesh.GetFace().size();
	const std::vector<YsMesh::ExtFace>& extFaces = mesh.GetExtFace();

	for (UINT i = 0; i<(UINT)FaceTblNum; i++) {
		//---------------------------------------------
		// AABB����ɂ��A���炩�ɓ������Ă��Ȃ����̂͏��O
		//---------------------------------------------
		if (BoxToBox3D(vMin, vMax, extFaces[i].vMin, extFaces[i].vMax) == false)continue;

		//---------------------------------------------
		// 0,1,2�̖ʂƃ��C����
		//---------------------------------------------
		bHit = D3DXIntersectTri(
			&mesh.GetVertex_Pos(i, 0),
			&mesh.GetVertex_Pos(i, 1),
			&mesh.GetVertex_Pos(i, 2),
			&vRayPos1,
			&vDir,
			nullptr,
			nullptr,
			&d) == TRUE;
		if (bHit) {

			if (d < fDist) {
				fDist = d;
				hitFaceIdx = i;
			}

			RetBl = true;
		}
	}

	if(RetBl){
		// ���C�͈̔͊O�Ȃ瓖�����ĂȂ�
		if(fDist > rayLen){
			return false;
		}

		// �������Ă�Ƃ��́A�����Ɩʔԍ�������
		if (outDist)*outDist = fDist;
		if (outHitFaceIndex)*outHitFaceIndex = hitFaceIdx;

		return RetBl;
	}

	return false;
}

bool YsCollision::MeshToSphere(const YsMesh& mesh, const YsMatrix* lpMat, const YsVec3& vQPos, float Qr, YsVec3* outMove, int startFaceNo, int faceNum, std::vector<MTS_HitData>* outHitDataTbl)
{
	// �ʂ��Ȃ��ꍇ�́A�Ȃɂ����Ȃ�
	if(mesh.GetExtFace().size() == 0)return false;


	// �s��w�肪����ꍇ�́A���̍��W�����[�J�����W�n�֕ϊ�
	YsVec3 QPos;
	YsMatrix tranMat;
	YsMatrix invTranMat;
	if(lpMat){
		tranMat = *lpMat;
		lpMat->CreateInverse(invTranMat);
	}

	// �Ƃ肠������ԑ傫�Ȏ���AABB�����
	YsVec3 vScale = invTranMat.GetScale();
	float maxScale = vScale.x;
	if (maxScale < vScale.y)maxScale = vScale.y;
	if (maxScale < vScale.z)maxScale = vScale.z;
	YsVec3 vP;
	vQPos.Transform(vP, invTranMat);
	YsVec3 vQ_Min(vP.x - Qr*maxScale, vP.y - Qr*maxScale, vP.z - Qr*maxScale);
	YsVec3 vQ_Max(vP.x + Qr*maxScale, vP.y + Qr*maxScale, vP.z + Qr*maxScale);

	//---------------------------------------------
	// �������ׁ̈AAABB�ŊȈՔ�����s��
	//---------------------------------------------
	// ���b�V���Ƌ���AABB����
	if(BoxToBox3D(vQ_Min, vQ_Max, mesh.GetAABBMin(), mesh.GetAABBMax()) == false)return false;

	//---------------------------------------------
	// ��������ڍה���
	//---------------------------------------------
	// ���̍��W
	QPos = vQPos;

	// �߂�l�p
	bool bRet = false;

	// �o�̓x�N�g��
	YsVec3 vOut;

	// ���_�ϊ��p�f�[�^
	YsVec3 vertexPos[3];
	YsVec3 vertexN;
	YsVec3 vMin, vMax;

	// ��Ɨp�ϐ�
	int iFace;
	YsVec3 ret;
	YsVec3 vTmp, v1, v2, vQ, vPos;
	float dot, dot2;


	// �O�p�`�̕ӂƓ_ vs ���Ƃ̔���֐�
	auto FaceLineToSphere = [&]()
	{
		float f;
		DWORD j;
		{
			// �ӂ�
			float t;
			v1 = vertexPos[1] - vertexPos[0];
			vQ = QPos - vertexPos[0];
			t = YsVec3::Dot(vQ, v1) / YsVec3::Dot(v1, v1);
			vPos = vertexPos[0] + v1*t;
			v1 = QPos - vPos;
			if (t >= 0 && t <= 1) {
				if (YsVec3::Dot(v1, v1) <= Qr*Qr) {
					f = Qr - v1.Length();
					v1.Normalize();
					vOut += v1 * f;
					QPos += v1 * f;	// ���̍��W���ύX
					if(outHitDataTbl)(*outHitDataTbl).emplace_back(iFace, vPos);
					bRet = true;
				}
			}

			v1 = vertexPos[2] - vertexPos[1];
			vQ = QPos - vertexPos[1];
			t = YsVec3::Dot(vQ, v1) / YsVec3::Dot(v1, v1);
			vPos = vertexPos[1] + v1*t;
			v1 = QPos - vPos;
			if (t >= 0 && t <= 1) {
				if (YsVec3::Dot(v1, v1) <= Qr*Qr) {
					f = Qr - v1.Length();
					v1.Normalize();
					vOut += v1 * f;
					QPos += v1 * f;	// ���̍��W���ύX
					if(outHitDataTbl)(*outHitDataTbl).emplace_back(iFace, vPos);
					bRet = true;
				}
			}

			v1 = vertexPos[0] - vertexPos[2];
			vQ = QPos - vertexPos[2];
			t = YsVec3::Dot(vQ, v1) / YsVec3::Dot(v1, v1);
			vPos = vertexPos[2] + v1*t;
			v1 = QPos - vPos;
			if (t >= 0 && t <= 1) {
				if (YsVec3::Dot(v1, v1) <= Qr*Qr) {
					f = Qr - v1.Length();
					v1.Normalize();
					vOut += v1 * f;
					QPos += v1 * f;	// ���̍��W���ύX
					if(outHitDataTbl)(*outHitDataTbl).emplace_back(iFace, vPos);
					bRet = true;
				}
			}

			// �_�Ƃ̔���
			for (j = 0; j < 3; j++) {
				v1 = QPos - vertexPos[j];
				if (YsVec3::Dot(v1, v1) <= Qr*Qr) {
					f = Qr - v1.Length();
					v1.Normalize();
					vOut += v1 * f;
					QPos += v1 * f;	// ���̍��W���ύX
					if(outHitDataTbl)(*outHitDataTbl).emplace_back(iFace, vertexPos[j]);
					bRet = true;
				}
			}
		}
	};


	const std::vector<YsMesh::Face>& faces = mesh.GetFace();
	const std::vector<YsMesh::ExtFace>& extFaces = mesh.GetExtFace();

	if (faceNum < 0) {
		faceNum = faces.size();
	}

	for(iFace = startFaceNo; iFace<faceNum; iFace++){

		// ���_�����s��ŕϊ�
		mesh.GetVertex_Pos(iFace, 0).Transform(vertexPos[0], tranMat);
		mesh.GetVertex_Pos(iFace, 1).Transform(vertexPos[1], tranMat);
		mesh.GetVertex_Pos(iFace, 2).Transform(vertexPos[2], tranMat);
		extFaces[iFace].vN.TransformNormal(vertexN, tranMat);
		vertexN.Normalize();


		// �@�������ʂƋ��Ƃ̔���
		v1 = QPos - vertexPos[0];
		dot2 = YsVec3::Dot(v1, vertexN);
		if(dot2 > -Qr && dot2 < Qr){ // ���a���߂��Ȃ�A�Փ˂��Ă���
//		if(dot2 >= 0 && dot2 < Qr){ // ���a���߂��Ȃ�A�Փ˂��Ă���
			// �A�O�p�`�Ɠ_�Ƃ̓��O����
			v1 = vertexPos[1]-vertexPos[0];
			v2 = QPos - vertexPos[0];
			YsVec3::Cross(ret, v1, v2);
			dot = YsVec3::Dot(ret, vertexN);
			if(dot < 0.0f){	// �������ĂȂ�
				// �BAABB����œ������Ă�Ȃ�A��������s��
				if( (extFaces[iFace].vMax.x >= vQ_Min.x) && (extFaces[iFace].vMin.x <= vQ_Max.x)
				&&	(extFaces[iFace].vMax.y >= vQ_Min.y) && (extFaces[iFace].vMin.y <= vQ_Max.y)
				&&	(extFaces[iFace].vMax.z >= vQ_Min.z) && (extFaces[iFace].vMin.z <= vQ_Max.z) ){
					// �C�O�p�`�̕ӂƋ��Ƃ̔���
					FaceLineToSphere();
				}

				continue;// �O������
			}

			v1 = vertexPos[2]-vertexPos[1];
			v2 = QPos - vertexPos[1];
			YsVec3::Cross(ret, v1, v2);
			dot = YsVec3::Dot(ret, vertexN);
			if(dot < 0.0f){	// �������ĂȂ�
				// �BAABB����œ������Ă�Ȃ�A��������s��
				if( (extFaces[iFace].vMax.x >= vQ_Min.x) && (extFaces[iFace].vMin.x <= vQ_Max.x)
				&&	(extFaces[iFace].vMax.y >= vQ_Min.y) && (extFaces[iFace].vMin.y <= vQ_Max.y)
				&&	(extFaces[iFace].vMax.z >= vQ_Min.z) && (extFaces[iFace].vMin.z <= vQ_Max.z) ){
					// �C�O�p�`�̕ӂƋ��Ƃ̔���
					FaceLineToSphere();
				}
				continue;// �O������
			}

			v1 = vertexPos[0]-vertexPos[2];
			v2 = QPos-vertexPos[2];
			YsVec3::Cross(ret, v1, v2);
			dot = YsVec3::Dot(ret, vertexN);
			if(dot < 0.0f){	// �������ĂȂ�
				// �BAABB����œ������Ă�Ȃ�A��������s��
				if( (extFaces[iFace].vMax.x >= vQ_Min.x) && (extFaces[iFace].vMin.x <= vQ_Max.x)
				&&	(extFaces[iFace].vMax.y >= vQ_Min.y) && (extFaces[iFace].vMin.y <= vQ_Max.y)
				&&	(extFaces[iFace].vMax.z >= vQ_Min.z) && (extFaces[iFace].vMin.z <= vQ_Max.z) ){
					// �C�O�p�`�̕ӂƋ��Ƃ̔���
					FaceLineToSphere();
				}
				continue;// �O������
			}

			// �\�����̎��̂݁A�����o�����ɉ�����
			if(dot2 >= 0){
				vTmp.Set(vertexN * (Qr - dot2));
				vOut += vTmp;
				QPos += vTmp;	// ���̍��W���ύX

			}
			if(outHitDataTbl)(*outHitDataTbl).emplace_back(iFace, QPos - vertexN*dot2);
			bRet = true;
		}
	}
	
	if (outMove) {
		*outMove = vOut;
	}

	return bRet;
}

bool YsCollision::MeshToSphere(const YsMesh& mesh, int mateNo, const YsMatrix* lpMat, const YsVec3& vQPos, float Qr, YsVec3* outMove)
{
	if (mesh.GetExtFace().size() == 0)return false;

	const YsMeshSubset* subset = mesh.GetSubset(mateNo);
	if(subset->FaceCount == 0)return false;

	return MeshToSphere(mesh, lpMat, vQPos, Qr, outMove, subset->FaceStart, subset->FaceCount);
}

