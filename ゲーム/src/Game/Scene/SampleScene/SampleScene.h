#ifndef __SampleScene_h__
#define __SampleScene_h__


#include "Game/GameCamera.h"
#include "Sample_Data.h"

//=========================================================
// �^�C�g���V�[���N���X
//=========================================================
class SampleScene : public BaseScene {
public:
	// �����ݒ�֐�
	void Init();
	// ����֐�
	void Release();

	// �X�V����
	virtual int Update() override;
	// �`�揈��
	virtual void Draw() override;

	// 
	SampleScene() {
	}
	// 
	virtual ~SampleScene() {
		Release();
	}

public:

	static SampleScene *s_pScene;

	//=========================================================
	// �^�X�N�֌W
	//=========================================================
	TaskManager		m_TaskMgr;
	TaskManager		m_EffectTaskMgr;

	//=========================================================
	// ���̑��f�[�^
	//=========================================================
	// ���s����
	SPtr<DirLight>	m_DirLight;

	// �J����
	GameCamera		m_Cam;

	bool			m_PauseFlag;	// �|�[�Y
	bool			m_ShowPointLoght = false;	// �|�C���g���C�g���f�o�b�O�\��

	//	CDSBuffer		m_sndSE1;
	//	CDSBuffer		m_sndBGM;

	// �e�N�X�`��
	YsTexture			m_texBack;		// YsTexture�����̂܂܎g�p�ł��邪�A���܂肨���߂��܂���B
	// ���_�F�g�������ȒP�@���_�F���Ƌ��L���ɂ���
	SPtr<YsTexture>		m_texBack2;		// YsTexture��shared_ptr�ŗp�ӁB������̕������낢��Ɠs���������B
	// ���_�F���Ƌ��L���₷��
	YsTextureSet		m_tsLaser;		// SPtr<YsTexture>�𕡐����������́B����V�F�[�_�ł̍��i���ȕ\���̎��ɕK�v�B
	YsTextureSet		m_tsExplode;
	// ��������TextureSet�����Ă��OK�B
	// ���_�F�����̃e�N�X�`���������邵�A���L�����₷��

	// ���f��
	SPtr<YsGameModel>	m_pSm;
	SPtr<YsGameModel>	m_pSm2;
	SPtr<YsGameModel>	m_xF15;
	SPtr<YsGameModel>	m_xSu27;
	SPtr<YsGameModel>	m_xBox;
	SPtr<YsGameModel>	m_xMap;

	// �X�e�[�W�}�b�v�p
	YsModelObject		m_moMap;
	YsMatrix			m_mMap;
	YsBulletRigidBody	m_btMap;	// �X�e�[�W�}�b�v�p�̍���

	// �������[���h
	SampleBulletWorld	m_Bullet;


	// �T�E���h
	SPtr<YsSoundData>	m_sndSE;


	float				m_LightPower;	// ���s�����̖��邳

	SPtr<PointLight>		m_PL[2];
	SPtr<SpotLight>		m_SL;
};

#endif