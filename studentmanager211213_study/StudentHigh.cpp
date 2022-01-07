#include "StudentHigh.h"

void StudentHigh::SetData(int index)
{
	m_index = index;
	strcpy(m_name, "none");
	m_kor = rand() % 100;
	m_eng = rand() % 100;
	m_mat = rand() % 100;
	m_total = m_kor + m_eng + m_mat;
	m_average = m_total / 3.0f;

}

void StudentHigh::Show()
{
	std::cout << m_index << " " << m_type << " " << m_kor << " " << m_eng << " " << m_mat << " " << m_total << " " << m_average << std::endl;
}
void StudentHigh::Save()
{
	Student::Save();
	char* uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_kor, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_eng, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(uData, &m_mat, sizeof(int));
	m_nowposition += sizeof(int);

	m_buffersize = m_nowposition;
}
void StudentHigh::Load()
{
	Student:: Load();
	char* uData = &m_buffer[m_nowposition];
	memcpy(&m_kor, uData, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(&m_eng, uData, sizeof(int));
	m_nowposition += sizeof(int);

	uData = &m_buffer[m_nowposition];
	memcpy(&m_mat, uData, sizeof(int));
	m_nowposition += sizeof(int);

	m_total = m_kor + m_eng + m_mat;
	m_average = m_total / 3.0f;

}