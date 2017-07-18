#ifndef __SampleShader_h__
#define __SampleShader_h__

//==============================================================
//
// �T���v���V�F�[�_�N���X
// ���낢��ȋ@�\�����Ă��܂��̂ŁA�Q�l�ɂǂ����B
// <Model.hlsl�ɂ͈ȉ��̋@�\�����Ă��܂�>
//	�E���s����(�����o�[�g�g�U�Ɩ��@�ő�3��)
//	�E�_����(�|�C���g���C�e�B���O�@�ő�100��)
//	�E�X�|�b�g����(�X�|�b�g���C�e�B���O�@�ő�20��)
//	�E�g�D�[��(��L�̃��C�e�B���O���ʂ����Ƀg�D�[���p�e�N�X�`��������ۂɎg�����̋������擾����)
//	�E�t�H�����ʔ���(�X�y�L����)
//	�@�����C�g�̃f�[�^�́ALightManager�N���X�̓��e���g�p
//	�E�L���[�u���}�b�s���O(�^���f�荞��)
//	�E�@���}�b�s���O + �����}�b�s���O(���ʕ\��)
//	�E���Z�X�t�B�A�}�b�s���O(MMD�̃}�l  ������ۂ��\��)
//	�E�����t�H�O(������)
//	�E�G�~�b�V�u�}�b�v(���Ȕ���  ����������RT1�ɏo�͂����)
//
//	�ʓr�A�֊s�V�F�[�_(Outline.hlsl)�A�G�t�F�N�g�p�V�F�[�_(Effect.hlsl)������Ă܂�
//  ���ʂŎg�p����萔�o�b�t�@�Ȃǂ�Common.hlsl�ɋL�q���Ă��܂�
//
//==============================================================
class SampleShader {
public:
	//=============================================
	// �������E���
	//=============================================
	// ������
	bool Init();
	// ���
	void Release();
	// �`�撼�O��PerFrame�̐ݒ�
	void WritePerFrameData();

	//=============================================
	// �p�����[�^�ݒ�n
	//=============================================
	// ���C�e�B���OON/OFF
	void SetLightEnable(bool enable){
		m_cb1_PerObject.m_Data.LightEnable = enable ? 1 : 0;
	}

	// �t�H�O�ݒ�
	//  fogColor	�c �t�H�O�̐F
	//  fogRange	�c �t�H�O�͈̔�(x:�t�H�O��������n�߂鋗�� y:�t�H�O�����S�ɂ����鋗��)
	void SetFog(bool enable, const YsVec3* fogColor = nullptr, const YsVec2* fogRange = nullptr){
		m_cb1_PerObject.m_Data.FogEnable = enable ? 1 : 0;
		if(fogColor){
			m_cb1_PerObject.m_Data.FogColor = *fogColor;
		}
		if(fogRange){
			m_cb1_PerObject.m_Data.FogRange = *fogRange;
		}
	}

	// �F�ύX
	//  color��nullptr��n����(1,1,1,1)�ɂȂ�
	void SetMulColor(const YsVec4* color = &YsVec4::One){
		m_cb1_PerObject.m_Data.MulColor = *color;
	}

	//=============================================
	// �`��n
	//=============================================
private:
	// �}�e���A���ƃe�N�X�`���Ȃǂ��Z�b�g
	void SetMaterial(const YsMaterial& mate);

public:
	// ���f���`��
	//  mat		�c [�X�^�e�B�b�N���b�V����p]���[���h�s������Ă�����
	void DrawModel(YsModelObject& mo, const YsMatrix* mat = nullptr);
		// �����b�V���P�i�`��Version ���Ȃ݂ɃX�L�����b�V���ł��X�^�e�B�b�N���b�V���̕`��ƂȂ�܂�(�{�[���f�[�^����������)
	void DrawMeshOnly(YsMesh& mesh, const YsVec4* color, const YsMatrix* mat = nullptr);
		// ���f���֊s�`��
		//  outlineSize �c x:�ŏ��T�C�Y y:�ő�T�C�Y(�J�����̋����ɂ��ω�)
		void DrawModelOutline(YsModelObject& mo, const YsVec4* outlineColor, const YsVec2* outlineSize, const YsMatrix* mat = nullptr);


	// �r���{�[�h�`��
	void DrawBillBoard(YsBillBoard& bb, const YsMatrix* mat);
	// ���[�U�`��
	void DrawLaser(YsLaser& lz, int mode);

	// ���`��
	void DrawLine(const YsVec3& p1, const YsVec3& p2, const YsVec4* color);
	// BOX���`��
	void DrawBoxLine(const YsVec3& vMin, const YsVec3& vMax, const YsVec4* color, const YsMatrix* mat);

	// �~���`�� XY���ʂ̉~�ɂȂ�̂ŁA��]�������ꍇ��mat�Ŏw�肵�Ă�
	void DrawCircleLine(float radius, const YsVec4* color, const YsMatrix* mat);
	// �����`��
	void DrawSphereLine(float radius, const YsVec4* color, const YsMatrix* mat);


	// �R���p�C���ς݃V�F�[�_���t�@�C���ɕۑ�
	void SaveCsoToFile(){
		m_VS.SaveToFile();
		m_SkinVS.SaveToFile();
		m_PS.SaveToFile();
		m_OutlineVS.SaveToFile();
		m_OutlineSkinVS.SaveToFile();
		m_OutlinePS.SaveToFile();
		m_EffectVS.SaveToFile();
		m_EffectPS.SaveToFile();
	}

	// 
	SampleShader() {
	}

	~SampleShader() {
		Release();
	}

private:
	enum { MAX_BONE = 400 };	// �{�[���ő吔

	//=============================================
	// �e�V�F�[�_�I�u�W�F�N�g
	//=============================================
	// Model.hlsl�p
	YsVertexShader		m_VS;		// ���_�V�F�[�_
	YsVertexShader		m_SkinVS;	// �X�L�����b�V���p���_�V�F�[�_
	YsPixelShader		m_PS;		// �s�N�Z���V�F�[�_

	// Outline.hlsl�p
	YsVertexShader		m_OutlineVS;		// ���_�V�F�[�_
	YsVertexShader		m_OutlineSkinVS;	// �X�L�����b�V�����_�V�F�[�_
	YsPixelShader		m_OutlinePS;		// �s�N�Z���V�F�[�_

	// Effect.hlsl�p
	YsVertexShader		m_EffectVS;		// ���_�V�F�[�_
	YsPixelShader		m_EffectPS;		// �s�N�Z���V�F�[�_

	// ���I���_�o�b�t�@�p(���`��ȂǂŎg�p)
	YsPolygon				m_poly;

	

	//=============================================
	// �R���X�^���g(�萔)�o�b�t�@
	//=============================================
	// �P�t���[���P�ʂł̐ݒ�f�[�^
	struct cbPerFrame {
		YsMatrix	mV;				// �r���[�s��
		YsMatrix	mP;				// �ˉe�s��

		YsVec3		CamPos;			// �J�������W
			int			tmp2;

	};

	//+++++++++++++++++++++++++++++++++++++++++++++
	// Model.hlsl�p
	//+++++++++++++++++++++++++++++++++++++++++++++
	// �I�u�W�F�N�g�P�ʂł̐ݒ�f�[�^(�X�^�e�B�b�N���b�V���p)
	struct cbPerObject {
		YsMatrix	mW;				// ���[���h�s��
		YsVec4		MulColor;		// �������ō�������F �����I�ɐF��ύX�������ꍇ�Ɏg�p
		int			LightEnable;	// ���C�g�L��/����
			int tmp[3];

		int			FogEnable;		// �t�H�O�L��
		YsVec3		FogColor;		// �t�H�O�F
		YsVec2		FogRange;		// �t�H�O�͈�
			int	tmp3[2];

		cbPerObject(){
			LightEnable = 1;
			FogEnable = 0;
			MulColor.Set(1, 1, 1, 1);
		}
	};

	// �I�u�W�F�N�g�P�ʂł̐ݒ�f�[�^(�X�L�����b�V���p)
	struct cbPerSkinObject {
		YsMatrix	mWArray[MAX_BONE];	// �{�[���s��z��
	};

	// �}�e���A���P�ʂł̐ݒ�f�[�^
	struct cbPerMaterial {
		YsVec4		Diffuse = YsVec4(1,1,1,1);	// ��{�F
		YsVec4		Ambient = YsVec4(0,0,0,0);	// ���F
		YsVec4		Specular = YsVec4(1,1,1,1);	// �X�y�L�����F
		float		SpePower = 0;					// �X�y�L�����̉s��
			float tmp2[3];
		YsVec4		Emissive = YsVec4(0,0,0,0);	// �G�~�b�V�u
		float		ParallaxVal = 0;				// �����x
			float tmp3[3];

	};

	//+++++++++++++++++++++++++++++++++++++++++++++
	// Outline.hlsl�p
	//+++++++++++++++++++++++++++++++++++++++++++++
	// ���֊s�p
	struct cbPerObject_Outline {
		YsMatrix	mW;							// ���[���h�s��
		YsVec4		Color = YsVec4(1,1,1,1);	// �֊s�F
		YsVec2		OutlineSize = YsVec2(0.0005f, 0.01f);	// �֊s�T�C�Y(�Œ�`�ō���)
		float		OutlineDist = 10;	// �����ɂ��֊s�̑����𒲐�����l
			float		tmp;
	};

	//+++++++++++++++++++++++++++++++++++++++++++++
	// Effect.hlsl�p
	//+++++++++++++++++++++++++++++++++++++++++++++
	// ��Effect�p
	struct cbPerObject_Effect {
		YsMatrix	mW;				// ���[���h�s��
		YsVec4		Color = YsVec4(1,1,1,1);			// �F

	};

	// �A�v�� -> �V�F�[�_�[ �f�[�^���M�p�̃R���X�^���g�o�b�t�@�N���X
	// ����
	YsConstantBuffer<cbPerFrame>				m_cb0_PerFrame;			// �P�t���[���P�ʂł̍X�V

	// Model.hlsl�p
	YsConstantBuffer<cbPerObject>				m_cb1_PerObject;		// �I�u�W�F�N�g�P�ʂł̍X�V(�X�^�e�B�b�N���b�V���p)
	YsConstantBuffer<cbPerSkinObject>			m_cb3_PerSkinObject;	// �I�u�W�F�N�g�P�ʂł̍X�V(�X�L�����b�V���p)
	YsConstantBuffer<cbPerMaterial>				m_cb2_PerMaterial;		// �}�e���A���P�ʂł̍X�V

	// Outline.hlsl�p
	YsConstantBuffer<cbPerObject_Outline>		m_cb1_PerObject_Outline;	// �I�u�W�F�N�g�P�ʂł̍X�V(�֊s�p)

	// Effect.hlsl�p
	YsConstantBuffer<cbPerObject_Effect>		m_cb1_PerObject_Effect;	// �I�u�W�F�N�g�P�ʂł̍X�V(�G�t�F�N�g�p)

};

#endif
