#include "YskLib.h"

using namespace YskLib;

// 球 vs レイ
bool YsCollision::SphereToRay(const YsVec3& vQpos, float Qr, const YsVec3& rayPos, const YsVec3& rayDir, float* dist, YsVec3* outPush)
{
	bool bHit = false;	// HITしたかどうかを入れる

	YsVec3 v = vQpos - rayPos;
	float l = YsVec3::Dot(v, rayDir);

	if(l < 0){	// 内積がマイナスだと、触れていない可能性が高いので別判定
		// 球とレイ座標(点)との判定
		l = v.Length();
		if(l <= Qr){	// HIT確定
			bHit = true;
			if(dist)*dist = 0;

			// 点の場合は、点と球を結ぶ方向に押し戻すためのベクトルを返す
			if(outPush){
				*outPush = v;
				((YsVec3*)outPush)->Normalize();
				*outPush *= Qr - l;
			}
		}
	}
	// 内積がプラスの時は、レイの線上にいる
	else{
		YsVec3 pos = rayPos + rayDir * l;
		YsVec3 vQ = vQpos - pos;
		float L = vQ.Length();
		if(L  < Qr){
			bHit = true;
			if(dist)*dist = l - sqrtf(Qr*Qr - L*L);

			// 線上の場合は、垂直の方向に押し戻すためのベクトルを返す
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

	// レイ情報を対象の座標系へ変換
	YsVec3 vRayPos1, vRayPos2;
	YsVec3 vDir;
	float rayLen = 0;
	if (lpMat) {
		YsMatrix invMat;
		lpMat->CreateInverse(invMat);
		// レイ情報を指定した行列の逆行列で変換し、ローカル座標系へ変換する。
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
	// 高速化の為、AABBで簡易判定を行う
	//---------------------------------------------
	// レイのAABB
	YsVec3 vMin, vMax;
	YsVec3::CreateAABBFromLineSegment(vRayPos1, vRayPos2, vMin, vMax);
	if (BoxToBox3D(vMin, vMax, mesh.GetAABBMin(), mesh.GetAABBMax()) == false)return false;


	//---------------------------------------------
	// 判定
	//---------------------------------------------

	// 返信用
	float fDist = FLT_MAX;
	UINT hitFaceIdx;

	// 作業用
	float d = 0;
	bool bHit;
	bool RetBl = false;

	const UINT FaceTblNum = (UINT)mesh.GetFace().size();
	const std::vector<YsMesh::ExtFace>& extFaces = mesh.GetExtFace();

	for (UINT i = 0; i<(UINT)FaceTblNum; i++) {
		//---------------------------------------------
		// AABB判定により、明らかに当たっていないものは除外
		//---------------------------------------------
		if (BoxToBox3D(vMin, vMax, extFaces[i].vMin, extFaces[i].vMax) == false)continue;

		//---------------------------------------------
		// 0,1,2の面とレイ判定
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
		// レイの範囲外なら当たってない
		if(fDist > rayLen){
			return false;
		}

		// 当たってるときは、距離と面番号を入れる
		if (outDist)*outDist = fDist;
		if (outHitFaceIndex)*outHitFaceIndex = hitFaceIdx;

		return RetBl;
	}

	return false;
}

bool YsCollision::MeshToSphere(const YsMesh& mesh, const YsMatrix* lpMat, const YsVec3& vQPos, float Qr, YsVec3* outMove, int startFaceNo, int faceNum, std::vector<MTS_HitData>* outHitDataTbl)
{
	// 面がない場合は、なにもしない
	if(mesh.GetExtFace().size() == 0)return false;


	// 行列指定がある場合は、球の座標をローカル座標系へ変換
	YsVec3 QPos;
	YsMatrix tranMat;
	YsMatrix invTranMat;
	if(lpMat){
		tranMat = *lpMat;
		lpMat->CreateInverse(invTranMat);
	}

	// とりあえず一番大きな軸のAABBを作る
	YsVec3 vScale = invTranMat.GetScale();
	float maxScale = vScale.x;
	if (maxScale < vScale.y)maxScale = vScale.y;
	if (maxScale < vScale.z)maxScale = vScale.z;
	YsVec3 vP;
	vQPos.Transform(vP, invTranMat);
	YsVec3 vQ_Min(vP.x - Qr*maxScale, vP.y - Qr*maxScale, vP.z - Qr*maxScale);
	YsVec3 vQ_Max(vP.x + Qr*maxScale, vP.y + Qr*maxScale, vP.z + Qr*maxScale);

	//---------------------------------------------
	// 高速化の為、AABBで簡易判定を行う
	//---------------------------------------------
	// メッシュと球のAABB判定
	if(BoxToBox3D(vQ_Min, vQ_Max, mesh.GetAABBMin(), mesh.GetAABBMax()) == false)return false;

	//---------------------------------------------
	// ここから詳細判定
	//---------------------------------------------
	// 球の座標
	QPos = vQPos;

	// 戻り値用
	bool bRet = false;

	// 出力ベクトル
	YsVec3 vOut;

	// 頂点変換用データ
	YsVec3 vertexPos[3];
	YsVec3 vertexN;
	YsVec3 vMin, vMax;

	// 作業用変数
	int iFace;
	YsVec3 ret;
	YsVec3 vTmp, v1, v2, vQ, vPos;
	float dot, dot2;


	// 三角形の辺と点 vs 球との判定関数
	auto FaceLineToSphere = [&]()
	{
		float f;
		DWORD j;
		{
			// 辺と
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
					QPos += v1 * f;	// 球の座標も変更
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
					QPos += v1 * f;	// 球の座標も変更
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
					QPos += v1 * f;	// 球の座標も変更
					if(outHitDataTbl)(*outHitDataTbl).emplace_back(iFace, vPos);
					bRet = true;
				}
			}

			// 点との判定
			for (j = 0; j < 3; j++) {
				v1 = QPos - vertexPos[j];
				if (YsVec3::Dot(v1, v1) <= Qr*Qr) {
					f = Qr - v1.Length();
					v1.Normalize();
					vOut += v1 * f;
					QPos += v1 * f;	// 球の座標も変更
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

		// 頂点情報を行列で変換
		mesh.GetVertex_Pos(iFace, 0).Transform(vertexPos[0], tranMat);
		mesh.GetVertex_Pos(iFace, 1).Transform(vertexPos[1], tranMat);
		mesh.GetVertex_Pos(iFace, 2).Transform(vertexPos[2], tranMat);
		extFaces[iFace].vN.TransformNormal(vertexN, tranMat);
		vertexN.Normalize();


		// ①無限平面と球との判定
		v1 = QPos - vertexPos[0];
		dot2 = YsVec3::Dot(v1, vertexN);
		if(dot2 > -Qr && dot2 < Qr){ // 半径より近いなら、衝突している
//		if(dot2 >= 0 && dot2 < Qr){ // 半径より近いなら、衝突している
			// ②三角形と点との内外判定
			v1 = vertexPos[1]-vertexPos[0];
			v2 = QPos - vertexPos[0];
			YsVec3::Cross(ret, v1, v2);
			dot = YsVec3::Dot(ret, vertexN);
			if(dot < 0.0f){	// 当たってない
				// ③AABB判定で当たってるなら、線分判定行き
				if( (extFaces[iFace].vMax.x >= vQ_Min.x) && (extFaces[iFace].vMin.x <= vQ_Max.x)
				&&	(extFaces[iFace].vMax.y >= vQ_Min.y) && (extFaces[iFace].vMin.y <= vQ_Max.y)
				&&	(extFaces[iFace].vMax.z >= vQ_Min.z) && (extFaces[iFace].vMin.z <= vQ_Max.z) ){
					// ④三角形の辺と球との判定
					FaceLineToSphere();
				}

				continue;// 外側決定
			}

			v1 = vertexPos[2]-vertexPos[1];
			v2 = QPos - vertexPos[1];
			YsVec3::Cross(ret, v1, v2);
			dot = YsVec3::Dot(ret, vertexN);
			if(dot < 0.0f){	// 当たってない
				// ③AABB判定で当たってるなら、線分判定行き
				if( (extFaces[iFace].vMax.x >= vQ_Min.x) && (extFaces[iFace].vMin.x <= vQ_Max.x)
				&&	(extFaces[iFace].vMax.y >= vQ_Min.y) && (extFaces[iFace].vMin.y <= vQ_Max.y)
				&&	(extFaces[iFace].vMax.z >= vQ_Min.z) && (extFaces[iFace].vMin.z <= vQ_Max.z) ){
					// ④三角形の辺と球との判定
					FaceLineToSphere();
				}
				continue;// 外側決定
			}

			v1 = vertexPos[0]-vertexPos[2];
			v2 = QPos-vertexPos[2];
			YsVec3::Cross(ret, v1, v2);
			dot = YsVec3::Dot(ret, vertexN);
			if(dot < 0.0f){	// 当たってない
				// ③AABB判定で当たってるなら、線分判定行き
				if( (extFaces[iFace].vMax.x >= vQ_Min.x) && (extFaces[iFace].vMin.x <= vQ_Max.x)
				&&	(extFaces[iFace].vMax.y >= vQ_Min.y) && (extFaces[iFace].vMin.y <= vQ_Max.y)
				&&	(extFaces[iFace].vMax.z >= vQ_Min.z) && (extFaces[iFace].vMin.z <= vQ_Max.z) ){
					// ④三角形の辺と球との判定
					FaceLineToSphere();
				}
				continue;// 外側決定
			}

			// 表方向の時のみ、押し出す情報に加える
			if(dot2 >= 0){
				vTmp.Set(vertexN * (Qr - dot2));
				vOut += vTmp;
				QPos += vTmp;	// 球の座標も変更

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

