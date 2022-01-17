#include <Windows.h>
#include <iostream>

using namespace std;
DWORD SectorsPerCluster;
DWORD BytesPerSector;
DWORD NumberOfFreeClusters;
DWORD TotalNumberOfClusters;

void main()
{
	WCHAR lpRootPathName[] = L"c:\\";
	//원하는 디스크의 총용량과 사용량을 구하는 함수==================================================
	//1.용량을 확인할 드라이브의 루트경로 - lpRootPathName
	//2.클러스트 당 섹터 - SectorsPerCluster
	//3.섹터당 바이트 - BytesPerSector
	//4.사용하지 않는 클러스트의 수 - NumberOfFreeClusters
	//5.전체 클러스트가 저장될 수 있는 변수 - TotalNumberOfClusters
	GetDiskFreeSpace(lpRootPathName,&SectorsPerCluster,&BytesPerSector,&NumberOfFreeClusters,&TotalNumberOfClusters);

	//이 프로젝트는 유니코드인데, cout은 유니코드를 지원하지 않고 멀티바이트를 지원==================
	//그래서 유니코드로 만들어주는 w가 붙은 wcout쓰면 영문은 나오지만 한글은 안나옴
	//왜냐면 윈도우는 기본 영어윈도우로 설정되어있기때문 그래서 아래 코드로 한글판으로 해줌
	setlocale(LC_ALL, "KOREAN");

	//// 파일생성======================================================================================
	////(파일이름/목적,권한/공유모드/보안속성/파일이 생성되는 방법/파일 속성/복사하기 위한 속성을 가진 파일의 핸들)
	//HANDLE hFile = CreateFile(L"test.txt",GENERIC_WRITE, 0, NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	////파일이 생성되었다면!
	//if (hFile != NULL)
	//{
	//	WCHAR buffer[] = L"Hello~~~~~~~!!!!!!!";
	//	DWORD dwLength = sizeof(buffer);
	//	DWORD dwWritten;
	//	//파일 데이터를 쓰는 함수
	//	//(파일이나 IO 장치의 핸들/기록할 데이터를 가진 버퍼/데이터 길이/기록된 바이트수를 리턴받는곳/overlapped)
	//	//overlapped - 비동기 입출력을 위한 overlapped 구조체 포인터로 사용안하면 NULL
	//	BOOL ret = WriteFile(hFile, buffer, dwLength,&dwWritten, NULL);
	//	if (ret == TRUE)
	//	{
	//		std::wcout << L"ohohohohoh~출력성공!"<<std::endl;
	//	}
	//}
	//CloseHandle(hFile);

	//// 파일생성=======================================================================================
	////OPEN_EXISTING - 파일이 존재할 경우에만 파일을 연다
	//HANDLE hFileRead = CreateFile(L"test.txt",GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL); 

	//if (hFileRead != NULL)
	//{
	//	WCHAR buffer[512] = { 0, };
	//	DWORD dwLength = sizeof(buffer);
	//	DWORD dwRead;
	//	//파일 데이터를 읽는 함수
	//	//(파일이나 IO 장치의 핸들/데이터를 받아올 버퍼/데이터 길이/읽어들인 바이트수를 리턴받는곳/overlapped)
	//	//CreateFile에 GENERIC_READ(읽기 권한)이 반드시 있어야 함
	//	BOOL ret = ReadFile(hFileRead, buffer, dwLength,&dwRead, NULL);
	//	if (ret == TRUE)
	//	{
	//		std::wcout << buffer;
	//	}
	//}
	//CloseHandle(hFileRead);
	
	//LARGE_INTEGER - LongLong /64 비트 signed integer 를 지원하기 위한 구조체
	//LowPart :하위 32비트 DWORD 형으로
	//HighPart : 상위 32비트 LONG 형
	//QUADPART : 64비트의 signed interg 형 -> 실제 데이타 값
	LARGE_INTEGER filesize;
	
	WCHAR* g_buffer = NULL;

	OVERLAPPED g_ReadOV = { 0, };
	OVERLAPPED g_WriteOV = { 0, };

	// 파일생성================================================================================================================
	//FILE_FLAG_OVERLAPPED - 비동기 입출력
	//FILE_FLAG_NO_BUFFERING - 데이터 읽기와 쓰기에 시스템 캐싱을 사용하지 않고 파일이나 장치를 연다.
	//                       - 이 플래그는 하드 디스크 캐싱이나 메모리 맵 파일들에 영향을 미치는 것은 아니다.
	HANDLE hFileAsync = CreateFile(L"Teams_windows_x64.exe",GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,NULL); 

	//INVALID_HANDLE_VALUE - 파일 생성실패 / 성공하면 핸들값 반환
	if (hFileAsync != INVALID_HANDLE_VALUE)
	{
		//파일 사이즈 얻어오기 (파일/사이즈 저장할 변수)
		GetFileSizeEx(hFileAsync, &filesize);
		DWORD dwSize = 0;
		if (filesize.QuadPart % BytesPerSector != 0)
		{
			//dwsize = 실제 데이터 사이즈/섹터당 바이트 ->실제데이터의 나머지값이 버려짐
			dwSize = filesize.QuadPart / BytesPerSector;
			//그래서 여기서 나머지값을 넣어줄 공간 +1을 하고 섹터당 바이트를 다시 곱하면
			//데이터를 넣을 수 있는 섹터당바이트값의 배수값인 최소사이즈가 생성됨
			dwSize = (dwSize + 1) * BytesPerSector;
		}
		g_buffer = new WCHAR[filesize.QuadPart];
		DWORD dwRead;
		//(파일이나 IO 장치의 핸들/데이터를 받아올 버퍼/데이터 길이/읽어들인 바이트수를 리턴받는곳/overlapped)
		BOOL ret = ReadFile(hFileAsync, g_buffer,dwSize,&dwRead, &g_ReadOV);
		BOOL bPanding = FALSE;
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				wcout << L"로드중!"<<endl;
				bPanding = TRUE;
			}
			else
			{
				wcout << L"로드 실패" << endl;
				bPanding = FALSE;
			}
		}
		else
		{
			wcout << L"로드 완료@@@@@@!!!!!!" << endl;
		}
		//로드 중인 경우
		//현재까지 입출력한 바이트 수이다. 하지만 완료 시에만 제대로 조사됨
		DWORD dwTrans = 0;
		BOOL bReturn;
		while(bPanding)
		{
			//GetOverlappedResult - 결과는 반환 하지만 얼마 만큼 읽고 있다는 건 안알려줌
			//마지막 인자가 false이면 비동기, true이면 동기
			bReturn = GetOverlappedResult(hFileAsync, &g_ReadOV, &dwTrans,FALSE);
			if (bReturn = TRUE)
			{
				wcout << L"로드 완료@@@@@@!!!!!!" << endl;
				bPanding = FALSE;
			}
			else
			{
				//ERROR_IO_INCOMPLETE - 중복 된 작업이 아직 진행 중임을 나타내는 오류 코드
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					wcout << L".";
				}
				else
				{
					wcout << L"로드 실패" << endl;
					bPanding = FALSE;
				}
			}
			Sleep(1000);
		}
	}
	CloseHandle(hFileAsync);
	//===========================================================================================================
	wcout << L"복사 시작"<<endl;
	//파일 생성
	//FILE_FLAG_WRITE_THROUGH - 
	HANDLE hFileWriteAsync = CreateFile(L"Copy.iso", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH, NULL);
	if (hFileWriteAsync != NULL)
	{
		DWORD dwLength = filesize.QuadPart;
		DWORD dwWritten;
		BOOL ret = WriteFile(hFileWriteAsync,g_buffer, dwLength, &dwWritten,&g_WriteOV);
		BOOL bPanding = FALSE;
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				wcout << L"출력중!" << endl;
				bPanding = TRUE;
			}
			else
			{
				wcout << L"출력 실패" << endl;
			}
		}
		else
		{
			wcout << L"출력 완료@@@@@@!!!!!!" << endl;
			bPanding = FALSE;
		}
		//출력 중인 경우
		DWORD dwTrans = 0;
		BOOL bReturn;
		while (bPanding)
		{
			bReturn = GetOverlappedResult(hFileWriteAsync, &g_WriteOV, &dwTrans, FALSE);
			if (bReturn == TRUE)
			{
				wcout << L"출력 완료@@@@@@!!!!!!" << endl;
				bPanding = FALSE;
			}
			else
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					wcout << L".";
				}
				else
				{
					wcout << L"로드 실패" << endl;
					bPanding = FALSE;
				}
			}
			Sleep(1000);
		}
	}
	CloseHandle(hFileWriteAsync);
}