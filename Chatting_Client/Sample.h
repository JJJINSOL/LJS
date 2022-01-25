#pragma once
#include "Core.h"
#include "AsyncSelect.h"
#include "Message.h"
class Sample : public Core
{
	AsyncSelect m_net;
	Message m_msg;
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
