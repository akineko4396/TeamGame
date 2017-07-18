#ifndef __TitleScene_h__
#define __TitleScene_h__

#include "Game/GameCamera.h"

//=========================================================
// �^�C�g���V�[���N���X
//=========================================================
class TitleScene : public BaseScene {
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
	TitleScene() {
	}
	// 
	virtual ~TitleScene() {
		Release();
	}

public:
	//=========================================================
	// ���̑��f�[�^
	//=========================================================

	// ���s����
	SPtr<DirLight>	m_DirLight;

	// �J����
	GameCamera		m_Cam;			// �J�����N���X

	// �e�N�X�`��
	SPtr<YsTexture>		m_texBack;

	// �L�����N�^�ɕK�v�ȃf�[�^���܂Ƃ߂������̍\����
	struct Chara {
		YsModelObject	m_Mo;		// ���b�V���I�u�W�F�N�g�@���f���f�[�^(YsGameModel)��������
		YsMatrix		m_Mat;		// �s��

		YsAnimator		m_Anim;		// �A�j���[�^�[�@�A�j���[�V������S������
		YsAnimator		m_Anim2;	// �A�j���[�^�[�͂��������Ă��ǂ��@�����g���ƃA�j���̓���ȍ������\
	};

};


#endif