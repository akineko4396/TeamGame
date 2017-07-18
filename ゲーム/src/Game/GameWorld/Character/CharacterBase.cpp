
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

	//// 処理実行
	//auto it = m_List.begin();
	//while (it != m_List.end()){

	//	if ((*it)->GetKillFlg() == false){		// 削除フラグがOFFの時
	//		// 処理実行
	//		(*it)->Update();

	//		++it;
	//	}
	//	// 削除フラグが立ってるときは、タスクリストから削除
	//	else{
	//		it = m_List.erase(it);	// リストのノードを削除し、次へ。(シェアードポインタのカウントが０になったら、データも解放、つまりdeleteされる)
	//	}
	//}
}

void CharacterBase::AllDraw()
{
	for (UINT c = 0; c < m_List.size(); c++)
	{
		//メッシュ数ぶんループ
		for (UINT i = 0; i < m_List[c]->GetModelObject().GetGameModel()->GetModelTbl().size(); i++) {
			//YsSingleModelからメッシュを取得
			SPtr<YsSingleModel> model = m_List[c]->GetModelObject().GetGameModel()->GetModelTbl()[i];
			SPtr<YsMesh> mesh = model->GetMesh();

			//視錘台で描画判定
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