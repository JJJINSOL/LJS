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
	//���ϴ� ��ũ�� �ѿ뷮�� ��뷮�� ���ϴ� �Լ�==================================================
	//1.�뷮�� Ȯ���� ����̺��� ��Ʈ��� - lpRootPathName
	//2.Ŭ����Ʈ �� ���� - SectorsPerCluster
	//3.���ʹ� ����Ʈ - BytesPerSector
	//4.������� �ʴ� Ŭ����Ʈ�� �� - NumberOfFreeClusters
	//5.��ü Ŭ����Ʈ�� ����� �� �ִ� ���� - TotalNumberOfClusters
	GetDiskFreeSpace(lpRootPathName,&SectorsPerCluster,&BytesPerSector,&NumberOfFreeClusters,&TotalNumberOfClusters);

	//�� ������Ʈ�� �����ڵ��ε�, cout�� �����ڵ带 �������� �ʰ� ��Ƽ����Ʈ�� ����==================
	//�׷��� �����ڵ�� ������ִ� w�� ���� wcout���� ������ �������� �ѱ��� �ȳ���
	//�ֳĸ� ������� �⺻ ����������� �����Ǿ��ֱ⶧�� �׷��� �Ʒ� �ڵ�� �ѱ������� ����
	setlocale(LC_ALL, "KOREAN");

	//// ���ϻ���======================================================================================
	////(�����̸�/����,����/�������/���ȼӼ�/������ �����Ǵ� ���/���� �Ӽ�/�����ϱ� ���� �Ӽ��� ���� ������ �ڵ�)
	//HANDLE hFile = CreateFile(L"test.txt",GENERIC_WRITE, 0, NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); 
	////������ �����Ǿ��ٸ�!
	//if (hFile != NULL)
	//{
	//	WCHAR buffer[] = L"Hello~~~~~~~!!!!!!!";
	//	DWORD dwLength = sizeof(buffer);
	//	DWORD dwWritten;
	//	//���� �����͸� ���� �Լ�
	//	//(�����̳� IO ��ġ�� �ڵ�/����� �����͸� ���� ����/������ ����/��ϵ� ����Ʈ���� ���Ϲ޴°�/overlapped)
	//	//overlapped - �񵿱� ������� ���� overlapped ����ü �����ͷ� �����ϸ� NULL
	//	BOOL ret = WriteFile(hFile, buffer, dwLength,&dwWritten, NULL);
	//	if (ret == TRUE)
	//	{
	//		std::wcout << L"ohohohohoh~��¼���!"<<std::endl;
	//	}
	//}
	//CloseHandle(hFile);

	//// ���ϻ���=======================================================================================
	////OPEN_EXISTING - ������ ������ ��쿡�� ������ ����
	//HANDLE hFileRead = CreateFile(L"test.txt",GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL); 

	//if (hFileRead != NULL)
	//{
	//	WCHAR buffer[512] = { 0, };
	//	DWORD dwLength = sizeof(buffer);
	//	DWORD dwRead;
	//	//���� �����͸� �д� �Լ�
	//	//(�����̳� IO ��ġ�� �ڵ�/�����͸� �޾ƿ� ����/������ ����/�о���� ����Ʈ���� ���Ϲ޴°�/overlapped)
	//	//CreateFile�� GENERIC_READ(�б� ����)�� �ݵ�� �־�� ��
	//	BOOL ret = ReadFile(hFileRead, buffer, dwLength,&dwRead, NULL);
	//	if (ret == TRUE)
	//	{
	//		std::wcout << buffer;
	//	}
	//}
	//CloseHandle(hFileRead);
	
	//LARGE_INTEGER - LongLong /64 ��Ʈ signed integer �� �����ϱ� ���� ����ü
	//LowPart :���� 32��Ʈ DWORD ������
	//HighPart : ���� 32��Ʈ LONG ��
	//QUADPART : 64��Ʈ�� signed interg �� -> ���� ����Ÿ ��
	LARGE_INTEGER filesize;
	
	WCHAR* g_buffer = NULL;

	OVERLAPPED g_ReadOV = { 0, };
	OVERLAPPED g_WriteOV = { 0, };

	// ���ϻ���================================================================================================================
	//FILE_FLAG_OVERLAPPED - �񵿱� �����
	//FILE_FLAG_NO_BUFFERING - ������ �б�� ���⿡ �ý��� ĳ���� ������� �ʰ� �����̳� ��ġ�� ����.
	//                       - �� �÷��״� �ϵ� ��ũ ĳ���̳� �޸� �� ���ϵ鿡 ������ ��ġ�� ���� �ƴϴ�.
	HANDLE hFileAsync = CreateFile(L"Teams_windows_x64.exe",GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,NULL); 

	//INVALID_HANDLE_VALUE - ���� �������� / �����ϸ� �ڵ鰪 ��ȯ
	if (hFileAsync != INVALID_HANDLE_VALUE)
	{
		//���� ������ ������ (����/������ ������ ����)
		GetFileSizeEx(hFileAsync, &filesize);
		DWORD dwSize = 0;
		if (filesize.QuadPart % BytesPerSector != 0)
		{
			//dwsize = ���� ������ ������/���ʹ� ����Ʈ ->������������ ���������� ������
			dwSize = filesize.QuadPart / BytesPerSector;
			//�׷��� ���⼭ ���������� �־��� ���� +1�� �ϰ� ���ʹ� ����Ʈ�� �ٽ� ���ϸ�
			//�����͸� ���� �� �ִ� ���ʹ����Ʈ���� ������� �ּһ���� ������
			dwSize = (dwSize + 1) * BytesPerSector;
		}
		g_buffer = new WCHAR[filesize.QuadPart];
		DWORD dwRead;
		//(�����̳� IO ��ġ�� �ڵ�/�����͸� �޾ƿ� ����/������ ����/�о���� ����Ʈ���� ���Ϲ޴°�/overlapped)
		BOOL ret = ReadFile(hFileAsync, g_buffer,dwSize,&dwRead, &g_ReadOV);
		BOOL bPanding = FALSE;
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				wcout << L"�ε���!"<<endl;
				bPanding = TRUE;
			}
			else
			{
				wcout << L"�ε� ����" << endl;
				bPanding = FALSE;
			}
		}
		else
		{
			wcout << L"�ε� �Ϸ�@@@@@@!!!!!!" << endl;
		}
		//�ε� ���� ���
		//������� ������� ����Ʈ ���̴�. ������ �Ϸ� �ÿ��� ����� �����
		DWORD dwTrans = 0;
		BOOL bReturn;
		while(bPanding)
		{
			//GetOverlappedResult - ����� ��ȯ ������ �� ��ŭ �а� �ִٴ� �� �Ⱦ˷���
			//������ ���ڰ� false�̸� �񵿱�, true�̸� ����
			bReturn = GetOverlappedResult(hFileAsync, &g_ReadOV, &dwTrans,FALSE);
			if (bReturn = TRUE)
			{
				wcout << L"�ε� �Ϸ�@@@@@@!!!!!!" << endl;
				bPanding = FALSE;
			}
			else
			{
				//ERROR_IO_INCOMPLETE - �ߺ� �� �۾��� ���� ���� ������ ��Ÿ���� ���� �ڵ�
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					wcout << L".";
				}
				else
				{
					wcout << L"�ε� ����" << endl;
					bPanding = FALSE;
				}
			}
			Sleep(1000);
		}
	}
	CloseHandle(hFileAsync);
	//===========================================================================================================
	wcout << L"���� ����"<<endl;
	//���� ����
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
				wcout << L"�����!" << endl;
				bPanding = TRUE;
			}
			else
			{
				wcout << L"��� ����" << endl;
			}
		}
		else
		{
			wcout << L"��� �Ϸ�@@@@@@!!!!!!" << endl;
			bPanding = FALSE;
		}
		//��� ���� ���
		DWORD dwTrans = 0;
		BOOL bReturn;
		while (bPanding)
		{
			bReturn = GetOverlappedResult(hFileWriteAsync, &g_WriteOV, &dwTrans, FALSE);
			if (bReturn == TRUE)
			{
				wcout << L"��� �Ϸ�@@@@@@!!!!!!" << endl;
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
					wcout << L"�ε� ����" << endl;
					bPanding = FALSE;
				}
			}
			Sleep(1000);
		}
	}
	CloseHandle(hFileWriteAsync);
}