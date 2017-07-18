//=================================================================
//
// YsConstantBufferテンプレートクラスの実装部分
//
//=================================================================

// 定数バッファ作成
template<typename T>
bool YsConstantBuffer<T>::Create(UINT StartSlot, bool isDynamic)
{
	Release();

	// サイズチェック(定数バッファは16バイトアライメント(16の倍数)にする必要がある)
	int sz = sizeof(T);
	if (sz % 16 != 0) {
		MessageBox(nullptr, "コンスタントバッファは、構造体のサイズが16の倍数でなければならない！", "YsConstantBuffer", MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sz;	// D3D11_BIND_CONSTANT_BUFFERの場合は、float4の倍数(16バイト)が必須
	if (isDynamic) {
		cb.Usage = D3D11_USAGE_DYNAMIC;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		cb.Usage = D3D11_USAGE_DEFAULT;
		cb.CPUAccessFlags = 0;
	}
	cb.StructureByteStride = 0;
	cb.MiscFlags = 0;
	if (FAILED(YsDx.GetDev()->CreateBuffer(&cb, nullptr, &m_pBuffer)))
	{
		return false;
	}

	m_StartSlot = StartSlot;
	if (isDynamic) {
		m_TypeFlag = BufType::Dynamic;
	}
	else {
		m_TypeFlag = BufType::Default;
	}
	return true;
}

// 解放
template<typename T>
void YsConstantBuffer<T>::Release()
{
	YsSafe_Release(m_pBuffer);
	m_TypeFlag = BufType::None;
	m_StartSlot = 0;
}

template<typename T>
void YsConstantBuffer<T>::WriteData(int writeBytes)
{
	if (writeBytes == -1) {
		writeBytes = sizeof(T);
	}

	if (m_TypeFlag == BufType::Dynamic) {
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(YsDx.GetDevContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			memcpy_s(pData.pData, pData.RowPitch, &m_Data, writeBytes);
			YsDx.GetDevContext()->Unmap(m_pBuffer, 0);
		}
	}
	else if (m_TypeFlag == BufType::Default) {
		YsDx.GetDevContext()->UpdateSubresource(m_pBuffer, 0, nullptr, &m_Data, 0, 0);
	}
}