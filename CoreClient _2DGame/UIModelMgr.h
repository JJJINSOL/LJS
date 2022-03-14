#pragma once
#include "BaseMgr.h"
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
		DisplayText("TStatePlayData\n");
	}
	StatePlayData(Texture* a, Sound* b)
	{
		pTex = a;
		pSound = b;
		DisplayText("StatePlayData\n");
	}
};
//======================================================================
// 0번 - 배경이미지 타입 -> Red
// 1번 - 대화상자(패널)  -> Green
// 2번 - 버튼타입        -> Blue
class UIModel : public Object2D
{
public:
	std::vector<StatePlayData> m_pStatePlayList;
	int  m_index;
	bool m_start = false;;
	RECT m_rtOffset;
	RECT m_rtOffsetTex;

	virtual UIModel* Clone() { return nullptr; };
	virtual void  UpdateData() {}
};
//=======================================================================
class UIModelComposed : public UIModel
{
public:
	std::list< UIModel*> m_Components;
public:
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual void Add(UIModel* pObj);
	virtual UIModel* Clone();
	virtual void UpdateData();
};
//============================================================================
class UIModelMgr : public BaseMgr<UIModel, UIModelMgr>
{
	friend class Singleton<UIModelMgr>;
private:
	UIModelMgr() {};
public:
	~UIModelMgr() {};
};
#define I_UI UIModelMgr::Get()

