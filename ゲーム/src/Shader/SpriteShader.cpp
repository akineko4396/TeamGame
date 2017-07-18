#include "main.h"

void SpriteShader::Init()
{
	Release();

	//hlslファイル読み込み
	UINT shaderFlag = D3DXSHADER_DEBUG;

	// 頂点シェーダ作成
	YsVertexTypeData vtd = YsVertex_Pos_UV::GetVertexTypeData();
	m_VS.Create("Shader/Sprite.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// ピクセルシェーダ
	m_PS.Create("Shader/Sprite.hlsl", "PS", "ps_5_0", shaderFlag);

	//コンスタントバッファー作成
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

	// 現在のDepthステート記憶
	m_dsBackUp.SetAll_GetState();
	// Z書き込み、Z判定無効化
	YsDepthStencilState ds;
	ds.Set_FromDesc(m_dsBackUp.GetDesc());
	ds.Set_ZEnable(false);
	ds.Set_ZWriteEnable(false);
	ds.SetState();

	// 各シェーダセット
	m_VS.SetShader();
	m_PS.SetShader();

	// 定数バッファセット
	m_cb1_PerObject.SetVS();
	m_cb1_PerObject.SetPS();
}

void SpriteShader::End()
{
	if(!m_bBegin)return;
	m_bBegin = false;

	// Depthステート復元
	m_dsBackUp.SetState();
}

void SpriteShader::Draw2D(ID3D11ShaderResourceView* srcTex, float x, float y, float w, float h, const YsVec4* color)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_bBegin;
	if(!bBgn)Begin();

	// 定数バッファ書き込み
	m_cb1_PerObject.m_Data.m_Color = *color;
	m_cb1_PerObject.WriteData();

	// テクスチャ
	if(srcTex){
		YsDx.GetDevContext()->PSSetShaderResources(0, 1, &srcTex);
	}
	else{
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	// 四角形ポリゴン描画
	YsDx.DrawQuad((int)x, (int)y, (int)w, (int)h);

	// セットされているテクスチャの解除
	YsDx.RemoveTexturePS(0);

	if(!bBgn)End();
}

// 2D線描画
void SpriteShader::DrawLine2D(const YsVec3& p1, const YsVec3& p2, const YsVec4* color)
{
	// もし開始していない場合は、開始(最後にEnd())
	bool bBgn = m_bBegin;
	if(!bBgn)Begin();

	// 定数バッファ書き込み
	m_cb1_PerObject.m_Data.m_Color = *color;
	m_cb1_PerObject.WriteData();

	// テクスチャ
	YsDx.GetWhiteTex()->SetTexturePS(0);

	//==================================================
	// ピクセル座標系 → 射影座標系へ変換
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

	// 線描画
	YsVertex_Pos_UV_Color vertices[] = {
		{ YsVec3(tx1, -ty1, 0), YsVec2(0, 0), YsVec4::One },
		{ YsVec3(tx2, -ty2, 0), YsVec2(0, 0), YsVec4::One }
	};

	m_poly.WriteVertexData(vertices, 2);

	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 描画
	m_poly.Draw(2);

	// セットされているテクスチャの解除
	YsDx.RemoveTexturePS(0);

	if(!bBgn)End();
}