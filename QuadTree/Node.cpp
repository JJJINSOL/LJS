#include "Node.h"
//���ο��� �ԷµǴ� ������Ʈ �߰�
void Node::AddObject(int x, int y)
{
	m_objectlist.push_back(new Object(x, y));
}
//���ο��� �Էµ� ������Ʈ ��ġ�� �̰��� �´��� Ȯ��
    