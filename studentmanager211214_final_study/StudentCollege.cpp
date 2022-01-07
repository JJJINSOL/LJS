#include "StudentCollege.h"

template <typename T>
T StudentCollege::operator[] (int i)
{
	int a = Student::operator[]<int>(i);
	if (a < 0)
	{
		if (i == 2) return m_graphic;
		if (i == 3) return m_physics;
		if (i == 4) return m_ai;
	}
	return a;
}
template <>
float StudentCollege:: operator[] (int i)
{
	int a = Student::operator[]<float>(i);
	if (a < 0)
	{
		if (i == 6) return m_average;
	}
	return a;
}
void StudentCollege::SetData(int iIndex)
{
	m_index = iIndex;
	strcpy(m_name, "none");
	m_graphic = rand() % 100;
	m_physics = rand() % 100;
	m_ai = rand() % 100;
	m_total = m_graphic + m_physics + m_ai;
	m_average = m_total / 3.0f;
}
void StudentCollege::Show()
{
	for (int i = 0; i < 6; i++)
	{
		std::cout << operator[]<int>(i) << " ";
	}
	std::cout << operator[]<int>(6) << std::endl;
}
void StudentCollege::Save()
{
	Student::Save();
	char* data = &m_Buffer[m_currentposition];
	memcpy(data, &m_graphic, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(data, &m_physics, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(data, &m_ai, sizeof(int));
	m_currentposition += sizeof(int);

	m_buffersize = m_currentposition;
}
void StudentCollege::Load()
{
	Student::Load();
	char* data = &m_Buffer[m_currentposition];
	memcpy(&m_graphic, data, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(&m_physics, data, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(&m_ai, data, sizeof(int));
	m_currentposition += sizeof(int);

	m_total = m_graphic + m_physics + m_ai;
	m_average = m_total / 3.0f;
}
