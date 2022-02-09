#include "Odbc.h"

bool Odbc::Exec(const TCHAR* sql)
{
	return true;
}
bool Odbc::ExecSelect(const TCHAR* sql, int iTableIndex)
{
	SQLLEN lName, lPrice, lKorean;
	TCHAR Name[21];
	int Price;
	BOOL Korean;

	//SQLExecDirect - sql문의 명령을 직접 실행한다.
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}

	SQLLEN len;
	SQLSMALLINT iNumCols;
	// select문 제외
	SQLRowCount(m_hStmt, &len); // 행의 개수
	SQLNumResultCols(m_hStmt, &iNumCols); // 결과 컬럼 개수 얻음

	//for (int iCols = 1; iCols <= iNumCols; iCols++)
	//{
	//	int iSize = _countof(szColName[iCols]);
	//	SQLDescribeCol(m_hStmt, iCols, 
	//		szColName[iCols], _countof(szColName[iCols]),
	//		&iColNameLength[iCols],
	//		&iColDataType[iCols],
	//		&iColDataSize[iCols], // 데이터형
	//		NULL, // 10진수 자리수
	//		NULL);// NULL 허용여부
	//}

	//// 결과
	//ret = SQLBindCol(m_hStmt, 1, SQL_UNICODE, Name, sizeof(Name), &lName);
	//ret = SQLBindCol(m_hStmt, 2, SQL_C_ULONG, &Price, 0, &lPrice);
	//ret = SQLBindCol(m_hStmt, 3, SQL_C_ULONG, &Korean, 0, &lKorean);


	//while (SQLFetch(m_hStmt) != SQL_NO_DATA)
	//{
	//	std::wcout << Name << " " << Price << " " << Korean << std::endl;
	//}
	SQLCloseCursor(m_hStmt);
	return true;
}
bool Odbc::ExecUpdate(const TCHAR* sql, int iTableIndex)
{
	TCHAR sql4[MAX_PATH] = { 0, };// L"select szData,price,korean from tblCigar='%s'";
	wsprintf(sql4, L"update tblCigar set szData='%s' where szData='%s'",L"코로나", L"88 Light");
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)&sql4, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
	{
		Check();
		return false;
	}
	SQLLEN len;
	SQLSMALLINT Cols;
	// select문 제외
	SQLRowCount(m_hStmt, &len);
	SQLNumResultCols(m_hStmt, &Cols);

	SQLCloseCursor(m_hStmt);
	return true;
}
bool Odbc::ExecDelete(const TCHAR* sql, int iTableIndex)
{
	TCHAR sql3[MAX_PATH] = { 0, };// L"select szData,price,korean from tblCigar='%s'";
	wsprintf(sql3, L"delete from tblCigar where szData='%s'", L"xxxxxx");
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	SQLLEN len;
	SQLSMALLINT Cols;
	// select문 제외
	SQLRowCount(m_hStmt, &len);
	SQLNumResultCols(m_hStmt, &Cols);

	SQLCloseCursor(m_hStmt);
	return true;
}
bool Odbc::ExecInsert(const TCHAR* sql, int iTableIndex)
{
	TCHAR sql3[MAX_PATH] = { 0, };// L"select szData,price,korean from tblCigar='%s'";
	wsprintf(sql3, L"insert into tblCigar (szData,price, korean) values ('%s',%d,%d)", L"디스플러스", 4100, 1);
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	SQLLEN len;
	SQLSMALLINT Cols;
	// select문 제외
	SQLRowCount(m_hStmt, &len);
	SQLNumResultCols(m_hStmt, &Cols);

	SQLCloseCursor(m_hStmt);
	return true;
}
bool Odbc::Init()
{
	//환경 핸들 할당
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
	{
		return false;
	}
	//환경 핸들 속성 지정, 세팅
	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return false;
	}
	//연결 핸들 할당
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc) != SQL_SUCCESS)
	{
		return false;
	}

	return true;
}
void Odbc::Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(m_hEnv, m_hDbc,m_hStmt, szSQLState, &iSQLCode, errorBuffer, sizeof(errorBuffer), &length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}
bool Odbc::Connect(int type, const TCHAR* dsn)
{
	SQLTCHAR OutCon[255];
	SQLSMALLINT cbOutCon;
	TCHAR InCon[256] = { 0, };
	int iSize = sizeof(OutCon);

	SQLSMALLINT cbOutLen;
	SQLRETURN ret;

	switch (type)
	{
	case 0:
	{
		//directx.kr 서버
		_stprintf(InCon, _T("%s"), _T("Driver={SQL Server};Server=directx.kr;Address=211.47.119.43,1433;Network=dbmssocn;Database=KGCAGAME;Uid=sa;Pwd=kgca!@34;"));
		//SQL 드라이브 연결
		ret = SQLDriverConnect(m_hDbc, NULL, InCon, _countof(InCon), OutCon, _countof(OutCon), &cbOutLen, SQL_DRIVER_NOPROMPT);
	}break;
	case 1:
	{
		_stprintf(InCon, _T("Dsn=%s"), dsn);
		//SQL 연결
		//               연결핸들/DB이름       /<-관련    /사용자 ID       /<-관련   /사용자 PW               /<-관련
		ret = SQLConnect(m_hDbc, (SQLTCHAR*)dsn, SQL_NTS, (SQLTCHAR*)L"sa", SQL_NTS, (SQLTCHAR*)L"zhqhfdl147", SQL_NTS);
	}break;
	case 2: 
	{
		_stprintf(InCon, _T("FileDsn=%s"), dsn);
		ret = SQLDriverConnect(m_hDbc, NULL, InCon, _countof(InCon), OutCon, _countof(OutCon), &cbOutLen, SQL_DRIVER_NOPROMPT);
	}break;
	case 3: 
	{
		// access 대화상자 버전
		//GetDesktopWindow - 데스크탑 윈도우 핸들 반환
		HWND hWnd = GetDesktopWindow();
		SQLSMALLINT len;
		ret = SQLDriverConnect(m_hDbc, hWnd,(SQLWCHAR*)L"Driver={Microsoft Access Driver (*.mdb, *.accdb)}", SQL_NTS,(SQLWCHAR*)InCon, _countof(InCon),	&len, SQL_DRIVER_PROMPT);
	}break;
		// SQL Server 대화상자 버전
	case 4: 
	{
		HWND hWnd = GetDesktopWindow();
		SQLSMALLINT len;
		ret = SQLDriverConnect(m_hDbc, hWnd,(SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS, (SQLWCHAR*)InCon, _countof(InCon), &len, SQL_DRIVER_PROMPT);
	}break;
	}
	
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return false;
	}
	//명령 핸들 할당
	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt) != SQL_SUCCESS)
	{
		return false;
	}
	return true;
}
bool Odbc::ExecTableInfo(const TCHAR* szTableName)
{
	TTableInfo table;
	table.szTableName = szTableName;
	std::wstring sql = L"select * from ";
	sql += szTableName;

	//명령문 직접 실행
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}

	SQLSMALLINT iNumCols;
	SQLNumResultCols(m_hStmt, &table.iNumcol);//결과 컬럼 개수 얻음
	for (int iCols = 1; iCols <= table.iNumcol; iCols++)
	{
		TColumnInfo column;
		
		//컬럼 번호 1부터~ (테이블 시작은 0이 아닌 1)
		column.icol = iCols;
		
		//_countof - 배열 개수 구하는 매크로
		int iSize = _countof(column.szColName);
		
		//컬럼에 대한 세부적인 설명 얻음 - SQLDescribeCol
		SQLDescribeCol(m_hStmt, //명령 핸들
			iCols, //데이터 열의 수
			column.szColName, //컬럼 이름
			iSize, //컬럼 이름 버퍼의 길이
			&column.NameLengthPtr,
			&column.pfSqlType,// 데이터 형식을 반환할 버퍼 포인터
			&column.ColumnSizePtr, // 열의 크기를 반환할 버퍼 포인터
			&column.DecimalDigitsPtr, // 10진수 자리수
			&column.pfNullable);// NULL 허용여부
		table.ColumnList.push_back(column);
	}

	SQLLEN lTemp;
	TCHAR szData[100][21] = { 0, };
	int   iData[100];
	TRecord rData;
	for (int iBind = 0; iBind < table.ColumnList.size(); iBind++)
	{
		switch (table.ColumnList[iBind].pfSqlType)
		{
		case SQL_TYPE_TIMESTAMP:
		{
			TField data;
			data.iDataType = SQL_UNICODE;
			ret = SQLBindCol(m_hStmt, iBind + 1,SQL_TYPE_TIMESTAMP, &szData[iBind], 0, &lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		}break;
		case SQL_WCHAR:
		case SQL_WVARCHAR: 
		{
			TField data;
			data.iDataType = SQL_UNICODE;
			ret = SQLBindCol(m_hStmt, iBind + 1, SQL_UNICODE, szData[iBind], sizeof(szData[iBind]), &lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		}break;
		case SQL_INTEGER: 
		{
			TField data;
			data.iDataType = SQL_INTEGER;
			ret = SQLBindCol(m_hStmt, iBind + 1, SQL_INTEGER, &iData[iBind], 0, &lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		}break;
		case -7: 
		{
			TField data;
			data.iDataType = SQL_C_ULONG;
			ret = SQLBindCol(m_hStmt, iBind + 1, SQL_C_ULONG, &iData[iBind], 0, &lTemp);
			if (ret != SQL_SUCCESS)
			{
				Check();
				return false;
			}
			rData.record.push_back(data);
		};
		}
	}
	//다음 행 패치
	while (SQLFetch(m_hStmt) != SQL_NO_DATA)
	{
		for (int iCol = 0; iCol < table.ColumnList.size(); iCol++)
		{
			rData.record[iCol].iDataType = rData.record[iCol].iDataType;
			if (rData.record[iCol].iDataType == SQL_UNICODE)
			{
				rData.record[iCol].szData = szData[iCol];
			}
			else
			{
				rData.record[iCol].szData = std::to_wstring(iData[iCol]);
			}
		}
		//컬럼을 제외한 데이터 정보 저장됨
		m_RealStringData.push_back(rData);
	}
	SQLCloseCursor(m_hStmt);
	m_TableList.push_back(table);
	return true;
}
bool Odbc::Release()
{
	SQLCloseCursor(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLDisconnect(m_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	return true;
}