//===============================================================
//! @file YsConstantBuffer.h
//! @brief 定数バッファクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsConstantBuffer_h__
#define __YsConstantBuffer_h__

namespace YskLib
{

//==============================================================
//!
//! @brief コンスタント(定数)バッファ テンプレートクラス
//!
//! [主な機能]														\n
//!	・Create()で指定クラス・構造体をコンスタントバッファとして作成	\n
//!	・WriteData()でバッファに書き込み								\n
//!	・SetVS()やSetPS()などでデバイスへセット						\n
//!
//! @ingroup Graphics_Shader
//==============================================================
template<typename T>
class YsConstantBuffer{
public:

	//! @brief 定数バッファデータ　WriteData関数でこいつの内容が書き込まれる
	T			m_Data;

	//! @brief バッファの種類
	enum class BufType{
		None,			//!< なし
		Default,		//!< GPUからWrite○ Read○　CPUからWrite×Read×　(ただしUpdateSubresource()で更新は可能)　高速
		Dynamic,		//!< GPUからWrite× Read○　CPUからWrite○Read×　頻繁に更新されるようなバッファはこっちの方が効率いいが、Defaultに比べたら少し速度は落ちる
	};

	//! @brief 初期化されているか
	bool IsInit() { return m_pBuffer == nullptr ? false : true; }

	//--------------------------------------------------------
	//! @brief 作成
	//! @param[in] StartSlot : 定数バッファをセットする場所(HLSLのregister(b0)とかの、b0のところの番号)
	//! @param[in] isDynamic : 動的バッファとして作成する(頻繁に更新する場合はこれがいい)
	//! @return 作成成功:true
	//--------------------------------------------------------
	bool Create(UINT StartSlot, bool isDynamic = true);

	//--------------------------------------------------------
	//! @brief 解放
	//--------------------------------------------------------
	void Release();

	//--------------------------------------------------------
	//! @brief m_Dataを定数バッファに書き込む
	//! @param[in]	writeBytes … 書き込むバイト数 -1で全部
	//--------------------------------------------------------
	void WriteData(int writeBytes = -1);

	//--------------------------------------------------------
	//! @brief 頂点シェーダに設定
	//! @param[in] slotNo … 定数バッファをセットする場所(HLSLのregister(b0)とかの、b0のところの番号) -1でCreate()したときに指定した番号
	//--------------------------------------------------------
	void SetVS(int slotNo = -1){
		if(slotNo < 0)slotNo = m_StartSlot;
		YsDx.GetDevContext()->VSSetConstantBuffers(slotNo, 1, &m_pBuffer);
	}

	//--------------------------------------------------------
	//! @brief ピクセルシェーダに設定
	//! @param[in] slotNo … 定数バッファをセットする場所(HLSLのregister(b0)とかの、b0のところの番号) -1でCreate()したときに指定した番号
	//--------------------------------------------------------
	void SetPS(int slotNo = -1){
		if(slotNo < 0)slotNo = m_StartSlot;
		YsDx.GetDevContext()->PSSetConstantBuffers(slotNo, 1, &m_pBuffer);
	}

	//--------------------------------------------------------
	//! @brief ジオメトリシェーダに設定
	//! @param[in] slotNo … 定数バッファをセットする場所(HLSLのregister(b0)とかの、b0のところの番号) -1でCreate()したときに指定した番号
	//--------------------------------------------------------
	void SetGS(int slotNo = -1){
		if(slotNo < 0)slotNo = m_StartSlot;
		YsDx.GetDevContext()->GSSetConstantBuffers(slotNo, 1, &m_pBuffer);
	}

	//--------------------------------------------------------
	//! @brief コンピュートシェーダに設定
	//! @param[in] slotNo … 定数バッファをセットする場所(HLSLのregister(b0)とかの、b0のところの番号) -1でCreate()したときに指定した番号
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
	ID3D11Buffer*		m_pBuffer;		//!< Direct3D11定数バッファ
	int					m_StartSlot;	//!< セットする定数バッファ番号
	BufType				m_TypeFlag;		//!< BufType

private:
	// コピー禁止用
	YsConstantBuffer(const YsConstantBuffer& src) {}
	void operator=(const YsConstantBuffer& src) {}
};

#include "YsConstantBuffer.inl"

}
#endif
