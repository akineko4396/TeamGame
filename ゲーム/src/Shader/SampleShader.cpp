#include "main.h"

bool SampleShader::Init()
{
	Release();

	//hlslファイル読み込み
	UINT shaderFlag = D3DXSHADER_DEBUG;

	//============================================
	// モデル描画用
	//============================================
	// 頂点シェーダ作成
	YsVertexTypeData vtd = YsVertex_Pos_UV_TBN::GetVertexTypeData();
	m_VS.Create("Shader/Sample/Model.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// 頂点シェーダ作成(スキンメッシュ用)
	vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
	m_SkinVS.Create("Shader/Sample/Model.hlsl", "SkinVS", "vs_5_0", shaderFlag, vtd);
	// ピクセルシェーダ
	m_PS.Create("Shader/Sample/Model.hlsl", "PS", "ps_5_0", shaderFlag);

	//============================================
	// 輪郭描画用
	//============================================
	vtd = YsVertex_Pos_UV_TBN::GetVertexTypeData();
	// 頂点シェーダ作成
	m_OutlineVS.Create("Shader/Sample/Outline.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// 頂点シェーダ作成(スキンメッシュ用)
	vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
	m_OutlineSkinVS.Create("Shader/Sample/Outline.hlsl", "SkinVS", "vs_5_0", shaderFlag, vtd);
	// ピクセルシェーダ
	m_OutlinePS.Create("Shader/Sample/Outline.hlsl", "PS", "ps_5_0", shaderFlag);

	//============================================
	// Effect描画用
	//============================================
	vtd = YsVertex_Pos_UV_Color::GetVertexTypeData();
	// 頂点シェーダ作成
	m_EffectVS.Create("Shader/Sample/Effect.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// ピクセルシェーダ
	m_EffectPS.Create("Shader/Sample/Effect.hlsl", "PS", "ps_5_0", shaderFlag);

	//コンスタントバッファー作成
	m_cb0_PerFrame.Create(0);
	m_cb1_PerObject.Create(1);
	m_cb1_PerObject_Outline.Create(1);
	m_cb1_PerObject_Effect.Create(1);
	m_cb2_PerMaterial.Create(2);
	m_cb3_PerSkinObject.Create(3);


	// DrawQuad用ポリゴン作成
	m_poly.Create(nullptr, 100, YsVertex_Pos_UV_Color::GetVertexTypeData(), true);

	return true;
}

void SampleShader::Release()
{
	m_cb0_PerFrame.Release();
	m_cb1_PerObject.Release();
	m_cb1_PerObject_Outline.Release();
	m_cb1_PerObject_Effect.Release();
	m_cb2_PerMaterial.Release();
	m_cb3_PerSkinObject.Release();


	m_VS.Release();
	m_SkinVS.Release();
	m_PS.Release();

	m_EffectVS.Release();
	m_EffectPS.Release();

	m_OutlineVS.Release();
	m_OutlinePS.Release();

}

void SampleShader::WritePerFrameData() 
{
	// ビュー行列
	m_cb0_PerFrame.m_Data.mV = YsCamera::LastCam.mView;

	// 射影行列
	m_cb0_PerFrame.m_Data.mP = YsCamera::LastCam.mProj;

	// カメラ座標
	m_cb0_PerFrame.m_Data.CamPos = YsCamera::LastCam.mCam.GetPos();

	// 書き込み
	m_cb0_PerFrame.WriteData();

}


void SampleShader::SetMaterial(const YsMaterial& mate)
{

	//========================================================
	// マテリアル単位用のデータを書き込む
	//========================================================
	{
		// 拡散色(Diffuse)
		m_cb2_PerMaterial.m_Data.Diffuse = mate.Diffuse;
		// 環境色(Ambient)
//		m_cb2_PerMaterial.m_Data.Ambient.Set(mate.Ambient * ShMgr.m_LightMgr.m_AmbientLight, 0);
		m_cb2_PerMaterial.m_Data.Ambient.Set(ShMgr.m_LightMgr.m_AmbientLight, 0);		// 今回はマテリアルのアンビエントは無視
		// 反射色(Specular)
		m_cb2_PerMaterial.m_Data.Specular = mate.Specular;
		// 反射の鋭さ(Power)
		m_cb2_PerMaterial.m_Data.SpePower = mate.Power;
		// 発光色(Emissive)
		m_cb2_PerMaterial.m_Data.Emissive = mate.Emissive;
		
		//========================================================
		// テクスチャをセット
		//========================================================

		// MeshTex
		if(mate.TexSet.MeshTex && mate.TexSet.MeshTex->GetTex()) {	// テクスチャが存在スr場合は、それをセット
			mate.TexSet.MeshTex->SetTexturePS(0);
		}
		else{
			YsDx.GetWhiteTex()->SetTexturePS(0);					// テクスチャがない場合は、白テクスチャをセット
		}

		// エミッシブマップ
		if(mate.TexSet.ExtTex && mate.TexSet.ExtTex->GetTex()) {
			mate.TexSet.ExtTex->SetTexturePS(1);
		}
		else{
			YsDx.RemoveTexturePS(1);
		}

		// トゥーンテクスチャ
		if(mate.TexSet.ExtTex2 && mate.TexSet.ExtTex2->GetTex()) {
			mate.TexSet.ExtTex2->SetTexturePS(2);
		}
		else{
			YsDx.GetToonTex()->SetTexturePS(2);
		}

		// 材質加算スフィアマップ
		if(mate.TexSet.ExtTex3 && mate.TexSet.ExtTex3->GetTex()) {
			mate.TexSet.ExtTex3->SetTexturePS(3);
		}
		else{
			YsDx.RemoveTexturePS(3);
		}

		// リフレクションマップ
		if(mate.TexSet.ExtTex4 && mate.TexSet.ExtTex4->GetTex()) {
			mate.TexSet.ExtTex4->SetTexturePS(4);
		}
		else{
			YsDx.RemoveTexturePS(4);
		}

		// スペキュラマップ
		if(mate.TexSet.ExtTex5 && mate.TexSet.ExtTex5->GetTex()) {
			mate.TexSet.ExtTex5->SetTexturePS(5);
		}
		else{
			YsDx.GetWhiteTex()->SetTexturePS(5);
		}

		// 法線マップ
		if(mate.TexSet.NormalTex && mate.TexSet.NormalTex->GetTex()) {
			mate.TexSet.NormalTex->SetTexturePS(10);
		}
		else{
			YsDx.GetNormalTex()->SetTexturePS(10);
		}

	}

	// 定数バッファ書き込み
	m_cb2_PerMaterial.WriteData();
}


void SampleShader::DrawModel(YsModelObject& mo, const YsMatrix* mat)
{
	// モデルが無いなら終了
	if(mo.GetGameModel() == nullptr)return;

	// ボーン数が限界超えてる
	if(mo.GetBoneTree().size() >= MAX_BONE) {
		MessageBox(0, "ボーン数が限界超えてる", "", MB_OK);
		return;
	}

	//※表面描画、両面描画を切り替える(※毎回ステートをCreate()するとそれなりに重いので、本来は工夫する必要があります)
	// 現在のラスタライザステートを記憶
	YsRasterizeState rsBackUp;
	rsBackUp.SetAll_GetState();
	// 両面描画
	YsRasterizeState rsNone;
	rsNone.SetAll_GetState();
	rsNone.Set_CullMode_None();
	// 表面描画
	YsRasterizeState rsBack;
	rsBack.SetAll_GetState();
	rsBack.Set_CullMode_Back();


	//========================================================
	// 使用するコンスタント(定数)バッファをデバイスへセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject.SetVS();
	m_cb1_PerObject.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// 定数バッファにデータを書き込む
	//========================================================
	// --- オブジェクト単位用のデータ(スタティックメッシュ) ---
	// 行列指定があるなら、その行列を使う
	if(mat){
		m_cb1_PerObject.m_Data.mW = *mat;
	}

	// バッファに書き込み
	m_cb1_PerObject.WriteData();

	//================================================
	// スタティックメッシュ描画
	//================================================
	// 使用するシェーダをデバイスへセット
	m_VS.SetShader();		// 頂点シェーダをセット
	m_PS.SetShader();		// ピクセルシェーダをセット

	// メッシュループ
	auto& staticModels = mo.GetGameModel()->GetModelTbl_Static();	// スタティックメッシュのみの配列取得
	for(UINT mi = 0; mi<staticModels.size(); mi++) {
		YsSingleModel* model = staticModels[mi].get();	// メッシュモデル

		// メッシュ描画情報セット(頂点バッファ、インデックスバッファ、プリミティブ・トポロジーなどをデバイスへセットする)
		model->GetMesh()->SetDrawData();

		// マテリアルループ
		for(UINT i = 0; i<model->GetMaterials().size(); i++) {
			// 面数が0ならスキップ
			if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

			YsMaterial& mate = model ->GetMaterials()[i];

			if(mate.DF_IsBothFace()){	// 両面描画
				rsNone.SetState();
			}
			else{						// 表面のみ描画
				rsBack.SetState();
			}

			// マテリアル、テクスチャなどをセット
			SetMaterial(mate);

			// i番目のサブセットを描画
			model->GetMesh()->DrawSubset(i);
		}

	}

	//================================================
	// スキンメッシュ描画
	//================================================
	auto& skinModels = mo.GetGameModel()->GetModelTbl_Skin();	// スキンメッシュのみの配列取得
	if(skinModels.size() > 0){
		// 頂点シェーダをセット(スキンメッシュ用)
		m_SkinVS.SetShader();
		//※ピクセルシェーダは同じやつを使う

		// 定数バッファデータにボーン行列をセット
		auto& boneTree = mo.GetBoneTree();		// ボーン配列
		for(UINT bn = 0; bn<boneTree.size(); bn++) {
			// 描画に必要なOffsetWorld行列を算出し、定数バッファのデータに入れる
			boneTree[bn]->CalcOffsetWorldMat( m_cb3_PerSkinObject.m_Data.mWArray[bn] );
		}
		// 実際に書き込む
		m_cb3_PerSkinObject.WriteData(sizeof(YsMatrix) * boneTree.size());

		// スキンメッシュ用の定数バッファをセット
		m_cb3_PerSkinObject.SetVS();
		m_cb3_PerSkinObject.SetPS();

		// メッシュループ
		for(UINT mi = 0; mi<skinModels.size(); mi++) {
			YsSingleModel* model = skinModels[mi].get();	// メッシュモデル

			// メッシュ描画情報セット
			model->GetMesh()->SetDrawData();

			// マテリアルループ
			for(UINT i = 0; i<model->GetMaterials().size(); i++) {

				// 面数が0ならスキップ
				if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

				YsMaterial& mate = model->GetMaterials()[i];

				if(mate.DF_IsBothFace()){	// 両面描画
					rsNone.SetState();
				}
				else{						// 表面のみ描画
					rsBack.SetState();
				}

				// マテリアル、テクスチャなどをセット
				SetMaterial(mate);

				// 描画
				model->GetMesh()->DrawSubset(i);
			}
		}
	}

	// ラスタライザステートを戻す
	rsBackUp.SetState();
}

// ※メッシュ単品描画Version スタティックメッシュのみ描画
void SampleShader::DrawMeshOnly(YsMesh& mesh, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// 使用するコンスタント(定数)バッファをデバイスへセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject.SetVS();
	m_cb1_PerObject.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// 定数バッファにデータを書き込む
	//========================================================
	// --- オブジェクト単位用のデータ(スタティックメッシュ) ---
	// 行列指定があるなら、その行列を使う
	if(mat){
		m_cb1_PerObject.m_Data.mW = *mat;
	}

	// バッファに書き込み
	m_cb1_PerObject.WriteData();

	//================================================
	// スタティックメッシュ描画
	//================================================
	// 使用するシェーダをデバイスへセット
	m_VS.SetShader();		// 頂点シェーダをセット
	m_PS.SetShader();		// ピクセルシェーダをセット


	//
	if(color){
		m_cb2_PerMaterial.m_Data.Diffuse.Set(*color);
	}
	m_cb2_PerMaterial.m_Data.Emissive.Set(0, 0, 0, 0);
	m_cb2_PerMaterial.WriteData();

	YsDx.GetWhiteTex()->SetTexturePS(0);
	YsDx.RemoveTexturePS(1);
	YsDx.RemoveTexturePS(2);
	YsDx.RemoveTexturePS(3);
	YsDx.RemoveTexturePS(4);
	YsDx.RemoveTexturePS(5);

	// メッシュ描画情報セット(頂点バッファ、インデックスバッファ、プリミティブ・トポロジーなどをデバイスへセットする)
	mesh.SetDrawData();

	// マテリアル(サブセット)ループ
	for(UINT i = 0; i<mesh.GetSubset().size(); i++) {
		// i番目のサブセットを描画
		mesh.DrawSubset(i);
	}
}

void SampleShader::DrawModelOutline(YsModelObject& mo, const YsVec4* outlineColor, const YsVec2* outlineSize, const YsMatrix* mat)
{
	if(mo.GetBoneTree().size() >= MAX_BONE) {
		MessageBox(0, "ボーン数が限界超えてる", "", MB_OK);
		return;
	}

	// ラスタライザステート
	YsRasterizeState rsBackUp;
	rsBackUp.SetAll_GetState();

	YsRasterizeState rsFront;	// 裏面描画用
	rsFront.SetAll_GetState();
	rsFront.Set_CullMode_Front();
	rsFront.SetState();

	//========================================================
	// 使用するコンスタント(定数)バッファセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Outline.SetVS();
	m_cb1_PerObject_Outline.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();
	m_cb3_PerSkinObject.SetVS();	// スキンメッシュ用
	m_cb3_PerSkinObject.SetPS();	// スキンメッシュ用

	//========================================================
	// 定数バッファにデータ書き込み
	//========================================================
	// --- オブジェクト単位のデータ ---
	// 輪郭用パラメータセット
	if(outlineSize){
		m_cb1_PerObject_Outline.m_Data.OutlineSize = *outlineSize;
	}
	if(outlineColor){
		m_cb1_PerObject_Outline.m_Data.Color = *outlineColor;
	}
	// 行列
	// 行列指定があるなら、その行列を使う
	if (mat) {
		m_cb1_PerObject_Outline.m_Data.mW = *mat;
	}

	// 書き込み
	m_cb1_PerObject_Outline.WriteData();


	//================================================
	// スタティックメッシュ描画
	//================================================
	// 使用するシェーダ
	m_OutlineVS.SetShader();	// 頂点シェーダ設定
	m_OutlinePS.SetShader();	// ピクセルシェーダ設定
	// 
	auto& staticModels = mo.GetGameModel()->GetModelTbl_Static();
	for(UINT mi = 0; mi<staticModels.size(); mi++) {
		YsSingleModel* model = staticModels[mi].get();	// メッシュモデル

		// メッシュ描画情報セット
		model->GetMesh()->SetDrawData();

		// マテリアルループ
		for(UINT i = 0; i<model->GetMaterials().size(); i++) {
			// 面数が0ならスキップ
			if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

			// 描画
			model->GetMesh()->DrawSubset(i);
		}

	}

	//================================================
	// スキンメッシュ描画
	//================================================
	auto& skinModels = mo.GetGameModel()->GetModelTbl_Skin();
	if(skinModels.size() > 0){
		// 頂点シェーダ設定
		m_OutlineSkinVS.SetShader();

		// ボーン行列セット
		auto& boneTree = mo.GetBoneTree();		// ボーン配列
		for (UINT bn = 0; bn<boneTree.size(); bn++) {
			// 描画に必要なOffsetWorld行列を算出し、定数バッファのデータに入れる
			// ボーンのOffsetMat * ボーンのWorldMat が描画に必要な行列になる
			boneTree[bn]->CalcOffsetWorldMat(m_cb3_PerSkinObject.m_Data.mWArray[bn]);
		}
		m_cb3_PerSkinObject.WriteData(sizeof(YsMatrix) * boneTree.size());

		// 描画
		for(UINT mi = 0; mi<skinModels.size(); mi++) {
			YsSingleModel* model = skinModels[mi].get();	// メッシュコンテナ

			// メッシュ描画情報セット
			model->GetMesh()->SetDrawData();

			// マテリアルループ
			for(UINT i = 0; i<model->GetMaterials().size(); i++) {
				// 面数が0ならスキップ
				if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

				// 描画
				model->GetMesh()->DrawSubset(i);
			}
		}
	}

	// ラスタライザステートを戻す
	rsBackUp.SetState();
}


void SampleShader::DrawBillBoard(YsBillBoard& bb, const YsMatrix* mat)
{
	//========================================================
	// コンスタント(定数)バッファセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// オブジェクト単位用のデータを定数バッファに書き込む
	//========================================================
	// 行列
	if(mat) {
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// 色
	m_cb1_PerObject_Effect.m_Data.Color = bb.GetColor();
	// 書き込み
	m_cb1_PerObject_Effect.WriteData();

	// 各シェーダセット
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// テクスチャ
	if(bb.GetTex().MeshTex && bb.GetTex().MeshTex->GetTex()) {
		bb.GetTex().MeshTex->SetTexturePS(0);
	}
	else{
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	// エミッシブマップ
	if(bb.GetTex().ExtTex && bb.GetTex().ExtTex->GetTex()) {
		bb.GetTex().ExtTex->SetTexturePS(1);
	}
	else{
		YsDx.RemoveTexturePS(1);
	}

	// 描画
	bb.Draw();

}

// レーザ描画
void SampleShader::DrawLaser(YsLaser& lz, int mode)
{
	// ラスタライザステート
	YsRasterizeState rsBackUp;
	rsBackUp.SetAll_GetState();

	YsRasterizeState rsBoth;	// 両面描画用
	rsBoth.SetAll_GetState();
	rsBoth.Set_CullMode_None();
	rsBoth.SetState();

	//========================================================
	// コンスタント(定数)バッファセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// オブジェクト単位用のデータを定数バッファに書き込む
	//========================================================
	// 行列
	m_cb1_PerObject_Effect.m_Data.mW.CreateIdentity();
	// 色
	m_cb1_PerObject_Effect.m_Data.Color = lz.GetColor();
	// 書き込み
	m_cb1_PerObject_Effect.WriteData();

	// 各シェーダセット
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// テクスチャ
	if(lz.GetTex().MeshTex && lz.GetTex().MeshTex->GetTex()) {
		lz.GetTex().MeshTex->SetTexturePS(0);
	}
	else {
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	// エミッシブマップ
	if(lz.GetTex().ExtTex && lz.GetTex().ExtTex->GetTex()) {
		lz.GetTex().ExtTex->SetTexturePS(1);
	}
	else{
		YsDx.RemoveTexturePS(1);
	}

	// 描画
	if(mode == 0){
		lz.Draw(YsCamera::LastCam.mCam.GetPos());
	}
	else{
		lz.DrawStrip();
	}

	rsBackUp.SetState();
}

void SampleShader::DrawLine(const YsVec3& p1, const YsVec3& p2, const YsVec4* color)
{
	//========================================================
	// コンスタント(定数)バッファセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// オブジェクト単位用のデータを定数バッファに書き込む
	//========================================================
	// 行列
	m_cb1_PerObject_Effect.m_Data.mW.CreateIdentity();
	// 色
	if(color){
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// 書き込み
	m_cb1_PerObject_Effect.WriteData();

	// 各シェーダセット
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// テクスチャ
	YsDx.GetWhiteTex()->SetTexturePS(0);

	// 
	YsVertex_Pos_UV_Color vertices[] = {
		{ p1, YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ p2, YsVec2(0, 0), YsVec4(1, 1, 1, 1) }
	};

	// 四角形ポリゴン作成
	m_poly.WriteVertexData(vertices, 2);

	// 描画用データをセット
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 描画
	m_poly.Draw(2);
}

void SampleShader::DrawBoxLine(const YsVec3& vMin, const YsVec3& vMax, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// コンスタント(定数)バッファセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// オブジェクト単位用のデータを定数バッファに書き込む
	//========================================================
	// 行列
	if(mat){
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// 色
	if(color){
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// 書き込み
	m_cb1_PerObject_Effect.WriteData();

	// 各シェーダセット
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// テクスチャ
	YsDx.GetWhiteTex()->SetTexturePS(0);

	// 頂点
	YsVertex_Pos_UV_Color vertices[24] = {
		{ YsVec3(vMin.x, vMin.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMin.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMin.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMin.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMin.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMin.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMin.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMin.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },

		{ YsVec3(vMin.x, vMax.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMax.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMax.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMax.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMax.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMax.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMax.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMax.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },

		{ YsVec3(vMin.x, vMin.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMax.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },

		{ YsVec3(vMin.x, vMin.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMin.x, vMax.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },

		{ YsVec3(vMax.x, vMin.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMax.y, vMin.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },

		{ YsVec3(vMax.x, vMin.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ YsVec3(vMax.x, vMax.y, vMax.z), YsVec2(0, 0), YsVec4(1, 1, 1, 1) }
	};

	// 四角形ポリゴン作成
	m_poly.WriteVertexData(vertices, 24);

	// 描画用データをセット
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 描画
	m_poly.Draw(24);

}

void SampleShader::DrawCircleLine(float radius, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// コンスタント(定数)バッファセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// オブジェクト単位用のデータを定数バッファに書き込む
	//========================================================
	// 行列
	if(mat) {
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// 色
	if(color) {
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// 書き込み
	m_cb1_PerObject_Effect.WriteData();

	// 各シェーダセット
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// テクスチャ
	YsDx.GetWhiteTex()->SetTexturePS(0);

	static const int num = 32;
	YsVertex_Pos_UV_Color vertices[num + 1];

	for(int i = 0; i<num + 1; i++) {
		float x = cos(YsToRadian((i)* (360.0f / num))) * radius;
		float y = sin(YsToRadian((i)* (360.0f / num))) * radius;

		vertices[i].Pos.x = x;
		vertices[i].Pos.y = y;
		vertices[i].Pos.z = 0;

		vertices[i].UV.x = 0;
		vertices[i].UV.y = 0;
		if(color) {
			vertices[i].Color = *color;
		}
	}
	// ポリゴン作成
	m_poly.WriteVertexData(vertices, num + 1);
	// 描画用データをセット
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 描画
	m_poly.Draw(num + 1);
}

void SampleShader::DrawSphereLine(float radius, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// コンスタント(定数)バッファセット
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// オブジェクト単位用のデータを定数バッファに書き込む
	//========================================================
	// 行列
	if (mat) {
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// 色
	if (color) {
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// 書き込み
	m_cb1_PerObject_Effect.WriteData();

	// 各シェーダセット
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// テクスチャ
	YsDx.GetWhiteTex()->SetTexturePS(0);

	static const int num = 32;
	YsVertex_Pos_UV_Color vertices[num + 1];

	for (int i = 0; i<num + 1; i++) {
		float x = cos(YsToRadian((i)* (360.0f / num))) * radius;
		float y = sin(YsToRadian((i)* (360.0f / num))) * radius;

		vertices[i].Pos.x = x;
		vertices[i].Pos.y = y;
		vertices[i].Pos.z = 0;

		vertices[i].UV.x = 0;
		vertices[i].UV.y = 0;
		if (color) {
			vertices[i].Color = *color;
		}
	}
	// ポリゴン作成
	m_poly.WriteVertexData(vertices, num + 1);
	// 描画用データをセット
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	for (int n = 0; n < 3; n++) {

		m_cb1_PerObject_Effect.m_Data.mW = *mat;
		m_cb1_PerObject_Effect.m_Data.mW.RotateY_Local(n*(360.0f / 6));
		m_cb1_PerObject_Effect.WriteData();

		// 描画
		m_poly.Draw(num + 1);
	}

	m_cb1_PerObject_Effect.m_Data.mW = *mat;
	m_cb1_PerObject_Effect.m_Data.mW.RotateX_Local(90);
	m_cb1_PerObject_Effect.WriteData();

	// 描画
	m_poly.Draw(num + 1);
}
