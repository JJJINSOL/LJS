#pragma once
#include "Object2D.h"
class PlayerObj2D : public Object2D
{
public:
	virtual void	HitOverlap(BaseObject* pObj, DWORD dwState);
	bool Frame() override;
	PlayerObj2D();
	virtual ~PlayerObj2D();
};

