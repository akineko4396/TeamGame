//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// エフェクト用のシェーダ
//　・ライティング無し
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Common.hlsl"

//===================================
// オブジェクト単位データ
//===================================
cbuffer cbPerObject : register(b1)
{
	row_major float4x4	g_mW;			// ワールド変換行列
	float4				g_Color;		// 色
};

// テクスチャ
Texture2D InputTex : register(t0);
Texture2D EmissiveTex : register(t1);
SamplerState ClampSmp : register(s1);

// 頂点シェーダ出力用
struct VS_OUT{
	float4 Pos : SV_Position;
	float2 UV  : TEXCOORD0;
	float4 Color : COLOR;
};

// 頂点シェーダ
VS_OUT VS(	float3 pos : POSITION,
			float2 uv : TEXCOORD0,
			float4 color : COLOR0
)
{
	VS_OUT Out = (VS_OUT)0;

	// 2D変換
	Out.Pos = mul(float4(pos, 1), g_mW);
	Out.Pos = mul(Out.Pos, g_mV);
	Out.Pos = mul(Out.Pos, g_mP);

	// UVはそのまま入れる
	Out.UV = uv;

	// 頂点色もそのまま渡す
	Out.Color = color;

	return Out;
}

//ピクセルシェーダ出力データ
struct PS_OUT {
	float4 Color : SV_Target0;
	float4 Bloom : SV_Target1;
};

// ピクセルシェーダ
PS_OUT PS(VS_OUT In) : SV_Target0
{

	// 色
	float4 Col = InputTex.Sample(ClampSmp, In.UV) * g_Color * In.Color;

	// エミッシブマップから取得
	float4 emiCol = EmissiveTex.Sample(ClampSmp, In.UV);

	PS_OUT Out;

	Out.Color = Col;

	Out.Bloom = emiCol;
	Out.Bloom.a = Out.Color.a;

	return Out;
}
