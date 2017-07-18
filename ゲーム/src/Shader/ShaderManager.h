#ifndef __ShaderManager_h__
#define __ShaderManager_h__

#include "LightManager.h"

#include "EasyShader.h"
#include "SampleShader.h"
#include "SpriteShader.h"






//=======================================================================
//
// �V�F�[�_�Ǘ��N���X
//�@�e�V�F�[�_�`��֌W�̂��̊Ǘ�������N���X
// 
//
// �����Ƃł͒萔�o�b�t�@�͈ȉ��̂悤�Ɏg���\��ł�
//  cbuffer(b0�`b8)		�c ���R �V�F�[�_���Ƃɕς��܂�
//  cbuffer(b9)			�c (�Œ�)���C�g�f�[�^�Ƃ��Ďg�p�\��@b12�Ɠ��������ɂȂ邯�ǁA�׋��̂��ߎ����ō��܂��傤
//  cbuffer(b10)		�c (�Œ�)�V���h�E�}�b�v�Ŏg�p�\��
//  cbuffer(b11)		�c ���g�p�@���R�Ɏg���Ă�
//  cbuffer(b12)		�c (�Œ�)SampleShader�ł̃��C�g�f�[�^�Ŏg���Ă܂�
//  cbuffer(b13)		�c (�Œ�)�f�o�b�O�p�Ŏg�p�\��
//  �����̂�����̎d�l�́A�����ł��₷���悤�ɃJ�X�^�}�C�Y���Ă݂Ă��������B
//
// �����Ƃł̓T���v���X�e�[�g�͈ȉ��̂悤�Ɏg���\��ł�	
//  s0					�c Linear��Ԃ�Wrap�T���v��
//  s1					�c Linear��Ԃ�Clamp�T���v��
//  s2�`9				�c ���R�@�D���Ȃ悤�Ɏg���Ă�������
//  s10					�c ShadowMap�p Comparison�T���v��
//  s11�`s15			�c ���R�@�D���Ȃ悤�Ɏg���Ă�������
//
//=======================================================================
class ShaderManager {
public:
	//================================================
	// ���C�g
	//================================================
	LightManager		m_LightMgr;			// ���C�g�Ǘ��N���X

	//================================================
	// �X�e�[�g
	//================================================
	YsBlendState		m_bsAlpha;			// �������������[�h
	YsBlendState		m_bsAdd;			// ���Z�������[�h
	YsBlendState		m_bsNoAlpha;		// �A���t�@�������[�h

	//================================================
	//
	// �T���v��
	//
	//================================================
	YsSamplerState		m_smp0_Linear_Wrap;		// (s0) Linear��� & Wrap���[�h�p�T���v��
	YsSamplerState		m_smp1_Linear_Clamp;	// (s1) Linear��� & Clamp���[�h�p�T���v��


	//================================================
	//
	// �V�F�[�_�N���X
	//
	//================================================
	EasyShader		m_Es;				// �׋��p�̒P���ȃV�F�[�_
	SampleShader	m_Samp;				// ���낢��ȋ@�\����ꂽ�T���v���p�V�F�[�_�N���X
	SpriteShader	m_Sprite;			// 2D�`��p�V�F�[�_�N���X







	//================================================
	// ���̑�
	//================================================

	YsTexture		m_texCube;			// �L���[�u�}�b�v�e�N�X�`��(���}�b�s���O�p)



	//================================================
	// �ŏ��Ɉ����s
	//================================================
	void Init() {
		// �V�F�[�_�N���X������
		m_Es.Init();
		m_Samp.Init();
		m_Sprite.Init();







		//-------------------------------
		// �悭�g���u�����h�X�e�[�g���쐬
		//-------------------------------
		// �����������p�X�e�[�g
		m_bsAlpha.Set_Alpha(-1);
//		m_bsAlpha.Set_AlphaToCoverageEnable(true);
		m_bsAlpha.Create();
		// ���Z�����p�X�e�[�g
		m_bsAdd.Set_Add(-1);
//		m_bsAlpha.Set_AlphaToCoverageEnable(true);
		m_bsAdd.Create();
		// �A���t�@�l�������[�h
		m_bsNoAlpha.Set_NoAlpha(-1, true);
		m_bsNoAlpha.Create();

		//-------------------------------
		// �悭�g���T���v���쐬 & �Z�b�g
		//-------------------------------
		m_smp0_Linear_Wrap.SetAll_Standard();
		m_smp0_Linear_Wrap.Set_Wrap();
		m_smp0_Linear_Wrap.Create();
		m_smp0_Linear_Wrap.SetStateVS(0);
		m_smp0_Linear_Wrap.SetStatePS(0);

		m_smp1_Linear_Clamp.SetAll_Standard();
		m_smp1_Linear_Clamp.Set_Clamp();
		m_smp1_Linear_Clamp.Create();
		m_smp1_Linear_Clamp.SetStateVS(1);
		m_smp1_Linear_Clamp.SetStatePS(1);




		// ���C�g�Ǘ��N���X
		m_LightMgr.Init();

		// �L���[�u�}�b�v
		m_texCube.LoadTexture("data/Texture/CubeMap.dds");
		m_texCube.SetTexturePS(20);
	}

	//================================================
	// �Ō�Ɉ����s�����
	//================================================
	void Release() {
		// �V�F�[�_�N���X���
		m_Es.Release();
		m_Samp.Release();
		m_Sprite.Release();







		// 
		m_bsAdd.Release();
		m_bsAlpha.Release();

		// 
		m_smp0_Linear_Wrap.Release();
		m_smp1_Linear_Clamp.Release();

		m_LightMgr.Release();
	}

	//================================================
	// �`�撼�O�ɍX�V����f�[�^
	//================================================
	void UpdateBeforeDraw() {
		// �e�V�F�[�_�̃t���[���P�ʂł̃f�[�^�X�V
		m_Es.WritePerFrameData();		// �萔�o�b�t�@�֕K�v�ȃf�[�^���������񂾂肷��
		m_Samp.WritePerFrameData();		// �萔�o�b�t�@�֕K�v�ȃf�[�^���������񂾂肷��




		// ���C�g���
		m_LightMgr.Update();
	}

	//================================================
	// �e�V�F�[�_�̃R���p�C���ς݂̃V�F�[�_(cso)���t�@�C���ɕۑ�
	//================================================
	void SaveCsoToFile(){
		m_Es.SaveCsoToFile();
		m_Samp.SaveCsoToFile();
		m_Sprite.SaveCsoToFile();





	}

};

#endif
