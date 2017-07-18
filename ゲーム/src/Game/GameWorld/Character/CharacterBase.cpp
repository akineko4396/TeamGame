
#include "main.h"

#include "Game\GameWorld\GameWorld.h"

#include "CharacterBase.h"

#include "Game\GameWorld\CollisionEngine\CollisionEngine.h"

std::vector<SPtr<CharacterBase>> CharacterBase::m_List;

void CharacterBase::Update(){}

void CharacterBase::Draw()
{
	ShMgr.m_Samp.DrawModel(m_Mo, &m_Mat);
}

void CharacterBase::AllUpdate()
{
	for (UINT ec = 0; ec < m_List.size(); ec++)
	{
		m_List[ec]->Update();
	}

	//if (m_List.empty())return;

	//// �������s
	//auto it = m_List.begin();
	//while (it != m_List.end()){

	//	if ((*it)->GetKillFlg() == false){		// �폜�t���O��OFF�̎�
	//		// �������s
	//		(*it)->Update();

	//		++it;
	//	}
	//	// �폜�t���O�������Ă�Ƃ��́A�^�X�N���X�g����폜
	//	else{
	//		it = m_List.erase(it);	// ���X�g�̃m�[�h���폜���A���ցB(�V�F�A�[�h�|�C���^�̃J�E���g���O�ɂȂ�����A�f�[�^������A�܂�delete�����)
	//	}
	//}
}

void CharacterBase::AllDraw()
{
	for (UINT c = 0; c < m_List.size(); c++)
	{
		//���b�V�����Ԃ񃋁[�v
		for (UINT i = 0; i < m_List[c]->GetModelObject().GetGameModel()->GetModelTbl().size(); i++) {
			//YsSingleModel���烁�b�V�����擾
			SPtr<YsSingleModel> model = m_List[c]->GetModelObject().GetGameModel()->GetModelTbl()[i];
			SPtr<YsMesh> mesh = model->GetMesh();

			//������ŕ`�攻��
			if (CheckViewFrustum(m_List[c], mesh)) {
				m_List[c]->Draw();
			}
		}
	}
}

void CharacterBase::CheckKillFlg()
{
	std::vector<SPtr<CharacterBase>>::iterator it = m_List.begin();
	while (it != m_List.end())
	{
		if ((*it)->GetKillFlg() == 1)
		{
			*it = NULL;
			it = m_List.erase(it);
		}
		else{
			it++;
		}
	}
}