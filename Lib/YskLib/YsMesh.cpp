#include "YskLib.h"

using namespace YskLib;

//=======================================================================
//
// YsMesh
//
//=======================================================================

const YsMeshSubset* YsMesh::GetSubset(int subsetNo) const
{
	if(subsetNo >= (int)m_Subset.size())return nullptr;

	return &m_Subset[subsetNo];
}

void YsMesh::SetDrawData()
{
	// 頂点バッファーをセット
	UINT stride = m_VertexTypeData.ByteSize;
	UINT offset = 0;
	YsDx.GetDevContext()->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);

	// インデックスバッファセット
	YsDx.GetDevContext()->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	//プリミティブ・トポロジーをセット
	YsDx.GetDevContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void YsMesh::DrawSubset(int subsetNo)
{
	if(subsetNo >= (int)m_Subset.size())return;
	if(m_Subset[subsetNo].FaceCount == 0)return;

	// メッシュ描画
	YsDx.GetDevContext()->DrawIndexed(m_Subset[subsetNo].FaceCount * 3, m_Subset[subsetNo].FaceStart * 3, 0);
}

void YsMesh::DrawSubsetInstance(int subsetNo, int instanceNum)
{
	if(subsetNo >= (int)m_Subset.size())return;
	if(m_Subset[subsetNo].FaceCount == 0)return;

	// メッシュ描画
	YsDx.GetDevContext()->DrawIndexedInstanced(m_Subset[subsetNo].FaceCount * 3, instanceNum, m_Subset[subsetNo].FaceStart * 3, 0, 0);
}

void YsMesh::Release()
{
	m_VertexBuf.clear();
	m_VertexBuf.shrink_to_fit();

	m_Faces.clear();
	m_Faces.shrink_to_fit();

	m_ExtFaces.clear();
	m_ExtFaces.shrink_to_fit();

	YsSafe_Release(m_VB);
	YsSafe_Release(m_IB);
	m_Subset.clear();

	ZeroMemory(&m_VertexTypeData, sizeof(m_VertexTypeData));
}

bool YsMesh::CreateVertexBuffer()
{
	if (m_VertexBuf.size() == 0)return false;

	YsSafe_Release(m_VB);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = m_VertexBuf.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_VertexBuf[0];
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	if (FAILED(YsDx.GetDev()->CreateBuffer(&bd, &InitData, &m_VB))) {
		return false;
	}

	return true;
}

bool YsMesh::SetFaceArray(const Face* faceArray, unsigned int numFace, bool bCreateIndexBuffer)
{
	m_Faces.resize(numFace);
	m_Faces.shrink_to_fit();

	// 面情報コピー
	memcpy(&m_Faces[0], faceArray, numFace * sizeof(Face));

	// 
	m_ExtFaces.clear();

	// インデックスバッファ作成
	if (bCreateIndexBuffer) {
		return CreateIndexBuffer();
	}
	return true;
}


bool YsMesh::CreateIndexBuffer()
{
	if (m_Faces.size() == 0)return false;

	YsSafe_Release(m_IB);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = m_Faces.size() * sizeof(Face);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(UINT);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_Faces[0];
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(YsDx.GetDev()->CreateBuffer(&bd, &InitData, &m_IB))) {
		return false;
	}

	return true;
}

bool YsMesh::CreateExtData()
{
	if (m_Faces.size() == 0 || m_VertexBuf.size() == 0) {
		m_vBBMin.Set(0, 0, 0);
		m_vBBMax.Set(0, 0, 0);
		m_vBSCenter.Set(0, 0, 0);
		m_fBSRadius = 0;
		m_ExtFaces.clear();
		return false;
	}

	// 面拡張データ作成
	m_ExtFaces.resize(m_Faces.size());
	m_ExtFaces.shrink_to_fit();
	for (UINT i = 0; i < m_ExtFaces.size(); i++) {
		// 頂点座標
		const YsVec3& vp0 = GetVertex_Pos(i, 0);
		const YsVec3& vp1 = GetVertex_Pos(i, 1);
		const YsVec3& vp2 = GetVertex_Pos(i, 2);

		// 面の方向
		YsVec3 v1 = vp1 - vp0;
		YsVec3 v2 = vp2 - vp0;
		YsVec3::Cross(m_ExtFaces[i].vN, v1, v2);	// 外積
		m_ExtFaces[i].vN.Normalize();

		// 面のAABB
		YsVec3::CreateAABB(vp0, vp1, vp2, m_ExtFaces[i].vMin, m_ExtFaces[i].vMax);
	}

	// メッシュ自体のAABB
	YsVec3 vMin = GetVertex_Pos(0);
	YsVec3 vMax = vMin;
	for (UINT i = 1; i<GetNumVerts(); i++) {
		vMin.x = min(vMin.x, GetVertex_Pos(i).x);
		vMin.y = min(vMin.y, GetVertex_Pos(i).y);
		vMin.z = min(vMin.z, GetVertex_Pos(i).z);

		vMax.x = max(vMax.x, GetVertex_Pos(i).x);
		vMax.y = max(vMax.y, GetVertex_Pos(i).y);
		vMax.z = max(vMax.z, GetVertex_Pos(i).z);
	}
	m_vBBMin = vMin;
	m_vBBMax = vMax;

	// バウンディングスフィア(現在簡易的なものです)
	YsVec3 v = vMax - vMin;
	m_vBSCenter = vMin + v*0.5f;
	m_fBSRadius = v.Length();

	return true;
}


