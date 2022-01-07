#include "Sample.h"
bool Sample:: Init()
{
	for(int i = 0; i<12; i++)
	{
		DxObject obj;
		obj.Init();

		if (obj.Create(m_pd3dDevice, m_pImmediateContext, Vector2(-100 * i, i * 50), 400, 30))
		{
			m_ObjectList.push_back(obj);
		}
	}

	return true;
}
bool Sample:: Frame()
{
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].Frame();
	}
	return true;
}
bool Sample:: Render()
{
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].Render();
	}
	return true;
}
bool Sample:: Release()
{
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		m_ObjectList[i].Release();
	}
	return true;
}
Sample::Sample()
{

}
Sample:: ~Sample()
{

}
RUN();