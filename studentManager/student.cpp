#include "student.h"

int& Student:: operator [](int i)
{
	if (i == 0) return m_uIndex;
	else if (i == 1)	return m_uKor;
	else if (i == 2)	return m_uEng;
	else if (i == 3)	return m_uMat;
	return m_uTotal;
}
std::ostream& operator << (std::ostream& os, Student& data)
{
	os << data[0] << " " << data[1] << " " << data[2] << " " << data[3] << " " << data[4] << " " << data.m_uAve << std::endl;
	return os;
}