#include "StdAfx.h"


CFunction	gFunc;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
LPSTR CFunction::FileMissed(int File)
{
	switch (File)
	{
		case 0:
		{
			return "Server.conf";
		} break;
		// -----
		default:
			return "Unknown File";
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CFunction::BuxConvert(LPSTR Buff, int dwSize)
{
	static BYTE bBuxCode[3]	=	{0xFC, 0xCF, 0xAB};
	// -----
	for ( int n=0; n<dwSize; n++)
	{
		Buff[n] ^= bBuxCode[n%3];
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CFunction::CheckSQLSyntex(LPSTR String)
{
	bool bReturn = false;
	// -----
	LPSTR szTemp = strchr(String, 39);
	// -----
	if ( !szTemp )
	{
		szTemp = strchr(String, 32);
		// -----
		if ( !szTemp )
		{
			bReturn = true;
		}
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool CFunction::CheckSpaceSyntex(LPSTR String)
{
	bool bReturn = false;
	// -----
	LPSTR szTemp = strchr(String, 32);
	// -----
	if ( !szTemp )
	{
		bReturn = true;
	}
	// -----
	return bReturn;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD CFunction::CreateAccountKey(LPTSTR lpszAccountID)
{
	DWORD dwKey = 0;
	// -----
	int aLen = strlen(lpszAccountID);
	// -----
	if( aLen <= 0 ) return FALSE;
	// -----
	for ( int i=0; i<aLen; i++)
	{
		dwKey += lpszAccountID[i] + 17;
	}
	// -----
	dwKey = (dwKey + (10 - aLen) * 17) % 256;
	// -----
	return dwKey % 256;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CFunction::PHeadSetW(LPBYTE lpBuf, BYTE head, int Size)
{
	lpBuf[0] = 0xC2;
	lpBuf[1] = SET_NUMBERH(Size);
	lpBuf[2] = SET_NUMBERL(Size);
	lpBuf[3] = head;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CFunction::PHeadSubSetB(LPBYTE lpBuf, BYTE Head, BYTE SubHead, int Size)
{
	lpBuf[0] = 0xC1;
	lpBuf[1] = Size;
	lpBuf[2] = Head;
	lpBuf[3] = SubHead;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
