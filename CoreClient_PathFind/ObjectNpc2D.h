#pragma once
#include "Object2D.h"
#include "StateMove.h"
#include "StateStop.h"
#include "StateAttack.h"

class ObjectNpc2D : public ObjectAI
{
public:
	bool Init();
	bool Frame() override;
	void SetTransition(DWORD dwEvent) override;
};

