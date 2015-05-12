#include "../Header Files/stdafx.h"
#include "../Header Files/Item380System.h"
#include "../Header Files/Gamemain.h"
#include "../Header Files/CastleSiegeSync.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/ReadScript.h"
#include "../Header Files/Functions.H"
#include "../Header Files/ChaosBox.h"
#include "../Header Files/Defines.H"
#include "../Header Files/SendProto.H"


CItem380System g_Item380System;

CItem380System::CItem380System(void)
{
	// ----

}

CItem380System::~CItem380System(void)
{
	return;
}

void CItem380System::Init()
{
	memset(this->m_itemOption, 0, sizeof(this->m_itemOption));

	this->m_bSystemFor380ItemOption				= GetPrivateProfileInt(ITEM380_HDR,"Enabled",0,ITEM380_FILE_PATH);
	this->m_iNeedZenFor380Option				= GetPrivateProfileInt(ITEM380_HDR,"NeedZenForMix",0,ITEM380_FILE_PATH);
	this->m_iRateSuccessRateForMix1				= GetPrivateProfileInt(ITEM380_HDR,"MixRateForGrade1",0,ITEM380_FILE_PATH);
	this->m_iRateSuccessRateForMix2				= GetPrivateProfileInt(ITEM380_HDR,"MixRateForGrade2",0,ITEM380_FILE_PATH);
	this->m_iRateSuccessRateForMix3				= GetPrivateProfileInt(ITEM380_HDR,"MixRateForGrade3",0,ITEM380_FILE_PATH);
}

void CItem380System::Load380ItemOptionInfo(const char* filename)
{
	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[Item380System] Info file Load Fail [%s]", filename);
		return;
	}

	g_Item380System.Init();

	int Token;
	int type = -1;

	while ( true )
	{
		Token = GetToken();

		if ( Token == END )
		{
			break;
		}

		if ( Token == NUMBER )
		{
			int Index = (int)TokenNumber;

			while ( true )
			{
				if ( Index == 0 )
				{
					Token = (SMDToken)GetToken();
					int _type = (int)TokenNumber;

					if ( Token == NAME )
					{
						if ( !strcmp("end", TokenString))
						{
							return;
						}
					}

					Token = GetToken();
					int _index = (int)TokenNumber;

					ITEMOPTION_FOR380ITEM * p = &this->m_itemOption[ITEMGET(_type, _index)];
					p->IsLoad = TRUE;
					p->m_Type = _type;
					p->m_Index = _index;

					Token = GetToken();
					p->m_Option1 = (BYTE)TokenNumber;

					Token = GetToken();
					p->m_Value1 = (WORD)TokenNumber;

					Token = GetToken();
					p->m_Option2 = (BYTE)TokenNumber;

					Token = GetToken();
					p->m_Value2 = (WORD)TokenNumber;

					Token = GetToken();
					int iTime = (int)TokenNumber;
				}
			}
		}
	}
	return;
}

BOOL CItem380System::Is380Item(CItem const *  pItem)
{
	if ( pItem->m_Type < 0 || pItem->m_Type > MAX_ITEMS-1 )
	{
		CLog.LogAdd("[380Item] OptionItem Check error: (iItemNum:%d)", pItem->m_Type);
		return FALSE;
	}

	if ( this->m_itemOption[pItem->m_Type ].IsLoad == FALSE )
		return FALSE;

	return TRUE;
}

BOOL CItem380System::Is380OptionItem(CItem const *  pItem)
{
	if ( this->Is380Item(pItem) == FALSE )
		return FALSE;

	BYTE bResult = (pItem->m_ItemOptionEx & 0x80) >> 7;

	if ( bResult != 1 )
		return FALSE;

	return TRUE;
}

void CItem380System::InitEffectValue(ITEMOPTION_FOR380ITEM_EFFECT * pItemEffect)
{
	pItemEffect->OpAddAttackSuccessRatePVP = 0;
	pItemEffect->OpAddDamage = 0;
	pItemEffect->OpAddDefenseSuccessRatePvP = 0;
	pItemEffect->OpAddDefense = 0;
	pItemEffect->OpAddMaxHP = 0;
	pItemEffect->OpAddMaxSD = 0;
	pItemEffect->OpRefillOn  = 0;
	pItemEffect->OpAddRefillSD = 0;
}

BOOL CItem380System::ApplyFor380Option(LPOBJ lpObj)
{
	ITEMOPTION_FOR380ITEM_EFFECT * pItemEffect = &lpObj->m_ItemOptionExFor380;
	this->InitEffectValue(pItemEffect);
	int iItemIndex = 0;

	for ( iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE ; iItemIndex++)
	{
		CItem * pItem = &lpObj->pInventory[iItemIndex];

		if ( pItem->IsItem() &&
			 pItem->m_IsValidItem &&
			 this->Is380OptionItem(pItem) == TRUE )
		{
			BYTE iOption1, iOption2;
			WORD iValue1, iValue2;

			iOption1 = this->m_itemOption[pItem->m_Type ].m_Option1;
			iValue1 = this->m_itemOption[pItem->m_Type ].m_Value1;
			iOption2 = this->m_itemOption[pItem->m_Type ].m_Option2;
			iValue2 = this->m_itemOption[pItem->m_Type ].m_Value2;
			BOOL bResult = FALSE;
			bResult = this->_CalcItemEffectValue(iOption1, iValue1, pItemEffect);
			bResult |= this->_CalcItemEffectValue(iOption2, iValue2, pItemEffect);
		}
	}

	lpObj->AddLife += pItemEffect->OpAddMaxHP;
	lpObj->iAddShield += pItemEffect->OpAddMaxSD;

	return TRUE;
}

BOOL CItem380System::_CalcItemEffectValue(int iItemOptionType, int iItemEffectValue, ITEMOPTION_FOR380ITEM_EFFECT * pItemEffect)
{
	if ( iItemOptionType == 0 )
		return FALSE;

	BOOL bResult = TRUE;

	switch ( iItemOptionType )
	{
		//case ITEMOPTION_FOR380ITEM_EFFECT_NONE:
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDATTACKSUCCESSRATEPVP:
			pItemEffect->OpAddAttackSuccessRatePVP += iItemEffectValue;
			DebugLog("380효과 : 공격성공율상승 %d\n", iItemEffectValue);
			break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDDAMAGE:
			pItemEffect->OpAddDamage += iItemEffectValue;
			DebugLog("380효과 : 데미지 상승 %d\n", iItemEffectValue);
			break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSESUCCESSRATEPVP:
			pItemEffect->OpAddDefenseSuccessRatePvP += iItemEffectValue;
			DebugLog("380효과 : 방어성공율상승 %d\n", iItemEffectValue);
			break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSE:
			pItemEffect->OpAddDefense += iItemEffectValue;
			DebugLog("380효과 : 방어력 상승 %d\n", iItemEffectValue);
			break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXHP:
			pItemEffect->OpAddMaxHP += iItemEffectValue;
			DebugLog("380효과 : 최대 HP 상승 %d\n", iItemEffectValue);
			break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXSD:
			pItemEffect->OpAddMaxSD += iItemEffectValue;
			DebugLog("380효과 : 최대 SD 상승 %d\n", iItemEffectValue);
			break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPREFILLON:
			pItemEffect->OpRefillOn += 1;
			DebugLog("380효과 : SD 자동 회복 %d\n", iItemEffectValue);
			break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDREFILLSD:
			pItemEffect->OpAddRefillSD += iItemEffectValue;
			DebugLog("380효과 : SD자동회복량 증가 %d\n", iItemEffectValue);
			break;
		default:
			bResult = FALSE;
			break;
	}

	return bResult;
}

BOOL CItem380System::_SetOption(CItem * pItem, BOOL bOption)
{
	if ( !this->Is380Item(pItem) )
		return FALSE;

	if ( bOption  == TRUE )
		pItem->m_ItemOptionEx |= 0x80;
	else
		pItem->m_ItemOptionEx &= ~0x80;

	return TRUE;
}

void CItem380System::SetOptionItemByMacro(LPOBJ lpObj, BYTE invenrotyTargetPos, int bOption)
{
	// HERE GOES A MACRO
	return;
	CItem * pItem=NULL;
}

BOOL CItem380System::ChaosMix380ItemOption(LPOBJ lpObj)
{
	if ( this->m_bSystemFor380ItemOption != TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13,55)), lpObj->m_Index, 1);
		lpObj->bIsChaosMixCompleted = false;
		return FALSE;
	}

	lpObj->ChaosLock = TRUE;

	int iValidItemCount = 0;
	int iJewelOfHarmony = 0;
	int iJewelOfSuho = 0;
	int iInvalidItemCount = 0;
	int iMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = 0;
	CItem * pTargetItem = NULL;
	int iPosOfJewelOfHarmony= -1;
	int iPosOfJewelOfSuho= -1;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
	if ( lpObj->pChaosBox[n].IsItem() == TRUE )
	{
		if ( this->Is380Item(&lpObj->pChaosBox[n]) == TRUE && this->Is380OptionItem(&lpObj->pChaosBox[n]) == FALSE &&  lpObj->pChaosBox[n].m_Level > 3 && (lpObj->pChaosBox[n].m_Option3<<2) > 3)
		{
			iValidItemCount++;
			pTargetItem = &lpObj->pChaosBox[n];
		}
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(lpObj->pChaosBox[n].m_Type) == TRUE )
		{
			iJewelOfHarmony++;
			iPosOfJewelOfHarmony = n;
		}
		else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14, 31) )
		{
			iJewelOfSuho++;
			iPosOfJewelOfSuho = n;
		}
		else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )
		{
			iCharmOfLuckCount += (int)lpObj->pChaosBox[n].m_Durability;
		}
		else
		{
			iInvalidItemCount++;
		}
	}
	}

	if ( iInvalidItemCount || iValidItemCount != 1 || iJewelOfHarmony != 1 || iJewelOfSuho != 1 || pTargetItem == NULL || iPosOfJewelOfHarmony == -1 || iPosOfJewelOfSuho == -1 || iCharmOfLuckCount > 10 )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	iMixPrice = this->m_iNeedZenFor380Option;
	int iChaosTaxMoney =  iMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
		iChaosTaxMoney = 0;

	iMixPrice += iChaosTaxMoney;

	if ( iMixPrice < 0 )
		iMixPrice = 0;

	if ( lpObj->Money < iMixPrice )
	{
		pMsg.Result = 2;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	lpObj->Money -= iMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	LogChaosItem(lpObj, "380Item][Item Mix");
	CLog.LogAdd("[380Item][Item Mix] - Mix Start");

	int iRate =	rand() % 100;
	int iRateSuccess = this->m_iRateSuccessRateForMix1;

	if ( pTargetItem->m_Level < 7 )
		iRateSuccess = this->m_iRateSuccessRateForMix1;
	else if ( pTargetItem->m_Level < 10 )
		iRateSuccess = this->m_iRateSuccessRateForMix2;
	else if ( pTargetItem->m_Level < 16 )
		iRateSuccess = this->m_iRateSuccessRateForMix3;

	iRateSuccess += iCharmOfLuckCount;

	lpObj->pChaosBox[iPosOfJewelOfHarmony].Clear();
	lpObj->pChaosBoxMap[iPosOfJewelOfHarmony] = -1;
	lpObj->pChaosBox[iPosOfJewelOfSuho].Clear();
	lpObj->pChaosBoxMap[iPosOfJewelOfSuho] = -1;

	if ( iRate < iRateSuccess )
	{
		this->_SetOption(pTargetItem, TRUE);
		GCUserChaosBoxSend(lpObj, 0);

		CLog.LogAdd("[380Item][ItemMix] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d) Option(%d,%d) OptionValue(%d,%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money,
			iMixPrice, iRate, iRateSuccess,
			this->m_itemOption[(pTargetItem->m_Type )].m_Option1,
			this->m_itemOption[(pTargetItem->m_Type )].m_Option2,
			this->m_itemOption[(pTargetItem->m_Type )].m_Value1,
			this->m_itemOption[(pTargetItem->m_Type )].m_Value2);
	}
	else
	{
		GCUserChaosBoxSend(lpObj, 0);
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		CLog.LogAdd("[380Item][ItemMix] Mix Fail [%s][%s], Money(%d-%d) Rate(%d/%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money,
			iMixPrice, iRate, iRateSuccess);

	}

	lpObj->ChaosLock = FALSE;
	return TRUE;
}

