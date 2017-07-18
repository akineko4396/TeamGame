//===============================================================
//! @file YsPixelShader.h
//! @brief ピクセルシェーダクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsPixelShader_h__
#define __YsPixelShader_h__

namespace YskLib
{

//=======================================================================
//! @brief ピクセルシェーダクラス
//!
//! [主な機能]									\n
//!	・HLSLファイルからピクセルシェーダ作成		\n
//!	・それをデバイスにセットする機能			\n
//!
//! @ingroup Graphics_Shader
//=======================================================================
class YsPixelShader {
public:

	//--------------------------------------------------------------------------
	//! @brief HLSLファイルを読み込み、ピクセルシェーダを作成する。
	//! コンパイル済みのシェーダファイルがあれば、それを優先的に読み込む
	//! @param[in] hlslFileName		… HLSLファイルパス
	//! @param[in] VsFuncName		… HLSL内のピクセルシェーダ関数名
	//! @param[in] shaderModel		… 使用するシェーダモデルを指定 例)"ps_5_0"
	//! @param[in] shaderFlag		… 追加のシェーダコンパイルフラグ
	//! @return 成功:true
	//--------------------------------------------------------------------------
	bool Create(const std::string& hlslFileName, const std::string& PsFuncName, const std::string& shaderModel, UINT shaderFlag);

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
	//! @brief ピクセルシェーダを無効にする
	//! SetShader()で最後設定すると有効になる
	//--------------------------------------------------------------------------
	static void DisableShader();

	// 
	YsPixelShader() : m_PS(0), m_pCompiledShader(0){
	}
	~YsPixelShader() {
		Release();
	}

private:
	ID3D11PixelShader*	m_PS;				//!< ピクセルシェーダー

	ID3DBlob*			m_pCompiledShader;	//!< コンパイル済みシェーダデータ

	std::string			m_hlslFileName;		//!< HLSLファイル名
	std::string			m_csoFileName;		//!< コンパイル済みシェーダ(CSO)時のファイル名

private:
	// コピー禁止用
	YsPixelShader(const YsPixelShader& src) {}
	void operator=(const YsPixelShader& src) {}
};

}
#endif
