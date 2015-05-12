//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU New Quest System Functions							//
// Coded & Modified By:	M.E.S & WolF									//
//**********************************************************************//

#include "../Header Files/stdafx.h"
#include "../Header Files/SQuest.h"
#include "../Header Files/Defines.h"
#include "../Header Files/GenSystem.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/Notice.H"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/SendProto.H"


SQuest QuestNew;
QUEST_STRUCT QuestsInfo[6];

char QuestsFiles[6][44] =
{
	"..\\Data\\Quests\\ElfSoldierFirstQuest.ini",
	"..\\Data\\Quests\\ElfSoldierSecondQuest.ini",
	"..\\Data\\Quests\\ElfSoldierThirdQuest.ini",
	"..\\Data\\Quests\\GensQuest.ini",
	"..\\Data\\Quests\\TutorialQuest.ini",
	"..\\Data\\Quests\\MercenaryQuest.ini",
};
// --------------------------------------------------------------------------------------
SQuest::SQuest(void)
{
	return;
}
// --------------------------------------------------------------------------------------
SQuest::~SQuest(void)
{
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::Init()
{
	// ----
	QuestNew.Enabled = 1;
	// ----
	for ( int i=0;i<6;i++ )
	{
		for ( int h=0;h<200;h++ )
		{
			QuestsInfo[i].Type[h] = 0;
			QuestsInfo[i].SubType[h] = 0;
			QuestsInfo[i].Repeat[h] = -1;
			QuestsInfo[i].GenType[h] = 0;
			QuestsInfo[i].Number[h] = -1;

			QuestsInfo[i].AgreeType[h] = 0;
			QuestsInfo[i].AgreeSubType[h] = 0;
			QuestsInfo[i].RefuseType[h] = 0;
			QuestsInfo[i].RefuseSubType[h] = 0;
			QuestsInfo[i].MinLevel[h] = 0;
			QuestsInfo[i].MaxLevel[h] = 0;
			QuestsInfo[i].SendInfo[h] = 0;
			QuestsInfo[i].TotalObjCount[h] = 0;
			QuestsInfo[i].TotalRewCount[h] = 0;
			QuestsInfo[i].RequiredClass[0][h] = 0;
			QuestsInfo[i].RequiredClass[1][h] = 0;
			QuestsInfo[i].RequiredClass[2][h] = 0;
			QuestsInfo[i].RequiredClass[3][h] = 0;
			QuestsInfo[i].RequiredClass[4][h] = 0;
			QuestsInfo[i].RequiredClass[5][h] = 0;
			QuestsInfo[i].RequiredClass[6][h] = 0;
			for ( int j=0;j<5;j++ )
			{
				QuestsInfo[i].ObjetiveType[j][h] = 0;
				QuestsInfo[i].ObjMobLvlSk[j][h] = 0;
				QuestsInfo[i].ObjCount[j][h] = 0;
				QuestsInfo[i].ObjItemType[j][h] = 0;
				QuestsInfo[i].ObjItemIndex[j][h] = 0;
				QuestsInfo[i].ObjItemLevel[j][h] = 0;
				QuestsInfo[i].ObjItemDur[j][h] = 0;
				QuestsInfo[i].ObjItemSkill[j][h] = 0;
				QuestsInfo[i].ObjItemLuck[j][h] = 0;
				QuestsInfo[i].ObjItemAddOpt[j][h] = 0;
				QuestsInfo[i].ObjItemExe[j][h] = 0;
				QuestsInfo[i].ObjItemAnc[j][h] = 0;
				QuestsInfo[i].ObjItemSocket[0][j][h] = 0;
				QuestsInfo[i].ObjItemSocket[1][j][h] = 0;
				QuestsInfo[i].ObjItemSocket[2][j][h] = 0;
				QuestsInfo[i].ObjItemSocket[3][j][h] = 0;
				QuestsInfo[i].ObjItemSocket[4][j][h] = 0;

				QuestsInfo[i].RewardType[j][h] = 0;
				QuestsInfo[i].RewZCE[j][h] = 0;
				QuestsInfo[i].RewItemCount[j][h] = 0;
				QuestsInfo[i].RewItemType[j][h] = 0;
				QuestsInfo[i].RewItemIndex[j][h] = 0;
				QuestsInfo[i].RewItemLevel[j][h] = 0;
				QuestsInfo[i].RewItemDur[j][h] = 0;
				QuestsInfo[i].RewItemSkill[j][h] = 0;
				QuestsInfo[i].RewItemLuck[j][h] = 0;
				QuestsInfo[i].RewItemAddOpt[j][h] = 0;
				QuestsInfo[i].RewItemExe[j][h] = 0;
				QuestsInfo[i].RewItemAnc[j][h] = 0;
				QuestsInfo[i].RewItemSocket[0][j][h] = 0;
				QuestsInfo[i].RewItemSocket[1][j][h] = 0;
				QuestsInfo[i].RewItemSocket[2][j][h] = 0;
				QuestsInfo[i].RewItemSocket[3][j][h] = 0;
				QuestsInfo[i].RewItemSocket[4][j][h] = 0;
			}
		}
	}
	// ----
	QuestNew.QuestCount[0] = 0;
	QuestNew.QuestCount[1] = 0;
	QuestNew.QuestCount[2] = 0;
	QuestNew.QuestCount[3] = 0;
	QuestNew.QuestCount[4] = 0;
	QuestNew.QuestCount[5] = 0;

	QuestNew.ReadQuestsFile(0);
	QuestNew.ReadQuestsFile(1);
	QuestNew.ReadQuestsFile(2);
	QuestNew.ReadQuestsFile(3);
	QuestNew.ReadQuestsFile(4);
	QuestNew.ReadQuestsFile(5);

	return;
}
// --------------------------------------------------------------------------------------
void SQuest::ReadQuestsFile(char Type)
{
	SMDFile = fopen(QuestsFiles[Type], "r");

	if ( SMDFile == NULL )
	{
		return;
	}

	int Token;
	int iType = -1;
	int Index = 0;

	while ( true )
	{
		Token = GetToken();

        if( Token == 2)
		{
            break;
		}

		Index = 0;
		iType = TokenNumber;

		while ( true )
		{
			if( iType == 0 )	// Quest Info
			{
				Token = GetToken();

				if ( strcmp("end", TokenString ) == 0)
				{
					break;
				}

				Index = TokenNumber;

				if ( Type == 1 )
				{
					Token = GetToken();
					QuestsInfo[Type].Repeat[Index] = TokenNumber;
				}
				else if ( Type == 2 )
				{
					Token = GetToken();
					QuestsInfo[Type].Number[Index] = TokenNumber;
				}
				else if ( Type == 3 )
				{
					Token = GetToken();
					QuestsInfo[Type].GenType[Index] = TokenNumber;
				}
				else if ( Type == 5 )
				{
					Token = GetToken();
					QuestsInfo[Type].NPCNumber[Index] = TokenNumber;

					Token = GetToken();
					QuestsInfo[Type].Register[Index] = TokenNumber;

					Token = GetToken();
					QuestsInfo[Type].StartDayWeek[Index] = TokenNumber;

					Token = GetToken();
					QuestsInfo[Type].EndDayWeek[Index] = TokenNumber;
				}

				Token = GetToken();
				QuestsInfo[Type].Type[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].SubType[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].AgreeType[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].AgreeSubType[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RefuseType[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RefuseSubType[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].MinLevel[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].MaxLevel[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].SendInfo[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].TotalObjCount[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].TotalRewCount[Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RequiredClass[0][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RequiredClass[1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RequiredClass[2][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RequiredClass[3][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RequiredClass[4][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RequiredClass[5][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RequiredClass[6][Index] = TokenNumber;

				QuestNew.QuestCount[Type]++;
			}
			else if( iType >= 1 && iType <= 5 )	// Quest Objectives
			{
				Token = GetToken();

				if ( strcmp("end", TokenString ) == 0)
				{
					break;
				}
				Index = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjetiveType[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjMobLvlSk[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjCount[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemType[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemIndex[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemLevel[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemDur[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemSkill[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemLuck[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemAddOpt[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemExe[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemAnc[iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemSocket[0][iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemSocket[1][iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemSocket[2][iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemSocket[3][iType-1][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].ObjItemSocket[4][iType-1][Index] = TokenNumber;

			}
			else if( iType >= 6 && iType <= 10 )	// Quest Rewards
			{
				Token = GetToken();

				if ( strcmp("end", TokenString ) == 0)
				{
					break;
				}

				Index = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewardType[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewZCE[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemCount[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemType[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemIndex[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemLevel[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemDur[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemSkill[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemLuck[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemAddOpt[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemExe[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemAnc[iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemSocket[0][iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemSocket[1][iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemSocket[2][iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemSocket[3][iType-6][Index] = TokenNumber;

				Token = GetToken();
				QuestsInfo[Type].RewItemSocket[4][iType-6][Index] = TokenNumber;
			}
		}
	}
	CLog.LogAdd("[QuestSystem]: Read File -> %s loaded. Quest Count %d",QuestsFiles[Type],QuestNew.QuestCount[Type]);
	fclose(SMDFile);
}
// --------------------------------------------------------------------------------------
int SQuest::GetQuestIndex(int aIndex,unsigned char Type,unsigned char SubType, char QType)
{
	for ( int i=0;i<QuestNew.QuestCount[QType];i++ )
	{
		if ( QuestsInfo[QType].RequiredClass[gObj[aIndex].Class][i] == 1 )
		{
			if ( gObj[aIndex].Level >= QuestsInfo[QType].MinLevel[i] && gObj[aIndex].Level <= QuestsInfo[QType].MaxLevel[i] )
			{
				if ( QuestsInfo[QType].Type[i] == Type && QuestsInfo[QType].SubType[i] == SubType )
				{
					return i;
				}
			}
		}
	}
	return -1;
}
// --------------------------------------------------------------------------------------
int SQuest::GetQuestIndex2(int aIndex,unsigned char Type,unsigned char SubType, char QType)
{
	for ( int i=0;i<QuestNew.QuestCount[QType];i++ )
	{
		if ( QuestsInfo[QType].RequiredClass[gObj[aIndex].Class][i] == 1 )
		{
			if ( gObj[aIndex].Level >= QuestsInfo[QType].MinLevel[i] && gObj[aIndex].Level <= QuestsInfo[QType].MaxLevel[i] )
			{
				if ( QuestsInfo[QType].AgreeType[i] == Type && QuestsInfo[QType].AgreeSubType[i] == SubType )
				{
					return i;
				}
			}
		}
	}
	return -1;
}
// --------------------------------------------------------------------------------------
char SQuest::ReturnQuestPos(char QuestType)
{
	char Value = -1;

	switch ( QuestType )
	{
	case 18:
		Value = 0;
		break;
	case 15:
		Value = 1;
		break;
	case 19:
		Value = 2;
		break;
	case 16:
	case 17:
		Value = 3;
		break;
	case 20:
		Value = 5;
		break;
	}

	return Value;
}
// --------------------------------------------------------------------------------------
void SQuest::ProtocolManager(unsigned char * aRecv,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	switch ( aRecv[3] )
	{
		// --- Quest Select
	case 0x0A:
		QuestNew.SelectQuest(aRecv,aIndex);
		CheckQuest(aIndex);
		break;
		// --- Quest Agree or Refuse
	case 0x0B:
		//if ( lpObj->QuestNPC == 566 ) // Will Make New Func For Daily Quest
		QuestNew.QuestAnswer(aRecv,aIndex);
		CheckQuest(aIndex);
		break;
	case 0x0D:
		QuestNew.QuestFinish(aRecv,aIndex);
		CheckQuest(aIndex);
		break;
		// --- NPC Talk Quests
	case 0x30:
		if ( lpObj->QuestNPC == 257 )
		{
			QuestNew.ElfSoldierQuest(aIndex);
		}
		else if ( lpObj->QuestNPC == 566 )
		{
			QuestNew.GyroRogerTraders(aIndex);
		}
		else if ( lpObj->QuestNPC == 567 )
		{
			QuestNew.OracleLayla(aIndex);
		}
		else if ( lpObj->QuestNPC == 568 )
		{
			QuestNew.PorterMercenary(aIndex);
		}
		else if ( lpObj->QuestNPC == 543 || lpObj->QuestNPC == 544 )
		{
			QuestNew.Gens(aIndex);
		}
		CheckQuest(aIndex);
		break;
		// --- Quest Give Up
	case 0x0F:
		QuestNew.GiveUpQuest(aRecv,aIndex);
		CheckQuest(aIndex);
		break;
		// --- T Click
	case 0x1B:
		QuestNew.TClick(aRecv,aIndex);
		break;
		// --- Elf Buff Request
	case 0x31:
		ElfSoldierBuf(lpObj);
		break;
	}
}
// --------------------------------------------------------------------------------------
void SQuest::TClick(unsigned char * aRecv,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	char QuestType = QuestNew.ReturnQuestPos(aRecv[6]);
	int QuestIndex = QuestNew.GetQuestIndex2(aIndex,aRecv[6],aRecv[4],QuestType);
	// ----
	int j = 0;
	// ----
	BYTE AnswerPacket[500] = {0};

	AnswerPacket[0] = 0xC1;
	AnswerPacket[1] = 0xF1;
	AnswerPacket[2] = 0xF6;
	AnswerPacket[3] = 0x1B;
	AnswerPacket[4] = aRecv[4]; // Quest Sub Type
	AnswerPacket[5] = 0; // Quest Sub Type
	AnswerPacket[6] = aRecv[6]; // Quest Type
	AnswerPacket[7] = 0; // Quest Type
	AnswerPacket[8] = QuestsInfo[QuestType].TotalObjCount[QuestIndex]; // Quest Objectives Count
	AnswerPacket[9] = QuestsInfo[QuestType].TotalRewCount[QuestIndex]; // Quest Rewards Count
	AnswerPacket[10] = 1;
	// ---- OBJECTIVES
	BYTE ObjPack = 0x00;
	BYTE RewPack = 0x00;

	for (int n=0;n<50;n++)
	{
		if ( lpObj->NewQuestInfo[n*12] == aRecv[6] && lpObj->NewQuestInfo[(n*12)+1] == aRecv[4] )
		{
			j = n*12;
			break;
		}
	}
	// -----------------------------------------------------
	// OBJECTIVES
	// -----------------------------------------------------
	for ( int i=0;i<QuestsInfo[QuestType].TotalObjCount[QuestIndex];i++)
	{
		if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 1 )
		{
			// ------------------------------
			// MONSTER OBJECTIVE
			// ------------------------------
			AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
			AnswerPacket[15+ObjPack] = LOBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
			AnswerPacket[16+ObjPack] = HIBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
			AnswerPacket[17+ObjPack] = QuestsInfo[QuestType].ObjCount[i][QuestIndex];
			AnswerPacket[21+ObjPack] = (lpObj->NewQuestInfo[j+3+(2*i)]*256) + lpObj->NewQuestInfo[j+2+(2*i)];
			ObjPack += 26;
		}
		else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 2 )
		{
			// ------------------------------
			// SKILL OBJECTIVE
			// ------------------------------
			AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
			AnswerPacket[15+ObjPack] = LOBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
			AnswerPacket[16+ObjPack] = HIBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
			if ( gObjCheckMagic(lpObj,QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]) == false )
			{
				AnswerPacket[21+ObjPack] = 0;
			}
			else
			{
				AnswerPacket[21+ObjPack] = 1;
			}
			ObjPack += 26;
		}
		else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 16 )
		{
			// ------------------------------
			// MENU OBJECTIVE
			// ------------------------------
			AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
			AnswerPacket[15+ObjPack] = 0;
			AnswerPacket[16+ObjPack] = 0;
			AnswerPacket[17+ObjPack] = 0;
			AnswerPacket[21+ObjPack] = 0;	// Completed
			ObjPack += 26;
		}
		else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 8 )
		{
			// ------------------------------
			// LEVEL OBJECTIVE
			// ------------------------------
			AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
			AnswerPacket[17+ObjPack] = LOBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
			AnswerPacket[18+ObjPack] = HIBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
			if ( lpObj->Level < QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex] )
			{
				AnswerPacket[21+ObjPack] = 0;
			}
			else
			{
				AnswerPacket[21+ObjPack] = 1;
			}
			ObjPack += 26;
		}
		else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 4 )
		{
			// ------------------------------
			// ITEM OBJECTIVE
			// ------------------------------
			int ItemID = 0;
			int Option1 = 0;
			int Option2 = 0;
			int ItemCount = 0;
			unsigned char Durability = 0;
			char IsExe = 0;
			char IsAnc = 0;

			if ( QuestsInfo[QuestType].ObjItemExe[i][QuestIndex] > 0 )
			{
				IsExe = 1;
			}

			if ( QuestsInfo[QuestType].ObjItemAnc[i][QuestIndex] > 0 )
			{
				IsAnc = 1;
			}
			
			ItemCount = gObjGetCompleteItemCountInIventory(aIndex,QuestsInfo[QuestType].ObjItemType[i][QuestIndex],QuestsInfo[QuestType].ObjItemIndex[i][QuestIndex],
				QuestsInfo[QuestType].ObjItemLevel[i][QuestIndex],QuestsInfo[QuestType].ObjItemLuck[i][QuestIndex],QuestsInfo[QuestType].ObjItemSkill[i][QuestIndex],
				QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex],QuestsInfo[QuestType].ObjItemExe[i][QuestIndex]);

			ItemID = (QuestsInfo[QuestType].ObjItemType[i][QuestIndex]*512) + QuestsInfo[QuestType].ObjItemIndex[i][QuestIndex];
			Option1 = (QuestsInfo[QuestType].ObjItemLuck[i][QuestIndex]*4);
			Option1 += (QuestsInfo[QuestType].ObjItemSkill[i][QuestIndex]*128);
			Option1 += (QuestsInfo[QuestType].ObjItemLevel[i][QuestIndex]*8);

			if ( QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex] > 3 )
			{
				Option1 += 3;
				int AuxOpt = QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex]-3;
				for (int j=0;j<AuxOpt;j++)
				{
					Option2+=16;
				}
			}
			else
			{
				Option1 += (QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex]);
			}

			Option2 += (QuestsInfo[QuestType].ObjItemExe[i][QuestIndex]);

			if ( QuestsInfo[QuestType].ObjItemDur[i][QuestIndex] <= 0 )
			{
				Durability = ItemGetDurability(ItemID,QuestsInfo[QuestType].ObjItemLevel[i][QuestIndex],IsExe,IsAnc);
			}
			else
			{
				Durability = QuestsInfo[QuestType].ObjItemDur[i][QuestIndex];
			}

			AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
			AnswerPacket[15+ObjPack] = 0;
			AnswerPacket[16+ObjPack] = 0;
			AnswerPacket[17+ObjPack] = QuestsInfo[QuestType].ObjCount[i][QuestIndex];
			AnswerPacket[18+ObjPack] = 0;
			AnswerPacket[19+ObjPack] = 0;
			AnswerPacket[20+ObjPack] = 0;
			AnswerPacket[21+ObjPack] = ItemCount;
			AnswerPacket[25+ObjPack] = QuestsInfo[QuestType].ObjItemIndex[i][QuestIndex];
			AnswerPacket[26+ObjPack] = Option1;	// Add + Luck + Skill + Level
			AnswerPacket[27+ObjPack] = Durability;
			AnswerPacket[28+ObjPack] = Option2;	// Add + Exe
			AnswerPacket[29+ObjPack] = QuestsInfo[QuestType].ObjItemAnc[i][QuestIndex];
			AnswerPacket[30+ObjPack] |= (ItemID & 0x1E00 ) >> 5;
			AnswerPacket[31+ObjPack] |= (0 & 0x80 ) >> 4;
			//AnswerPacket[32+ObjPack] = 0xFE;
			//AnswerPacket[33+ObjPack] = 0xFE;
			//AnswerPacket[34+ObjPack] = 0xFE;
			//AnswerPacket[35+ObjPack] = 0xFE;
			//AnswerPacket[36+ObjPack] = 0xFE;
			ObjPack += 26;
		}
		else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 32 )
		{
			// ------------------------------
			// BONUS OBJECTIVE
			// ------------------------------
			AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];	// Bonus
			AnswerPacket[12+ObjPack] = 0;
			AnswerPacket[13+ObjPack] = 0;
			AnswerPacket[14+ObjPack] = 0;
			AnswerPacket[15+ObjPack] = 0;
			AnswerPacket[16+ObjPack] = 0;
			AnswerPacket[17+ObjPack] = 0;
			AnswerPacket[21+ObjPack] = 1;	// Completed
			ObjPack += 26;
		}
		else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 64 )
		{
			// ------------------------------
			// CHAOS CASTLE OBJECTIVE
			// ------------------------------
			AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];	// Chaos Castle
			AnswerPacket[15+ObjPack] = 1;	// LOBYTE (Chaos Castle Level)
			AnswerPacket[16+ObjPack] = 0;	// HIBYTE (Chaos Castle Level)

			AnswerPacket[17+ObjPack] = 3;	// LOBYTE LOWORD (Total Players)
			AnswerPacket[18+ObjPack] = 0;	// HIBYTE LOWORD (Total Players)
			AnswerPacket[19+ObjPack] = 0;	// LOBYTE HIWORD (Total Players)
			AnswerPacket[20+ObjPack] = 0;	// HIBYTE HIWORD (Total Players)

			AnswerPacket[21+ObjPack] = 2;	// LOBYTE LOWORD (Killed Players)
			AnswerPacket[22+ObjPack] = 0;	// HIBYTE LOWORD (Killed Players)
			AnswerPacket[23+ObjPack] = 0;	// LOBYTE HIWORD (Killed Players)
			AnswerPacket[24+ObjPack] = 0;	// HIBYTE HIWORD (Killed Players)
			ObjPack += 26;
		}
	}
	// -----------------------------------------------------
	// REWARDS
	// -----------------------------------------------------
	for (int i=0;i<QuestsInfo[QuestType].TotalRewCount[QuestIndex];i++)
	{
		if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 1 )
		{
			// ------------------------------
			// EXPERIENCE REWARD
			// ------------------------------
			AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
			AnswerPacket[147+RewPack] = LOBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[148+RewPack] = HIBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[149+RewPack] = LOBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[150+RewPack] = HIBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			RewPack+=22;
		}
		else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 2 )
		{
			// ------------------------------
			// ZEN REWARD
			// ------------------------------
			AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];	// Zen
			AnswerPacket[147+RewPack] = LOBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[148+RewPack] = HIBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[149+RewPack] = LOBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[150+RewPack] = HIBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			RewPack+=22;
		}
		else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 4 )
		{
			// ------------------------------
			// ITEM REWARD
			// ------------------------------
			int ItemID = 0;
			int Option1 = 0;
			int Option2 = 0;
			unsigned char Durability;
			char IsExe;
			char IsAnc;

			if ( QuestsInfo[QuestType].RewItemExe[i][QuestIndex] > 0 ) 
			{
				IsExe = 1;
			}

			if ( QuestsInfo[QuestType].RewItemAnc[i][QuestIndex] > 0 ) 
			{
				IsAnc = 1;
			}

			ItemID = (QuestsInfo[QuestType].RewItemType[i][QuestIndex]*512) + QuestsInfo[QuestType].RewItemIndex[i][QuestIndex];

			Option1 = (QuestsInfo[QuestType].RewItemLuck[i][QuestIndex]*4);
			Option1 += (QuestsInfo[QuestType].RewItemSkill[i][QuestIndex]*128);
			Option1 += (QuestsInfo[QuestType].RewItemLevel[i][QuestIndex]*8);

			if ( QuestsInfo[QuestType].RewItemAddOpt[i][QuestIndex] > 3 )
			{
				Option1 += 3;
				int AuxOpt = QuestsInfo[QuestType].RewItemAddOpt[i][QuestIndex]-3;
				for (int j=0;j<AuxOpt;j++)
				{
					Option2+=16;
				}
			}
			else
			{
				Option1 += (QuestsInfo[QuestType].RewItemAddOpt[i][QuestIndex]);
			}

			Option2 += (QuestsInfo[QuestType].RewItemExe[i][QuestIndex]);

			if ( QuestsInfo[QuestType].RewItemDur[i][QuestIndex] <= 0 )
			{
				Durability = ItemGetDurability(ItemID,QuestsInfo[QuestType].RewItemLevel[i][QuestIndex],IsExe,IsAnc);
			}
			else
			{
				Durability = QuestsInfo[QuestType].RewItemDur[i][QuestIndex];
			}

			AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
			AnswerPacket[147+RewPack] = QuestsInfo[QuestType].RewItemCount[i][QuestIndex];
			AnswerPacket[151+RewPack] = QuestsInfo[QuestType].RewItemIndex[i][QuestIndex];
			AnswerPacket[152+RewPack] = Option1;
			AnswerPacket[153+RewPack] = Durability;
			AnswerPacket[154+RewPack] = Option2;
			AnswerPacket[155+RewPack] = QuestsInfo[QuestType].RewItemAnc[i][QuestIndex];	// Ancient
			AnswerPacket[156+RewPack] |= (ItemID & 0x1E00 ) >> 5;
			AnswerPacket[156+RewPack] |= (0 & 0x80 ) >> 4;
			//AnswerPacket[157+RewPack] = 0xFF;
			//AnswerPacket[158+RewPack] = 0xFF;
			//AnswerPacket[159+RewPack] = 0xFF;
			//AnswerPacket[160+RewPack] = 0xFF;
			//AnswerPacket[161+RewPack] = 0xFF;
			RewPack+=22;
		}
		else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 8 )
		{
			// ------------------------------
			// BONUS REWARD
			// ------------------------------
			AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
			RewPack+=22;
		}
		else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 16 )
		{
			// ------------------------------
			// CONTRIBUTION REWARD
			// ------------------------------
			AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
			AnswerPacket[147+RewPack] = LOBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[148+RewPack] = HIBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[149+RewPack] = LOBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			AnswerPacket[150+RewPack] = HIBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
			RewPack+=22;
		}
		else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 32 )
		{
			// ------------------------------
			// RANDOM REWARD
			// ------------------------------
			AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
			RewPack+=22;
		}
	}

	gSendProto.DataSend(aIndex,AnswerPacket,AnswerPacket[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::ElfSoldierQuest(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	BYTE Packet[200] = {0};
	bool AddQuest = true;
	// ----
	Packet[0] = 0xC1;
	Packet[1] = 0x08;
	Packet[2] = 0xF6;
	Packet[3] = 0x0A;
	Packet[4] = 0x01;
	Packet[5] = 0x01;
	Packet[6] = 0x00;
	Packet[7] = 0x00;

	if ( QuestNew.Enabled == 1 )
	{
		for ( int i=0;i<3;i++ )
		{
			for ( int j=0;j<QuestNew.QuestCount[i];j++ )
			{
				for ( int h=0;h<50;h++ )
				{
					if ( lpObj->NewQuestInfo[h*12] == QuestsInfo[i].AgreeType[j] && lpObj->NewQuestInfo[(h*12)+1] == QuestsInfo[i].AgreeSubType[j] )
					{
						if ( QuestsInfo[i].RequiredClass[lpObj->Class][j] == 1 )
						{
							AddQuest = false;
							Packet[1] += 0x04;
							Packet[6] += 0x01;
							Packet[Packet[1]-0x02] = lpObj->NewQuestInfo[h*12];
							Packet[Packet[1]-0x04] = lpObj->NewQuestInfo[(h*12)+1];
							break;
						}
					}
				}
				if ( AddQuest == true )
				{
					if ( QuestsInfo[i].RequiredClass[lpObj->Class][j] == 1 )
					{
						if ( lpObj->Level >= QuestsInfo[i].MinLevel[j] && lpObj->Level <= QuestsInfo[i].MaxLevel[j] )
						{
							Packet[1] += 0x04;
							Packet[6] += 0x01;
							Packet[Packet[1]-0x02] = QuestsInfo[i].Type[j];
							Packet[Packet[1]-0x04] = QuestsInfo[i].SubType[j];				
						}
					}
				}
				AddQuest = true;
			}
		}
	}
	// ----
	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::Gens(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	bool AddQuest = true;
	// ----
	if (lpObj->GenFamily == 0)
	{
		BYTE Packet[5] = {0xC1, 0x05, 0xF8, 0x0A, 0x06}; // You have not Joined The Gens
		gSendProto.DataSend (lpObj->m_Index,Packet , Packet[1]);
		return;
	}
	else if ( (lpObj->GenFamily == 1 && lpObj->GenNPC == 544 ) || (lpObj->GenFamily == 2 && lpObj->GenNPC == 543))
	{
		BYTE Packet[5] = {0xc1, 0x05, 0xF8, 0x04, 0x03}; // You are a member of the different Gens
		gSendProto.DataSend(lpObj->m_Index, Packet, Packet[1]);
		return;
	}
	BYTE Packet[100]= {0xC1, 0x08, 0xF6, 0x0A, 0x1F, 0x02, 0x00, 0x00};

	if ( QuestNew.Enabled == 1 )
	{
		for ( int j=0;j<QuestNew.QuestCount[3];j++ )
		{
			for ( int h=0;h<50;h++ )
			{
				if ( lpObj->NewQuestInfo[h*12] == QuestsInfo[3].AgreeType[j] && lpObj->NewQuestInfo[(h*12)+1] == QuestsInfo[3].AgreeSubType[j] )
				{
					if ( QuestsInfo[3].RequiredClass[lpObj->Class][j] == 1 )
					{
						AddQuest = false;
						Packet[1] += 0x04;
						Packet[6] += 0x01;
						Packet[Packet[1]-0x02] = lpObj->NewQuestInfo[h*12];
						Packet[Packet[1]-0x04] = lpObj->NewQuestInfo[(h*12)+1];
						break;
					}
				}
			}
			if ( AddQuest == true )
			{
				if ( lpObj->GenFamily == QuestsInfo[3].GenType[j] )
				{
					if ( QuestsInfo[3].RequiredClass[lpObj->Class][j] == 1 )
					{
						if ( lpObj->Level >= QuestsInfo[3].MinLevel[j] && lpObj->Level <= QuestsInfo[3].MaxLevel[j] )
						{
							Packet[1] += 0x04;
							Packet[6] += 0x01;
							Packet[Packet[1]-0x02] = QuestsInfo[3].Type[j];
							Packet[Packet[1]-0x04] = QuestsInfo[3].SubType[j];				
						}
					}
				}
			}
			AddQuest = true;
		}
	}

	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
bool SQuest::UseQuestScroll(int aIndex, char Type)
{
	LPOBJ lpObj = &gObj[aIndex];
	bool Added = false;
	// ----
	for (int h=0;h<50;h++)
	{
		if ( (lpObj->NewQuestInfo[h*12] == 20 && lpObj->NewQuestInfo[(h*12)+1] == 8 ) )
		{
			return false;
		}
	}
	for (int n=0;n<50;n++)
	{
		if ( (lpObj->NewQuestInfo[n*12] == 0 && lpObj->NewQuestInfo[(n*12)+1] == 0) || (lpObj->NewQuestInfo[n*12] == 0xFF && lpObj->NewQuestInfo[(n*12)+1] == 0xFF) )
		{
			lpObj->NewQuestInfo[n*12] = 20;
			lpObj->NewQuestInfo[(n*12)+1] = 8;
			lpObj->NewQuestInfo[(n*12)+2] = 0;
			lpObj->NewQuestInfo[(n*12)+3] = 0;
			lpObj->NewQuestInfo[(n*12)+4] = 0;
			lpObj->NewQuestInfo[(n*12)+5] = 0;
			lpObj->NewQuestInfo[(n*12)+6] = 0;
			lpObj->NewQuestInfo[(n*12)+7] = 0;
			lpObj->NewQuestInfo[(n*12)+8] = 0;
			lpObj->NewQuestInfo[(n*12)+9] = 0;
			lpObj->NewQuestInfo[(n*12)+10] = 0;
			lpObj->NewQuestInfo[(n*12)+11] = 0;
			break;
		}
	}
	return true;
}
// --------------------------------------------------------------------------------------
void SQuest::GyroRogerTraders(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	bool AddQuest = true;
	BYTE Packet[200] = {0};
	SYSTEMTIME t;
	GetLocalTime(&t);

	// ----
	Packet[0] = 0xC1;
	Packet[1] = 0x08;
	Packet[2] = 0xF6;
	Packet[3] = 0x0A;
	Packet[4] = 0x01;
	Packet[5] = 0x01;
	Packet[6] = 0x00;
	Packet[7] = 0x00;

	for ( int i=0;i<QuestNew.QuestCount[5];i++ )
	{
		if ( lpObj->Level >= QuestsInfo[5].MinLevel[i] && lpObj->Level <= QuestsInfo[5].MaxLevel[i] )
		{
			if ( QuestsInfo[5].RequiredClass[lpObj->Class][i] == 1 && QuestsInfo[5].NPCNumber[i] == 566 )
			{
				if ( QuestsInfo[5].Register[i] == 1 )
				{
					if ( lpObj->QuestRegistered == 0 )
					{
						Packet[1] += 0x04;
						Packet[6] += 0x01;
						Packet[Packet[1]-0x02] = QuestsInfo[5].Type[i];
						Packet[Packet[1]-0x04] = QuestsInfo[5].SubType[i];
						break;
					}
				}
				else if ( QuestsInfo[5].Register[i] == 0 )
				{
					if ( lpObj->QuestRegistered == 1 )
					{
						if ( QuestsInfo[5].StartDayWeek[i] == t.wDayOfWeek )
						{
							Packet[1] += 0x04;
							Packet[6] += 0x01;
							Packet[Packet[1]-0x02] = QuestsInfo[5].Type[i];
							Packet[Packet[1]-0x04] = QuestsInfo[5].SubType[i];
						}
					}
				}
			}
		}
	}
		
	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::OracleLayla(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE Packet[200] = {0};

	// ----
	Packet[0] = 0xC1;
	Packet[1] = 0x08;
	Packet[2] = 0xF6;
	Packet[3] = 0x0A;
	Packet[4] = 0x01;
	Packet[5] = 0x01;
	Packet[6] = 0x00;
	Packet[7] = 0x00;

	if ( lpObj->QuestRegistered == 1 )
	{
	}

	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::PorterMercenary(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE Packet[200] = {0};

	// ----
	Packet[0] = 0xC1;
	Packet[1] = 0x08;
	Packet[2] = 0xF6;
	Packet[3] = 0x0A;
	Packet[4] = 0x01;
	Packet[5] = 0x01;
	Packet[6] = 0x00;
	Packet[7] = 0x00;

	if ( lpObj->QuestRegistered == 1 )
	{
	}

	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::IsRegister(int aIndex,unsigned char Type,unsigned char SubType, char QType)
{
	if ( QType != 5 )
	{
		return;
	}
	for ( int i=0;i<QuestNew.QuestCount[QType];i++ )
	{
		if ( QuestsInfo[QType].Type[i] == Type && QuestsInfo[QType].SubType[i] == SubType )
		{
			if ( QuestsInfo[QType].Register[i] == 3 && gObj[aIndex].QuestRegistered == 0 )
			{
				gObj[aIndex].QuestRegistered = 1;
				break;
			}
		}
	}
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::SelectQuest(unsigned char * aRecv,int aIndex)
{
	BYTE Packet[11] = { 0xC1, 0x0B, 0xF6, 0x0B, aRecv[4], 0x00, aRecv[6], 0x00, 0x00, 0x00, 0x00 };
	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::QuestAnswer(unsigned char * aRecv,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	char QuestType = QuestNew.ReturnQuestPos(aRecv[6]);
	int QuestIndex = QuestNew.GetQuestIndex(aIndex,aRecv[6],aRecv[4],QuestType);
	// Agree = 2 Refuse
	// Agree = 1 Accept
	BYTE Agree = aRecv[8];
	// ----
	BYTE AnswerPacket[500] = {0};
	// --- Quest Agree
	AnswerPacket[0] = 0xC1;
	AnswerPacket[1] = 0xF1;
	AnswerPacket[2] = 0xF6;
	AnswerPacket[3] = 0x0C;
	// ---- OBJECTIVES
	BYTE ObjPack = 0x00;
	BYTE RewPack = 0x00;
	BYTE j = 0;
	bool CanAddQuest = false;
	bool Added = false;

	if ( Agree == 2 )
	{
		AnswerPacket[4] = QuestsInfo[QuestType].RefuseSubType[QuestIndex];
		AnswerPacket[6] = QuestsInfo[QuestType].RefuseType[QuestIndex];
		AnswerPacket[8] = 0;
		AnswerPacket[9] = 0;
		AnswerPacket[10] = 0;
	}
	else if ( Agree == 1 )
	{
		IsRegister(lpObj->m_Index,aRecv[6],aRecv[4],QuestType);
		// ----
		AnswerPacket[4] = QuestsInfo[QuestType].AgreeSubType[QuestIndex]; // Quest Sub Type
		AnswerPacket[5] = 0;
		AnswerPacket[6] = QuestsInfo[QuestType].AgreeType[QuestIndex]; // Quest Type
		AnswerPacket[7] = 0;
		AnswerPacket[8] = QuestsInfo[QuestType].TotalObjCount[QuestIndex];
		AnswerPacket[9] = QuestsInfo[QuestType].TotalRewCount[QuestIndex];
		AnswerPacket[10] = 1;

		for (int h=0;h<50;h++)
		{
			if ( (lpObj->NewQuestInfo[h*12] == QuestsInfo[QuestType].AgreeType[QuestIndex] && lpObj->NewQuestInfo[(h*12)+1] == QuestsInfo[QuestType].AgreeSubType[QuestIndex] ) )
			{
				j = h*12;
				Added = true;
				break;
			}
		}
		if (Added == false )
		{
			for (int n=0;n<50;n++)
			{
				if ( (lpObj->NewQuestInfo[n*12] == 0 && lpObj->NewQuestInfo[(n*12)+1] == 0) || (lpObj->NewQuestInfo[n*12] == 0xFF && lpObj->NewQuestInfo[(n*12)+1] == 0xFF) )
				{
					j = n*12;
					lpObj->NewQuestInfo[n*12] = QuestsInfo[QuestType].AgreeType[QuestIndex];
					lpObj->NewQuestInfo[(n*12)+1] = QuestsInfo[QuestType].AgreeSubType[QuestIndex];
					lpObj->NewQuestInfo[(n*12)+2] = 0;
					lpObj->NewQuestInfo[(n*12)+3] = 0;
					lpObj->NewQuestInfo[(n*12)+4] = 0;
					lpObj->NewQuestInfo[(n*12)+5] = 0;
					lpObj->NewQuestInfo[(n*12)+6] = 0;
					lpObj->NewQuestInfo[(n*12)+7] = 0;
					lpObj->NewQuestInfo[(n*12)+8] = 0;
					lpObj->NewQuestInfo[(n*12)+9] = 0;
					lpObj->NewQuestInfo[(n*12)+10] = 0;
					lpObj->NewQuestInfo[(n*12)+11] = 0;
					break;
				}
			}
		}
		// -----------------------------------------------------
		// OBJECTIVES
		// -----------------------------------------------------
		for ( int i=0;i<QuestsInfo[QuestType].TotalObjCount[QuestIndex];i++)
		{
			if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 1 )
			{
				// ------------------------------
				// MONSTER OBJECTIVE
				// ------------------------------
				AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
				AnswerPacket[15+ObjPack] = LOBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
				AnswerPacket[16+ObjPack] = HIBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
				AnswerPacket[17+ObjPack] = QuestsInfo[QuestType].ObjCount[i][QuestIndex];
				AnswerPacket[21+ObjPack] = (lpObj->NewQuestInfo[j+3+(2*i)]*256) + lpObj->NewQuestInfo[j+2+(2*i)];
				ObjPack += 26;
			}
			else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 2 )
			{
				// ------------------------------
				// SKILL OBJECTIVE
				// ------------------------------
				AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
				AnswerPacket[15+ObjPack] = LOBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
				AnswerPacket[16+ObjPack] = HIBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
				if ( gObjCheckMagic(lpObj,QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]) == false )
				{
					AnswerPacket[21+ObjPack] = 0;
				}
				else
				{
					AnswerPacket[21+ObjPack] = 1;
				}
				ObjPack += 26;
			}
			else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 16 )
			{
				// ------------------------------
				// MENU OBJECTIVE
				// ------------------------------
				AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
				AnswerPacket[15+ObjPack] = 0;
				AnswerPacket[16+ObjPack] = 0;
				AnswerPacket[17+ObjPack] = 0;
				AnswerPacket[21+ObjPack] = 0;	// Completed
				ObjPack += 26;
			}
			else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 8 )
			{
				// ------------------------------
				// LEVEL OBJECTIVE
				// ------------------------------
				AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];
				AnswerPacket[17+ObjPack] = LOBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
				AnswerPacket[18+ObjPack] = HIBYTE(QuestsInfo[QuestType].ObjMobLvlSk[i][QuestIndex]);
				if ( lpObj->Level < QuestsInfo[QuestType].ObjCount[i][QuestIndex] )
				{
					AnswerPacket[21+ObjPack] = 0;
				}
				else
				{
					AnswerPacket[21+ObjPack] = 1;
				}
				ObjPack += 26;
			}
			else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 4 )
			{
				// ------------------------------
				// ITEM OBJECTIVE
				// ------------------------------
				int ItemID = 0;
				int Option1 = 0;
				int Option2 = 0;
				int ItemCount = 0;
				char IsExe = 0;
				char IsAnc = 0;
				unsigned char Durability;

				if ( QuestsInfo[QuestType].ObjItemExe[i][QuestIndex] > 0 )
				{
					IsExe = 1;
				}

				if ( QuestsInfo[QuestType].ObjItemAnc[i][QuestIndex] > 0 )
				{
					IsAnc = 1;
				}

				ItemCount = gObjGetCompleteItemCountInIventory(aIndex,QuestsInfo[QuestType].ObjItemType[i][QuestIndex], QuestsInfo[QuestType].ObjItemIndex[i][QuestIndex],
				QuestsInfo[QuestType].ObjItemLevel[i][QuestIndex],QuestsInfo[QuestType].ObjItemLuck[i][QuestIndex],QuestsInfo[QuestType].ObjItemSkill[i][QuestIndex],
				QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex],QuestsInfo[QuestType].ObjItemExe[i][QuestIndex]);

				ItemID = (QuestsInfo[QuestType].ObjItemType[i][QuestIndex]*512) + QuestsInfo[QuestType].ObjItemIndex[i][QuestIndex];
				Option1 = (QuestsInfo[QuestType].ObjItemLuck[i][QuestIndex]*4);
				Option1 += (QuestsInfo[QuestType].ObjItemSkill[i][QuestIndex]*128);
				Option1 += (QuestsInfo[QuestType].ObjItemLevel[i][QuestIndex]*8);

				if ( QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex] > 3 )
				{
					Option1 += 3;
					int AuxOpt = QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex]-3;
					for (int j=0;j<AuxOpt;j++)
					{
						Option2+=16;
					}
				}
				else
				{
					Option1 += (QuestsInfo[QuestType].ObjItemAddOpt[i][QuestIndex]);
				}

				Option2 += (QuestsInfo[QuestType].ObjItemExe[i][QuestIndex]);

				if ( QuestsInfo[QuestType].ObjItemDur[i][QuestIndex] <= 0 )
				{
					Durability = ItemGetDurability(ItemID,QuestsInfo[QuestType].ObjItemSkill[i][QuestIndex],IsExe,IsAnc);
				}
				else
				{
					Durability = QuestsInfo[QuestType].ObjItemDur[i][QuestIndex];
				}

				AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];	// Item
				AnswerPacket[15+ObjPack] = 0;
				AnswerPacket[16+ObjPack] = 0;
				AnswerPacket[17+ObjPack] = QuestsInfo[QuestType].ObjCount[i][QuestIndex];	// Total Item Count
				AnswerPacket[18+ObjPack] = 0;
				AnswerPacket[19+ObjPack] = 0;
				AnswerPacket[20+ObjPack] = 0;
				AnswerPacket[21+ObjPack] = ItemCount;	// Actual Item Count
				AnswerPacket[25+ObjPack] = QuestsInfo[QuestType].ObjItemIndex[i][QuestIndex];	// Item Index
				AnswerPacket[26+ObjPack] = Option1;	// Add + Luck + Skill + Level
				AnswerPacket[27+ObjPack] = Durability;	// Durability
				AnswerPacket[28+ObjPack] = Option2;	// Add + Exe
				AnswerPacket[29+ObjPack] = QuestsInfo[QuestType].ObjItemAnc[i][QuestIndex];
				AnswerPacket[30+ObjPack] |= (ItemID & 0x1E00 ) >> 5;
				AnswerPacket[31+ObjPack] |= (0 & 0x80 ) >> 4;
				//AnswerPacket[32+ObjPack] = 0xFE;
				//AnswerPacket[33+ObjPack] = 0xFE;
				//AnswerPacket[34+ObjPack] = 0xFE;
				//AnswerPacket[35+ObjPack] = 0xFE;
				//AnswerPacket[36+ObjPack] = 0xFE;
				ObjPack += 26;
			}
			else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 32 )
			{
				// ------------------------------
				// BONUS OBJECTIVE
				// ------------------------------
				AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];	// Bonus
				AnswerPacket[12+ObjPack] = 0;
				AnswerPacket[13+ObjPack] = 0;
				AnswerPacket[14+ObjPack] = 0;
				AnswerPacket[15+ObjPack] = 0;
				AnswerPacket[16+ObjPack] = 0;
				AnswerPacket[17+ObjPack] = 0;
				AnswerPacket[21+ObjPack] = 1;	// Completed
				ObjPack += 26;
			}
			else if ( QuestsInfo[QuestType].ObjetiveType[i][QuestIndex] == 64 )
			{
				// ------------------------------
				// CHAOS CASTLE OBJECTIVE
				// ------------------------------
				AnswerPacket[11+ObjPack] = QuestsInfo[QuestType].ObjetiveType[i][QuestIndex];	// Chaos Castle
				AnswerPacket[15+ObjPack] = 1;	// LOBYTE (Chaos Castle Level)
				AnswerPacket[16+ObjPack] = 0;	// HIBYTE (Chaos Castle Level)

				AnswerPacket[17+ObjPack] = 3;	// LOBYTE LOWORD (Total Players)
				AnswerPacket[18+ObjPack] = 0;	// HIBYTE LOWORD (Total Players)
				AnswerPacket[19+ObjPack] = 0;	// LOBYTE HIWORD (Total Players)
				AnswerPacket[20+ObjPack] = 0;	// HIBYTE HIWORD (Total Players)

				AnswerPacket[21+ObjPack] = 2;	// LOBYTE LOWORD (Killed Players)
				AnswerPacket[22+ObjPack] = 0;	// HIBYTE LOWORD (Killed Players)
				AnswerPacket[23+ObjPack] = 0;	// LOBYTE HIWORD (Killed Players)
				AnswerPacket[24+ObjPack] = 0;	// HIBYTE HIWORD (Killed Players)
				ObjPack += 26;
			}
		}
		// -----------------------------------------------------
		// REWARDS
		// -----------------------------------------------------
		for (int i=0;i<QuestsInfo[QuestType].TotalRewCount[QuestIndex];i++)
		{
			if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 1 )
			{
				// ------------------------------
				// EXPERIENCE REWARD
				// ------------------------------
				AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
				AnswerPacket[147+RewPack] = LOBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[148+RewPack] = HIBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[149+RewPack] = LOBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[150+RewPack] = HIBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				RewPack+=22;
			}
			else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 2 )
			{
				// ------------------------------
				// ZEN REWARD
				// ------------------------------
				AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];	// Zen
				AnswerPacket[147+RewPack] = LOBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[148+RewPack] = HIBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[149+RewPack] = LOBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[150+RewPack] = HIBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				RewPack+=22;
			}
			else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 4 )
			{
				// ------------------------------
				// ITEM REWARD
				// ------------------------------
				int ItemID = 0;
				int Option1 = 0;
				int Option2 = 0;
				unsigned char Durability = 0;
				char IsExe = 0;
				char IsAnc = 0;

				if ( QuestsInfo[QuestType].RewItemExe[i][QuestIndex] > 0 )
				{
					IsExe = 1;
				}

				if ( QuestsInfo[QuestType].RewItemAnc[i][QuestIndex] > 0 )
				{
					IsAnc = 1;
				}

				ItemID = (QuestsInfo[QuestType].RewItemType[i][QuestIndex]*512) + QuestsInfo[QuestType].RewItemIndex[i][QuestIndex];

				Option1 = (QuestsInfo[QuestType].RewItemLuck[i][QuestIndex]*4);
				Option1 += (QuestsInfo[QuestType].RewItemSkill[i][QuestIndex]*128);
				Option1 += (QuestsInfo[QuestType].RewItemLevel[i][QuestIndex]*8);

				if ( QuestsInfo[QuestType].RewItemAddOpt[i][QuestIndex] > 3 )
				{
					Option1 += 3;
					int AuxOpt = QuestsInfo[QuestType].RewItemAddOpt[i][QuestIndex]-3;
					for (int j=0;j<AuxOpt;j++)
					{
						Option2+=16;
					}
				}
				else
				{
					Option1 += (QuestsInfo[QuestType].RewItemAddOpt[i][QuestIndex]);
				}

				Option2 += (QuestsInfo[QuestType].RewItemExe[i][QuestIndex]);

				if ( QuestsInfo[QuestType].RewItemDur[i][QuestIndex] <= 0 )
				{
					Durability = ItemGetDurability(ItemID,QuestsInfo[QuestType].RewItemLevel[i][QuestIndex],IsExe,IsAnc);
				}
				else
				{
					Durability = QuestsInfo[QuestType].RewItemDur[i][QuestIndex];
				}

				AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
				AnswerPacket[147+RewPack] = QuestsInfo[QuestType].RewItemCount[i][QuestIndex];
				AnswerPacket[151+RewPack] = QuestsInfo[QuestType].RewItemIndex[i][QuestIndex];
				AnswerPacket[152+RewPack] = Option1;
				AnswerPacket[153+RewPack] = Durability;
				AnswerPacket[154+RewPack] = Option2;
				AnswerPacket[156+RewPack] = QuestsInfo[QuestType].RewItemAnc[i][QuestIndex];
				AnswerPacket[156+RewPack] |= (ItemID & 0x1E00 ) >> 5;
				AnswerPacket[156+RewPack] |= (0 & 0x80 ) >> 4;
				AnswerPacket[157+RewPack] = 0x00;
				//AnswerPacket[158+RewPack] = 0xFF;
				//AnswerPacket[159+RewPack] = 0xFF;
				//AnswerPacket[160+RewPack] = 0xFF;
				//AnswerPacket[161+RewPack] = 0xFF;
				//AnswerPacket[162+RewPack] = 0xFF;
				RewPack+=22;
			}
			else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 8 )
			{
				// ------------------------------
				// BONUS REWARD
				// ------------------------------
				AnswerPacket[141+RewPack] = 8;
				RewPack+=22;
			}
			else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 16 )
			{
				// ------------------------------
				// CONTRIBUTION REWARD
				// ------------------------------
				AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
				AnswerPacket[147+RewPack] = LOBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[148+RewPack] = HIBYTE(LOWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[149+RewPack] = LOBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				AnswerPacket[150+RewPack] = HIBYTE(HIWORD(QuestsInfo[QuestType].RewZCE[i][QuestIndex]));
				RewPack+=22;
			}
			else if ( QuestsInfo[QuestType].RewardType[i][QuestIndex] == 32 )
			{
				// ------------------------------
				// RANDOM REWARD
				// ------------------------------
				AnswerPacket[141+RewPack] = QuestsInfo[QuestType].RewardType[i][QuestIndex];
				RewPack+=22;
			}
		}
	}

	gSendProto.DataSend(aIndex,AnswerPacket,AnswerPacket[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::CheckQuest(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	BYTE Packet[100] = {0x00};
	Packet[0] = 0xC1;
	Packet[1] = 0x05;
	Packet[2] = 0xF6;
	Packet[3] = 0x1A;
	Packet[4] = 0x00; // Count

	//if ( lpObj->QuestTutorial == 0 )
	//{
	//	BYTE Packet2[12] = {0xC1, 0x0C, 0xF6, 0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00};
	//	gSendProto.DataSend(aIndex,Packet2,Packet2[1]);
	//}
	if ( QuestNew.Enabled == 1 )
	{
		for ( int i=0;i<50;i++)
		{
			if ( (lpObj->NewQuestInfo[i*12] != 0 && lpObj->NewQuestInfo[(i*12)+1] != 0) && (lpObj->NewQuestInfo[i*12] != 0xFF && lpObj->NewQuestInfo[(i*12)+1] != 0xFF) )
			{
				Packet[1] += 0x04;
				Packet[4] += 0x01;
				Packet[Packet[1]-0x02] = lpObj->NewQuestInfo[i*12];
				Packet[Packet[1]-0x04] = lpObj->NewQuestInfo[(i*12)+1];
			}
		}
	}
	
	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::GiveUpQuest(unsigned char * aRecv,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	for (int n=0;n<50;n++)
	{
		if ( lpObj->NewQuestInfo[n*12] == aRecv[6] && lpObj->NewQuestInfo[(n*12)+1] == aRecv[4] )
		{
			lpObj->NewQuestInfo[n*12] = 0;
			lpObj->NewQuestInfo[(n*12)+1] = 0;
			lpObj->NewQuestInfo[(n*12)+2] = 0;
			lpObj->NewQuestInfo[(n*12)+3] = 0;
			lpObj->NewQuestInfo[(n*12)+4] = 0;
			lpObj->NewQuestInfo[(n*12)+5] = 0;
			lpObj->NewQuestInfo[(n*12)+6] = 0;
			lpObj->NewQuestInfo[(n*12)+7] = 0;
			lpObj->NewQuestInfo[(n*12)+8] = 0;
			lpObj->NewQuestInfo[(n*12)+9] = 0;
			lpObj->NewQuestInfo[(n*12)+10] = 0;
			lpObj->NewQuestInfo[(n*12)+11] = 0;
			break;
		}
	}
	PMSG_GIVE_UP_QUEST pMsg;
	
	pMsg.c = 0xC1;
	pMsg.size = 0x08;
	pMsg.headcode = 0xF6;
	pMsg.subcode = 0x0F;
	pMsg.subtype1 = aRecv[4];
	pMsg.subtype2 = 0x00;
	pMsg.type1 = aRecv[6];
	pMsg.type2 = 0x00;

	gSendProto.DataSend(aIndex,(BYTE *)&pMsg,pMsg.size);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::QuestFinish(unsigned char * aRecv,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int j=0;
	for (int n=0;n<50;n++)
	{
		if ( lpObj->NewQuestInfo[n*12] == aRecv[6] && lpObj->NewQuestInfo[(n*12)+1] == aRecv[4] )
		{
			//if ( QuestNew.CanReward(aIndex,aRecv[6],aRecv[4] ) == false )
			//{
			//	return;
			//}
			////else
			////{
				lpObj->NewQuestInfo[n*12] = 0;
				lpObj->NewQuestInfo[(n*12)+1] = 0;
				lpObj->NewQuestInfo[(n*12)+2] = 0;
				lpObj->NewQuestInfo[(n*12)+3] = 0;
				lpObj->NewQuestInfo[(n*12)+4] = 0;
				lpObj->NewQuestInfo[(n*12)+5] = 0;
				lpObj->NewQuestInfo[(n*12)+6] = 0;
				lpObj->NewQuestInfo[(n*12)+7] = 0;
				lpObj->NewQuestInfo[(n*12)+8] = 0;
				lpObj->NewQuestInfo[(n*12)+9] = 0;
				lpObj->NewQuestInfo[(n*12)+10] = 0;
				lpObj->NewQuestInfo[(n*12)+11] = 0;
			////}
			break;
		}
	}
	BYTE Packet[9] = { 0xC1, 0x09, 0xF6, 0x0D, aRecv[4], 0x00, aRecv[6], 0x00, 0x01};
	gSendProto.DataSend(aIndex,Packet,Packet[1]);
	return;
}
// --------------------------------------------------------------------------------------
void SQuest::MonsterKillManager(int aIndex,int MonsterIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpMobObj = &gObj[MonsterIndex];
	int AuxTotalCount = 0;
	char QuestType = 0;
	int QuestIndex = 0;

	if ( QuestNew.Enabled == 0 )
	{
		return;
	}

	for (int n=0;n<50;n++)
	{
		if ( (lpObj->NewQuestInfo[(12*n)] != 0 && lpObj->NewQuestInfo[1+(12*n)] != 0) && (lpObj->NewQuestInfo[(12*n)] != 0xFF && lpObj->NewQuestInfo[1+(12*n)] != 0xFF) )
		{
			QuestType = QuestNew.ReturnQuestPos(lpObj->NewQuestInfo[(12*n)]);
			QuestIndex = QuestNew.GetQuestIndex2(aIndex,lpObj->NewQuestInfo[(12*n)],lpObj->NewQuestInfo[1+(12*n)],QuestType);
			// ----
			for (int h=0;h<QuestsInfo[QuestType].TotalObjCount[QuestIndex];h++)
			{
				if ( QuestsInfo[QuestType].ObjetiveType[h][QuestIndex] == 1 )
				{
					if ( QuestsInfo[QuestType].ObjMobLvlSk[h][QuestIndex] == lpMobObj->Class )
					{
						if ( QuestsInfo[QuestType].ObjCount[h][QuestIndex] > lpObj->NewQuestInfo[(12*n)+2+(2*h)] + (lpObj->NewQuestInfo[(12*n)+3+(2*h)]*256) )
						{
							if ( lpObj->NewQuestInfo[(12*n)+2+(2*h)] == 255 )
							{
								lpObj->NewQuestInfo[(12*n)+3+(2*h)]+= 1;
								lpObj->NewQuestInfo[(12*n)+2+(2*h)] = 0;
							}
							else
							{
								lpObj->NewQuestInfo[(12*n)+2+(2*h)]+=1;
							}
						}
					}
				}
			}
		}
	}
}
// --------------------------------------------------------------------------------------
//bool SQuest::CanReward(int aIndex,int Type,int SubType)
//{
//	LPOBJ lpObj = &gObj[aIndex];
//	int ItemCount = 0;
//	bool ItemObj = true;
//	bool RewObj = true;
//	// ----
//	for (int i=0;i<255;i++)
//	{
//		//if ( QuestsInfo[i].Type == (unsigned char *)Type && QuestsInfo[i].SubType == (unsigned char *)SubType )
//		//{
//		//	for (int j=0;j<QuestsInfo[i].TotalObjCount[Type];j++)
//		//	{
//		//		if ( QuestsInfo[i].ObjetiveType[Type][SubType][j] == 4 )
//		//		{
//		//			ItemCount = gObjGetCompleteItemCountInIventory(aIndex,
//		//				QuestsInfo[i].ObjItemType[Type][SubType], 
//		//				QuestsInfo[i].ObjItemIndex[Type][SubType],
//		//				QuestsInfo[i].ObjItemLevel[Type][SubType],
//		//				QuestsInfo[i].ObjItemLuck[Type][SubType],
//		//				QuestsInfo[i].ObjItemSkill[Type][SubType],
//		//				QuestsInfo[i].ObjItemAddOpt[Type][SubType],
//		//				QuestsInfo[i].ObjItemExe[Type][SubType]);
//
//		//			if ( ItemCount < QuestsInfo[i].ObjCount[Type][SubType])
//		//			{
//		//				ItemObj = false;
//		//				return false;
//		//			}
//		//			else
//		//			{
//		//				ItemObj = true;
//		//			}
//		//		}
//		//	}
//			//if ( ItemObj == false )
//			//{
//			//	return false;
//			//}
//			//else
//			//{
//			//	for (int j=0;j<QuestNew.TotalObjCount[Type][SubType];j++)
//			//	{
//			//		if ( QuestsInfo[i].ObjetiveType[Type][SubType][j] == 4 )
//			//		{
//			//			ItemCount = gObjGetCompleteItemCountInIventory(aIndex,QuestNew.ObjItemType[Type][SubType][j], QuestNew.ObjItemIndex[Type][SubType][j],
//			//						QuestNew.ObjItemLevel[Type][SubType][j],QuestNew.ObjItemLuck[Type][SubType][j],QuestNew.ObjItemSkill[Type][SubType][j],
//			//						QuestNew.ObjItemAddOpt[Type][SubType][j],QuestNew.ObjItemExe[Type][SubType][j]);
//			//			// ----
//			//			int ItemIndex = (QuestNew.ObjItemType[Type][SubType][j]*512) + QuestNew.ObjItemIndex[Type][SubType][j];
//			//			int Level = QuestsInfo[i].ObjItemLevel[Type][SubType][j];
//			//			int Luck = QuestsInfo[i].ObjItemLuck[Type][SubType][j];
//			//			int Skill = QuestsInfo[i].ObjItemSkill[Type][SubType][j];
//			//			int AddOpt = QuestsInfo[i].ObjItemAddOpt[Type][SubType][j];
//			//			int Exe = QuestsInfo[i].ObjItemExe[Type][SubType][j];
//
//			//			for (int h=0;h<ItemCount;h++)
//			//			{
//			//				gUser.gObjInventoryDeleteItemComplete(aIndex,ItemIndex,Level,Skill,Luck,AddOpt,Exe);
//			//			}
//			//		}
//			//	}
//			//}
//			//for (int n=0;n<QuestNew.TotalRewCount[Type][SubType];n++)
//			//{
//			//	if ( QuestNew.RewardType[Type][SubType][n] == 4 )
//			//	{
//			//		int ItemID = (QuestNew.RewItemType[Type][SubType][n]*512) + QuestNew.RewItemIndex[Type][SubType][n];
//			//		BYTE Luck = QuestNew.RewItemLuck[Type][SubType][n];
//			//		BYTE Skill = QuestNew.RewItemSkill[Type][SubType][n];
//			//		BYTE Level = QuestNew.RewItemLevel[Type][SubType][n];
//			//		BYTE AddOpt = QuestNew.RewItemAddOpt[Type][SubType][n];
//			//		BYTE Exe = QuestNew.RewItemExe[Type][SubType][n];
//			//		int Count = QuestNew.RewItemCount[Type][SubType][n];
//			//		BYTE Socket[5];
//			//		Socket[0] = 0xFF;
//			//		Socket[1] = 0xFF;
//			//		Socket[2] = 0xFF;
//			//		Socket[3] = 0xFF;
//			//		Socket[4] = 0xFF;
//			//		for (int h=0;h<Count;h++)
//			//		{
//			//			ItemSerialCreateSend(aIndex,lpObj->MapNumber,lpObj->X,lpObj->Y,ItemID,Level,0,Skill,Luck,AddOpt,aIndex,Exe,0, Socket);
//			//		}
//			//	}
//			//	else if ( QuestNew.RewardType[Type][SubType][n] == 1 )
//			//	{
//			//		if ( gUser.gObjIsMasteringLevel(lpObj) == true )
//			//		{
//			//			lpObj->MLExperience += QuestNew.RewExp[Type][SubType][n];
//			//		}
//			//		else
//			//		{
//			//			lpObj->Experience += QuestNew.RewExp[Type][SubType][n];
//			//		}
//			//		gObjLevelUp(lpObj,0,0,0);
//			//	}
//			//	else if ( QuestNew.RewardType[Type][SubType][n] == 2 )
//			//	{
//			//		if ( lpObj->Money + QuestNew.RewZen[Type][SubType][n] > MAX_ZEN )
//			//		{
//			//			lpObj->Money = MAX_ZEN;
//			//		}
//			//		else
//			//		{
//			//			lpObj->Money += QuestNew.RewZen[Type][SubType][n];
//			//		}
//			//		GCMoneySend(aIndex,lpObj->Money);
//			//	}
//			//	else if ( QuestNew.RewardType[Type][SubType][n] == 16 )
//			//	{
//			//		if ( lpObj->GenFamily != 0 )
//			//		{
//			//			lpObj->GenContribution += QuestNew.RewContribution[Type][SubType][n];
//			//		}
//			//	}
//			//}
//			break;
//		}
//	}
//	return true;
//}
// --------------------------------------------------------------------------------------