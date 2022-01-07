#include "Octree.h"
void Octree::Init(int xsize, int ysize, int zsize, int maxdep)
{
	m_SizeX = xsize;
	m_SizeY = ysize;
	m_SizeZ = zsize;
	m_MaxDep = maxdep;
	count = 0;
	m_RootNode = CreateNode(nullptr, 0, 0, 0, m_SizeX, m_SizeY, m_SizeZ);
	BuildTree(m_RootNode);
}
Node* Octree::CreateNode(Node* parent, float x, float y, float z, float w, float h, float d)
{
	Node* nd = new Node(x, y, z, w, h, d);
	nd->index = count++;
	if (parent != nullptr)
	{
		nd->m_Depth = parent->m_Depth + 1;
	}
	return nd;
}
void Octree::BuildTree(Node* parent)
{
	if (parent->m_Depth == m_MaxDep) return;
	parent->m_Child[0] = CreateNode(
		parent,
		parent->m_box.vMin.x, parent->m_box.vMin.y, parent->m_box.vMin.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[0]);
	parent->m_Child[1] = CreateNode(
		parent,
		parent->m_box.vMiddle.x, parent->m_box.vMin.y, parent->m_box.vMin.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[1]);
	parent->m_Child[2] = CreateNode(
		parent,
		parent->m_box.vMiddle.x, parent->m_box.vMiddle.y, parent->m_box.vMin.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[2]);
	parent->m_Child[3] = CreateNode(
		parent,
		parent->m_box.vMin.x, parent->m_box.vMiddle.y, parent->m_box.vMin.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[3]);

	parent->m_Child[4] = CreateNode(
		parent,
		parent->m_box.vMin.x, parent->m_box.vMin.y, parent->m_box.vMiddle.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[4]);
	parent->m_Child[5] = CreateNode(
		parent,
		parent->m_box.vMiddle.x, parent->m_box.vMin.y, parent->m_box.vMiddle.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[5]);
	parent->m_Child[6] = CreateNode(
		parent,
		parent->m_box.vMiddle.x, parent->m_box.vMiddle.y, parent->m_box.vMiddle.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[6]);
	parent->m_Child[7] = CreateNode(
		parent,
		parent->m_box.vMin.x, parent->m_box.vMiddle.y, parent->m_box.vMiddle.z,
		parent->m_box.vSize.x / 2.0f, parent->m_box.vSize.y / 2.0f, parent->m_box.vSize.z / 2.0f);
	BuildTree(parent->m_Child[7]);
}
bool Octree::AddObject(Object* ob, int obindex)
{
	Node* find = FindNode(m_RootNode, ob->m_box);
	if (find != nullptr)
	{
		ob->m_obindex = obindex;
		find->AddObject(ob);
		return true;
	}
	return false;
}
bool Octree::DeleteObject(Node* nd)
{
	if (nd == nullptr) return false;
	nd->m_ObjectList.clear();
	for (int i = 0; i < 8; i++)
	{
		DeleteObject(nd->m_Child[i]);
	}
	return true;

}
Node* Octree::FindNode(Node* node, Box bx)
{
	do
	{
		for (int i = 0; i < 8; i++)
		{
			if (node->m_Child[i] != nullptr)
			{
				//Box ibox;
				Type u = Collision::BoxToBox(node->m_Child[i]->m_box, bx);
				if (u == RECT_IN)
				{
					g_Queue.push(node->m_Child[i]);
					break;
				}
			}
		}
		if (g_Queue.empty())break;
		node = g_Queue.front();
		g_Queue.pop();
	} while (node);
	return node;
}

void Octree::PrintfObjectList(Node* node)
{
	if (node == nullptr) return;
	for (std::list<Object*>::iterator i = node->m_ObjectList.begin(); i != node->m_ObjectList.end(); i++)
	{
		Object* ob = *i;
		std::cout << "[" << node->index << "]" << "[" << node->m_Depth << "]" << "[" << ob->m_obindex << "]" << ob->m_vPos.x << ":" << ob->m_vPos.y << ":" << ob->m_vPos.z << " / ";
	}
	std::cout << std::endl;
	for (int a = 0; a < 8; a++)
	{
		PrintfObjectList(node->m_Child[a]);
	}
}