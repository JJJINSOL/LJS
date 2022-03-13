#pragma once
#include "World.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
#include "SoundMgr.h"
#include "UIObject.h"

class GameWorld : public World
{
public:
	Texture*		m_BGTex;
	PlayerObj2D		m_PlayerObj;
	Sound*			m_pBackGroundMusic;
	vector<ObjectNpc2D*> m_npclist;
	
public:
	
	bool			CreateModelType();
	bool			CreateNPC();
	bool			Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

