//===============================================================
//! @file YsGeometryShader.h
//! @brief ジオメトリシェーダクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsGeometryShader_h__
#define __YsGeometryShader_h__

namespace YskLib
{

//=======================================================================
//! @brief ジオメトリシェーダクラス
//!
//! [主な機能]									\n
//!	・HLSLファイルからジオメトリシェーダ作成	n\
//!	・それをデバイスにセットする機能			\n
//!
//! @ingroup Graphics_Shader
//=======================================================================
class YsGeometryShader {
public:

	//--------------------------------------------------------------------------
	//! @brief HLSLファイルを読み込み、ジオメトリシェーダを作成する。
	//! コンパイル済みのシェーダファイルがあれば、それを優先的に読み込む
	//! @param[in] hlslFileName		… HLSLファイルパス
	//! @param[in] VsFuncName		… HLSL内のジオメトリシェーダ関数名
	//! @param[in] shaderModel		… 使用するシェーダモデルを指定 例)"ps_5_0"
	//! @param[in] shaderFlag		… 追加のシェーダコンパイルフラグ
	//! @return 成功:true
	//--------------------------------------------------------------------------
	bool Create(const std::string& hlslFileName, const std::string& GsFuncName, const std::string& shaderModel, UINT shaderFlag);

	//--------------------------------------------------------------------------
	//! @brief コンパイル済みシェーダをファイルに保存する
	//! Create()時に指定した、[hlslFileName]_[VsFuncName].cso のファイル名で保存される
	//! @param[in] filename		… 保存するファイルパス
	//! @return 成功:true
	//--------------------------------------------------------------------------
	bool SaveToFile();


	//--------------------------------------------------------------------------
	//! @brief 解放
	//--------------------------------------------------------------------------
	void Release();

	//--------------------------------------------------------------------------
	//! @brief シェーダをデバイスにセット
	//--------------------------------------------------------------------------
	void SetShader();

	//--------------------------------------------------------------------------
	//! @brief ジオメトリシェーダを無効にする
	//! SetShader()で最後設定すると有効になる
	//--------------------------------------------------------------------------
	static void DisableShader();

	// 
	YsGeometryShader() : m_GS(0), m_pCompiledShader(0){
	}

	~YsGeometryShader() {
		Release();
	}

private:
	ID3D11GeometryShader*	m_GS;			//!< ジオメトリシェーダー

	ID3DBlob*			m_pCompiledShader;	//!< コンパイル済みシェーダデータ

	std::string			m_hlslFileName;		//!< HLSLファイル名
	std::string			m_csoFileName;		//!< コンパイル済みシェーダ(CSO)時のファイル名

private:
	// コピー禁止用
	YsGeometryShader(const YsGeometryShader& src) {}
	void operator=(const YsGeometryShader& src) {}
};

}
#endif
