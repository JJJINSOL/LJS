#pragma once
#include "Std.h"
#include <set>
#include <algorithm>

struct IndexMap
{
	int m_X;
	int m_Y;
	bool    operator ==(const IndexMap& p)
	{
		return (m_X == p.m_X && m_Y == p.m_Y);
	}
	IndexMap    operator +(const IndexMap& p)
	{
		return { m_X + p.m_X, m_Y + p.m_Y };
	}
	IndexMap(int x, int y)
	{
		m_X = x;
		m_Y = y;
	}
	IndexMap()
	{
		m_X = m_Y = 0;
	}
};
struct Node
{
	IndexMap	m_index;
	Node* m_pParent;
	DWORD		m_dwCost;
	DWORD		m_dwCostTotal;
	Node(IndexMap index, Node* parent = nullptr)
	{
		m_index = index;
		m_pParent = parent;
		m_dwCost = 0;
		m_dwCostTotal = 0;
	}
};
class AStar
{
	std::vector<IndexMap>  m_FildPathList;
public:
	IndexMap  m_WorldSize;
	std::vector<IndexMap>  m_Walls;
	std::vector<IndexMap>  m_Direction;
	std::set<Node*>  OpenList;
	std::set<Node*>  CloseList;
public:
	void		AddCollision(IndexMap index);
	bool		FindPath(IndexMap s, IndexMap e);
	bool		DetectCollision(IndexMap& index);
	Node* FindNodeOnList(std::set<Node*>& list, IndexMap& coord);
	IndexMap    GetDelta(IndexMap s, IndexMap e);
	AStar();
};

