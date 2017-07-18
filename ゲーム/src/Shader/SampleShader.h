#ifndef __SampleShader_h__
#define __SampleShader_h__

//==============================================================
//
// サンプルシェーダクラス
// いろいろな機能を入れていますので、参考にどうぞ。
// <Model.hlslには以下の機能を入れています>
//	・平行光源(ランバート拡散照明　最大3つ)
//	・点光源(ポイントライティング　最大100個)
//	・スポット光源(スポットライティング　最大20個)
//	・トゥーン(上記のライティング結果を元にトゥーン用テクスチャから実際に使う光の強さを取得する)
//	・フォン鏡面反射(スペキュラ)
//	　※ライトのデータは、LightManagerクラスの内容を使用
//	・キューブ環境マッピング(疑似映り込み)
//	・法線マッピング + 視差マッピング(凹凸表現)
//	・加算スフィアマッピング(MMDのマネ  光沢っぽい表現)
//	・距離フォグ(霧効果)
//	・エミッシブマップ(自己発光  発光部分はRT1に出力される)
//
//	別途、輪郭シェーダ(Outline.hlsl)、エフェクト用シェーダ(Effect.hlsl)もいれてます
//  共通で使用する定数バッファなどはCommon.hlslに記述しています
//
//==============================================================
class SampleShader {
public:
	//=============================================
	// 初期化・解放
	//=============================================
	// 初期化
	bool Init();
	// 解放
	void Release();
	// 描画直前でPerFrameの設定
	void WritePerFrameData();

	//=============================================
	// パラメータ設定系
	//=============================================
	// ライティングON/OFF
	void SetLightEnable(bool enable){
		m_cb1_PerObject.m_Data.LightEnable = enable ? 1 : 0;
	}

	// フォグ設定
	//  fogColor	… フォグの色
	//  fogRange	… フォグの範囲(x:フォグがかかり始める距離 y:フォグが完全にかかる距離)
	void SetFog(bool enable, const YsVec3* fogColor = nullptr, const YsVec2* fogRange = nullptr){
		m_cb1_PerObject.m_Data.FogEnable = enable ? 1 : 0;
		if(fogColor){
			m_cb1_PerObject.m_Data.FogColor = *fogColor;
		}
		if(fogRange){
			m_cb1_PerObject.m_Data.FogRange = *fogRange;
		}
	}

	// 色変更
	//  colorにnullptrを渡すと(1,1,1,1)になる
	void SetMulColor(const YsVec4* color = &YsVec4::One){
		m_cb1_PerObject.m_Data.MulColor = *color;
	}

	//=============================================
	// 描画系
	//=============================================
private:
	// マテリアルとテクスチャなどをセット
	void SetMaterial(const YsMaterial& mate);

public:
	// モデル描画
	//  mat		… [スタティックメッシュ専用]ワールド行列をしていする
	void DrawModel(YsModelObject& mo, const YsMatrix* mat = nullptr);
		// ※メッシュ単品描画Version ちなみにスキンメッシュでもスタティックメッシュの描画となります(ボーンデータが無いから)
	void DrawMeshOnly(YsMesh& mesh, const YsVec4* color, const YsMatrix* mat = nullptr);
		// モデル輪郭描画
		//  outlineSize … x:最少サイズ y:最大サイズ(カメラの距離により変化)
		void DrawModelOutline(YsModelObject& mo, const YsVec4* outlineColor, const YsVec2* outlineSize, const YsMatrix* mat = nullptr);


	// ビルボード描画
	void DrawBillBoard(YsBillBoard& bb, const YsMatrix* mat);
	// レーザ描画
	void DrawLaser(YsLaser& lz, int mode);

	// 線描画
	void DrawLine(const YsVec3& p1, const YsVec3& p2, const YsVec4* color);
	// BOX線描画
	void DrawBoxLine(const YsVec3& vMin, const YsVec3& vMax, const YsVec4* color, const YsMatrix* mat);

	// 円線描画 XY平面の円になるので、回転したい場合はmatで指定してね
	void DrawCircleLine(float radius, const YsVec4* color, const YsMatrix* mat);
	// 球線描画
	void DrawSphereLine(float radius, const YsVec4* color, const YsMatrix* mat);


	// コンパイル済みシェーダをファイルに保存
	void SaveCsoToFile(){
		m_VS.SaveToFile();
		m_SkinVS.SaveToFile();
		m_PS.SaveToFile();
		m_OutlineVS.SaveToFile();
		m_OutlineSkinVS.SaveToFile();
		m_OutlinePS.SaveToFile();
		m_EffectVS.SaveToFile();
		m_EffectPS.SaveToFile();
	}

	// 
	SampleShader() {
	}

	~SampleShader() {
		Release();
	}

private:
	enum { MAX_BONE = 400 };	// ボーン最大数

	//=============================================
	// 各シェーダオブジェクト
	//=============================================
	// Model.hlsl用
	YsVertexShader		m_VS;		// 頂点シェーダ
	YsVertexShader		m_SkinVS;	// スキンメッシュ用頂点シェーダ
	YsPixelShader		m_PS;		// ピクセルシェーダ

	// Outline.hlsl用
	YsVertexShader		m_OutlineVS;		// 頂点シェーダ
	YsVertexShader		m_OutlineSkinVS;	// スキンメッシュ頂点シェーダ
	YsPixelShader		m_OutlinePS;		// ピクセルシェーダ

	// Effect.hlsl用
	YsVertexShader		m_EffectVS;		// 頂点シェーダ
	YsPixelShader		m_EffectPS;		// ピクセルシェーダ

	// 動的頂点バッファ用(線描画などで使用)
	YsPolygon				m_poly;

	

	//=============================================
	// コンスタント(定数)バッファ
	//=============================================
	// １フレーム単位での設定データ
	struct cbPerFrame {
		YsMatrix	mV;				// ビュー行列
		YsMatrix	mP;				// 射影行列

		YsVec3		CamPos;			// カメラ座標
			int			tmp2;

	};

	//+++++++++++++++++++++++++++++++++++++++++++++
	// Model.hlsl用
	//+++++++++++++++++++++++++++++++++++++++++++++
	// オブジェクト単位での設定データ(スタティックメッシュ用)
	struct cbPerObject {
		YsMatrix	mW;				// ワールド行列
		YsVec4		MulColor;		// 無条件で合成する色 強制的に色を変更したい場合に使用
		int			LightEnable;	// ライト有効/無効
			int tmp[3];

		int			FogEnable;		// フォグ有効
		YsVec3		FogColor;		// フォグ色
		YsVec2		FogRange;		// フォグ範囲
			int	tmp3[2];

		cbPerObject(){
			LightEnable = 1;
			FogEnable = 0;
			MulColor.Set(1, 1, 1, 1);
		}
	};

	// オブジェクト単位での設定データ(スキンメッシュ用)
	struct cbPerSkinObject {
		YsMatrix	mWArray[MAX_BONE];	// ボーン行列配列
	};

	// マテリアル単位での設定データ
	struct cbPerMaterial {
		YsVec4		Diffuse = YsVec4(1,1,1,1);	// 基本色
		YsVec4		Ambient = YsVec4(0,0,0,0);	// 環境色
		YsVec4		Specular = YsVec4(1,1,1,1);	// スペキュラ色
		float		SpePower = 0;					// スペキュラの鋭さ
			float tmp2[3];
		YsVec4		Emissive = YsVec4(0,0,0,0);	// エミッシブ
		float		ParallaxVal = 0;				// 視差度
			float tmp3[3];

	};

	//+++++++++++++++++++++++++++++++++++++++++++++
	// Outline.hlsl用
	//+++++++++++++++++++++++++++++++++++++++++++++
	// ※輪郭用
	struct cbPerObject_Outline {
		YsMatrix	mW;							// ワールド行列
		YsVec4		Color = YsVec4(1,1,1,1);	// 輪郭色
		YsVec2		OutlineSize = YsVec2(0.0005f, 0.01f);	// 輪郭サイズ(最低～最高幅)
		float		OutlineDist = 10;	// 距離により輪郭の太さを調整する値
			float		tmp;
	};

	//+++++++++++++++++++++++++++++++++++++++++++++
	// Effect.hlsl用
	//+++++++++++++++++++++++++++++++++++++++++++++
	// ※Effect用
	struct cbPerObject_Effect {
		YsMatrix	mW;				// ワールド行列
		YsVec4		Color = YsVec4(1,1,1,1);			// 色

	};

	// アプリ -> シェーダー データ送信用のコンスタントバッファクラス
	// 共通
	YsConstantBuffer<cbPerFrame>				m_cb0_PerFrame;			// １フレーム単位での更新

	// Model.hlsl用
	YsConstantBuffer<cbPerObject>				m_cb1_PerObject;		// オブジェクト単位での更新(スタティックメッシュ用)
	YsConstantBuffer<cbPerSkinObject>			m_cb3_PerSkinObject;	// オブジェクト単位での更新(スキンメッシュ用)
	YsConstantBuffer<cbPerMaterial>				m_cb2_PerMaterial;		// マテリアル単位での更新

	// Outline.hlsl用
	YsConstantBuffer<cbPerObject_Outline>		m_cb1_PerObject_Outline;	// オブジェクト単位での更新(輪郭用)

	// Effect.hlsl用
	YsConstantBuffer<cbPerObject_Effect>		m_cb1_PerObject_Effect;	// オブジェクト単位での更新(エフェクト用)

};

#endif
