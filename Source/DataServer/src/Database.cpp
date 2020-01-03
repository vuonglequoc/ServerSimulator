#include "StdAfx.H"
#include "Database.H"
#include "Main.H"


CDataBase gDataBase;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CDataBase::CDataBase()
{
	m_SQLErrorCount		= 1;
	m_AfftedRowCount 	= -1;
	m_Return			= SQL_SUCCESS;
	// -----
	memset(m_szTemp, 0, sizeof(m_szTemp));
	memset(m_szMsg,0 , sizeof(m_szMsg));
	memset(m_SqlState, 0, sizeof(m_SqlState));
	// -----
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, & m_hEnv);
	// -----
	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CDataBase::~CDataBase(void)
{
	if (m_hStmt)	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	// -----
	if (m_hDbc)		SQLDisconnect(m_hDbc);
	// -----
	if (m_hDbc)		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	// -----
	if (m_hEnv)		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CDataBase::Initialize()
{
	bool bReturn = false;
	// -----
	if(this->Connect(gMainApp.MSTR.SQL_Host, gMainApp.MSTR.SQL_DB, gMainApp.MSTR.SQL_User, gMainApp.MSTR.SQL_Pass) == true)
	{
		CLog.Info("SQL Database Connection Successfully Initialized.");
		// -----
		bReturn = true;
	}
	else
	{
		this->DiagnosticConnection();
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CDataBase::Connect(const char * szHost, const char * szDataBase, const char * szUser, const char * szPassword)
{
	bool bReturn		= false;
	int	iRecored		= 1;
	// ----
	SQLSMALLINT cbOutCon					= 0;
	SQLCHAR InCon[255]						= {0};
	SQLCHAR OutCon[255]						= {0};
	SQLWCHAR szSQLState[MAX_DB_TEXT_LEN]	= {0};
	SQLWCHAR szSQLMsg[MAX_DB_TEXT_LEN]		= {0};
	// ----
	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, & m_hDbc);
	// ----
	sprintf((char *)InCon, "DRIVER=SQL Server;Server=%s;DATABASE=%s;UID=%s;PWD=%s;", szHost, szDataBase, szUser, szPassword);
	// ----
	m_Return = SQLDriverConnect(m_hDbc, NULL, InCon, sizeof(InCon), OutCon, sizeof(OutCon), & cbOutCon, SQL_DRIVER_NOPROMPT);
	// ----
	if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
	{
		m_Return = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, & m_hStmt);
		// ----
		if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
		{
			m_strHost = szHost;
			m_strDataBase = szDataBase;
			m_strUser = szUser;
			m_strPassword = szPassword;
			// ----
			bReturn  = true;
		}
		else
		{
			m_hStmt = 0;
		}
	}
	// ----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CDataBase::Connect(const char * szDnsName)
{
	bool bReturn		= false;
	int	iRecored		= 1;
	// ----
	SQLSMALLINT cbOutCon					= 0;
	SQLCHAR InCon[255]						= {0};
	SQLCHAR OutCon[255]						= {0};
	SQLWCHAR szSQLState[MAX_DB_TEXT_LEN]	= {0};
	SQLWCHAR szSQLMsg[MAX_DB_TEXT_LEN]		= {0};
	// ----
	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, & m_hDbc);
	// ----
	sprintf((char *)InCon, "DATABASE=%s;DRIVER=SQL Server;Server=(local);", szDnsName);
	// ----
	CLog.Connect((char *)InCon);
	// ----
	m_Return = SQLDriverConnect(m_hDbc, NULL, InCon, sizeof(InCon), OutCon, sizeof(OutCon), & cbOutCon, SQL_DRIVER_NOPROMPT);
	// ----
	if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
	{
		m_Return = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, & m_hStmt);
		// ----
		if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
		{
			m_strDnsName = szDnsName;
			// ----
			bReturn  = true;
		}
		else
		{
			m_hStmt = 0;
		}
	}
	// ----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CDataBase::Reconnect()
{
	if(m_strDnsName.length() == NULL)
	{
		return Connect(m_strHost.c_str(), m_strDataBase.c_str(), m_strUser.c_str(), m_strPassword.c_str());
	}
	else
	{
		return Connect(m_strDnsName.c_str());
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CDataBase::Exec(const char * szQuery)
{
	bool bReturn = false;
	// ----
	Clear();
	// ----
	m_Return = SQLExecDirect(m_hStmt, (SQLCHAR *)szQuery, SQL_NTS);
	// ----
	CLog.Query(szQuery);
	// ----
	if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO) || (m_Return == SQL_NO_DATA))
	{
		SQLRowCount(m_hStmt, &m_AfftedRowCount);
		SQLNumResultCols(m_hStmt, & m_nCol);
		// ----
		if (m_nCol > MAX_COLUMN)
		{
			CLog.Error("You have exceeded the Maximum No. of Columns");
		}
		else if(m_nCol == 0)
		{
			Clear();
			// ----
			bReturn = true;
		}
		else
		{
			for (int i = 0 ; i < m_nCol ; i++)
			{
				SQLBindCol(m_hStmt, i+1, SQL_C_CHAR, m_Col[i], 255, & m_lCol[i]);
				SQLDescribeCol(m_hStmt, i+1, m_ColName[i], 30, NULL, NULL, NULL, NULL, NULL);
			}
			// ----
			bReturn = true;
		}
	}
	else
	{
		Diagnostic();
	}
	// ----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CDataBase::ExecFormat(const char * szQuery, ...)
{
	va_start	( m_pArgs, szQuery );
	vsprintf	( m_szTemp, szQuery, m_pArgs );
	va_end		( m_pArgs );
	// ----
	return Exec(m_szTemp);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CDataBase::FindCol(const char * szName)
{
	int iReturn = -1;
	// ----
	for (int i = 0 ; i < m_nCol ; i++)
	{
		if (strcmp(szName, (LPCTSTR)m_ColName[i]) == 0)
		{
			iReturn = i;
			// ----
			break;
		}
	}
	// ----
	return iReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CDataBase::GetInt(int nCol)
{
	int iReturn = QUERY_NO_COLUMN;
	// ----
	if (nCol <= m_nCol)
	{
		if (m_lCol[nCol] != SQL_NULL_DATA)
		{
			iReturn = atoi(m_Col[nCol]);
		}
		else
		{
			iReturn = QUERY_NULL;
		}
	}
	// ----
	return iReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CDataBase::GetInt(const char *sCol)
{
	int iReturn = QUERY_NO_COLUMN;
	int n		= -1;
	// ----
	n = FindCol(sCol);
	// ----
	if (n != -1)
	{
		iReturn = GetInt(n);
	}
	// ----
	return iReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
__int64 CDataBase::GetInt64(int nCol)
{
	__int64 iReturn = QUERY_NO_COLUMN;
	// ----
	if (nCol <= m_nCol)
	{
		if (m_lCol[nCol] != SQL_NULL_DATA)
		{
			iReturn = _atoi64(m_Col[nCol]);
		}
		else
		{
			iReturn = QUERY_NULL;
		}
	}
	// ----
	return iReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
__int64 CDataBase::GetInt64(const char *sCol)
{
	__int64 iReturn = QUERY_NO_COLUMN;
	int n		= -1;
	// ----
	n = FindCol(sCol);
	// ----
	if (n != -1)
	{
		iReturn = GetInt64(n);
	}
	// ----
	return iReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
float CDataBase::GetFloat(int nCol)
{
	float fReturn = QUERY_NO_COLUMN;
	// ----
	if (nCol <= m_nCol)
	{ 
		if (m_lCol[nCol] != SQL_NULL_DATA)
		{
			fReturn = (float)atof(m_Col[nCol]);
		}
		else
		{
			fReturn = QUERY_NULL;
		}
	}
	// ----
	return fReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
float CDataBase::GetFloat(const char *sCol)
{
	float fReturn	= QUERY_NO_COLUMN;
	int n			= -1;
	// ----
	n = FindCol(sCol);
	// ----
	if (n != -1)
	{
		fReturn = GetFloat(n);
	}
	// ----
	return fReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CDataBase::GetStr(int nCol, char *buf)
{
	if (nCol > m_nCol)
	{
		buf[0] = '\0';
		return;
	}
	// ----
	if (m_lCol[nCol] == SQL_NULL_DATA) 
	{
		buf[0] = '\0';
	} 
	else 
	{
		lstrcpy(buf, m_Col[nCol]);
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CDataBase::GetStr(const char *sCol, char *buf)
{
	int n = -1;
	// ----
	n = FindCol(sCol);
	// ----
	if (n == -1)
	{
		buf[0] = '\0';
	} 
	else 
	{
		GetStr(n, buf);
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CDataBase::GetAsBinary(LPTSTR lpszStatement, LPBYTE OUT lpszReturnBuffer)
{
	SQLCHAR * pSQLBuf;
	SQLINTEGER BufLen;
	SQLCHAR SQLBinary[10000];
	SQLINTEGER lOfs = 0;
	SQLINTEGER SQLLen;
	SQLRETURN SQLResult;
	// -----
	this->Clear();
	// -----
	CLog.Query(lpszStatement);
	// -----
	SQLResult = SQLExecDirect(m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);
	// -----
	if ( SQLResult != SQL_SUCCESS )
	{
		Diagnostic();
		return -1;
	}
	// -----
	while ( true )
	{
		SQLResult = SQLFetch(m_hStmt);
		// -----
		if ( SQLResult == SQL_NO_DATA ) break;
		// -----
		pSQLBuf = lpszReturnBuffer;
		// -----
		while ( true )
		{
			SQLResult = SQLGetData(m_hStmt, 1, SQL_C_BINARY , SQLBinary, sizeof(SQLBinary), &SQLLen);
			// -----
			if ( SQLResult == SQL_NO_DATA ) break;
			// -----
			if ( SQLLen == -1 )
			{
				Clear();
				// -----
				return FALSE;
			}
			// -----
			if ( SQLResult == SQL_SUCCESS )
			{
				BufLen = SQLLen;
			}
			else
			{
				BufLen = sizeof(SQLBinary);
			}
			// -----
			lOfs	+= BufLen;
			memcpy(pSQLBuf, SQLBinary, BufLen);
			pSQLBuf = &pSQLBuf[lOfs];
		}
	}
	// -----
	return BufLen;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CDataBase::SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, SQLUINTEGER BinaryBufferSize)
{
	SQLINTEGER cbValueSize = -0x64 - BinaryBufferSize;
	SQLPOINTER pToken;
	BYTE cBUFFER[10000];
	SQLRETURN Result;
	int lOfs = 0;
	// -----
	this->Clear();
	// -----
	SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY , SQL_LONGVARBINARY, BinaryBufferSize, 0, (SQLPOINTER)1, 0, &cbValueSize);
	// -----
	CLog.Query(lpszStatement);
	// -----
	SQLExecDirect(m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);
	// -----
	Result = SQLParamData(m_hStmt, &pToken);
	// -----
	while ( Result == SQL_NEED_DATA )
	{
		memcpy(cBUFFER, lpBinaryBuffer, sizeof(cBUFFER));
		// -----
		Result	= SQLPutData(m_hStmt, cBUFFER, BinaryBufferSize);
		Result	= SQLParamData(m_hStmt, &pToken);
		lOfs	+= sizeof(cBUFFER);
	}
	// -----
	SQLParamData(m_hStmt, &pToken);
	// -----
	Clear();
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
