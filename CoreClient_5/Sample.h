#pragma once
#include "Core.h"
#include "AsyncSelect.h"
#include "IntroWorld.h"
#include "LoginWorld.h"
#include "ZoneWorld.h"
class Sample : public Core
{
public:
	IntroWorld			m_IntroWorld;
	//LoginWorld			m_LoginWorld;
	ZoneWorld			m_ZoneWorld;

	AsyncSelect m_net;
	HWND m_edit;
	HWND m_button;
	HWND m_listbox;
	int m_chatcount;
public:
	virtual void CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void DeleteResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};
