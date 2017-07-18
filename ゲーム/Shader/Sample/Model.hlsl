//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 3Dモデル用のシェーダ
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================
// コンスタント(定数)バッファの作成
// 定数バッファとはグローバル定数をグループ化した物。VC側からデータを渡せる場所。
// Direct3D11ではグローバル定数は、この定数バッファにする必要がある。
//
// register(b0)のb0が定数バッファのグループ番号
// 最大14個(b0～b13)作れる
// VC側からはこの番号を使って定数バッファをセットする
//
// 定数バッファにはパッキング規則というものがあり、きちんと意識していないと
// VC側から正常に値を持っていけないので注意
// [定数変数のパッキング規則 (DirectX HLSL)]
// https://msdn.microsoft.com/ja-jp/library/bb509632(v=vs.85).aspx
//===========================================================

#include "Common.hlsl"

//===================================
// [定数バッファ]オブジェクト単位データ
// packoffsetとは … この定数バッファ内のデータ位置を指定できる。省略した場合は自動で決定される
//===================================
cbuffer cbPerObject : register(b1)
{
	row_major float4x4	g_mW	: packoffset(c0);	// ワールド行列

	// 
	float4  g_MulColor      : packoffset(c4.x);     // 無条件に合成する色 強制的に色を変更したい場合に使用

	// ライト
	int		g_LightEnable	: packoffset(c5.x);		// ライティング有効/無効フラグ

	// フォグ
	int		g_FogEnable		: packoffset(c6.x);		// フォグ有効/無効フラグ
	float3	g_FogColor		: packoffset(c6.y);		// フォグ色
	float2	g_FogRange		: packoffset(c7);		// フォグ範囲
};

//===================================
// [定数バッファ]マテリアル単位データ
//===================================
cbuffer cbPerMaterial : register(b2)
{
	float4	g_Diffuse;			// 拡散色(基本色)
	float4	g_Ambient;			// 環境色
	float4	g_Specular;			// スペキュラ(反射)
	float	g_SpePower;			// スペキュラの鋭さ
	float4	g_Emissive;			// エミッシブ(自己発光)

	float	g_ParallaxVal;		// 視差度
};

//===================================
// [定数バッファ]オブジェクト単位データ(スキンメッシュ用)
//===================================
cbuffer cbPerSkinObject : register(b3)
{
	row_major float4x4	g_mWArray[MAX_BONE];	// ボーン用行列配列
};

//====================================================
// テクスチャ
//  VC側からセットするときに指定する番号は、t0～t127の位置に対応している
//  最大128(t0～t127)
//====================================================
Texture2D MeshTex			: register(t0);		// メッシュの通常テクスチャ(ディフューズテクスチャ)
Texture2D EmissiveTex		: register(t1);		// 発光テクスチャ
Texture2D ToonTex			: register(t2);		// トゥーンテクスチャ(陰影用)
Texture2D MaterialSphereTex : register(t3);		// 材質用加算スフィアマップ(MMDっぽい表現をするため)
Texture2D SphereParamTex	: register(t4);		// 映り込み(スフィア環境)マップ
Texture2D SpecularTex		: register(t5);		// スペキュラマップ
Texture2D NormalTex			: register(t10);	// 法線マップテクスチャ

TextureCube CubeTex         : register(t20);    // キューブ環境マップ


//====================================================
// サンプラ
//  VC側からセットするときに指定する番号は、s0～s15の位置に対応している
//  最大16(s0～s15)
//====================================================
SamplerState WrapSmp : register(s0);	// Wrap用のサンプラ
SamplerState ClampSmp : register(s1);	// Clamp用のサンプラ



//===============================================
//
// 頂点シェーダー
//
//===============================================

// 頂点シェーダ出力データ
struct VS_OUT{
	float4 Pos		: SV_Position;		// 2D座標(射影座標系)
	float2 UV		: TEXCOORD0;		// UV
	float3 wT		: TEXCOORD1;		// 接線(Tangent)
	float3 wB		: TEXCOORD2;		// 従法線(BiNormal)
	float3 wN		: TEXCOORD3;		// 法線(Normal)
	float3 wPos		: TEXCOORD4;		// 3D座標(ワールド座標系)
	float3 wvPos	: TEXCOORD5;		// 3D座標(ビュー座標系)
};

//===============================================
// スタティックメッシュ用頂点シェーダー
//===============================================
VS_OUT VS(	float4 pos : POSITION,		// 頂点座標(ローカル)
			float3 tangent : TANGENT,	// 接線
			float3 binormal : BINORMAL,	// 従法線
			float3 normal : NORMAL,		// 法線
			float2 uv : TEXCOORD0		// UV
			)
{
	VS_OUT Out = (VS_OUT)0;
	
	// 2D変換
	Out.Pos = mul(pos, g_mW);
		Out.wPos = Out.Pos.xyz;
	Out.Pos = mul(Out.Pos, g_mV);
		Out.wvPos = Out.Pos.xyz;
	Out.Pos = mul(Out.Pos, g_mP);
	
	// UVはそのまま入れる
	Out.UV = uv;
	
	// ３種の法線
	Out.wT = normalize(mul(tangent, (float3x3)g_mW));
	Out.wB = normalize(mul(binormal, (float3x3)g_mW));
	Out.wN = normalize(mul(normal, (float3x3)g_mW));
	
	return Out;
}

//===============================================
// スキンメッシュ用頂点シェーダー
//===============================================
VS_OUT SkinVS( 	float4 pos : POSITION,
				float3 tangent : TANGENT,
				float3 binormal : BINORMAL,
				float3 normal : NORMAL,
				float2 uv : TEXCOORD0,
				float4 blendWeight : BLENDWEIGHT,	// ボーンウェイトx4
				uint4 blendIndex : BLENDINDICES		// ボーン番号x4
){
	VS_OUT Out;

	//----------------------------------------------------------------------	
	// 4つのボーン行列で頂点座標や法線をブレンドし、3Dワールド座標・法線を求める
	float3      Pos = 0.0f;         // ブレンド後のワールド座標
	float3      Normal = 0.0f;      // ブレンド後のワールド法線Z
	float3      Binormal = 0.0f;    // ブレンド後のワールド法線Y
	float3      Tangent = 0.0f;     // ブレンド後のワールド法線X

	for(int i=0;i<4;i++){
		// 座標
		Pos += mul(pos, g_mWArray[blendIndex[i]]).xyz * blendWeight[i];
		// 法線３つ
		Normal += mul(normal, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
		Binormal += mul(binormal, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
		Tangent += mul(tangent, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
	}
	//----------------------------------------------------------------------	

	// 座標変換
	Out.wPos = Pos;
	Out.Pos = mul(float4(Pos, 1), g_mV);
		Out.wvPos = Out.Pos.xyz;
	Out.Pos = mul(Out.Pos, g_mP);

	// UV
	Out.UV = uv;

	// 法線
	Out.wT = Tangent;
	Out.wB = Binormal;
	Out.wN = Normal;

	return Out;
}

//===============================================
//
// ピクセルシェーダ
//
//===============================================

//===============================================
// スペキュラの強度を求める
// <引数>
//  [in] lightDir	… ライトの方向
//  [in] normal		… 法線ベクトル(正規化したものを渡すこと)
//  [in] vCam		… ピクセルの座標からカメラへの方向(正規化したものを渡すこと)
// <戻り値>スペキュラの強さ
//===============================================
float CalcSpecular(float3 lightDir, float3 normal, float3 vCam)
{
	float3 vRef = normalize(reflect(lightDir, normal));
	float SpePow = dot(vCam, vRef); // カメラの角度差(-1～1)

	SpePow = max(0, SpePow); // マイナスは打ち消す
	SpePow = pow(SpePow, g_SpePower+0.001);		// 0乗だとなんかヤバい数値になったりするので、少しだけ値を加算しておく

	return SpePow;
}

//===============================================
// ライティング計算を行う
// 平行光源、点光源、スポット光源の拡散色、反射色を求める
// <引数>
//    [in]  wPos     … ピクセルの3D座標
//    [in]  normal   … 法線
//    [in]  vCam     … ピクセルからカメラへの方向
//    [out] LightCol … 最終的なライトの拡散色(Diffuse)が入る
//    [out] LightSpe … 最終的なライトの反射色(Specular)が入る
//===============================================
void CalcLighting(float3 wPos, float3 normal, float3 vCam, inout float3 LightCol, inout float4 LightSpe)
{
	//============================================================
	// 平行光源ライティング
	//============================================================
	// ライト数ぶん処理
	for (int di = 0; di < g_DL_Cnt; di++)
	{
		//============================================================
		// ランバート拡散照明
		//============================================================
		float LPow = dot(-g_DL[di].Dir, normal);
		LPow = max(0, LPow);

		//============================================================
		// トゥーン
		//  ライティング結果の光の強さを元にトゥーンテクスチャから、カスタマイズされた光の強さを取得
		//============================================================
		float4 ToonCol = ToonTex.Sample(ClampSmp, float2(LPow, 0.5));


		//============================================================
		// シャドウマップでの光の遮断判定
		//============================================================











		//============================================================
		// ライト色を加算する
		//============================================================
		LightCol.rgb += g_DL[di].Color.rgb * ToonCol.rgb;

		//============================================================
		// スペキュラ(フォン鏡面反射)
		//============================================================
		// スペキュラ計算
		/*
		float3 vRef = normalize(reflect(g_DL[di].Dir, w_normal));
		float SpePow = dot(vCam, vRef); // カメラの角度差(-1～1)
		SpePow = max(0, SpePow); // 0～１にする
		SpePow = pow(SpePow, g_SpePower+0.001);		// 0乗だとなんかヤバい数値になったりするので、少しだけ値を加算しておく
		*/
		float SpePow = CalcSpecular(g_DL[di].Dir, normal, vCam);
		// ライトスペキュラ色を加算
		LightSpe += g_DL[di].Color * SpePow;
	}

	//============================================================
	// 点光源処理(ポイントライト)
	//  ※このライトはトゥーンを使用しません
	//============================================================
	// ライト数ぶん処理
	for (int pi = 0; pi < g_PL_Cnt; pi++)
	{
		// 色
		// このピクセルと点光源の距離を調べる
		float3 vDir = g_PL[pi].Pos - wPos; // 点光源への方向
		float dist = length(vDir); // 距離

		// 範囲内
		if (dist < g_PL[pi].Radius)
		{
			// 法線とライトの方向から内積で光の強さを求める
			// 点光源への方向と法線との角度の差が明るさとなる(ランバートと同じ)
	//		vDir = normalize(vDir);     //  正規化
			vDir /= dist;               //  正規化
			float PLPow = dot(normal, vDir);    // 法線とライトの方向との角度 (-1～1)
			PLPow = saturate(PLPow);            // 0～1にする

			// 半径範囲による減衰を行う  ライトの座標になればなるほど明るく、半径ぎりぎりは暗い
			float attePow = 1 - saturate(dist / g_PL[pi].Radius); // 1(近い=最大の明るさ)　～　0(遠い=完全に暗い)
			attePow *= attePow; // 光の強さは、距離の二乗に反比例する
			PLPow *= attePow; // 減衰適用

			// ライト色を加算する
			LightCol.rgb += g_PL[pi].Color.rgb * PLPow;

			// スペキュラ (処理が重いと感じた場合は消してもよい)
			{
				float SpePow = CalcSpecular(-vDir, normal, vCam);
				SpePow *= attePow; // 範囲減衰
				// ライトスペキュラ色を加算
				LightSpe += g_PL[pi].Color * SpePow;
			}
		}
	}

	//============================================================
	// スポットライト
	//  ※このライトはトゥーンを使用しません
	//============================================================
	for (int si = 0; si < g_SL_Cnt; si++)
	{
		// このピクセルと光源の方向・距離を調べる
		float3 vDir = g_SL[si].Pos - wPos; // 光源への方向
		float dist = length(vDir); // 距離
//		vDir = normalize(vDir); // 正規化した光源への方向
		vDir /= dist; // 正規化した光源への方向

		// スポット光源の向きの角度差を求める
		float dt = acos(saturate(dot(-vDir, g_SL[si].Dir)));

		float innerAng = (g_SL[si].MinAngle * 0.5); // 内側コーン(100%光る部分)の角度の半分
		float outerAng = (g_SL[si].MaxAngle * 0.5); // 外側コーン(減衰する部分)の角度の半分

		// コーン内かどうかの角度判定
		if (dt <= outerAng)
		{
			// 法線と光の方向から内積で光の強さを求める
			float SLPow = dot(normal, vDir);
			SLPow = max(0, SLPow);

			// 距離による減衰を行う
			float attePow = 1 - saturate(dist / g_SL[si].Range);	// 1(近い=最大の明るさ)　～　0(遠い=完全に暗い)
			attePow = attePow;		// 光の強さは、距離の二乗に反比例する
			SLPow *= attePow;		// 減衰適用

			// 外側コーン角度による減衰
			SLPow *= saturate((outerAng - dt) / (outerAng - innerAng));

			// ライト色を加算する
			LightCol.rgb += g_SL[si].Color.rgb * SLPow;

			// スペキュラ (処理が重いと感じた場合は消してもよい)
			{
				
				float SpePow = CalcSpecular(-vDir, normal, vCam);
				SpePow *= attePow;	// 範囲減衰

				// ライトスペキュラ色を加算
				LightSpe += g_SL[si].Color * SpePow;

			}
		}

	}

}

//ピクセルシェーダ出力データ
struct PS_OUT{
	float4 Color : SV_Target0;  // 通常色
	float4 Bloom : SV_Target1;  // 発光色
};

//======================================================================
//
// ピクセルシェーダ
// [主な機能]
// ・法線マップ(視差マップ)    … 凸凹表現
// ・平行光源(最大3個)         … 太陽光的な、無限距離ライト
// ・ポイント光源(最大100個)   … 電球的な、有限距離の全方向ライト
// ・スポット光源(最大20個)    … 懐中電灯的な、有限距離の角度制限付きライト
// ・アルファテスト            … 一定以下の透明なピクセルは捨て、なかったことにする
// ・加算スフィアマッピング    … MMD風のつやつやメタリック表現
// ・キューブ環境マッピング    … 疑似的な映り込み表現
// ・距離フォグ                … 霧の効果
//
// ※別途、RenderTargetの1番目に、発光色を出力しています
//
//======================================================================
PS_OUT PS(VS_OUT In)
{
	// このピクセルからカメラへの方向
	float3 vCam = normalize(g_CamPos - In.wPos);

	//============================================================
	// 法線マップ
	//============================================================
	// 法線マップ用変換行列作成
	row_major float3x3 mTBN = { 
		normalize( In.wT ),     // X軸
		normalize( In.wB ),     // Y軸
		normalize( In.wN )      // Z軸
	};

	// 視差マップ用
	row_major float3x3 mInvTBN = transpose(mTBN);	// 転置行列(ある条件下の行列だと、逆行列と同じ意味になる)
	float3 vCamLocal = mul(vCam, mInvTBN);
	//In.UV += g_ParallaxVal * NormalTex.Sample(WrapSmp, In.UV).a * vCamLocal.xy; // 射影(2D)座標系で、視差ぶんUVをずらす。g_ParallaxValは調整用


	// 法線マップ取得
	float4 normal = NormalTex.Sample(WrapSmp, In.UV);
	normal.xyz = normal.xyz*2.0 - 1.0;                     // 0～1から-1～1へ変換

	float3 w_normal = normalize(mul(normal.xyz, mTBN));    // ローカルな法線を行列で変換(面の向きを考慮したことになる)

	//w_normal = normalize(In.wN);	// 法線マップ未使用版

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	// 材質の色関係
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	float4 MateCol = 1;		// 自分(材質)の色(ライトは含まない)　DiffuseとTextureと環境マップなど

	//============================================================
	// 材質の色
	//============================================================
	float4 texCol = MeshTex.Sample(WrapSmp, In.UV);		// テクスチャ取得
	MateCol = g_Diffuse * texCol * g_MulColor;			// 材質色 * テクスチャ色 * 強制合成色   の合成

	//============================================================
	// アルファテスト
	//  一定値以下の透明なピクセルは捨てる(Zバッファにも書き込まれなくなる)
	//============================================================
	if (MateCol.a < 0.01)discard;


	//============================================================
	// MMD風 加算スフィアマップ
	// PMXEditorだと、ビュー座標系での法線をそのままUVとしてつかってるっぽい
	//============================================================
	float2 uv = normalize(mul(w_normal, (float3x3)g_mV)).xy;	// 法線をビュー座標系に変換
	uv = uv * 0.5 + 0.5;		// 3DからUVへ変換
	uv.y = 1 - uv.y;			// 
	MateCol.rgb += MaterialSphereTex.Sample(ClampSmp, uv).rgb;		// そこの色を加算

	//============================================================
	// キューブ環境マップ(映り込み)
	//============================================================
	{
		// リフレクションマップ(映り込み率テクスチャ)から取得
		float4 SphParam = SphereParamTex.Sample(WrapSmp, In.UV);

		float3 vRef = reflect(-vCam, w_normal);         // カメラからピクセルを見たときの反射ベクトル算出(-1～1)
		float4 envCol = CubeTex.Sample(ClampSmp, vRef);

		// 合成(ブレンド)
		MateCol.rgb = lerp(MateCol.rgb, envCol.rgb, SphParam.r);
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	// ライトの色関係
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	float4 BlendSpe = 0;    // 材質とライトのスペキュラを混ぜた色
	float4 FinalCol = 0;    // 材質の色とライトの色とスペキュラの全てを混ぜた、最終的な色を入れるための変数

	// ライト有効時
	if (g_LightEnable)
	{
		float3 LightCol = 0;	// ライトのみの総拡散色(Diffuse) ここに複数のライトの色が加算されていく(照らされていく)
		float4 LightSpe = 0;	// ライトのみの総反射色(Speculer) ここに複数のライトのスペキュラが加算されていく(照らされていく)

		// スペキュラマップのデータを取得
		float4 SpeMap = SpecularTex.Sample(WrapSmp, In.UV);

		//-------------------------------------------
		// ライティング計算を行い、このピクセルに当たる最終的な光の色(拡散色と反射色)を求める
		//  平行光源・ポイント光源・スポット光源
		//-------------------------------------------
		CalcLighting(In.wPos, w_normal, vCam, LightCol, LightSpe);

		// (おまけ)カメラライトスペキュラ
		// (法線マップを目立たたせる工夫　必要なければ消しても良い)
		{
			float SpePow = CalcSpecular(-vCam, w_normal, vCam);
			LightSpe += 0.5 * SpeMap.r * SpePow;			// 
		}

        // 環境光も加える
        LightCol.rgb += g_Ambient.rgb;

		// ライト拡散色(Diffuse)を、材質(マテリアル)に影響させる
		FinalCol = MateCol;
		FinalCol.rgb *= LightCol;

		// ライト反射色(Specular)を、材質(マテリアル)の「反射色」に影響させ 、最終色に加算
		BlendSpe = LightSpe * g_Specular * SpeMap.r;
		FinalCol += BlendSpe;

	}
	// ライト無効時
	else{
		BlendSpe = 0;           // スペキュラは無し
		FinalCol = MateCol;     // 材質色そのまま使う
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	//============================================================
	// 距離フォグ　最終的な色に対して霧効果を付ける
	//============================================================
	if(g_FogEnable){
		// フォグ 1～0(遠い)
		float fogPow = (g_FogRange.y - In.wvPos.z) / (g_FogRange.y - g_FogRange.x);
		fogPow = saturate(fogPow);	// 0～1の範囲内に切り詰める
		FinalCol.rgb = lerp(g_FogColor.rgb, FinalCol.rgb, fogPow);
	}

	//============================================================
	// 出力色
	//============================================================
	PS_OUT Out;
	// 最終的な出力色
	Out.Color = FinalCol;

	//============================================================
	// 発光色
	//============================================================
	// エミッシブマップから取得
	float4 emiCol = EmissiveTex.Sample(WrapSmp, In.UV);

	// 発光色
	//  光るものはスペキュラ、マテリアルのエミッシブ色、エミッシブマップの色
	Out.Bloom.rgb = BlendSpe.rgb + g_Emissive.rgb + emiCol.rgb*2;
	Out.Bloom.a = 1;

	return Out;
}
