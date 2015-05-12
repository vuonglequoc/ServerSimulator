#include "../Header Files/stdafx.h"
#include "../Header Files/Raklion.h"
#include "../Header Files/Defines.h"
#include "../Header Files/Notice.H"
#include "../Header Files/Functions.h"
#include "../Header Files/gObjMonster.h"
#include "../Header Files/Event.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/GameMain.h"
#include "../Header Files/MoveSystem.h"


CRaklion gRaklion;

CRaklion::CRaklion(void)
{

}

CRaklion::~CRaklion(void)
{

}

void CRaklion::Init()
{
	gRaklion.Enabled			= GetPrivateProfileInt(RAKLION_HDR,"Enabled",0,RAKLION_FILE_PATH);
	gRaklion.MaxUsers			= GetPrivateProfileInt(RAKLION_HDR,"MaxUsers",0,RAKLION_FILE_PATH);
	gRaklion.MaxItemDrop		= GetPrivateProfileInt(RAKLION_HDR,"MaxItemDrop",0,RAKLION_FILE_PATH);
	gRaklion.StandByTime		= GetPrivateProfileInt(RAKLION_HDR,"TimeBeforeStart",0,RAKLION_FILE_PATH);
	gRaklion.KillSpidersTime	= GetPrivateProfileInt(RAKLION_HDR,"KillSpidersTime",0,RAKLION_FILE_PATH);
	gRaklion.KillEggsTime		= GetPrivateProfileInt(RAKLION_HDR,"KillEggsTime",0,RAKLION_FILE_PATH);
	gRaklion.KillSelupanTime	= GetPrivateProfileInt(RAKLION_HDR,"KillSelupanTime",0,RAKLION_FILE_PATH);
	gRaklion.SelupanMaxItem		= GetPrivateProfileInt(RAKLION_HDR,"SelupanMaxItemDrop",0,RAKLION_FILE_PATH);
	gRaklion.ItemCount			= 0;
	gRaklion.AuxStandByTime		= gRaklion.StandByTime*60;
	gRaklion.AuxKillSpidersTime	= gRaklion.KillSpidersTime*60;
	gRaklion.AuxKillEggsTime	= gRaklion.KillEggsTime*60;
	gRaklion.AuxKillSelupanTime	= gRaklion.KillSelupanTime*60;
	gRaklion.MonsterKilled		= 0;
	gRaklion.Stage				= 0;
	gRaklion.Running			= 0;
	gRaklion.UserCount			= 0;
	gRaklion.MonsterCount		= 0;
	gRaklion.StandBy			= 0;
	gRaklion.Started			= 0;

	if ( gRaklion.MaxUsers > 50 )
	{
		gRaklion.MaxUsers = 50;
	}

	for (int i=0;i<50;i++)
	{
		gRaklion.UserIndex[i] = 0;
	}

	for (int i=0;i<255;i++)
	{
		gRaklion.SpidersPos[i] = -1;
		gRaklion.EggsPos[i] = -1;
	}
	gRaklion.SelupanPos = -1;
	gRaklion.EggsCount = 0;
	gRaklion.SpidersCount = 0;

	for (int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( gMSetBase.m_Mp[n].m_MapNumber == 58 )
		{
			if ( gMSetBase.m_Mp[n].m_Type == 459 )
			{
				gRaklion.SelupanPos = n;
			}
			else if ( gMSetBase.m_Mp[n].m_Type == 457 )
			{
				gRaklion.SpidersPos[gRaklion.SpidersCount] = n;
				gRaklion.SpidersCount++;
			}
			else if ( gMSetBase.m_Mp[n].m_Type == 460 || gMSetBase.m_Mp[n].m_Type == 461 || gMSetBase.m_Mp[n].m_Type == 462 )
			{
				gRaklion.EggsPos[gRaklion.EggsCount] = n;
				gRaklion.EggsType[gRaklion.EggsCount] = gMSetBase.m_Mp[n].m_Type;
				gRaklion.EggsCount++;
			}
		}
	}
}

void CRaklion::ReadFile()
{
	SMDFile = fopen(RAKLIONDR_FILE_PATH, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[BloodCastle] Info file Load Fail [%s]", RAKLIONDR_FILE_PATH);
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

		gRaklion.ItemType[gRaklion.ItemCount] = TokenNumber;
				
		Token = GetToken();
		gRaklion.ItemIndex[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemLevel[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemDur[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemSkill[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemLuck[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemOpt[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemExe[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemAncient[gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemSocket[0][gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemSocket[1][gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemSocket[2][gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemSocket[3][gRaklion.ItemCount] = TokenNumber;

		Token = GetToken();
		gRaklion.ItemSocket[4][gRaklion.ItemCount] = TokenNumber;

		gRaklion.ItemCount++;
    }

	fclose(SMDFile);
	return;
}

void CRaklion::Clear()
{
	gRaklion.Running			= 0;
	gRaklion.UserCount			= 0;
	gRaklion.MonsterCount		= 0;
	gRaklion.StandBy			= 0;
	gRaklion.AuxStandByTime		= gRaklion.StandByTime*60;
	gRaklion.AuxKillSpidersTime	= gRaklion.KillSpidersTime*60;
	gRaklion.AuxKillEggsTime	= gRaklion.KillEggsTime*60;
	gRaklion.AuxKillSelupanTime	= gRaklion.KillSelupanTime*60;
	gRaklion.Stage				= 0;
	gRaklion.MonsterKilled		= 0;
	gRaklion.Started			= 0;

	for (int i=0;i<gRaklion.MaxUsers;i++)
	{
		if ( gRaklion.UserIndex[i] != 0 )
		{
			if ( gObjIsConnected(&gObj[gRaklion.UserIndex[i]]) == true )
			{
				if ( gObj[gRaklion.UserIndex[i]].MapNumber == 58 )
				{
					gObjTeleport(gRaklion.UserIndex[i],57,223,211);
				}
			}
		}
	}

	for (int i=0;i<50;i++)
	{
		gRaklion.UserIndex[i] = 0;
	}
}

void CRaklion::Run()
{
	if ( gRaklion.Enabled == 0 )
	{
		return;
	}
	for (int i=0;i<gRaklion.MaxUsers;i++)
	{
		if ( gRaklion.UserIndex[i] != 0 )
		{
			if ( gObjIsConnected(&gObj[gRaklion.UserIndex[i]]) == false )
			{
				gRaklion.UserIndex[i] = 0;
				gRaklion.UserCount--;
			}
			else
			{
				if ( gObj[gRaklion.UserIndex[i]].MapNumber != 58 )
				{
					SendMsg.MessageOut(gRaklion.UserIndex[i], 0x01, "[Raklion]: You have falied");
					gRaklion.UserIndex[i] = 0;
					gRaklion.UserCount--;
				}
			}
		}
	}
	if ( gRaklion.Running == 1 && gRaklion.UserCount == 0 )
	{
		gRaklion.Clear();
		return;
	}
	for (int i=OBJMIN;i<OBJMAX;i++)
	{
		if ( gObjIsConnected(&gObj[i]) == true )
		{
			gRaklion.MoveToGate(i);
		}
	}
}

void CRaklion::Start()
{
	if ( gRaklion.Enabled == 0 )
	{
		return;
	}
	if ( gRaklion.Started == 0 )
	{
		gRaklion.Started = 1;
		gRaklion.StandBy = 1;
		SendMsg.MessageOutAll(0x00, "[Raklion]: Event will start in %d minutes",gRaklion.StandByTime);
		_beginthread(Raklion_EventStart,0,NULL);
	}
}

void CRaklion::MoveToGate(int aIndex)
{
	if ( gRaklion.Enabled == 0 )
	{
		return;
	}
	if ( gRaklion.Running == 1 )
	{
		return;
	}
	if ( gRaklion.StandBy == 0 )
	{
		return;
	}
	if ( gObj[aIndex].MapNumber == 57 )
	{
		if ( gObj[aIndex].X == 171 && ( gObj[aIndex].Y >= 23 && gObj[aIndex].Y <= 25 ) )
		{
			if ( gRaklion.UserCount < gRaklion.MaxUsers )
			{
				gRaklion.UserIndex[gRaklion.UserCount] = aIndex;
				gRaklion.UserCount++;
				gObjTeleport(aIndex,58,160,25);
				return;
			}
		}
	}
}

void CRaklion::MonsterKillManager(LPOBJ lpObj,int MaxHitUser)
{
	switch ( lpObj->Class )
	{
	case 457:
		gRaklion.MonsterCount--;
		// ----
		if ( gRaklion.MonsterCount > 0 )
		{
			SendMsg.MessageOutMap(58, OBJ_SEND_ALL_NORMAL, "[Raklion]: Left %d Spiders", gRaklion.MonsterCount);
		}
		break;
	case 460:
	case 461:
	case 462:
		gRaklion.MonsterCount--;
		// ----
		if ( gRaklion.MonsterCount > 0 )
		{
			SendMsg.MessageOutMap(58, OBJ_SEND_ALL_NORMAL, "[Raklion]: Left %d Eggs", gRaklion.MonsterCount);
		}
		break;
	case 459:
		SendMsg.MessageOutMap(58, 0x00, "[Raklion]: Selupan has been killed by %s",gObj[MaxHitUser].Name);
		break;
	}
}

void CRaklion::SetMonsters(int Stage)
{
	int result;
	// ----
	switch ( Stage )
	{
	case 1:
		gRaklion.MonsterCount = 0;
		for (int i=0;i<gRaklion.SpidersCount;i++)
		{
			if ( gRaklion.SpidersPos[i] != -1 )
			{
				result = gObjAddMonster(58);

				if ( result >= 0 )
				{
					gObj[result].m_PosNum = gRaklion.SpidersPos[i];
					gObj[result].X = gMSetBase.m_Mp[gRaklion.SpidersPos[i]].m_X;
					gObj[result].Y = gMSetBase.m_Mp[gRaklion.SpidersPos[i]].m_Y;
					gObj[result].MapNumber = 58;
					gObj[result].TX = gObj[result].X;
					gObj[result].TY = gObj[result].Y;
					gObj[result].m_OldX = gObj[result].X;
					gObj[result].m_OldY = gObj[result].Y;
					gObj[result].StartX = (BYTE)gObj[result].X;
					gObj[result].StartY = (BYTE)gObj[result].Y;
					gObjSetMonster(result, 457);
					gObj[result].MaxRegenTime = 0;
					gObj[result].Dir = rand() % 8;
					MonsterCount++;
				}
			}
		}
		break;
	case 2:
		gRaklion.MonsterCount = 0;
		for (int i=0;i<gRaklion.EggsCount;i++)
		{
			if ( gRaklion.EggsPos[i] != -1 )
			{
				result = gObjAddMonster(58);

				if ( result >= 0 )
				{
					gObj[result].m_PosNum = gRaklion.EggsPos[i];
					gObj[result].X = gMSetBase.m_Mp[gRaklion.EggsPos[i]].m_X;
					gObj[result].Y = gMSetBase.m_Mp[gRaklion.EggsPos[i]].m_Y;
					gObj[result].MapNumber = 58;
					gObj[result].TX = gObj[result].X;
					gObj[result].TY = gObj[result].Y;
					gObj[result].m_OldX = gObj[result].X;
					gObj[result].m_OldY = gObj[result].Y;
					gObj[result].StartX = (BYTE)gObj[result].X;
					gObj[result].StartY = (BYTE)gObj[result].Y;
					gObjSetMonster(result, gRaklion.EggsType[i]);
					gObj[result].MaxRegenTime = 0;
					gObj[result].Dir = rand() % 8;
					MonsterCount++;
				}
			}
		}
		break;
	case 3:
		if ( gRaklion.SelupanPos != -1 )
		{
			result = gObjAddMonster(58);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = gRaklion.SelupanPos;
				gObj[result].X = gMSetBase.m_Mp[gRaklion.SelupanPos].m_X;
				gObj[result].Y = gMSetBase.m_Mp[gRaklion.SelupanPos].m_Y;
				gObj[result].MapNumber = 58;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].StartX = (BYTE)gObj[result].X;
				gObj[result].StartY = (BYTE)gObj[result].Y;
				gObjSetMonster(result, 459);
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = rand() % 8;
			}
		}
		break;
	}
}

bool CRaklion::DropManager(LPOBJ lpObj)
{
	bool bResult = false;
	
	// ----
	if ( lpObj->MapNumber == 58 )
	{
		switch ( lpObj->Class )
		{
		case 457:	// -- Spider
			bResult = true;
			break;
		case 459:	// -- Selupan
			bResult = true;
			break;
		case 460:	// -- Egg 1
			bResult = true;
			break;
		case 461:	// -- Egg 2
			bResult = true;
			break;
		case 462:	// -- Egg 3
			bResult = true;
			break;
		}
	}
	return bResult;
}

void CRaklion::SelupanDrop(LPOBJ lpObj,unsigned char X,unsigned char Y)
{
	unsigned char cDropX = X;
	unsigned char cDropY = Y;
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
	unsigned char IsExe;
	unsigned char IsAnc;

	if ( gRaklion.SelupanMaxItem <= 0 )
	{
		return;
	}

	if ( gRaklion.ItemCount <= 0 )
	{
		return;
	}

	for (int i=0;i<gRaklion.SelupanMaxItem;i++)
	{
		Skill = 0;
		Luck = 0;
		Opt = 0;
		Level = 0;
		Dur = 0;
		Exe = 0;
		Anc = 0;
		IsExe = 0;
		IsAnc = 0;
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;
		cDropX = X;
		cDropY = Y;
		// ----
		if ( gObjGetRandomItemDropLocation(lpObj->MapNumber, cDropX, cDropY, 4, 4, 10) == false )
		{
			cDropX = X;
			cDropY = Y;
		}

		RandomItem = rand()%gRaklion.ItemCount;

		Item = ITEMGET(gRaklion.ItemType[RandomItem],gRaklion.ItemIndex[RandomItem]);

		if ( rand()%1000 < 200 )
		{
			Level = rand()%(gRaklion.ItemLevel[RandomItem]+1);
		}

		if ( rand()%1000 < 200 )
		{
			if ( gRaklion.ItemLuck[RandomItem] == 1 )
			{
				Luck = 1;
			}
		}

		if ( rand()%1000 < 200 )
		{
			if ( gRaklion.ItemSkill[RandomItem] == 1 )
			{
				Skill = 1;
			}
		}

		if ( rand()%1000 < 200 )
		{
			Opt = rand()%(gRaklion.ItemOpt[RandomItem]+1);
		}

		if ( rand()%1000 < 200 )
		{
			if ( gRaklion.ItemAncient[RandomItem] == 5 || gRaklion.ItemAncient[RandomItem] == 10 )
			{
				Anc = gRaklion.ItemAncient[RandomItem];
				IsAnc = 1;
			}
		}

		if ( rand()%1000 < 200 )
		{
			Exe = GetRandomExeOption(gRaklion.ItemExe[RandomItem]+1);
			IsExe = 1;
		}

		if ( rand()%1000 < 200 )
		{
			Socket[0] = gRaklion.ItemSocket[0][RandomItem];

			if ( rand()%1000 < 200 )
			{
				Socket[1] = gRaklion.ItemSocket[1][RandomItem];

				if ( rand()%1000 < 200 )
				{
					Socket[2] = gRaklion.ItemSocket[2][RandomItem];

					if ( rand()%1000 < 200 )
					{
						Socket[3] = gRaklion.ItemSocket[3][RandomItem];

						if ( rand()%1000 < 200 )
						{
							Socket[4] = gRaklion.ItemSocket[4][RandomItem];
						}
					}
				}
			}
		}

		if ( Dur == 0 )
		{
			Dur = ItemGetDurability(Item,Level,IsExe,IsAnc);
		}

		ItemSerialCreateSend(lpObj->m_Index,lpObj->MapNumber,cDropX,cDropY,Item,Level,Dur,Skill,Luck,Opt,lpObj->m_Index,Exe,Anc,Socket);
	}
}

void Raklion_EventStart(void * lpParam)
{
	while ( true )
	{
		if ( gRaklion.StandBy == 1 )
		{
			if ( gRaklion.AuxStandByTime <= 0 )
			{
				SendMsg.MessageOutAll(0x00, "[Raklion]: Event is closed");
				gRaklion.StandBy = 0;
				gRaklion.Running = 1;
				gRaklion.Stage = 1;
				gRaklion.SetMonsters(1);

				SendMsg.MessageOutMap(58, 0x00, "[Raklion]: Kill Spiders Until time finish. Time left %d minute(s)",gRaklion.KillSpidersTime);
			}
			else
			{
				gRaklion.AuxStandByTime--;

				if ( gRaklion.AuxStandByTime == 600 )
				{
					SendMsg.MessageOutAll(0x00, "[Raklion]: Event will start in 10 minutes");
				}
				else if ( gRaklion.AuxStandByTime == 300 )
				{
					SendMsg.MessageOutAll(0x00, "[Raklion]: Event will start in 5 minutes");
				}
				else if ( gRaklion.AuxStandByTime == 240 )
				{
					SendMsg.MessageOutAll(0x00, "[Raklion]: Event will start in 4 minutes");
				}
				else if ( gRaklion.AuxStandByTime == 180 )
				{
					SendMsg.MessageOutAll(0x00, "[Raklion]: Event will start in 3 minutes");
				}
				else if ( gRaklion.AuxStandByTime == 120 )
				{
					SendMsg.MessageOutAll(0x00, "[Raklion]: Event will start in 2 minutes");
				}
				else if ( gRaklion.AuxStandByTime == 60 )
				{
					SendMsg.MessageOutAll(0x00, "[Raklion]: Event will start in 1 minutes");
				}
			}
		}
		else
		{
			if ( gRaklion.Stage == 1 )
			{
				if ( gRaklion.AuxKillSpidersTime <= 0)
				{
					SendMsg.MessageOutMap(58, 0x00, "[Raklion]: Event failed. Spiders have not been killed");
					_beginthread(Raklion_EndEvent,0,NULL);
					_endthread();
				}
				if ( gRaklion.MonsterCount <= 0 )
				{
					SendMsg.MessageOutMap(58, 0x00, "[Raklion]: Killed all Spiders. Prepare to Stage 2");

					gRaklion.Stage = 2;
					gRaklion.MonsterCount = 0;
					Sleep(10000);
					gRaklion.SetMonsters(2);
				}
				
				gRaklion.AuxKillSpidersTime--;

				if ( gRaklion.AuxKillSpidersTime == 300 )
				{
					SendMsg.MessageOutMap(58, 0x00, "[Raklion]: 5 minute(s) left to kill Spiders");
				}
				else if ( gRaklion.AuxKillSpidersTime == 240 )
				{
					SendMsg.MessageOutMap(58, 0x00, "[Raklion]: 4 minute(s) left to kill Spiders");
				}
				else if ( gRaklion.AuxKillSpidersTime == 180 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 3 minute(s) left to kill Spiders");
				}
				else if ( gRaklion.AuxKillSpidersTime == 120 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 2 minute(s) left to kill Spiders");
				}
				else if ( gRaklion.AuxKillSpidersTime == 60 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 1 minute(s) left to kill Spiders");
				}
			}
			else if ( gRaklion.Stage == 2 )
			{
				if ( gRaklion.AuxKillEggsTime <= 0)
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: Event failed. Eggs have not been killed");
					_beginthread(Raklion_EndEvent,0,NULL);
					_endthread();
				}

				if ( gRaklion.MonsterCount <= 0 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: Killed all Eggs. Selpuan will appear in 10 seconds");

					gRaklion.Stage = 3;
					gRaklion.MonsterCount = 0;
					Sleep(10000);
					gRaklion.SetMonsters(3);
				}

				gRaklion.AuxKillEggsTime--;

				if ( gRaklion.AuxKillEggsTime == 300 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 5 minute(s) left to kill Eggs");
				}
				else if ( gRaklion.AuxKillEggsTime == 240 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 4 minute(s) left to kill Eggs");
				}
				else if ( gRaklion.AuxKillEggsTime == 180 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 3 minute(s) left to kill Eggs");
				}
				else if ( gRaklion.AuxKillEggsTime == 120 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 2 minute(s) left to kill Eggs");
				}
				else if ( gRaklion.AuxKillEggsTime == 60 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 1 minute(s) left to kill Eggs");
				}
			}
			else if ( gRaklion.Stage == 3 )
			{
				if ( gRaklion.AuxKillSelupanTime <= 0)
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: Event failed. Selupan has not been killed");
					_beginthread(Raklion_EndEvent,0,NULL);
					_endthread();
				}

				if ( gRaklion.MonsterCount <= 0 )
				{
					// Event Success
					gRaklion.Stage = 4;
					gRaklion.MonsterCount = 0;
				}

				gRaklion.AuxKillSelupanTime--;

				if ( gRaklion.AuxKillSelupanTime == 300 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 5 minute(s) left to kill Selupan");
				}
				else if ( gRaklion.AuxKillSelupanTime == 240 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 4 minute(s) left to kill Selupan");
				}
				else if ( gRaklion.AuxKillSelupanTime == 180 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 3 minute(s) left to kill Selupan");
				}
				else if ( gRaklion.AuxKillSelupanTime == 120 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 2 minute(s) left to kill Selupan");
				}
				else if ( gRaklion.AuxKillSelupanTime == 60 )
				{
					SendMsg.MessageOutMap(58,0x00, "[Raklion]: 1 minute(s) left to kill Selupan");
				}
			}
		}
		Sleep(1000);
	}
}

void Raklion_EndEvent(void * lpParam)
{
	SendMsg.MessageOutMap(58,0x00, "[Raklion]: Event will finish in 30 seconds");
	Sleep(30000);
	gRaklion.Clear();
}