#include "Odbc.h"
/* SQL data type codes */
//#define SQL_UNKNOWN_TYPE    0
//#define SQL_CHAR            1
//#define SQL_NUMERIC         2
//#define SQL_DECIMAL         3
//#define SQL_INTEGER         4
//#define SQL_SMALLINT        5
//#define SQL_FLOAT           6
//#define SQL_REAL            7
//#define SQL_DOUBLE          8
//#if (ODBCVER >= 0x0300)
//#define SQL_DATETIME        9
//#endif
//#define SQL_VARCHAR         12
//
///* One-parameter shortcuts for date/time data types */
//#if (ODBCVER >= 0x0300)
//#define SQL_TYPE_DATE       91
//#define SQL_TYPE_TIME       92
//#define SQL_TYPE_TIMESTAMP  93
//#endif
///* SQL extended datatypes */
//#define SQL_DATE                                9
//#if (ODBCVER >= 0x0300)
//#define SQL_INTERVAL                            10
//#endif  /* ODBCVER >= 0x0300 */
//#define SQL_TIME                                10
//#define SQL_TIMESTAMP                           11
//#define SQL_LONGVARCHAR                         (-1)
//#define SQL_BINARY                              (-2)
//#define SQL_VARBINARY                           (-3)
//#define SQL_LONGVARBINARY                       (-4)
//#define SQL_BIGINT                              (-5)
//#define SQL_TINYINT                             (-6)
//#define SQL_BIT                                 (-7)
//#define SQL_GUID                (-11)//
//#define SQL_WCHAR           (-8)
//#define SQL_WVARCHAR        (-9)
//#define SQL_WLONGVARCHAR    (-10)
//#define SQL_C_WCHAR         SQL_WCHAR
//
//#ifdef UNICODE
//#define SQL_C_TCHAR         SQL_C_WCHAR
//#else
//#define SQL_C_TCHAR         SQL_C_CHAR
//#endif
void main()
{
	setlocale(LC_ALL, "korean");
	Odbc odbc;
	odbc.Init();

	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	std::wstring dbpath = dir;
	dbpath += L"\\gameuser.dsn";
	//클라이언트 응용 프로그램이 데이터베이스를 찾고 연결하는 데 사용되는 포인터의 이름
	if (odbc.Connect(4, dbpath.c_str()))
	{
		odbc.ExecTableInfo(L"USERLIST");
	}
	//std::wstring sql = L"select * from tblCigar";
	//odbc.ExecSelect(sql.c_str(), 0);
	odbc.Release();
}
//ret = SQLBindCol(m_hStmt, 1, SQL_C_ULONG, &Price, 0, &lPrice);
//TCHAR sql2[MAX_PATH] = { 0, };// L"select szData,price,korean from tblCigar='%s'";
//wsprintf(sql2, L"select price from tblCigar where  szData='%s'", L"장미");
//ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)&sql2, SQL_NTS);

//while (SQLFetch(m_hStmt) != SQL_NO_DATA)
//{
//	std::wcout << Price << std::endl;
//}
//SQLCloseCursor(m_hStmt);
