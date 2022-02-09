#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <tchar.h>

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
void main()
{
	setlocale(LC_ALL, "korean");
	SQLHENV henv = SQL_NULL_HENV;
	SQLHDBC hdbc = SQL_NULL_HDBC;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLHSTMT hstmt1 = SQL_NULL_HSTMT;
	SQLRETURN retcode;


	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);


	SQLSMALLINT cbCon;
	SQLWCHAR connStrbuf[1024] = { 0, };
	//SQL_DRIVER_PROMPT - 대화 상자 항상 시작
	//SQL_DRIVER_NOPROMPT - 사용자에게 정보를 요청하는 프롬프트 표시 안함
	//retcode 결과값 1=연결됨 / -1=연결안됨
	retcode = SQLDriverConnect(hdbc, GetDesktopWindow(), (SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS,(SQLWCHAR*)connStrbuf, _countof(connStrbuf), &cbCon, SQL_DRIVER_PROMPT);
	
	//===============================================================

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	SWORD sReturn = 0; //프로시저 반환값받는 곳
	SQLLEN cbRetParam = SQL_NTS; 
	//SQLBindCol - 리턴을 받을때
	//SQLBindParameter - 이건 값을 넘겨줄때
	
	//1번 ? //?=call Account1Create(?,?)
	//                                   output - 반환                                      반환값
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	//2번 ? //?=call Account1Create(?,?)
	SQLWCHAR id[10] = L"testuser";
	//                                   input - 입력     <-유니코드                                id값
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id), 0, id, sizeof(id), NULL);
	
	//3번 ? //?=call Account1Create(?,?)
	SQLWCHAR ps[10] = L"11111";
	retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, sizeof(ps), 0, ps, sizeof(ps), NULL);

	//프로시저 호출!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	TCHAR callsp[] = L"{?=call AccountCreate(?,?)}";
	//사전에 컴파일을 다 해두겠다/쿼리를 직접안하고 미리 생성->컴파일부터 문법 검증까지 다 해줌
	retcode = SQLPrepare(hstmt, callsp, SQL_NTS);
	//retcode = SQLExecDirect(hstmt,callsp, SQL_NTS);
	//SQLExecute - 한 번 이상 SQLPrepare()를 사용하여 준비된 명령문을 실행
	//원래는 SQLExecDirect(hstmt, callsp, SQL_NTS) 를 사용했었음
	//위에서 명령핸들이 만들어졌음으로 실행만 하면 됨!
	retcode = SQLExecute(hstmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) //(0값과 1값을 갖고 있음 - 음수만 아님 된다라는 뜻)
	{
		Error(henv, hdbc, hstmt);
		return;
	}
	//패치를 한다는건 뭔가를 받는다는것
	/*while (SQLFetch(hstmt) != SQL_NO_DATA)
	{
	}*/
	//반환값받을때
	//SQLMoreResults - 패치를 다 하고나서 결과를 보려는것
	while (SQLMoreResults(hstmt) != SQL_NO_DATA);
	
	//핸들 옵션
	//SQL_UNBIND - 파리미터 바인딩 된거 다 풀어주기 -> 사용안할거임
	//SQL_UNBIND - prepare한것도 없어짐 -> prepare를 하면 재사용하겠다는 의미
	//계속 사용하면 SQL_RESET_PARAMS만 쓰는데 이 코드에서는 마무리 부분이니 둘 다 사용
	SQLFreeStmt(hstmt, SQL_UNBIND);
	//SQL_RESET_PARAMS - 재사용한다는 뜻
	SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
	SQLCloseCursor(hstmt);

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

	TCHAR callsp1[] = L"{?=call CheckPaswordRet(?,?)}";
	retcode = SQLPrepare(hstmt1, callsp1, SQL_NTS);
	//retcode = SQLExecDirect(hstmt,callsp1, SQL_NTS);
	retcode = SQLExecute(hstmt1);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Error(henv, hdbc, hstmt);
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



