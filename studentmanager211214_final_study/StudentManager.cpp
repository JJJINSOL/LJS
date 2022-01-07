#include "StudentManager.h"
NodeBox<Student>* const StudentManager::NewNode()
{
	NodeBox<Student>* inodebox = new NodeBox<Student>();
	int itype = rand() % 3;
	Student* makeob = nullptr;
	if (itype = 0)
		makeob = new StudentMiddle();
	else if (itype = 1)
		makeob = new StudentHigh();
	else
		makeob = new StudentCollege();

	makeob->SetData(m_list. m_nodenumber);
	inodebox->m_next = makeob;
	return inodebox;
}
