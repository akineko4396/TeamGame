//===============================================================
//! @file YsVertexShader.h
//! @brief ���_�V�F�[�_�N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsVertexShader_h__
#define __YsVertexShader_h__

namespace YskLib
{

//=======================================================================
//! @brief ���_�V�F�[�_�N���X
//!
//! [��ȋ@�\]								\n
//!	�EHLSL�t�@�C�����璸�_�V�F�[�_�쐬		\n
//!	�E���_���C�A�E�g�쐬					\n
//!	�E�������f�o�C�X�ɃZ�b�g����@�\		\n
//!
//! @ingroup Graphics_Shader
//=======================================================================
class YsVertexShader{
public:

	//--------------------------------------------------------------------------
	//! @brief HLSL�t�@�C����ǂݍ��݁A���_�V�F�[�_�E���_���C�A�E�g���쐬����B
	//! �R���p�C���ς݂̃V�F�[�_�t�@�C��������΁A�����D��I�ɓǂݍ���
	//! @param[in] hlslFileName		�c HLSL�t�@�C���p�X
	//! @param[in] VsFuncName		�c HLSL���̒��_�V�F�[�_�֐���
	//! @param[in] shaderModel		�c �g�p����V�F�[�_���f�����w�� ��)"vs_5_0"
	//! @param[in] shaderFlag		�c �ǉ��̃V�F�[�_�R���p�C���t���O
	//! @param[in] vertexTypeData	�c ���_���C�A�E�g�p�̃f�[�^�@���_�V�F�[�_�̓��͂ɂ������`����n������
	//! @return ����:true
	//--------------------------------------------------------------------------
	bool Create(const std::string& hlslFileName, const std::string& VsFuncName, const std::string& shaderModel, UINT shaderFlag, const YsVertexTypeData& vertexTypeData);

	//--------------------------------------------------------------------------
	//! @brief ���̒��_�V�F�[�_�p�̒��_���C�A�E�g���쐬����BCreate()������ɌĂԂ��ƁB
	//! @param[in] vertexTypeData	�c ���_���C�A�E�g�p�̃f�[�^�@���_�V�F�[�_�̓��͂ɂ������`����n������
	//! @return �����F�쐬���ꂽID3D11InputLayout�̃A�h���X ���g��Ȃ��Ȃ�����Release()���ĂԂ���
	//--------------------------------------------------------------------------
	ID3D11InputLayout* CreateVertexLayout(const YsVertexTypeData& vertexTypeData);

	//--------------------------------------------------------------------------
	//! @brief �R���p�C���ς݃V�F�[�_���t�@�C���ɕۑ�����
	//! Create()���Ɏw�肵���A[hlslFileName]_[VsFuncName].cso �̃t�@�C�����ŕۑ������
	//! @param[in] filename		�c �ۑ�����t�@�C���p�X
	//! @return ����:true
	//--------------------------------------------------------------------------
	bool SaveToFile();

	//--------------------------------------------------------------------------
	//! @brief ���
	//--------------------------------------------------------------------------
	void Release();

	//--------------------------------------------------------------------------
	//! @brief �V�F�[�_���f�o�C�X�ɃZ�b�g(���_���C�A�E�g���Z�b�g)
	//! @param[in] vertexLayout		�c ���_���C�A�E�g�@NULL����Init()���Ɏw�肵�����̂��g�p�����
	//--------------------------------------------------------------------------
	void SetShader(ID3D11InputLayout* vertexLayout = nullptr);

	//--------------------------------------------------------------------------
	//! @brief ���_�V�F�[�_�𖳌��ɂ���
	//! SetShader()�ōŌ�ݒ肷��ƗL���ɂȂ�
	//--------------------------------------------------------------------------
	static void DisableShader();

	//
	YsVertexShader() : m_VS(0), m_VLayout(0), m_pCompiledShader(0){
	}
	~YsVertexShader(){
		Release();
	}

private:
	ID3D11VertexShader*	m_VS;				//!< ���_�V�F�[�_�[
	ID3D11InputLayout*	m_VLayout;			//!< ���_�C���v�b�g���C�A�E�g

	ID3DBlob*			m_pCompiledShader;	//!< �R���p�C���ς݃V�F�[�_�f�[�^

	std::string			m_hlslFileName;		//!< HLSL�t�@�C����
	std::string			m_csoFileName;		//!< �R���p�C���ς݃V�F�[�_(CSO)���̃t�@�C����

private:
	// �R�s�[�֎~�p
	YsVertexShader(const YsVertexShader& src) {}
	void operator=(const YsVertexShader& src) {}
};



}

#endif
