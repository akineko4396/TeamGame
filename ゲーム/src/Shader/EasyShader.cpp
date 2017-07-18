#include "main.h"

bool EasyShader::Init()
{
	Release();

	UINT shaderFlag = D3DXSHADER_DEBUG;

	//============================================
	// HLSLからシェーダ作成
	//============================================
	// 頂点シェーダ作成
	YsVertexTypeData vtd = YsVertex_Pos_UV_TBN::GetVertexTypeData();			// 使用する頂点形式。描画するメッシュの頂点のレイアウトをあらかじめ教えておく必要がある
	m_VS.Create("Shader/Easy.hlsl", "VS", "vs_5_0", shaderFlag, vtd);

	// 頂点シェーダ作成(スキンメッシュ用)
	vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
	m_SkinVS.Create("Shader/Easy.hlsl", "SkinVS", "vs_5_0", shaderFlag, vtd);
	
	// ピクセルシェーダ
	m_PS.Create("Shader/Easy.hlsl", "PS", "ps_5_0", shaderFlag);

	//============================================
	// 定数バッファー作成
	//============================================
	m_cb0_PerFrame.Create(0);		// 引数の番号は、HLSL側のregister(b0)のb0に相当する。また、SetPS関数時にこの番号は変更できます。
	m_cb1_PerObject.Create(1);
	m_cb2_PerMaterial.Create(2);
	m_cb3_PerSkinObject.Create(3);

	return true;
}

void EasyShader::Release()
{
	//============================================
	// 各オブジェクト解放
	//============================================

	// 定数バッファ解放
	m_cb0_PerFrame.Release();
	m_cb1_PerObject.Release();
	m_cb2_PerMaterial.Release();
	m_cb3_PerSkinObject.Release();

	// シェーダ解放
	m_VS.Release();
	m_SkinVS.Release();
	m_PS.Release();

}

void EasyShader::WritePerFrameData()
{
	// ビュー行列
	m_cb0_PerFrame.m_Data.mV = YsCamera::LastCam.mView;

	// 射影行列
	m_cb0_PerFrame.m_Data.mP = YsCamera::LastCam.mProj;

	// カメラ座標
	m_cb0_PerFrame.m_Data.CamPos = YsCamera::LastCam.mCam.GetPos();

	// ライト方向
	m_cb0_PerFrame.m_Data.LightDir.Set(0, -1, 0);

//	m_cb0_PerFrame.m_Data.LightDir = ShMgr.m_LightMgr.m_DirLight->Direction;
//	m_cb0_PerFrame.m_Data.LightDir.Normalize();

	// 書き込み
	m_cb0_PerFrame.WriteData();
}

void EasyShader::SetMaterial(const YsMaterial& mate)
{
	//========================================================
	// 簡易的に、各材質の色をライトの色と合成して定数バッファに書き込む
	//========================================================
	// 基本色
	m_cb2_PerMaterial.m_Data.Diffuse = mate.Diffuse;
	// 環境色
	m_cb2_PerMaterial.m_Data.Ambient.Set(0.2f, 0.2f, 0.2f, 0);
	// 反射色
	m_cb2_PerMaterial.m_Data.Specular = mate.Specular;
	// 反射の鋭さ
	m_cb2_PerMaterial.m_Data.SpePower = mate.Power;
	// 発光色
	m_cb2_PerMaterial.m_Data.Emissive = mate.Emissive;

	//========================================================
	// テクスチャとサンプラをセット
	//========================================================

	// MeshTex
	if(mate.TexSet.MeshTex && mate.TexSet.MeshTex->GetTex()) {	// テクスチャが存在する
		// テクスチャをHLSL側のregister(t0)にセット(引数の0がt0に相当する)
		mate.TexSet.MeshTex->SetTexturePS(0);
	}
	else{	// テクスチャが無い！！
		// 1x1の色テクスチャをセット
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	//========================================================
	// 実際の定数バッファオブジェクトに書き込み
	//========================================================
	m_cb2_PerMaterial.WriteData();
}

void EasyShader::DrawModel(YsModelObject& mo, const YsMatrix* mat)
{
	// モデルのNULLチェック
	if(mo.GetGameModel() == nullptr)return;

	// ボーン数チェック
	if(mo.GetBoneTree().size() >= MAX_BONE) {
		MessageBox(0, "ボーン数が限界超えてる", "", MB_OK);
		return;
	}

	//========================================================
	// 使用するコンスタント(定数)バッファをデバイスへセット
	//========================================================
	m_cb0_PerFrame.SetVS();		// この定数バッファを、頂点シェーダにセット(描画時にこの定数バッファが使われる)
	m_cb0_PerFrame.SetPS();		// この定数バッファを、ピクセルシェーダにセット(描画時にこの定数バッファが使われる)
	m_cb1_PerObject.SetVS();	// 以下同様に使用する定数バッファを全部セットする
	m_cb1_PerObject.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();
	m_cb3_PerSkinObject.SetVS();
	m_cb3_PerSkinObject.SetPS();

	//========================================================
	// オブジェクト単位用定数バッファにデータを書き込む
	//========================================================
	// 行列
	// 行列指定があるなら、その行列を使う
	if (mat) {
		m_cb1_PerObject.m_Data.mW = *mat;
	}

	// 実際の定数バッファオブジェクトに書き込み
	m_cb1_PerObject.WriteData();

	//================================================
	//
	// スタティックメッシュ描画
	//
	//================================================
	// 使用するシェーダをセット
	m_VS.SetShader();		// 頂点シェーダをセット(描画時にこの頂点シェーダが使用される)
	m_PS.SetShader();		// ピクセルシェーダをセット(描画時にこのピクセルシェーダが使用される)

	// メッシュループ(スタティックメッシュのみ)
	auto& staticModels = mo.GetGameModel()->GetModelTbl_Static();
	for(UINT mi = 0; mi<staticModels.size(); mi++)
	{
		YsSingleModel* model = staticModels[mi].get();	// メッシュモデルを一時的にポインタにひろってくる

		// メッシュ描画情報セット(メッシュのデータをデバイスへセットする)
		model->GetMesh()->SetDrawData();

		// マテリアルループ
		for(UINT i = 0; i<model->GetMaterials().size(); i++)
		{
			// 面数が0ならスキップ
			if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

			// マテリアルデータ
			YsMaterial& mate = model->GetMaterials()[i];

			// マテリアル、テクスチャ、サンプラをセット
			SetMaterial(mate);

			// 描画
			model->GetMesh()->DrawSubset(i);
		}

	}

	//================================================
	// スキンメッシュ描画
	//================================================
	auto& skinModels = mo.GetGameModel()->GetModelTbl_Skin();
	if(skinModels.size() > 0){
		// 頂点シェーダをセット(描画時にこの頂点シェーダが使用される)
		m_SkinVS.SetShader();
		//※ピクセルシェーダは同じやつを使う


		// 全ボーン行列を書き込む
		auto& boneTree = mo.GetBoneTree();		// ボーン配列
		for (UINT bn = 0; bn<boneTree.size(); bn++) {
			// 描画に必要なOffsetWorld行列を算出し、定数バッファのデータに入れる
			// ボーンのOffsetMat * ボーンのWorldMat が描画に必要な行列になる
			boneTree[bn]->CalcOffsetWorldMat(m_cb3_PerSkinObject.m_Data.mWArray[bn]);
		}
		// ボーン数ぶん、実際に定数バッファオブジェクトに書き込む
		m_cb3_PerSkinObject.WriteData(sizeof(YsMatrix) * boneTree.size());

		// メッシュループ(スキンメッシュのみ)
		// ※あとは、スタティックメッシュの時と同じです
		for(UINT mi = 0; mi<skinModels.size(); mi++)
		{
			YsSingleModel* model= skinModels[mi].get();	// メッシュモデルを一時的にポインタにひろってくる

			// メッシュ描画情報セット(メッシュのデータをデバイスへセットする)
			model->GetMesh()->SetDrawData();

			// マテリアルループ
			for(UINT i = 0; i<model->GetMaterials().size(); i++)
			{
				// 面数が0ならスキップ
				if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

				// マテリアルデータ
				YsMaterial& mate = model->GetMaterials()[i];

				// マテリアル、テクスチャなどをセット
				SetMaterial(mate);

				// 描画
				model->GetMesh()->DrawSubset(i);
			}
		}
	}

}
