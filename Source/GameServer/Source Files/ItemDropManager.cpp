#include "../Header Files/stdafx.h"
#include "../Header Files/gObjMonster.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/WzMemScript.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/Defines.h"
#include "../Header Files/ItemDropManager.h"
#include "../Header Files/Functions.h"

CItemDropManager gItemDropManager;

CItemDropManager::CItemDropManager(void)
{
	return;
}

CItemDropManager::~CItemDropManager(void)
{
	return;
}

void CItemDropManager::Init()
{
	SMDFile = fopen(ITEMDROP_FILE_PATH, "r");

	gItemDropManager.ItemsCount = 0;

	if (SMDFile == NULL)
	{
		CLog.MsgBox("[ItemDropManager]: Failed to load file %s", ITEMDROP_FILE_PATH);
		return;
	}

	int iToken = 0;

	while (true)
	{
		iToken = GetToken();

		if (iToken == END)
		{
			break;
		}

		if (iToken == NUMBER)
		{
			gItemDropManager.ItemType[gItemDropManager.ItemsCount] = TokenNumber;
			
			iToken = GetToken();
			gItemDropManager.ItemIndex[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.ItemLevel[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.ItemDur[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.ItemLuck[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.ItemSkill[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.ItemOption[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.ItemExe[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.ItemAncient[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.Socket[0][gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.Socket[1][gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.Socket[2][gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.Socket[3][gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.Socket[4][gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.DropMap[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.MobMinLvl[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.MobMaxLvl[gItemDropManager.ItemsCount] = TokenNumber;

			iToken = GetToken();
			gItemDropManager.DropRate[gItemDropManager.ItemsCount] = TokenNumber;

			gItemDropManager.ItemsCount++;
		}
	}

	fclose(SMDFile);
	CLog.LogAdd("[ItemDropManager]: Successfuly read file %s", ITEMDROP_FILE_PATH);

	return;
}

bool CItemDropManager::ProccessItemDrop(LPOBJ lpMobObj)
{
	unsigned char Map = lpMobObj->MapNumber;
	unsigned char X = lpMobObj->X;
	unsigned char Y = lpMobObj->Y;
	int Type = 0;
	char Level = 0;
	unsigned char Dur = 0;
	char Luck = 0;
	char Skill = 0;
	char Opt = 0;
	char Exe = 0;
	char Anc = 0;
	unsigned char Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	if ( gItemDropManager.ItemsCount > 0 )
	{
		int iTopHitUser = gObjMonsterTopHitDamageUser(lpMobObj);

		for (int i=0;i<gItemDropManager.ItemsCount;++i)
		{
			if ( lpMobObj->MapNumber == gItemDropManager.DropMap[i] || gItemDropManager.DropMap[i] == -1 )
			{
				if ( lpMobObj->Level >= gItemDropManager.MobMinLvl[i] && lpMobObj->Level <= gItemDropManager.MobMaxLvl[i] )
				{
					if ( (rand()%10000) < gItemDropManager.DropRate[i] )
					{
						Type = ItemGetNumberMake(gItemDropManager.ItemType[i],gItemDropManager.ItemIndex[i]);

						Level = gItemDropManager.ItemLevel[i];
						Dur = gItemDropManager.ItemDur[i];
						Luck = gItemDropManager.ItemLuck[i];
						Skill = gItemDropManager.ItemSkill[i];
						Opt = gItemDropManager.ItemOption[i];
						if ( gItemDropManager.ItemExe[i] > 0 )
						{
							Exe = GetRandomExeOption(gItemDropManager.ItemExe[i]+1);
						}
						if ( gItemDropManager.ItemAncient[i] == 5 || gItemDropManager.ItemAncient[i] == 10 )
						{
							Anc = gItemDropManager.ItemAncient[i];
						}
						if ( gItemDropManager.Socket[0][i] != 255 )
						{
							Socket[0] = gItemDropManager.Socket[0][i];

							if ( gItemDropManager.Socket[1][i] != 255 )
							{
								Socket[1] = gItemDropManager.Socket[1][i];

								if ( gItemDropManager.Socket[2][i] != 255 )
								{
									Socket[2] = gItemDropManager.Socket[2][i];

									if ( gItemDropManager.Socket[3][i] != 255 )
									{
										Socket[3] = gItemDropManager.Socket[3][i];

										if ( gItemDropManager.Socket[4][i] != 255 )
										{
											Socket[4] = gItemDropManager.Socket[4][i];
										}
									}
								}
							}
						}

						ItemSerialCreateSend(lpMobObj->m_Index,Map,X,Y,Type,Level,Dur,Skill,Luck,Opt,iTopHitUser,Exe,Anc,Socket);
						return true;
					}
				}
			}
		}
	}
	return false;
}