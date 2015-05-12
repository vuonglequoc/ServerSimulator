//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Accept IP Structures & Functions						//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/AcceptIp.h"
#include "../Header Files/GameMain.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/ReadScript.h"

CAcceptIp acceptIP;

CAcceptIp::CAcceptIp()
{
	this->Init();
}

CAcceptIp::~CAcceptIp()
{
	return;
}

void CAcceptIp::Init()
{
	for (int n=0; n<MAX_USE_IP;n++)
	{
		memset(this->szIp[n] , 0, sizeof(szIp[n]) );
		this->UseIp[n]= false;
	}
}

BOOL CAcceptIp::IsIp(LPSTR ip)
{
	for(int n=0;n<MAX_USE_IP;n++)
	{
		if ( this->UseIp[n] != false )
		{
			if ( strcmp(this->szIp[n], ip)== 0 )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CAcceptIp::Load(const char *filename)
{
	int count=0;
	int Token;
	
	SMDFile=fopen(filename, "r");

	if (SMDFile == NULL)
	{
		CLog.MsgBox("%s File Not Found!", filename);
		return;
	}

	int n=0;

	while ( true )
	{
		Token=GetToken();

		if (Token == END)
		{
			break;
		}

		if ( Token == NAME )
		{
			strcpy(this->szIp[count] , TokenString);
			this->UseIp[count] = true;
			count++;
		}
	}

	fclose(SMDFile);
	CLog.LogAddC(TColor.BlueViolet(),"IPList.dat Loaded IP: (%s)", szIp);
}