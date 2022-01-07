#include "Quadtree.h"
//����ڰ� ������ ũ��� ������ ���� �Է��ϸ� �ʱ�ȭ�ϱ�
void Quadtree::Init(int width, int height, int maxdepth)
{
	m_width = width;
	m_maxdepth = maxdepth;
	m_height = height;

	m_root = CreateNode(nullptr, 0, 0, m_width, m_height);
	BulidTree(m_root);
}
//�ʱ�ȭ/Ʈ�� ����⿡�� ȣ��Ǹ� �Ķ���͸� �̿��Ͽ� ��带 ������, �Ķ������ ���� �����ɶ� node() �����ڰ� ���� �°� �ʱ�ȭ�ϵ��� ����
Node* Quadtree:: CreateNode(Node* parent, float x, float y, float w, float h)
{
	Node* node = new Node(x, y, w, h);
	if (parent != nullptr)
	{
		node->m_depth = parent->m_depth + 1;
	}
	return node;
}
//����Ʈ������ ���� �����ϱ�
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
//ť�� �̿��Ͽ� ������Ʈ�� ���� ��ġ�ϰ� �ִ� ������ ��带 ã��
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
//������Ʈ�� ��� ���ҵ� ������ �ִ��� ȭ������ ���
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