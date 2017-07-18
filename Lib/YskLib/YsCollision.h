//===============================================================
//! @file YsCollision.h
//! @brief 当たり判定クラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsCollision_h__
#define __YsCollision_h__

namespace YskLib{

class YsMesh;

//================================================================
//! @brief 当たり判定クラス
//!
//! 関数はすべてstatic関数です
//!
//! @ingroup Collision
//================================================================
class YsCollision{
public:
	//=======================================================================
	//! @brief 点と立方体(回転なし)
	//! @param[in] vMin		… BOX1の左上の座標
	//! @param[in] vMax		… BOX1の右上の座標
	//! @param[in] pnt		… 点の座標
	//! @return trueだとヒットした
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
	//! @brief 2D四角形の同士判定(回転なし)
	//! @param[in] min1		… BOX1の左上の座標
	//! @param[in] max1		… BOX1の右上の座標
	//! @param[in] min2		… BOX2の左上の座標
	//! @param[in] max2		… BOX2の右上の座標
	//! @return trueだとヒットした
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
	//! @brief 3D四角形の同士判定(回転なし)
	//! @param[in] min1		… BOX1の最少座標
	//! @param[in] max1		… BOX1の最大座標
	//! @param[in] min2		… BOX2の最少座標
	//! @param[in] max2		… BOX2の最大座標
	//! @return trueだとヒットした
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
	//! @brief 球 vs 球
	//! @param[in]	vQpos1	… 球１の座標
	//! @param[in]	r1		… 球１の半径
	//! @param[in]	vQpos2	… 球２の座標
	//! @param[in]	r2		… 球２の半径
	//! @param[out] outMove	… 球２から球１へのベクトルが入る
	//! @return めり込んでいる距離。つまり、＋値だとＨＩＴ、－値だと当たっていない。
	//=======================================================================
	static float SphereToSphere(const YsVec3& vQpos1, float r1, const YsVec3& vQpos2, float r2, YsVec3& outMove)
	{
		outMove = vQpos1 - vQpos2;
		float len = YsVec3::Length(outMove);

		return r1 + r2 - len;
	}
	//=======================================================================
	//! @brief 球 vs 球
	//! @param[in] vQpos1	… 球１の座標
	//! @param[in] r1		… 球１の半径
	//! @param[in] vQpos2	… 球２の座標
	//! @param[in] r2		… 球２の半径
	//! @return trueだとヒットした
	//=======================================================================
	static bool SphereToSphere(const YsVec3& vQpos1, float r1, const YsVec3& vQpos2, float r2)
	{
		YsVec3 vOut = vQpos1 - vQpos2;
		float lenPow2 = YsVec3::Dot(vOut, vOut);

		return lenPow2 < (r1 + r2);
	}

	//=======================================================================
	//! @brief 球 vs レイ
	//! @param[in]	vQpos		… 球の座標
	//! @param[in]	Qr			… 球の半径
	//! @param[in]	rayPos		… レイの座用
	//! @param[in]	rayDir		… レイの方向(正規化しておくこと)
	//! @param[out]	dist		… レイがヒットした距離が入る
	//! @param[out] outPush		… 球を押し戻したい場合に使用できるベクトルが入る
	//! @return trueだとヒット
	//=======================================================================
	static bool SphereToRay(const YsVec3& vQpos, float Qr, const YsVec3& rayPos, const YsVec3& rayDir, float* dist, YsVec3* outPush);

	//=======================================================================
	//! @brief レイ vs 三角形
	//!  引数はD3DXIntersectTri()参照
	//=======================================================================
	static BOOL RayToTriangle(const YsVec3& p1, const YsVec3& p2, const YsVec3& p3, const YsVec3& rayPos, const YsVec3& rayDir, float* pU, float* pV, float* dist)
	{
		return D3DXIntersectTri(&p1,&p2,&p3,&rayPos,&rayDir,pU,pV,dist);
	}


	//=======================================================================
	//! @brief メッシュとレイとの判定(レイの指定は方向ではなく座標１と座標２での指定なので注意)
	//!
	//! 高速化のためmeshDataの内部にあるAABBデータを使って簡易判定を行い、明らかにHITしないものは除外する			\n
	//! それによりレイの方向を指定ではなく、レイの開始座標～終了座標(長さのあるレイ)を指定する形になっています。	\n
	//! 対象メッシュの変換行列も指定できるので、動いている物体とも判定可能。
	//!
	//! @param[in]	mesh			… 判定するメッシュ
	//! @param[in]	lpMat			… メッシュの変換行列　nullptrだと指定なし
	//! @param[in]	rayPosStart		… レイの開始座標
	//! @param[in]	rayPosEnd		… レイの終了座標
	//! @param[out]	outDist			… レイがヒットした距離が入る
	//! @param[out]	outHitFaceIndex	… レイがヒットした面の番号が入る
	//! @return trueだとヒット
	//=======================================================================
	static bool RayToMesh(const YsMesh& mesh, const YsMatrix* lpMat, const YsVec3& rayPosStart, const YsVec3& rayPosEnd, float* outDist, UINT* outHitFaceIndex);


	// 
	struct MTS_HitData{
		int		faceNo;
		YsVec3	vHitPos;

		MTS_HitData(int no, const YsVec3& hitPos) : faceNo(no), vHitPos(hitPos){}
	};

	//=======================================================================
	//! @brief 球とメッシュ(YsMeshの中にある頂点や面情報を使用)
	//! @param[in]	meshData	… 判定するメッシュ
	//! @param[in]	lpMat		… 変換行列 nullptrだと指定なし
	//! @param[in]	lpQPos		… 球の位置
	//! @param[in]	Qr			… 球の半径
	//! @param[out] outMove		… めり込んだ分のベクトルが返る(面→球の方向)
	//! @param[in] startFaceNo	… 判定する面の開始番号　0で最初から
	//! @param[in] faceNum		… 判定する面の数　-1で全ての面数
	//! @param[out] outHitDataTbl … ヒットした座標が返る
	//! @return trueだとヒット
	//=======================================================================
	static bool MeshToSphere(const YsMesh& mesh, const YsMatrix* lpMat, const YsVec3& vQPos, float Qr, YsVec3* outMove, int startFaceNo = 0, int faceNum = -1, std::vector<MTS_HitData>* outHitDataTbl = nullptr);

	//=======================================================================
	//! @brief 球とメッシュ(YsMeshの中にある頂点や面情報を使用 指定マテリアルの面のみ)
	//! meshDataの指定したマテリアルNoの面とだけ判定
	//! @param[in]	mateNo	… 判定したいマテリアル番号
	//=======================================================================
	static bool MeshToSphere(const YsMesh& mesh, int mateNo, const YsMatrix* lpMat, const YsVec3& vQPos, float Qr, YsVec3* outMove);
};

}
#endif