//===============================================================
//! @file YsGeometryShader.h
//! @brief �W�I���g���V�F�[�_�N���X
//!
//! @author ���c
//===============================================================
#ifndef __YsGeometryShader_h__
#define __YsGeometryShader_h__

namespace YskLib
{

//=======================================================================
//! @brief �W�I���g���V�F�[�_�N���X
//!
//! [��ȋ@�\]									\n
//!	�EHLSL�t�@�C������W�I���g���V�F�[�_�쐬	n\
//!	�E������f�o�C�X�ɃZ�b�g����@�\			\n
//!
//! @ingroup Graphics_Shader
//=======================================================================
class YsGeometryShader {
public:

	//--------------------------------------------------------------------------
	//! @brief HLSL�t�@�C����ǂݍ��݁A�W�I���g���V�F�[�_���쐬����B
	//! �R���p�C���ς݂̃V�F�[�_�t�@�C��������΁A�����D��I�ɓǂݍ���
	//! @param[in] hlslFileName		�c HLSL�t�@�C���p�X
	//! @param[in] VsFuncName		�c HLSL���̃W�I���g���V�F�[�_�֐���
	//! @param[in] shaderModel		�c �g�p����V�F�[�_���f�����w�� ��)"ps_5_0"
	//! @param[in] shaderFlag		�c �ǉ��̃V�F�[�_�R���p�C���t���O
	//! @return ����:true
	//--------------------------------------------------------------------------
	bool Create(const std::string& hlslFileName, const std::string& GsFuncName, const std::string& shaderModel, UINT shaderFlag);

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
	//! @brief �V�F�[�_���f�o�C�X�ɃZ�b�g
	//--------------------------------------------------------------------------
	void SetShader();

	//--------------------------------------------------------------------------
	//! @brief �W�I���g���V�F�[�_�𖳌��ɂ���
	//! SetShader()�ōŌ�ݒ肷��ƗL���ɂȂ�
	//--------------------------------------------------------------------------
	static void DisableShader();

	// 
	YsGeometryShader() : m_GS(0), m_pCompiledShader(0){
	}

	~YsGeometryShader() {
		Release();
	}

private:
	ID3D11GeometryShader*	m_GS;			//!< �W�I���g���V�F�[�_�[

	ID3DBlob*			m_pCompiledShader;	//!< �R���p�C���ς݃V�F�[�_�f�[�^

	std::string			m_hlslFileName;		//!< HLSL�t�@�C����
	std::string			m_csoFileName;		//!< �R���p�C���ς݃V�F�[�_(CSO)���̃t�@�C����

private:
	// �R�s�[�֎~�p
	YsGeometryShader(const YsGeometryShader& src) {}
	void operator=(const YsGeometryShader& src) {}
};

}
#endif
