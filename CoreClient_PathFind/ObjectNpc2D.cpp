#include "ObjectNpc2D.h"
#include "Fsm.h"
bool ObjectNpc2D::Init()
{
	m_pStateArray[STATE_STOP] = new StateStop(this);
	m_pStateArray[STATE_MOVE] = new StateMove(this);
	m_pStateArray[STATE_ATTACK] = new StateAttack(this);
	m_pStateAction = m_pStateArray[STATE_STOP];
	m_dwStateAction = m_pStateAction->m_dwStateAI;
	return true;
}
void ObjectNpc2D::SetTransition(DWORD dwEvent)
{
	m_dwStateAction = I_Fsm.GetTransition(m_dwStateAction, dwEvent);
	m_pStateAction = m_pStateArray[m_dwStateAction];
}
bool ObjectNpc2D::Frame()
{
	//m_pState = m_pStateArray[m_dwStateAI];
	//m_vPos += TVector2(10 * g_fSecPerFrame, 10 * g_fSecPerFrame);
	//SetPosition(m_vPos);
	//AddPosition(TVector2(30 * g_fSecPerFrame, 30 * g_fSecPerFrame));

	return true;
}