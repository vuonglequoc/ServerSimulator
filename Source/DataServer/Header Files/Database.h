#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "StdAfx.H"
#include "Database.H"
#include "LogProc.H"


class CDataBase
{
public:
	CDataBase();
	virtual ~CDataBase();
	// ----
	bool		Initialize();
	bool		Connect(const char * szHost, const char * szDataBase, const char * szUser, const char * szPassword);
	bool		Connect(const char * szDnsName);
	bool		Reconnect();
	bool		Exec(const char * szQuery);
	bool		ExecFormat(const char * szQuery, ...);
	int			FindCol(const char * szName);
	int			GetInt(int nCol);
	int			GetInt(const char *sCol);
	__int64		GetInt64(int nCol);
	__int64		GetInt64(const char *sCol);
	float		GetFloat(int nCol);
	float		GetFloat(const char *sCol);
	void		GetStr(int nCol, char *buf);
	void		GetStr(const char *sCol, char *buf);
	int			GetAsBinary(LPSTR lpszStatement, LPBYTE OUT lpszReturnBuffer);
	void		SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, SQLUINTEGER BinaryBufferSize);
	// ----
	SQLRETURN	inline Fetch(){ return SQLFetch(m_hStmt); };
	void		inline Clear(){ SQLCloseCursor(m_hStmt) ; SQLFreeStmt(m_hStmt, SQL_UNBIND); };
	// ----
	void Diagnostic()
	{
		if(SQLGetDiagRec(SQL_HANDLE_STMT, m_hStmt, m_SQLErrorCount, m_SqlState, &m_NativeError, m_szMsg, sizeof(m_szMsg), &m_MsgOutLen) != SQL_NO_DATA)
		{
			m_SQLErrorCount++;
			// ----
			memcpy(m_szTemp, m_szMsg , m_MsgOutLen);
			// ----
			CLog.Error(m_szTemp);
		}
		else m_SQLErrorCount = 1;
		// ----
		if( strcmp((LPCTSTR)m_SqlState, "08S01") == 0 )
		{
			Reconnect();
		}
	};
	// ----
	void DiagnosticConnection()
	{
		if(SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, m_SQLErrorCount, m_SqlState, &m_NativeError, m_szMsg, sizeof(m_szMsg), &m_MsgOutLen) != SQL_NO_DATA)
		{
			m_SQLErrorCount++;
			// ----
			memcpy(m_szTemp, m_szMsg , m_MsgOutLen);
			// ----
			CLog.Error(m_szTemp);
		}
		else m_SQLErrorCount = 1;
		// ----
		if( strcmp((LPCTSTR)m_SqlState, "08S01") == 0 )
		{
			Reconnect();
		}
	};
	// -----
private:
	SQLINTEGER 				m_AfftedRowCount;
	SQLRETURN				m_Return;
	// ----
	SQLHENV 				m_hEnv;
	SQLHDBC 				m_hDbc;
	SQLHSTMT				m_hStmt;	
	SQLSMALLINT				m_nCol;
	// ----
	char					m_Col[MAX_COLUMN][255];
	SQLCHAR					m_ColName[MAX_COLUMN][50];
	SQLINTEGER				m_lCol[MAX_COLUMN];
	// ----
	std::string				m_strHost;
	std::string				m_strUser;
	std::string				m_strPassword;
	std::string				m_strDataBase;
	std::string				m_strDnsName;
	// ----
	char					m_szTemp[MAX_DB_TEXT_LEN];
	SQLCHAR					m_szMsg[MAX_DB_TEXT_LEN];
	SQLCHAR					m_SqlState[MAX_DB_TEXT_LEN];
	// ----
	int						m_SQLErrorCount;
	SQLSMALLINT				m_MsgOutLen;
	SQLINTEGER				m_NativeError;
	// ----
	va_list					m_pArgs;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
extern CDataBase gDataBase;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* _DATABASE_H_ */
// -------------------------------------------------------------------------------------------------------------------------------------------------------