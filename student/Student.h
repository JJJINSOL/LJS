#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

class User
{
	public:
		int uIndex;
		int uKor;
		User* uNext;
		
		//�⺻������
		User()
		{
			uIndex = 0;
			uKor = 0;
			uNext = NULL;
		}
		//�����Ҷ� �ʱ�ȭ���� �޾Ƽ� ������
		User(int i, int kor)
		{
			uIndex = i;
			uKor = kor;
			uNext = NULL;
		}
		//�⺻�Ҹ���
		~User()
		{
		}
};
class Student
{
	public:
		Student() 
		{
			uMaxCount = 0;
			HeadUser = 0;
			EndUser = 0;
		}
		~Student()
		{
			DelAll();
		}
		//private ������ �ܺο��� ���� ������ �Ұ����ϹǷ� ���� �����ϰų� ������ؼ��� �Լ��� ������ش�.
		void setCount(int num)
		{
			uMaxCount = num;
		}
		int getCount()
		{
			return uMaxCount;
		}
	private:
		int uMaxCount;
		User* HeadUser;
		User* EndUser;
	public:
		//��� ���� �Լ�
		User* NewNode();
		//��� ���Ḯ��Ʈ ����� �Լ�
		void AddLink(User* newuser);
		//������ ���� �Լ�
		void Create();
		//���Ḯ��Ʈ�� �ִ� �����͸� ���Ͽ� �����ϴ� �Լ�
		bool FileSave();
		//���Ḯ��Ʈ�� ������ �߰��ϴ� �Լ�
		int Insert(FILE* fp);
		//���Ḯ��Ʈ�� ������ ��ü �����ϴ� �Լ�
		void DelAll();
		//���Ͽ� �ִ� �����͸� ���Ḯ��Ʈ�� �ε��ϴ� �Լ�
		void Load();
		//���Ḯ��Ʈ�� �ִ� ������ ����ϴ� �Լ�
		void Draw();
};

