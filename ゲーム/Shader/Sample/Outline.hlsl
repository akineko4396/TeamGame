//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// �֊s�p�̃V�F�[�_
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Common.hlsl"

//===================================
// �I�u�W�F�N�g�P�ʃf�[�^
//===================================
cbuffer cbPerObject : register(b1)
{
	row_major float4x4	g_mW;			// ���[���h�ϊ��s��
	float4				g_Color;		// �F
	float2				g_OutlineSize;	// �֊s�T�C�Y(x:�Œ� �` y:�ō���)
	float				g_OutlineDist;	// �����ɂ��֊s�̑����������鋗��
};

//===================================
// �I�u�W�F�N�g�P�ʃf�[�^(�X�L�����b�V���p)
//===================================
cbuffer cbPerSkinObject : register(b3)
{
	row_major float4x4	g_mWArray[MAX_BONE];	// �{�[���p�s��z��
};

// ���_�V�F�[�_�o�͗p
struct VS_OUT{
	float4 Pos : SV_Position;
};

// ���_�V�F�[�_
VS_OUT VS(	float3 pos : POSITION,
			float3 normal : NORMAL,
			float2 uv : TEXCOORD0
)
{
	VS_OUT Out = (VS_OUT)0;

	float3  Pos = mul(float4(pos, 1), g_mW).xyz;

	float z = mul(float4(Pos, 1), g_mV).z;
	float zRate = 1 - saturate((g_OutlineDist - z) / g_OutlineDist);

	// ���W��@�������ɂ��炷
    //  �J��������̋����ɂ��A������ς���
	float3 newPos = pos + normal * lerp(g_OutlineSize.x, g_OutlineSize.y, zRate);

	// 2D�ϊ�
	Out.Pos = mul(float4(newPos, 1), g_mW);
	Out.Pos = mul(Out.Pos, g_mV);
	Out.Pos = mul(Out.Pos, g_mP);

	return Out;
}

// �X�L�����b�V���p���_�V�F�[�_�[
VS_OUT SkinVS(float4 pos : POSITION,
			float3 tangent : TANGENT,
			float3 binormal : BINORMAL,
			float3 normal : NORMAL,
			float2 uv : TEXCOORD0,
			float4 blendWeight : BLENDWEIGHT,	// �{�[���E�F�C�gx4
			uint4 blendIndex : BLENDINDICES		// �{�[���ԍ�x4
){
	VS_OUT Out;

	float3      Pos = 0.0f;
	float3      Normal = 0.0f;

	// 4�̍s��Œ��_�u�����h
	for(int i = 0; i<4; i++){
		Pos += mul(pos, g_mWArray[blendIndex[i]]).xyz * blendWeight[i];
		Normal += mul(normal, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
	}

	float z = mul(float4(Pos, 1), g_mV).z;
	float zRate = 1 - saturate((g_OutlineDist - z) / g_OutlineDist);

	// ���W��@�������ɂ��炷
    //  �J��������̋����ɂ��A������ς���
	Pos += normalize(Normal) * lerp(g_OutlineSize.x, g_OutlineSize.y, zRate);

	// 2D�ϊ�
	Out.Pos = mul(float4(Pos, 1), g_mV);
	Out.Pos = mul(Out.Pos, g_mP);

	return Out;
}


// �s�N�Z���V�F�[�_
float4 PS(VS_OUT In) : SV_Target0
{
	return g_Color;
}
