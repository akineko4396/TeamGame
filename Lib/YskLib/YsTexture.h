//===============================================================
//! @file YsTexture.h
//! @brief テクスチャクラス
//!
//! @author 鎌田
//===============================================================
#ifndef __YsTexture_h__
#define __YsTexture_h__

namespace YskLib
{

//===========================================================================
//!
//! @brief テクスチャリソースクラス
//!
//!  ※以下のものが取り扱える
//!   - シェーダリソース(通常のテクスチャ)
//!   - レンダーターゲット(描画先として設定可能なRenderTargetテクスチャ)
//!   - Zバッファ(Zバッファとステンシルバッファ)
//!   - アンオーダードアクセステクスチャ
//!   .
//!  ※その他
//!   - MSAA(マルチサンプル アンチエイリアシング)の設定は、バックバッファの設定を使用します
//!
//!  ※Direct3D11などでは、テクスチャ配列というものが使用できる
//!    一言でいうと、１つのリソースデータの中に複数の画像が入る感じ(Direct3D9でいうキューブマップのようなもの)
//!
//! @ingroup Graphics_Important
//===========================================================================
class YsTexture : public YsIResource {
public:
	//===================================================================
	// 読み込み・作成
	//===================================================================

	//--------------------------------------------------------------------------
	//! @brief 画像ファイル読み込み
	//! 基本的な設定で読み込む、ShaderResourceViewのみを作成します
	//! @param[in] filename		… 読み込む画像ファイル名
	//! @return  true:成功
	//--------------------------------------------------------------------------
	bool LoadTexture(const std::string& filename);

	//--------------------------------------------------------------------------
	//! @brief 画像ファイル読み込み
	//! 詳細設定で読み込む、ShaderResourceViewのみを作成します
	//! @param[in] filename		… 読み込む画像ファイル名
	//! @param[in] MipLevels	… 作成するミップマップ数。0ですべてのミップマップを作成する。1だと1枚のみ(ミップマップ作成しない)。
	//! @param[in] format		… 読み込んだテクスチャの形式を指定する。DXGI_FORMAT_FROM_FILEでファイルから形式を取得。
	//! @param[in] bindFlag		… テクスチャーをバインドできるようにするパイプライン ステージ
	//! @return  true:成功
	//--------------------------------------------------------------------------
	bool LoadTextureEx(const std::string& filename, UINT MipLevels = 0, DXGI_FORMAT format = DXGI_FORMAT_FROM_FILE, UINT bindFlag = D3D11_BIND_SHADER_RESOURCE);

	//--------------------------------------------------------------------------
	//! @brief 画像ファイル読み込み
	//! 詳細設定で読み込む、ShaderResourceViewのみを作成します
	//! @param[in] filename		… 読み込む画像ファイル名
	//! @param[in] loadInfo		… 読み込み詳細設定
	//! @return  true:成功
	//--------------------------------------------------------------------------
	bool LoadTextureEx(const std::string& filename, D3DX11_IMAGE_LOAD_INFO* loadInfo);

	//--------------------------------------------------------------------------
	//! @brief 通常テクスチャ作成(ShaderResourceViewのみの作成)
	//! テクスチャリソースを作成し、ShaderResourceViewのみを作成します
	//! @param[in] w			… 画像の幅(ピクセル)
	//! @param[in] h			… 画像の幅(ピクセル)
	//! @param[in] format		… 画像の形式
	//! @param[in] fillData		… バッファに書き込むデータ。w*h*formatのサイズで持っていくこと。nullptrだと書き込みなし。
	//! @return true:成功
	//--------------------------------------------------------------------------
	bool Create(int w, int h, DXGI_FORMAT format, const void* fillData = nullptr);

	//--------------------------------------------------------------------------
	//! @brief レンダーターゲットテクスチャとして作成(RenderTargetViewとShaderResourceViewを作成する)
	//! テクスチャリソースを作成し、RenderTargetViewとShaderResourceViewの2種類を作成します
	//! @param[in] w			… 画像の幅(ピクセル)
	//! @param[in] h			… 画像の幅(ピクセル)
	//! @param[in] format		… 画像の形式
	//! @param[in] arrayCnt		… テクスチャ配列を使用する場合、その数。1で通常の1枚テクスチャ
	//! @param[in] fillData		… バッファに書き込むデータ。w*h*formatのサイズで持っていくこと。nullptrだと書き込みなし。
	//! @return true:成功
	//--------------------------------------------------------------------------
	bool CreateRT(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const void* fillData = nullptr);

	//--------------------------------------------------------------------------
	//! @brief レンダーターゲットテクスチャとして作成(キューブマップ用)(RenderTargetViewとShaderResourceViewを作成する)
	//!
	//! キューブマップは、テクスチャ配列6枚と同じ。ShaderResourceViewにD3D11_SRV_DIMENSION_TEXTURECUBEのフラグが付く
	//!
	//! @param[in] w			… 画像の幅(ピクセル)
	//! @param[in] h			… 画像の幅(ピクセル)
	//! @param[in] format		… 画像の形式
	//! @return true:成功
	//--------------------------------------------------------------------------
	bool CreateCubeRT(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	//===================================================================
	//! @brief アンオーダードアクセステクスチャとして作成(UnorderedAccessViewとShaderResourceViewを作成する)
	//! @param[in] w			… 画像の幅(ピクセル)
	//! @param[in] h			… 画像の幅(ピクセル)
	//! @param[in] format		… 画像の形式
	//! @param[in] arrayCnt		… テクスチャ配列を使用する場合、その数。1で通常の1枚テクスチャ
	//! @param[in] fillData		… バッファに書き込むデータ。w*h*formatのサイズで持っていくこと。nullptrだと書き込みなし。
	//! return true:成功
	//--------------------------------------------------------------------------
	bool CreateUAV(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const void* fillData = nullptr);

	//===================================================================

	//--------------------------------------------------------------------------
	//! @brief Zバッファテクスチャとして作成(DepthStencilViewとShaderResourceViewを作成する)
	//! テクスチャリソースを作成し、DepthStencilViewとShaderResourceViewの2種類を作成します
	//! @param[in] w			… 画像の幅(ピクセル)
	//! @param[in] h			… 画像の幅(ピクセル)
	//! @param[in] format		… 画像の形式
	//! @param[in] arrayCnt	… テクスチャ配列を使用する場合、その数。1で通常の1枚テクスチャ
	//! @param[in] fillData	… バッファに書き込むデータ。w*h*formatのサイズで持っていくこと。nullptrだと書き込みなし。
	//! @return true:成功
	//--------------------------------------------------------------------------
	bool CreateDepth(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS, UINT arrayCnt = 1, const void* fillData = nullptr);

	//--------------------------------------------------------------------------
	//! @brief 直接Texture2Dリソースをセット
	//! 引数で渡したテクスチャリソースに最適なビューを作成する
	//! @param[in]  pTexture2D	… セットしたいテクスチャ
	//! @return true:成功
	//--------------------------------------------------------------------------
	bool CreateFromTexture2D(ID3D11Texture2D* pTexture2D);

	private:
	//! @brief m_pTexture2Dの設定を元に、各ビューを作成する
	bool CreateViewsFromTexture2D();
	public:

	//--------------------------------------------------------------------------
	//! @brief 解放
	//--------------------------------------------------------------------------
	void Release();

	//===================================================================
	// クリア関係
	//===================================================================

	//--------------------------------------------------------------------------
	//! @brief RenderTargetをクリア
	//! ※RenderTargetViewを作成する必要があります
	//! ※テクスチャ配列の場合は、全部のテクスチャがクリアされる
	//! @param[in]	ColorRGBA	… クリアする色
	//--------------------------------------------------------------------------
	void ClearRT(const YsVec4& ColorRGBA);

	//--------------------------------------------------------------------------
	//! @brief テクスチャ配列の複数RenderTargetのうち、１つだけをクリア
	//! テクスチャ配列で作成したRenderTargetの、指定番目のものだけをクリアできる。
	//! テクスチャ配列として作成していなければこの関数は意味ないです。
	//! @param[in]	idx			… テクスチャ配列の番号
	//! @param[in]	ColorRGBA	… クリアする色
	//--------------------------------------------------------------------------
	void ClearRTArray(int idx, const YsVec4& ColorRGBA);

	//--------------------------------------------------------------------------
	//! @brief DepthStencilをクリア
	//! ※DepthStencilViewを作成する必要があります
	//! ※テクスチャ配列の場合は、全部のテクスチャがクリアされる
	//! @param[in] bDepth		… Zバッファクリアする？
	//! @param[in] bStencil		… ステンシルバッファクリアする？
	//! @param[in] depth		… クリアするZバッファの値
	//! @param[in] stencil		… クリアするステンシルバッファの値
	//--------------------------------------------------------------------------
	void ClearDepth(bool bDepth = true, bool bStencil = true, FLOAT depth = 1.0f, UINT8 stencil = 0);

	//--------------------------------------------------------------------------
	//! @brief テクスチャ配列の複数DepthStencilのうち、１つだけをクリア
	//! テクスチャ配列で作成したDepthStencilの、指定番目のものだけをクリアできる。
	//! テクスチャ配列として作成していなければこの関数は意味ないです。
	//! ※DepthStencilViewを作成する必要があります
	//! @param[in] bDepth		… Zバッファクリアする？
	//! @param[in] bStencil		… ステンシルバッファクリアする？
	//! @param[in] depth		… クリアするZバッファの値
	//! @param[in] stencil		… クリアするステンシルバッファの値
	//--------------------------------------------------------------------------
	void ClearDepthArray(int idx, bool bDepth = true, bool bStencil = true, FLOAT depth = 1.0f, UINT8 stencil = 0);

	//===================================================================
	// 取得・設定
	//===================================================================

	//! @brief 現在読み込んでいるファイル名取得
	const std::string&			GetFileName() { return m_strFileName; }

	//! @brief テクスチャリソース取得(※戻り値のビューの参照カウンタはAddRefしません)
	ID3D11Texture2D*			GetTexture2D(){ return m_pTexture2D; }

	//! @brief ShaderResourceビュー取得(※戻り値のビューの参照カウンタはAddRefしません)
	ID3D11ShaderResourceView*	GetTex() { return m_pTexView; }

	//! @brief テクスチャ配列用 ShaderResourceビュー取得(※戻り値のビューの参照カウンタはAddRefしません)
	//! ※テクスチャ配列として作成しない場合のみ使用可能
	ID3D11ShaderResourceView*	GetTexArray(int no) { return m_pTexViewArray[no]; }



	//! @brief RenderTargetビュー取得(※戻り値のビューの参照カウンタはAddRefしていません)
	ID3D11RenderTargetView*		GetRTTex(){ return m_pTexRTView; }

	//! @brief テクスチャ配列用 RenderTargetビュー取得(※戻り値のビューの参照カウンタはAddRefしません)
	//! ※テクスチャ配列として作成しない場合のみ使用可能
	ID3D11RenderTargetView*		GetRTTexArray(int no) { return m_pTexRTViewArray[no]; }



	//! @brief UnorderedAccessView取得(※戻り値のビューの参照カウンタはAddRefしません)
	ID3D11UnorderedAccessView*	GetUATex(){ return m_pTexUAView; }



	//! @brief DepthStencilビュー取得(※戻り値のビューの参照カウンタはAddRefしません)
	ID3D11DepthStencilView*		GetDepthTex(){ return m_pTexDepthView; }	
	
	//! @brief テクスチャ配列用 DepthStencilビュー取得(※戻り値のビューの参照カウンタはAddRefしません)
	//! ※テクスチャ配列として作成しない場合のみ使用可能
	ID3D11DepthStencilView*		GetDepthTexArray(int no) { return m_pTexDSViewArray[no]; }



	//! @brief 画像情報取得
	const D3D11_TEXTURE2D_DESC&	GetInfo() const{ return m_Desc; }		// 情報

	//! @brief 画像の幅・高のビューポートに変更する
	void SetViewport() const;
	
	//===================================================================
	// 各シェーダーステージにテクスチャをセット
	//  SlotNo	… HLSL側のregister(t0)の番号にあたる　範囲は0～127
	//===================================================================

	//! @brief ピクセルシェーダステージにテクスチャをセットする
	void SetTexturePS(UINT SlotNo) const;
	
	//! @brief 頂点シェーダステージにテクスチャをセットする
	void SetTextureVS(UINT SlotNo) const;
	
	//! @brief ジオメトリシェーダステージにテクスチャをセットする
	void SetTextureGS(UINT SlotNo) const;
	
	//! @brief コンピュートシェーダステージにテクスチャをセットする
	void SetTextureCS(UINT SlotNo) const;

	//===================================================================
	// その他
	//===================================================================

	//! @brief リソース情報
	virtual std::string GetTypeName() const override {
		std::string str = "YsTexture : ";
		str += m_strFileName;
		return str;
	}

	// 
	YsTexture() : m_pTexture2D(0), m_pTexView(0), m_pTexRTView(0), m_pTexUAView(0), m_pTexDepthView(0)
	{
		ZeroMemory( &m_Desc, sizeof( m_Desc ) );
	}

	~YsTexture(){
		Release();
	}


private:

	std::string					m_strFileName;		//!< ファイル名(LoadTexture()した場合のみ)

	ID3D11Texture2D*			m_pTexture2D;		//!< テクスチャリソース(画像データ本体)

	ID3D11ShaderResourceView*	m_pTexView;			//!< シェーダリソースビュー(m_pTexture2Dから色を読み込むためのもの)
	ID3D11RenderTargetView*		m_pTexRTView;		//!< レンダーターゲットビュー(m_pTexture2Dへ書き込みを行うためのもの)
	ID3D11UnorderedAccessView*	m_pTexUAView;		//!< アンオーダードアクセスビュー
	ID3D11DepthStencilView*		m_pTexDepthView;	//!< 深度ステンシルビュー(m_pTexture2DをZバッファとして書き込むためのもの)

	//! SRVテクスチャ配列用
	std::vector<ID3D11ShaderResourceView*>	m_pTexViewArray;
	//! SRVテクスチャ配列解放
	void ClearSRVArray(){
		for(auto& val : m_pTexViewArray){
			YsSafe_Release(val);
		}
		m_pTexViewArray.clear();
		m_pTexViewArray.shrink_to_fit();
	}

	//! RTVテクスチャ配列用
	std::vector<ID3D11RenderTargetView*>	m_pTexRTViewArray;
	//! RTVテクスチャ配列解放
	void ClearRTVArray(){
		for(auto& val : m_pTexRTViewArray){
			YsSafe_Release(val);
		}
		m_pTexRTViewArray.clear();
		m_pTexRTViewArray.shrink_to_fit();
	}

	//! DSVテクスチャ配列用
	std::vector<ID3D11DepthStencilView*>	m_pTexDSViewArray;
	//! DSVテクスチャ配列解放
	void ClearDSVArray(){
		for(auto& val : m_pTexDSViewArray){
			YsSafe_Release(val);
		}
		m_pTexDSViewArray.clear();
		m_pTexDSViewArray.shrink_to_fit();
	}

	D3D11_TEXTURE2D_DESC		m_Desc;				//!< 情報

private:
	// コピー禁止用
	YsTexture(const YsTexture& src) {}
	void operator=(const YsTexture& src) {}
};

// formatメモ
// _FLOAT … 浮動小数点型。floatみたいな。
// _SINT  … 符号付き整数型。intみたいな。
// _UINT  … 符号なし整数型。unsigned intみたいな。
// _SNORM … 符号付き正規化整数型。つまり-1.0～1.0で扱われる。
// _UNORM … 符号なし正規化整数型。つまり0.0～1.0で扱われる。
// _TYPELESS … 型なしデータ型。RTとかには使えないっぽい。Depthはこれじゃないといけないぽい。



//================================================================================================
//! @brief テクスチャセットクラス
//! 
//! 単純に複数のテクスチャ(YsTexture)を１つの構造体にまとめたもの \n
//! LoadTexture関数により、ファイル名をキーに一気に読み込まれる \n
//! シェーダで高品質な表現を行うのに便利						\n
//!																\n
//! 例)LoadTexture("aaa.png");		\n
//!			↓
//!　　・MeshTex   … aaa.pngが読み込まれる\n
//!　　・NormalTex … aaa.normal.pngが読み込まれる\n
//!　　・ExtTex    … aaa.ext.pngが読み込まれる\n
//!　　・ExtTex2   … aaa.ext2.pngが読み込まれる\n
//!　　・ExtTex3   … aaa.ext3.pngが読み込まれる\n
//!　　・ExtTex4   … aaa.ext4.pngが読み込まれる\n
//!　　・ExtTex5   … aaa.ext5.pngが読み込まれる\n
//!　　・ExtTex6   … aaa.ext6.pngが読み込まれる\n
//!　　・ExtTex7   … aaa.ext7.pngが読み込まれる\n
//!　　・ExtTex8   … aaa.ext8.pngが読み込まれる\n
//!　※ファイル名と拡張子の間に、上記のような名前を挿入したファイルを読み込むようになる\n
//!
//! @ingroup Graphics_Important
//================================================================================================
struct YsTextureSet {
	SPtr<YsTexture>	MeshTex;	//!< デカールテクスチャ
	SPtr<YsTexture>	NormalTex;	//!< 法線テクスチャ
	SPtr<YsTexture>	ExtTex;		//!< その他拡張テクスチャ
	SPtr<YsTexture>	ExtTex2;	// …
	SPtr<YsTexture>	ExtTex3;
	SPtr<YsTexture>	ExtTex4;
	SPtr<YsTexture>	ExtTex5;
	SPtr<YsTexture>	ExtTex6;
	SPtr<YsTexture>	ExtTex7;
	SPtr<YsTexture>	ExtTex8;

	//! @brief テクスチャセット読み込み(法線マップや拡張テクスチャなども読み込む)
	//! テクスチャの読み込みには、YsDirectXGraphicsに登録されているResourceStorageが使われます
	//! @param[in] filename … ファイル名
	void LoadTextureSet(const std::string& filename);

	//! @brief 解放。shared_ptrなのでnullptrを入れてるだけ。
	void Release(){
		MeshTex = nullptr;
		NormalTex = nullptr;
		ExtTex = nullptr;
		ExtTex2 = nullptr;
		ExtTex3 = nullptr;
		ExtTex4 = nullptr;
		ExtTex5 = nullptr;
		ExtTex6 = nullptr;
		ExtTex7 = nullptr;
		ExtTex8 = nullptr;
	}

	// 
	YsTextureSet(){
	}
	YsTextureSet(SPtr<YsTexture> src){
		*this = src;
	}

	//! @brief YsTextureのshared_ptrからコピーしてくる。srcはMeshTexに格納される。
	//! MeshTex以外のメンバは、空のYsTextureになる
	void operator=(SPtr<YsTexture> src){
		if(src != nullptr){
			MeshTex = src;
		}
		else{
			MeshTex = std::make_shared<YsTexture>();
		}
		NormalTex = std::make_shared<YsTexture>();
		ExtTex = std::make_shared<YsTexture>();
		ExtTex2 = ExtTex;
		ExtTex3 = ExtTex;
		ExtTex4 = ExtTex;
		ExtTex5 = ExtTex;
		ExtTex6 = ExtTex;
		ExtTex7 = ExtTex;
		ExtTex8 = ExtTex;
	}

	void operator=(const YsTextureSet& src){
		if(&src == nullptr){
			*this = nullptr;
		}
		else{
			MeshTex = src.MeshTex;
			NormalTex = src.NormalTex;
			ExtTex = src.ExtTex;
			ExtTex2 = src.ExtTex2;
			ExtTex3 = src.ExtTex3;
			ExtTex4 = src.ExtTex4;
			ExtTex5 = src.ExtTex5;
			ExtTex6 = src.ExtTex6;
			ExtTex7 = src.ExtTex7;
			ExtTex8 = src.ExtTex8;
		}
	}
};

}

#endif
