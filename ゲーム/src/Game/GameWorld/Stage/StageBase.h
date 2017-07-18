
#pragma once

class Stage{
public:
	virtual ~Stage(){}
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	CollisionEngine		m_ColEng;
};

