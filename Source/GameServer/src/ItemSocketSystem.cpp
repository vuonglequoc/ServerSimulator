//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Item Socket System
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "User.h"
#include "GameMain.h"
#include "ItemSocketSystem.h"
#include "LogProc.H"
#include "WzMemScript.h"
#include "ReadScript.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"
#include "Notice.H"
#include "Functions.H"
#include "SendProto.H"


CItemSocket gItemSocket;
// -----------------------------------------------------------------------------------------------------------------------
CItemSocket::CItemSocket(void)
{
    for(int n=0;n < MAX_SOCKET_OPTION;n++)
	{
	    this->m_ItemSocketInfo[n].Index = -1;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
CItemSocket::~CItemSocket(void)
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
BOOL CItemSocket::Load(const char * lpszFileName)
{
	SMDFile = fopen(lpszFileName, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[SocketItemSystem] Info file Load Fail [%s]", lpszFileName);
		return FALSE;
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
		    if( type == 0 )
			{
			    Token = GetToken();

				if ( strcmp("end", TokenString ) == 0)
				{
					break;
				}

				ITEM_SOCKET_INFO SocketR;
		        memset( &SocketR, 0, sizeof(SocketR) );

		        SocketR.Index = TokenNumber;

				Token = GetToken();
		        SocketR.Type = TokenNumber;

				Token = GetToken();
		        SocketR.Level = TokenNumber;

		        Token = GetToken();
				strcpy(&SocketR.Name[0], TokenString );

				Token = GetToken();
		        SocketR.BonusType = TokenNumber;

		        Token = GetToken();
		        SocketR.Bonus[0] = TokenNumber;

		        Token = GetToken();
		        SocketR.Bonus[1] = TokenNumber;

		        Token = GetToken();
		        SocketR.Bonus[2] = TokenNumber;

		        Token = GetToken();
		        SocketR.Bonus[3] = TokenNumber;

		        Token = GetToken();
		        SocketR.Bonus[4] = TokenNumber;

		        this->SetInfo(SocketR);
			}
			else if( type == 1)
			{
			    Token = GetToken();

				if(strcmp("end",TokenString ) == 0)
				{
					break;
				}

				Token = GetToken();
				Token = GetToken();
				Token = GetToken();
			}
			else if( type == 2)
			{
			    Token = GetToken();

				if(strcmp("end",TokenString ) == 0)
				{
					break;
				}
			}
			else
			{
			    break;
			}
		}
    }

	fclose(SMDFile);
	CLog.LogAdd("%s file load!", lpszFileName);
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::SetInfo(ITEM_SOCKET_INFO info)
{
    if(info.Index >= MAX_SOCKET_OPTION)
	{
		CLog.LogAddC( 2, "[SocketSystem] Maximum Number is Excceeded Socket ( Index: [%d]  Max Socket: [%d] ).", info.Index, MAX_SOCKET_OPTION );
	    return;
	}

    this->m_ItemSocketInfo[info.Index] = info;
}
// -----------------------------------------------------------------------------------------------------------------------
ITEM_SOCKET_INFO* CItemSocket::GetInfo(int index)
{
    return &this->m_ItemSocketInfo[index];
}
// -----------------------------------------------------------------------------------------------------------------------
int CItemSocket::GetOptionCount(int type)
{
	int count = 0;

    for(int n=0;n < MAX_SOCKET_OPTION;n++)
	{
	    if(this->m_ItemSocketInfo[n].Index >= 0 && this->m_ItemSocketInfo[n].Type == type)
		{
			count++;
		}
	}

	return count;
}
// -----------------------------------------------------------------------------------------------------------------------
BYTE CItemSocket::CalcOption(int type,int level,int SeedOption)
{
    BYTE option = 0xFE;

	int count = SeedOption;

	for(int n=0;n < MAX_SOCKET_OPTION;n++)
	{
	    if(this->m_ItemSocketInfo[n].Index >= 0 && this->m_ItemSocketInfo[n].Type == type)
		{
            if(count == 0)
			{
			    option = this->m_ItemSocketInfo[n].Index;
				break;
			}

			count--;
		}
	}

	return (option+(level*MAX_SOCKET_OPTION)); //50
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::InitEffect(ITEM_SOCKET_EFFECT* lpEffect)
{
    memset(lpEffect,0,sizeof(ITEM_SOCKET_EFFECT));
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::ApplyEffect(LPOBJ lpObj)
{
	int SocketCount[6] = {0,0,0,0,0,0};

    ITEM_SOCKET_EFFECT* lpEffect = &lpObj->ItemSocketEffect;

	this->InitEffect(lpEffect);

    for(int n=0;n < 7;n++)
	{
	    if(lpObj->pInventory[n].IsItem() == 0 || lpObj->pInventory[n].IsSocketItem() == 0 || lpObj->pInventory[n].m_Durability == 0.0f || lpObj->pInventory[n].m_IsValidItem == 0)
		{
		    continue;
		}

		for(int i=0;i<5;i++)
		{
		    if(lpObj->pInventory[n].m_ItemSocket[i] < 0xFE)
			{
			    this->CalcEffect(lpObj->pInventory[n].m_ItemSocket[i],lpEffect);
			}
		}
	}

	lpObj->m_AttackDamageMinLeft += lpEffect->IncreaseDamage;
	lpObj->m_AttackDamageMinLeft += lpEffect->IncreaseMinDamage;
	lpObj->m_AttackDamageMaxLeft += lpEffect->IncreaseDamage;
	lpObj->m_AttackDamageMaxLeft += lpEffect->IncreaseMaxDamage;

	lpObj->m_AttackDamageMinRight += lpEffect->IncreaseDamage;
	lpObj->m_AttackDamageMinRight += lpEffect->IncreaseMinDamage;
	lpObj->m_AttackDamageMaxRight += lpEffect->IncreaseDamage;
	lpObj->m_AttackDamageMaxRight += lpEffect->IncreaseMaxDamage;

	lpObj->m_MagicDamageMin += lpEffect->IncreaseSkillPower;
	lpObj->m_MagicDamageMin += lpEffect->IncreaseMinSkillPower;
	lpObj->m_MagicDamageMax += lpEffect->IncreaseSkillPower;
	lpObj->m_MagicDamageMax += lpEffect->IncreaseMaxSkillPower;

	lpObj->m_AttackSpeed += lpEffect->IncreaseAttackSpeed;
	lpObj->m_MagicSpeed += lpEffect->IncreaseAttackSpeed;

	lpObj->m_SuccessfulBlocking += (lpObj->m_SuccessfulBlocking*lpEffect->IncreaseDefenseSuccessRate)/100;

	lpObj->m_Defense += lpEffect->IncreaseDefense;

	lpObj->DamageReflect += lpEffect->IncreaseDamageReflect;

	lpObj->DamageMinus += lpEffect->IncreaseDamageReduction;

	lpObj->AddLife += (lpObj->MaxLife*lpEffect->IncreaseMaxHP)/100;

	lpObj->AddMana += (lpObj->MaxMana*lpEffect->IncreaseMaxMP)/100; 

	lpObj->AddBP += (lpObj->MaxBP*lpEffect->IncreaseMaxAG)/100;

	lpObj->m_ExcelentDamage += lpEffect->IncreaseExcellentDamageRate;

	lpObj->m_CriticalDamage += lpEffect->IncreaseCriticalDamageRate;
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::CalcEffect(BYTE option,ITEM_SOCKET_EFFECT* lpEffect)
{
	int index = 0;
    int level = 0;

	if(option > 0)
	{
		index = option%MAX_SOCKET_OPTION;
	    level = option/MAX_SOCKET_OPTION;
	}

	int bonus = this->m_ItemSocketInfo[index].Bonus[level];

	switch(index)
	{
	    case ITEM_SOCKET_INCREASE_DAMAGE:
			lpEffect->IncreaseDamage += bonus;
	        break;
        case ITEM_SOCKET_INCREASE_ATTACK_SPEED:
			lpEffect->IncreaseAttackSpeed += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_MAX_DAMAGE_AND_SKILL_POWER:
			lpEffect->IncreaseMaxDamage += bonus;
			lpEffect->IncreaseMaxSkillPower += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_MIN_DAMAGE_AND_SKILL_POWER:
			lpEffect->IncreaseMinDamage += bonus;
			lpEffect->IncreaseMinSkillPower += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_DAMAGE_AND_SKILL_POWER:
			lpEffect->IncreaseDamage += bonus;
			lpEffect->IncreaseSkillPower += bonus;
	        break;
	    case ITEM_SOCKET_DECREASE_AG_CONSUMPTION:
			lpEffect->IncreaseAGReduction += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_DEFENSE_SUCCESS_RATE:
			lpEffect->IncreaseDefenseSuccessRate += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_DEFENSE:
			lpEffect->IncreaseDefense += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_SHIELD_DEFENSE:
			lpEffect->IncreaseShieldDefense += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_DAMAGE_REDUCTION:
            lpEffect->IncreaseDamageReduction += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_DAMAGE_REFLECT:
			lpEffect->IncreaseDamageReflect += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_RECOVER_LIFE:
            lpEffect->IncreaseRecoverLife += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_RECOVER_MANA:
			lpEffect->IncreaseRecoverMana += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_SKILL_POWER:
			lpEffect->IncreaseSkillPower += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_ATTACK_SUCCESS_RATE:
			lpEffect->IncreaseAttackSuccessRate += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_ITEM_DURABILITY:
	        break;
	    case ITEM_SOCKET_INCREASE_LIFE_AUTORECOVERY:
	        break;
	    case ITEM_SOCKET_INCREASE_MAX_HP:
			lpEffect->IncreaseMaxHP += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_MAX_MP:
			lpEffect->IncreaseMaxMP += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_MANA_AUTORECOVERY:
	        break;
	    case ITEM_SOCKET_INCREASE_MAX_AG:
			lpEffect->IncreaseMaxAG += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_AG_AUTORECOVERY:
	        break;
	    case ITEM_SOCKET_INCREASE_EXCELLENT_DAMAGE:
			lpEffect->IncreaseExcellentDamage += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_EXCELLENT_DAMAGE_RATE:
			lpEffect->IncreaseExcellentDamageRate += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_CRITICAL_DAMAGE:
			lpEffect->IncreaseCriticalDamage += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_CRITICAL_DAMAGE_RATE:
			lpEffect->IncreaseCriticalDamageRate += bonus;
	        break;
	    case ITEM_SOCKET_INCREASE_STRENGTH:
	        break;
	    default:
		    break;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
bool CItemSocket::IsSeason4Item(int index)
{
    if(index >= ITEMGET(0,26) && index <= ITEMGET(0,28))
	{
	    return 1;
	}

    if(index == ITEMGET(2,16) || index == ITEMGET(2,17))
	{
	    return 1;
	}

	if(index == ITEMGET(4,23))
	{
	    return 1;
	}

	if(index == ITEMGET(5,30) || index == ITEMGET(5,31))
	{
	    return 1;
	}

	if(index >= ITEMGET(6,17) && index <= ITEMGET(6,20))
	{
	    return 1;
	}

	int section = index/512;

	if(section >= 7 && section <= 11)
	{
	    int type = index%512;

		if(type >= 45 && type <= 53)
		{
		    return 1;
		}
	}

	return 0;
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::SeedExtract(LPOBJ lpObj) //GOOD
{
    int ChaosCount = 0;
	int HarmonyCount = 0;
	int CreationCount = 0;
	int ExcItemCount = 0;
	int SetItemCount = 0;

	lpObj->ChaosLock = 1;

    for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
	    if(lpObj->pChaosBox[n].IsItem() == 0)
		{
		    continue;
		}

        if(lpObj->pChaosBox[n].m_Type == ITEMGET(12,15))
		{
		    ChaosCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,42))
		{
		    HarmonyCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,22))
		{
		    CreationCount++;
		}
		else if(lpObj->pChaosBox[n].IsExtItem() != 0 && lpObj->pChaosBox[n].m_Level >= 4)
		{
		    ExcItemCount++;
		}
		else if(lpObj->pChaosBox[n].IsSetItem() != 0 && lpObj->pChaosBox[n].m_Level >= 4)
		{
		    SetItemCount++;
		}
	}

    PMSG_CHAOSMIXRESULT pMsg;

    C1HeadSet((LPBYTE)&pMsg.h,0x86,sizeof(pMsg));

	if(ChaosCount != 1 || HarmonyCount != 1 || CreationCount != 1 || ExcItemCount != 1 || SetItemCount != 1)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_INCORRECT_MIX_ITEMS;
        gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg,pMsg.h.size);
		return;
	}

	lpObj->ChaosSuccessRate = 90;

	lpObj->ChaosMoney = 1000000;
	
	int TaxMoney = (lpObj->ChaosMoney*  g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < lpObj->ChaosMoney)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_NOT_ENOUGH_ZEN;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

	int type = rand()%6;
	// ----
	if ( type < 0 )
	{
		type = 0;
	}
	else if ( type > 5 )
	{
		type = 5;
	}
	int count = this->GetOptionCount(type);
	
	if(count == 0)
	{
        lpObj->ChaosLock = 0;
        pMsg.Result = CB_ERROR;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		CLog.LogAdd("[%s][%s] Seed Extract Mix Failed",lpObj->AccountID,lpObj->Name);
		return;
	}

    lpObj->Money -= lpObj->ChaosMoney;
    GCMoneySend(lpObj->m_Index,lpObj->Money);

	g_CastleSiegeSync.AddTributeMoney(TaxMoney);

    if((rand()%100) < lpObj->ChaosSuccessRate)
	{
		int index = ITEMGET(12,60)+type;
		int level;
		if ( type == 5 )
		{
			level = 2;
		}
		else
		{
			level = rand()%count;
		}
		BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

	    ItemSerialCreateSend(lpObj->m_Index,-1,0,0,index,level,0,0,0,0,-1,0,0,Socket);
		CLog.LogAdd("[%s][%s] Seed Extract Mix Success - Seed Info ( Index : %d, Level : %d )",lpObj->AccountID,lpObj->Name,index,level);
	}
	else
	{
	    gChaosBox.ChaosBoxInit(lpObj);
        GCUserChaosBoxSend(lpObj,0);

	    pMsg.Result = CB_ERROR;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
	    CLog.LogAdd("[%s][%s] Seed Extract Mix Failed",lpObj->AccountID,lpObj->Name);
	}

    lpObj->ChaosLock = 0;
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::SeedSphere(LPOBJ lpObj)
{
    int ChaosCount = 0;
	int CreationCount = 0;
	int SeedCount = 0;
	int SeedType = 0;
	int SeedLevel = 0;
	int SphereCount = 0;
	int SphereType = 0;

	lpObj->ChaosLock = 1;

    for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
	    if(lpObj->pChaosBox[n].IsItem() == 0)
		{
		    continue;
		}

        if(lpObj->pChaosBox[n].m_Type == ITEMGET(12,15))
		{
		    ChaosCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,22))
		{
		    CreationCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type >= ITEMGET(12,60) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,66))
		{
		    SeedCount++;
			SeedType = lpObj->pChaosBox[n].m_Type - ITEMGET(12,60); // From 0 to 5
			SeedLevel = lpObj->pChaosBox[n].m_Level;
		}
		else if(lpObj->pChaosBox[n].m_Type >= ITEMGET(12,70) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,74))
		{
		    SphereCount++;
			SphereType = lpObj->pChaosBox[n].m_Type - ITEMGET(12,70); // From 0 to 4
		}
	}

    PMSG_CHAOSMIXRESULT pMsg;

    C1HeadSet((LPBYTE)&pMsg.h,0x86,sizeof(pMsg));

	if(ChaosCount != 1 || CreationCount != 1 || SeedCount != 1 || SphereCount != 1)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_INCORRECT_MIX_ITEMS;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

	lpObj->ChaosSuccessRate = 85-(SphereType*5);

	lpObj->ChaosMoney = 1000000;
	
	int TaxMoney = (lpObj->ChaosMoney*g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < lpObj->ChaosMoney)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_NOT_ENOUGH_ZEN;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

    lpObj->Money -= lpObj->ChaosMoney;
    GCMoneySend(lpObj->m_Index,lpObj->Money);

	g_CastleSiegeSync.AddTributeMoney(TaxMoney);

	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;

    if((rand()%100) < lpObj->ChaosSuccessRate)
	{
		int index = ITEMGET(12,100)+(SphereType*6)+SeedType;
		int level = SeedLevel;

	    ItemSerialCreateSend(lpObj->m_Index,-1,0,0,index,level,0,0,0,0,-1,0,0,Socket);
        CLog.LogAdd("[%s][%s] SeedSphere Composite Success - Seed Info ( Index : %d, Level : %d )",lpObj->AccountID,lpObj->Name,index,level);
	}
	else
	{
	    gChaosBox.ChaosBoxInit(lpObj);
        GCUserChaosBoxSend(lpObj,0);

	    pMsg.Result = 0;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
	    CLog.LogAdd("[%s][%s] SeedSphere Composite Failed",lpObj->AccountID,lpObj->Name);
	}

    lpObj->ChaosLock = 0;
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::SeedCalc(LPOBJ lpObj,BYTE info)
{
    int ChaosCount = 0;
	int CreationCount = 0;
	int SeedSphereCount = 0;
	int SeedSphereType = 0;
	int SeedSphereLevel = 0;
	int SeedSphereOption = 0;
	int ItemCount = 0;
	int ItemSlot = 0;

	lpObj->ChaosLock = 1;

    for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
	    if(lpObj->pChaosBox[n].IsItem() == 0)
		{
		    continue;
		}

        if(lpObj->pChaosBox[n].m_Type == ITEMGET(12,15))
		{
		    ChaosCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,22))
		{
		    CreationCount++;
		}
		else if(lpObj->pChaosBox[n].m_Type >= ITEMGET(12,100) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,129))
		{
		    SeedSphereCount++;
			SeedSphereType = gItemSocket.CalculateSeedType(lpObj->pChaosBox[n].m_Type); // From 1 to 6
			SeedSphereLevel = gItemSocket.CalculateSeedLevel(lpObj->pChaosBox[n].m_Type); // From 0 to 4
	        SeedSphereOption = lpObj->pChaosBox[n].m_Level; 
		}
		else if( ItemAttribute[lpObj->pChaosBox[n].m_Type].SocketItem != 0 && lpObj->pChaosBox[n].m_ItemSocket[info] == 0xFE)
		{
	        ItemCount++;
			ItemSlot = n;
		}
	}

    PMSG_CHAOSMIXRESULT pMsg;

    C1HeadSet((LPBYTE)&pMsg.h,0x86,sizeof(pMsg));

	if(ChaosCount != 1 || CreationCount != 1 || SeedSphereCount != 1 || ItemCount != 1)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_INCORRECT_MIX_ITEMS;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

	lpObj->ChaosMoney = 1000000;
	
	int TaxMoney = (lpObj->ChaosMoney*g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < lpObj->ChaosMoney)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_NOT_ENOUGH_ZEN;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

	BYTE option = gItemSocket.CalculateSeedOption(SeedSphereType,SeedSphereLevel,SeedSphereOption);

	if(option == 0xFE)
	{
        lpObj->ChaosLock = 0;

        pMsg.Result = CB_ERROR;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

    lpObj->Money -= lpObj->ChaosMoney;
    GCMoneySend(lpObj->m_Index,lpObj->Money);

	g_CastleSiegeSync.AddTributeMoney(TaxMoney);

	CItem item = lpObj->pChaosBox[ItemSlot];

	item.m_ItemSocket[info] = option;

	pMsg.Result = CB_SUCCESS;

	ItemByteConvert(pMsg.ItemInfo,item);

	gChaosBox.ChaosBoxInit(lpObj);

    gObjChaosBoxInsertItemPos(lpObj->m_Index,item,0,-1);
	gObjChaosItemSet(lpObj->m_Index,0,1);

	gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);

	lpObj->ChaosLock = 0;
}
// -----------------------------------------------------------------------------------------------------------------------
void CItemSocket::NewMix4(LPOBJ lpObj,BYTE info)
{
    int ItemCount = 0;
	int ItemSlot = 0;

	lpObj->ChaosLock = 1;

    for(int n=0;n < CHAOS_BOX_SIZE;n++)
	{
	    if( ItemAttribute[lpObj->pChaosBox[n].m_Type].SocketItem > 0 && lpObj->pChaosBox[n].m_ItemSocket[info] < 0xFE)
		{
	        ItemCount++;
			ItemSlot = n;
		}
	}

    PMSG_CHAOSMIXRESULT pMsg;

    C1HeadSet((LPBYTE)&pMsg.h,0x86,sizeof(pMsg));

	if(ItemCount != 1)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_INCORRECT_MIX_ITEMS;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

	lpObj->ChaosMoney = 1000000;
	
	int TaxMoney = (lpObj->ChaosMoney*g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index))/100;

	lpObj->ChaosMoney += TaxMoney;

	if(lpObj->Money < lpObj->ChaosMoney)
	{
	    lpObj->ChaosLock = 0;

        pMsg.Result = CB_NOT_ENOUGH_ZEN;
        gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);
		return;
	}

    lpObj->Money -= lpObj->ChaosMoney;
    GCMoneySend(lpObj->m_Index,lpObj->Money);

	g_CastleSiegeSync.AddTributeMoney(TaxMoney);

	CItem item = lpObj->pChaosBox[ItemSlot];

	item.m_ItemSocket[info] = 0xFE;

	pMsg.Result = CB_SUCCESS;

	ItemByteConvert(pMsg.ItemInfo,item);

	gChaosBox.ChaosBoxInit(lpObj);

    gObjChaosBoxInsertItemPos(lpObj->m_Index,item,0,-1);
	gObjChaosItemSet(lpObj->m_Index,0,1);

	gSendProto.DataSend(lpObj->m_Index,(BYTE *)&pMsg, pMsg.h.size);

	lpObj->ChaosLock = 0;
}
// -----------------------------------------------------------------------------------------------------------------------
int CItemSocket::CalculateSeedLevel(int ItemType)
{
	int SeedLevel = 0;
	switch ( ItemType )
	{
	// ---- Level 1
	case ITEMGET(12,100):
	case ITEMGET(12,101):
	case ITEMGET(12,102):
	case ITEMGET(12,103):
	case ITEMGET(12,104):
	case ITEMGET(12,105):
		SeedLevel = 0;
		break;
	// ---- Level 2
	case ITEMGET(12,106):
	case ITEMGET(12,107):
	case ITEMGET(12,108):
	case ITEMGET(12,109):
	case ITEMGET(12,110):
	case ITEMGET(12,111):
		SeedLevel = 1;
		break;
	// ---- Level 3
	case ITEMGET(12,112):
	case ITEMGET(12,113):
	case ITEMGET(12,114):
	case ITEMGET(12,115):
	case ITEMGET(12,116):
	case ITEMGET(12,117):
		SeedLevel = 2;
		break;
	// ---- Level 4
	case ITEMGET(12,118):
	case ITEMGET(12,119):
	case ITEMGET(12,120):
	case ITEMGET(12,121):
	case ITEMGET(12,122):
	case ITEMGET(12,123):
		SeedLevel = 3;
		break;
	// ---- Level 5
	case ITEMGET(12,124):
	case ITEMGET(12,125):
	case ITEMGET(12,126):
	case ITEMGET(12,127):
	case ITEMGET(12,128):
	case ITEMGET(12,129):
		SeedLevel = 4;
		break;
	}
	return SeedLevel;
}
// -----------------------------------------------------------------------------------------------------------------------
int CItemSocket::CalculateSeedType(int ItemType)
{
	int SeedType = 0;
	switch ( ItemType )
	{
	// ---- Fire Type
	case ITEMGET(12,100):
	case ITEMGET(12,106):
	case ITEMGET(12,112):
	case ITEMGET(12,118):
	case ITEMGET(12,124):
		SeedType = 1;
		break;
	// ---- Water Type
	case ITEMGET(12,101):
	case ITEMGET(12,107):
	case ITEMGET(12,113):
	case ITEMGET(12,119):
	case ITEMGET(12,125):
		SeedType = 2;
		break;
	// ---- Ice Type
	case ITEMGET(12,102):
	case ITEMGET(12,108):
	case ITEMGET(12,114):
	case ITEMGET(12,120):
	case ITEMGET(12,126):
		SeedType = 3;
		break;
	// ---- Wind Type
	case ITEMGET(12,103):
	case ITEMGET(12,109):
	case ITEMGET(12,115):
	case ITEMGET(12,121):
	case ITEMGET(12,127):
		SeedType = 4;
		break;
	// ---- Lightning Type
	case ITEMGET(12,104):
	case ITEMGET(12,110):
	case ITEMGET(12,116):
	case ITEMGET(12,122):
	case ITEMGET(12,128):
		SeedType = 5;
		break;
	// ---- Earth Type
	case ITEMGET(12,105):
	case ITEMGET(12,111):
	case ITEMGET(12,117):
	case ITEMGET(12,123):
	case ITEMGET(12,129):
		SeedType = 6;
		break;
	}
	return SeedType;
}
// -----------------------------------------------------------------------------------------------------------------------
int CItemSocket::CalculateSeedOption(int SeedType, int SeedLevel, int SeedOption)
{
	int Index = 0;

	switch ( SeedType )
	{
	// --- Fire
	case 1:		Index = 0+SeedOption; break;
	// --- Water
	case 2:		Index = 10+SeedOption; break;
	// --- Ice
	case 3:		Index = 16+SeedOption; break;
	// --- Wind
	case 4:		Index = 21+SeedOption; break;
	// --- Lightning
	case 5:		Index = 29+SeedOption; break;
	// --- Earth
	case 6:		Index = 36; break;
	}

	Index+=(50*SeedLevel);

	return Index;
}
// -----------------------------------------------------------------------------------------------------------------------