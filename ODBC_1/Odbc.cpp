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

	//SQLExecDirect - sql���� ����� ���� �����Ѵ�.
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}

	SQLLEN len;
	SQLSMALLINT iNumCols;
	// select�� ����
	SQLRowCount(m_hStmt, &len); // ���� ����
	SQLNumResultCols(m_hStmt, &iNumCols); // ��� �÷� ���� ����

	//for (int iCols = 1; iCols <= iNumCols; iCols++)
	//{
	//	int iSize = _countof(szColName[iCols]);
	//	SQLDescribeCol(m_hStmt, iCols, 
	//		szColName[iCols], _countof(szColName[iCols]),
	//		&iColNameLength[iCols],
	//		&iColDataType[iCols],
	//		&iColDataSize[iCols], // ��������
	//		NULL, // 10���� �ڸ���
	//		NULL);// NULL ��뿩��
	//}

	//// ���
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
	wsprintf(sql4, L"update tblCigar set szData='%s' where szData='%s'",L"�ڷγ�", L"88 Light");
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)&sql4, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_NO_DATA)
	{
		Check();
		return false;
	}
	SQLLEN len;
	SQLSMALLINT Cols;
	// select�� ����
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
	// select�� ����
	SQLRowCount(m_hStmt, &len);
	SQLNumResultCols(m_hStmt, &Cols);

	SQLCloseCursor(m_hStmt);
	return true;
}
bool Odbc::ExecInsert(const TCHAR* sql, int iTableIndex)
{
	TCHAR sql3[MAX_PATH] = { 0, };// L"select szData,price,korean from tblCigar='%s'";
	wsprintf(sql3, L"insert into tblCigar (szData,price, korean) values ('%s',%d,%d)", L"���÷���", 4100, 1);
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}
	SQLLEN len;
	SQLSMALLINT Cols;
	// select�� ����
	SQLRowCount(m_hStmt, &len);
	SQLNumResultCols(m_hStmt, &Cols);

	SQLCloseCursor(m_hStmt);
	return true;
}
bool Odbc::Init()
{
	//ȯ�� �ڵ� �Ҵ�
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
	{
		return false;
	}
	//ȯ�� �ڵ� �Ӽ� ����, ����
	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return false;
	}
	//���� �ڵ� �Ҵ�
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
		//directx.kr ����
		_stprintf(InCon, _T("%s"), _T("Driver={SQL Server};Server=directx.kr;Address=211.47.119.43,1433;Network=dbmssocn;Database=KGCAGAME;Uid=sa;Pwd=kgca!@34;"));
		//SQL ����̺� ����
		ret = SQLDriverConnect(m_hDbc, NULL, InCon, _countof(InCon), OutCon, _countof(OutCon), &cbOutLen, SQL_DRIVER_NOPROMPT);
	}break;
	case 1:
	{
		_stprintf(InCon, _T("Dsn=%s"), dsn);
		//SQL ����
		//               �����ڵ�/DB�̸�       /<-����    /����� ID       /<-����   /����� PW               /<-����
		ret = SQLConnect(m_hDbc, (SQLTCHAR*)dsn, SQL_NTS, (SQLTCHAR*)L"sa", SQL_NTS, (SQLTCHAR*)L"zhqhfdl147", SQL_NTS);
	}break;
	case 2: 
	{
		_stprintf(InCon, _T("FileDsn=%s"), dsn);
		ret = SQLDriverConnect(m_hDbc, NULL, InCon, _countof(InCon), OutCon, _countof(OutCon), &cbOutLen, SQL_DRIVER_NOPROMPT);
	}break;
	case 3: 
	{
		// access ��ȭ���� ����
		//GetDesktopWindow - ����ũž ������ �ڵ� ��ȯ
		HWND hWnd = GetDesktopWindow();
		SQLSMALLINT len;
		ret = SQLDriverConnect(m_hDbc, hWnd,(SQLWCHAR*)L"Driver={Microsoft Access Driver (*.mdb, *.accdb)}", SQL_NTS,(SQLWCHAR*)InCon, _countof(InCon),	&len, SQL_DRIVER_PROMPT);
	}break;
		// SQL Server ��ȭ���� ����
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
	//��� �ڵ� �Ҵ�
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

	//��ɹ� ���� ����
	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return false;
	}

	SQLSMALLINT iNumCols;
	SQLNumResultCols(m_hStmt, &table.iNumcol);//��� �÷� ���� ����
	for (int iCols = 1; iCols <= table.iNumcol; iCols++)
	{
		TColumnInfo column;
		
		//�÷� ��ȣ 1����~ (���̺� ������ 0�� �ƴ� 1)
		column.icol = iCols;
		
		//_countof - �迭 ���� ���ϴ� ��ũ��
		int iSize = _countof(column.szColName);
		
		//�÷��� ���� �������� ���� ���� - SQLDescribeCol
		SQLDescribeCol(m_hStmt, //��� �ڵ�
			iCols, //������ ���� ��
			column.szColName, //�÷� �̸�
			iSize, //�÷� �̸� ������ ����
			&column.NameLengthPtr,
			&column.pfSqlType,// ������ ������ ��ȯ�� ���� ������
			&column.ColumnSizePtr, // ���� ũ�⸦ ��ȯ�� ���� ������
			&column.DecimalDigitsPtr, // 10���� �ڸ���
			&column.pfNullable);// NULL ��뿩��
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
	//���� �� ��ġ
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
		//�÷��� ������ ������ ���� �����
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