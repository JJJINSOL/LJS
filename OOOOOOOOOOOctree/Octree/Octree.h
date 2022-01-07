#pragma once
#include "Node.h"
class Octree
{
public:
	int m_SizeX;
	int m_SizeY;
	int m_SizeZ;
	int m_MaxDep;

	int count;

	Node* m_RootNode;

	std::queue<Node*> g_Queue;

	void Init(int xsize, int ysize, int zsize, int maxdep);
	Node* CreateNode(Node* parent, float x, float y, float z, float w, float h, float q);
	void BuildTree(Node* parent);
	bool AddObject(Object* ob, int obindex);
	bool DeleteObject(Node* nd);
	Node* FindNode(Node* node, Box bx);

	void PrintfObjectList(Node* node);
};

