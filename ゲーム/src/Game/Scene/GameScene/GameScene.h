#ifndef __GameScene_h__
#define __GameScene_h__

#include "Game/GameCamera.h"

class Stage;

//=========================================================
// �Q�[���V�[���N���X
//=========================================================
class GameScene : public BaseScene{
public:

	// �����ݒ�֐�
	void Init();
	// ����֐�
	void Release();
	// �X�V����
	virtual int Update() override;
	// �`�揈��
	virtual void Draw() override;

	// �X�e�[�W����
	static SPtr<Stage>m_Stage;
	static WPtr<Stage>GetStage(){ return m_Stage; }
	int m_StageNo;
	GameScene(int _StageNo) {
		m_StageNo=_StageNo;
	}

	virtual ~GameScene() {
		Release();
	}

public:
	//=========================================================
	// ���̑��f�[�^
	//=========================================================




	
};

#endif