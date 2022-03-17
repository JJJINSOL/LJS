#pragma once
#include "Object2D.h"

class State;
enum T_EnemyState
{
	STATE_STOP = 0,
	STATE_MOVE,
	STATE_ATTACK,
};
enum T_EnemyEvent
{
	STATE_STOP_TIMER = 0,
	STATE_MOVE_TIMER,
	STATE_FIND_TARGET,
	STATE_LOST_TARGET,
};
class ObjectAI : public Object2D
{
public:
	State* m_pStateArray[3];
	State* m_pStateAction = nullptr;
	DWORD   m_dwStateAction;
public:
	float m_fMoveTimer = 3.0f;
	float m_fStopTimer = 2.0f;
public:
	virtual void SetTransition(DWORD dwEvent) = 0;
	ObjectAI()
	{
		ResetTimer();
	}
	void ResetTimer()
	{
		m_fMoveTimer = randstep(1.0f, 3.0f);
		m_fStopTimer = randstep(1.0f, 3.0f);;
	}
};
class State
{
public:
	ObjectAI* m_pOwner;
	DWORD	   m_dwStateAI;
public:
	virtual void	Process(Object2D* pPlayer) = 0;
	State(ObjectAI* owner) : m_pOwner(owner)
	{

	}
};

