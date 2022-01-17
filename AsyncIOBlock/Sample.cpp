#include <windows.h>
#include <iostream>

#define BlockSize 2048	
using namespace std;
void main()
{
	setlocale(LC_ALL, "KOREAN");
	WCHAR* g_buffer = NULL;
	LARGE_INTEGER filesize;
	OVERLAPPED g_ReadOV = { 0, };
	OVERLAPPED g_WriteOV = { 0, };

	HANDLE hFileAsync;
	HANDLE hFileCopy;
	hFileAsync = CreateFile(L"Teams_windows_x64.exe", GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
	if (hFileAsync == INVALID_HANDLE_VALUE)
	{
		return;
	}
	hFileCopy = CreateFile(L"Copy.exe", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hFileCopy == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFileAsync);
		return;
	}
	GetFileSizeEx(hFileAsync, &filesize);

	DWORD dwRead;
	DWORD dwWritten;
	DWORD dwTotalWrite = 0;

	g_buffer = new WCHAR[BlockSize];
	for (int i = 0;; i++)
	{
		ZeroMemory(g_buffer, BlockSize);

		//비동기 로드===================================================================
		
		//Offset - 파일 포인터 하위 워드 
		//OffsetHigh - 파일 포인터 상위 워드
		g_ReadOV.Offset = i * BlockSize;
		g_ReadOV.OffsetHigh = 0;

		BOOL ret = ReadFile(hFileAsync, g_buffer, BlockSize, &dwRead, &g_ReadOV);
		BOOL bReturn = GetOverlappedResult(hFileAsync, &g_ReadOV, &dwRead, TRUE);

		//비동기 출력===================================================================
		g_WriteOV.Offset = i * BlockSize;
		g_WriteOV.OffsetHigh = 0;

		ret = WriteFile(hFileCopy, g_buffer, dwRead, &dwWritten, &g_WriteOV);
		bReturn = GetOverlappedResult(hFileCopy, &g_WriteOV, &dwWritten, TRUE);

		dwTotalWrite += dwWritten;

		if (filesize.LowPart == dwTotalWrite)
		{
			break;
		}
	}
	wcout << L"끝" << endl;
	CloseHandle(hFileAsync);
	CloseHandle(hFileCopy);
}