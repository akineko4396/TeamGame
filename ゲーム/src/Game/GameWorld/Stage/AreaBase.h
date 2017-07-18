
#pragma once

class Area{
public:
	virtual ~Area(){}
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};