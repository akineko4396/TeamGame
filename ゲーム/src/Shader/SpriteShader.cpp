#include "main.h"

void SpriteShader::Init()
{
	Release();

	//hlsl�t�@�C���ǂݍ���
	UINT shaderFlag = D3DXSHADER_DEBUG;

	// ���_�V�F�[�_�쐬
	YsVertexTypeData vtd = YsVertex_Pos_UV::GetVertexTypeData();
	m_VS.Create("Shader/Sprite.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// �s�N�Z���V�F�[�_
	m_PS.Create("Shader/Sprite.hlsl", "PS", "ps_5_0", shaderFlag);

	//�R���X�^���g�o�b�t�@�[�쐬
	m_cb1_PerObject.Create(1);

	m_poly.Create(nullptr, 100, YsVertex_Pos_UV_Color::GetVertexTypeData(), true);

}

void SpriteShader::Release()
{
	m_cb1_PerObject.Release();

	m_VS.Release();
	m_PS.Release();
}

void SpriteShader::Begin()
{
	if(m_bBegin)return;
	m_bBegin = true;

	// ���݂�Depth�X�e�[�g�L��
	m_dsBackUp.SetAll_GetState();
	// Z�������݁AZ���薳����
	YsDepthStencilState ds;
	ds.Set_FromDesc(m_dsBackUp.GetDesc());
	ds.Set_ZEnable(false);
	ds.Set_ZWriteEnable(false);
	ds.SetState();

	// �e�V�F�[�_�Z�b�g
	m_VS.SetShader();
	m_PS.SetShader();

	// �萔�o�b�t�@�Z�b�g
	m_cb1_PerObject.SetVS();
	m_cb1_PerObject.SetPS();
}

void SpriteShader::End()
{
	if(!m_bBegin)return;
	m_bBegin = false;

	// Depth�X�e�[�g����
	m_dsBackUp.SetState();
}

void SpriteShader::Draw2D(ID3D11ShaderResourceView* srcTex, float x, float y, float w, float h, const YsVec4* color)
{
	// �����J�n���Ă��Ȃ��ꍇ�͊J�n����(�Ō��End())
	bool bBgn = m_bBegin;
	if(!bBgn)Begin();

	// �萔�o�b�t�@��������
	m_cb1_PerObject.m_Data.m_Color = *color;
	m_cb1_PerObject.WriteData();

	// �e�N�X�`��
	if(srcTex){
		YsDx.GetDevContext()->PSSetShaderResources(0, 1, &srcTex);
	}
	else{
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	// �l�p�`�|���S���`��
	YsDx.DrawQuad((int)x, (int)y, (int)w, (int)h);

	// �Z�b�g����Ă���e�N�X�`���̉���
	YsDx.RemoveTexturePS(0);

	if(!bBgn)End();
}

// 2D���`��
void SpriteShader::DrawLine2D(const YsVec3& p1, const YsVec3& p2, const YsVec4* color)
{
	// �����J�n���Ă��Ȃ��ꍇ�́A�J�n(�Ō��End())
	bool bBgn = m_bBegin;
	if(!bBgn)Begin();

	// �萔�o�b�t�@��������
	m_cb1_PerObject.m_Data.m_Color = *color;
	m_cb1_PerObject.WriteData();

	// �e�N�X�`��
	YsDx.GetWhiteTex()->SetTexturePS(0);

	//==================================================
	// �s�N�Z�����W�n �� �ˉe���W�n�֕ϊ�
	//==================================================
	UINT pNumVierports = 1;
	D3D11_VIEWPORT vp;
	YsDx.GetDevContext()->RSGetViewports(&pNumVierports, &vp);

	float fConvX = (vp.Width * 0.5f);
	float fConvY = (vp.Height * 0.5f);

	float tx1 = p1.x / fConvX - 1.0f;
	float ty1 = p1.y / fConvY - 1.0f;

	float tx2 = p2.x / fConvX - 1.0f;
	float ty2 = p2.y / fConvY - 1.0f;

	// ���`��
	YsVertex_Pos_UV_Color vertices[] = {
		{ YsVec3(tx1, -ty1, 0), YsVec2(0, 0), YsVec4::One },
		{ YsVec3(tx2, -ty2, 0), YsVec2(0, 0), YsVec4::One }
	};

	m_poly.WriteVertexData(vertices, 2);

	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �`��
	m_poly.Draw(2);

	// �Z�b�g����Ă���e�N�X�`���̉���
	YsDx.RemoveTexturePS(0);

	if(!bBgn)End();
}