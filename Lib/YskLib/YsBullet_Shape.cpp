#include "YskLib.h"

using namespace YskLib;


//==========================================================================
// �����`��N���X
//==========================================================================
void YsBulletShape_Compound::AddChildShape(SPtr<YsBulletShape_Base> shape, const YsMatrix& mat)
{
	// �q���X�g�ɒǉ�
	m_ShapeList.push_back(shape);

	btTransform t;
	t.setFromOpenGLMatrix(&mat._11);
	m_Shape->addChildShape(t, shape->GetShape().get());
	
//	UpdateInertia();
}

void YsBulletShape_Compound::Release()
{
	m_Shape = nullptr;

	// �q�`��폜
	m_ShapeList.clear();
}


//==========================================================================
// ���b�V���`��N���X
//==========================================================================
void YsBulletShape_Mesh::CreateFromMesh(const YsMesh& pMesh, const YsVec3& localScale)
{
	int vertStride = sizeof(btVector3);
	int indexStride = 3 * sizeof(int);

	// ���_��
	int totalVerts = pMesh.GetNumVerts();
	m_Verts = new btVector3[totalVerts];
	// ���_�f�[�^
	for(int i = 0; i < totalVerts; i++) {
		const YsVec3& pos0 = pMesh.GetVertex_Pos(i);
		m_Verts[i] = btVector3(pos0.x, pos0.y, pos0.z);
	}


	// �C���f�b�N�X �ʐ����R
	int totalTriangles = pMesh.GetNumFaces();
	m_Idxs = new int[totalTriangles * 3];

	// �C���f�b�N�X�f�[�^�쐬
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
	m_Shape = SPtr<btBvhTriangleMeshShape>(new btBvhTriangleMeshShape(m_indexVertexArrays, useQuantizedAabbCompression, aabbMin, aabbMax));	// ��Bullet���C�u������make_shared�g���Ǝ��s���鎞���������̂ŁA�ʏ�쐬
	m_Shape->setLocalScaling(btVector3(localScale.x, localScale.y, localScale.z));

//	m_Mass = 0;
//	m_Inertia.setValue(0, 0, 0);
}
