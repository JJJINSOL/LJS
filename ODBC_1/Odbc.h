#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <vector>
#include <sql.h>
#include <sqlext.h>

struct TColumnInfo
{
	SQLUSMALLINT        icol;
	SQLWCHAR			szColName[20];
	SQLSMALLINT         BufferLength;
	SQLSMALLINT			NameLengthPtr;
	SQLSMALLINT			pfSqlType;
	SQLULEN				ColumnSizePtr;
	SQLSMALLINT			DecimalDigitsPtr;
	SQLSMALLINT			pfNullable;
};
struct TTableInfo
{
	std::vector<TColumnInfo>	ColumnList;
	SQLSMALLINT 				iNumcol;
	std::wstring				szTableName;
};
struct TField
{
	std::wstring	szData;
	int				iDataType;
};
struct TRecord
{
	std::vector<TField>  record;
};
class Odbc
{
public:
	SQLHENV		m_hEnv;//환경
	SQLHDBC		m_hDbc;//연결
	SQLHSTMT	m_hStmt;//명령
	std::vector<TTableInfo> m_TableList;
	std::vector<TRecord> m_RealStringData;
public:
	bool		Init();
	bool		Connect(int type, const TCHAR* dsn);
	bool		Release();
	bool		ExecTableInfo(const TCHAR* szTableName);
	bool		Exec(const TCHAR* sql);
	bool		ExecSelect(const TCHAR* sql, int iTableIndex = 0);
	bool		ExecUpdate(const TCHAR* sql, int iTableIndex = 0);
	bool		ExecDelete(const TCHAR* sql, int iTableIndex = 0);
	bool		ExecInsert(const TCHAR* sql, int iTableIndex = 0);
	void		Check();
};

