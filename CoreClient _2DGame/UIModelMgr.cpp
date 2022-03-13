#include "UIModelMgr.h"
#include "ObjectMgr.h"
bool UIModelComposed::Frame()
{
	std::list< UIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Frame();
	}
	return true;
}
bool UIModelComposed::Render()
{
	std::list< UIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Render();
	}
	return true;
}
bool UIModelComposed::Release()
{
	std::list< UIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Release();
	}
	return true;
}
void UIModelComposed::Add(UIModel* pObj)
{
	_ASSERT(pObj);//에러 검출용 코드
	pObj->m_pParent = this;
	m_Components.push_back(pObj);
}
UIModel* UIModelComposed::Clone()
{
	UIModelComposed* pModel = new UIModelComposed;
	std::list< UIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		pModel->Add((*iter)->Clone());

	}
	return pModel;
};
void UIModelComposed::UpdateData()
{
	std::list< UIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->UpdateData();
		(*iter)->SetCollisionType(CollisionType::Ignore,SelectType::Select_Overlap);
	}
}