
#pragma once

#include "../StageBase.h"

#include "Data\DataManager.h"

// �z�u
#include "Game\Edit\Put.h"

#include "Game\GameAI\NavigationAI.h"

// ver.1
//
// �X�e�[�W�P(��) : �X�e�[�W���Ƃ̏���
//
// Stage���p��

// �y�����o�z
//
// �Em_wpCamera...���_�ɂȂ�L����������
// �Em_wpPlayer...�L�[�����Ȃǂ�������L����������
//


class GameWorld : public Stage{
public:

	void Init();
	void Release();

	void Update();
	void Draw();

	~GameWorld(){
		Release();
	}
	
	// �|�[�Y�J����
	GameCamera			m_Cam;

	// ���݃J�������_�ɂȂ��Ă�L�����ւ̃A�h���X
	WPtr<CharacterBase>	m_wpCamera;	

	// ���삷��v���C���[�L�����ւ̃A�h���X
	WPtr<CharacterBase>	m_wpPlayer;		



	//											//
	//  �� �ォ��ύX���邩������Ȃ������o ��	//
	//											//

	Put m_Put;	//�v���C���[������z�u����


	// �f�o�b�O�p ���[�h�ؑ�

	enum{NORMAL,EDIT,CREATE};
	int m_EditFlg = NORMAL;


	// �Ö��f�[�^
	float				m_BlackCurtainPower = 0;	// �Ö��̋���(�t�F�[�h�C��/�A�E�g�Ŏg�p)
	float				m_BlackCurtainAdd = 0;		// m_BlackCurtainPower�ɖ�����Z����l

	//==============================================
	// �����G���W��
	//==============================================
	YsBulletWorld		m_Bullet;					// �����G���W��

	//==============================================
	// ���̑�
	//==============================================
	bool				m_PauseFlag = false;		// �|�[�Y�t���O
	bool				m_Debug_DrawHit = false;	// �����蔻��f�o�b�O�\���t���O

};
