
#pragma once

#include "../StageBase.h"
#include "../AreaBase.h"

class Stage2 : public Stage{
public:
	~Stage2(){}
	void Init();
	void Release();
	void Update();
	void Draw();

	SPtr<Area> m_NowArea;
};