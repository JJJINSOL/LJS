#pragma once
#include "Core.h"
#include "TFbxObj.h"
class Sample : public Core
{
	std::vector<Fbx> m_FbxObj;
	Texture* m_pLightTex;
	Texture* m_pNormalMap;
public:
	virtual bool Init()  override;
	virtual bool Frame()  override;
	virtual bool Render()  override;
	virtual bool Release()  override;	

	virtual void CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void DeleteResizeDevice(UINT iWidth, UINT iHeight) override;

	void	DisplayErrorBox(const WCHAR* lpszFunction);
	DWORD	LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list);

public:
	Sample();
	virtual ~Sample();
};

