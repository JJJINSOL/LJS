#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <iostream>
using namespace std;

//�ڵ� 4����
SQLHENV g_hEnv; //ȯ�� �ڵ�(�θ�) �θ� - X
SQLHDBC g_hDbc; //���� �ڵ�(�ڽ�) ���� - ȯ��
SQLHSTMT g_hStmt; //��� �ڵ�(�ڽ�->�ڽ�) �θ� - ����
SQLHDESC g_hDesc; //������ �ڵ�

//���� üũ
void Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	//       ȯ��h   ����h   ���h    ���� Ŭ����   ���� ���� �ڵ�  msg �ؽ�Ʈ����   <-���� �ִ� ����     <-<-���ۿ� �����ϴµ� ����� �� �ִ� ����
	SQLError(g_hEnv, g_hDbc, g_hStmt, szSQLState,   &iSQLCode,      errorBuffer,     sizeof(errorBuffer), &length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}

void main()
{
	//ȯ�� �ڵ�(�޸�)�Ҵ�------------------------------------------------------
	//(�ڵ�Ÿ��/������ �θ� �ڵ�/������ �ڵ� �ּ�)
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS)
	{
		//�ڵ� �Ҵ��� �����ϸ�
		return;
	}
	//ȯ�� �ڵ� �Ӽ� ����(����)
	//(ȯ���ڵ�/�Ӽ� ���� ����(����)/�Ӽ� �� ����(����)/�Ӽ� �� ����)
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		//���н�
		return;
	}
	//-----------------------------------------------------------------------------
	//���� �ڵ� �Ҵ�---------------------------------------------------------------
	if (SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc) != SQL_SUCCESS)
	{
		//���н�
		return;
	}
	//-----------------------------------------------------------------------------
	// //���� �ڵ��� �̿��Ͽ� �����ͺ��̽�(DB)�� ���� - SQLDriverConnect-----------
	//W - �����ڵ� ���� / MAX_PATH - �ִ�ġ
	SQLWCHAR dir[MAX_PATH] = { 0, };
	//���� ����̺� ��� ��������
	GetCurrentDirectory(MAX_PATH, dir);
	wstring dbpath = dir;
	dbpath+= L"\\cigarette.dsn";

	// ���ڿ� �տ� '_T'�� �ٿ��ָ� ������ ȯ�濡 ���� ���ڿ��� ó���� �� �ְ�  -> #include <tchar.h>
	// TCHAR - char Ÿ�� �ڵ� ����
	//SQLSMALLINT = short
	//c_str = string -> char
	SQLTCHAR OutCon[255];
	SQLSMALLINT cbOutCon;
	TCHAR InCon[256] = { 0, };
	int size = sizeof(OutCon);
	//ù��° ���ڿ� �� ���� ���ڰ��� �־���
	_stprintf(InCon, _T("FileDsn=%s"), dbpath.c_str());
	SQLSMALLINT cbOutLen;
	
	//(���� �ڵ�/������ �ڵ�/���� ���� ���ڿ�/���� ���ڿ� ����   / �Ϸ�� ���� ��Ʈ���� ���� ������/<-������ �ִ�ũ��/<-<-���ۿ� �����ϴµ� ����� �� �ִ� ���� ��/����Ʈ�� ǥ���� ����)
	//sizeof�� � ����ü��, �迭�� ��ü �������� byte�� �����شٸ�, _countof �� �迭�� ������ ���ϴ� ��ũ�� �̴�.
	SQLRETURN ret = SQLDriverConnect(g_hDbc,NULL, InCon,_countof(InCon), OutCon, _countof(OutCon), &cbOutLen, SQL_DRIVER_NOPROMPT);
	//SQL_SUCCESS_WITH_INFO - �����ε� �Ϻ��� ������ �ƴ�
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}
	//-----------------------------------------------------------------------------
	//��� �ڵ� �Ҵ�---------------------------------------------------------------
	//(�ڵ�Ÿ��/������ �θ� �ڵ�/������ �ڵ� �ּ�)
	if (SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt) != SQL_SUCCESS)
	{
		return;
	}
	//-----------------------------------------------------------------------------

	SQLLEN lenName, lenPrice, lenKorean;
	TCHAR Name[21];
	int Price;
	BOOL Korean;

	//-----------------------------------------------------------------------------

	//�÷��� ������ ���ε��� ���Ϲޱ� (DB ������ �޾ƿ���)
	// ���
	//               ���h   ���ε��� �÷� ��ȣ    <- ������ Ÿ��    ����� ���� ����    ���� ����     �÷� ���̳� ���� ����
	ret = SQLBindCol(g_hStmt, 1,                   SQL_UNICODE,      Name,               sizeof(Name), &lenName);
	ret = SQLBindCol(g_hStmt, 2,                   SQL_C_ULONG,      &Price,             0,            &lenPrice);
	ret = SQLBindCol(g_hStmt, 3,                   SQL_C_ULONG,      &Korean,            0,            &lenKorean);

	//select - ��ȸ (�� ���̺� ������ ��� ��ȸ)
	TCHAR sql[MAX_PATH] = L"select * from tblCigar";
	//                  ����h     spl��           ���ڿ�����
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		wcout << Name << " " << Price << " " << Korean << endl;
	}
	//�˻��ϰ� Ŀ���� �ٽ� ������ ���� ������ �۾��Ҷ� �ٽ� �տ��� ���� �� �� ����
	SQLCloseCursor(g_hStmt);

	//--------------------------------------------------------------------------------

	ret = SQLBindCol(g_hStmt, 1, SQL_C_ULONG, &Price, 0, &lenPrice);
	TCHAR sql2[MAX_PATH] = { 0, };
	//���۹��ڿ� ����
	wsprintf(sql2, L"select price from tblCigar where  name='%s'", L"���");
	//                  ����h     spl��           ���ڿ�����
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql2, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Price << std::endl;
	}
	SQLCloseCursor(g_hStmt);

	//������ �߰�**************************************************************************************************
	//TCHAR sql3[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	//wsprintf(sql3, L"insert into tblCigar (name,price, korean) values ('%s',%d,%d)", L"���÷���", 4100, 1);
	//ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	//if (ret != SQL_SUCCESS )
	//{
	//	Check();
	//	return;
	//}	
	//SQLCloseCursor(g_hStmt);

	//������ ����**************************************************************************************************
	//TCHAR sql3[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	//wsprintf(sql3, L"delete from tblCigar where name='%s'",
	//	L"xxxxxx");
	//ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	//if (ret != SQL_SUCCESS)
	//{
	//	Check();
	//	return;
	//}
	//SQLCloseCursor(g_hStmt);

	//������ ����**************************************************************************************************
	//TCHAR sql4[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	//wsprintf(sql4, L"update tblCigar set name='%s' where name='%s'",
	//	L"�ڷγ�", L"88 Light");
	//ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql4, SQL_NTS);
	//if (ret != SQL_SUCCESS)
	//{
	//	Check();
	//	return;
	//}

	SQLCloseCursor(g_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);

	SQLDisconnect(g_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);

	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}