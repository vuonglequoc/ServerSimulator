//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Gate System Functions & Structures					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/Gate.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/Main.H"
#include "../Header Files/WzMemScript.h"
#include "../Header Files/ReadScript.h"


GCGateSystem GateC;
// --------------------------------------------------------------------------------------------------------------------------------------------
GCGateSystem::GCGateSystem(void)
{
	GateC.InitGate();

	return;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
GCGateSystem::~GCGateSystem(void)
{
	return;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
void GCGateSystem::InitGate()
{
	for(int n=0; n<MAX_GATES; n++)
	{
		GateC.iIndex[n]		 = -1;
		GateC.iType[n]		 = -1;
		GateC.iMapNum[n]	 = -1;
		GateC.iX1[n]		 = -1;
		GateC.iY1[n]		 = -1;
		GateC.iX2[n]		 = -1;
		GateC.iY2[n]		 = -1;
		GateC.iTargetGate[n] = -1;
		GateC.iDir[n]		 = -1;
		GateC.iLevel[n]		 = -1;
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------
void GCGateSystem::LoadGate(const char* filename)
{
	int Token;
	int iIndex;
	int iType		= 0;
	int iMapNum		= 0;
	int iX1			= 0;
	int iY1			= 0;
	int iX2			= 0;
	int iY2			= 0;
	int iTarget		= 0;
	int iDir		= 0;
	int iLevel		= 0;
	int GateReqs	= 0;

	this->InitGate();

	if ( (SMDFile = fopen(filename, "r")) == NULL )
	{
		MessageBox(0,"CMoveManager::LoadGateFile() error","CRITICAL ERROR",0);
		ExitProcess(1);
		return;
	}

	while(true)
	{
		Token = GetToken();

		if(GateReqs == MAX_GATES)
			break;

		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			iIndex = (int)TokenNumber;

			Token = GetToken();
			iType = (int)TokenNumber;

			Token = GetToken();
			iMapNum = (int)TokenNumber;

			Token = GetToken();
			iX1 = (int)TokenNumber;
		
			Token = GetToken();
			iY1 = (int)TokenNumber;

			Token = GetToken();
			iX2 = (int)TokenNumber;

			Token = GetToken();
			iY2 = (int)TokenNumber;

			Token = GetToken();
			iTarget = (int)TokenNumber;

			Token = GetToken();
			iDir = (int)TokenNumber;

			Token = GetToken();
			iLevel = (int)TokenNumber;

			this->InsertGate(iIndex,iType,iMapNum,iX1,iY1,iX2,iY2,iTarget,iDir,iLevel);	
			GateReqs++;
		}
	}

	fclose(SMDFile);
	CLog.LogAddC(TColor.Green(), "[%s] File Information Data Load Complete.", filename);
	return;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
void GCGateSystem::LoadGateB(char* Buffer, int iSize)	//-- Load with Parameters
{
	int Token;
	int Number;

	this->InitGate();

	CWzMemScript WzMemScript;

	WzMemScript.SetBuffer(Buffer, iSize);

	while(true)
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			Number = WzMemScript.GetNumber();
			GateC.iIndex[Number] = Number;

			Token = WzMemScript.GetToken();
			GateC.iType[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iMapNum[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iX1[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iY1[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iX2[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iY2[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iTargetGate[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iDir[Number] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			GateC.iLevel[Number] = WzMemScript.GetNumber();

			Number++;
		}
	}

	CLog.LogAddC(TColor.Orange(), "Gates Information Data Load Complete.");
}
// --------------------------------------------------------------------------------------------------------------------------------------------
void GCGateSystem::InsertGate(int iIndex,int iType,int iMapNum,int iX1,int iY1,int iX2,int iY2,int iTarget,int iDir,int iLevel)
{
	GateC.iIndex[iIndex]		= iIndex;
	GateC.iType [iIndex]		= iType;
	GateC.iMapNum[iIndex]		= iMapNum;
	GateC.iX1[iIndex]			= iX1;
	GateC.iY1[iIndex]			= iY1;
	GateC.iX2[iIndex]			= iX2;
	GateC.iY2[iIndex]			= iY2;
	GateC.iTargetGate[iIndex]	= iTarget;
	GateC.iDir[iIndex]			= iDir;
	GateC.iLevel[iIndex]		= iLevel;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
bool GCGateSystem::IsGate(int GateNumber)
{
	if ( (GateNumber < 0) || (GateNumber > MAX_GATES-1) )
	{
		CLog.LogAddC(TColor.Red(), "Error() Gate :: Information Exceeded [Gate:%d] (%s %d)", GateNumber, __FILE__, __LINE__);
		return false;
	}

	if ( GateC.iIndex[GateNumber] == (BYTE)-1 )
	{
		CLog.LogAddC(TColor.Red(), "Error() Gate :: Information: -1 (%s %d)", __FILE__, __LINE__);
		return false;
	}

	return true;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
int GCGateSystem::GetGate(int GateIndex, BYTE& CordX, BYTE& CordY, BYTE& MapNumber, BYTE& Dir, short& Level)
{
	int iGate;
	int iX;
	int iY;
	int LoopCount;
	BYTE iAttribute;
	
	if ( this->IsGate(GateIndex) == false )
	{
		return -1;
	}

	iGate = GateC.iTargetGate[GateIndex];

	if ( iGate == NULL )
	{
		//-- This For Move Menu!
		iGate = GateIndex;
	}
	
	LoopCount = 10;

	while ( LoopCount-- != 0 )
	{
		//-- This For CordX
		if ( (GateC.iX2[iGate] - GateC.iX1[iGate]) > 0 )
		{
			iX = GateC.iX1[iGate] + ( rand() % ( GateC.iX2[iGate] - GateC.iX1[iGate] ) );	// Choose any X position between XBegin and XEnd
		}
		else
		{
			iX = GateC.iX1[iGate];
		}

		// For Y coords
		if ( (GateC.iY2[iGate] - GateC.iY1[iGate]) > 0 )
		{
			iY = GateC.iY1[iGate] + ( rand() % ( GateC.iY2[iGate] - GateC.iY1[iGate] ) );	// Choose any Y position between YBegin and YEnd
		}
		else
		{
			iY = GateC.iY1[iGate];
		}

		iAttribute = MapC[MapNumber].GetAttr(iX, iY);

		if( (iAttribute&4) != 4 && (iAttribute&2) != 2 && (iAttribute&8) != 8 )
		{
			break;
		}
	}

	CordX		= iX;
	CordY		= iY;
	MapNumber	= GateC.iMapNum[iGate];
	Dir			= GateC.iDir[iGate];
	Level		= GateC.iLevel[iGate];

	return iGate;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
int GCGateSystem::GetLevel(int GateNumber)
{
	if (GateNumber < 0 || GateNumber > MAX_GATES-1 )
	{
		CLog.LogAddC(TColor.Red(), "Error() Gate :: Information Exceeded [Gate:%d] (%s %d)", GateNumber, __FILE__, __LINE__);
		return -1;
	}

	return GateC.iLevel[GateNumber];
}
// --------------------------------------------------------------------------------------------------------------------------------------------
bool GCGateSystem::IsInGate(int aIndex, int GateNumber)
{
	LPOBJ lpObj = &gObj[aIndex];

	if( this->IsGate(GateNumber) == false )
	{
		return false;
	}

	int level = GateC.iLevel[GateNumber];

	if( lpObj->Class == CLASS_DARKLORD  || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_FIGHTER  )
	{
		if ( level > 0 )
		{
			level = level / 3 * 2;
		}
	}

	if( lpObj->Level < level )
	{
		return false;
	}

	if( lpObj->MapNumber != GateC.iMapNum[GateNumber] )
	{
		return false;
	}

	if( lpObj->X < GateC.iX1[GateNumber]-5 || lpObj->X > GateC.iX2[GateNumber]+5 || lpObj->Y < GateC.iY1[GateNumber]-5 || lpObj->Y > GateC.iY2[GateNumber]+5 )
	{
		return false;
	}

	return true;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
bool GCGateSystem::CheckGateLevel(int aIndex, int GateNumber)
{
	LPOBJ lpObj = &gObj[aIndex];

	if( this->IsGate(GateNumber) == false )
	{
		return false;
	}

	int level = GateC.iLevel[GateNumber];

	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_FIGHTER )
	{
		if ( level > 0 )
		{
			level = level / 3 * 2;
		}
	}

	if( lpObj->Level < level )
	{
		return false;
	}

	return true;
}
// --------------------------------------------------------------------------------------------------------------------------------------------