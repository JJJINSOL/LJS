#pragma once
#include "State.h"
class StateStop : public State
{
public:
	virtual void	Process(Object2D* pPlayer) override;
	StateStop(ObjectAI* owner) : State(owner)
	{

	}
};

