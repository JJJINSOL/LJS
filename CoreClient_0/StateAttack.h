#pragma once
#include "State.h"
class StateAttack : public State
{
public:
	virtual void	Process(Object2D* pPlayer) override;
	StateAttack(ObjectAI* owner) : State(owner)
	{

	}
};

