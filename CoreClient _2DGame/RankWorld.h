#pragma once
#include "World.h"
class RankWorld : public World
{
public:
	bool			Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

