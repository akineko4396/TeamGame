#include "YskLib.h"

using namespace YskLib;

bool YsPixelShader::Create(const std::string& hlslFileName, const std::string& PsFuncName, const std::string& shaderModel, UINT shaderFlag)
{
	Release();

	// �œK��
#ifdef OPTIMIZESHADER
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL3;	// �ő�̍œK�� �����[�X�p
#else
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL0 | D3DXSHADER_DEBUG;	// �Œ�̍œK�� �J���p
#endif


	ID3DBlob *pErrors = nullptr;

	m_hlslFileName = hlslFileName;

	//==========================================================================
	// �R���p�C���ς�HLSL�t�@�C��(cso)������΁A�������D��œǂݍ���
	//==========================================================================
	// cso�t�@�C��������( [hlslFileName]_[VsFuncName].cso )
	char szDir[_MAX_PATH];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(hlslFileName.c_str(), nullptr, szDir, szFname, szExt);

	m_csoFileName = szDir;
	m_csoFileName += szFname;
	m_csoFileName += "_";
	m_csoFileName += PsFuncName;
	m_csoFileName += ".cso";
	{
		std::ifstream ifs(m_csoFileName, std::ios::binary);
		if(!ifs.fail()){
			// �t�@�C���T�C�Y
			ifs.seekg(0, std::ios::end);
			int len = (int)ifs.tellg();
			ifs.seekg(0, std::ios::beg);

			// Blob(�o�b�t�@)�쐬
			D3DCreateBlob(len, &m_pCompiledShader);
			// �ǂݍ���
			ifs.read((char*)m_pCompiledShader->GetBufferPointer(), len);

			Dw_Scroll(0, "HLSL�͓ǂݍ��܂��Acso��ǂݍ��݂܂�[%s]", m_csoFileName.c_str());
		}
	}

	//==========================================================================
	// HLSL����s�N�Z���V�F�[�_���R���p�C��
	//==========================================================================
	if(m_pCompiledShader == nullptr){
		if(FAILED(D3DX11CompileFromFile(hlslFileName.c_str(), nullptr, nullptr, PsFuncName.c_str(), shaderModel.c_str(), shaderFlag, 0, nullptr, &m_pCompiledShader, &pErrors, nullptr))) {
			if(pErrors){
				MessageBox(0, (char*)pErrors->GetBufferPointer(), "HLSL�R���p�C�����s", MB_OK);
			}
			else{
				MessageBox(0, "HLSL�t�@�C�������݂��Ȃ�", hlslFileName.c_str(), MB_OK);
			}
			return false;
		}
		YsSafe_Release(pErrors);
	}

	//==========================================================================
	// �u���u����s�N�Z���V�F�[�_�[�쐬
	//==========================================================================
	if(FAILED(YsDx.GetDev()->CreatePixelShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), nullptr, &m_PS))) {
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", hlslFileName.c_str(), MB_OK);
		Release();
		return false;
	}

	return true;
}

bool YsPixelShader::SaveToFile()
{
	if(m_pCompiledShader == nullptr)return false;

	//==========================================================================
	// �R���p�C���ς݂̃V�F�[�_��ۑ�����
	//==========================================================================
	std::ofstream ofs(m_csoFileName, std::ios::binary);
	ofs.write((char*)m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize());

	return true;
}

void YsPixelShader::Release() 
{
	YsSafe_Release(m_pCompiledShader);
	YsSafe_Release(m_PS);
}

void YsPixelShader::SetShader()
{
	// �s�N�Z���V�F�[�_�ݒ�
	YsDx.GetDevContext()->PSSetShader(m_PS, 0, 0);
}

void YsPixelShader::DisableShader()
{
	// �s�N�Z���V�F�[�_����
	YsDx.GetDevContext()->PSSetShader(nullptr, 0, 0);
}
