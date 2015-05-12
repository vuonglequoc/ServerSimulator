//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Connect Member Structures & Functions				//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/ConMember.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/ReadScript.h"

CConMember ConMember;

CConMember::CConMember()
{
	return;
}

CConMember::~CConMember()
{
	return;
}


void CConMember::Init()
{
	this->m_szAccount.clear();
}

BOOL CConMember::IsMember(char * AccountID )
{
	if ( (AccountID == NULL ) || ( strcmp(AccountID, "") == 0 ) )
	{
		return FALSE;
	}

	std::map<std::string, int>::iterator it = this->m_szAccount.find( (std::string) AccountID );
	
	if ( it != this->m_szAccount.end() )
	{
		return TRUE;
	}

	return FALSE;
}

void CConMember::Load(const char* filename)
{
	int count=0;
	int Token;

	this->Init();

	SMDFile = fopen( filename, "r");

	if ( SMDFile == 0 )
	{
		CLog.MsgBox( lMsg.Get( MSGGET( 0, 112 ) ), filename );
		return;
	}

	int n = 0;

	while ( true ) 
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 0 )
			{
				this->m_szAccount.insert( std::pair<std::string, int>( (std::string)TokenString, 0 ) );
			}
		}
	}

	fclose( SMDFile );
}