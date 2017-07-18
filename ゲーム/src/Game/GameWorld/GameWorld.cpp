#include "main.h"

#include "GameWorld.h"

Stage * BaseGameObj::GetStage()
{
	if (GameScene::GetStage().expired()==false)
	{
		return GameScene::GetStage().lock().get();


	}

	return nullptr;
}