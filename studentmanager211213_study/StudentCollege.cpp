#include "StudentCollege.h"

void StudentCollege::SetData(int index)
{
	m_index = index;
	strcpy(m_name, "none");
	m_graphic = rand() % 100;
	m_physics = rand() % 100;
	m_ai = rand() % 100;
	m_total = m_graphic + m_physics + m_ai;
	m_average = m_total / 3.0f;

}

void StudentCollege::Show()
{
	std::cout << m_index << " " << m_type << " " << m_graphic << " " << m_physics << " " << m_ai << " " << m_total << " " << m_average << std::endl;
}
void StudentCollege::Save()
{
	Student::Save();
	char* uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_graphic, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_physics, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_ai, sizeof(int));
	m_nowposition += sizeof(int);

	m_buffersize = m_nowposition;
}
void StudentCollege::Load()
{
	Student:: Load();
	char* uData = &m_buffer[m_nowposition];
	memcpy(&m_graphic, uData, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(&m_physics, uData, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(&m_ai, uData, sizeof(int));
	m_nowposition += sizeof(int);

	m_total = m_graphic + m_physics + m_ai;
	m_average = m_total / 3.0f;

}