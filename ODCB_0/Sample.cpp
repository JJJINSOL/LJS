#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <tchar.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <iostream>
using namespace std;

//핸들 4가지
SQLHENV g_hEnv; //환경 핸들(부모) 부모 - X
SQLHDBC g_hDbc; //연결 핸들(자식) 보모 - 환경
SQLHSTMT g_hStmt; //명령 핸들(자식->자식) 부모 - 연결
SQLHDESC g_hDesc; //설명자 핸들

//오류 체크
void Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	//       환경h   연결h   명령h    오류 클래스   고유 오류 코드  msg 텍스트버퍼   <-버퍼 최대 길이     <-<-버퍼에 리턴하는데 사용할 수 있는 길이
	SQLError(g_hEnv, g_hDbc, g_hStmt, szSQLState,   &iSQLCode,      errorBuffer,     sizeof(errorBuffer), &length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}

void main()
{
	//환경 핸들(메모리)할당------------------------------------------------------
	//(핸들타입/생성할 부모 핸들/생성할 핸들 주소)
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS)
	{
		//핸들 할당이 실패하면
		return;
	}
	//환경 핸들 속성 지정(세팅)
	//(환경핸들/속성 종류 지정(버전)/속성 값 지정(버전)/속성 값 길이)
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		//실패시
		return;
	}
	//-----------------------------------------------------------------------------
	//연결 핸들 할당---------------------------------------------------------------
	if (SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc) != SQL_SUCCESS)
	{
		//실패시
		return;
	}
	//-----------------------------------------------------------------------------
	// //연결 핸들을 이용하여 데이터베이스(DB)에 연결 - SQLDriverConnect-----------
	//W - 유니코드 버전 / MAX_PATH - 최대치
	SQLWCHAR dir[MAX_PATH] = { 0, };
	//현재 드라이브 경로 가져오기
	GetCurrentDirectory(MAX_PATH, dir);
	wstring dbpath = dir;
	dbpath+= L"\\cigarette.dsn";

	// 문자열 앞에 '_T'를 붙여주면 컴파일 환경에 따라 문자열을 처리할 수 있게  -> #include <tchar.h>
	// TCHAR - char 타입 자동 설정
	//SQLSMALLINT = short
	//c_str = string -> char
	SQLTCHAR OutCon[255];
	SQLSMALLINT cbOutCon;
	TCHAR InCon[256] = { 0, };
	int size = sizeof(OutCon);
	//첫번째 인자에 그 다음 인자값들 넣어줌
	_stprintf(InCon, _T("FileDsn=%s"), dbpath.c_str());
	SQLSMALLINT cbOutLen;
	
	//(연결 핸들/윈도우 핸들/연결 정보 문자열/연결 문자열 길이   / 완료된 연결 스트링의 버퍼 포인터/<-버퍼의 최대크기/<-<-버퍼에 리턴하는데 사용할 수 있는 버퍼 수/프롬트를 표시할 시점)
	//sizeof가 어떤 구조체나, 배열의 전체 사이즈의 byte를 구해준다면, _countof 는 배열의 개수를 구하는 매크로 이다.
	SQLRETURN ret = SQLDriverConnect(g_hDbc,NULL, InCon,_countof(InCon), OutCon, _countof(OutCon), &cbOutLen, SQL_DRIVER_NOPROMPT);
	//SQL_SUCCESS_WITH_INFO - 성공인데 완벽한 성공은 아님
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}
	//-----------------------------------------------------------------------------
	//명령 핸들 할당---------------------------------------------------------------
	//(핸들타입/생성할 부모 핸들/생성할 핸들 주소)
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

	//컬럼과 변수를 바인딩해 리턴받기 (DB 데이터 받아오기)
	// 결과
	//               명령h   바인딩될 컬럼 번호    <- 데이터 타입    결과값 저장 버퍼    버퍼 길이     컬럼 길이나 상태 리턴
	ret = SQLBindCol(g_hStmt, 1,                   SQL_UNICODE,      Name,               sizeof(Name), &lenName);
	ret = SQLBindCol(g_hStmt, 2,                   SQL_C_ULONG,      &Price,             0,            &lenPrice);
	ret = SQLBindCol(g_hStmt, 3,                   SQL_C_ULONG,      &Korean,            0,            &lenKorean);

	//select - 조회 (한 테이블 데이터 모두 조회)
	TCHAR sql[MAX_PATH] = L"select * from tblCigar";
	//                  연결h     spl문           문자열길이
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		wcout << Name << " " << Price << " " << Korean << endl;
	}
	//검색하고 커서를 다시 앞으로 빼야 다음에 작업할때 다시 앞에서 부터 할 수 있음
	SQLCloseCursor(g_hStmt);

	//--------------------------------------------------------------------------------

	ret = SQLBindCol(g_hStmt, 1, SQL_C_ULONG, &Price, 0, &lenPrice);
	TCHAR sql2[MAX_PATH] = { 0, };
	//버퍼문자열 복사
	wsprintf(sql2, L"select price from tblCigar where  name='%s'", L"장미");
	//                  연결h     spl문           문자열길이
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql2, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Price << std::endl;
	}
	SQLCloseCursor(g_hStmt);

	//데이터 추가**************************************************************************************************
	//TCHAR sql3[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	//wsprintf(sql3, L"insert into tblCigar (name,price, korean) values ('%s',%d,%d)", L"디스플러스", 4100, 1);
	//ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	//if (ret != SQL_SUCCESS )
	//{
	//	Check();
	//	return;
	//}	
	//SQLCloseCursor(g_hStmt);

	//데이터 삭제**************************************************************************************************
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

	//데이터 수정**************************************************************************************************
	//TCHAR sql4[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	//wsprintf(sql4, L"update tblCigar set name='%s' where name='%s'",
	//	L"코로나", L"88 Light");
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