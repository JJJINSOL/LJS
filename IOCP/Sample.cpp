//IOCP - 통신에서 이용하며, 파일을 소켓으로해서 함
//동시에 돌아가야해서 쓰레드를 사용함
#include <windows.h>
#include <iostream>
//104857600
#define BlockSize (6553600)	
#define MAX_WORKER_THREAD 3

using namespace std;

LARGE_INTEGER filesize;
WCHAR* g_buffer = NULL;

//원래는 한개씩 읽고 한개씩 쓰고 그래서 배열 필요 없었지만 쓰레드를 사용하기에
//순서가 바뀔 수 있으니까 배열을 사용 g_dwCurrentRead/g_dwCurrentWrite은 배열 인덱스
OVERLAPPED g_ReadOVArray[1000];
OVERLAPPED g_WriteOVArray[1000];
DWORD	   g_dwCurrentRead = 0;
DWORD	   g_dwCurrentWrite = 0;

HANDLE g_hFileRead;
HANDLE g_hFileWrite;
HANDLE g_hWorkThread[MAX_WORKER_THREAD];

//완료 포트 관리 객체
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
		//WAIT_OBJECT_0 - 지정한 오브젝트가 시그널 상태로 되었다.
		if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		//IOCP의 입출력 완료 대기열로 부터 입출력 완료를 기다린다. 만약 대기열에 완료가 없다면, 이 함수는 대기열에 입출력 완료가 있을 때까지 대기한다.
		//완료 큐에 데이터가 있으면 작업시작
		//쓰레드가 완료한 작업이 있는지 물어봄
		//(핸들/전송된 데이터 크기/key/송신,수신 함수 호출시 Overlapped 구조체 변수 주소를 저장할 변수의 주소)
		BOOL bReturn = ::GetQueuedCompletionStatus(g_hIOCP,&dwTransfer,&KeyValue,&pOverlapped, 1);

		if (bReturn == TRUE)
		{
			if (KeyValue == 1111)
			{
				if (!DispatchRead(dwTransfer, pOverlapped))
				{
					//SetEvent- g_hKillEvent를 시그널 상태로 함
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
	//(무시,NULL/TRUE이면 리셋 반드시 시켜야함/초기 이벤트 상태 = flase/이벤트 이름)
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

	// 비동기입출력 작업결과 저장 큐
	//(장치에 대한 핸들(파일, 소켓 등)/CP전달/사용자가 임의로 전달할 인자 값/동일 시간에 동시에 수행할 수 있는 스레드의 최대 개수)
	//마지막 인자에 0을 전달하면 설치된 CPU의 개수를 동시에 수행 가능한 스레드의 최대 개수로 설정
	//I / O Completion Port생성을 위해서는 INVALID_HANDLE_VALUE
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
	::CreateIoCompletionPort(g_hFileRead, g_hIOCP, 1111, 0);
	::CreateIoCompletionPort(g_hFileWrite, g_hIOCP, 2222, 0);

	GetFileSizeEx(g_hFileRead, &filesize);

	g_buffer = new WCHAR[BlockSize];

	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;
		//CreateThread(보안 속성자, 스택사이즈, 윈도우프로시저같은 사용자 생성 함수, 앞으 함수 인자값, 스레드 일 언제 시킬지 선택, 생성된 스레드에 대한 index)
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

	//비동기 로드
	DWORD dwRead;
	//이거 없으면 파일 생성 안됨
	BOOL ret = ReadFile(g_hFileRead, g_buffer, BlockSize, &dwRead, &g_ReadOVArray[g_dwCurrentRead++]);

	//3. TRUE -  모든 배열에 있는 모든 객체 핸들이 시그널 상태가 되어야 반환한다
	//4. INFINITE - 있으면 반환, 없으면 놀고 있음
	//4. 1 이면 생성되다가 만다.
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