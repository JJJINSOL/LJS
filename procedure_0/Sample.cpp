#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <tchar.h>
#include <vector>

SQLHENV henv = SQL_NULL_HENV;
SQLHDBC hdbc = SQL_NULL_HDBC;
SQLHSTMT g_hAccountStmt = SQL_NULL_HSTMT;
SQLHSTMT hstmt1 = SQL_NULL_HSTMT;
SQLHSTMT hstmtInfo = SQL_NULL_HSTMT;
SQLRETURN retcode;

struct TDataBinding
{
	//���̺� �԰� ����ִ� ����ü
	SQLSMALLINT type;
	SQLWCHAR* valuePtr;
	SQLINTEGER  length;
	SQLLEN      result;
	TDataBinding()
	{
	}
	TDataBinding(const TDataBinding& data)
	{
		type = data.type;
		length = data.length;
		result = data.result;
		valuePtr = new WCHAR[length];
		memcpy(valuePtr, data.valuePtr, length);
	}
	~TDataBinding()
	{
		delete valuePtr;
		valuePtr = nullptr;
	}
	std::vector< TDataBinding >     tablelist;
};
std::vector< TDataBinding > g_CatalogList;

void Error(SQLHENV env, SQLHDBC dbc, SQLHSTMT stmt)
{
	SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
	//���°�
	SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
	SQLINTEGER sqlcode;
	SQLSMALLINT length;
	while (SQLError(env, dbc, stmt,sqlstate, &sqlcode,buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length) == SQL_SUCCESS)
	{
		WCHAR szBuffer[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
		memcpy(szBuffer, buffer, length * sizeof(WCHAR));
		std::wcout << szBuffer;
	}
}
bool SQLSuccess(SQLRETURN ret)
{
	return(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}
SQLWCHAR id[10] = L"NONE";
SQLWCHAR ps[10] = L"NONE";
void CreatePrepare()
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &g_hAccountStmt);

	SWORD sReturn = 0; //���ν��� ��ȯ���޴� ��
	SQLLEN cbRetParam = SQL_NTS;
	//SQLBindCol - ������ ������
	//SQLBindParameter - �̰� ���� �Ѱ��ٶ�

	//1�� ? //?=call Account1Create(?,?)
	//                                   output - ��ȯ                                      ��ȯ��
	retcode = SQLBindParameter(g_hAccountStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	//2�� ? //?=call Account1Create(?,?)
	//                                   input - �Է�     <-�����ڵ�                                id��
	retcode = SQLBindParameter(g_hAccountStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id), 0, id, sizeof(id), NULL);

	//3�� ? //?=call Account1Create(?,?)
	retcode = SQLBindParameter(g_hAccountStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, sizeof(ps), 0, ps, sizeof(ps), NULL);

	//���ν��� ȣ��!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	TCHAR callsp[] = L"{?=call AccountCreate(?,?)}";
	//������ �������� �� �صΰڴ�/������ �������ϰ� �̸� ����->�����Ϻ��� ���� �������� �� ����
	retcode = SQLPrepare(g_hAccountStmt, callsp, SQL_NTS);
}
void ExecutePrepare(const TCHAR* szid, const TCHAR* szps)
{
	memcpy(id, szid, sizeof(id));
	memcpy(ps, szps, sizeof(ps));
	retcode = SQLExecute(g_hAccountStmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) //(0���� 1���� ���� ���� - ������ �ƴ� �ȴٶ�� ��)
	{
		Error(henv, hdbc, g_hAccountStmt);
		return;
	}
	//SQLMoreResults - ��ġ�� �� �ϰ��� ����� �����°� //��ȯ�������� 
	while (SQLMoreResults(g_hAccountStmt) != SQL_NO_DATA);

	//SQL_CLOSE - �ϳ��� ����� �����ڴ�
	SQLFreeStmt(g_hAccountStmt, SQL_CLOSE);
	SQLCloseCursor(g_hAccountStmt);
}
void PrintCatalog(TDataBinding* data)
{
	if (data->result != SQL_NULL_DATA)
	{
		std::wcout << "\nCatalog= " << data->valuePtr << std::endl;
	}
}
void GetDBInfo() 
{
	// ms sql -> odbc 3.8(��Ʈ���� ����/������ �Ķ���ͷ� ���� �� ����) -> ����̺�
	// SQLGetInfo - ���� ������ ��ȯ
	WCHAR szVer[20] = { 0, };//����3.5
	SQLSMALLINT cch = 0;
	// ODBC ���� Ȯ��
	int iOdbcMajor;
	int iOdbcMinor;
	retcode = SQLGetInfo(hdbc, SQL_DRIVER_ODBC_VER, szVer, _countof(szVer), &cch);
	if (SQL_SUCCEEDED(retcode)) {}

	//DB �̸�
	SQLWCHAR dbname[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_DATABASE_NAME, dbname, _countof(dbname), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	//DBMS �̸�
	SQLWCHAR dbmsname[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_DBMS_NAME, dbmsname, _countof(dbmsname), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	//DBMS ����
	SQLWCHAR dbmsver[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_DBMS_VER, dbmsver, _countof(dbmsver), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	//����� ����(��Ű��??)
	SQLWCHAR username[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_USER_NAME, username, _countof(username), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	
	//���̺���� ���� ��ȸ �Լ� - SQLTables();
	//���̺� ����Ʈ

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmtInfo);

	TDataBinding tbResult;
	tbResult.type = SQL_UNICODE;
	tbResult.length = 1024 + 1;
	tbResult.valuePtr = new WCHAR[tbResult.length];

	SQLBindCol(hstmtInfo, 1, tbResult.type, (SQLPOINTER)tbResult.valuePtr, tbResult.length, &tbResult.result);

	// table list
	// 0 -> catalog(db list) // �����ͺ��̽�
	// 1 -> schema ->dbo
	// 2 -> table name
	// 3 -> type

	//SQL_ALL_CATALOGS - ��� DB // LJSGAME DB�Ӹ� �ƴ϶� ����� ��� DB
	//īŻ�α׸� �޶�� �ǹ�
	retcode = SQLTables(hstmtInfo,
		(SQLWCHAR*)SQL_ALL_CATALOGS, SQL_NTS,
		(SQLWCHAR*)L"", SQL_NTS,
		(SQLWCHAR*)L"", SQL_NTS,
		(SQLWCHAR*)L"", SQL_NTS);

	for (retcode = SQLFetch(hstmtInfo); SQLSuccess(retcode); retcode = SQLFetch(hstmtInfo))
	{
		TDataBinding db = tbResult;
		g_CatalogList.push_back(db);
	}
	SQLCloseCursor(hstmtInfo);
	SQLFreeStmt(hstmtInfo, SQL_UNBIND);

	for (int iDB = 0; iDB < g_CatalogList.size(); iDB++)
	{
		// table list
		// 0 -> catalog(db list) // �����ͺ��̽�
		// 1 -> schema ->dbo
		// 2 -> table name
		// 3 -> type
		TDataBinding tbResult[4];
		for (int i = 0; i < 4; i++)
		{
			tbResult[i].type = SQL_UNICODE;
			tbResult[i].length = 1024 + 1;
			tbResult[i].valuePtr = new WCHAR[tbResult[i].length];

			SQLBindCol(hstmtInfo, i + 1,
						tbResult[i].type,
						(SQLPOINTER)tbResult[i].valuePtr,
						tbResult[i].length,
						&tbResult[i].result);
		}
 	
		//�������̶�� ��������
		retcode = SQLTables(hstmtInfo,  g_CatalogList[iDB].valuePtr, SQL_NTS, //�� �����ͺ��̽���
										username, SQL_NTS, //�� ��Ű���� ����ϰ� �ִ�
										(SQLWCHAR*)SQL_ALL_TABLE_TYPES, SQL_NTS, //������ �����ּ���
										(SQLWCHAR*)L"'TABLE'", SQL_NTS); //��� ���̺���

		for (retcode = SQLFetch(hstmtInfo);SQLSuccess(retcode); retcode = SQLFetch(hstmtInfo))
		{
			TDataBinding db = tbResult[2];
			g_CatalogList[iDB].tablelist.push_back(db);
			//PrintCatalog(tbResult);
		}
		SQLCloseCursor(hstmtInfo);
		SQLFreeStmt(hstmtInfo, SQL_UNBIND);
	}

	SQLCloseCursor(hstmtInfo);
	SQLFreeStmt(hstmtInfo, SQL_UNBIND);

}
void main()
{
	setlocale(LC_ALL, "korean");

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

	SQLSMALLINT cbCon;
	SQLWCHAR connStrbuf[1024] = { 0, };

	//��ȭ���ڴ� odbc �����ϴ� ���� ����-----------------------------------------------
	//SQL_DRIVER_PROMPT - ��ȭ ���� �׻� ����
	//SQL_DRIVER_NOPROMPT - ����ڿ��� ������ ��û�ϴ� ������Ʈ ǥ�� ����
	//retcode ����� 1=����� / -1=����ȵ�
	//retcode = SQLDriverConnect(hdbc, GetDesktopWindow(), (SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS,(SQLWCHAR*)connStrbuf, _countof(connStrbuf), &cbCon, SQL_DRIVER_PROMPT);
	//----------------------------------------------------------------------------------
	//����� DSN
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"LJS", SQL_NTS, (SQLWCHAR*)L"sa", SQL_NTS, (SQLWCHAR*)L"zhqhfdl147", SQL_NTS);

	//===============================================================

	GetDBInfo();

	CreatePrepare(); // �ʱ�ȭ

	ExecutePrepare(L"adduser1", L"123"); //�����Ѹ���� //����Ʈ - g_hAccountStmt


	//�ٷ� ������ �����Ҽ� �ְ� �Ķ���� ���ε� Ǯ���־�����
	//(���뵵 �����ϱ��ѵ� �������ν����� ����ڵ� ����°� ����)
	//���� ���ν����� �������̸� �׸�ŭ�� ����ڵ��� �����θ��
	//��� �ٷ� �� SQLFreeStmt �ʿ������
	//========================================================================
	/// <summary>
	/// ///
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt1);

	SWORD sReturn1 = 0;
	SQLLEN cbRetParam1 = SQL_NTS;
	retcode = SQLBindParameter(hstmt1, 1, SQL_PARAM_OUTPUT,
		SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn1, 0, &cbRetParam1);

	SQLWCHAR id1[10] = L"testuser";
	retcode = SQLBindParameter(hstmt1, 2, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id1), 0, id1, sizeof(id1), NULL);
	
	SQLWCHAR outps[10] = { 0, };
	retcode = SQLBindParameter(hstmt1, 3, SQL_PARAM_OUTPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(outps), 0, outps, sizeof(outps), NULL);

	TCHAR callsp1[] = L"{?=call CheckPasswordRet(?,?)}";
	retcode = SQLPrepare(hstmt1, callsp1, SQL_NTS);
	//retcode = SQLExecDirect(hstmt,callsp1, SQL_NTS);
	retcode = SQLExecute(hstmt1);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Error(henv, hdbc, hstmt1);
		return;
	}
	/*while (SQLFetch(hstmt) != SQL_NO_DATA)
	{
	}*/
	while (SQLMoreResults(hstmt1) != SQL_NO_DATA);
	SQLFreeStmt(hstmt1, SQL_UNBIND);
	SQLFreeStmt(hstmt1, SQL_RESET_PARAMS);
	SQLCloseCursor(hstmt1);
	/// </summary>
	return;
}



