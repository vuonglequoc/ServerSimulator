#include "WZATool.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

static unsigned char bBuxCode[3]	=	{0xFC, 0xCF, 0xAB};	
//-------------------------------------------------------------------------------------------------------------------------------------------

void BuxConvert(char* buf, int size)
{
	int n = 0;
	// ----
	for (n=0;n<size;n++)
	{
		buf[n]^=bBuxCode[n%3];
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool SQLSyntexCheck(char* SQLString)
{
	bool bReturn = false;
	// ----
	char* temp = strchr(SQLString, 39);
	// ----
	if(temp==0)
	{
		temp=strchr(SQLString, 32);
		// ----
		if(temp==0)
		{
			bReturn = true;
		}
		else
		{
			bReturn = false;
		}
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool SQLSyntexCheckConvert(char* SQLString)
{
	bool bReturn = true;
	// ----
	char tempSQL[256]	= {0};
	// ----
	char* temp			= NULL;
	char* temp2			= NULL;
	// ----
	int lOfs			=0;
	int tOfs			=0;
	int sOfs			=0;
	// ----
	temp2				= SQLString;
	// ----
	if(SQLString==0)
	{
		bReturn = false;
	}
	while (true)
	{
		temp = strchr(temp2, 39);
		// ----
		if(temp != 0 )
		{
			if(temp - temp2 != 0)
			{
				memcpy(&tempSQL[tOfs], temp2, temp-temp2);
			}
			lOfs			= temp - (temp2 + sOfs);
			tOfs			+= lOfs;
			temp2			+= lOfs;
			// ----
			tempSQL[tOfs]	=0;
			// ----
			tOfs			+= 2;
			temp2			++;
			// ----
			strcat(tempSQL, "''");
		}
		else
		{
			strcpy(&tempSQL[tOfs], temp2);
			// ----
			break;
		}
	}
	// ----
	strcpy(SQLString, tempSQL);
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool SpaceSyntexCheck(char* string)
{
	bool bReturn = false;
	// ----
	char* temp = strchr(string, 32);
	// ----
	if(temp ==0 )
	{
		bReturn = true;
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void GetTodayString(char* szDate)
{
	tm* today		= {0};
	time_t ltime	= {0};
	// ----
	time(&ltime);
	today=localtime(&ltime);
	// ----
	today->tm_year = today->tm_year +1900;
	// ----
	wsprintf(szDate, "%02d%02d%02d", today->tm_year , today->tm_mon +1 , today->tm_mday );
}
//-------------------------------------------------------------------------------------------------------------------------------------------

DWORD MakeAccountKey(LPTSTR lpszAccountID)
{
	int len = strlen(lpszAccountID);
	// ----
	if( len <= 0 )
	{
		return 0;
	}
	// ----
	DWORD dwKey = 0;
	// ----
	for(int i=0;i<len;i++)
	{
		dwKey += lpszAccountID[i] + 17;
	}
	// ----
	dwKey = (dwKey + (10 - len) * 17) % 256;
	// ----
	return dwKey % 256;
}

//-------------------------------------------------------------------------------------------------------------------------------------------	

void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
{
	lpBuf[0]=0xC1;	// Packets
	lpBuf[1]=size;
	lpBuf[2]=head;
	lpBuf[3]=sub;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void PHeadSetW(LPBYTE lpBuf, BYTE head,  int size) 
{
	lpBuf[0]=0xC2;	// Packets Header
	lpBuf[1]=SET_NUMBERH(size);
	lpBuf[2]=SET_NUMBERL(size);
	lpBuf[3]=head;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool ParseHeaderPacket(LPBYTE Buffer, int * lpSize, BYTE * lpheadcode, BYTE * lpxcode)
{
	bool bReturn = true;
	// ----
	int Size;
	BYTE headcode;
	BYTE xcode;
	// ----
	if((Buffer[0] == 0xC1) || (Buffer[0] == 0xC3))
	{
		Size		= Buffer[1];
		headcode	= Buffer[2];
		xcode		= Buffer[0];
	}
	else if((Buffer[0] == 0xC2) || (Buffer[0] == 0xC4))
	{
		Size		= Buffer[1] * 256;
		Size		|=Buffer[2];
		headcode	= Buffer[3];
		xcode		= Buffer[0];
	}
	else
	{
		bReturn = false;
	}
	// ----
	lpSize		= &Size;
	lpheadcode	= &headcode;
	lpxcode		= &xcode;
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------