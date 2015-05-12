// TMonsterAIMovePath.cpp: implementation of the TMonsterAIMovePath class.
//	GS-N	2.00.05	JPN	0x00561880	-	Completed
//////////////////////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/TMonsterAIMovePath.h"


#include "../Header Files/ReadScript.h"

#include "../Header Files/LogProc.H"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIMovePath::TMonsterAIMovePath()
{
	this->DelAllAIMonsterMovePath();
}



TMonsterAIMovePath::~TMonsterAIMovePath()
{
	return;
}




BOOL TMonsterAIMovePath::LoadData(const char* lpszFileName)
{
	this->m_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		CLog.MsgBox("[Monster AI MovePath] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			CLog.MsgBox("[Monster AI MovePath] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		this->DelAllAIMonsterMovePath();
		int iType = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			iType = (int)TokenNumber;

			while ( true )
			{
				if ( iType == 2 )
				{
					int iSpotType = -1;
					int iMapNumber = -1;
					int iX = -1;
					int iY = -1;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iSpotType = (int)TokenNumber;

					Token = GetToken();
					iMapNumber = (int)TokenNumber;

					Token = GetToken();
					iX = (int)TokenNumber;

					Token = GetToken();
					iY = (int)TokenNumber;

					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iType = iSpotType;
					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iMapNumber = iMapNumber;
					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iPathX = iX;
					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iPathY = iY;

					this->m_iMovePathSpotCount++;

					if ( this->m_iMovePathSpotCount > MAX_MONSTER_AI_MOVE_PATH )
					{
						CLog.MsgBox("[Monster AI MovePath] Exceed Max Move Path-Spot ");
						this->DelAllAIMonsterMovePath();

						return FALSE;
					}
				}
			}
		}

		fclose(SMDFile);

		CLog.LogAddC(TColor.Red(), "[Monster AI MovePath ] - %s file is Loaded", lpszFileName);

		this->m_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		CLog.MsgBox("[Monster AI MovePath] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}



BOOL TMonsterAIMovePath::DelAllAIMonsterMovePath()
{
	for ( int i=0;i<MAX_MONSTER_AI_MOVE_PATH;i++)
	{
		this->m_MovePathInfo[i].Reset();
	}

	this->m_iMovePathSpotCount = 0;
	this->m_bDataLoad = FALSE;

	return FALSE;
}