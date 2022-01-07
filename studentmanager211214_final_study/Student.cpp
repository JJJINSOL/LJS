#include "Student.h"
template<typename T>
T Student:: operator[] (int i)
{
	if (i == Index) return m_index;
	if (i == Type) return m_type;
	if (i == Total) return m_total;
	return -1;
}
template<>
float Student:: operator[] (int i)
{
	if (i == Average) return m_average;
	return -1.0f;
}
void Student::SetData(int iIndex)
{
	m_index = iIndex;
	m_total = 0;
	strcpy(m_name, "none");
	m_average = m_total / 3.0f;
}
void Student::Show() 
{
	std::cout << operator[]<int>(Index) << " " << operator[]<int>(Type) << " " << operator[]<float>(Average) << std::endl;
}
void Student::Save() 
{
	char* data = &m_Buffer[m_currentposition];
	memcpy(data, &m_index, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(data, &m_type, sizeof(int));
	m_currentposition += sizeof(int);
}
void Student::Load() 
{
	char* data = &m_Buffer[m_currentposition];
	memcpy(&m_index, data, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(&m_type, data, sizeof(int));
	m_currentposition += sizeof(int);
}