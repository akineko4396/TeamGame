#include "main.h"

bool SampleShader::Init()
{
	Release();

	//hlsl�t�@�C���ǂݍ���
	UINT shaderFlag = D3DXSHADER_DEBUG;

	//============================================
	// ���f���`��p
	//============================================
	// ���_�V�F�[�_�쐬
	YsVertexTypeData vtd = YsVertex_Pos_UV_TBN::GetVertexTypeData();
	m_VS.Create("Shader/Sample/Model.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// ���_�V�F�[�_�쐬(�X�L�����b�V���p)
	vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
	m_SkinVS.Create("Shader/Sample/Model.hlsl", "SkinVS", "vs_5_0", shaderFlag, vtd);
	// �s�N�Z���V�F�[�_
	m_PS.Create("Shader/Sample/Model.hlsl", "PS", "ps_5_0", shaderFlag);

	//============================================
	// �֊s�`��p
	//============================================
	vtd = YsVertex_Pos_UV_TBN::GetVertexTypeData();
	// ���_�V�F�[�_�쐬
	m_OutlineVS.Create("Shader/Sample/Outline.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// ���_�V�F�[�_�쐬(�X�L�����b�V���p)
	vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
	m_OutlineSkinVS.Create("Shader/Sample/Outline.hlsl", "SkinVS", "vs_5_0", shaderFlag, vtd);
	// �s�N�Z���V�F�[�_
	m_OutlinePS.Create("Shader/Sample/Outline.hlsl", "PS", "ps_5_0", shaderFlag);

	//============================================
	// Effect�`��p
	//============================================
	vtd = YsVertex_Pos_UV_Color::GetVertexTypeData();
	// ���_�V�F�[�_�쐬
	m_EffectVS.Create("Shader/Sample/Effect.hlsl", "VS", "vs_5_0", shaderFlag, vtd);
	// �s�N�Z���V�F�[�_
	m_EffectPS.Create("Shader/Sample/Effect.hlsl", "PS", "ps_5_0", shaderFlag);

	//�R���X�^���g�o�b�t�@�[�쐬
	m_cb0_PerFrame.Create(0);
	m_cb1_PerObject.Create(1);
	m_cb1_PerObject_Outline.Create(1);
	m_cb1_PerObject_Effect.Create(1);
	m_cb2_PerMaterial.Create(2);
	m_cb3_PerSkinObject.Create(3);


	// DrawQuad�p�|���S���쐬
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
	// �r���[�s��
	m_cb0_PerFrame.m_Data.mV = YsCamera::LastCam.mView;

	// �ˉe�s��
	m_cb0_PerFrame.m_Data.mP = YsCamera::LastCam.mProj;

	// �J�������W
	m_cb0_PerFrame.m_Data.CamPos = YsCamera::LastCam.mCam.GetPos();

	// ��������
	m_cb0_PerFrame.WriteData();

}


void SampleShader::SetMaterial(const YsMaterial& mate)
{

	//========================================================
	// �}�e���A���P�ʗp�̃f�[�^����������
	//========================================================
	{
		// �g�U�F(Diffuse)
		m_cb2_PerMaterial.m_Data.Diffuse = mate.Diffuse;
		// ���F(Ambient)
//		m_cb2_PerMaterial.m_Data.Ambient.Set(mate.Ambient * ShMgr.m_LightMgr.m_AmbientLight, 0);
		m_cb2_PerMaterial.m_Data.Ambient.Set(ShMgr.m_LightMgr.m_AmbientLight, 0);		// ����̓}�e���A���̃A���r�G���g�͖���
		// ���ːF(Specular)
		m_cb2_PerMaterial.m_Data.Specular = mate.Specular;
		// ���˂̉s��(Power)
		m_cb2_PerMaterial.m_Data.SpePower = mate.Power;
		// �����F(Emissive)
		m_cb2_PerMaterial.m_Data.Emissive = mate.Emissive;
		
		//========================================================
		// �e�N�X�`�����Z�b�g
		//========================================================

		// MeshTex
		if(mate.TexSet.MeshTex && mate.TexSet.MeshTex->GetTex()) {	// �e�N�X�`�������݃Xr�ꍇ�́A������Z�b�g
			mate.TexSet.MeshTex->SetTexturePS(0);
		}
		else{
			YsDx.GetWhiteTex()->SetTexturePS(0);					// �e�N�X�`�����Ȃ��ꍇ�́A���e�N�X�`�����Z�b�g
		}

		// �G�~�b�V�u�}�b�v
		if(mate.TexSet.ExtTex && mate.TexSet.ExtTex->GetTex()) {
			mate.TexSet.ExtTex->SetTexturePS(1);
		}
		else{
			YsDx.RemoveTexturePS(1);
		}

		// �g�D�[���e�N�X�`��
		if(mate.TexSet.ExtTex2 && mate.TexSet.ExtTex2->GetTex()) {
			mate.TexSet.ExtTex2->SetTexturePS(2);
		}
		else{
			YsDx.GetToonTex()->SetTexturePS(2);
		}

		// �ގ����Z�X�t�B�A�}�b�v
		if(mate.TexSet.ExtTex3 && mate.TexSet.ExtTex3->GetTex()) {
			mate.TexSet.ExtTex3->SetTexturePS(3);
		}
		else{
			YsDx.RemoveTexturePS(3);
		}

		// ���t���N�V�����}�b�v
		if(mate.TexSet.ExtTex4 && mate.TexSet.ExtTex4->GetTex()) {
			mate.TexSet.ExtTex4->SetTexturePS(4);
		}
		else{
			YsDx.RemoveTexturePS(4);
		}

		// �X�y�L�����}�b�v
		if(mate.TexSet.ExtTex5 && mate.TexSet.ExtTex5->GetTex()) {
			mate.TexSet.ExtTex5->SetTexturePS(5);
		}
		else{
			YsDx.GetWhiteTex()->SetTexturePS(5);
		}

		// �@���}�b�v
		if(mate.TexSet.NormalTex && mate.TexSet.NormalTex->GetTex()) {
			mate.TexSet.NormalTex->SetTexturePS(10);
		}
		else{
			YsDx.GetNormalTex()->SetTexturePS(10);
		}

	}

	// �萔�o�b�t�@��������
	m_cb2_PerMaterial.WriteData();
}


void SampleShader::DrawModel(YsModelObject& mo, const YsMatrix* mat)
{
	// ���f���������Ȃ�I��
	if(mo.GetGameModel() == nullptr)return;

	// �{�[���������E�����Ă�
	if(mo.GetBoneTree().size() >= MAX_BONE) {
		MessageBox(0, "�{�[���������E�����Ă�", "", MB_OK);
		return;
	}

	//���\�ʕ`��A���ʕ`���؂�ւ���(������X�e�[�g��Create()����Ƃ���Ȃ�ɏd���̂ŁA�{���͍H�v����K�v������܂�)
	// ���݂̃��X�^���C�U�X�e�[�g���L��
	YsRasterizeState rsBackUp;
	rsBackUp.SetAll_GetState();
	// ���ʕ`��
	YsRasterizeState rsNone;
	rsNone.SetAll_GetState();
	rsNone.Set_CullMode_None();
	// �\�ʕ`��
	YsRasterizeState rsBack;
	rsBack.SetAll_GetState();
	rsBack.Set_CullMode_Back();


	//========================================================
	// �g�p����R���X�^���g(�萔)�o�b�t�@���f�o�C�X�փZ�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject.SetVS();
	m_cb1_PerObject.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �萔�o�b�t�@�Ƀf�[�^����������
	//========================================================
	// --- �I�u�W�F�N�g�P�ʗp�̃f�[�^(�X�^�e�B�b�N���b�V��) ---
	// �s��w�肪����Ȃ�A���̍s����g��
	if(mat){
		m_cb1_PerObject.m_Data.mW = *mat;
	}

	// �o�b�t�@�ɏ�������
	m_cb1_PerObject.WriteData();

	//================================================
	// �X�^�e�B�b�N���b�V���`��
	//================================================
	// �g�p����V�F�[�_���f�o�C�X�փZ�b�g
	m_VS.SetShader();		// ���_�V�F�[�_���Z�b�g
	m_PS.SetShader();		// �s�N�Z���V�F�[�_���Z�b�g

	// ���b�V�����[�v
	auto& staticModels = mo.GetGameModel()->GetModelTbl_Static();	// �X�^�e�B�b�N���b�V���݂̂̔z��擾
	for(UINT mi = 0; mi<staticModels.size(); mi++) {
		YsSingleModel* model = staticModels[mi].get();	// ���b�V�����f��

		// ���b�V���`����Z�b�g(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�v���~�e�B�u�E�g�|���W�[�Ȃǂ��f�o�C�X�փZ�b�g����)
		model->GetMesh()->SetDrawData();

		// �}�e���A�����[�v
		for(UINT i = 0; i<model->GetMaterials().size(); i++) {
			// �ʐ���0�Ȃ�X�L�b�v
			if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

			YsMaterial& mate = model ->GetMaterials()[i];

			if(mate.DF_IsBothFace()){	// ���ʕ`��
				rsNone.SetState();
			}
			else{						// �\�ʂ̂ݕ`��
				rsBack.SetState();
			}

			// �}�e���A���A�e�N�X�`���Ȃǂ��Z�b�g
			SetMaterial(mate);

			// i�Ԗڂ̃T�u�Z�b�g��`��
			model->GetMesh()->DrawSubset(i);
		}

	}

	//================================================
	// �X�L�����b�V���`��
	//================================================
	auto& skinModels = mo.GetGameModel()->GetModelTbl_Skin();	// �X�L�����b�V���݂̂̔z��擾
	if(skinModels.size() > 0){
		// ���_�V�F�[�_���Z�b�g(�X�L�����b�V���p)
		m_SkinVS.SetShader();
		//���s�N�Z���V�F�[�_�͓�������g��

		// �萔�o�b�t�@�f�[�^�Ƀ{�[���s����Z�b�g
		auto& boneTree = mo.GetBoneTree();		// �{�[���z��
		for(UINT bn = 0; bn<boneTree.size(); bn++) {
			// �`��ɕK�v��OffsetWorld�s����Z�o���A�萔�o�b�t�@�̃f�[�^�ɓ����
			boneTree[bn]->CalcOffsetWorldMat( m_cb3_PerSkinObject.m_Data.mWArray[bn] );
		}
		// ���ۂɏ�������
		m_cb3_PerSkinObject.WriteData(sizeof(YsMatrix) * boneTree.size());

		// �X�L�����b�V���p�̒萔�o�b�t�@���Z�b�g
		m_cb3_PerSkinObject.SetVS();
		m_cb3_PerSkinObject.SetPS();

		// ���b�V�����[�v
		for(UINT mi = 0; mi<skinModels.size(); mi++) {
			YsSingleModel* model = skinModels[mi].get();	// ���b�V�����f��

			// ���b�V���`����Z�b�g
			model->GetMesh()->SetDrawData();

			// �}�e���A�����[�v
			for(UINT i = 0; i<model->GetMaterials().size(); i++) {

				// �ʐ���0�Ȃ�X�L�b�v
				if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

				YsMaterial& mate = model->GetMaterials()[i];

				if(mate.DF_IsBothFace()){	// ���ʕ`��
					rsNone.SetState();
				}
				else{						// �\�ʂ̂ݕ`��
					rsBack.SetState();
				}

				// �}�e���A���A�e�N�X�`���Ȃǂ��Z�b�g
				SetMaterial(mate);

				// �`��
				model->GetMesh()->DrawSubset(i);
			}
		}
	}

	// ���X�^���C�U�X�e�[�g��߂�
	rsBackUp.SetState();
}

// �����b�V���P�i�`��Version �X�^�e�B�b�N���b�V���̂ݕ`��
void SampleShader::DrawMeshOnly(YsMesh& mesh, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// �g�p����R���X�^���g(�萔)�o�b�t�@���f�o�C�X�փZ�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject.SetVS();
	m_cb1_PerObject.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �萔�o�b�t�@�Ƀf�[�^����������
	//========================================================
	// --- �I�u�W�F�N�g�P�ʗp�̃f�[�^(�X�^�e�B�b�N���b�V��) ---
	// �s��w�肪����Ȃ�A���̍s����g��
	if(mat){
		m_cb1_PerObject.m_Data.mW = *mat;
	}

	// �o�b�t�@�ɏ�������
	m_cb1_PerObject.WriteData();

	//================================================
	// �X�^�e�B�b�N���b�V���`��
	//================================================
	// �g�p����V�F�[�_���f�o�C�X�փZ�b�g
	m_VS.SetShader();		// ���_�V�F�[�_���Z�b�g
	m_PS.SetShader();		// �s�N�Z���V�F�[�_���Z�b�g


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

	// ���b�V���`����Z�b�g(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�v���~�e�B�u�E�g�|���W�[�Ȃǂ��f�o�C�X�փZ�b�g����)
	mesh.SetDrawData();

	// �}�e���A��(�T�u�Z�b�g)���[�v
	for(UINT i = 0; i<mesh.GetSubset().size(); i++) {
		// i�Ԗڂ̃T�u�Z�b�g��`��
		mesh.DrawSubset(i);
	}
}

void SampleShader::DrawModelOutline(YsModelObject& mo, const YsVec4* outlineColor, const YsVec2* outlineSize, const YsMatrix* mat)
{
	if(mo.GetBoneTree().size() >= MAX_BONE) {
		MessageBox(0, "�{�[���������E�����Ă�", "", MB_OK);
		return;
	}

	// ���X�^���C�U�X�e�[�g
	YsRasterizeState rsBackUp;
	rsBackUp.SetAll_GetState();

	YsRasterizeState rsFront;	// ���ʕ`��p
	rsFront.SetAll_GetState();
	rsFront.Set_CullMode_Front();
	rsFront.SetState();

	//========================================================
	// �g�p����R���X�^���g(�萔)�o�b�t�@�Z�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Outline.SetVS();
	m_cb1_PerObject_Outline.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();
	m_cb3_PerSkinObject.SetVS();	// �X�L�����b�V���p
	m_cb3_PerSkinObject.SetPS();	// �X�L�����b�V���p

	//========================================================
	// �萔�o�b�t�@�Ƀf�[�^��������
	//========================================================
	// --- �I�u�W�F�N�g�P�ʂ̃f�[�^ ---
	// �֊s�p�p�����[�^�Z�b�g
	if(outlineSize){
		m_cb1_PerObject_Outline.m_Data.OutlineSize = *outlineSize;
	}
	if(outlineColor){
		m_cb1_PerObject_Outline.m_Data.Color = *outlineColor;
	}
	// �s��
	// �s��w�肪����Ȃ�A���̍s����g��
	if (mat) {
		m_cb1_PerObject_Outline.m_Data.mW = *mat;
	}

	// ��������
	m_cb1_PerObject_Outline.WriteData();


	//================================================
	// �X�^�e�B�b�N���b�V���`��
	//================================================
	// �g�p����V�F�[�_
	m_OutlineVS.SetShader();	// ���_�V�F�[�_�ݒ�
	m_OutlinePS.SetShader();	// �s�N�Z���V�F�[�_�ݒ�
	// 
	auto& staticModels = mo.GetGameModel()->GetModelTbl_Static();
	for(UINT mi = 0; mi<staticModels.size(); mi++) {
		YsSingleModel* model = staticModels[mi].get();	// ���b�V�����f��

		// ���b�V���`����Z�b�g
		model->GetMesh()->SetDrawData();

		// �}�e���A�����[�v
		for(UINT i = 0; i<model->GetMaterials().size(); i++) {
			// �ʐ���0�Ȃ�X�L�b�v
			if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

			// �`��
			model->GetMesh()->DrawSubset(i);
		}

	}

	//================================================
	// �X�L�����b�V���`��
	//================================================
	auto& skinModels = mo.GetGameModel()->GetModelTbl_Skin();
	if(skinModels.size() > 0){
		// ���_�V�F�[�_�ݒ�
		m_OutlineSkinVS.SetShader();

		// �{�[���s��Z�b�g
		auto& boneTree = mo.GetBoneTree();		// �{�[���z��
		for (UINT bn = 0; bn<boneTree.size(); bn++) {
			// �`��ɕK�v��OffsetWorld�s����Z�o���A�萔�o�b�t�@�̃f�[�^�ɓ����
			// �{�[����OffsetMat * �{�[����WorldMat ���`��ɕK�v�ȍs��ɂȂ�
			boneTree[bn]->CalcOffsetWorldMat(m_cb3_PerSkinObject.m_Data.mWArray[bn]);
		}
		m_cb3_PerSkinObject.WriteData(sizeof(YsMatrix) * boneTree.size());

		// �`��
		for(UINT mi = 0; mi<skinModels.size(); mi++) {
			YsSingleModel* model = skinModels[mi].get();	// ���b�V���R���e�i

			// ���b�V���`����Z�b�g
			model->GetMesh()->SetDrawData();

			// �}�e���A�����[�v
			for(UINT i = 0; i<model->GetMaterials().size(); i++) {
				// �ʐ���0�Ȃ�X�L�b�v
				if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

				// �`��
				model->GetMesh()->DrawSubset(i);
			}
		}
	}

	// ���X�^���C�U�X�e�[�g��߂�
	rsBackUp.SetState();
}


void SampleShader::DrawBillBoard(YsBillBoard& bb, const YsMatrix* mat)
{
	//========================================================
	// �R���X�^���g(�萔)�o�b�t�@�Z�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �I�u�W�F�N�g�P�ʗp�̃f�[�^��萔�o�b�t�@�ɏ�������
	//========================================================
	// �s��
	if(mat) {
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// �F
	m_cb1_PerObject_Effect.m_Data.Color = bb.GetColor();
	// ��������
	m_cb1_PerObject_Effect.WriteData();

	// �e�V�F�[�_�Z�b�g
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// �e�N�X�`��
	if(bb.GetTex().MeshTex && bb.GetTex().MeshTex->GetTex()) {
		bb.GetTex().MeshTex->SetTexturePS(0);
	}
	else{
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	// �G�~�b�V�u�}�b�v
	if(bb.GetTex().ExtTex && bb.GetTex().ExtTex->GetTex()) {
		bb.GetTex().ExtTex->SetTexturePS(1);
	}
	else{
		YsDx.RemoveTexturePS(1);
	}

	// �`��
	bb.Draw();

}

// ���[�U�`��
void SampleShader::DrawLaser(YsLaser& lz, int mode)
{
	// ���X�^���C�U�X�e�[�g
	YsRasterizeState rsBackUp;
	rsBackUp.SetAll_GetState();

	YsRasterizeState rsBoth;	// ���ʕ`��p
	rsBoth.SetAll_GetState();
	rsBoth.Set_CullMode_None();
	rsBoth.SetState();

	//========================================================
	// �R���X�^���g(�萔)�o�b�t�@�Z�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �I�u�W�F�N�g�P�ʗp�̃f�[�^��萔�o�b�t�@�ɏ�������
	//========================================================
	// �s��
	m_cb1_PerObject_Effect.m_Data.mW.CreateIdentity();
	// �F
	m_cb1_PerObject_Effect.m_Data.Color = lz.GetColor();
	// ��������
	m_cb1_PerObject_Effect.WriteData();

	// �e�V�F�[�_�Z�b�g
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// �e�N�X�`��
	if(lz.GetTex().MeshTex && lz.GetTex().MeshTex->GetTex()) {
		lz.GetTex().MeshTex->SetTexturePS(0);
	}
	else {
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	// �G�~�b�V�u�}�b�v
	if(lz.GetTex().ExtTex && lz.GetTex().ExtTex->GetTex()) {
		lz.GetTex().ExtTex->SetTexturePS(1);
	}
	else{
		YsDx.RemoveTexturePS(1);
	}

	// �`��
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
	// �R���X�^���g(�萔)�o�b�t�@�Z�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �I�u�W�F�N�g�P�ʗp�̃f�[�^��萔�o�b�t�@�ɏ�������
	//========================================================
	// �s��
	m_cb1_PerObject_Effect.m_Data.mW.CreateIdentity();
	// �F
	if(color){
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// ��������
	m_cb1_PerObject_Effect.WriteData();

	// �e�V�F�[�_�Z�b�g
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// �e�N�X�`��
	YsDx.GetWhiteTex()->SetTexturePS(0);

	// 
	YsVertex_Pos_UV_Color vertices[] = {
		{ p1, YsVec2(0, 0), YsVec4(1, 1, 1, 1) },
		{ p2, YsVec2(0, 0), YsVec4(1, 1, 1, 1) }
	};

	// �l�p�`�|���S���쐬
	m_poly.WriteVertexData(vertices, 2);

	// �`��p�f�[�^���Z�b�g
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �`��
	m_poly.Draw(2);
}

void SampleShader::DrawBoxLine(const YsVec3& vMin, const YsVec3& vMax, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// �R���X�^���g(�萔)�o�b�t�@�Z�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �I�u�W�F�N�g�P�ʗp�̃f�[�^��萔�o�b�t�@�ɏ�������
	//========================================================
	// �s��
	if(mat){
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// �F
	if(color){
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// ��������
	m_cb1_PerObject_Effect.WriteData();

	// �e�V�F�[�_�Z�b�g
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// �e�N�X�`��
	YsDx.GetWhiteTex()->SetTexturePS(0);

	// ���_
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

	// �l�p�`�|���S���쐬
	m_poly.WriteVertexData(vertices, 24);

	// �`��p�f�[�^���Z�b�g
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �`��
	m_poly.Draw(24);

}

void SampleShader::DrawCircleLine(float radius, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// �R���X�^���g(�萔)�o�b�t�@�Z�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �I�u�W�F�N�g�P�ʗp�̃f�[�^��萔�o�b�t�@�ɏ�������
	//========================================================
	// �s��
	if(mat) {
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// �F
	if(color) {
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// ��������
	m_cb1_PerObject_Effect.WriteData();

	// �e�V�F�[�_�Z�b�g
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// �e�N�X�`��
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
	// �|���S���쐬
	m_poly.WriteVertexData(vertices, num + 1);
	// �`��p�f�[�^���Z�b�g
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// �`��
	m_poly.Draw(num + 1);
}

void SampleShader::DrawSphereLine(float radius, const YsVec4* color, const YsMatrix* mat)
{
	//========================================================
	// �R���X�^���g(�萔)�o�b�t�@�Z�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();
	m_cb0_PerFrame.SetPS();
	m_cb1_PerObject_Effect.SetVS();
	m_cb1_PerObject_Effect.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();

	//========================================================
	// �I�u�W�F�N�g�P�ʗp�̃f�[�^��萔�o�b�t�@�ɏ�������
	//========================================================
	// �s��
	if (mat) {
		m_cb1_PerObject_Effect.m_Data.mW = *mat;
	}
	// �F
	if (color) {
		m_cb1_PerObject_Effect.m_Data.Color = *color;
	}
	// ��������
	m_cb1_PerObject_Effect.WriteData();

	// �e�V�F�[�_�Z�b�g
	m_EffectVS.SetShader();
	m_EffectPS.SetShader();

	// �e�N�X�`��
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
	// �|���S���쐬
	m_poly.WriteVertexData(vertices, num + 1);
	// �`��p�f�[�^���Z�b�g
	m_poly.SetDrawData(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	for (int n = 0; n < 3; n++) {

		m_cb1_PerObject_Effect.m_Data.mW = *mat;
		m_cb1_PerObject_Effect.m_Data.mW.RotateY_Local(n*(360.0f / 6));
		m_cb1_PerObject_Effect.WriteData();

		// �`��
		m_poly.Draw(num + 1);
	}

	m_cb1_PerObject_Effect.m_Data.mW = *mat;
	m_cb1_PerObject_Effect.m_Data.mW.RotateX_Local(90);
	m_cb1_PerObject_Effect.WriteData();

	// �`��
	m_poly.Draw(num + 1);
}
