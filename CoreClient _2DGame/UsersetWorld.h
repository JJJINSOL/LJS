#pragma once 
#include "World.h"
#include "UIObject.h"
#include "SoundMgr.h"
class UsersetWorld : public World
{
public:
	Texture* m_BGTex;
	Sound* m_pBackGroundMusic;
	bool			CreateModelType();
	bool			Load(std::wstring file) override;
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
};

