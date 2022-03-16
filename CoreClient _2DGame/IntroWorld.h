#pragma once
#include "World.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
#include "UIObject.h"
#include "SoundMgr.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "EffectObj.h"
#include "GameWorld.h"
class IntroWorld : public World
{
public:

	EffectObj	m_ExplosionObj;
	World*		m_pNextWorld1;
	World*		m_pNextWorld2;
	World*		m_pNextWorld3;

	Sound*		m_pBackGroundMusic;
	
	Texture*	m_BGTex;
	Texture*	m_LOGOTex;


	bool CreateModelType();
	bool Load(std::wstring file) override;
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
};
