#pragma once
#include "Node.h"
class Quadtree
{
public:
	int m_width;
	int m_height;
	int m_maxdepth;
	Node* m_root;

	std::vector<Node*> g_array;
	std::queue<Node*> g_queue;
	int g_value;

	void Init(int width, int height, int maxdepth);
	Node* CreateNode(Node* parent, float x, float y, float w, float h);
	void BulidTree(Node* parent);
	bool AddObject(int x, int y);
	Node* FindNode(Node* node, int x, int y);

	void PrintObjectList(Node* node);

};

