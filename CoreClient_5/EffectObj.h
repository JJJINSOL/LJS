#pragma once
#include "Object2D.h"
#include "Sprite.h"
class EffectObj : public Object2D
{
public:
	Sprite*  m_pSprite;
	int		 m_iCurrentIndex = 0;
	float    m_fChangeTime;// = m_pSprite->m_fAnimTime / m_pSprite->m_rtArray.size();
	float    m_fTimer = 0.0f;

public:
	virtual void	HitSelect(BaseObject* pObj, DWORD dwState) override;
	virtual void	Reset();
	virtual bool	Init() override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool	Release()override;
};