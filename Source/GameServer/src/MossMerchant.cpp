#include "stdafx.h"
#include "MossMerchant.h"
#include "Defines.h"
#include "ReadScript.h"
#include "LogProc.H"
#include "User.h"
#include "DSProtocol.h"
#include "Packets.h"
#include "Notice.H"
#include "Functions.H"
#include "SendProto.H"


CMossMerchant gMossMerchant;

CMossMerchant::CMossMerchant(void)
{
	gMossMerchant.Init();
}

CMossMerchant::~CMossMerchant(void)
{

}

void CMossMerchant::Init()
{
	if ( GetPrivateProfileInt("MossMerchant","Enabled",0,MOSS_FILE_PATH) >= 1 )
	{
		gMossMerchant.Time						= GetPrivateProfileInt(MOSS_HDR,"EventMinutesDuration",0,MOSS_FILE_PATH);
		gMossMerchant.TimeAux					= gMossMerchant.Time*60;
		gMossMerchant.SpecialItem[0]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialItem[1]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialItem[2]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialItem[3]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialItem[4]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5Rate",0,MOSS_FILE_PATH);
		// ----
		gMossMerchant.LevelRate					= GetPrivateProfileInt(MOSS_HDR,"LevelRate",0,MOSS_FILE_PATH);
		gMossMerchant.ExeRate					= GetPrivateProfileInt(MOSS_HDR,"ExeRate",0,MOSS_FILE_PATH);
		gMossMerchant.LuckRate					= GetPrivateProfileInt(MOSS_HDR,"LuckRate",0,MOSS_FILE_PATH);
		gMossMerchant.SkillRate					= GetPrivateProfileInt(MOSS_HDR,"SkillRate",0,MOSS_FILE_PATH);
		gMossMerchant.OptionRate				= GetPrivateProfileInt(MOSS_HDR,"OptionRate",0,MOSS_FILE_PATH);
		gMossMerchant.SocketRate[0]				= GetPrivateProfileInt(MOSS_HDR,"Socket1Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SocketRate[1]				= GetPrivateProfileInt(MOSS_HDR,"Socket2Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SocketRate[2]				= GetPrivateProfileInt(MOSS_HDR,"Socket3Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SocketRate[3]				= GetPrivateProfileInt(MOSS_HDR,"Socket4Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SocketRate[4]				= GetPrivateProfileInt(MOSS_HDR,"Socket5Rate",0,MOSS_FILE_PATH);
		gMossMerchant.AncientRate				= GetPrivateProfileInt(MOSS_HDR,"AncientRate",0,MOSS_FILE_PATH);

		gMossMerchant.SpecialExeRate[0]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1ExeRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLevelRate[0]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1LevelRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLuckRate[0]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1LuckRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialOptionRate[0]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1OptionRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSkillRate[0]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1SkillRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[0][0]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1Socket1Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[1][0]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1Socket2Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[2][0]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1Socket3Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[3][0]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1Socket4Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[4][0]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem1Socket5Rate",0,MOSS_FILE_PATH);

		gMossMerchant.SpecialExeRate[1]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2ExeRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLevelRate[1]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2LevelRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLuckRate[1]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2LuckRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialOptionRate[1]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2OptionRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSkillRate[1]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2SkillRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[0][1]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2Socket1Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[1][1]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2Socket2Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[2][1]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2Socket3Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[3][1]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2Socket4Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[4][1]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem2Socket5Rate",0,MOSS_FILE_PATH);

		gMossMerchant.SpecialExeRate[2]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3ExeRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLevelRate[2]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3LevelRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLuckRate[2]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3LuckRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialOptionRate[2]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3OptionRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSkillRate[2]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3SkillRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[0][2]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3Socket1Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[1][2]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3Socket2Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[2][2]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3Socket3Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[3][2]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3Socket4Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[4][2]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem3Socket5Rate",0,MOSS_FILE_PATH);

		gMossMerchant.SpecialExeRate[3]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4ExeRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLevelRate[3]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4LevelRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLuckRate[3]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4LuckRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialOptionRate[3]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4OptionRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSkillRate[3]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4SkillRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[0][3]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4Socket1Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[1][3]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4Socket2Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[2][3]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4Socket3Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[3][3]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4Socket4Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[4][3]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem4Socket5Rate",0,MOSS_FILE_PATH);

		gMossMerchant.SpecialExeRate[4]			= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5ExeRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLevelRate[4]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5LevelRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialLuckRate[4]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5LuckRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialOptionRate[4]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5OptionRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSkillRate[4]		= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5SkillRate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[0][4]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5Socket1Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[1][4]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5Socket2Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[2][4]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5Socket3Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[3][4]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5Socket4Rate",0,MOSS_FILE_PATH);
		gMossMerchant.SpecialSocketRate[4][4]	= GetPrivateProfileInt(MOSS_HDR,"SpecialItem5Socket5Rate",0,MOSS_FILE_PATH);
		// ----
		gMossMerchant.Enabled = true;
		gMossMerchant.Running = false;
		// ----
		for (int j=0;j<5;j++)
		{
			for (int i=0;i<MAX_ITEM_SECTION;i++)
			{
				gMossMerchant.Type[j][i] = -1;
				gMossMerchant.Index[j][i] = -1;
				gMossMerchant.Level[j][i] = 0;
				gMossMerchant.Dur[j][i] = 0;
				gMossMerchant.Skill[j][i] = 0;
				gMossMerchant.Luck[j][i] = 0;
				gMossMerchant.Opt[j][i] = 0;
				gMossMerchant.Exe[j][i] = 0;
				gMossMerchant.Anc[j][i] = 0;
				gMossMerchant.Socket[0][j][i] = 0xFF;
				gMossMerchant.Socket[1][j][i] = 0xFF;
				gMossMerchant.Socket[2][j][i] = 0xFF;
				gMossMerchant.Socket[3][j][i] = 0xFF;
				gMossMerchant.Socket[4][j][i] = 0xFF;
			}

			gMossMerchant.ItemCount[j] = 0;

			if ( gMossMerchant.SpecialItem[j] < 0 )
			{
				gMossMerchant.SpecialItem[j] = 0;
			}
			else if ( gMossMerchant.SpecialItem[j] > 10000 )
			{
				gMossMerchant.SpecialItem[j] = 10000;
			}
		}
		gMossMerchant.ReadFile();
	}
	else
	{
		gMossMerchant.Enabled = false;
	}
}

void CMossMerchant::ReadFile()
{
	SMDFile = fopen(MOSSSELL_FILE_PATH, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[MossMerchant] Info file Load Fail [%s]", MOSSSELL_FILE_PATH);
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
			if ( type < 0 || type > 4 )
			{
				break;
			}
		    Token = GetToken();

			if ( strcmp("end", TokenString ) == 0)
			{
				break;
			}

			gMossMerchant.Type[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Index[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Level[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Dur[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Skill[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Luck[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Opt[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Exe[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Anc[type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Socket[0][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Socket[1][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Socket[2][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Socket[3][type][ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gMossMerchant.Socket[4][type][ItemCount[type]] = TokenNumber;

			gMossMerchant.ItemCount[type]++;
		        
		}
    }

	fclose(SMDFile);
	return;
}

void CMossMerchant::Start()
{
	if ( gMossMerchant.Enabled == false )
	{
		return;
	}

	if ( gMossMerchant.Running == false )
	{
		gMossMerchant.Running = true;
		SendMsg.MessageOutAll(0x00, "[MossMerchant]: Merchant started selling unique items");
		_beginthread(MossEventRun,0,NULL);
	}

	return;
}

struct PMSG_BUYRESULT
{
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char Result;	// 3
	unsigned char ItemInfo[MAX_ITEM_INFO];	// 4
};

void CMossMerchant::ItemBuy(int aIndex,int Item)
{
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_BUYRESULT pResult;

	pResult.c = 0xC1;
	pResult.size = sizeof(PMSG_BUYRESULT);
	pResult.headcode = 0x32;
	pResult.Result = -1;

	if ( !PacketCheckTime(lpObj))
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		if ( lpObj->m_IfState.type != 3 )
		{
			gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);
			return;
		}
	}

	if ( gMossMerchant.Enabled == false )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);
		return;
	}

	if ( gMossMerchant.Running == false )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);
		return;
	}

	if ( CheckInventoryEmptySpace(lpObj,4,2) == false )
	{
		GCServerMsgStringSend("[MossMerchant]: Need to make space in the inventory (4x2)",aIndex,1);
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);
		return;
	}

	if ( lpObj->Money < 1000000 )
	{
		GCServerMsgStringSend("[MossMerchant]: Need 1.000.000 zen to buy here",aIndex,1);
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);
		return;
	}

	int ItemIndex = -1;

	switch ( Item )
	{
	case 0x00:
		ItemIndex = 0;
		break;
	case 0x02:
		ItemIndex = 1;
		break;
	case 0x04:
		ItemIndex = 2;
		break;
	case 0x06:
		ItemIndex = 3;
		break;
	case 0x18:
		ItemIndex = 4;
		break;
	}

	if ( ItemIndex < 0 || ItemIndex > 4 )
	{
		return;
	}

	if ( gMossMerchant.ItemCount[ItemIndex] == 0 )
	{
		return;
	}

	int Random = rand()%gMossMerchant.ItemCount[ItemIndex];

	int NewItem = 0;
	unsigned char NewLevel = 0;
	unsigned char NewDur = 0;
	unsigned char NewSkill = 0;
	unsigned char NewLuck = 0;
	unsigned char NewOpt = 0;
	unsigned char NewExe = 0;
	unsigned char NewAnc = 0;
	unsigned char NewSocket[5];
	unsigned char IsExe = 0;
	unsigned char IsAnc = 0;
	NewSocket[0] = 0xFF;
	NewSocket[1] = 0xFF;
	NewSocket[2] = 0xFF;
	NewSocket[3] = 0xFF;
	NewSocket[4] = 0xFF;

	if ( rand()%1000 < gMossMerchant.OptionRate )
	{
		NewOpt = rand()%(gMossMerchant.Opt[ItemIndex][Random]+1);
	}

	if ( rand()%1000 < gMossMerchant.LuckRate )
	{
		NewLuck = gMossMerchant.Luck[ItemIndex][Random];
	}

	if ( rand()%1000 < gMossMerchant.SkillRate )
	{
		NewSkill = gMossMerchant.Skill[ItemIndex][Random];
	}

	if ( rand()%1000 < gMossMerchant.LevelRate )
	{
		NewLevel = rand()%(gMossMerchant.Level[ItemIndex][Random]+1);
	}

	if ( rand()%1000 < gMossMerchant.AncientRate )
	{
		if ( gMossMerchant.Anc[ItemIndex][Random] == 5 || gMossMerchant.Anc[ItemIndex][Random] == 10 )
		{
			NewAnc = gMossMerchant.Anc[ItemIndex][Random];
			IsAnc = 1;
		}
		else
		{
			IsAnc = 0;
		}
	}

	if ( rand()%1000 < gMossMerchant.ExeRate )
	{
		NewExe = GetRandomExeOption(gMossMerchant.Exe[ItemIndex][Random]);
		IsExe = 1;
	}

	if ( rand()%1000 < gMossMerchant.SocketRate[0] )
	{
		NewSocket[0] = gMossMerchant.Socket[0][ItemIndex][Random];

		if ( rand()%1000 < gMossMerchant.SocketRate[1] )
		{
			NewSocket[1] = gMossMerchant.Socket[1][ItemIndex][Random];

			if ( rand()%1000 < gMossMerchant.SocketRate[2] )
			{
				NewSocket[2] = gMossMerchant.Socket[2][ItemIndex][Random];

				if ( rand()%1000 < gMossMerchant.SocketRate[3] )
				{
					NewSocket[3] = gMossMerchant.Socket[3][ItemIndex][Random];

					if ( rand()%1000 < gMossMerchant.SocketRate[4] )
					{
						NewSocket[4] = gMossMerchant.Socket[4][ItemIndex][Random];
					}
				}
			}
		}
	}

	if ( rand()%10000 < gMossMerchant.SpecialItem[ItemIndex] )
	{
		switch ( ItemIndex )
		{
		case 0:
			NewItem = ITEMGET(3,11);	// Brova
			break;
		case 1:
			NewItem = ITEMGET(5,33);	// Chromatic Staff
			break;
		case 2:
			NewItem = ITEMGET(4,24);	// Aileen Bow
			break;
		case 3:
			NewItem = ITEMGET(2,18);	// Striker Scepter
			break;
		case 4:
			NewItem = ITEMGET(5,34);	// Raven Stick
			break;
		}

		if ( rand()%10000 < gMossMerchant.SpecialExeRate[ItemIndex] )
		{
			NewExe = GetRandomExeOption(4);
			if ( NewExe > 0 )
			{
				IsExe = 1;
			}
		}

		if ( rand()%10000 < gMossMerchant.SpecialOptionRate[ItemIndex] )
		{
			NewOpt = rand()%5;
		}

		if ( rand()%10000 < gMossMerchant.SpecialLuckRate[ItemIndex] )
		{
			NewLuck = 1;
		}

		if ( rand()%10000 < gMossMerchant.SpecialSkillRate[ItemIndex] )
		{
			NewSkill = 1;			
		}

		if ( rand()%10000 < gMossMerchant.SpecialLevelRate[ItemIndex] )
		{
			NewLevel = rand()%16;
		}
		
		if ( rand()%10000 < gMossMerchant.SpecialSocketRate[0][ItemIndex] )
		{
			NewSocket[0] = 0xFE;

			if ( rand()%10000 < gMossMerchant.SpecialSocketRate[1][ItemIndex] )
			{
				NewSocket[1] = 0xFE;

				if ( rand()%10000 < gMossMerchant.SpecialSocketRate[2][ItemIndex] )
				{
					NewSocket[2] = 0xFE;

					if ( rand()%10000 < gMossMerchant.SpecialSocketRate[3][ItemIndex] )
					{
						NewSocket[3] = 0xFE;

						if ( rand()%10000 < gMossMerchant.SpecialSocketRate[4][ItemIndex] )
						{
							NewSocket[4] = 0xFE;
						}
					}
				}
			}
		}

		GCServerMsgStringSend("[MossMerchant]: Congratulations! You have received Unique Item",aIndex,1);
	}
	else
	{
		NewItem = ITEMGET(gMossMerchant.Type[ItemIndex][Random],gMossMerchant.Index[ItemIndex][Random]);
	}

	if ( IsAnc == 1 )
	{
		NewSkill = 1;
	}

	if ( IsExe == 1 )
	{
		NewSkill = 1;
	}

	NewDur = ItemGetDurability(NewItem,NewLevel,IsExe,IsAnc);

	ItemSerialCreateSend(aIndex,236,0,0,NewItem,NewLevel,NewDur,NewSkill,NewLuck,NewOpt,aIndex,NewExe,NewAnc,NewSocket);

	lpObj->Money -= 1000000;
	GCMoneySend(aIndex,lpObj->Money);

	gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.size);

	return;
}

void MossEventRun(void * lpParam)
{
	while ( true )
	{
		if ( gMossMerchant.TimeAux <= 0 )
		{
			SendMsg.MessageOutAll(0x00, "[MossMerchant]: Merchant stoped selling unique items");
			gMossMerchant.Running = false;
			gMossMerchant.TimeAux = gMossMerchant.Time*60;
			_endthread();
		}

		gMossMerchant.TimeAux--;

		Sleep(1000);
	}
}