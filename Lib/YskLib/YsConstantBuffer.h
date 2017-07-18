//===============================================================
//! @file YsConstantBuffer.h
//! @brief �萔�o�b�t�@�N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsConstantBuffer_h__
#define __YsConstantBuffer_h__

namespace YskLib
{

//==============================================================
//!
//! @brief �R���X�^���g(�萔)�o�b�t�@ �e���v���[�g�N���X
//!
//! [��ȋ@�\]														\n
//!	�ECreate()�Ŏw��N���X�E�\���̂��R���X�^���g�o�b�t�@�Ƃ��č쐬	\n
//!	�EWriteData()�Ńo�b�t�@�ɏ�������								\n
//!	�ESetVS()��SetPS()�ȂǂŃf�o�C�X�փZ�b�g						\n
//!
//! @ingroup Graphics_Shader
//==============================================================
template<typename T>
class YsConstantBuffer{
public:

	//! @brief �萔�o�b�t�@�f�[�^�@WriteData�֐��ł����̓��e���������܂��
	T			m_Data;

	//! @brief �o�b�t�@�̎��
	enum class BufType{
		None,			//!< �Ȃ�
		Default,		//!< GPU����Write�� Read���@CPU����Write�~Read�~�@(������UpdateSubresource()�ōX�V�͉\)�@����
		Dynamic,		//!< GPU����Write�~ Read���@CPU����Write��Read�~�@�p�ɂɍX�V�����悤�ȃo�b�t�@�͂������̕��������������ADefault�ɔ�ׂ��班�����x�͗�����
	};

	//! @brief ����������Ă��邩
	bool IsInit() { return m_pBuffer == nullptr ? false : true; }

	//--------------------------------------------------------
	//! @brief �쐬
	//! @param[in] StartSlot : �萔�o�b�t�@���Z�b�g����ꏊ(HLSL��register(b0)�Ƃ��́Ab0�̂Ƃ���̔ԍ�)
	//! @param[in] isDynamic : ���I�o�b�t�@�Ƃ��č쐬����(�p�ɂɍX�V����ꍇ�͂��ꂪ����)
	//! @return �쐬����:true
	//--------------------------------------------------------
	bool Create(UINT StartSlot, bool isDynamic = true);

	//--------------------------------------------------------
	//! @brief ���
	//--------------------------------------------------------
	void Release();

	//--------------------------------------------------------
	//! @brief m_Data��萔�o�b�t�@�ɏ�������
	//! @param[in]	writeBytes �c �������ރo�C�g�� -1�őS��
	//--------------------------------------------------------
	void WriteData(int writeBytes = -1);

	//--------------------------------------------------------
	//! @brief ���_�V�F�[�_�ɐݒ�
	//! @param[in] slotNo �c �萔�o�b�t�@���Z�b�g����ꏊ(HLSL��register(b0)�Ƃ��́Ab0�̂Ƃ���̔ԍ�) -1��Create()�����Ƃ��Ɏw�肵���ԍ�
	//--------------------------------------------------------
	void SetVS(int slotNo = -1){
		if(slotNo < 0)slotNo = m_StartSlot;
		YsDx.GetDevContext()->VSSetConstantBuffers(slotNo, 1, &m_pBuffer);
	}

	//--------------------------------------------------------
	//! @brief �s�N�Z���V�F�[�_�ɐݒ�
	//! @param[in] slotNo �c �萔�o�b�t�@���Z�b�g����ꏊ(HLSL��register(b0)�Ƃ��́Ab0�̂Ƃ���̔ԍ�) -1��Create()�����Ƃ��Ɏw�肵���ԍ�
	//--------------------------------------------------------
	void SetPS(int slotNo = -1){
		if(slotNo < 0)slotNo = m_StartSlot;
		YsDx.GetDevContext()->PSSetConstantBuffers(slotNo, 1, &m_pBuffer);
	}

	//--------------------------------------------------------
	//! @brief �W�I���g���V�F�[�_�ɐݒ�
	//! @param[in] slotNo �c �萔�o�b�t�@���Z�b�g����ꏊ(HLSL��register(b0)�Ƃ��́Ab0�̂Ƃ���̔ԍ�) -1��Create()�����Ƃ��Ɏw�肵���ԍ�
	//--------------------------------------------------------
	void SetGS(int slotNo = -1){
		if(slotNo < 0)slotNo = m_StartSlot;
		YsDx.GetDevContext()->GSSetConstantBuffers(slotNo, 1, &m_pBuffer);
	}

	//--------------------------------------------------------
	//! @brief �R���s���[�g�V�F�[�_�ɐݒ�
	//! @param[in] slotNo �c �萔�o�b�t�@���Z�b�g����ꏊ(HLSL��register(b0)�Ƃ��́Ab0�̂Ƃ���̔ԍ�) -1��Create()�����Ƃ��Ɏw�肵���ԍ�
	//--------------------------------------------------------
	void SetCS(int slotNo = -1){
		if(slotNo < 0)slotNo = m_StartSlot;
		YsDx.GetDevContext()->CSSetConstantBuffers(slotNo, 1, &m_pBuffer);
	}


	//
	YsConstantBuffer() : m_pBuffer(0), m_StartSlot(-1), m_TypeFlag(BufType::None) {
	}
	//
	~YsConstantBuffer(){
		Release();
	}

protected:
	ID3D11Buffer*		m_pBuffer;		//!< Direct3D11�萔�o�b�t�@
	int					m_StartSlot;	//!< �Z�b�g����萔�o�b�t�@�ԍ�
	BufType				m_TypeFlag;		//!< BufType

private:
	// �R�s�[�֎~�p
	YsConstantBuffer(const YsConstantBuffer& src) {}
	void operator=(const YsConstantBuffer& src) {}
};

#include "YsConstantBuffer.inl"

}
#endif
