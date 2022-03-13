#include "UsersetWorld.h"
bool UsersetWorld::Init()
{
	World::Init();
	return true;
}
bool UsersetWorld::Load(std::wstring file)
{
	return true;
}
bool UsersetWorld::Frame()
{
	//m_pBackGroundMusic->Frame();
	World::Frame();
	return true;
}
bool UsersetWorld::Render()
{
	World::Render();
	return true;
}
bool UsersetWorld::Release()
{
	World::Release();
	return true;
}