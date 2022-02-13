#include "Core.h"
bool Core::CoreInit()
{
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
	Frame();
	return true;
}
bool Core::CoreRender()
{
	
	float color[4] = { 1, 1, 1,1 };//��� ���
	//����� �����ش� - (ClearRenderTargetView)ȭ���� ������ �����شٴ� �ǹ�
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, color);

	// ����ۿ� ������ �Ѵ�.
	Render();

	m_pSwapChain->Present(0, 0);
	return true;
}
bool Core::CoreRelease()
{
	Release();
	CleanupDevice();
	return true;
}