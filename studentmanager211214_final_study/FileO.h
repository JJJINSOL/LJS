#pragma once
#include <iostream>
class FileO
{
	FILE* m_stream = nullptr;
public:
	bool CreateFile(const char* filename);
	bool OpenFile(const char* filename);
	bool CloseFile();
	size_t Write(void const* data, int size);
	size_t Read(void * data, int size);
};

