//===============================================================
//! @file YsDirectXGraphics.h
//! @brief Direct3D関係のクラス
//!
//! @author 鎌田
//===============================================================

#ifndef __YsDirectXGraphics_h__
#define __YsDirectXGraphics_h__

namespace YskLib
{

//================================================
//! @brief Direct3Dクラス
//!
//! Direct3D11の基本的な機能をまとめたクラス
//!
//! @ingroup Graphics_Important
//================================================
class YsDirectXGraphics {
public:

	//==============================================================
	// 取得
	//==============================================================
	//! @brief デバイス取得。リソースの作成およびディスプレイ アダプターの機能の列挙などで使用。
	//! ※参照カウンタは加算していません
	ID3D11Device*			GetDev() { return m_pDevice; }

	//! @brief デバイスコンテキスト取得。描画やステートの変更などで使用。
	//! ※参照カウンタは加算していません
	ID3D11DeviceContext*	GetDevContext(){return m_pDeviceContext;}

	//! @brief スワップチェイン取得。
	//! ※参照カウンタは加算していません
	IDXGISwapChain*			GetSwapChain(){return m_pGISwapChain;}

	//! @brief X解像度取得
	int						GetRezoW(){return (int)m_DXGISwapChainDesc.BufferDesc.Width;}

	//! @brief Y解像度取得
	int						GetRezoH(){return (int)m_DXGISwapChainDesc.BufferDesc.Height;}

	//! @brief バックバッファ取得。
	SPtr<YsTexture>			GetBackBuffer() { return m_texBackBuffer; }

	//! @brief Zバッファ取得。
	SPtr<YsTexture>		GetDepthStencil() { return m_texDepthStencil; }

	//! @brief スプライトクラス取得
	YsSprite&				GetSprite() { return m_Sprite; }


	//! @brief 1x1 白テクスチャ取得
	SPtr<YsTexture>			GetWhiteTex() { return m_texWhite; }

	//! @brief 1x1 Z方向法線マップ取得
	SPtr<YsTexture>			GetNormalTex() { return m_texNormal; }

	//! @brief 通常トゥーンテクスチャ取得
	SPtr<YsTexture>			GetToonTex() { return m_texToon; }
	
	//! @brief リソース倉庫取得
	YsResourceStorage*		GetResStg() { return m_pResStg; }


	//! @brief DirectX9のデバイスを取得。
	//! ※参照カウンタは加算していません
	LPDIRECT3DDEVICE9		GetD3D9Dev(){ return m_pD3D9Device; }

	//! @brief アンチエイリアス用定数
	enum MSAASetting{
		MSAA_None = 1,
		MSAA_x2 = 2,
		MSAA_x4 = 4,
		MSAA_x8 = 8,
	};

	//==============================================================
	// 初期化・解放
	//==============================================================

	//--------------------------------------------------------------------------
	//! @brief 初期設定
	//! Direct3D11を初期化する
	//! @param[in]	hWnd		… ウィンドウのハンドル
	//! @param[in]	w			… 解像度 X
	//! @param[in]	h			… 解像度 Y
	//! @param[in]	resStg		… 登録するリソース管理クラス　YskLib内でテクスチャなどのリソースを読み込む際には、こいつが使用される
	//! @param[in]	msaa		… アンチエイリアスを使うか　テクスチャの形式とかいろいろ面倒になるので使用しないこと推奨　AAは別途シェーダでやるほうがいいと思う
	//! @param[out]	errMsg		… エラーメッセージが返る
	//! @return 成功 … true
	//--------------------------------------------------------------------------
	bool Init(HWND hWnd, int w, int h, YsResourceStorage* resStg, MSAASetting msaa = MSAA_None, std::string* errMsg = nullptr);

	//--------------------------------------------------------------------------
	//! @brief 解放
	//--------------------------------------------------------------------------
	void Release();

	//--------------------------------------------------------------------------
	//! @brief 標準的なステート設定をする
	//--------------------------------------------------------------------------
	void SetDefaultStates();

	//--------------------------------------------------------------------------
	//! @brief フルスクリーン切り替え
	//! @param[in]	bFullScreen	… true:フルスクリーン　false:ウィンドウ
	//--------------------------------------------------------------------------
	void SetFullScreen(bool bFullScreen){
		m_pGISwapChain->SetFullscreenState((BOOL)bFullScreen, 0);
	}

	//--------------------------------------------------------------------------
	//! @brief 解像度変更
	//! @param[in]	w		… 解像度 X
	//! @param[in]	h		… 解像度 Y
	//--------------------------------------------------------------------------
	bool ResizeRezolution(int w, int h);

	//--------------------------------------------------------------------------
	//! @brief ビューポート設定
	//!
	//! w,hに0を指定すると、バックバッファの大きさで設定される。
	//!
	//! @param[in]	w … 幅
	//! @param[in]	h … 高さ
	//--------------------------------------------------------------------------
	void SetViewport(float w = 0, float h = 0);

	//==============================================================
	// ラスタライザステート関係
	//==============================================================

	//! @brief カリングモードの変更
	//! ※ラスタライザステートのカリング以外の項目は基本的なものに設定されます
	//! @param[in]	mode … カリングモード
	//!						- D3D11_CULL_NONE … 両面描画
	//!						- D3D11_CULL_FRONT … 裏面のみ表示
	//!						- D3D11_CULL_BACK … 表面のみ表示
	void RS_CullMode(D3D11_CULL_MODE mode);

	//==============================================================
	// 深度ステンシルステート関係
	//  詳細はYsDepthStencilStateクラスを使用してください
	//==============================================================

	//! @brief Z判定やZ書き込みモードのON/OFF
	//! ※深度ステンシルステートのその他の項目は基本的なものに設定されます
	void DS_ZSetting(bool ZEnable, bool ZWriteEnable);

	//==============================================================
	// 描画関係
	//==============================================================
	//! @brief 2D四角形描画
	// x,y,w,hはピクセル座標系で指定
	void DrawQuad(int x, int y, int w, int h);

	
	void RemoveTextureVS(int slotNo = -1);		//!< 頂点シェーダの指定スロットのテクスチャを解除 -1で全てのスロット解除
	void RemoveTexturePS(int slotNo = -1);		//!< ピクセルシェーダの指定スロットのテクスチャを解除 -1で全てのスロット解除
	void RemoveTextureGS(int slotNo = -1);		//!< ジオメトリシェーダの指定スロットのテクスチャを解除 -1で全てのスロット解除
	void RemoveTextureCS(int slotNo = -1);		//!< コンピュートシェーダの指定スロットのテクスチャを解除 -1で全てのスロット解除

	//! @brief バックバッファを画面に表示
	HRESULT Present(UINT SyncInterval, UINT Flags);

private:

	ID3D11Device*			m_pDevice;			//!< DirectX11の中心になるクラス。全体の管理とバッファ、シェーダ、テクスチャなどのリソース作成などを行う。DX9とは違って、このクラスは描画関係のメンバ関数を持たない。
	ID3D11DeviceContext*	m_pDeviceContext;	//!< 描画処理を行うクラス。
												//!< 内部的には、レンダリングコマンドと呼ばれるバイナリデータを作成し、GPUに送る。GPUがレンダリングコマンドを解析することで実際の描画処理が行われる。
												//!< 複数作成することができるため、レンダリングコマンドを事前に作成したり、スレッドでの並列処理が可能？

	IDXGIAdapter*			m_pGIAdapter;		//!< ディスプレイ サブシステム(1つまたは複数のGPU、DAC、およびビデオ メモリー)。
	IDXGIFactory*			m_pGIFactory;		//!< フルスクリーン切り替えなどで使用。
	IDXGISwapChain*			m_pGISwapChain;		//!< ウインドウへの表示やダブルバッファリングなどを行うクラス。マルチサンプリング、リフレッシュレートなどの設定もできるみたい。
	DXGI_SWAP_CHAIN_DESC	m_DXGISwapChainDesc;//!< スワップチェーンの設定データ

	SPtr<YsTexture>			m_texBackBuffer;	//!< バックバッファ
	SPtr<YsTexture>			m_texDepthStencil;	//!< Zバッファ

	YsSprite				m_Sprite;			//!< 2Dスプライト描画関係

	SPtr<YsTexture>			m_texWhite;			//!< 1x1 白テクスチャ マテリアルのテクスチャが無い時用
	SPtr<YsTexture>			m_texNormal;		//!< 1x1 Z方向法線マップ 法線マップが無い時用
	SPtr<YsTexture>			m_texToon;			//!< data\Texture\toon.png

	YsResourceStorage*		m_pResStg;			//!< リソース倉庫への参照

	YsSamplerState			m_ssLinear_Wrap;	//!< Linear補間 Wrap 
	YsSamplerState			m_ssLinear_Clamp;	//!< Linear補間 Clamp 


	ID3D11ShaderResourceView* m_srvReset[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];	// 全てnullptr　Shader Resource Viewのリセット用

	// DrawQuad用
	YsPolygon				m_polyQuad;

	//=============================================
	// DirectX9(Xファイルの読み込みなどで使用)
	//=============================================
	LPDIRECT3D9				m_pD3D9;			//!< Direct3D9オブジェクト
	LPDIRECT3DDEVICE9		m_pD3D9Device;		//!< Direct3D9デバイス

	bool CreateD3D9Device(HWND hWnd, std::string* errMsg = nullptr);

//==============================================================
// シングルトン
private:
	YsDirectXGraphics() : m_pDevice(0), m_pDeviceContext(0), m_pGIAdapter(0), m_pGIFactory(0), m_pGISwapChain(0), m_pResStg(0),
							m_pD3D9(0), m_pD3D9Device(0)
	{
		ZeroMemory(m_srvReset, sizeof(m_srvReset));
	}

	~YsDirectXGraphics() {
		Release();
	}

public:
	static YsDirectXGraphics& GetInstance() {
		static YsDirectXGraphics Instance;
		return Instance;
	}


//==============================================================
};


}
#endif
