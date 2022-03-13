#pragma once
#include "World.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
#include "SoundMgr.h"
#include "UIObject.h"
#include "EffectObj.h"
class GameWorld : public World
{
public:
	Texture*		m_BGTex;
	PlayerObj2D		m_PlayerObj;
	Sound*			m_pBackGroundMusic;
	vector<ObjectNpc2D*> m_npclist;
	World*			m_pNextWorld;
	
	float			m_fStarChangeTime = 2;
	float			m_fStarTimer = 0.0f;
	int				m_iStarCurrentIndex = 0;

public:
	
	bool			CreateModelType();
	bool			CreateNPC();
	//int				Life();
	bool			Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

