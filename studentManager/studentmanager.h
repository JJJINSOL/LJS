#pragma once
#include "student.h"

class studentmanager
{
public:
	studentmanager()
	{
		uMaxCount = 0;
		HeadUser = 0;
		EndUser = 0;
	}
	~studentmanager()
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
	Student* HeadUser;
	Student* EndUser;
public:
	//��� ���� �Լ�
	Student* NewNode();
	//��� ���Ḯ��Ʈ ����� �Լ�
	void AddLink(Student* newuser);
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

