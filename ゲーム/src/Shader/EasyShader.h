#ifndef __EasyShader_h__
#define __EasyShader_h__

//==============================================================
//
// 勉強用の単純なシェーダ
//  Easy.hlslを使用します
//
//==============================================================
class EasyShader{
public:
	//=============================================
	// 初期化・解放
	//=============================================
	// 初期化
	//  ここで、HLSLファイルから頂点シェーダやピクセルシェーダを作成したり、定数バッファを作成したり
	//  シェーダ描画で必要なものを作成する
	bool Init();

	// 解放
	//  作ったものを解放する
	void Release();

	// 描画直前でPerFrameの設定(基本は1フレームに１回だけ、描画の直前で実行する。ShaderManagerクラスのメンバ関数内で呼んでます)
	//  主に、定数バッファであるm_cb0_PerFrameの内容を更新している
	void WritePerFrameData();

	//=============================================
	// 描画系
	//=============================================
private:
	// マテリアルの内容を定数バッファに書き込んだり、テクスチャをセットしたりする
	void SetMaterial(const YsMaterial& mate);

public:
	// メッシュ描画
	//  mat		… [スタティックメッシュ専用]ワールド行列をしていする
	void DrawModel(YsModelObject& mo, const YsMatrix* mat = nullptr);

	//=============================================
	// その他
	//=============================================

	// コンパイル済みシェーダをファイルに保存
	void SaveCsoToFile(){
		m_VS.SaveToFile();
		m_SkinVS.SaveToFile();
		m_PS.SaveToFile();
	}

	EasyShader(){
	}

	~EasyShader(){
		Release();
	}

	//=============================================
private:

	// ボーン最大数
	enum { MAX_BONE = 400 };

	//=============================================
	// シェーダオブジェクト
	//=============================================
	YsVertexShader		m_VS;		// 頂点シェーダ
	YsVertexShader		m_SkinVS;	// スキンメッシュ用頂点シェーダ
	YsPixelShader		m_PS;		// ピクセルシェーダ

	//=============================================
	// コンスタント(定数)バッファ
	//  シェーダ側のグローバル定数にデータを書き込むためのもの
	//  DirectX9の時と違い、定数バッファというグループ単位でグローバル定数を作ることになる
	//  できるだけ、更新頻度単位でグループにまとめること
	// <<注意！>>
	// ※必ず16バイトの倍数にしないといけない！
	// ※さらに、HLSL側のパッキング規則に合わせないとデータがズレる
	// ※そのために、tmpという無駄な変数を追加している
	//=============================================

	// １フレーム単位での更新データ
	//  カメラとか、１フレームに1回くらいか更新しないようなデータ
	struct cbPerFrame {
		YsMatrix	mV;				// ビュー行列　(行列はベクトル4つ分なので16の倍数)
		YsMatrix	mP;				// 射影行列　　(行列はベクトル4つ分なので16の倍数)

		YsVec3		LightDir = YsVec3(0, -1, 0);		// ライト方向　(3Dベクトルは12バイトなので4バイトたりない)
		float		tmp;			//             (そのため4バイトの変数をねじ込む)

		YsVec3		CamPos;			// カメラ座標　(上記と同じ)
		int			tmp2;			// 

	};

	// オブジェクト単位での更新データ(スタティックメッシュ用)
	//  モデルの行列とか、モデル単位でデータが変更されるもの
	struct cbPerObject {
		YsMatrix	mW;				// ワールド行列

		int			LightEnable = 1;	// ライト有効フラグ
			int			tmp[3];			// 16バイトに揃えるための12バイト追加(パッキング規則により、ここに追加しないといけない)

	};

	// オブジェクト単位での更新データ(スキンメッシュ用)
	//  スキンメッシュモデルの全ボーン行列
	struct cbPerSkinObject {
		YsMatrix	mWArray[MAX_BONE];
	};

	// マテリアル単位での更新データ
	//  １つのモデルでも、マテリアルは複数あります。なのでcbPerObject内ではなく、別に作る
	struct cbPerMaterial {
		YsVec4		Diffuse = YsVec4(1, 1, 1, 1);		// 基本色
		YsVec4		Ambient = YsVec4(0, 0, 0, 0);		// 環境色
		YsVec4		Specular = YsVec4(1, 1, 1, 1);	// スペキュラ色
		float		SpePower = 0;						// スペキュラの鋭さ
			float tmp2[3];								// 16バイトに揃えるための12バイト追加(パッキング規則により、ここに追加しないといけない)
		YsVec4		Emissive = YsVec4(0, 0, 0, 0);	// エミッシブ

	};

	// 上記の構造体の型の定数バッファを作成！
	YsConstantBuffer<cbPerFrame>			m_cb0_PerFrame;			// １フレーム単位での更新
	YsConstantBuffer<cbPerObject>			m_cb1_PerObject;		// オブジェクト単位での更新(スタティックメッシュ用)
	YsConstantBuffer<cbPerSkinObject>		m_cb3_PerSkinObject;	// オブジェクト単位での更新(スキンメッシュ用)
	YsConstantBuffer<cbPerMaterial>			m_cb2_PerMaterial;		// マテリアル単位での更新
};

#endif
