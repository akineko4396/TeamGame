#include "main.h"

#include "../../GameWorld.h"

#include "Stage2.h"


void Stage2::Init()
{
}
void Stage2::Release()
{

}
void Stage2::Update()
{

}
void Stage2::Draw()
{





	// 説明文字
	std::string text;
	text += "Stage 2";
	text += "\n";
	// 2D描画
	YsDx.GetSprite().Begin(true);
	{
		YsDx.GetSprite().DrawFont(text, 1, 1, &YsVec4(0, 0, 0, 1));
		YsDx.GetSprite().DrawFont(text, 0, 0, &YsVec4(1, 1, 1, 1));
	}
	YsDx.GetSprite().End();
}