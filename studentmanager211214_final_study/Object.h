#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream> // std::cout, std::in
#include <memory.h>
#include <stdlib.h>
class Object
{
public:
	int m_type;
	char m_name[64];
	char m_Buffer[1000];
	int m_buffersize;
	int m_currentposition;

	//순수 가상 함수
	virtual void Save();
	virtual void Load();
	virtual void Show();

	Object()
	{
		m_type = -1;
		m_buffersize = 0;
		m_currentposition = 0;
		memset(m_name, 0, sizeof(char) * 64);
		memset(m_Buffer, 0, sizeof(char) * 1000);
	}
	~Object()
	{
		m_type = -1;
	}
};

