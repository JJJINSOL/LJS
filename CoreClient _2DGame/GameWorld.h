#pragma once
#include "World.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
#include "SoundMgr.h"
#include "UIObject.h"
#include "EffectObj.h"
#include "Life2D.h"
#include "Timer.h"
#include "WriteFont.h"
#include "IntroWorld.h"

class GameWorld : public World
{
public:
	Texture*		m_BGTex;
	Life2D			m_Life;
	PlayerObj2D		m_PlayerObj;
	Sound*			m_pBackGroundMusic;
	Sound*			m_peffect;
	vector<ObjectNpc2D*> m_npclist;
	World*			m_pNextWorld;
	bool			m_gamestart = false;
	bool			hard = false;
	
	float			m_fStarChangeTime = 2;
	float			m_fStarTimer = 0.0f;
	int				m_iStarCurrentIndex = 0;

	float			m_fStarChangeTime1 = 0.1;
	float			m_fStarTimer1 = 0.0f;
	int				m_iStarCurrentIndex1 = 0;

	float			m_fHeartMakeTime = 20;
	float			m_fHeartTimer = 0.0f;
	int				m_iHeartCurrentIndex = 0;

	int				m_score = 0;
	bool			m_plusescore = false;

public:
	
	bool			CreateModelType();
	bool			CreateNPC();
	bool			CreatePlusLife();
	bool			CreatePlusScore();
	bool			Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

