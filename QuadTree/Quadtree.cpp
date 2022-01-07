#include "Quadtree.h"
//사용자가 공간의 크기와 분할할 숫자 입력하면 초기화하기
void Quadtree::Init(int width, int height, int maxdepth)
{
	m_width = width;
	m_maxdepth = maxdepth;
	m_height = height;

	m_root = CreateNode(nullptr, 0, 0, m_width, m_height);
	BulidTree(m_root);
}
//초기화/트리 만들기에서 호출되면 파라미터를 이용하여 노드를 생성함, 파라미터의 값은 생성될때 node() 생성자가 값에 맞게 초기화하도록 구현
Node* Quadtree:: CreateNode(Node* parent, float x, float y, float w, float h)
{
	Node* node = new Node(x, y, w, h);
	if (parent != nullptr)
	{
		node->m_depth = parent->m_depth + 1;
	}
	return node;
}
//쿼드트리만들어서 공간 분할하기
void Quadtree:: BulidTree(Node* parent)
{
	if (parent->m_depth == m_maxdepth)return;
	else
	{
		parent->m_child[0] = CreateNode(parent, parent->m_rect.p1.x, parent->m_rect.p1.y, parent->m_rect.size.x / 2.0f, parent->m_rect.size.y / 2.0f);
		BulidTree(parent->m_child[0]);

		parent->m_child[1] = CreateNode(parent, parent->m_rect.middle.x, parent->m_rect.p1.y, parent->m_rect.size.x / 2.0f, parent->m_rect.size.y / 2.0f);
		BulidTree(parent->m_child[1]);

		parent->m_child[2] = CreateNode(parent, parent->m_rect.middle.x, parent->m_rect.middle.y, parent->m_rect.size.x / 2.0f, parent->m_rect.size.y / 2.0f);
		BulidTree(parent->m_child[2]);

		parent->m_child[3] = CreateNode(parent, parent->m_rect.p1.x, parent->m_rect.middle.y, parent->m_rect.size.x / 2.0f, parent->m_rect.size.y / 2.0f);
		BulidTree(parent->m_child[3]);

	}
}

bool Quadtree:: AddObject(int x, int y)
{
	Node* findnode = FindNode(m_root, x, y);
	if (findnode != nullptr)
	{
		findnode->AddObject(x, y);
		return true;
	}
	return false;
}
//큐를 이용하여 오브젝트가 현재 위치하고 있는 영역의 노드를 찾음
Node* Quadtree:: FindNode(Node* node, int x, int y)
{
	do
	{
		for (int i = 0; i < 4; i++)
		{
			if (node->m_child[i] != nullptr)
			{
				if (node->m_child[i]->IsRect(x, y))
				{
					g_queue.push(node->m_child[i]);
					break;
				}
			}
		}
		if (g_queue.empty())break;
		node = g_queue.front();
		g_queue.pop();
	} while (node);
	return node;
}
//오브젝트가 어느 분할된 공간에 있는지 화면으로 출력
void Quadtree:: PrintObjectList(Node* node)
{
	if (node == nullptr) return;
	for (std::list<Object*>::iterator iter = node->m_objectlist.begin(); iter != node->m_objectlist.end(); iter++)
	{
		Object* u = *iter;
		std::cout << "[" << node->m_depth << "]" << u->m_pos.x << ":" << u->m_pos.y << " ";
	}
	std::cout << std:: endl;
	for (int i=0;i<4;i++)
	{
		PrintObjectList(node->m_child[i]);
	}
}