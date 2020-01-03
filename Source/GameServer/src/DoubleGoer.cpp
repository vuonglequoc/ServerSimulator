//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Double Goer Event
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#include "stdafx.h"

#if (GS_CASTLE==0)

#include "DoubleGoer.h"
#include "Defines.h"
#include "Notice.H"
#include "LogProc.H"
#include "Functions.h"
#include "ReadScript.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ObjUseSkill.h"
#include "MoveSystem.h"

#include "SendProto.H"

static const struct DOUBLE_ZONE
{
	unsigned char btStartX;
	unsigned char btStartY;
	unsigned char btEndX;
	unsigned char btEndY;

} DoubleGoerEntranceMap[4] = {

	195, 33, 199, 36,	// Double Goer 65
	135, 76, 140, 80,	// Double Goer 66
	106, 66, 111, 69,	// Double Goer 67
	93, 23, 97, 25	// Double Goer 68
}, DoublePortalCoords[4] = {
	194, 25, 200, 30,	// Double Goer 65
	134, 67, 141, 73,	// Double Goer 66
	107, 56, 113, 62,	// Double Goer 67
	92, 11, 98, 18	// Double Goer 68
};

CDoubleGoer gDoubleGoer;
// ----------------------------------------------------------------------------------------
CDoubleGoer::CDoubleGoer(void)
{
	return;
}
// ----------------------------------------------------------------------------------------
CDoubleGoer::~CDoubleGoer(void)
{
	return;
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::Init()
{
	gDoubleGoer.Enabled						= GetPrivateProfileInt(DOUBLEGR_HDR,"Enabled",0,DOUBLEGR_FILE_PATH);

	if ( gDoubleGoer.Enabled == 1 )
	{
		gDoubleGoer.OriginalTimer				= GetPrivateProfileInt(DOUBLEGR_HDR,"Time",0,DOUBLEGR_FILE_PATH);
		gDoubleGoer.OriginalStandByTimer		= GetPrivateProfileInt(DOUBLEGR_HDR,"StandByTime",0,DOUBLEGR_FILE_PATH);
		gDoubleGoer.LarvaRate					= GetPrivateProfileInt(DOUBLEGR_HDR,"LarvaRate",0,DOUBLEGR_FILE_PATH);

		gDoubleGoer.BlockDoubleGoerEntrance(65);
		gDoubleGoer.BlockDoubleGoerEntrance(66);
		gDoubleGoer.BlockDoubleGoerEntrance(67);
		gDoubleGoer.BlockDoubleGoerEntrance(68);

		gDoubleGoer.StandByTimer			= gDoubleGoer.OriginalStandByTimer;
		gDoubleGoer.Timer					= gDoubleGoer.OriginalTimer*60;
		gDoubleGoer.Running					= 0;
		gDoubleGoer.UserCount				= 0;
		gDoubleGoer.MonstersAddCount		= 0;
		gDoubleGoer.PartyActived			= false;
		gDoubleGoer.UserIndexMaxHit[0]		= 0;
		gDoubleGoer.UserIndexMaxHit[1]		= 0;
		gDoubleGoer.UserIndexMaxHit[2]		= 0;
		gDoubleGoer.Success					= 0;
		gDoubleGoer.MobNearY				= 255;
		gDoubleGoer.MonsterPos				= 0;
		gDoubleGoer.MonsterPosIndex			= -1;
		gDoubleGoer.GoldenChestIndex		= -1;

		for (int i=0;i<255;i++)
		{
			gDoubleGoer.MonstersAddIndex[i]	= -1;
			gDoubleGoer.NormalMonsterPos[0][i] = -1;
			gDoubleGoer.NormalMonsterPos[1][i] = -1;
			gDoubleGoer.NormalMonsterPos[2][i] = -1;
			gDoubleGoer.NormalMonsterPos[3][i] = -1;
		}

		for (int i=0;i<50;i++)
		{
			gDoubleGoer.LarvaIndex[0][i] = -1;
			gDoubleGoer.LarvaIndex[1][i] = -1;
			gDoubleGoer.LarvaIndex[2][i] = -1;
		}

		gDoubleGoer.LarvaCount[0] = 0;
		gDoubleGoer.LarvaCount[1] = 0;
		gDoubleGoer.LarvaCount[2] = 0;

		gDoubleGoer.NormalMonsterCount[0] = 0;
		gDoubleGoer.NormalMonsterCount[1] = 0;
		gDoubleGoer.NormalMonsterCount[2] = 0;
		gDoubleGoer.NormalMonsterCount[3] = 0;

		gDoubleGoer.BossMonsterPos[0][0] = -1;
		gDoubleGoer.BossMonsterPos[1][0] = -1;
		gDoubleGoer.BossMonsterPos[2][0] = -1;
		gDoubleGoer.BossMonsterPos[3][0] = -1;

		gDoubleGoer.BossMonsterPos[0][1] = -1;
		gDoubleGoer.BossMonsterPos[1][1] = -1;
		gDoubleGoer.BossMonsterPos[2][1] = -1;
		gDoubleGoer.BossMonsterPos[3][1] = -1;

		gDoubleGoer.BossMonsterPos[0][2] = -1;
		gDoubleGoer.BossMonsterPos[1][2] = -1;
		gDoubleGoer.BossMonsterPos[2][2] = -1;
		gDoubleGoer.BossMonsterPos[3][2] = -1;
	
		gDoubleGoer.MonsterCount			= 0;
		gDoubleGoer.EventMap				= 0;
		gDoubleGoer.EventX					= 0;
		gDoubleGoer.EventY					= 0;
		gDoubleGoer.StandBy					= 0;

		gDoubleGoer.ChestIndex[0][0]		= -1;
		gDoubleGoer.ChestIndex[0][1]		= -1;
		gDoubleGoer.ChestIndex[0][2]		= -1;
		gDoubleGoer.ChestIndex[1][0]		= -1;
		gDoubleGoer.ChestIndex[1][1]		= -1;
		gDoubleGoer.ChestIndex[1][2]		= -1;
		gDoubleGoer.ChestIndex[2][0]		= -1;
		gDoubleGoer.ChestIndex[2][1]		= -1;
		gDoubleGoer.ChestIndex[2][2]		= -1;

		for (int i=0;i<5;i++)
		{
			gDoubleGoer.UserIndex[i]		= 0;
		}

		for (int n=0;n<gMSetBase.m_Count;n++)
		{
			if ( DG_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) == true )
			{
				char Type = gDoubleGoer.GetEventIndex(gMSetBase.m_Mp[n].m_MapNumber);

				if ( Type >= 0 && Type <= 3 )
				{
					if ( gMSetBase.m_Mp[n].m_Type >= 533 && gMSetBase.m_Mp[n].m_Type <= 539 )
					{
						gDoubleGoer.NormalMonsterPos[Type][gDoubleGoer.NormalMonsterCount[Type]] = n;
						gDoubleGoer.NormalMonsterCount[Type]++;
					}
					else if ( gMSetBase.m_Mp[n].m_Type == 530 )
					{
						gDoubleGoer.BossMonsterPos[Type][0] = n;
					}
					else if ( gMSetBase.m_Mp[n].m_Type == 529 )
					{
						gDoubleGoer.BossMonsterPos[Type][1] = n;
					}
					else if ( gMSetBase.m_Mp[n].m_Type == 531 )
					{
						gDoubleGoer.BossMonsterPos[Type][2] = n;
					}
				}
				else
				{
					CLog.LogAdd("[BUG TRACER][DoubleGoer]: Init -> Type Error [%d]",Type);
				}
			}
		}

		gDoubleGoer.ReadFile();

		CLog.LogAdd("[DoubleGoer]: Init -> Enabled");
	}
	else
	{
		CLog.LogAdd("[DoubleGoer]: Init -> Disabled");
	}
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::ReadFile()
{
	SMDFile = fopen(DOUBLEGR_DROP_FILE, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[BUG TRACER][DoubleGoer]: Read File -> Info File Load Fail [%s]", DOUBLEGR_DROP_FILE);
		return;
	}

	int Token;
	int type = -1;

	while ( true )
	{
	    Token = GetToken();

        if( Token == 2)
		{
            break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type < 0 || type > 7 )
			{
				break;
			}
		    Token = GetToken();

			if ( strcmp("end", TokenString ) == 0)
			{
				break;
			}

			gDoubleGoer.Type[type][ItemCount[type]] = TokenNumber;
				
			Token = GetToken();
			gDoubleGoer.Index[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Level[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Dur[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Skill[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Luck[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Opt[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Exe[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Anc[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Socket[0][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Socket[1][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Socket[2][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Socket[3][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gDoubleGoer.Socket[4][type][ItemCount[type]] = TokenNumber;

			gDoubleGoer.ItemCount[type]++;
		        
		}
    }

	fclose(SMDFile);
	

	SMDFile = fopen(DOUBLEGR_CFG_FILE, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[BUG TRACER][DoubleGoer]: Read File -> Info File Load Fail [%s]", DOUBLEGR_DROP_FILE);
		return;
	}

	type = -1;
	int AuxCounter = 0;

	while ( true )
	{
	    Token = GetToken();

		if( Token == 2)
		{
            break;
		}

		type = TokenNumber;

		AuxCounter = 0;

		if ( type == 0 )
		{
			while ( true )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString ) == 0)
				{
					break;
				}

				gDoubleGoer.PlayerMinLvl[gDoubleGoer.MobCfgIndex] = TokenNumber;
				
				Token = GetToken();
				gDoubleGoer.PlayerMaxLvl[gDoubleGoer.MobCfgIndex] = TokenNumber;
			}
		}
		else if ( type == 1 )
		{
			while ( true )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString ) == 0)
				{
					break;
				}

				gDoubleGoer.MobID[gDoubleGoer.MobCfgIndex][AuxCounter] = TokenNumber;

				Token = GetToken();
				gDoubleGoer.MobLvl[gDoubleGoer.MobCfgIndex][AuxCounter] = TokenNumber;

				Token = GetToken();
				gDoubleGoer.MobHP[gDoubleGoer.MobCfgIndex][AuxCounter] = TokenNumber;

				Token = GetToken();
				gDoubleGoer.MobMana[gDoubleGoer.MobCfgIndex][AuxCounter] = TokenNumber;

				Token = GetToken();
				gDoubleGoer.MobMinDmg[gDoubleGoer.MobCfgIndex][AuxCounter] = TokenNumber;

				Token = GetToken();
				gDoubleGoer.MobMaxDmg[gDoubleGoer.MobCfgIndex][AuxCounter] = TokenNumber;

				Token = GetToken();
				gDoubleGoer.MobDef[gDoubleGoer.MobCfgIndex][AuxCounter] = TokenNumber;

				AuxCounter++;
			}
		}
		else
		{
			CLog.LogAdd("[BUG TRACER][DoubleGoer]: Read File -> Type Error %d",type);
		}
		
		gDoubleGoer.MobCfgIndex++;
    }

	fclose(SMDFile);
	return;
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::Run()
{
	if ( gDoubleGoer.Enabled == 0 )
	{
		return;
	}
	// ----
	if ( gDoubleGoer.Running == 0 )
	{
		return;
	}
	// ----
	for (int i=0;i<5;i++)
	{
		if ( gDoubleGoer.UserIndex[i] != 0 )
		{
			LPOBJ lpObj = &gObj[gDoubleGoer.UserIndex[i]];
			if ( gObjIsConnected(lpObj) == false )
			{
				gDoubleGoer.UserIndex[i] = 0;
				gDoubleGoer.UserCount--;
			}
			// ----
			else if ( gObjIsConnected(lpObj) == true && lpObj->MapNumber != gDoubleGoer.EventMap && gDoubleGoer.StandBy == 0 )
			{
				if ( gDoubleGoer.Success == 1 )
				{
					gDoubleGoer.UserIndex[i] = 0;
					gDoubleGoer.UserCount--;
				}
				else
				{
					gDoubleGoer.EventEnd(gDoubleGoer.UserIndex[i],0x01);
					gDoubleGoer.UserIndex[i] = 0;
					gDoubleGoer.UserCount--;
				}
			}
			else if ( gDoubleGoer.PartyActived == true && lpObj->PartyNumber == -1 )
			{
				gObjMoveGate(gDoubleGoer.UserIndex[i],267);
				gDoubleGoer.UserIndex[i] = 0;
				gDoubleGoer.UserCount--;
				if ( gDoubleGoer.Success == 0 )
				{
					gDoubleGoer.EventEnd(gDoubleGoer.UserIndex[i],0x01);
				}
			}
		}
	}
	if ( gDoubleGoer.Running == 1 && gDoubleGoer.UserCount == 0 )
	{
		gDoubleGoer.Clear();
		return;
	}
	// ----
	
	for(int n=0;n<23;n++)
	{
		for (int i=0;i<5;i++)
		{
			if ( gDoubleGoer.UserIndex[i] != 0 )
			{
				if ( (gObj[gDoubleGoer.UserIndex[i]].Y > gDoubleGoer.BossStartY-(n*gDoubleGoer.CalcDistanceY) ) && 
					 (gObj[gDoubleGoer.UserIndex[i]].Y <= (gDoubleGoer.BossStartY+gDoubleGoer.CalcDistanceY)-(n*gDoubleGoer.CalcDistanceY) ) )
				{
					gDoubleGoer.UserPos[i] = n;
				}
			}
		}
		if ( gDoubleGoer.BossMonsterIndex[2] != -1 )
		{
			if ( (gObj[gDoubleGoer.BossMonsterIndex[2]].Y >= gDoubleGoer.BossStartY-(n*gDoubleGoer.CalcDistanceY) ) &&
				 (gObj[gDoubleGoer.BossMonsterIndex[2]].Y < (gDoubleGoer.BossStartY+gDoubleGoer.CalcDistanceY)-(n*gDoubleGoer.CalcDistanceY) ) )
			{
				gDoubleGoer.BossMonsterPoss = n;
			}
		}
		else
		{
			gDoubleGoer.BossMonsterPoss = 0;
		}
		for (int i=0;i<255;i++)
		{
			if ( gDoubleGoer.MonstersAddIndex[i] != -1 )
			{
				if ( (gObj[gDoubleGoer.MonstersAddIndex[i]].Y <= ::DoublePortalCoords[gDoubleGoer.EventMap-65].btEndY && gObj[gDoubleGoer.MonstersAddIndex[i]].Y >= ::DoublePortalCoords[gDoubleGoer.EventMap-65].btStartY) && 
					 (gObj[gDoubleGoer.MonstersAddIndex[i]].X <= ::DoublePortalCoords[gDoubleGoer.EventMap-65].btEndX && gObj[gDoubleGoer.MonstersAddIndex[i]].X >= ::DoublePortalCoords[gDoubleGoer.EventMap-65].btStartX))
				{
					gDoubleGoer.MonsterCount++;
					gObjDel(gDoubleGoer.MonstersAddIndex[i]);
					gDoubleGoer.MonstersAddIndex[i] = -1;
					continue;
				}
				// ----
				if ( gObj[gDoubleGoer.MonstersAddIndex[i]].Life > 0.0f )
				{
					if ( ( gObj[gDoubleGoer.MonstersAddIndex[i]].Y > gDoubleGoer.BossStartY-(n*gDoubleGoer.CalcDistanceY) ) &&
						(gObj[gDoubleGoer.MonstersAddIndex[i]].Y <= (gDoubleGoer.BossStartY+gDoubleGoer.CalcDistanceY)-(n*gDoubleGoer.CalcDistanceY) ) )
					{

					}
				}
				else if ( gDoubleGoer.MonstersAddIndex[i] == gDoubleGoer.MonsterPosIndex )
				{
					
				}
			}
		}
	}
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::CheckCanEnter(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}
	if ( gDoubleGoer.Enabled == 0 )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[DoubleGoer]: Event is disabled");
		return;
	}
	if ( gDoubleGoer.Running == 1 )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[DoubleGoer]: Event is running, please try later");
		return;
	}
	if ( gObjGetItemCountInIventory(aIndex,14,111,0) < 1 )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[DoubleGoer]: Need Mirror of Dimensions to enter");
		return;
	}
	gObjDelteItemCountInInventory(aIndex,14,111,1);
	
	gDoubleGoer.Teleport(aIndex);
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::MapCheck()
{
	unsigned char Room = (rand()%4)+65;
	// ----
	if ( Room == 65 )
	{
		gDoubleGoer.EventX = 197;
		gDoubleGoer.EventY = 27;
		gDoubleGoer.EventMap = 65;
		gDoubleGoer.BossStartX = 225;
		gDoubleGoer.BossStartY = 101;
		gDoubleGoer.CalcDistanceY = 3;
	}
	else if ( Room == 66 )
	{
		gDoubleGoer.EventX = 138;
		gDoubleGoer.EventY = 70;
		gDoubleGoer.EventMap = 66;
		gDoubleGoer.BossStartX = 114;
		gDoubleGoer.BossStartY = 180;
		gDoubleGoer.CalcDistanceY = 5;
	}
	else if ( Room == 67 )
	{
		gDoubleGoer.EventX = 110;
		gDoubleGoer.EventY = 58;
		gDoubleGoer.EventMap = 67;
		gDoubleGoer.BossStartX = 110;
		gDoubleGoer.BossStartY = 150;
		gDoubleGoer.CalcDistanceY = 4;
	}
	else if ( Room == 68 )
	{
		gDoubleGoer.EventX = 95;
		gDoubleGoer.EventY = 15;
		gDoubleGoer.EventMap = 68;
		gDoubleGoer.BossStartX = 43;
		gDoubleGoer.BossStartY = 109;
		gDoubleGoer.CalcDistanceY = 4;
	}
	else
	{
		gDoubleGoer.EventX = 197;
		gDoubleGoer.EventY = 27;
		gDoubleGoer.EventMap = 65;
		gDoubleGoer.BossStartX = 225;
		gDoubleGoer.BossStartY = 101;
		gDoubleGoer.CalcDistanceY = 3;
	}
	gDoubleGoer.DistanceY = gDoubleGoer.BossStartY - gDoubleGoer.EventY;

	CLog.LogAdd("[DoubleGoer]: Map Check -> Random Map: %d",gDoubleGoer.EventMap);
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::Teleport(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	gDoubleGoer.MapCheck();
	// ----
	if ( lpObj->PartyNumber == -1 )
	{
		gDoubleGoer.PartyActived = false;
		gDoubleGoer.UserCount = 1;
		gDoubleGoer.UserIndex[0] = aIndex;
		gObjTeleport(aIndex,gDoubleGoer.EventMap,gDoubleGoer.EventX,gDoubleGoer.EventY);
		SendDoubleGoerEntranceBlock(gDoubleGoer.EventMap,aIndex,1);
		// ----
		SendMsg.MessageOut(aIndex,  0x01, "[DoubleGoer]: Event will start in %d seconds",gDoubleGoer.StandByTimer);
	}
	else
	{
		for (int i=OBJMIN;i<OBJMAX+1;i++)
		{
			// ----
			if ( gObj[i].Type != OBJ_USER )
			{
				continue;
			}
			if ( gObj[i].PartyNumber == lpObj->PartyNumber )
			{
				gDoubleGoer.UserIndex[gDoubleGoer.UserCount] = i;
				gDoubleGoer.UserCount++;
				gObjTeleport(i,gDoubleGoer.EventMap,gDoubleGoer.EventX,gDoubleGoer.EventY);
				// ----
				SendMsg.MessageOut(i,  0x01, "[DoubleGoer]: Event will start in %d seconds",gDoubleGoer.StandByTimer);
				SendDoubleGoerEntranceBlock(gDoubleGoer.EventMap,i,1);
			}
		}
		gDoubleGoer.PartyActived = true;
		SendMsg.MessageOut(aIndex,  0x01, "[DoubleGoer]: Total User Count %d",gDoubleGoer.UserCount);
	}
	for (int i=0;i<5;i++)
	{
		if ( gDoubleGoer.UserIndex[i] != 0 )
		{
			if ( (gObj[gDoubleGoer.UserIndex[i]].Level+gObj[gDoubleGoer.UserIndex[i]].MasterLevel) > gDoubleGoer.EventMaxLevel )
			{
				gDoubleGoer.EventMaxLevel = gObj[gDoubleGoer.UserIndex[i]].Level+gObj[gDoubleGoer.UserIndex[i]].MasterLevel;
			}
		}
	}

	for (int i=0;i<50;i++)
	{
		if ( (gDoubleGoer.EventMaxLevel >= gDoubleGoer.PlayerMinLvl[i]) && (gDoubleGoer.EventMaxLevel <= gDoubleGoer.PlayerMaxLvl[i]) )
		{
			gDoubleGoer.EventLevel = i;
			break;
		}
	}

	CLog.LogAdd("[DoubleGoer]: Teleport -> User Count: %d, Max Event Level: %d",gDoubleGoer.UserCount, gDoubleGoer.EventMaxLevel);
	gDoubleGoer.Running = 1;
	gDoubleGoer.StandBy = 1;
	_beginthread( StandByStart, 0, NULL  );
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::InitTimer()
{
	for (int i=0;i<5;i++)
	{
		if ( gDoubleGoer.UserIndex[i] != 0 )
		{
			gDoubleGoer.UserPos[i] = 22;
		}
	}
	//Update Bar(Triangles)
	PMSG_DGOER_UPDATE_BAR pMsg0;
	pMsg0.c = 0xC1;
	pMsg0.size = 0x1C;
	pMsg0.headcode = 0xBF;
	pMsg0.subcode = 0x12;
	pMsg0.TimerL = LOBYTE(gDoubleGoer.Timer);
	pMsg0.TimerH = HIBYTE(gDoubleGoer.Timer);
	pMsg0.UserCount = gDoubleGoer.UserCount;
	pMsg0.Unk = 0;
	pMsg0.User1H = HIBYTE(gDoubleGoer.UserIndex[0]);
	pMsg0.User1L = LOBYTE(gDoubleGoer.UserIndex[0]);
	pMsg0.Unk1 = 0;
	pMsg0.Pos1 = gDoubleGoer.UserPos[0];
	pMsg0.User2H = HIBYTE(gDoubleGoer.UserIndex[1]);
	pMsg0.User2L = LOBYTE(gDoubleGoer.UserIndex[1]);
	pMsg0.Unk2 = 0;
	pMsg0.Pos2 = gDoubleGoer.UserPos[0];
	pMsg0.User3H = HIBYTE(gDoubleGoer.UserIndex[2]);
	pMsg0.User3L = LOBYTE(gDoubleGoer.UserIndex[2]);
	pMsg0.Unk3 = 0;
	pMsg0.Pos3 = gDoubleGoer.UserPos[0];
	pMsg0.User4H = HIBYTE(gDoubleGoer.UserIndex[3]);
	pMsg0.User4L = LOBYTE(gDoubleGoer.UserIndex[3]);
	pMsg0.Unk4 = 0;
	pMsg0.Pos4 = gDoubleGoer.UserPos[0];
	pMsg0.User5H = HIBYTE(gDoubleGoer.UserIndex[4]);
	pMsg0.User5L = LOBYTE(gDoubleGoer.UserIndex[4]);
	pMsg0.Unk5 = 0;
	pMsg0.Pos5 = gDoubleGoer.UserPos[0];

	//Init Event Timer
	PMSG_DGOER_TIMER_INIT pMsg1;
	pMsg1.c = 0xC1;
	pMsg1.size = 0x05;
	pMsg1.headcode = 0xBF;
	pMsg1.subcode = 0x10;
	pMsg1.Timer = 2;

	//Double Goer Set Line Position  
	PMSG_DGOER_TRIANG_POS pMsg2;
	pMsg2.c = 0xC1;
	pMsg2.size = 0x05;
	pMsg2.headcode = 0xBF;
	pMsg2.subcode = 0x0F;
	pMsg2.Pos = 0x00;

	//Set Monster Kill Count
	PMSG_DGOER_MONSTER_COUNT pMsg3;
	pMsg3.c = 0xC1;
	pMsg3.size = 0x06;
	pMsg3.headcode = 0xBF;
	pMsg3.subcode = 0x14;
	pMsg3.Total = 3;
	pMsg3.Killed = gDoubleGoer.MonsterCount;

	//Double Goer Set Line Position  
	PMSG_DGOER_RTRIANG_POS pMsg4;
	pMsg4.c = 0xC1;
	pMsg4.size = 0x06;
	pMsg4.headcode = 0xBF;
	pMsg4.subcode = 0x11;
	pMsg4.Unk = 0;
	pMsg4.Pos = gDoubleGoer.BossMonsterPoss;

	for(int i=0;i<5;i++)
	{
		if( gDoubleGoer.UserIndex[i] != 0 )
		{
			gSendProto.DataSend(gDoubleGoer.UserIndex[i], (BYTE*)&pMsg0, pMsg0.size);
			gSendProto.DataSend(gDoubleGoer.UserIndex[i], (BYTE*)&pMsg1, pMsg1.size);
			gSendProto.DataSend(gDoubleGoer.UserIndex[i], (BYTE*)&pMsg2, pMsg2.size);
			gSendProto.DataSend(gDoubleGoer.UserIndex[i], (BYTE*)&pMsg3, pMsg3.size);
			gSendProto.DataSend(gDoubleGoer.UserIndex[i], (BYTE*)&pMsg4, pMsg4.size);
		}
	}
	gDoubleGoer.ReleaseDoubleGoerEntrance(gDoubleGoer.EventMap);
	// ----
	for ( int i=0;i<5;i++)
	{
		if ( gDoubleGoer.UserIndex[i] != 0 )
		{
			gDoubleGoer.SendDoubleGoerEntranceBlock(gDoubleGoer.EventMap,gDoubleGoer.UserIndex[i],0);
		}
	}
	// ---- SET MONSTERS FOR MAP
	gDoubleGoer.SetMonsters();
	gDoubleGoer.BossStage = 0;
	CLog.LogAdd("[DoubleGoer]: Init Timer -> Event Run");
	// ---- Begin Event
	_beginthread( EventRun, 0, NULL  );
}
// ----------------------------------------------------------------------------------------
void EventRun(void * lpParam)
{
	// ----
	while ( true )
	{
		if ( gDoubleGoer.Timer < 1 )	// Time Out
		{
			gDoubleGoer.Timer = 0;
			// ----
			for (int i=0;i<5;i++)
			{
				if ( gDoubleGoer.UserIndex[i] != 0 )
				{
					gDoubleGoer.EventEnd(gDoubleGoer.UserIndex[i],0x01);
				}
			}
			CLog.LogAdd("[DoubleGoer]: Event Run -> Event Fail, Time is Out");
			gDoubleGoer.Clear();
			_endthread();
		}
		if ( gDoubleGoer.MonsterCount == 3 )	// Monsters Passed Portal
		{
			gDoubleGoer.Timer = 0;
			// ----
			for (int i=0;i<5;i++)
			{
				if ( gDoubleGoer.UserIndex[i] != 0 )
				{
					gDoubleGoer.EventEnd(gDoubleGoer.UserIndex[i],0x01);
				}
			}
			CLog.LogAdd("[DoubleGoer]: Event Run -> Event Fail, Monsters have passed Portal");
			gDoubleGoer.Clear();
			_endthread();
		}
		if ( gDoubleGoer.BossStage >= 3 )
		{
			gDoubleGoer.Timer = 0;

			for (int i=0;i<5;i++)
			{
				if ( gDoubleGoer.UserIndex[i] != 0 )
				{
					gDoubleGoer.EventEnd(gDoubleGoer.UserIndex[i],0x00);
				}
			}
			gDoubleGoer.DeleteMonsters();
			gDoubleGoer.Success = 1;
			gDoubleGoer.AddMonster(gDoubleGoer.EventMap,542,gDoubleGoer.BossStartX,gDoubleGoer.BossStartY,0);
			_beginthread( EventCompleted, 0, NULL  );
			_endthread();
		}
		// ----
		// ---- Set Players Triangle Position
		PMSG_DGOER_UPDATE_BAR pMsg;
		// ----
		pMsg.c = 0xC1;
		pMsg.size = 0x1C;
		pMsg.headcode = 0xBF;
		pMsg.subcode = 0x12;
		pMsg.TimerL = LOBYTE(gDoubleGoer.Timer);
		pMsg.TimerH = HIBYTE(gDoubleGoer.Timer);
		pMsg.UserCount = gDoubleGoer.UserCount;
		pMsg.Unk = 0;
		// ----
		
		pMsg.User1H = HIBYTE(gDoubleGoer.UserIndex[0]);
		pMsg.User1L = LOBYTE(gDoubleGoer.UserIndex[0]);
		pMsg.Unk1 = 0;
		pMsg.Pos1 = gDoubleGoer.UserPos[0];

		pMsg.User2H = 0x00;
		pMsg.User2L = 0x00;
		pMsg.Unk2 = 0;
		pMsg.Pos2 = gDoubleGoer.UserPos[1];

		pMsg.User3H = 0x00;
		pMsg.User3L = 0x00;
		pMsg.Unk3 = 0;
		pMsg.Pos3 = gDoubleGoer.UserPos[2];

		pMsg.User4H = 0x00;
		pMsg.User4L = 0x00;
		pMsg.Unk4 = 0;
		pMsg.Pos4 = gDoubleGoer.UserPos[3];

		pMsg.User5H = 0x00;
		pMsg.User5L = 0x00;
		pMsg.Unk5 = 0;
		pMsg.Pos5 = gDoubleGoer.UserPos[4];

		if ( gDoubleGoer.UserIndex[0] != 0 )
		{
			gSendProto.DataSend(gDoubleGoer.UserIndex[0], (BYTE*)&pMsg, pMsg.size);
		}

		pMsg.User1H = 0x00;
		pMsg.User1L = 0x00;
		pMsg.Unk1 = 0;
		pMsg.Pos1 = gDoubleGoer.UserPos[0];

		pMsg.User2H = HIBYTE(gDoubleGoer.UserIndex[1]);
		pMsg.User2L = LOBYTE(gDoubleGoer.UserIndex[1]);
		pMsg.Unk2 = 0;
		pMsg.Pos2 = gDoubleGoer.UserPos[1];

		pMsg.User3H = 0x00;
		pMsg.User3L = 0x00;
		pMsg.Unk3 = 0;
		pMsg.Pos3 = gDoubleGoer.UserPos[2];

		pMsg.User4H = 0x00;
		pMsg.User4L = 0x00;
		pMsg.Unk4 = 0;
		pMsg.Pos4 = gDoubleGoer.UserPos[3];

		pMsg.User5H = 0x00;
		pMsg.User5L = 0x00;
		pMsg.Unk5 = 0;
		pMsg.Pos5 = gDoubleGoer.UserPos[4];

		if ( gDoubleGoer.UserIndex[1] != 0 )
		{
			gSendProto.DataSend(gDoubleGoer.UserIndex[1], (BYTE*)&pMsg, pMsg.size);
		}
		
		pMsg.User1H = 0x00;
		pMsg.User1L = 0x00;
		pMsg.Unk1 = 0;
		pMsg.Pos1 = gDoubleGoer.UserPos[0];

		pMsg.User2H = 0x00;
		pMsg.User2L = 0x00;
		pMsg.Unk2 = 0;
		pMsg.Pos2 = gDoubleGoer.UserPos[1];

		pMsg.User3H = HIBYTE(gDoubleGoer.UserIndex[2]);
		pMsg.User3L = LOBYTE(gDoubleGoer.UserIndex[2]);
		pMsg.Unk3 = 0;
		pMsg.Pos3 = gDoubleGoer.UserPos[2];

		pMsg.User4H = 0x00;
		pMsg.User4L = 0x00;
		pMsg.Unk4 = 0;
		pMsg.Pos4 = gDoubleGoer.UserPos[3];

		pMsg.User5H = 0x00;
		pMsg.User5L = 0x00;
		pMsg.Unk5 = 0;
		pMsg.Pos5 = gDoubleGoer.UserPos[4];

		if ( gDoubleGoer.UserIndex[2] != 0 )
		{
			gSendProto.DataSend(gDoubleGoer.UserIndex[2], (BYTE*)&pMsg, pMsg.size);
		}

		pMsg.User1H = 0x00;
		pMsg.User1L = 0x00;
		pMsg.Unk1 = 0;
		pMsg.Pos1 = gDoubleGoer.UserPos[0];

		pMsg.User2H = 0x00;
		pMsg.User2L = 0x00;
		pMsg.Unk2 = 0;
		pMsg.Pos2 = gDoubleGoer.UserPos[1];

		pMsg.User3H = 0x00;
		pMsg.User3L = 0x00;
		pMsg.Unk3 = 0;
		pMsg.Pos3 = gDoubleGoer.UserPos[2];

		pMsg.User4H = HIBYTE(gDoubleGoer.UserIndex[3]);
		pMsg.User4L = LOBYTE(gDoubleGoer.UserIndex[3]);
		pMsg.Unk4 = 0;
		pMsg.Pos4 = gDoubleGoer.UserPos[3];

		pMsg.User5H = 0x00;
		pMsg.User5L = 0x00;
		pMsg.Unk5 = 0;
		pMsg.Pos5 = gDoubleGoer.UserPos[4];

		if ( gDoubleGoer.UserIndex[3] != 0 )
		{
			gSendProto.DataSend(gDoubleGoer.UserIndex[3], (BYTE*)&pMsg, pMsg.size);
		}

		pMsg.User1H = 0x00;
		pMsg.User1L = 0x00;
		pMsg.Unk1 = 0;
		pMsg.Pos1 = gDoubleGoer.UserPos[0];

		pMsg.User2H = 0x00;
		pMsg.User2L = 0x00;
		pMsg.Unk2 = 0;
		pMsg.Pos2 = gDoubleGoer.UserPos[1];

		pMsg.User3H = 0x00;
		pMsg.User3L = 0x00;
		pMsg.Unk3 = 0;
		pMsg.Pos3 = gDoubleGoer.UserPos[2];

		pMsg.User4H = 0x00;
		pMsg.User4L = 0x00;
		pMsg.Unk4 = 0;
		pMsg.Pos4 = gDoubleGoer.UserPos[3];

		pMsg.User5H = HIBYTE(gDoubleGoer.UserIndex[4]);
		pMsg.User5L = LOBYTE(gDoubleGoer.UserIndex[4]);
		pMsg.Unk5 = 0;
		pMsg.Pos5 = gDoubleGoer.UserPos[4];

		if ( gDoubleGoer.UserIndex[4] != 0 )
		{
			gSendProto.DataSend(gDoubleGoer.UserIndex[4], (BYTE*)&pMsg, pMsg.size);
		}


		// Set Red Triangle Position
		PMSG_DGOER_RTRIANG_POS pMsg2;
		pMsg2.c = 0xC1;
		pMsg2.size = 0x06;
		pMsg2.headcode = 0xBF;
		pMsg2.subcode = 0x11;
		pMsg2.Unk = 0;
		pMsg2.Pos = gDoubleGoer.BossMonsterPoss;

		// Set Yellow Line Position
		PMSG_DGOER_TRIANG_POS pMsg3;
		pMsg3.c = 0xC1;
		pMsg3.size = 0x05;
		pMsg3.headcode = 0xBF;
		pMsg3.subcode = 0x0F;
		pMsg3.Pos = gDoubleGoer.MonsterPos; // -- Set Monster Pos

		//Set Monster Kill Count
		PMSG_DGOER_MONSTER_COUNT pMsg4;
		pMsg4.c = 0xC1;
		pMsg4.size = 0x06;
		pMsg4.headcode = 0xBF;
		pMsg4.subcode = 0x14;
		pMsg4.Total = 3;
		pMsg4.Killed = gDoubleGoer.MonsterCount;
		//pMsg4.Killed = 0x00;

		for (int i=0;i<5;i++)
		{
			if ( gDoubleGoer.UserIndex[i] != 0 )
			{
				//DataSend(gDoubleGoer.UserIndex[i],(BYTE*)&pMsg,pMsg.size);
				gSendProto.DataSend(gDoubleGoer.UserIndex[i],(BYTE*)&pMsg2,pMsg2.size);
				gSendProto.DataSend(gDoubleGoer.UserIndex[i],(BYTE*)&pMsg3,pMsg3.size);
				gSendProto.DataSend(gDoubleGoer.UserIndex[i],(BYTE*)&pMsg4,pMsg4.size);
			}
		}
		
		Sleep(1000);
		gDoubleGoer.Timer--;
	}
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::EventEnd(int aIndex, BYTE State)
{
	PMSG_DGOER_END pMsg;

	pMsg.c = 0xC1;
	pMsg.size = 0x05;
	pMsg.headcode = 0xBF;
	pMsg.subcode = 0x13;
	pMsg.Status = State;

	gSendProto.DataSend(aIndex,(BYTE*)&pMsg,pMsg.size);
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::Clear()
{
	for (int i=0;i<5+1;i++)
	{
		LPOBJ lpObj = &gObj[gDoubleGoer.UserIndex[i]];
		// ----
		if ( gObjIsConnected(lpObj) == true && lpObj->MapNumber == gDoubleGoer.EventMap )
		{
			gObjMoveGate(gDoubleGoer.UserIndex[i],267);
			// ----
			if ( gDoubleGoer.Success == 0 )
			{
				gDoubleGoer.EventEnd(gDoubleGoer.UserIndex[i],0x01);
			}
		}
	}
	gDoubleGoer.BossStage = 0;
	gDoubleGoer.Running = 0;
	gDoubleGoer.EventMap = 0;
	gDoubleGoer.EventGate = 0;
	gDoubleGoer.EventX = 0;
	gDoubleGoer.EventY = 0;
	gDoubleGoer.UserCount = 0;
	gDoubleGoer.PartyActived = false;
	gDoubleGoer.UserIndexMaxHit[0] = 0;
	gDoubleGoer.UserIndexMaxHit[1] = 0;
	gDoubleGoer.UserIndexMaxHit[2] = 0;
	gDoubleGoer.BossStage = 0;
	gDoubleGoer.BossMonsterIndex[0] = -1;
	gDoubleGoer.BossMonsterIndex[1] = -1;
	gDoubleGoer.BossMonsterIndex[2] = -1;
	gDoubleGoer.BossMonsterPoss = 0;
	gDoubleGoer.MonsterPos = 0;
	gDoubleGoer.Success = 0;
	gDoubleGoer.MobNearY = 255;
	gDoubleGoer.MonsterPosIndex = -1;
	gDoubleGoer.GoldenChestIndex = -1;

	gDoubleGoer.BlockDoubleGoerEntrance(65);
	gDoubleGoer.BlockDoubleGoerEntrance(66);
	gDoubleGoer.BlockDoubleGoerEntrance(67);
	gDoubleGoer.BlockDoubleGoerEntrance(68);

	for (int i=0;i<3;i++)
	{
		if ( gDoubleGoer.BossMonsterIndex[i] != -1 )
		{
			gObjDel(gDoubleGoer.BossMonsterIndex[i]);
			gDoubleGoer.BossMonsterIndex[i] = -1;
		}
	}

	gDoubleGoer.ChestIndex[0][0] = -1;
	gDoubleGoer.ChestIndex[0][1] = -1;
	gDoubleGoer.ChestIndex[0][2] = -1;
	gDoubleGoer.ChestIndex[1][0] = -1;
	gDoubleGoer.ChestIndex[1][1] = -1;
	gDoubleGoer.ChestIndex[1][2] = -1;
	gDoubleGoer.ChestIndex[2][0] = -1;
	gDoubleGoer.ChestIndex[2][1] = -1;
	gDoubleGoer.ChestIndex[2][2] = -1;

	gDoubleGoer.Timer = gDoubleGoer.OriginalTimer*60;
	gDoubleGoer.StandBy = 0;
	gDoubleGoer.StandByTimer = gDoubleGoer.OriginalStandByTimer*10;
	// ----
	for (int i=0;i<5;i++)
	{
		gDoubleGoer.UserIndex[i] = 0;
	}
	// ----
	gDoubleGoer.DeleteMonsters();
	gDoubleGoer.DeleteChests(0,0);
	gDoubleGoer.DeleteChests(0,1);
	gDoubleGoer.DeleteChests(0,2);
	gDoubleGoer.DeleteChests(1,0);
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::SetMonsters()
{
	char Type = gDoubleGoer.GetEventIndex(gDoubleGoer.EventMap);

	if ( Type == -1 )
	{
		CLog.LogAdd("[BUG TRACER][DoubleGoer]: Set Monsters -> Type Error");
		return;
	}

	if ( gDoubleGoer.NormalMonsterCount[Type] <= 0 )
	{
		CLog.LogAdd("[BUG TRACER][DoubleGoer]: Set Monsters -> Monster Count Error");
		return;
	}
	int result = -1;
	int MobCt = gDoubleGoer.NormalMonsterCount[Type];

	for (int i=0;i<gDoubleGoer.NormalMonsterCount[Type];i++)
	{
		if ( gDoubleGoer.NormalMonsterPos[Type][i] != -1 )
		{
			result = gObjAddMonster(gDoubleGoer.EventMap);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = gDoubleGoer.NormalMonsterPos[Type][i];
				gObj[result].X = gMSetBase.m_Mp[gDoubleGoer.NormalMonsterPos[Type][i]].m_X;
				gObj[result].Y = gMSetBase.m_Mp[gDoubleGoer.NormalMonsterPos[Type][i]].m_Y;
				gObj[result].MapNumber = gDoubleGoer.EventMap;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObj[result].MTX = gDoubleGoer.EventX;
				gObj[result].MTY = gDoubleGoer.EventY;
				gObjSetMonster(result, gMSetBase.m_Mp[gDoubleGoer.NormalMonsterPos[Type][i]].m_Type);
				gObj[result].Dir = rand()%8;

				gDoubleGoer.MonstersAddIndex[gDoubleGoer.MonstersAddCount] = result;
				gDoubleGoer.MonstersAddCount++;
			}
			else
			{
				CLog.LogAdd("[BUG TRACER][DoubleGoer]: Set Monsters -> Add Monster Error");
			}
		}
	}
	for (int i=0;i<3;i++)
	{
		if ( gDoubleGoer.BossMonsterPos[Type][i] != -1 )
		{
			result = gObjAddMonster(gDoubleGoer.EventMap);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = gDoubleGoer.BossMonsterPos[Type][i];
				gObj[result].X = gMSetBase.m_Mp[gDoubleGoer.BossMonsterPos[Type][i]].m_X;
				gObj[result].Y = gMSetBase.m_Mp[gDoubleGoer.BossMonsterPos[Type][i]].m_Y;
				gObj[result].MapNumber = gDoubleGoer.EventMap;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObj[result].MTX = gDoubleGoer.EventX;
				gObj[result].MTY = gDoubleGoer.EventY;
				gObj[result].MaxRegenTime = 0;
				gObjSetMonster(result, gMSetBase.m_Mp[gDoubleGoer.BossMonsterPos[Type][i]].m_Type);
				gObj[result].Dir = rand()%8;

				gDoubleGoer.BossMonsterIndex[i] = result;
			}
			else
			{
				CLog.LogAdd("[DoubleGoer]: Set Monsters -> Add Monster Error");
			}
		}
	}


}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::DeleteMonsters()
{
	for (int i=0;i<255;i++)
	{
		if ( gDoubleGoer.MonstersAddIndex[i] != -1 )
		{
			gObjDel(gDoubleGoer.MonstersAddIndex[i]);
			gDoubleGoer.MonstersAddIndex[i] = -1;
		}
	}
	gDoubleGoer.MonstersAddCount = 0;

	for (int i=0;i<50;i++)
	{
		if ( gDoubleGoer.LarvaIndex[0][i] != -1 )
		{
			gObjDel(gDoubleGoer.LarvaIndex[0][i]);
			gDoubleGoer.LarvaIndex[0][i] = -1;
		}
		if ( gDoubleGoer.LarvaIndex[1][i] != -1 )
		{
			gObjDel(gDoubleGoer.LarvaIndex[1][i]);
			gDoubleGoer.LarvaIndex[1][i] = -1;
		}
		if ( gDoubleGoer.LarvaIndex[2][i] != -1 )
		{
			gObjDel(gDoubleGoer.LarvaIndex[2][i]);
			gDoubleGoer.LarvaIndex[2][i] = -1;
		}
	}

	gDoubleGoer.LarvaCount[0] = 0;
	gDoubleGoer.LarvaCount[1] = 0;
	gDoubleGoer.LarvaCount[2] = 0;
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::OpenChest(int aIndex,LPOBJ lpNpc,BYTE Chest)
{
	if ( DG_MAP_RANGE(gObj[aIndex].MapNumber) == false )
	{
		SendMsg.MessageOut(aIndex, 0x01, "[DoubleGoer]: Chest can only be opened in Double Goer Map");
		return;
	}
	if ( gObj[aIndex].MapNumber != gDoubleGoer.EventMap )
	{
		CLog.LogAdd("[BUG TRACER][DoubleGoer]: Open Chest -> Map Error");
		return;
	}
	// ----
	unsigned char Map = 0;
	unsigned char X = 0;
	unsigned char Y = 0;
	char Type = -1;
	// ----
	if ( Chest == 0 )	// Silver Chest
	{
		for ( int i=0;i<3;i++)
		{
			if ( gDoubleGoer.ChestIndex[0][i] != -1 )
			{
				if ( gDoubleGoer.ChestIndex[0][i] == lpNpc->m_Index )
				{
					Map = gObj[gDoubleGoer.ChestIndex[0][i]].MapNumber;
					X = gObj[gDoubleGoer.ChestIndex[0][i]].X;
					Y = gObj[gDoubleGoer.ChestIndex[0][i]].Y;
					Type = 0;
					break;
				}
			}
			else if ( gDoubleGoer.ChestIndex[1][i] != -1 )
			{
				if ( gDoubleGoer.ChestIndex[1][i] == lpNpc->m_Index )
				{
					Map = gObj[gDoubleGoer.ChestIndex[1][i]].MapNumber;
					X = gObj[gDoubleGoer.ChestIndex[1][i]].X;
					Y = gObj[gDoubleGoer.ChestIndex[1][i]].Y;
					Type = 1;
					break;
				}
			}
			else if ( gDoubleGoer.ChestIndex[2][i] != -1 )
			{
				if ( gDoubleGoer.ChestIndex[2][i] == lpNpc->m_Index )
				{
					Map = gObj[gDoubleGoer.ChestIndex[2][i]].MapNumber;
					X = gObj[gDoubleGoer.ChestIndex[2][i]].X;
					Y = gObj[gDoubleGoer.ChestIndex[2][i]].Y;
					Type = 2;
					break;
				}
			}
		}

		if ( Type == -1 )
		{
			CLog.LogAdd("[BUG TRACER][DoubleGoer]: Open Chest -> Chest Index Error");
			return;
		}

		if ( gDoubleGoer.UserIndexMaxHit[Type] == aIndex )
		{
			if ( rand()%1000 < gDoubleGoer.LarvaRate )
			{
				for (int i=0;i<8;i++)
				{
					gDoubleGoer.AddMonster(Map,532,X,Y,Type);
				}
				CLog.LogAdd("[DoubleGoer]: Open Chest -> Silver Chest was a trap, larva monster added");
			}
			else
			{
				CLog.LogAdd("[DoubleGoer]: Open Chest -> Silver Chest opened successfully");
				gDoubleGoer.ChestDrop(aIndex,Chest,Map,X,Y);
			}
			gDoubleGoer.DeleteChests(Chest,Type);
			return;
		}
		else
		{
			SendMsg.MessageOut(aIndex,  0x01, "[DoubleGoer]: This Chest can only be opened by %s",gObj[gDoubleGoer.UserIndexMaxHit[Type]].Name);
			return;
		}
	}
	else if ( Chest == 1 )	// Golden Chest
	{
		Map = lpNpc->MapNumber;
		X = lpNpc->X;
		Y = lpNpc->Y;
		CLog.LogAdd("[DoubleGoer]: Open Chest -> Golden Chest opened successfully");
		gDoubleGoer.ChestDrop(aIndex,Chest,Map,X,Y);
		gDoubleGoer.DeleteChests(Chest,Type);
		return;
	}
	
}
// ----------------------------------------------------------------------------------------
bool CDoubleGoer::ChestCreate(LPOBJ lpObj)
{
	bool bResult = false;
	switch ( lpObj->Class )
	{
	case 529:	// -- Angry Butcher
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y,1);
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y-1,1);
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y+1,1);
		bResult = true;
		break;
	case 530:	// -- Butcher
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y,0);
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y-1,0);
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y+1,0);
		bResult = true;
		break;
	case 531:	// -- Ice Walker
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y,2);
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y-1,2);
		gDoubleGoer.AddMonster(lpObj->MapNumber,541,lpObj->X,lpObj->Y+1,2);
		bResult = true;
		break;
	}
	return bResult;
}
// ----------------------------------------------------------------------------------------
void EventCompleted(void * lpParam)
{
	Sleep(60000);
	gDoubleGoer.Clear();
	_endthread();
}
// ----------------------------------------------------------------------------------------.
int CDoubleGoer::CheckSignPos(LPOBJ lpObj)
{
	for ( int x = 0;x<MAIN_INVENTORY_SIZE;x++ )
	{
		if ( lpObj->pInventory[x].IsItem() == true )
		{
			if ( lpObj->pInventory[x].m_Type == ITEMGET(14, 110) )
			{
				int iITEM_DUR = (int)lpObj->pInventory[x].m_Durability;

				if ( SIGN_MARK_RANGE(iITEM_DUR) == TRUE )
				{
						return x;
				}
			}
		}
	}

	return -1;
}
// ----------------------------------------------------------------------------------------
char CDoubleGoer::GetEventIndex(unsigned char MapNumber)
{
	char btReturn = -1;

	switch ( MapNumber )
	{
	case 65:
		btReturn = 0;
		break;
	case 66:
		btReturn = 1;
		break;
	case 67:
		btReturn = 2;
		break;
	case 68:
		btReturn = 3;
		break;
	}

	return btReturn;
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::AddMonster(unsigned char Map,short MonsterType,unsigned char X, unsigned char Y, unsigned char Type)
{
	int result = -1;
	// ----
	result = gObjAddMonster(Map);

	if ( result >= 0 )
	{
		gObj[result].X = X;
		gObj[result].Y = Y;
		gObj[result].MapNumber = Map;
		gObj[result].TX = gObj[result].X;
		gObj[result].TY = gObj[result].Y;
		gObj[result].m_OldX = gObj[result].X;
		gObj[result].m_OldY = gObj[result].Y;
		gObj[result].StartX = gObj[result].X;
		gObj[result].StartY = gObj[result].Y;
		gObjSetMonster(result, MonsterType);
		gObj[result].Dir = rand()%8;

		if ( MonsterType == 541 )
		{
			for (int i=0;i<3;i++)
			{
				if ( gDoubleGoer.ChestIndex[Type][i] == -1 )
				{
					gDoubleGoer.ChestIndex[Type][i] = result;
					break;
				}
			}
		}
		else if ( MonsterType == 532 )
		{
			gDoubleGoer.LarvaIndex[Type][gDoubleGoer.LarvaCount[Type]] = result;
			gDoubleGoer.LarvaCount[Type]++;
		}
		else
		{
			gDoubleGoer.GoldenChestIndex = result;
		}
		CLog.LogAdd("[DoubleGoer]: Add Monster -> Added Monster (%)",result);
	}
	else
	{
		CLog.LogAdd("[BUG TRACER][DoubleGoer]: Add Monster -> Add Monster Error");
	}
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::DeleteChests(char ChestType, char Type)
{
	if ( ChestType == 0 )	// Silver Chest
	{
		for ( int i=0;i<3;i++)
		{
			if ( gDoubleGoer.ChestIndex[Type][i] != -1 )
			{
				CLog.LogAdd("[DoubleGoer]: Delete Chests -> Delete Silver Chest (%d)",gDoubleGoer.ChestIndex[Type][i]);
				gObjDel(gDoubleGoer.ChestIndex[Type][i]);
				gDoubleGoer.ChestIndex[Type][i] = -1;
			}
		}
	}
	else if ( ChestType == 1 )	// Golden Chest
	{
		if ( gDoubleGoer.GoldenChestIndex != -1 )
		{
			CLog.LogAdd("[DoubleGoer]: Delete Chests -> Delete Golden Chest (%d)",gDoubleGoer.GoldenChestIndex);
			gObjDel(gDoubleGoer.GoldenChestIndex);
			gDoubleGoer.GoldenChestIndex = -1;
		}
	}

}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::ChestDrop(int aIndex, unsigned char Type, unsigned char Map, unsigned char X, unsigned char Y)
{
	LPOBJ lpObj = &gObj[aIndex];

	int RandomItem;
	int Item;
	unsigned char Skill;
	unsigned char Luck;
	unsigned char Opt;
	unsigned char Level;
	unsigned char Dur;
	unsigned char Exe;
	unsigned char Anc;
	unsigned char Socket[5];
	
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	Skill = 0;
	Luck = 0;
	Opt = 0;
	Level = 0;
	Dur = 0;
	Exe = 0;
	Anc = 0;
	Item = 0;

	RandomItem = rand()%gDoubleGoer.ItemCount[Type];

	Item = ITEMGET(gDoubleGoer.Type[Type][RandomItem],gDoubleGoer.Index[Type][RandomItem]);

	if ( rand()%1000 < 50 )
	{
		Skill = gDoubleGoer.Skill[Type][RandomItem];
	}

	if ( rand()%1000 < 50 )
	{
		Luck = gDoubleGoer.Luck[Type][RandomItem];
	}

	if ( rand()%1000 < 50 )
	{
		if ( gDoubleGoer.Level[Type][RandomItem] > 0 )
		{
			Level = rand()%(gDoubleGoer.Level[Type][RandomItem]+1);
		}
		else
		{
			Level = 0;
		}
	}

	if ( rand()%1000 < 50 )
	{
		if ( gDoubleGoer.Opt[Type][RandomItem] > 0 )
		{
			Opt = rand()%(gDoubleGoer.Opt[Type][RandomItem]+1);
		}
		else
		{
			Opt = 0;
		}
	}

	if ( rand()%1000 < 50 )
	{
		Exe = GetRandomExeOption(gDoubleGoer.Exe[Type][RandomItem]+1);
	}

	if ( rand()%1000 < 10 )
	{
		if ( gDoubleGoer.Anc[Type][RandomItem] == 5 || gDoubleGoer.Anc[Type][RandomItem] == 10 )
		{
			Anc = gDoubleGoer.Anc[Type][RandomItem];
		}
		else
		{
			Anc = 0;
		}
	}
	if ( rand()%1000 < 50 )
	{
		Socket[0] = gDoubleGoer.Socket[0][Type][RandomItem];

		if ( rand()%1000 < 50 )
		{
			Socket[1] = gDoubleGoer.Socket[1][Type][RandomItem];

			if ( rand()%1000 < 50 )
			{
				Socket[2] = gDoubleGoer.Socket[2][Type][RandomItem];

				if ( rand()%1000 < 50 )
				{
					Socket[3] = gDoubleGoer.Socket[3][Type][RandomItem];

					if ( rand()%1000 < 50 )
					{
						Socket[4] = gDoubleGoer.Socket[4][Type][RandomItem];
					}
				}
			}
		}
	}

	ItemSerialCreateSend(aIndex,Map,X,Y,Item,Level,Dur,Luck,Skill,Opt,aIndex,Exe,Anc,Socket);

}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::MonsterMoveProccess(LPOBJ lpObj)
{

}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::BlockDoubleGoerEntrance(char Map)
{
	for ( int i= ::DoubleGoerEntranceMap[Map-65].btStartX;i<= ::DoubleGoerEntranceMap[Map-65].btEndX;i++)
	{
		for ( int j= ::DoubleGoerEntranceMap[Map-65].btStartY;j<= ::DoubleGoerEntranceMap[Map-65].btEndY;j++)
		{

			MapC[Map].m_attrbuf[j * 256 + i] |= 8;
		}
	}
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::SendDoubleGoerEntranceBlock(char Map,int aIndex,bool Live)
{
	char cTEMP_BUF[256];
	PMSG_DG_MAPATTR * lpMsg = (PMSG_DG_MAPATTR *)cTEMP_BUF;

	lpMsg->c = 0xC1;
	lpMsg->size = sizeof(PMSG_DG_MAPATTR)+sizeof(PMSG_DG_SETATTR)*6;
	lpMsg->headcode = 0x46;
	PMSG_DG_SETATTR * lpMsgBody = (PMSG_DG_SETATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 4;
	(Live)?(lpMsg->btMapSetType=0) :( lpMsg->btMapSetType=1);

	lpMsgBody[0].btX = ::DoubleGoerEntranceMap[Map-65].btStartX;
	lpMsgBody[0].btY = ::DoubleGoerEntranceMap[Map-65].btStartY;
	lpMsgBody[1].btX   = ::DoubleGoerEntranceMap[Map-65].btEndX;
	lpMsgBody[1].btY   = ::DoubleGoerEntranceMap[Map-65].btEndY;

	gSendProto.DataSend(aIndex, (unsigned char *)lpMsg, lpMsg->size);
}
// ----------------------------------------------------------------------------------------
void CDoubleGoer::ReleaseDoubleGoerEntrance(char Map)
{
	for ( int i= ::DoubleGoerEntranceMap[Map-65].btStartX;i<= ::DoubleGoerEntranceMap[Map-65].btEndX;i++)
	{
		for ( int j= ::DoubleGoerEntranceMap[Map-65].btStartY;j<= ::DoubleGoerEntranceMap[Map-65].btEndY;j++)
		{
			MapC[Map].m_attrbuf[j * 256 + i] &= ~8;
		}
	}
}
// ----------------------------------------------------------------------------------------
void StandByStart(void * lpParam)
{
	while ( true )
	{
		if ( gDoubleGoer.StandByTimer <= 0 )
		{
			gDoubleGoer.StandBy = 0;
			gDoubleGoer.InitTimer();
			_endthread();
		}

		/*if ( gDoubleGoer.StandByTimer == 30 )
		{
			PMSG_DG_STANDBYTIMER pMsg;

			pMsg.c = 0xC1;
			pMsg.size = 0x04;
			pMsg.headcode = 0x92;
			pMsg.type = 0x04;

			for (int i=0;i<5;i++)
			{
				if ( gDoubleGoer.UserIndex[i] != 0 )
				{
					DataSend(gDoubleGoer.UserIndex[i],(BYTE *)&pMsg,pMsg.size);
				}
			}
			
		}*/

		gDoubleGoer.StandByTimer--;
		Sleep(1000);
	}
}
// ----------------------------------------------------------------------------------------
#endif