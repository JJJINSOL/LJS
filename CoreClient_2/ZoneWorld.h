#pragma once
#include "World.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
#include "SoundMgr.h"
class ZoneWorld : public World
{
public:
	PlayerObj2D		m_PlayerObj;
	Sound* m_pBackGroundMusic;
public:
	std::vector<ObjectNpc2D*> m_NpcLlist;
public:
	bool Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

