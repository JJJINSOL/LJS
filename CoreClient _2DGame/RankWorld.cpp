#include "RankWorld.h"
bool RankWorld::Init()
{
	World::Init();
	return true;
}
bool RankWorld::Load(std::wstring file)
{
	return true;
}
bool RankWorld::Frame()
{
	//m_pBackGroundMusic->Frame();
	World::Frame();
	return true;
}
bool RankWorld::Render()
{
	World::Render();
	return true;
}
bool RankWorld::Release()
{
	World::Release();
	return true;
}