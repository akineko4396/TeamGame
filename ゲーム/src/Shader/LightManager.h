#ifndef __LightManager_h__
#define __LightManager_h__
//================================================
//
// いろいろなライトのデータ
//
// ・DirLight		… 平行光源データ
// ・PointLight	… ポイントライトデータ
// ・SpotLight		… スポットライトデータ
// ・LightManager	… 上記のデータを管理する、ライト管理クラス
//
//================================================


//================================================
// 平行光源データ(ディレクショナルライト)
//================================================
struct DirLight {
	YsVec4		Diffuse = YsVec4(1, 1, 1, 1);	// ライト色
	YsVec3		Direction = YsVec3(0, -1, 0);	// 方向

	//=========================================================
	// 平行光源としてデータ設定
	// direction	… ライトの方向
	// diffuse		… 色
	// specular	… スペキュラ色
	//=========================================================
	void SetData(const YsVec3& direction, const YsVec4& diffuse) {
		Diffuse = diffuse;
		Direction = direction;
	}

};

//================================================
// 点光源データ(ポイントライト)
//================================================
struct PointLight {
	YsVec4			Diffuse = YsVec4(1, 1, 1, 1);	// 基本色
	YsVec3			Position;						// 座標

	float			Radius = 1;	// 半径

	//=========================================================
	// 点光源としてデータ設定
	// pos			… 座標
	// diffuse		… 色
	// radius		… 半径
	//=========================================================
	void SetData(const YsVec3& pos, const YsVec4& diffuse, float radius) {
		Position = pos;
		Diffuse = diffuse;
		Radius = radius;
	}

};

//================================================
// スポットライトデータ
//================================================
struct SpotLight {

	YsVec4			Diffuse = YsVec4(1, 1, 1, 1);		// 基本色
	YsVec3			Position;							// 座標
	YsVec3			Direction = YsVec3(0, 0, 1);		// 方向

	float			Range = 10;		// 照射距離

	float			MinAngle = 15;	// 角度
	float			MaxAngle = 20;	// 角度

	//=========================================================
	// スポット光源としてデータ設定
	//=========================================================
	void SetData(const YsVec3& pos, const YsVec3& direction, const YsVec4& diffuse, float range, float minAngle, float maxAngle) {
		Position = pos;
		Diffuse = diffuse;
		Direction = direction;
		Range = range;
		MinAngle = minAngle;
		MaxAngle = maxAngle;
	}
};

//================================================
//
// ライト管理
//
//   平行光源、点光源、スポット光源を管理するクラス
//
//   シェーダの定数バッファのb5,b6,b7を使用します
//
//
//================================================

/*
[使い方例]

<グローバルなどで用意>
SPtr<CPointLight> PL;

<ポイントライト作成>
PL = ShMgr.m_LightMgr.AddPointLight(YsVec3(0, 0, 0), YsVec4(2, 0.2f, 0.2f, 0), 10);

<ライト削除>
PL = nullptr;

※平行光源はCDirLight型、スポットライトはCSpotLight型を使う

*/

class LightManager {
public:

	//================================================
	// 初期化
	//================================================
	void Init();

	//================================================
	// 解放
	//  各ライトの管理リストは解放されません
	//  定数バッファなどが解放されます
	//================================================
	void Release();

	//
	enum {
		MAX_DIRLIGHT = 3,		// 最大平行光源数
		MAX_POINTLIGHT = 100,	// 最大ポイントライト数
		MAX_SPOTLIGHT = 20,		// 最大スポットライト数
	};

	//-------------------------------------------
	// 環境光
	//-------------------------------------------
	YsVec3				m_AmbientLight = YsVec3(0.3f, 0.3f, 0.3f);

	//-------------------------------------------
	// 平行光源
	//-------------------------------------------
	std::list<WPtr<DirLight>>		m_DirLightList;	// 平行光源管理リスト

	// 平行光源追加
	// 戻り値：追加した平行光源のshared_ptr
	//  点光源を追加する場合はこの関数を呼ぶ。内部ではWPtrで保持してるので、誰も保持しなくなった時点で光源は消滅する仕組み
	//  消滅時の解除処理はUpdateBeforeDraw()で行ってます。
	SPtr<DirLight> AddDirLight(){
		SPtr<DirLight> add(new DirLight());		// メモリ確保
		m_DirLightList.push_back(add);				// ライトリストへ追加
		return add;
	}

	//-------------------------------------------
	// 点光源
	//-------------------------------------------
	std::list<WPtr<PointLight>>	m_PointLightList;	// ポイントライト管理リスト

	// ポイントライト追加
	// 戻り値：追加したポイントライトのshared_ptr
	//  点光源を追加する場合はこの関数を呼ぶ。内部ではWPtrで保持してるので、誰も保持しなくなった時点で光源は消滅する仕組み
	//  消滅時の解除処理はUpdateBeforeDraw()で行ってます。
	SPtr<PointLight> AddPointLight(){
		SPtr<PointLight> add(new PointLight());		// メモリ確保
		m_PointLightList.push_back(add);				// ポイントライトリストへ追加
		return add;
	}

	//-------------------------------------------
	// スポット光源
	//-------------------------------------------
	std::list<WPtr<SpotLight>>		m_SpotLightList;		// スポットライト管理リスト

	// スポットライト追加
	// 戻り値：追加したスポットライトのshared_ptr
	//  スポット光源を追加する場合はこの関数を呼ぶ。内部ではWPtrで保持してるので、誰も保持しなくなった時点で光源は消滅する仕組み
	//  消滅時の解除処理はUpdateBeforeDraw()で行ってます。
	SPtr<SpotLight> AddSpotLight(){
		SPtr<SpotLight> add(new SpotLight());				// メモリ確保
		m_SpotLightList.push_back(add);						// スポットライトリストへ追加
		return add;
	}

	//-------------------------------------------
	// 更新処理
	//-------------------------------------------
	void Update();


	//===================================================
	// SampleShader用　ライト用定数バッファデータ
	//===================================================
	struct cbSampleShaderLight {
		// 各光源の数
		int			DL_Cnt = 0;	// 平行光数
		int			PL_Cnt = 0;	// ポイントライト数
		int			SL_Cnt = 0;	// スポットライト数
		int tmp[1];	// ※パッキング規則のため、ゴミを挟み込む

		//-------------------------------
		// 平行光源
		//-------------------------------

		// ライトのデータ
		struct DLData {
			YsVec4		Color;	// 色
			YsVec3		Dir;	// 方向
			float		tmp;	// 
		};
		DLData DL[MAX_DIRLIGHT];	// 平行光データ配列(最大3個)

		//-------------------------------
		// ポイントライト
		//-------------------------------
		// ライトのデータ
		struct PLData {
			YsVec4		Color;		// 色
			YsVec3		Pos;		// 座標
			float		Radius = 1;	// 半径
		};
		PLData PL[MAX_POINTLIGHT];	// ポイントライトデータ配列(最大100個)

		//-------------------------------
		// スポットライト
		//-------------------------------
		// ライトのデータ
		struct SLData {
			YsVec4		Color;	// 色
			YsVec3		Pos;	// 座標
			float		Range;	// 照射範囲

			YsVec3		Dir;	// 方向
			float		MinAng;	// min～maxにかけて、徐々に光の強さが減衰される
			float		MaxAng;	// 
			float tmp[3];
		};
		SLData SL[MAX_SPOTLIGHT];	// スポットライトデータ配列(最大100個)



	};
	YsConstantBuffer<cbSampleShaderLight>				m_cb12_SampleLight;			// SampleShader用ライト定数バッファ(通常は１フレーム単位での更新)

	//===================================================
	// MyShader用　ライト用定数バッファデータ
	//===================================================




};

#endif
