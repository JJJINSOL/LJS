#pragma once
#include "State.h"

//1,패스에니메이션(선형보간)->이동패턴
	//1)벡터의 직선의 방정식 ==> v = p + d*t(speed*경과시간);
	//2)선형보간 = v(t) = (1-t)*v0 + t*v1;
// 2,곡선패스에니메이션(2차베지어곡선)
	//제어점(3)-> 2차방정식
	//제어점(4)-> 3차방정식
// 3, 오브젝트의 회전 구분
class StateMove : public State
{
public:
	std::vector<Sphere>   m_MovePatternA;
	std::vector<Sphere>   m_MovePatternB;
	int	m_iPathIndex = 0;
	int c0 = 0;
	int c1 = 1;
	bool m_bCurve = true;
public:
	virtual void	Process(Object2D* pPlayer) override;
	StateMove(ObjectAI* owner) : State(owner)
	{
		//m_MovePatternA.vCenter.x = rand() % g_rtClient.right;
		//m_MovePatternA.vCenter.y = rand() % g_rtClient.bottom;
		Sphere v;
		v.vCenter.x = 400.0f; v.vCenter.y = 600.0f;
		m_MovePatternA.push_back(v);
		v.vCenter.x = 800.0f; v.vCenter.y = 0.0f;
		m_MovePatternA.push_back(v);
		v.vCenter.x = 0.0f;	v.vCenter.y = 300.0f;
		m_MovePatternA.push_back(v);
		v.vCenter.x = 800.0f; v.vCenter.y = 600.0f;
		m_MovePatternA.push_back(v);
		v.vCenter.x = 400.0f; v.vCenter.y = 0.0f;
		m_MovePatternA.push_back(v);
		v.vCenter.x = 0.0f; v.vCenter.y = 600.0f;
		m_MovePatternA.push_back(v);
		v.vCenter.x = 800.0f; v.vCenter.y = 300.0f;
		m_MovePatternA.push_back(v);
		v.vCenter.x = 0.0f;	v.vCenter.y = 0.0f;
		m_MovePatternA.push_back(v);
	}
};

