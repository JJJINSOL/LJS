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
		
		//기본생성자
		User()
		{
			uIndex = 0;
			uKor = 0;
			uNext = NULL;
		}
		//생성할때 초기화값을 받아서 생성함
		User(int i, int kor)
		{
			uIndex = i;
			uKor = kor;
			uNext = NULL;
		}
		//기본소멸자
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
		//private 변수는 외부에서 직접 접근이 불가능하므로 값을 변경하거나 얻기위해서는 함수를 만들어준다.
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
		//노드 생성 함수
		User* NewNode();
		//노드 연결리스트 만드는 함수
		void AddLink(User* newuser);
		//데이터 생성 함수
		void Create();
		//연결리스트에 있는 데이터를 파일에 저장하는 함수
		bool FileSave();
		//연결리스트에 데이터 추가하는 함수
		int Insert(FILE* fp);
		//연결리스트의 데이터 전체 삭제하는 함수
		void DelAll();
		//파일에 있는 데이터를 연결리스트로 로드하는 함수
		void Load();
		//연결리스트에 있는 데이터 출력하는 함수
		void Draw();
};

