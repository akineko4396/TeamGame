#include "YskLib.h"

using namespace YskLib;

bool YsVertexShader::Create(const std::string& hlslFileName, const std::string& VsFuncName, const std::string& shaderModel, UINT shaderFlag, const YsVertexTypeData& vertexTypeData)
{
	Release();

	// 最適化
#ifdef OPTIMIZESHADER
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL3;	// 最大の最適化 リリース用
#else
	shaderFlag |= D3DXSHADER_OPTIMIZATION_LEVEL0 | D3DXSHADER_DEBUG;	// 最低の最適化 開発用
#endif

	ID3DBlob* pErrors = nullptr;

	m_hlslFileName = hlslFileName;

	//==========================================================================
	// コンパイル済みHLSLファイル(cso)があれば、そっち優先で読み込む
	//==========================================================================
	// csoファイル名生成( [hlslFileName]_[VsFuncName].cso )
	// 例) aaa.hlslを指定した場合で頂点シェーダ関数名が"VS"の時、aaa_VS.cso ファイルがあればそれを読み込む
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
	// HLSLから頂点シェーダをコンパイルし読み込む
	//==========================================================================
	if(m_pCompiledShader == nullptr){
		if(FAILED(D3DX11CompileFromFile(hlslFileName.c_str(), nullptr, nullptr, VsFuncName.c_str(), shaderModel.c_str(), shaderFlag, 0, nullptr, &m_pCompiledShader, &pErrors, nullptr))) {
			if(pErrors){
				MessageBox(0, (char*)pErrors->GetBufferPointer(), "HLSLコンパイル失敗", MB_OK);
			}
			else{
				MessageBox(0, "HLSLファイルが存在しない", hlslFileName.c_str(), MB_OK);
			}
			Release();
			return false;
		}
		YsSafe_Release(pErrors);
	}

	//==========================================================================
	// ブロブから頂点シェーダー作成
	//==========================================================================
	if(FAILED(YsDx.GetDev()->CreateVertexShader(m_pCompiledShader->GetBufferPointer(), m_pCompiledShader->GetBufferSize(), nullptr, &m_VS))) {
		MessageBox(0, "頂点シェーダー作成失敗", hlslFileName.c_str(), MB_OK);
		Release();
		return false;
	}

	//==========================================================================
	// 頂点インプットレイアウトを作成
	//==========================================================================
	m_VLayout = CreateVertexLayout(vertexTypeData);
	if(m_VLayout == nullptr){
		MessageBox(0, "頂点インプットレイアウト作成失敗", hlslFileName.c_str(), MB_OK);
		Release();
		return false;
	}

	return true;
}

ID3D11InputLayout* YsVertexShader::CreateVertexLayout(const YsVertexTypeData& vertexTypeData)
{
	ID3D11InputLayout* layout = nullptr;

	//==========================================================================
	// 頂点インプットレイアウトを作成
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
	// コンパイル済みのシェーダを保存する
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

	// 頂点入力レイアウト設定
	YsDx.GetDevContext()->IASetInputLayout(vertexLayout);
	// 頂点シェーダ設定
	YsDx.GetDevContext()->VSSetShader(m_VS, nullptr, 0);
}

void YsVertexShader::DisableShader()
{
	// 頂点シェーダ解除
	YsDx.GetDevContext()->VSSetShader(nullptr, 0, 0);
}
