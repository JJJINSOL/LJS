#pragma once
#include "State.h"

//1,�н����ϸ��̼�(��������)->�̵�����
	//1)������ ������ ������ ==> v = p + d*t(speed*����ð�);
	//2)�������� = v(t) = (1-t)*v0 + t*v1;
// 2,��н����ϸ��̼�(2��������)
	//������(3)-> 2��������
	//������(4)-> 3��������
// 3, ������Ʈ�� ȸ�� ����
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

