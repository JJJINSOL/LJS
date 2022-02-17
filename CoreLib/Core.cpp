#include "Core.h"
#include "ObjectMgr.h"
bool Core::CoreInit()
{
	m_GameTimer.Init();
	Input::Get(), Init();
	InitDevice();
	Init();
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
	m_GameTimer.Frame();
	Input::Get().Frame();
	I_ObjectMgr.Frame();
	Frame();
	return true;
}
bool Core::CoreRender()
{
	
	float color[4] = { 1, 0, 0, 1 };//배경색
	//백버퍼 지워준다 - (ClearRenderTargetView)화면을 깨끗이 지워준다는 의미
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// 백버퍼에 랜더링 한다.
	Render();

	m_GameTimer.Render();
	Input::Get().Render();

	m_pSwapChain->Present(0, 0);
	return true;
}
bool Core::CoreRelease()
{
	Release();

	m_GameTimer.Release();
	Input::Get().Release();

	CleanupDevice();
	return true;
}