#pragma once
#include "Object2D.h"
class ObjectNpc2D : public Object2D
{
public:
	bool Frame() override;
	void HitOverlap(BaseObject* pObj, DWORD dwState);
public:
	ObjectNpc2D();
	virtual ~ObjectNpc2D();
};
