#include "Core.h"
bool Core::CoreInit()
{
	return true;
}
bool Core::GameRun()
{
	CoreInit();
	while (WinRun())
	{
		CoreFrame();
		CoreRender();
	}
	CoreRelease();
	return true;
}
bool Core::CoreFrame()
{
	return true;
}
bool Core::CoreRender()
{
	return true;
}
bool Core::CoreRelease()
{
	return true;
}