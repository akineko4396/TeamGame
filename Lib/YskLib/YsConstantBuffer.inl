//=================================================================
//
// YsConstantBuffer�e���v���[�g�N���X�̎�������
//
//=================================================================

// �萔�o�b�t�@�쐬
template<typename T>
bool YsConstantBuffer<T>::Create(UINT StartSlot, bool isDynamic)
{
	Release();

	// �T�C�Y�`�F�b�N(�萔�o�b�t�@��16�o�C�g�A���C�����g(16�̔{��)�ɂ���K�v������)
	int sz = sizeof(T);
	if (sz % 16 != 0) {
		MessageBox(nullptr, "�R���X�^���g�o�b�t�@�́A�\���̂̃T�C�Y��16�̔{���łȂ���΂Ȃ�Ȃ��I", "YsConstantBuffer", MB_OK);
		return false;
	}

	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sz;	// D3D11_BIND_CONSTANT_BUFFER�̏ꍇ�́Afloat4�̔{��(16�o�C�g)���K�{
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

// ���
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