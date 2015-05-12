//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef WZATOOL_H
#define WZATOOL_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include <windows.h>
#include <time.h>
#include <cstdio>
#include "MD5.h"
#include "../../Protocol/prodef.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

void	BuxConvert				(char* buf, int size);
bool	SQLSyntexCheck			(char* SQLString);
bool	SQLSyntexCheckConvert	(char* SQLString);
bool	SpaceSyntexCheck		(char* string);
DWORD	MakeAccountKey			(LPTSTR lpszAccountID);
bool	ParseHeaderPacket		(LPBYTE Buffer, int * lpSize, BYTE * lpheadcode, BYTE * lpxcode);
// ----
void	PHeadSubSetB			(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void	PHeadSetW				(LPBYTE lpBuf, BYTE head, int size) ;
//-------------------------------------------------------------------------------------------------------------------------------------------

class char_ID
{
public:
	// ----
	char_ID(LPSTR szName)
	{
		memset(this->Name, 0, sizeof(this->Name));
		// ----
		if( szName != NULL )
		{
			memcpy(this->Name, szName, MAX_ACCOUNT_LEN);
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------

	LPSTR GetBuffer()
	{
		return this->Name;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------

	int  GetLength()
	{
		return strlen(this->Name);
	}
private:
	char Name[MAX_ACCOUNT_LEN+1];
};

//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* WZATOOL_H */
//-------------------------------------------------------------------------------------------------------------------------------------------