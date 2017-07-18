#ifndef __EasyShader_h__
#define __EasyShader_h__

//==============================================================
//
// �׋��p�̒P���ȃV�F�[�_
//  Easy.hlsl���g�p���܂�
//
//==============================================================
class EasyShader{
public:
	//=============================================
	// �������E���
	//=============================================
	// ������
	//  �����ŁAHLSL�t�@�C�����璸�_�V�F�[�_��s�N�Z���V�F�[�_���쐬������A�萔�o�b�t�@���쐬������
	//  �V�F�[�_�`��ŕK�v�Ȃ��̂��쐬����
	bool Init();

	// ���
	//  ��������̂��������
	void Release();

	// �`�撼�O��PerFrame�̐ݒ�(��{��1�t���[���ɂP�񂾂��A�`��̒��O�Ŏ��s����BShaderManager�N���X�̃����o�֐����ŌĂ�ł܂�)
	//  ��ɁA�萔�o�b�t�@�ł���m_cb0_PerFrame�̓��e���X�V���Ă���
	void WritePerFrameData();

	//=============================================
	// �`��n
	//=============================================
private:
	// �}�e���A���̓��e��萔�o�b�t�@�ɏ������񂾂�A�e�N�X�`�����Z�b�g�����肷��
	void SetMaterial(const YsMaterial& mate);

public:
	// ���b�V���`��
	//  mat		�c [�X�^�e�B�b�N���b�V����p]���[���h�s������Ă�����
	void DrawModel(YsModelObject& mo, const YsMatrix* mat = nullptr);

	//=============================================
	// ���̑�
	//=============================================

	// �R���p�C���ς݃V�F�[�_���t�@�C���ɕۑ�
	void SaveCsoToFile(){
		m_VS.SaveToFile();
		m_SkinVS.SaveToFile();
		m_PS.SaveToFile();
	}

	EasyShader(){
	}

	~EasyShader(){
		Release();
	}

	//=============================================
private:

	// �{�[���ő吔
	enum { MAX_BONE = 400 };

	//=============================================
	// �V�F�[�_�I�u�W�F�N�g
	//=============================================
	YsVertexShader		m_VS;		// ���_�V�F�[�_
	YsVertexShader		m_SkinVS;	// �X�L�����b�V���p���_�V�F�[�_
	YsPixelShader		m_PS;		// �s�N�Z���V�F�[�_

	//=============================================
	// �R���X�^���g(�萔)�o�b�t�@
	//  �V�F�[�_���̃O���[�o���萔�Ƀf�[�^���������ނ��߂̂���
	//  DirectX9�̎��ƈႢ�A�萔�o�b�t�@�Ƃ����O���[�v�P�ʂŃO���[�o���萔����邱�ƂɂȂ�
	//  �ł��邾���A�X�V�p�x�P�ʂŃO���[�v�ɂ܂Ƃ߂邱��
	// <<���ӁI>>
	// ���K��16�o�C�g�̔{���ɂ��Ȃ��Ƃ����Ȃ��I
	// ������ɁAHLSL���̃p�b�L���O�K���ɍ��킹�Ȃ��ƃf�[�^���Y����
	// �����̂��߂ɁAtmp�Ƃ������ʂȕϐ���ǉ����Ă���
	//=============================================

	// �P�t���[���P�ʂł̍X�V�f�[�^
	//  �J�����Ƃ��A�P�t���[����1�񂭂炢���X�V���Ȃ��悤�ȃf�[�^
	struct cbPerFrame {
		YsMatrix	mV;				// �r���[�s��@(�s��̓x�N�g��4���Ȃ̂�16�̔{��)
		YsMatrix	mP;				// �ˉe�s��@�@(�s��̓x�N�g��4���Ȃ̂�16�̔{��)

		YsVec3		LightDir = YsVec3(0, -1, 0);		// ���C�g�����@(3D�x�N�g����12�o�C�g�Ȃ̂�4�o�C�g����Ȃ�)
		float		tmp;			//             (���̂���4�o�C�g�̕ϐ����˂�����)

		YsVec3		CamPos;			// �J�������W�@(��L�Ɠ���)
		int			tmp2;			// 

	};

	// �I�u�W�F�N�g�P�ʂł̍X�V�f�[�^(�X�^�e�B�b�N���b�V���p)
	//  ���f���̍s��Ƃ��A���f���P�ʂŃf�[�^���ύX��������
	struct cbPerObject {
		YsMatrix	mW;				// ���[���h�s��

		int			LightEnable = 1;	// ���C�g�L���t���O
			int			tmp[3];			// 16�o�C�g�ɑ����邽�߂�12�o�C�g�ǉ�(�p�b�L���O�K���ɂ��A�����ɒǉ����Ȃ��Ƃ����Ȃ�)

	};

	// �I�u�W�F�N�g�P�ʂł̍X�V�f�[�^(�X�L�����b�V���p)
	//  �X�L�����b�V�����f���̑S�{�[���s��
	struct cbPerSkinObject {
		YsMatrix	mWArray[MAX_BONE];
	};

	// �}�e���A���P�ʂł̍X�V�f�[�^
	//  �P�̃��f���ł��A�}�e���A���͕�������܂��B�Ȃ̂�cbPerObject���ł͂Ȃ��A�ʂɍ��
	struct cbPerMaterial {
		YsVec4		Diffuse = YsVec4(1, 1, 1, 1);		// ��{�F
		YsVec4		Ambient = YsVec4(0, 0, 0, 0);		// ���F
		YsVec4		Specular = YsVec4(1, 1, 1, 1);	// �X�y�L�����F
		float		SpePower = 0;						// �X�y�L�����̉s��
			float tmp2[3];								// 16�o�C�g�ɑ����邽�߂�12�o�C�g�ǉ�(�p�b�L���O�K���ɂ��A�����ɒǉ����Ȃ��Ƃ����Ȃ�)
		YsVec4		Emissive = YsVec4(0, 0, 0, 0);	// �G�~�b�V�u

	};

	// ��L�̍\���̂̌^�̒萔�o�b�t�@���쐬�I
	YsConstantBuffer<cbPerFrame>			m_cb0_PerFrame;			// �P�t���[���P�ʂł̍X�V
	YsConstantBuffer<cbPerObject>			m_cb1_PerObject;		// �I�u�W�F�N�g�P�ʂł̍X�V(�X�^�e�B�b�N���b�V���p)
	YsConstantBuffer<cbPerSkinObject>		m_cb3_PerSkinObject;	// �I�u�W�F�N�g�P�ʂł̍X�V(�X�L�����b�V���p)
	YsConstantBuffer<cbPerMaterial>			m_cb2_PerMaterial;		// �}�e���A���P�ʂł̍X�V
};

#endif
