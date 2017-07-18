//===============================================================
//! @file YsStates.h
//! @brief ステートクラス群、レンダーターゲット切り替えクラス
//!
//! ・YsBlendStateクラス		… 半透明や加算合成などを切り替える時に使う		\n
//! ・YsDepthStencilStateクラス	… Zバッファやステンシルバッファの動作設定		\n
//! ・YsSamplerStateクラス		… テクスチャの詳細な使用設定					\n
//! ・YsRasterizeStateクラス	… 3Dから2Dへのレンダリング時の動作設定			\n
//! ・YsRenderTargetsクラス		… MRT(マルチレンダーターゲット)も考慮したRT、Depth操作を行う	\n
//!
//! @author 鎌田
//===============================================================
#ifndef __YsStates_h__
#define __YsStates_h__

namespace YskLib {

//! @defgroup Graphics_States グラフィック関係(ステート、レンダーターゲット系)
//! @{

//================================================
//! @brief ブレンドステートクラス
//!
//!	半透明や加算合成などを切り替える時に使う
//================================================
class YsBlendState {
public:
	//===============================================
	// パラメータ設定系
	//  ※まだデバイスへはセットされません。SetState()でセットされます。
	//  ※また、これらの関数はステートオブジェクトを解放します(再作成が必要なため)
	//===============================================
	//! @brief 現在のパイプラインから取得した情報を使う
	void SetAll_GetState();

	//! @brief  α無し No … -1で全RT(0～7)にセットする
	//! @param[in]	RT_No　			… 設定するレンダーターゲット番号。-1で全RT(0～7)にセットする。
	//! @param[in]	bOverrideAlpha　… 描画先のアルファ値を上書きするか？ trueにすると描画先のアルファに描画元のアルファを上書きする(RTがアルファ値を持ってるテクスチャのみ有効)。
	void Set_NoAlpha(int RT_No = 0, bool bOverrideAlpha = false);
	
	//! @brief  半透明合成 No … -1で全RT(0～7)にセットする
	void Set_Alpha(int RT_No = 0);
	
	//! @brief  加算合成 No … -1で全RT(0～7)にセットする
	void Set_Add(int RT_No = 0);

	//! @brief  各チャンネルの書き込みマスクを設定
	//! @param[in]	RT_No　	… 設定するレンダーターゲット番号。-1で全RT(0～7)にセットする。
	//! @param[in]	r		… 赤成分を書き込むかどうか
	//! @param[in]	g		… 緑成分を書き込むかどうか
	//! @param[in]	b		… 青成分を書き込むかどうか
	//! @param[in]	a		… アルファ成分を書き込むかどうか
	void Set_WriteMask(int RT_No = 0, bool r = true, bool g = true, bool b = true, bool a = true);

	//! @brief アルファトゥカバレッジの設定。αテストみたいなもの。
	void Set_AlphaToCoverageEnable(bool bEnable){
		YsSafe_Release(m_pBlendState);
		m_Desc.AlphaToCoverageEnable = bEnable ? TRUE : FALSE;
	}

	//! @brief MRTで各レンダーターゲットのブレンドステートの設定を個別に設定できる
	//! @param[in] bEnable	…	false:0番目のみが使用される
	//!							true:0～7個別で使用される
	void Set_IndependentBlendEnable(bool bEnable) {
		YsSafe_Release(m_pBlendState);
		m_Desc.IndependentBlendEnable = bEnable ? TRUE : FALSE;
	}

	//! @brief ブレンディング係数設定。RGBA の成分。
	//!　※D3D11_BLEND_BLEND_FACTOR指定時のみ使用。
	void Set_BlendFactor(const float factor[4]){
		YsSafe_Release(m_pBlendState);
		for(int i = 0; i<4; i++){
			m_BlendFactor[i] = factor[i];
		}
	}

	//! @brief すべてのアクティブなレンダーターゲットでどのサンプルが更新されるかが判別されるマスクを設定
	void Set_SampleMask(UINT mask){
		YsSafe_Release(m_pBlendState);
		m_SampleMask = mask;
	}

	//! @brief descの内容をコピーする
	void Set_FromDesc(const D3D11_BLEND_DESC& desc){
		YsSafe_Release(m_pBlendState);
		m_Desc = desc;
	}

	//===============================================
	// 作成・セット
	//===============================================
	//! @brief 現在の設定内容を元に、ステートオブジェクトを作成のみ行う
	//! @return 作成成功:true
	bool Create();

	//! @brief ブレンドステートをデバイスへセットする
	//! ステートオブジェクトが作成されていない場合は、作成する
	void SetState();

	//! @brief 解放
	void Release(){
		YsSafe_Release(m_pBlendState);
	}

	//===============================================
	// 設定情報取得
	//===============================================
	//! @brief 設定情報取得
	const D3D11_BLEND_DESC&	GetDesc() { return m_Desc; }
	//! @brief ステートオブジェクト取得
	ID3D11BlendState*		GetState() { return m_pBlendState; }
	//! @brief BlendFactor取得
	float*					GetBlendFactor() { return m_BlendFactor; }
	//! @brief SampleMasl取得
	UINT					GetSampleMask() { return m_SampleMask; }

	//===============================================
	YsBlendState() : m_pBlendState(0) {
		ZeroMemory(&m_Desc, sizeof(D3D11_BLEND_DESC));
		Set_Alpha(0);
		ZeroMemory(m_BlendFactor, sizeof(m_BlendFactor));
		m_SampleMask = 0xFFFFFFFF;
		Set_WriteMask(-1);
	}
	~YsBlendState() {
		Release();
	}

private:
	// ブレンドステート
	D3D11_BLEND_DESC	m_Desc;				//!< 設定情報
	ID3D11BlendState*	m_pBlendState;		//!< ブレンドステートオブジェクト

	float				m_BlendFactor[4];	//!< BlendFactor
	UINT				m_SampleMask;		//!< SampleMask

private:
	// コピー禁止用
	YsBlendState(const YsBlendState& src) {}
	void operator=(const YsBlendState& src) {}
};


//================================================
//! @brief 深度ステンシルステートクラス
//!
//!　Zバッファやステンシルバッファの動作設定
//================================================
class YsDepthStencilState {
public:
	//===============================================
	// 作成・セット
	//  ※まだデバイスへはセットされません。SetState()でセットされます。
	//  ※また、これらの関数はステートオブジェクトを解放します(再作成が必要なため)
	//===============================================

	//----------------------------------------------------------
	//! @brief 基本的な設定をセット
	//----------------------------------------------------------
	void SetAll_Standard();

	//----------------------------------------------------------
	//! @brief 現在のパイプラインから取得した情報を使う
	//! ※ある条件下では、すべての項目が取得できないことがあります
	//----------------------------------------------------------
	void SetAll_GetState();

	//----------------------------------------------------------
	//! @brief Z判定のON/OFF
	void Set_ZEnable(bool enable){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.DepthEnable = enable ? TRUE : FALSE;
	}
	//! @brief Z書き込みのON/OFF
	void Set_ZWriteEnable(bool enable){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.DepthWriteMask = enable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	}
	//! @brief Z判定比較方法の設定 通常はD3D11_COMPARISON_LESS_EQUALでOK
	void Set_ZFunc(D3D11_COMPARISON_FUNC funcFlag = D3D11_COMPARISON_LESS_EQUAL){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.DepthFunc = funcFlag;
	}

	//! @brief ステンシルテストのON/OFF
	void Set_StencilEnable(bool enable){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.StencilEnable = enable ? TRUE : FALSE;
	}
	//! @brief ステンシルバッファを読み取る部分のマスク設定
	void Set_StencilReadMask(unsigned char mask){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.StencilReadMask = mask;
	}
	//! @brief ステンシルバッファを書き込む部分のマスク設定
	void Set_StencilWriteMask(unsigned char mask){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc.StencilWriteMask = mask;
	}
	//! @brief 深度ステンシル テストの実行時に基準として使用される参照値設定
	//! @param[in]	stencilRef	… ステンシル値
	void Set_StencilRef(UINT stencilRef) {
		YsSafe_Release(m_pDepthStencilState);
		m_StencilRef = stencilRef;
	}

	//! @brief descの内容をコピーする
	void Set_FromDesc(const D3D11_DEPTH_STENCIL_DESC& desc){
		YsSafe_Release(m_pDepthStencilState);
		m_Desc = desc;
	}
	//----------------------------------------------------------

	//===============================================
	// 作成・セット
	//===============================================
	//! @brief 現在のDescの内容を元にステートオブジェクトを作成のみ行う
	//! @return 作成成功:true
	bool Create();

	//! @brief 深度ステンシルステートをデバイスへセット
	//!	ステートオブジェクトが作成されていない場合は、作成する
	void SetState();

	//! @brief 解放
	void Release(){
		YsSafe_Release(m_pDepthStencilState);
	}

	//===============================================
	// 設定情報取得
	//===============================================
	//! @brief 設定情報取得
	const D3D11_DEPTH_STENCIL_DESC&	GetDesc() { return m_Desc; }
	//! @brief 深度ステンシルステートオブジェクト取得
	ID3D11DepthStencilState*		GetState() { return m_pDepthStencilState; }
	//! @brief シテンシル値取得
	UINT							GetStencilRef() { return m_StencilRef; }

	//===============================================
	YsDepthStencilState() : m_pDepthStencilState(0), m_StencilRef(0) {
	}

	~YsDepthStencilState() {
		Release();
	}

private:
	D3D11_DEPTH_STENCIL_DESC	m_Desc;					//!< 設定情報
	ID3D11DepthStencilState*	m_pDepthStencilState;	//!< ステートオブジェクト
	UINT						m_StencilRef;			//!< ステンシル値

private:
	// コピー禁止用
	YsDepthStencilState(const YsDepthStencilState& src) {}
	void operator=(const YsDepthStencilState& src) {}
};

//================================================
//! @brief サンプラーステートクラス
//!
//!　テクスチャの詳細な使用設定
//================================================
class YsSamplerState {
public:
	//===============================================
	// 作成・セット
	//  ※まだデバイスへはセットされません。SetState()でセットされます。
	//  ※また、これらの関数はステートオブジェクトを解放します(再作成が必要なため)
	//===============================================

	//----------------------------------------------------------
	//! @brief 標準的なサンプラ作成
	//----------------------------------------------------------
	void SetAll_Standard();

	//----------------------------------------------------------
	//! @brief ピクセルシェーダパイプラインから現在の設定情報を取得
	//! @param[in]	StartSlot	… 設定を取得したいスロット番号
	void SetAll_GetStatePS(UINT StartSlot);
	//! @brief 頂点シェーダパイプラインから現在の設定情報を取得
	//! @param[in]	StartSlot	… 設定を取得したいスロット番号
	void SetAll_GetStateVS(UINT StartSlot);
	//! @brief ジオメトリシェーダパイプラインから現在の設定情報を取得
	//! @param[in]	StartSlot	… 設定を取得したいスロット番号
	void SetAll_GetStateGS(UINT StartSlot);
	//! @brief コンピュータシェーダパイプラインから現在の設定情報を取得
	//! @param[in]	StartSlot	… 設定を取得したいスロット番号
	void SetAll_GetStateCS(UINT StartSlot);
	//----------------------------------------------------------

	//----------------------------------------------------------
	//! @brief テクスチャ アドレッシングモードをWRAPに設定
	void Set_Wrap();

	//! @brief テクスチャ アドレッシングモードをCLAMPに設定
	void Set_Clamp();
	//----------------------------------------------------------

	//----------------------------------------------------------
	//! @brief フィルタ設定
	//! @param[in]	filter	… フィルタ
	void Set_Filter(D3D11_FILTER filter, int maxAnisotropy = 1);
	void Set_Filter_Linear();		//!< MIN MAG MIP を全てLinearとして設定
	void Set_Filter_Point();		//!< MIN MAG MIP を全てPointとして設定
	void Set_Filter_Anisotropy(UINT MaxAnisotropy = 16);	//!< 異方性フィルタ設定
	//----------------------------------------------------------

	//! @brief 比較モード設定
	//! Percent Closer Filter(PCF)のソフトシャドウ表現が可能になる。\n
	//! ※HLSL側のサンプラはSamplerComparisonStateで用意し、Texture2DのSample()ではなく、SampleCmp()かSampleCmpLevelZero()を使うこと。
	//! @param[in]	compFunc	… D3D11_COMPARISON系のフラグを指定
	void Set_ComparisonMode(D3D11_COMPARISON_FUNC compFunc = D3D11_COMPARISON_LESS_EQUAL);

	//! @brief ミップマップレベルのバイアスを設定する
	//! ミップマップをよりかからないようにしたい場合はマイナスの値を指定する。より強くかけたい場合はプラス値を指定する。
	//! @param[in]	bias … 計算されたミップマップ レベルからのオフセット
	void Set_MipLODBias(float bias){
		m_Desc.MipLODBias = bias;
	}
	//! @brief ミップマップレベルの範囲を設定
	//! 例えばミップマップをかけたくない場合は、fMin=0 fMax=0)とかにする。
	//! @param[in]	fMin	… ミップマップ範囲の下限
	//! @param[in]	fMax	… ミップマップ範囲の上限
	void Set_MipLOD(float fMin, float fMax){
		m_Desc.MinLOD = fMin;
		m_Desc.MaxLOD = fMax;
	}

	//! @brief descの内容をコピーする
	void Set_FromDesc(const D3D11_SAMPLER_DESC& desc) {
		YsSafe_Release(m_pSampler);
		m_Desc = desc;
	}


	//===============================================
	// 作成・セット
	//===============================================
	//! @brief 現在のDescの内容を元にサンプラ作成
	//! @return 作成成功:true
	bool Create();

	//! @brief サンプラをピクセルシェーダーのパイプライン ステージにセットする。
	//! ステートオブジェクトが作成されていない場合は、作成する
	//! @param[in]	StartSlot	… セットするスロット番号
	void SetStatePS(UINT StartSlot = 0);

	//! @brief サンプラを頂点シェーダーのパイプライン ステージにセットする。
	//! ステートオブジェクトが作成されていない場合は、作成する
	//! @param[in]	StartSlot	… セットするスロット番号
	void SetStateVS(UINT StartSlot = 0);
	
	//! @brief サンプラをジオメトリシェーダーのパイプライン ステージにセットする。
	//! ステートオブジェクトが作成されていない場合は、作成する
	//! @param[in]	StartSlot	… セットするスロット番号
	void SetStateGS(UINT StartSlot = 0);
	
	//! @brief サンプラをコンピュートシェーダーのパイプライン ステージにセットする。
	//! ステートオブジェクトが作成されていない場合は、作成する
	//! @param[in]	StartSlot	… セットするスロット番号
	void SetStateCS(UINT StartSlot = 0);

	//===============================================
	//! @brief 解放
	//===============================================
	void Release(){
		YsSafe_Release(m_pSampler);
	}

	//===============================================
	// 設定情報取得
	//===============================================
	//! @brief 設定情報取得
	const D3D11_SAMPLER_DESC&	GetDesc() { return m_Desc; }
	//! @brief ステートオブジェクト取得
	ID3D11SamplerState*			GetState() { return m_pSampler; }

	//===============================================
	YsSamplerState() : m_pSampler(0) {
	}
	~YsSamplerState() {
		Release();
	}

private:
	D3D11_SAMPLER_DESC		m_Desc;			//!< 設定情報
	ID3D11SamplerState*		m_pSampler;		//!< ステートオブジェクト

private:
	// コピー禁止用
	YsSamplerState(const YsSamplerState& src) {}
	void operator=(const YsSamplerState& src) {}
};

//================================================
//! @brief ラスタライザステートクラス
//!
//!　3Dから2Dへのレンダリング時の動作設定
//================================================
class YsRasterizeState {
public:
	//===============================================
	// 作成・セット
	//  ※まだデバイスへはセットされません。SetState()でセットされます。
	//  ※また、これらの関数はステートオブジェクトを解放します(再作成が必要なため)
	//===============================================

	//----------------------------------------------------------
	//! @brief 標準的な情報作成
	//----------------------------------------------------------
	void SetAll_Standard();

	//----------------------------------------------------------
	//! @brief 現在のパイプラインから取得した情報を使う
	//----------------------------------------------------------
	void SetAll_GetState();

	//----------------------------------------------------------
	//! @brief カリングモード設定：表面のみ描画
	void Set_CullMode_Back() {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = D3D11_CULL_BACK;
	}
	//! @brief カリングモード設定：裏面のみ描画
	void Set_CullMode_Front() {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = D3D11_CULL_FRONT;
	}
	//! @brief カリングモード設定：両面描画
	void Set_CullMode_None() {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = D3D11_CULL_NONE;
	}
	//! @brief カリングモード設定
	void Set_CullMode(D3D11_CULL_MODE mode) {
		YsSafe_Release(m_pRasterize);
		m_Desc.CullMode = mode;
	}

	//! @brief 通常
	void Set_FillMode_Solid() {
		YsSafe_Release(m_pRasterize);
		m_Desc.FillMode = D3D11_FILL_SOLID;
	}
	//! @brief ワイヤーフレーム
	void Set_FillMode_Wireframe() {
		YsSafe_Release(m_pRasterize);
		m_Desc.FillMode = D3D11_FILL_WIREFRAME;
	}

	//! @brief descの内容をコピーする
	void Set_FromDesc(const D3D11_RASTERIZER_DESC& desc){
		YsSafe_Release(m_pRasterize);
		m_Desc = desc;
	}
	//----------------------------------------------------------

	//===============================================
	// 作成・セット
	//===============================================
	//! @brief 現在のDescの内容を元にサンプラ作成
	//! @return 作成成功:true
	bool Create();

	//! @brief ラスタライザーステートをデバイスへセット
	//!  ステートオブジェクトが作成されていない場合は、作成する
	void SetState();

	//! @brief 解放
	void Release(){
		YsSafe_Release(m_pRasterize);
	}

	//===============================================
	// 設定情報取得
	//===============================================
	//! @brief 設定情報取得
	const D3D11_RASTERIZER_DESC&	GetDesc() { return m_Desc; }
	//! @brief ラスタライザーステートオブジェクト取得
	ID3D11RasterizerState*			GetState() { return m_pRasterize; }

	//===============================================
	YsRasterizeState() : m_pRasterize(0) {
	}

	~YsRasterizeState() {
		Release();
	}

private:
	D3D11_RASTERIZER_DESC	m_Desc;			//!< 設定情報
	ID3D11RasterizerState*	m_pRasterize;	//!< ステートオブジェクト

private:
	// コピー禁止用
	YsRasterizeState(const YsRasterizeState& src) {}
	void operator=(const YsRasterizeState& src) {}
};

//================================================================
//! @brief レンダーターゲット、Zバッファの切り替え操作クラス
//!
//!	MRT(マルチレンダーターゲット)も考慮したRT、Depth操作を行う
//!
//================================================================
class YsRenderTargets {
public:
	//====================================================
	// 設定
	//====================================================

	//----------------------------------------------------------
	// 登録系

	//! @brief 指定Noのレンダーターゲットとして登録(デバイスにはまだセットしない)
	//! @param[in]	rtNo	… セットするレンダーターゲット番号
	//! @param[in]	rtTex	… このテクスチャ内のRenderTargerViewを登録する
	void RT(int rtNo, YsTexture& rtTex);

	//! @brief 指定Noのレンダーターゲットとして登録(デバイスにはまだセットしない)
	//! @param[in]	rtNo	… セットするレンダーターゲット番号
	//! @param[in]	rtView	… このRenderTargerViewを登録する
	void RT(int rtNo, ID3D11RenderTargetView* rtView);	// View指定Ver

	//! @brief rtdataに登録されている、指定Noのレンダーターゲットを取得し登録(デバイスにはまだセットしない)
	void RT(int rtNo, YsRenderTargets& rtdata);

	//! @brief 深度ステンシルを登録(デバイスにはまだセットしない)
	//! @param[in]	depthTex	… このテクスチャ内のDepthStencilViewを登録する
	void Depth(YsTexture& depthTex);

	//! @brief 深度ステンシルを登録(デバイスにはまだセットしない)
	//! @param[in]	depthView	… このDepthStencilViewを登録する
	void Depth(ID3D11DepthStencilView* depthView);	// View指定Ver

	//! @brief 深度ステンシルを登録(デバイスにはまだセットしない)
	//! @param[in]	rtdata	… このYsRenderTargetsに登録されているDepthを登録
	void Depth(YsRenderTargets& rtdata);
	//----------------------------------------------------------

	//----------------------------------------------------------
	// デバイスから取得系

	//! @brief 現在デバイスにセットされている全てのRTとDepthStencilを、このクラスに取得する
	void GetNowAll();

	//! @brief 現在デバイスにセットされている全てのRTを、このクラスに取得する
	void GetNowRTs();

	//! @brief 現在デバイスにセットされているDepthStencilを、このクラスに取得する
	void GetNowDepth();

	//----------------------------------------------------------

	//! @brief 登録されている情報を全て解除
	void Release();


	//====================================================
	// デバイスへセット
	//====================================================

	//! @brief このクラスに登録されているRT､Depthを、実際にデバイスへセットする(RT、Depthを実際に切り替えること)
	//! ※0番目のRTのサイズでビューポートも変更しています。
	//! @param[in] setRTNum		: セットするRTの数。-1ですべてセット(8枚)。例えば 2 だと、0と1の2枚がセットされる。
	void SetToDevice(int setRTNum = -1);

	//====================================================
	// 取得
	//====================================================

	//! @brief 現在このクラスに登録されているRTを取得(デバイスのRT取得ではありません)
	//! ※戻り値のビューは、参照カウンタをAddRefしません
	//! @param[in] rtNo	… RT番号(0～7)
	//! @return このクラスに登録されているrtNo番目のレンダーターゲットビュー
	ID3D11RenderTargetView* GetRT(int rtNo) { return m_pRTs[rtNo]; }

	//! @brief 現在このクラスに登録されているDepthを取得(デバイスのDepth取得ではありません)
	//! ※戻り値のビューは、参照カウンタをAddRefしません
	//! @return このクラスに登録されているidx番目のデプスステンシルビュー
	ID3D11DepthStencilView* GetDepth() { return m_pDepth; }

	//! @brief 指定RTの情報取得
	//! @param[in]	rtNo	… 情報を取得したいRT番号
	//! @param[out]	outDesc	… 情報を入れるための変数
	//! @return true:正常に取得できた
	bool GetRTDesc(int rtNo, D3D11_TEXTURE2D_DESC& outDesc);


	//====================================================

	// 
	YsRenderTargets() {
		ZeroMemory(m_pRTs, sizeof(m_pRTs));
		m_pDepth = nullptr;
	}
	// 
	~YsRenderTargets() {
		Release();
	}

	// コピー
	YsRenderTargets(const YsRenderTargets& src) : YsRenderTargets(){
		*this = src;
	}
	void operator=(const YsRenderTargets& src);

private:
	ID3D11RenderTargetView*		m_pRTs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];	//!< RT設定情報記憶用
	ID3D11DepthStencilView*		m_pDepth;										//!< Depth設定情報記憶用
	D3D11_VIEWPORT				m_VP;											//!< ビューポート取得用

};


//! @}

}

#endif
