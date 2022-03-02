#pragma once
#include "Object2D.h"
#include "SoundMgr.h"
struct StatePlayData
{
	Texture* pTex;
	Sound* pSound;
	StatePlayData(const StatePlayData& data)
	{
		pTex = data.pTex;
		pSound = data.pSound;
		DisplayText("StatePlayDataCopy\n");
	}
	StatePlayData()
	{
		DisplayText("StatePlayData\n");
	}
	StatePlayData(Texture* a, Sound* b)
	{
		pTex = a;
		pSound = b;
		DisplayText("StatePlayData\n");
	}
};
//===========================================================
class UIObject : public Object2D
{
public:
	std::vector<StatePlayData> m_pStatePlayList;
	RECT m_rtOffset;//¿ÃπÃ¡ˆ 9∞≥∑Œ ª—∑¡¡¸
	RECT m_rtOffsetTex;
public:
	bool Frame() override;
	bool Render()override;
	bool SetVertexData()override;
	bool SetIndexData()override;
	UIObject()
	{
		m_rtOffsetTex.left = 0;
		m_rtOffsetTex.top = 0;
		m_rtOffsetTex.right = 1;
		m_rtOffsetTex.bottom = 1;
	}
};
//==========================================================
class ImageObject : public UIObject
{
public:
	bool Init() override;
	bool Frame() override;
	bool Render()override;
};
//===========================================================
class ButtonObject : public UIObject
{
public:
	virtual void HitSelect(BaseObject* pObj, DWORD dwState) override;
	bool Init() override;
	bool Frame() override;
	bool Render()override;
};