#pragma once
#include "World.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
#include "UIObject.h"
#include "SoundMgr.h"
#include "ShaderMgr.h"
#include "TextureMgr.h"
#include "EffectObj.h"
class IntroWorld : public World
{
public:
	//TSprite	m_SpriteObj;
	EffectObj	m_ExplosionObj;
	World*		m_pNextWorld;
	//Object2D	m_PlayerObj;
	Sound*		m_pBackGroundMusic;
	Texture*	m_pChangeColorTex[10];
	Texture*	m_pColorTex;

	bool CreateModelType();
	bool Load(std::wstring file) override;
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
};
