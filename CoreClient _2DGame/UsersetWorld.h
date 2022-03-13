#pragma once 
#include "World.h"
class UsersetWorld : public World
{
public:
	bool			Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

