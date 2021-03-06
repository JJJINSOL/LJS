#include "Fsm.h"
void Fsm::AddTransition(DWORD dwState, DWORD dwEvent, DWORD dwOutput)
{
	FsmState* pStateFsm = nullptr;
	auto iter = m_mapStateClass.find(dwState);
	if (iter != m_mapStateClass.end())
	{
		pStateFsm = iter->second;
	}
	else
	{
		pStateFsm = new FsmState(dwState);
		m_mapStateClass.insert(std::make_pair(dwState, pStateFsm));
	}
	pStateFsm->m_mapEvent.insert(std::make_pair(dwEvent, dwOutput));
}
DWORD Fsm::GetTransition(DWORD dwState, DWORD dwEvent)
{
	FsmState* pStateFsm = nullptr;
	auto iterState = m_mapStateClass.find(dwState);
	if (iterState == m_mapStateClass.end())
	{
		//error
	}
	pStateFsm = iterState->second;
	auto iterOutput = pStateFsm->m_mapEvent.find(dwEvent);
	if (iterOutput == pStateFsm->m_mapEvent.end())
	{
		// error		
	}
	return iterOutput->second;
}