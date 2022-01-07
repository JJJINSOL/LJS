#include "NodeBox.h"

Student* NodeBox:: NewData(int index)
{
	int type = rand() % 3;
	Student* user = nullptr;
	if (type == 0)
		user = new StudentMiddle();
	else if (type == 1)
		user = new StudentHigh();
	else
		user = new StudentCollege();
	user->SetData(index);
	return user;
}

NodeBox::NodeBox() 
{
	m_Data = nullptr;
}
NodeBox::NodeBox(int index) 
{
	m_Data = NewData(index);
}
NodeBox::~NodeBox() 
{
	delete m_Data;
}