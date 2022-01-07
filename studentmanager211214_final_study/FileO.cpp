#include "FileO.h"
bool FileO:: CreateFile(const char* filename)
{
	if (m_stream != nullptr) return false;
	fopen_s(&m_stream, filename, "wb");
	if (m_stream == nullptr) return false;
	return true;
}
bool FileO::OpenFile(const char* filename)
{
	if (m_stream != nullptr) return false;
	fopen_s(&m_stream, filename, "rb");
	if (m_stream == nullptr) return false;
	return true;
}
bool FileO::CloseFile()
{
	fclose(m_stream);
	m_stream = nullptr;
}
size_t FileO::Write(void const* data, int size)
{
	if (m_stream == nullptr) return -1;
	size_t size = fwrite(data, size, 1, m_stream);
	return size;
}
size_t FileO::Read(void* data, int size)
{
	if (m_stream == nullptr) return -1;
	size_t size = fread(data, size, 1, m_stream);
	return size;
}