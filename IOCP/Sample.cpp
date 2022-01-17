//IOCP - ��ſ��� �̿��ϸ�, ������ ���������ؼ� ��
//���ÿ� ���ư����ؼ� �����带 �����
#include <windows.h>
#include <iostream>
//104857600
#define BlockSize (6553600)	
#define MAX_WORKER_THREAD 3

using namespace std;

LARGE_INTEGER filesize;
WCHAR* g_buffer = NULL;

//������ �Ѱ��� �а� �Ѱ��� ���� �׷��� �迭 �ʿ� �������� �����带 ����ϱ⿡
//������ �ٲ� �� �����ϱ� �迭�� ��� g_dwCurrentRead/g_dwCurrentWrite�� �迭 �ε���
OVERLAPPED g_ReadOVArray[1000];
OVERLAPPED g_WriteOVArray[1000];
DWORD	   g_dwCurrentRead = 0;
DWORD	   g_dwCurrentWrite = 0;

HANDLE g_hFileRead;
HANDLE g_hFileWrite;
HANDLE g_hWorkThread[MAX_WORKER_THREAD];

//�Ϸ� ��Ʈ ���� ��ü
HANDLE g_hIOCP;
LARGE_INTEGER g_LargeRead;
LARGE_INTEGER g_LargeWrite;
HANDLE g_hKillEvent;

bool DispatchRead(DWORD dwTransfer, OVERLAPPED* ov)
{
	g_LargeRead.QuadPart += dwTransfer;

	++g_dwCurrentWrite;
	g_WriteOVArray[g_dwCurrentWrite].Offset = g_LargeWrite.LowPart;
	g_WriteOVArray[g_dwCurrentWrite].OffsetHigh = g_LargeWrite.HighPart;

	DWORD dwWritten;
	BOOL ret = WriteFile(g_hFileWrite,g_buffer, dwTransfer, &dwWritten, &g_WriteOVArray[g_dwCurrentWrite]);
	if (ret == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool DispatchWrite(DWORD dwTransfer, OVERLAPPED* ov)
{
	g_LargeWrite.QuadPart += dwTransfer;

	if (filesize.QuadPart == g_LargeWrite.QuadPart)
	{
		::SetEvent(g_hKillEvent);
		return true;
	}

	++g_dwCurrentRead;
	g_ReadOVArray[g_dwCurrentRead].Offset = g_LargeRead.LowPart;
	g_ReadOVArray[g_dwCurrentRead].OffsetHigh = g_LargeRead.HighPart;

	DWORD dwRead;
	BOOL ret = ReadFile(g_hFileRead, g_buffer, BlockSize, &dwRead, &g_ReadOVArray[g_dwCurrentRead]);
	if (ret == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
DWORD WINAPI WorkerThread(LPVOID param)
{
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOverlapped;
	while (1)
	{
		//WAIT_OBJECT_0 - ������ ������Ʈ�� �ñ׳� ���·� �Ǿ���.
		if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		//IOCP�� ����� �Ϸ� ��⿭�� ���� ����� �ϷḦ ��ٸ���. ���� ��⿭�� �Ϸᰡ ���ٸ�, �� �Լ��� ��⿭�� ����� �Ϸᰡ ���� ������ ����Ѵ�.
		//�Ϸ� ť�� �����Ͱ� ������ �۾�����
		//�����尡 �Ϸ��� �۾��� �ִ��� ���
		//(�ڵ�/���۵� ������ ũ��/key/�۽�,���� �Լ� ȣ��� Overlapped ����ü ���� �ּҸ� ������ ������ �ּ�)
		BOOL bReturn = ::GetQueuedCompletionStatus(g_hIOCP,&dwTransfer,&KeyValue,&pOverlapped, 1);

		if (bReturn == TRUE)
		{
			if (KeyValue == 1111)
			{
				if (!DispatchRead(dwTransfer, pOverlapped))
				{
					//SetEvent- g_hKillEvent�� �ñ׳� ���·� ��
					::SetEvent(g_hKillEvent);
					break;
				}
			}
			if (KeyValue == 2222)
			{
				if (!DispatchWrite(dwTransfer, pOverlapped))
				{
					::SetEvent(g_hKillEvent);
					break;
				}
			}
		}
		else
		{
			if (GetLastError() != WAIT_TIMEOUT)
			{
				::SetEvent(g_hKillEvent);
				break;
			}
		}
	}
	return 1;

}
void main()
{
	setlocale(LC_ALL, "KOREAN");
	//(����,NULL/TRUE�̸� ���� �ݵ�� ���Ѿ���/�ʱ� �̺�Ʈ ���� = flase/�̺�Ʈ �̸�)
	g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	g_hFileRead = CreateFile(L"DXSDK_Jun10.exe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (g_hFileRead == INVALID_HANDLE_VALUE)
	{
		return;
	}
	g_hFileWrite = CreateFile(L"Copy.exe",GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (g_hFileWrite == INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hFileRead);
		return;
	}

	// �񵿱������ �۾���� ���� ť
	//(��ġ�� ���� �ڵ�(����, ���� ��)/CP����/����ڰ� ���Ƿ� ������ ���� ��/���� �ð��� ���ÿ� ������ �� �ִ� �������� �ִ� ����)
	//������ ���ڿ� 0�� �����ϸ� ��ġ�� CPU�� ������ ���ÿ� ���� ������ �������� �ִ� ������ ����
	//I / O Completion Port������ ���ؼ��� INVALID_HANDLE_VALUE
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
	::CreateIoCompletionPort(g_hFileRead, g_hIOCP, 1111, 0);
	::CreateIoCompletionPort(g_hFileWrite, g_hIOCP, 2222, 0);

	GetFileSizeEx(g_hFileRead, &filesize);

	g_buffer = new WCHAR[BlockSize];

	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;
		//CreateThread(���� �Ӽ���, ���û�����, ���������ν������� ����� ���� �Լ�, ���� �Լ� ���ڰ�, ������ �� ���� ��ų�� ����, ������ �����忡 ���� index)
		g_hWorkThread[i] = CreateThread(
			0, 
			0, 
			WorkerThread, 
			nullptr, 
			0, 
			&id);
	}
	g_LargeRead.QuadPart = 0;
	g_LargeWrite.QuadPart = 0;

	//�񵿱� �ε�
	DWORD dwRead;
	//�̰� ������ ���� ���� �ȵ�
	BOOL ret = ReadFile(g_hFileRead, g_buffer, BlockSize, &dwRead, &g_ReadOVArray[g_dwCurrentRead++]);

	//3. TRUE -  ��� �迭�� �ִ� ��� ��ü �ڵ��� �ñ׳� ���°� �Ǿ�� ��ȯ�Ѵ�
	//4. INFINITE - ������ ��ȯ, ������ ��� ����
	//4. 1 �̸� �����Ǵٰ� ����.
	WaitForMultipleObjects(MAX_WORKER_THREAD, g_hWorkThread, TRUE, INFINITE);

	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		CloseHandle(g_hWorkThread[i]);
	}

	CloseHandle(g_hIOCP);
	CloseHandle(g_hFileRead);
	CloseHandle(g_hFileWrite);

	return;
}