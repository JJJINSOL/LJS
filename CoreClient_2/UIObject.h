#pragma once
#include "Object2D.h"
class UIObject : public Object2D
{
public:
	bool Frame() override;
	bool Render()override;
};
//==========================================================
class ImageObject : public UIObject
{
public:
	float  m_fAlpha = 0.0f;
	bool   m_bFadeIn = false;
	bool   m_bFadeOut = false;
public:
	bool Init() override;
	bool Frame() override;
	bool Render()override;
	void FadeIn();
	void FadeOut();
};
//===========================================================
class ButtonObject : public UIObject
{
public:
	bool Frame() override;
	bool Render()override;
};