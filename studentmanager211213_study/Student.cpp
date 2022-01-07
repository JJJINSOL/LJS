#include "Student.h"

void Student::SetData(int index)
{
	m_index = index;
	m_total = 0;
	m_average = m_total/3.0f;
	strcpy(m_name, "none");
}

void Student:: Show()
{
	std::cout << m_index << " " << m_total << " " << m_average << std::endl;
}
void Student::Save()
{
	char* uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_index, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_type, sizeof(int));
	m_nowposition += sizeof(int);

}
void Student::Load()
{
	char * uData = &m_buffer[m_nowposition];
	memcpy(&m_index, uData, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(&m_type, uData, sizeof(int));
	m_nowposition += sizeof(int);

}

