#pragma once
#include "Object2D.h"
class Life2D : public Object2D
{
public:
	bool Frame() override;

	Life2D() {}
	virtual ~Life2D() {}
};

