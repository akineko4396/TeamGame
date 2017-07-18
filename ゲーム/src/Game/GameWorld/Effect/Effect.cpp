#include "main.h"
#include "Game\GameWorld\GameWorld.h"
#include "Effect.h"


std::vector<SPtr<BaseEffectObj>>BaseEffectObj::m_List;


void BaseEffectObj::AllUpdate(){

	if (m_List.empty())return;

	// �������s
	auto it = m_List.begin();
	while (it != m_List.end()){

		if ((*it)->GetKillFlg() == false){		// �폜�t���O��OFF�̎�
			// �������s
			(*it)->Update();

			++it;
		}
		// �폜�t���O�������Ă�Ƃ��́A�^�X�N���X�g����폜
		else{
			it = m_List.erase(it);	// ���X�g�̃m�[�h���폜���A���ցB(�V�F�A�[�h�|�C���^�̃J�E���g���O�ɂȂ�����A�f�[�^������A�܂�delete�����)
		}
	}

}

void BaseEffectObj::AllDraw()
{
	if (m_List.empty())return;

	// �������s
	auto it = m_List.begin();
	while (it != m_List.end()){
			// �������s
			(*it)->Draw();
			++it;
	}
}

