#pragma once
#include "LinkedList.cpp"
#include "FileO.h"
class StudentManager
{
	StudentManager() {}
	~StudentManager() { AllDelete(); }
	
	LinkedList<Student> m_list;
	FileO m_file;

	NodeBox<Student>* const NewNode();
	bool FileSave(const char* filename);
	void AllDelete();
	void Create();
	void Load(const char* filename);
	void Draw();
};

