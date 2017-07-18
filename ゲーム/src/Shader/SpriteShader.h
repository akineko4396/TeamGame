#ifndef __SpriteShader_h__
#define __SpriteShader_h__

//=========================================================================================
//
// 2D描画シェーダ
//  ※YskLibのYsSpriteクラスより効率は悪いと思う。2D描画の参考用として見てください。
//    2Dをいっぱい出す場合はYsSpriteクラスをお勧め(DirectX Tool KitのSpriteBatch機能を使ってます)
// Sprite.hlslを使用します
// 
//=========================================================================================
class SpriteShader {
public:
	//=============================================
	// 初期化・解放
	//=============================================
	// 初期化
	void Init();
	// 解放
	void Release();

	//=============================================
	// 描画系
	//=============================================

	// 2D描画開始
	void Begin();
	// 2D描画終了
	void End();

	// 2D描画
	// ※Begin()を読んでいない場合は効率悪い処理が入るので、大量に呼びす場合はBegin()～End()の間で呼ぶことを推奨
	void Draw2D(ID3D11ShaderResourceView* srcTex, float x, float y, float w, float h, const YsVec4* color = &YsVec4(1, 1, 1, 1));
	void Draw2D(YsTexture& srcTex, float x, float y, float w, float h, const YsVec4* color = &YsVec4(1, 1, 1, 1)){
		Draw2D(srcTex.GetTex(), x, y, w, h, color);
	}

	// 2D線描画
	void DrawLine2D(const YsVec3& p1, const YsVec3& p2, const YsVec4* color = &YsVec4(1, 1, 1, 1));

	//=============================================
	// その他
	//=============================================

	// コンパイル済みシェーダをファイルに保存
	void SaveCsoToFile(){
		m_VS.SaveToFile();
		m_PS.SaveToFile();
	}

	// 
	SpriteShader() : m_bBegin(false){}
	~SpriteShader() { Release(); }

private:

	// 各シェーダオブジェクト
	YsVertexShader		m_VS;		// 頂点シェーダ
	YsPixelShader		m_PS;		// ピクセルシェーダ

	// 動的頂点バッファ用(線描画などで使用)
	YsPolygon			m_poly;

	// 定数バッファ
	struct cbPerObject {
		YsVec4	m_Color;		// 色
	};
	YsConstantBuffer<cbPerObject>	m_cb1_PerObject;

	// ステート
	YsDepthStencilState	m_dsBackUp;		// Depthステート記憶用

	// Flag
	bool			m_bBegin;
};

#endif
