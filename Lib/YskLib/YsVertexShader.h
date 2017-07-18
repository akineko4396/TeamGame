//===============================================================
//! @file YsVertexShader.h
//! @brief 頂点シェーダクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsVertexShader_h__
#define __YsVertexShader_h__

namespace YskLib
{

//=======================================================================
//! @brief 頂点シェーダクラス
//!
//! [主な機能]								\n
//!	・HLSLファイルから頂点シェーダ作成		\n
//!	・頂点レイアウト作成					\n
//!	・それらをデバイスにセットする機能		\n
//!
//! @ingroup Graphics_Shader
//=======================================================================
class YsVertexShader{
public:

	//--------------------------------------------------------------------------
	//! @brief HLSLファイルを読み込み、頂点シェーダ・頂点レイアウトを作成する。
	//! コンパイル済みのシェーダファイルがあれば、それを優先的に読み込む
	//! @param[in] hlslFileName		… HLSLファイルパス
	//! @param[in] VsFuncName		… HLSL内の頂点シェーダ関数名
	//! @param[in] shaderModel		… 使用するシェーダモデルを指定 例)"vs_5_0"
	//! @param[in] shaderFlag		… 追加のシェーダコンパイルフラグ
	//! @param[in] vertexTypeData	… 頂点レイアウト用のデータ　頂点シェーダの入力にあった形式を渡すこと
	//! @return 成功:true
	//--------------------------------------------------------------------------
	bool Create(const std::string& hlslFileName, const std::string& VsFuncName, const std::string& shaderModel, UINT shaderFlag, const YsVertexTypeData& vertexTypeData);

	//--------------------------------------------------------------------------
	//! @brief この頂点シェーダ用の頂点レイアウトを作成する。Create()した後に呼ぶこと。
	//! @param[in] vertexTypeData	… 頂点レイアウト用のデータ　頂点シェーダの入力にあった形式を渡すこと
	//! @return 成功：作成されたID3D11InputLayoutのアドレス ※使わなくなったらRelease()を呼ぶこと
	//--------------------------------------------------------------------------
	ID3D11InputLayout* CreateVertexLayout(const YsVertexTypeData& vertexTypeData);

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
	//! @brief シェーダをデバイスにセット(頂点レイアウトもセット)
	//! @param[in] vertexLayout		… 頂点レイアウト　NULLだとInit()時に指定したものが使用される
	//--------------------------------------------------------------------------
	void SetShader(ID3D11InputLayout* vertexLayout = nullptr);

	//--------------------------------------------------------------------------
	//! @brief 頂点シェーダを無効にする
	//! SetShader()で最後設定すると有効になる
	//--------------------------------------------------------------------------
	static void DisableShader();

	//
	YsVertexShader() : m_VS(0), m_VLayout(0), m_pCompiledShader(0){
	}
	~YsVertexShader(){
		Release();
	}

private:
	ID3D11VertexShader*	m_VS;				//!< 頂点シェーダー
	ID3D11InputLayout*	m_VLayout;			//!< 頂点インプットレイアウト

	ID3DBlob*			m_pCompiledShader;	//!< コンパイル済みシェーダデータ

	std::string			m_hlslFileName;		//!< HLSLファイル名
	std::string			m_csoFileName;		//!< コンパイル済みシェーダ(CSO)時のファイル名

private:
	// コピー禁止用
	YsVertexShader(const YsVertexShader& src) {}
	void operator=(const YsVertexShader& src) {}
};



}

#endif
