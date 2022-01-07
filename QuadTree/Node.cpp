#include "Node.h"
//메인에서 입력되는 오브젝트 추가
void Node::AddObject(int x, int y)
{
	m_objectlist.push_back(new Object(x, y));
}
//메인에서 입력된 오브젝트 위치가 이곳이 맞는지 확인
    