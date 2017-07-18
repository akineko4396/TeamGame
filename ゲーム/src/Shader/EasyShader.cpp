#include "main.h"

bool EasyShader::Init()
{
	Release();

	UINT shaderFlag = D3DXSHADER_DEBUG;

	//============================================
	// HLSL����V�F�[�_�쐬
	//============================================
	// ���_�V�F�[�_�쐬
	YsVertexTypeData vtd = YsVertex_Pos_UV_TBN::GetVertexTypeData();			// �g�p���钸�_�`���B�`�悷�郁�b�V���̒��_�̃��C�A�E�g�����炩���ߋ����Ă����K�v������
	m_VS.Create("Shader/Easy.hlsl", "VS", "vs_5_0", shaderFlag, vtd);

	// ���_�V�F�[�_�쐬(�X�L�����b�V���p)
	vtd = YsVertex_Pos_UV_TBN_Skin::GetVertexTypeData();
	m_SkinVS.Create("Shader/Easy.hlsl", "SkinVS", "vs_5_0", shaderFlag, vtd);
	
	// �s�N�Z���V�F�[�_
	m_PS.Create("Shader/Easy.hlsl", "PS", "ps_5_0", shaderFlag);

	//============================================
	// �萔�o�b�t�@�[�쐬
	//============================================
	m_cb0_PerFrame.Create(0);		// �����̔ԍ��́AHLSL����register(b0)��b0�ɑ�������B�܂��ASetPS�֐����ɂ��̔ԍ��͕ύX�ł��܂��B
	m_cb1_PerObject.Create(1);
	m_cb2_PerMaterial.Create(2);
	m_cb3_PerSkinObject.Create(3);

	return true;
}

void EasyShader::Release()
{
	//============================================
	// �e�I�u�W�F�N�g���
	//============================================

	// �萔�o�b�t�@���
	m_cb0_PerFrame.Release();
	m_cb1_PerObject.Release();
	m_cb2_PerMaterial.Release();
	m_cb3_PerSkinObject.Release();

	// �V�F�[�_���
	m_VS.Release();
	m_SkinVS.Release();
	m_PS.Release();

}

void EasyShader::WritePerFrameData()
{
	// �r���[�s��
	m_cb0_PerFrame.m_Data.mV = YsCamera::LastCam.mView;

	// �ˉe�s��
	m_cb0_PerFrame.m_Data.mP = YsCamera::LastCam.mProj;

	// �J�������W
	m_cb0_PerFrame.m_Data.CamPos = YsCamera::LastCam.mCam.GetPos();

	// ���C�g����
	m_cb0_PerFrame.m_Data.LightDir.Set(0, -1, 0);

//	m_cb0_PerFrame.m_Data.LightDir = ShMgr.m_LightMgr.m_DirLight->Direction;
//	m_cb0_PerFrame.m_Data.LightDir.Normalize();

	// ��������
	m_cb0_PerFrame.WriteData();
}

void EasyShader::SetMaterial(const YsMaterial& mate)
{
	//========================================================
	// �ȈՓI�ɁA�e�ގ��̐F�����C�g�̐F�ƍ������Ē萔�o�b�t�@�ɏ�������
	//========================================================
	// ��{�F
	m_cb2_PerMaterial.m_Data.Diffuse = mate.Diffuse;
	// ���F
	m_cb2_PerMaterial.m_Data.Ambient.Set(0.2f, 0.2f, 0.2f, 0);
	// ���ːF
	m_cb2_PerMaterial.m_Data.Specular = mate.Specular;
	// ���˂̉s��
	m_cb2_PerMaterial.m_Data.SpePower = mate.Power;
	// �����F
	m_cb2_PerMaterial.m_Data.Emissive = mate.Emissive;

	//========================================================
	// �e�N�X�`���ƃT���v�����Z�b�g
	//========================================================

	// MeshTex
	if(mate.TexSet.MeshTex && mate.TexSet.MeshTex->GetTex()) {	// �e�N�X�`�������݂���
		// �e�N�X�`����HLSL����register(t0)�ɃZ�b�g(������0��t0�ɑ�������)
		mate.TexSet.MeshTex->SetTexturePS(0);
	}
	else{	// �e�N�X�`���������I�I
		// 1x1�̐F�e�N�X�`�����Z�b�g
		YsDx.GetWhiteTex()->SetTexturePS(0);
	}

	//========================================================
	// ���ۂ̒萔�o�b�t�@�I�u�W�F�N�g�ɏ�������
	//========================================================
	m_cb2_PerMaterial.WriteData();
}

void EasyShader::DrawModel(YsModelObject& mo, const YsMatrix* mat)
{
	// ���f����NULL�`�F�b�N
	if(mo.GetGameModel() == nullptr)return;

	// �{�[�����`�F�b�N
	if(mo.GetBoneTree().size() >= MAX_BONE) {
		MessageBox(0, "�{�[���������E�����Ă�", "", MB_OK);
		return;
	}

	//========================================================
	// �g�p����R���X�^���g(�萔)�o�b�t�@���f�o�C�X�փZ�b�g
	//========================================================
	m_cb0_PerFrame.SetVS();		// ���̒萔�o�b�t�@���A���_�V�F�[�_�ɃZ�b�g(�`�掞�ɂ��̒萔�o�b�t�@���g����)
	m_cb0_PerFrame.SetPS();		// ���̒萔�o�b�t�@���A�s�N�Z���V�F�[�_�ɃZ�b�g(�`�掞�ɂ��̒萔�o�b�t�@���g����)
	m_cb1_PerObject.SetVS();	// �ȉ����l�Ɏg�p����萔�o�b�t�@��S���Z�b�g����
	m_cb1_PerObject.SetPS();
	m_cb2_PerMaterial.SetVS();
	m_cb2_PerMaterial.SetPS();
	m_cb3_PerSkinObject.SetVS();
	m_cb3_PerSkinObject.SetPS();

	//========================================================
	// �I�u�W�F�N�g�P�ʗp�萔�o�b�t�@�Ƀf�[�^����������
	//========================================================
	// �s��
	// �s��w�肪����Ȃ�A���̍s����g��
	if (mat) {
		m_cb1_PerObject.m_Data.mW = *mat;
	}

	// ���ۂ̒萔�o�b�t�@�I�u�W�F�N�g�ɏ�������
	m_cb1_PerObject.WriteData();

	//================================================
	//
	// �X�^�e�B�b�N���b�V���`��
	//
	//================================================
	// �g�p����V�F�[�_���Z�b�g
	m_VS.SetShader();		// ���_�V�F�[�_���Z�b�g(�`�掞�ɂ��̒��_�V�F�[�_���g�p�����)
	m_PS.SetShader();		// �s�N�Z���V�F�[�_���Z�b�g(�`�掞�ɂ��̃s�N�Z���V�F�[�_���g�p�����)

	// ���b�V�����[�v(�X�^�e�B�b�N���b�V���̂�)
	auto& staticModels = mo.GetGameModel()->GetModelTbl_Static();
	for(UINT mi = 0; mi<staticModels.size(); mi++)
	{
		YsSingleModel* model = staticModels[mi].get();	// ���b�V�����f�����ꎞ�I�Ƀ|�C���^�ɂЂ���Ă���

		// ���b�V���`����Z�b�g(���b�V���̃f�[�^���f�o�C�X�փZ�b�g����)
		model->GetMesh()->SetDrawData();

		// �}�e���A�����[�v
		for(UINT i = 0; i<model->GetMaterials().size(); i++)
		{
			// �ʐ���0�Ȃ�X�L�b�v
			if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

			// �}�e���A���f�[�^
			YsMaterial& mate = model->GetMaterials()[i];

			// �}�e���A���A�e�N�X�`���A�T���v�����Z�b�g
			SetMaterial(mate);

			// �`��
			model->GetMesh()->DrawSubset(i);
		}

	}

	//================================================
	// �X�L�����b�V���`��
	//================================================
	auto& skinModels = mo.GetGameModel()->GetModelTbl_Skin();
	if(skinModels.size() > 0){
		// ���_�V�F�[�_���Z�b�g(�`�掞�ɂ��̒��_�V�F�[�_���g�p�����)
		m_SkinVS.SetShader();
		//���s�N�Z���V�F�[�_�͓�������g��


		// �S�{�[���s�����������
		auto& boneTree = mo.GetBoneTree();		// �{�[���z��
		for (UINT bn = 0; bn<boneTree.size(); bn++) {
			// �`��ɕK�v��OffsetWorld�s����Z�o���A�萔�o�b�t�@�̃f�[�^�ɓ����
			// �{�[����OffsetMat * �{�[����WorldMat ���`��ɕK�v�ȍs��ɂȂ�
			boneTree[bn]->CalcOffsetWorldMat(m_cb3_PerSkinObject.m_Data.mWArray[bn]);
		}
		// �{�[�����Ԃ�A���ۂɒ萔�o�b�t�@�I�u�W�F�N�g�ɏ�������
		m_cb3_PerSkinObject.WriteData(sizeof(YsMatrix) * boneTree.size());

		// ���b�V�����[�v(�X�L�����b�V���̂�)
		// �����Ƃ́A�X�^�e�B�b�N���b�V���̎��Ɠ����ł�
		for(UINT mi = 0; mi<skinModels.size(); mi++)
		{
			YsSingleModel* model= skinModels[mi].get();	// ���b�V�����f�����ꎞ�I�Ƀ|�C���^�ɂЂ���Ă���

			// ���b�V���`����Z�b�g(���b�V���̃f�[�^���f�o�C�X�փZ�b�g����)
			model->GetMesh()->SetDrawData();

			// �}�e���A�����[�v
			for(UINT i = 0; i<model->GetMaterials().size(); i++)
			{
				// �ʐ���0�Ȃ�X�L�b�v
				if (model->GetMesh()->GetSubset(i)->FaceCount == 0)continue;

				// �}�e���A���f�[�^
				YsMaterial& mate = model->GetMaterials()[i];

				// �}�e���A���A�e�N�X�`���Ȃǂ��Z�b�g
				SetMaterial(mate);

				// �`��
				model->GetMesh()->DrawSubset(i);
			}
		}
	}

}
