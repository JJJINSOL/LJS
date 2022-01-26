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
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	virtual LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};
