#include "StudentHigh.h"
template <typename T>
T StudentHigh::operator[] (int i)
{
	int a = Student::operator[]<int>(i);
	if (a < 0)
	{
		if (i == 2) return m_kor;
		if (i == 3) return m_eng;
		if (i == 4) return m_mat;
	}
	return a;
}
template <>
float StudentHigh:: operator[] (int i)
{
	int a = Student::operator[]<float>(i);
	if (a < 0)
	{
		if (i == 6) return m_average;
	}
	return a;
}
void StudentHigh::SetData(int iIndex)
{
	m_index = iIndex;
	strcpy(m_name, "none");
	m_kor = rand() % 100;
	m_eng = rand() % 100;
	m_mat = rand() % 100;
	m_total = m_kor + m_eng + m_mat;
	m_average = m_total / 3.0f;
}
void StudentHigh::Show()
{
	for(int i=0; i<6; i++)
	{
		std::cout << operator[]<int>(i) << " ";
	}
	std::cout << operator[]<int>(6) << std::endl;
}
void StudentHigh::Save()
{
	Student::Save();
	char* data = &m_Buffer[m_currentposition];
	memcpy(data, &m_kor, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(data, &m_eng, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(data, &m_mat, sizeof(int));
	m_currentposition += sizeof(int);

	m_buffersize = m_currentposition;
}
void StudentHigh::Load()
{
	Student::Load();
	char* data = &m_Buffer[m_currentposition];
	memcpy(&m_kor, data, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(&m_eng, data, sizeof(int));
	m_currentposition += sizeof(int);

	data = &m_Buffer[m_currentposition];
	memcpy(&m_mat, data, sizeof(int));
	m_currentposition += sizeof(int);

	m_total = m_kor + m_eng + m_mat;
	m_average = m_total / 3.0f;
}
