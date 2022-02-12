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
	//테이블 규격 잡아주는 구조체
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
	//상태값
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

	SWORD sReturn = 0; //프로시저 반환값받는 곳
	SQLLEN cbRetParam = SQL_NTS;
	//SQLBindCol - 리턴을 받을때
	//SQLBindParameter - 이건 값을 넘겨줄때

	//1번 ? //?=call Account1Create(?,?)
	//                                   output - 반환                                      반환값
	retcode = SQLBindParameter(g_hAccountStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	//2번 ? //?=call Account1Create(?,?)
	//                                   input - 입력     <-유니코드                                id값
	retcode = SQLBindParameter(g_hAccountStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id), 0, id, sizeof(id), NULL);

	//3번 ? //?=call Account1Create(?,?)
	retcode = SQLBindParameter(g_hAccountStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, sizeof(ps), 0, ps, sizeof(ps), NULL);

	//프로시저 호출!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	TCHAR callsp[] = L"{?=call AccountCreate(?,?)}";
	//사전에 컴파일을 다 해두겠다/쿼리를 직접안하고 미리 생성->컴파일부터 문법 검증까지 다 해줌
	retcode = SQLPrepare(g_hAccountStmt, callsp, SQL_NTS);
}
void ExecutePrepare(const TCHAR* szid, const TCHAR* szps)
{
	memcpy(id, szid, sizeof(id));
	memcpy(ps, szps, sizeof(ps));
	retcode = SQLExecute(g_hAccountStmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) //(0값과 1값을 갖고 있음 - 음수만 아님 된다라는 뜻)
	{
		Error(henv, hdbc, g_hAccountStmt);
		return;
	}
	//SQLMoreResults - 패치를 다 하고나서 결과를 보려는것 //반환값받을때 
	while (SQLMoreResults(g_hAccountStmt) != SQL_NO_DATA);

	//SQL_CLOSE - 하나의 명령을 끝내겠다
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
	// ms sql -> odbc 3.8(스트리밍 가능/게임을 파라미터로 받을 수 있음) -> 드라이브
	// SQLGetInfo - 많은 정보들 반환
	WCHAR szVer[20] = { 0, };//버전3.5
	SQLSMALLINT cch = 0;
	// ODBC 버전 확인
	int iOdbcMajor;
	int iOdbcMinor;
	retcode = SQLGetInfo(hdbc, SQL_DRIVER_ODBC_VER, szVer, _countof(szVer), &cch);
	if (SQL_SUCCEEDED(retcode)) {}

	//DB 이름
	SQLWCHAR dbname[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_DATABASE_NAME, dbname, _countof(dbname), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	//DBMS 이름
	SQLWCHAR dbmsname[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_DBMS_NAME, dbmsname, _countof(dbmsname), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	//DBMS 버전
	SQLWCHAR dbmsver[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_DBMS_VER, dbmsver, _countof(dbmsver), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	//연결된 유저(스키마??)
	SQLWCHAR username[1024] = { 0, };
	retcode = SQLGetInfo(hdbc, SQL_USER_NAME, username, _countof(username), &cch);
	if (SQL_SUCCEEDED(retcode)) {}
	
	//테이블관련 정보 조회 함수 - SQLTables();
	//테이블 리스트

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmtInfo);

	TDataBinding tbResult;
	tbResult.type = SQL_UNICODE;
	tbResult.length = 1024 + 1;
	tbResult.valuePtr = new WCHAR[tbResult.length];

	SQLBindCol(hstmtInfo, 1, tbResult.type, (SQLPOINTER)tbResult.valuePtr, tbResult.length, &tbResult.result);

	// table list
	// 0 -> catalog(db list) // 데이터베이스
	// 1 -> schema ->dbo
	// 2 -> table name
	// 3 -> type

	//SQL_ALL_CATALOGS - 모든 DB // LJSGAME DB뿐만 아니라 연결된 모든 DB
	//카탈로그만 달라는 의미
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
		// 0 -> catalog(db list) // 데이터베이스
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
 	
		//쿼리문이라고 생각하자
		retcode = SQLTables(hstmtInfo,  g_CatalogList[iDB].valuePtr, SQL_NTS, //이 데이터베이스의
										username, SQL_NTS, //이 스키마를 사용하고 있는
										(SQLWCHAR*)SQL_ALL_TABLE_TYPES, SQL_NTS, //내용을 돌려주세요
										(SQLWCHAR*)L"'TABLE'", SQL_NTS); //모든 테이블의

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

	//대화상자는 odbc 선택하는 곳이 없음-----------------------------------------------
	//SQL_DRIVER_PROMPT - 대화 상자 항상 시작
	//SQL_DRIVER_NOPROMPT - 사용자에게 정보를 요청하는 프롬프트 표시 안함
	//retcode 결과값 1=연결됨 / -1=연결안됨
	//retcode = SQLDriverConnect(hdbc, GetDesktopWindow(), (SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS,(SQLWCHAR*)connStrbuf, _countof(connStrbuf), &cbCon, SQL_DRIVER_PROMPT);
	//----------------------------------------------------------------------------------
	//사용자 DSN
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"LJS", SQL_NTS, (SQLWCHAR*)L"sa", SQL_NTS, (SQLWCHAR*)L"zhqhfdl147", SQL_NTS);

	//===============================================================

	GetDBInfo();

	CreatePrepare(); // 초기화

	ExecutePrepare(L"adduser1", L"123"); //유저한명생성 //디폴트 - g_hAccountStmt


	//바로 위에서 재사용할수 있게 파라미터 바인딩 풀어주었지만
	//(재사용도 가능하긴한데 저장프로시저당 명령핸들 만드는게 조음)
	//저장 프로시저가 여러개이면 그만큼의 명령핸들을 만들어두면됨
	//고로 바로 위 SQLFreeStmt 필요없긴함
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



