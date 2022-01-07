#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory.h>
#include <stdlib.h>
class Object
{
public:
	int m_buffersize;
	char m_name[64];
	char m_buffer[1000];
	int m_nowposition;
	int m_type;

	virtual void Load() = 0;
	virtual void Save() = 0;
	virtual void Show() = 0;
	Object()
	{
		m_buffersize = 0;
		memset(m_name,0,sizeof(char)*64);
		memset(m_buffer, 0, sizeof(char) * 1000);
		m_nowposition = 0;
		m_type = -1;
	}
	~Object()
	{
		m_type = -1;
	}
};

