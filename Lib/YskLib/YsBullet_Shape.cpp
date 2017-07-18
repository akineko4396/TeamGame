#include "YskLib.h"

using namespace YskLib;


//==========================================================================
// 複合形状クラス
//==========================================================================
void YsBulletShape_Compound::AddChildShape(SPtr<YsBulletShape_Base> shape, const YsMatrix& mat)
{
	// 子リストに追加
	m_ShapeList.push_back(shape);

	btTransform t;
	t.setFromOpenGLMatrix(&mat._11);
	m_Shape->addChildShape(t, shape->GetShape().get());
	
//	UpdateInertia();
}

void YsBulletShape_Compound::Release()
{
	m_Shape = nullptr;

	// 子形状削除
	m_ShapeList.clear();
}


//==========================================================================
// メッシュ形状クラス
//==========================================================================
void YsBulletShape_Mesh::CreateFromMesh(const YsMesh& pMesh, const YsVec3& localScale)
{
	int vertStride = sizeof(btVector3);
	int indexStride = 3 * sizeof(int);

	// 頂点数
	int totalVerts = pMesh.GetNumVerts();
	m_Verts = new btVector3[totalVerts];
	// 頂点データ
	for(int i = 0; i < totalVerts; i++) {
		const YsVec3& pos0 = pMesh.GetVertex_Pos(i);
		m_Verts[i] = btVector3(pos0.x, pos0.y, pos0.z);
	}


	// インデックス 面数ｘ３
	int totalTriangles = pMesh.GetNumFaces();
	m_Idxs = new int[totalTriangles * 3];

	// インデックスデータ作成
	for(int i = 0; i<totalTriangles; i++) {
		m_Idxs[i * 3] = pMesh.GetFace()[i].idx[0];// i * 3;
		m_Idxs[i * 3 + 1] = pMesh.GetFace()[i].idx[1];//i * 3 + 1;
		m_Idxs[i * 3 + 2] = pMesh.GetFace()[i].idx[2];//i * 3 + 2;
	}

	// 
	m_indexVertexArrays = new btTriangleIndexVertexArray(
		totalTriangles,
		m_Idxs,
		indexStride,
		totalVerts,
		(btScalar*)m_Verts,
		vertStride);

	YsVec3 vBBMin = pMesh.GetAABBMin();
	YsVec3 vBBMax = pMesh.GetAABBMax();
	btVector3 aabbMin(vBBMin.x, vBBMin.y, vBBMin.z);
	btVector3 aabbMax(vBBMax.x, vBBMax.y, vBBMax.z);

	bool useQuantizedAabbCompression = true;
	m_Shape = SPtr<btBvhTriangleMeshShape>(new btBvhTriangleMeshShape(m_indexVertexArrays, useQuantizedAabbCompression, aabbMin, aabbMax));	// ※Bulletライブラリはmake_shared使うと失敗する時があったので、通常作成
	m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));

//	m_Mass = 0;
//	m_Inertia.setValue(0, 0, 0);
}
