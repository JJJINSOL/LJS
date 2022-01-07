#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include<iostream>

class Student
{
public:
	int m_uIndex;
	int m_uKor;
	int m_uEng;
	int m_uMat;
	int m_uTotal;
	float m_uAve;
	Student* m_uNext;

	int& operator [](int i);

	friend std::ostream& operator << (std::ostream& os, Student& data);

	//�⺻������
	Student()
	{
		m_uIndex = 0;
		m_uKor = 0;
		m_uNext = NULL;
	}
	//�����Ҷ� �ʱ�ȭ���� �޾Ƽ� ������
	Student(int i, int kor)
	{
		m_uIndex = i;
		m_uKor = kor;
		m_uNext = NULL;
	}
	//�⺻�Ҹ���
	~Student()
	{
	}
};