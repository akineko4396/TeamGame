#include "YskLib.h"

using namespace YskLib;

bool YsVertexShader::Create(const std::string& hlslFileName, const std::string& VsFuncName, const std::string& shaderModel, UINT shaderFlag, const YsVertexTypeData& vertexTypeData)
{
	Release();

	// �œK��
#ifdef OPTIMIZESHADER
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL3;	// �ő�̍œK�� �����[�X�p
#else
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL0 | D3DXSHADER_DEBUG;	// �Œ�̍œK�� �J���p
#endif

	ID3DBlob* pErrors = nullptr;

	m_hlslFileName = hlslFileName;

	//==========================================================================
	// �R���p�C���ς�HLSL�t�@�C��(cso)������΁A�������D��œǂݍ���
	//==========================================================================
	// cso�t�@�C��������( [hlslFileName]_[VsFuncName].cso )
	// ��) aaa.hlsl���w�肵���ꍇ�Œ��_�V�F�[�_�֐�����"VS"�̎��Aaaa_VS.cso �t�@�C��������΂����ǂݍ���
	char szDir[_MAX_PATH];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(hlslFileName.c_str(), nullptr, szDir, szFname, szExt);

	m_csoFileName = szDir;
	m_csoFileName += szFname;
	m_csoFileName += "_";
	m_csoFileName += VsFuncName;
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
	// HLSL���璸�_�V�F�[�_���R���p�C�����ǂݍ���
	//==========================================================================
	if(m_pCompiledShader == nullptr){
		if(FAILED(D3DX11CompileFromFile(hlslFileName.c_str(), nullptr, nullptr, VsFuncName.c_str(), shaderModel.c_str(), shaderFlag, 0, nullptr, &m_pCompiledShader, &pErrors, nullptr))) {
			if(pErrors){
				MessageBox(0, (char*)pErrors->GetBufferPointer(), "HLSL�R���p�C�����s", MB_OK);
			}
			else{
				MessageBox(0, "HLSL�t�@�C�������݂��Ȃ�", hlslFileName.c_str(), MB_OK);
			}
			Release();
			return false;
		}
		YsSafe_Release(pErrors);
	}

	//==========================================================================
	// �u���u���璸�_�V�F�[�_�[�쐬
	//==========================================================================
	if(FAILED(YsDx.GetDev()->CreateVertexShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), nullptr, &m_VS))) {
		MessageBox(0, "���_�V�F�[�_�[�쐬���s", hlslFileName.c_str(), MB_OK);
		Release();
		return false;
	}

	//==========================================================================
	// ���_�C���v�b�g���C�A�E�g���쐬
	//==========================================================================
	m_VLayout = CreateVertexLayout(vertexTypeData);
	if(m_VLayout == nullptr){
		MessageBox(0, "���_�C���v�b�g���C�A�E�g�쐬���s", hlslFileName.c_str(), MB_OK);
		Release();
		return false;
	}

	return true;
}

ID3D11InputLayout* YsVertexShader::CreateVertexLayout(const YsVertexTypeData& vertexTypeData)
{
	ID3D11InputLayout* layout = nullptr;

	//==========================================================================
	// ���_�C���v�b�g���C�A�E�g���쐬
	//==========================================================================
	if(FAILED(YsDx.GetDev()->CreateInputLayout(	vertexTypeData.pLayout,
												vertexTypeData.Num, 
												m_pCompiledShader->GetBufferPointer(),
												m_pCompiledShader->GetBufferSize(),
												&layout)))
	{
		return nullptr;
	}

	return layout;
}

bool YsVertexShader::SaveToFile()
{
	if(m_pCompiledShader == nullptr)return false;

	//==========================================================================
	// �R���p�C���ς݂̃V�F�[�_��ۑ�����
	//==========================================================================
	std::ofstream ofs(m_csoFileName, std::ios::binary);
	ofs.write((char*)m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize());

	return true;
}


void YsVertexShader::Release()
{
	YsSafe_Release(m_pCompiledShader);
	YsSafe_Release(m_VS);
	YsSafe_Release(m_VLayout);

	m_hlslFileName = "";
	m_csoFileName = "";
}

void YsVertexShader::SetShader(ID3D11InputLayout* vertexLayout)
{
	if(vertexLayout == nullptr)vertexLayout = m_VLayout;

	// ���_���̓��C�A�E�g�ݒ�
	YsDx.GetDevContext()->IASetInputLayout(vertexLayout);
	// ���_�V�F�[�_�ݒ�
	YsDx.GetDevContext()->VSSetShader(m_VS, nullptr, 0);
}

void YsVertexShader::DisableShader()
{
	// ���_�V�F�[�_����
	YsDx.GetDevContext()->VSSetShader(nullptr, 0, 0);
}
