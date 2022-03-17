#include "StateStop.h"
#include "Fsm.h"
void StateStop::Process(Object2D* pPlayer)
{
	m_pOwner->m_fMoveTimer -= g_fSecPerFrame;
	if (m_pOwner->m_fMoveTimer <= 0.0f)
	{
		m_pOwner->SetTransition(STATE_STOP_TIMER);
		m_pOwner->ResetTimer();
		return;
	}
	// 유저와 npc 거리 계산 > 반지름
	Vector2 vPC = pPlayer->m_vPos;
	Vector2 vNPC = m_pOwner->m_vPos;
	float fDistance = (vPC - vNPC).Length();
	if (fDistance < 200.0f)
	{
		m_pOwner->SetTransition(STATE_FIND_TARGET);
		m_pOwner->ResetTimer();
		return;
	}
}