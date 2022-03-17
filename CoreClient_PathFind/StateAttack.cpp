#include "StateAttack.h"
#include "Fsm.h"
void StateAttack::Process(Object2D* pPlayer)
{
	// 유저와 npc 거리 계산 > 반지름
	Vector2 vPC = pPlayer->m_vPos;
	Vector2 vNPC = m_pOwner->m_vPos;
	float fDistance = (vPC - vNPC).Length();
	if (fDistance > 300.0f)
	{
		m_pOwner->SetTransition(STATE_LOST_TARGET);
		return;
	}
	Vector2 vAdd = (vPC - vNPC).Normal() * g_fSecPerFrame * 300.0f;
	m_pOwner->AddPosition(vAdd);
}