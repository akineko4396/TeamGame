//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 3D���f���p�̃V�F�[�_
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================
// �R���X�^���g(�萔)�o�b�t�@�̍쐬
// �萔�o�b�t�@�Ƃ̓O���[�o���萔���O���[�v���������BVC������f�[�^��n����ꏊ�B
// Direct3D11�ł̓O���[�o���萔�́A���̒萔�o�b�t�@�ɂ���K�v������B
//
// register(b0)��b0���萔�o�b�t�@�̃O���[�v�ԍ�
// �ő�14��(b0�`b13)����
// VC������͂��̔ԍ����g���Ē萔�o�b�t�@���Z�b�g����
//
// �萔�o�b�t�@�ɂ̓p�b�L���O�K���Ƃ������̂�����A������ƈӎ����Ă��Ȃ���
// VC�����琳��ɒl�������Ă����Ȃ��̂Œ���
// [�萔�ϐ��̃p�b�L���O�K�� (DirectX HLSL)]
// https://msdn.microsoft.com/ja-jp/library/bb509632(v=vs.85).aspx
//===========================================================

#include "Common.hlsl"

//===================================
// [�萔�o�b�t�@]�I�u�W�F�N�g�P�ʃf�[�^
// packoffset�Ƃ� �c ���̒萔�o�b�t�@���̃f�[�^�ʒu���w��ł���B�ȗ������ꍇ�͎����Ō��肳���
//===================================
cbuffer cbPerObject : register(b1)
{
	row_major float4x4	g_mW	: packoffset(c0);	// ���[���h�s��

	// 
	float4  g_MulColor      : packoffset(c4.x);     // �������ɍ�������F �����I�ɐF��ύX�������ꍇ�Ɏg�p

	// ���C�g
	int		g_LightEnable	: packoffset(c5.x);		// ���C�e�B���O�L��/�����t���O

	// �t�H�O
	int		g_FogEnable		: packoffset(c6.x);		// �t�H�O�L��/�����t���O
	float3	g_FogColor		: packoffset(c6.y);		// �t�H�O�F
	float2	g_FogRange		: packoffset(c7);		// �t�H�O�͈�
};

//===================================
// [�萔�o�b�t�@]�}�e���A���P�ʃf�[�^
//===================================
cbuffer cbPerMaterial : register(b2)
{
	float4	g_Diffuse;			// �g�U�F(��{�F)
	float4	g_Ambient;			// ���F
	float4	g_Specular;			// �X�y�L����(����)
	float	g_SpePower;			// �X�y�L�����̉s��
	float4	g_Emissive;			// �G�~�b�V�u(���Ȕ���)

	float	g_ParallaxVal;		// �����x
};

//===================================
// [�萔�o�b�t�@]�I�u�W�F�N�g�P�ʃf�[�^(�X�L�����b�V���p)
//===================================
cbuffer cbPerSkinObject : register(b3)
{
	row_major float4x4	g_mWArray[MAX_BONE];	// �{�[���p�s��z��
};

//====================================================
// �e�N�X�`��
//  VC������Z�b�g����Ƃ��Ɏw�肷��ԍ��́At0�`t127�̈ʒu�ɑΉ����Ă���
//  �ő�128(t0�`t127)
//====================================================
Texture2D MeshTex			: register(t0);		// ���b�V���̒ʏ�e�N�X�`��(�f�B�t���[�Y�e�N�X�`��)
Texture2D EmissiveTex		: register(t1);		// �����e�N�X�`��
Texture2D ToonTex			: register(t2);		// �g�D�[���e�N�X�`��(�A�e�p)
Texture2D MaterialSphereTex : register(t3);		// �ގ��p���Z�X�t�B�A�}�b�v(MMD���ۂ��\�������邽��)
Texture2D SphereParamTex	: register(t4);		// �f�荞��(�X�t�B�A��)�}�b�v
Texture2D SpecularTex		: register(t5);		// �X�y�L�����}�b�v
Texture2D NormalTex			: register(t10);	// �@���}�b�v�e�N�X�`��

TextureCube CubeTex         : register(t20);    // �L���[�u���}�b�v


//====================================================
// �T���v��
//  VC������Z�b�g����Ƃ��Ɏw�肷��ԍ��́As0�`s15�̈ʒu�ɑΉ����Ă���
//  �ő�16(s0�`s15)
//====================================================
SamplerState WrapSmp : register(s0);	// Wrap�p�̃T���v��
SamplerState ClampSmp : register(s1);	// Clamp�p�̃T���v��



//===============================================
//
// ���_�V�F�[�_�[
//
//===============================================

// ���_�V�F�[�_�o�̓f�[�^
struct VS_OUT{
	float4 Pos		: SV_Position;		// 2D���W(�ˉe���W�n)
	float2 UV		: TEXCOORD0;		// UV
	float3 wT		: TEXCOORD1;		// �ڐ�(Tangent)
	float3 wB		: TEXCOORD2;		// �]�@��(BiNormal)
	float3 wN		: TEXCOORD3;		// �@��(Normal)
	float3 wPos		: TEXCOORD4;		// 3D���W(���[���h���W�n)
	float3 wvPos	: TEXCOORD5;		// 3D���W(�r���[���W�n)
};

//===============================================
// �X�^�e�B�b�N���b�V���p���_�V�F�[�_�[
//===============================================
VS_OUT VS(	float4 pos : POSITION,		// ���_���W(���[�J��)
			float3 tangent : TANGENT,	// �ڐ�
			float3 binormal : BINORMAL,	// �]�@��
			float3 normal : NORMAL,		// �@��
			float2 uv : TEXCOORD0		// UV
			)
{
	VS_OUT Out = (VS_OUT)0;
	
	// 2D�ϊ�
	Out.Pos = mul(pos, g_mW);
		Out.wPos = Out.Pos.xyz;
	Out.Pos = mul(Out.Pos, g_mV);
		Out.wvPos = Out.Pos.xyz;
	Out.Pos = mul(Out.Pos, g_mP);
	
	// UV�͂��̂܂ܓ����
	Out.UV = uv;
	
	// �R��̖@��
	Out.wT = normalize(mul(tangent, (float3x3)g_mW));
	Out.wB = normalize(mul(binormal, (float3x3)g_mW));
	Out.wN = normalize(mul(normal, (float3x3)g_mW));
	
	return Out;
}

//===============================================
// �X�L�����b�V���p���_�V�F�[�_�[
//===============================================
VS_OUT SkinVS( 	float4 pos : POSITION,
				float3 tangent : TANGENT,
				float3 binormal : BINORMAL,
				float3 normal : NORMAL,
				float2 uv : TEXCOORD0,
				float4 blendWeight : BLENDWEIGHT,	// �{�[���E�F�C�gx4
				uint4 blendIndex : BLENDINDICES		// �{�[���ԍ�x4
){
	VS_OUT Out;

	//----------------------------------------------------------------------	
	// 4�̃{�[���s��Œ��_���W��@�����u�����h���A3D���[���h���W�E�@�������߂�
	float3      Pos = 0.0f;         // �u�����h��̃��[���h���W
	float3      Normal = 0.0f;      // �u�����h��̃��[���h�@��Z
	float3      Binormal = 0.0f;    // �u�����h��̃��[���h�@��Y
	float3      Tangent = 0.0f;     // �u�����h��̃��[���h�@��X

	for(int i=0;i<4;i++){
		// ���W
		Pos += mul(pos, g_mWArray[blendIndex[i]]).xyz * blendWeight[i];
		// �@���R��
		Normal += mul(normal, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
		Binormal += mul(binormal, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
		Tangent += mul(tangent, (float3x3)g_mWArray[blendIndex[i]]) * blendWeight[i];
	}
	//----------------------------------------------------------------------	

	// ���W�ϊ�
	Out.wPos = Pos;
	Out.Pos = mul(float4(Pos, 1), g_mV);
		Out.wvPos = Out.Pos.xyz;
	Out.Pos = mul(Out.Pos, g_mP);

	// UV
	Out.UV = uv;

	// �@��
	Out.wT = Tangent;
	Out.wB = Binormal;
	Out.wN = Normal;

	return Out;
}

//===============================================
//
// �s�N�Z���V�F�[�_
//
//===============================================

//===============================================
// �X�y�L�����̋��x�����߂�
// <����>
//  [in] lightDir	�c ���C�g�̕���
//  [in] normal		�c �@���x�N�g��(���K���������̂�n������)
//  [in] vCam		�c �s�N�Z���̍��W����J�����ւ̕���(���K���������̂�n������)
// <�߂�l>�X�y�L�����̋���
//===============================================
float CalcSpecular(float3 lightDir, float3 normal, float3 vCam)
{
	float3 vRef = normalize(reflect(lightDir, normal));
	float SpePow = dot(vCam, vRef); // �J�����̊p�x��(-1�`1)

	SpePow = max(0, SpePow); // �}�C�i�X�͑ł�����
	SpePow = pow(SpePow, g_SpePower+0.001);		// 0�悾�ƂȂ񂩃��o�����l�ɂȂ����肷��̂ŁA���������l�����Z���Ă���

	return SpePow;
}

//===============================================
// ���C�e�B���O�v�Z���s��
// ���s�����A�_�����A�X�|�b�g�����̊g�U�F�A���ːF�����߂�
// <����>
//    [in]  wPos     �c �s�N�Z����3D���W
//    [in]  normal   �c �@��
//    [in]  vCam     �c �s�N�Z������J�����ւ̕���
//    [out] LightCol �c �ŏI�I�ȃ��C�g�̊g�U�F(Diffuse)������
//    [out] LightSpe �c �ŏI�I�ȃ��C�g�̔��ːF(Specular)������
//===============================================
void CalcLighting(float3 wPos, float3 normal, float3 vCam, inout float3 LightCol, inout float4 LightSpe)
{
	//============================================================
	// ���s�������C�e�B���O
	//============================================================
	// ���C�g���Ԃ񏈗�
	for (int di = 0; di < g_DL_Cnt; di++)
	{
		//============================================================
		// �����o�[�g�g�U�Ɩ�
		//============================================================
		float LPow = dot(-g_DL[di].Dir, normal);
		LPow = max(0, LPow);

		//============================================================
		// �g�D�[��
		//  ���C�e�B���O���ʂ̌��̋��������Ƀg�D�[���e�N�X�`������A�J�X�^�}�C�Y���ꂽ���̋������擾
		//============================================================
		float4 ToonCol = ToonTex.Sample(ClampSmp, float2(LPow, 0.5));


		//============================================================
		// �V���h�E�}�b�v�ł̌��̎Ւf����
		//============================================================











		//============================================================
		// ���C�g�F�����Z����
		//============================================================
		LightCol.rgb += g_DL[di].Color.rgb * ToonCol.rgb;

		//============================================================
		// �X�y�L����(�t�H�����ʔ���)
		//============================================================
		// �X�y�L�����v�Z
		/*
		float3 vRef = normalize(reflect(g_DL[di].Dir, w_normal));
		float SpePow = dot(vCam, vRef); // �J�����̊p�x��(-1�`1)
		SpePow = max(0, SpePow); // 0�`�P�ɂ���
		SpePow = pow(SpePow, g_SpePower+0.001);		// 0�悾�ƂȂ񂩃��o�����l�ɂȂ����肷��̂ŁA���������l�����Z���Ă���
		*/
		float SpePow = CalcSpecular(g_DL[di].Dir, normal, vCam);
		// ���C�g�X�y�L�����F�����Z
		LightSpe += g_DL[di].Color * SpePow;
	}

	//============================================================
	// �_��������(�|�C���g���C�g)
	//  �����̃��C�g�̓g�D�[�����g�p���܂���
	//============================================================
	// ���C�g���Ԃ񏈗�
	for (int pi = 0; pi < g_PL_Cnt; pi++)
	{
		// �F
		// ���̃s�N�Z���Ɠ_�����̋����𒲂ׂ�
		float3 vDir = g_PL[pi].Pos - wPos; // �_�����ւ̕���
		float dist = length(vDir); // ����

		// �͈͓�
		if (dist < g_PL[pi].Radius)
		{
			// �@���ƃ��C�g�̕���������ςŌ��̋��������߂�
			// �_�����ւ̕����Ɩ@���Ƃ̊p�x�̍������邳�ƂȂ�(�����o�[�g�Ɠ���)
	//		vDir = normalize(vDir);     //  ���K��
			vDir /= dist;               //  ���K��
			float PLPow = dot(normal, vDir);    // �@���ƃ��C�g�̕����Ƃ̊p�x (-1�`1)
			PLPow = saturate(PLPow);            // 0�`1�ɂ���

			// ���a�͈͂ɂ�錸�����s��  ���C�g�̍��W�ɂȂ�΂Ȃ�قǖ��邭�A���a���肬��͈Â�
			float attePow = 1 - saturate(dist / g_PL[pi].Radius); // 1(�߂�=�ő�̖��邳)�@�`�@0(����=���S�ɈÂ�)
			attePow *= attePow; // ���̋����́A�����̓��ɔ���Ⴗ��
			PLPow *= attePow; // �����K�p

			// ���C�g�F�����Z����
			LightCol.rgb += g_PL[pi].Color.rgb * PLPow;

			// �X�y�L���� (�������d���Ɗ������ꍇ�͏����Ă��悢)
			{
				float SpePow = CalcSpecular(-vDir, normal, vCam);
				SpePow *= attePow; // �͈͌���
				// ���C�g�X�y�L�����F�����Z
				LightSpe += g_PL[pi].Color * SpePow;
			}
		}
	}

	//============================================================
	// �X�|�b�g���C�g
	//  �����̃��C�g�̓g�D�[�����g�p���܂���
	//============================================================
	for (int si = 0; si < g_SL_Cnt; si++)
	{
		// ���̃s�N�Z���ƌ����̕����E�����𒲂ׂ�
		float3 vDir = g_SL[si].Pos - wPos; // �����ւ̕���
		float dist = length(vDir); // ����
//		vDir = normalize(vDir); // ���K�����������ւ̕���
		vDir /= dist; // ���K�����������ւ̕���

		// �X�|�b�g�����̌����̊p�x�������߂�
		float dt = acos(saturate(dot(-vDir, g_SL[si].Dir)));

		float innerAng = (g_SL[si].MinAngle * 0.5); // �����R�[��(100%���镔��)�̊p�x�̔���
		float outerAng = (g_SL[si].MaxAngle * 0.5); // �O���R�[��(�������镔��)�̊p�x�̔���

		// �R�[�������ǂ����̊p�x����
		if (dt <= outerAng)
		{
			// �@���ƌ��̕���������ςŌ��̋��������߂�
			float SLPow = dot(normal, vDir);
			SLPow = max(0, SLPow);

			// �����ɂ�錸�����s��
			float attePow = 1 - saturate(dist / g_SL[si].Range);	// 1(�߂�=�ő�̖��邳)�@�`�@0(����=���S�ɈÂ�)
			attePow = attePow;		// ���̋����́A�����̓��ɔ���Ⴗ��
			SLPow *= attePow;		// �����K�p

			// �O���R�[���p�x�ɂ�錸��
			SLPow *= saturate((outerAng - dt) / (outerAng - innerAng));

			// ���C�g�F�����Z����
			LightCol.rgb += g_SL[si].Color.rgb * SLPow;

			// �X�y�L���� (�������d���Ɗ������ꍇ�͏����Ă��悢)
			{
				
				float SpePow = CalcSpecular(-vDir, normal, vCam);
				SpePow *= attePow;	// �͈͌���

				// ���C�g�X�y�L�����F�����Z
				LightSpe += g_SL[si].Color * SpePow;

			}
		}

	}

}

//�s�N�Z���V�F�[�_�o�̓f�[�^
struct PS_OUT{
	float4 Color : SV_Target0;  // �ʏ�F
	float4 Bloom : SV_Target1;  // �����F
};

//======================================================================
//
// �s�N�Z���V�F�[�_
// [��ȋ@�\]
// �E�@���}�b�v(�����}�b�v)    �c �ʉ��\��
// �E���s����(�ő�3��)         �c ���z���I�ȁA�����������C�g
// �E�|�C���g����(�ő�100��)   �c �d���I�ȁA�L�������̑S�������C�g
// �E�X�|�b�g����(�ő�20��)    �c �����d���I�ȁA�L�������̊p�x�����t�����C�g
// �E�A���t�@�e�X�g            �c ���ȉ��̓����ȃs�N�Z���͎̂āA�Ȃ��������Ƃɂ���
// �E���Z�X�t�B�A�}�b�s���O    �c MMD���̂�⃁�^���b�N�\��
// �E�L���[�u���}�b�s���O    �c �^���I�ȉf�荞�ݕ\��
// �E�����t�H�O                �c ���̌���
//
// ���ʓr�ARenderTarget��1�ԖڂɁA�����F���o�͂��Ă��܂�
//
//======================================================================
PS_OUT PS(VS_OUT In)
{
	// ���̃s�N�Z������J�����ւ̕���
	float3 vCam = normalize(g_CamPos - In.wPos);

	//============================================================
	// �@���}�b�v
	//============================================================
	// �@���}�b�v�p�ϊ��s��쐬
	row_major float3x3 mTBN = { 
		normalize( In.wT ),     // X��
		normalize( In.wB ),     // Y��
		normalize( In.wN )      // Z��
	};

	// �����}�b�v�p
	row_major float3x3 mInvTBN = transpose(mTBN);	// �]�u�s��(����������̍s�񂾂ƁA�t�s��Ɠ����Ӗ��ɂȂ�)
	float3 vCamLocal = mul(vCam, mInvTBN);
	//In.UV += g_ParallaxVal * NormalTex.Sample(WrapSmp, In.UV).a * vCamLocal.xy; // �ˉe(2D)���W�n�ŁA�����Ԃ�UV�����炷�Bg_ParallaxVal�͒����p


	// �@���}�b�v�擾
	float4 normal = NormalTex.Sample(WrapSmp, In.UV);
	normal.xyz = normal.xyz*2.0 - 1.0;                     // 0�`1����-1�`1�֕ϊ�

	float3 w_normal = normalize(mul(normal.xyz, mTBN));    // ���[�J���Ȗ@�����s��ŕϊ�(�ʂ̌������l���������ƂɂȂ�)

	//w_normal = normalize(In.wN);	// �@���}�b�v���g�p��

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	// �ގ��̐F�֌W
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	float4 MateCol = 1;		// ����(�ގ�)�̐F(���C�g�͊܂܂Ȃ�)�@Diffuse��Texture�Ɗ��}�b�v�Ȃ�

	//============================================================
	// �ގ��̐F
	//============================================================
	float4 texCol = MeshTex.Sample(WrapSmp, In.UV);		// �e�N�X�`���擾
	MateCol = g_Diffuse * texCol * g_MulColor;			// �ގ��F * �e�N�X�`���F * ���������F   �̍���

	//============================================================
	// �A���t�@�e�X�g
	//  ���l�ȉ��̓����ȃs�N�Z���͎̂Ă�(Z�o�b�t�@�ɂ��������܂�Ȃ��Ȃ�)
	//============================================================
	if (MateCol.a < 0.01)discard;


	//============================================================
	// MMD�� ���Z�X�t�B�A�}�b�v
	// PMXEditor���ƁA�r���[���W�n�ł̖@�������̂܂�UV�Ƃ��Ă����Ă���ۂ�
	//============================================================
	float2 uv = normalize(mul(w_normal, (float3x3)g_mV)).xy;	// �@�����r���[���W�n�ɕϊ�
	uv = uv * 0.5 + 0.5;		// 3D����UV�֕ϊ�
	uv.y = 1 - uv.y;			// 
	MateCol.rgb += MaterialSphereTex.Sample(ClampSmp, uv).rgb;		// �����̐F�����Z

	//============================================================
	// �L���[�u���}�b�v(�f�荞��)
	//============================================================
	{
		// ���t���N�V�����}�b�v(�f�荞�ݗ��e�N�X�`��)����擾
		float4 SphParam = SphereParamTex.Sample(WrapSmp, In.UV);

		float3 vRef = reflect(-vCam, w_normal);         // �J��������s�N�Z���������Ƃ��̔��˃x�N�g���Z�o(-1�`1)
		float4 envCol = CubeTex.Sample(ClampSmp, vRef);

		// ����(�u�����h)
		MateCol.rgb = lerp(MateCol.rgb, envCol.rgb, SphParam.r);
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	// ���C�g�̐F�֌W
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	float4 BlendSpe = 0;    // �ގ��ƃ��C�g�̃X�y�L�������������F
	float4 FinalCol = 0;    // �ގ��̐F�ƃ��C�g�̐F�ƃX�y�L�����̑S�Ă��������A�ŏI�I�ȐF�����邽�߂̕ϐ�

	// ���C�g�L����
	if (g_LightEnable)
	{
		float3 LightCol = 0;	// ���C�g�݂̂̑��g�U�F(Diffuse) �����ɕ����̃��C�g�̐F�����Z����Ă���(�Ƃ炳��Ă���)
		float4 LightSpe = 0;	// ���C�g�݂̂̑����ːF(Speculer) �����ɕ����̃��C�g�̃X�y�L���������Z����Ă���(�Ƃ炳��Ă���)

		// �X�y�L�����}�b�v�̃f�[�^���擾
		float4 SpeMap = SpecularTex.Sample(WrapSmp, In.UV);

		//-------------------------------------------
		// ���C�e�B���O�v�Z���s���A���̃s�N�Z���ɓ�����ŏI�I�Ȍ��̐F(�g�U�F�Ɣ��ːF)�����߂�
		//  ���s�����E�|�C���g�����E�X�|�b�g����
		//-------------------------------------------
		CalcLighting(In.wPos, w_normal, vCam, LightCol, LightSpe);

		// (���܂�)�J�������C�g�X�y�L����
		// (�@���}�b�v��ڗ���������H�v�@�K�v�Ȃ���Ώ����Ă��ǂ�)
		{
			float SpePow = CalcSpecular(-vCam, w_normal, vCam);
			LightSpe += 0.5 * SpeMap.r * SpePow;			// 
		}

        // ������������
        LightCol.rgb += g_Ambient.rgb;

		// ���C�g�g�U�F(Diffuse)���A�ގ�(�}�e���A��)�ɉe��������
		FinalCol = MateCol;
		FinalCol.rgb *= LightCol;

		// ���C�g���ːF(Specular)���A�ގ�(�}�e���A��)�́u���ːF�v�ɉe������ �A�ŏI�F�ɉ��Z
		BlendSpe = LightSpe * g_Specular * SpeMap.r;
		FinalCol += BlendSpe;

	}
	// ���C�g������
	else{
		BlendSpe = 0;           // �X�y�L�����͖���
		FinalCol = MateCol;     // �ގ��F���̂܂܎g��
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	//============================================================
	// �����t�H�O�@�ŏI�I�ȐF�ɑ΂��Ė����ʂ�t����
	//============================================================
	if(g_FogEnable){
		// �t�H�O 1�`0(����)
		float fogPow = (g_FogRange.y - In.wvPos.z) / (g_FogRange.y - g_FogRange.x);
		fogPow = saturate(fogPow);	// 0�`1�͈͓̔��ɐ؂�l�߂�
		FinalCol.rgb = lerp(g_FogColor.rgb, FinalCol.rgb, fogPow);
	}

	//============================================================
	// �o�͐F
	//============================================================
	PS_OUT Out;
	// �ŏI�I�ȏo�͐F
	Out.Color = FinalCol;

	//============================================================
	// �����F
	//============================================================
	// �G�~�b�V�u�}�b�v����擾
	float4 emiCol = EmissiveTex.Sample(WrapSmp, In.UV);

	// �����F
	//  ������̂̓X�y�L�����A�}�e���A���̃G�~�b�V�u�F�A�G�~�b�V�u�}�b�v�̐F
	Out.Bloom.rgb = BlendSpe.rgb + g_Emissive.rgb + emiCol.rgb*2;
	Out.Bloom.a = 1;

	return Out;
}
