//===============================================================
//
//! @file YsSprite.h
//! @brief 2D描画クラス
//!
//! @author 鎌田
//
//===============================================================
#ifndef __YsSprite_h__
#define __YsSprite_h__

namespace YskLib{

//======================================================================
//!
//! @brief 2D描画用SpriteBatchクラス
//!
//!	DirectX Tool KitのSpriteBatchをラップしたクラス
//!	まとめて描画できるので高速な2D描画が可能
//!
//! @ingroup Graphics_Important
//======================================================================
class YsSprite {
public:
	//--------------------------------------------------------------------------
	//! @brief 初期化
	//! @param[in]	devContext	… デバイスコンテキスト
	//--------------------------------------------------------------------------
	void Init(ID3D11DeviceContext* devContext);

	//--------------------------------------------------------------------------
	//! @brief 解放
	//--------------------------------------------------------------------------
	void Release();

	//--------------------------------------------------------------------------
	//! @brief バッチ描画開始
	//!
	//! 各ステートは記憶され、End()で復元されます。Begin()～End()間でなにかステートを切り替えても無効になります。	\n
	//! bImmediateModeがfalseの場合は、Draw()時にすぐには描画されず、End()時にまとめて描画されるのでパフォーマンスがあがるが、\n
	//! Begin()～End()間でBlendStateなどを切り替えることが出来ない。	\n
	//! \n
	//! bImmediateModeがfalseの場合は、Draw()を呼ぶと即座に描画が実行されるが、大量に描画するとパフォーマンスが落ちる。	\n
	//! そのぶん、Begin()～End()間でステートを切り替えることが可能。
	//! 
	//! @param[in]	bImmediateMode	… 即座に描画するか？ true:Draw()関数を呼ぶとすぐに描画される false:End()時にまとめて描画される。
	//!	@param[in]	blendState		… 画像の合成方法であるブレンドステートオブジェクトを渡す。nullptrだと通常のアルファ合成になる。bImmediateModeがfalseの時にEnd()で使われる。
	//--------------------------------------------------------------------------
	void Begin(bool bImmediateMode = false, YsBlendState* blendState = nullptr);

	//--------------------------------------------------------------------------
	//! @brief バッチ描画終了(実際にまとめて描画される)
	//! 各ステートをBegin()を呼ぶ前の状態に復元します
	//--------------------------------------------------------------------------
	void End();

	//============================================================
	// 描画
	//  各描画関数はBegin()～End()間で呼ぶと効率良し。
	//  ※Begin()を呼んでいない場合でも内部で自動でBegin(),End()しますが効率は悪い。
	//============================================================
	// テクスチャ描画

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 行列を元に描画。※行列はZ回転のみ可能。	\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいテクスチャ
	//!  @param[in]	mat		… 変換行列
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, const YsMatrix& mat, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 行列を元に描画。※行列はZ回転のみ可能。	\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいテクスチャ
	//!  @param[in]	mat		… 変換行列
	//!  @param[in]	sx		… (描画元)X座標
	//!  @param[in]	sy		… (描画元)Y座標
	//!  @param[in]	sw		… (描画元)幅
	//!  @param[in]	sh		… (描画元)高さ
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, const YsMatrix& mat, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 行列を元に描画。※行列はZ回転のみ可能。	\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいYsTexture
	//!  @param[in]	mat		… 変換行列
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, const YsMatrix& mat, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), mat, Color);
	}

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 行列を元に描画。※行列はZ回転のみ可能。	\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいYsTexture
	//!  @param[in]	mat		… 変換行列
	//!  @param[in]	sx		… (描画元)X座標
	//!  @param[in]	sy		… (描画元)Y座標
	//!  @param[in]	sw		… (描画元)幅
	//!  @param[in]	sh		… (描画元)高さ
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, const YsMatrix& mat, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), mat, sx, sy, sw, sh, Color);
	}

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 描画先x,y,w,h指定		\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいテクスチャ
	//!  @param[in]	x		… X座標
	//!  @param[in]	y		… Y座標
	//!  @param[in]	w		… 幅
	//!  @param[in]	h		… 高さ
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, int x, int y, int w, int h, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 描画先x,y,w,h指定		\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいテクスチャ
	//!  @param[in]	x		… (描画先)X座標
	//!  @param[in]	y		… (描画先)Y座標
	//!  @param[in]	w		… (描画先)幅
	//!  @param[in]	h		… (描画先)高さ
	//!  @param[in]	sx		… (描画元)X座標
	//!  @param[in]	sy		… (描画元)Y座標
	//!  @param[in]	sw		… (描画元)幅
	//!  @param[in]	sh		… (描画元)高さ
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(ID3D11ShaderResourceView* tex, int x, int y, int w, int h, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1));

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 描画先x,y,w,h指定		\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいYsTexture
	//!  @param[in]	x		… X座標
	//!  @param[in]	y		… Y座標
	//!  @param[in]	w		… 幅
	//!  @param[in]	h		… 高さ
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, int x, int y, int w, int h, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), x, y, w, h, Color);
	}

	//--------------------------------------------------------------------------
	//! @brief テクスチャ描画
	//!
	//! 描画先x,y,w,h指定		\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//!  @param[in]	tex		… 描画したいYsTexture
	//!  @param[in]	x		… (描画先)X座標
	//!  @param[in]	y		… (描画先)Y座標
	//!  @param[in]	w		… (描画先)幅
	//!  @param[in]	h		… (描画先)高さ
	//!  @param[in]	sx		… (描画元)X座標
	//!  @param[in]	sy		… (描画元)Y座標
	//!  @param[in]	sw		… (描画元)幅
	//!  @param[in]	sh		… (描画元)高さ
	//!  @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void Draw(YsTexture& tex, int x, int y, int w, int h, int sx, int sy, int sw, int sh, const YsVec4* Color = &YsVec4(1, 1, 1, 1)){
		Draw(tex.GetTex(), x, y, w, h, sx, sy, sw, sh, Color);
	}

	//============================================================
	// フォント描画
	//============================================================

	//--------------------------------------------------------------------------
	//! @brief フォント描画
	//!
	//! 行列を元に描画。※行列はZ回転のみ可能。	\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//! @param[in]	text	… 描画する文字列
	//! @param[in]	mat		… 変換行列
	//! @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void DrawFont(const std::string& text, const YsMatrix& mat, const YsVec4* Color);

	//--------------------------------------------------------------------------
	//! @brief フォント描画
	//!
	//! 座標と色のみ指定	\n
	//! 大量に呼ぶ場合はBeing()～End()間で実行することでパフォーマンスがあがります。
	//!
	//! @param[in]	text	… 描画する文字列
	//! @param[in]	x		… X座標
	//! @param[in]	y		… Y座標
	//! @param[in]	Color	… 色
	//--------------------------------------------------------------------------
	void DrawFont(const std::string& text, int x, int y, const YsVec4* Color);


	//! @brief スプライトバッチオブジェクト取得
	DirectX::SpriteBatch* GetSpriteBatch() { return m_pSB; }


	// 
	YsSprite(){
	}
	// 
	~YsSprite();

private:
	
	DirectX::SpriteBatch*	m_pSB = nullptr;		//!< スプライトバッチ
	DirectX::SpriteFont*	m_pFont = nullptr;		//!< スプライトフォント
	bool					m_IsBegin = false;		//!< Begin中か？

	//! 各ステート保存用
	struct States {
		YsBlendState bs;
		YsRasterizeState rs;
		YsSamplerState ss;
		YsDepthStencilState ds;
	};
	UPtr<States>	m_SaveStates;			//!< ステート復元用データ

	//! 使用ステート
	YsBlendState m_bsAlpha;

private:
	// コピー禁止用
	YsSprite(const YsSprite& src) {}
	void operator=(const YsSprite& src) {}
};

}
#endif
