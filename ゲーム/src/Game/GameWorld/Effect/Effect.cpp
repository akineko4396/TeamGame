#include "main.h"
#include "Game\GameWorld\GameWorld.h"
#include "Effect.h"


std::vector<SPtr<BaseEffectObj>>BaseEffectObj::m_List;


void BaseEffectObj::AllUpdate(){

	if (m_List.empty())return;

	// 処理実行
	auto it = m_List.begin();
	while (it != m_List.end()){

		if ((*it)->GetKillFlg() == false){		// 削除フラグがOFFの時
			// 処理実行
			(*it)->Update();

			++it;
		}
		// 削除フラグが立ってるときは、タスクリストから削除
		else{
			it = m_List.erase(it);	// リストのノードを削除し、次へ。(シェアードポインタのカウントが０になったら、データも解放、つまりdeleteされる)
		}
	}

}

void BaseEffectObj::AllDraw()
{
	if (m_List.empty())return;

	// 処理実行
	auto it = m_List.begin();
	while (it != m_List.end()){
			// 処理実行
			(*it)->Draw();
			++it;
	}
}

