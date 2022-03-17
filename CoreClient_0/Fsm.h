#pragma once
#include "State.h"
class FsmState
{
public:
	DWORD  m_dwState;
	std::map<DWORD, DWORD> m_mapEvent;
	FsmState(DWORD state) : m_dwState(state)
	{

	}
};
class Fsm : public Singleton<Fsm>
{
	friend class Singleton<Fsm>;
public:
	std::map<DWORD, FsmState*> m_mapStateClass;
	void AddTransition(DWORD, DWORD, DWORD);
	DWORD GetTransition(DWORD dwState, DWORD dwOutput);
};
#define I_Fsm Fsm::Get()