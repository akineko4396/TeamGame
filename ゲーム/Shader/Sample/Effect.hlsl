//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// �G�t�F�N�g�p�̃V�F�[�_
//�@�E���C�e�B���O����
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
};

// �e�N�X�`��
Texture2D InputTex : register(t0);
Texture2D EmissiveTex : register(t1);
SamplerState ClampSmp : register(s1);

// ���_�V�F�[�_�o�͗p
struct VS_OUT{
	float4 Pos : SV_Position;
	float2 UV  : TEXCOORD0;
	float4 Color : COLOR;
};

// ���_�V�F�[�_
VS_OUT VS(	float3 pos : POSITION,
			float2 uv : TEXCOORD0,
			float4 color : COLOR0
)
{
	VS_OUT Out = (VS_OUT)0;

	// 2D�ϊ�
	Out.Pos = mul(float4(pos, 1), g_mW);
	Out.Pos = mul(Out.Pos, g_mV);
	Out.Pos = mul(Out.Pos, g_mP);

	// UV�͂��̂܂ܓ����
	Out.UV = uv;

	// ���_�F�����̂܂ܓn��
	Out.Color = color;

	return Out;
}

//�s�N�Z���V�F�[�_�o�̓f�[�^
struct PS_OUT {
	float4 Color : SV_Target0;
	float4 Bloom : SV_Target1;
};

// �s�N�Z���V�F�[�_
PS_OUT PS(VS_OUT In) : SV_Target0
{

	// �F
	float4 Col = InputTex.Sample(ClampSmp, In.UV) * g_Color * In.Color;

	// �G�~�b�V�u�}�b�v����擾
	float4 emiCol = EmissiveTex.Sample(ClampSmp, In.UV);

	PS_OUT Out;

	Out.Color = Col;

	Out.Bloom = emiCol;
	Out.Bloom.a = Out.Color.a;

	return Out;
}
