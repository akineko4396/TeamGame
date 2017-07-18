#include "YskLib.h"

using namespace YskLib;

// D3D11_USAGE_DYNAMIC Version(WriteVertexData()で更新頻度が多い場合に有利)
void YsPolygon::Create(const void* vertices, int vertexNum, const YsVertexTypeData& vtd, bool isDynamic)
{
	Release();

	m_VTD = vtd;
	m_NumVertex = vertexNum;

	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = m_VTD.ByteSize * vertexNum;
	if(isDynamic){
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else{
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if( FAILED( YsDx.GetDev()->CreateBuffer( &bd, (vertices ? &InitData : nullptr), &m_VB ) ) ){
		return;
	}

	if(isDynamic){
		m_TypeFlag = BufType::Dynamic;
	}
	else{
		m_TypeFlag = BufType::Default;
	}
}

void YsPolygon::Release()
{
	YsSafe_Release(m_VB);
	m_TypeFlag = BufType::None;
	m_NumVertex = 0;
}

void YsPolygon::WriteVertexData(const void* vertices, int vertexNum)
{
	if(m_TypeFlag == BufType::Dynamic){
		D3D11_MAPPED_SUBRESOURCE pData;
		if(SUCCEEDED(YsDx.GetDevContext()->Map(m_VB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

			memcpy_s(pData.pData, pData.RowPitch, vertices, m_VTD.ByteSize * vertexNum);

			YsDx.GetDevContext()->Unmap(m_VB, 0);
		}
	}
	else if(m_TypeFlag == BufType::Default) {
		YsDx.GetDevContext()->UpdateSubresource(m_VB, 0, 0, vertices, 0, 0);
	}
}

bool YsPolygon::Map(D3D11_MAPPED_SUBRESOURCE* pOutMappedResource)
{
	if(m_TypeFlag == BufType::Dynamic){
		if(SUCCEEDED(YsDx.GetDevContext()->Map(m_VB, 0, D3D11_MAP_WRITE_DISCARD, 0, pOutMappedResource))) {
			return true;
		}
	}
	return false;
}
void YsPolygon::Unmap()
{
	YsDx.GetDevContext()->Unmap(m_VB, 0);
}

void YsPolygon::SetDrawData(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	// 頂点バッファーをセット
	UINT stride = m_VTD.ByteSize;
	UINT offset = 0;
	YsDx.GetDevContext()->IASetVertexBuffers( 0, 1, &m_VB, &stride, &offset );

	// プリミティブ・トポロジーをセット
	YsDx.GetDevContext()->IASetPrimitiveTopology(primitiveTopology);
}

void YsPolygon::Draw(UINT vertexCount)
{
	if(vertexCount == 0xFFFFFFFF)vertexCount = m_NumVertex;

	YsDx.GetDevContext()->Draw(vertexCount, 0);
}
