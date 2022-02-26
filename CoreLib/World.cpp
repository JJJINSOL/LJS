#include "World.h"

World* World::m_pWorld = nullptr;

bool World::Load(std::wstring saveFile)
{
	return true;
}
bool World::Init()
{
	return true;
}
bool World::Frame()
{
	for (auto obj : m_UIObj)
	{
		Object2D* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	return true;
}
bool World::Render()
{
	for (auto obj : m_UIObj)
	{
		Object2D* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Render();
		}
	}
	return true;
}
bool World::Release()
{
	for (auto obj : m_UIObj)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_ItemObj)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_NpcObj)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_MapObj)
	{
		obj.second->Release();
		delete obj.second;
	}
	m_ItemObj.clear();
	m_UIObj.clear();
	m_NpcObj.clear();
	m_MapObj.clear();
	return true;
}
World::World()
{

}

World::~World()
{

}