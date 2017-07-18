#include "YskLib.h"

using namespace YskLib;

bool YsGeometryShader::Create(const std::string& hlslFileName, const std::string& GsFuncName, const std::string& shaderModel, UINT shaderFlag)
{
	Release();

	// 最適化
#ifdef OPTIMIZESHADER
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL3;	// 最大の最適化 リリース用
#else
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL0 | D3DXSHADER_DEBUG;	// 最低の最適化 開発用
#endif


	ID3DBlob *pErrors = nullptr;

	m_hlslFileName = hlslFileName;

	//==========================================================================
	// コンパイル済みHLSLファイル(cso)があれば、そっち優先で読み込む
	//==========================================================================
	// csoファイル名生成( [hlslFileName]_[VsFuncName].cso )
	char szDir[_MAX_PATH];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(hlslFileName.c_str(), nullptr, szDir, szFname, szExt);

	m_csoFileName = szDir;
	m_csoFileName += szFname;
	m_csoFileName += "_";
	m_csoFileName += GsFuncName;
	m_csoFileName += ".cso";
	{
		std::ifstream ifs(m_csoFileName, std::ios::binary);
		if(!ifs.fail()){
			// ファイルサイズ
			ifs.seekg(0, std::ios::end);
			int len = (int)ifs.tellg();
			ifs.seekg(0, std::ios::beg);

			// Blob(バッファ)作成
			D3DCreateBlob(len, &m_pCompiledShader);
			// 読み込み
			ifs.read((char*)m_pCompiledShader->GetBufferPointer(), len);

			Dw_Scroll(0, "HLSLは読み込まず、csoを読み込みます[%s]", m_csoFileName.c_str());
		}
	}

	//==========================================================================
	// HLSLからピクセルシェーダをコンパイル
	//==========================================================================
	if(m_pCompiledShader == nullptr){
		if(FAILED(D3DX11CompileFromFile(hlslFileName.c_str(), nullptr, nullptr, GsFuncName.c_str(), shaderModel.c_str(), shaderFlag, 0, nullptr, &m_pCompiledShader, &pErrors, nullptr))) {
			if(pErrors){
				MessageBox(0, (char*)pErrors->GetBufferPointer(), "HLSLコンパイル失敗", MB_OK);
			}
			else{
				MessageBox(0, "HLSLファイルが存在しない", hlslFileName.c_str(), MB_OK);
			}
			return false;
		}
		YsSafe_Release(pErrors);
	}

	//==========================================================================
	// ブロブからジオメトリシェーダー作成
	//==========================================================================
	if(FAILED(YsDx.GetDev()->CreateGeometryShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), nullptr, &m_GS))) {
		MessageBox(0, "ジオメトリシェーダー作成失敗", hlslFileName.c_str(), MB_OK);
		Release();
		return false;
	}

	return true;
}

bool YsGeometryShader::SaveToFile()
{
	if(m_pCompiledShader == nullptr)return false;

	//==========================================================================
	// コンパイル済みのシェーダを保存する
	//==========================================================================
	std::ofstream ofs(m_csoFileName, std::ios::binary);
	ofs.write((char*)m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize());

	return true;
}

void YsGeometryShader::Release()
{
	YsSafe_Release(m_pCompiledShader);
	YsSafe_Release(m_GS);
}

void YsGeometryShader::SetShader()
{
	// ジオメトリシェーダ設定
	YsDx.GetDevContext()->GSSetShader(m_GS, 0, 0);
}

void YsGeometryShader::DisableShader()
{
	// ジオメトリシェーダ解除
	YsDx.GetDevContext()->GSSetShader(nullptr, 0, 0);
}
