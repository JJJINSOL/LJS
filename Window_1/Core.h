#pragma once
#include "Window.h"
class Core : public Window
{
public:
	bool	CoreInit();
	bool	GameRun();
	bool	CoreFrame();
	bool	CoreRender();
	bool	CoreRelease();
};