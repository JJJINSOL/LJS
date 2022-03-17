#pragma once
#include "Core.h"
#include "AsyncSelect.h"
#include "PlayerObj2D.h"
#include "ObjectNpc2D.h"
class Sample : public Core
{
public:
	PlayerObj2D	m_PlayerObj;

	std::vector<std::shared_ptr<ObjectNpc2D>> m_NpcLlist;
	AsyncSelect m_net;
	HWND m_edit;
	HWND m_button;
	HWND m_listbox;
	int m_chatcount;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};
