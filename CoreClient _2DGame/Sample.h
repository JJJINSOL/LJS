#pragma once
#include "Core.h"
#include "AsyncSelect.h"
#include "IntroWorld.h"
#include "GameWorld.h"
#include "RankWorld.h"
#include "UsersetWorld.h"
#include "UIObject.h"
#include "UIModelMgr.h"
class Sample : public Core
{
public:
	IntroWorld		m_IntroWorld;
	GameWorld		m_GameWorld;
	RankWorld		m_RankWorld;
	UsersetWorld	m_UserserWorld;

	AsyncSelect m_net;
	HWND		m_edit;
	HWND		m_button;
	HWND		m_listbox;
	int			m_chatcount;
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
