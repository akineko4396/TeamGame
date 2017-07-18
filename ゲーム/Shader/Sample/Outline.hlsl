//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 輪郭用のシェーダ
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
	float2				g_OutlineSize;	// 輪郭サイズ(x:最低 ～ y:最高幅)
	float				g_OutlineDist;	// 距離により輪郭の太さ調整する距離
};

//===================================
// オブジェクト単位データ(スキンメッシュ用)
//===================================
cbuffer cbPerSkinObject : register(b3)
{
	row_major float4x4	g_mWArray[MAX_BONE];	// ボーン用行列配列
};

// 頂点シェーダ出力用
struct VS_OUT{
	float4 Pos : SV_Position;
};

// 頂点シェーダ
VS_OUT VS(	float3 pos : POSITION,
			float3 normal : NORMAL,
			float2 uv : TEXCOORD0
)
{
	VS_OUT Out = (VS_OUT)0;

	float3  Pos = mul(float4(pos, 1), g_mW).xyz;

	float z = mul(float4(Pos, 1), g_mV).z;
	float zRate = 1 - saturate((g_OutlineDist - z) / g_OutlineDist);

	// 座標を法線方向にずらす
    //  カメラからの距離により、太さを変える
	float3 newPos = pos + normal * lerp(g_OutlineSize.x, g_OutlineSize.y, zRate);

	// 2D変換
	Out.Pos = mul(float4(newPos, 1), g_mW);
	Out.Pos = mul(Out.Pos, g_mV);
	Out.Pos = mul(Out.Pos, g_mP);

	return Out;
}

// スキンメッシュ用頂点シェーダー
VS_OUT SkinVS(float4 pos : POSITION,
			float3 tangent : TANGENT,
			float3 binormal : BINORMAL,
			float3 normal : NORMAL,
			float2 uv : TEXCOORD0,
			float4 blendWeight : BLENDWEIGHT,	// ボーンウェイトx4
			uint4 blendIndex : BLENDINDICES		// ボーン番号x4
){
	VS_OUT Out;

	float3      Pos = 0.0f;
	float3      Normal = 0.0f;

	// 4つの行列で頂点ブレンド
	for(int i = 0; i<4; i++){
		Pos += mul(pos, g_mWArray[blendIndex[i]]).xyz * blendWeight[i];
		Normal += mul(normal, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
	}

	float z = mul(float4(Pos, 1), g_mV).z;
	float zRate = 1 - saturate((g_OutlineDist - z) / g_OutlineDist);

	// 座標を法線方向にずらす
    //  カメラからの距離により、太さを変える
	Pos += normalize(Normal) * lerp(g_OutlineSize.x, g_OutlineSize.y, zRate);

	// 2D変換
	Out.Pos = mul(float4(Pos, 1), g_mV);
	Out.Pos = mul(Out.Pos, g_mP);

	return Out;
}


// ピクセルシェーダ
float4 PS(VS_OUT In) : SV_Target0
{
	return g_Color;
}
