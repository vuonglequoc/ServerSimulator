//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU ObjBaseAttack Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "ObjBaseAttack.h"
#include "GameMain.h"
#include "LogProc.H"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "GenSystem.h"
#include "DuelSystem.h"
#include "Party.h"


CObjBaseAttack::CObjBaseAttack(void)
{
	return;
}

CObjBaseAttack::~CObjBaseAttack(void)
{
	return;
}
//Done 100%
bool CObjBaseAttack::DecreaseArrow(LPOBJ lpObj)
{
	if ( lpObj->Class == CLASS_ELF && lpObj->Type == OBJ_USER )
	{
		CItem * Right = &lpObj->pInventory[0];
		CItem * Left  = &lpObj->pInventory[1];

		if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type <= ITEMGET(4,14)) || 
			  Right->m_Type == ITEMGET(4,16) || 
			  Right->m_Type == ITEMGET(4,18) || 
			  Right->m_Type == ITEMGET(4,19) )
		{
			if ( Left->m_Type == ITEMGET(4,7) )
			{
				if ( Left->m_Durability < 1.0f )
				{
					return false;
				}

				Left->m_Durability -= 1.0f;
				GCItemDurSend(lpObj->m_Index, 1, Left->m_Durability, 0);

				if ( Left->m_Durability < 1.0f )
				{
					lpObj->pInventory[1].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, 1, 0);
				}
			}
			else
			{
				return false;
			}
		}
		else if ( (Left->m_Type >= ITEMGET(4,0) && Left->m_Type <= ITEMGET(4,6) ) ||
				   Left->m_Type == ITEMGET(4,17) ||
				   Left->m_Type == ITEMGET(4,20) ||
				   Left->m_Type == ITEMGET(4,21) ||
				   Left->m_Type == ITEMGET(4,22) ||
				   Left->m_Type == ITEMGET(4,23) ||
				   Left->m_Type == ITEMGET(4,24))
		{
			if ( Right->m_Type == ITEMGET(4,15) )
			{
				if ( Right->m_Durability < 1.0f )
				{
					return false;
				}

				Right->m_Durability -= 1.0f;
				GCItemDurSend(lpObj->m_Index, 0, Right->m_Durability, 0);

				if ( Right->m_Durability < 1.0f )
				{
					lpObj->pInventory[0].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
				}
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

bool CObjBaseAttack::CheckAttackArea(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpTargetObj->Type == OBJ_USER || lpTargetObj->m_RecallMon >= 0 )
	{
		BYTE attr = MapC[lpTargetObj->MapNumber].GetAttr((BYTE)lpTargetObj->X, (BYTE)lpTargetObj->Y);

		if ( (attr&1) == 1 )
		{
			return false;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		int iRet = gObjCheckAttackArea(lpObj->m_Index, lpTargetObj->m_Index);

		if ( iRet != 0 )
		{
			CLog.LogAdd("[%s][%s] Try Attack In Not Attack Area (%s,%d,%d) errortype = %d",
				lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 208) + lpObj->MapNumber),
				lpObj->X, lpObj->Y, iRet);

			/*if ( bIsIgnorePacketSpeedHackDetect != FALSE )
			{
				return false;
			}*/
		}
	}

	return true;
}

bool CObjBaseAttack::PkCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	/*if ( gLanguage == 0 || gLanguage == 2 || gLanguage == 4)
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if (lpObj->m_PK_Level >= 6 )
			{
				if ( lpObj->m_PK_Count >= 3 )
				{
					if ( gPkLimitFree == FALSE )
					{
						return false;
					}
				}
			}
		}
	}*/

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if ( lpTargetObj->Level <= 5 || lpObj->Level <= 5 )
		{
			return false;
		}
		else if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 ) // Rivals
		{
			/*if ( gNonPK == FALSE )
			{
				return true;
			}*/
		}
		else if ( lpObj->lpGuild != NULL && lpTargetObj->lpGuild != NULL )
		{
			if ( lpObj->lpGuild->WarState == 1 && lpTargetObj->lpGuild->WarState == 1 )
			{
				if ( lpObj->lpGuild->Number == lpTargetObj->lpGuild->Number )
				{
					return false;
				}
			}
		}
		
		if ( gObjTargetGuildWarCheck(lpObj, lpTargetObj) == FALSE && lpTargetObj->lpGuild != NULL && lpTargetObj->lpGuild->WarState != 0)
		{
			if ( lpTargetObj->lpGuild->WarType == 1 )
			{
				if ( lpTargetObj->MapNumber != 6 )
				{
					/*if ( !gNonPK )
					{
						return true;
					}*/
				}
			}

			if ( gUser.ChaosCastleMapRange(lpTargetObj->MapNumber) == false )
			{
				return false;
			}
		}

		if ( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
		{
			if ( g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpObj->MapNumber)) != 2 )
			{
				return FALSE;
			}
		}
		/*else if ( gNonPK != FALSE )
		{
			return false;
		}*/

		if ( DS_MAP_RANGE(lpObj->MapNumber) ) // DEvil
		{
			return false;
		}

		if ( gUser.BloodCastleMapRange(lpObj->MapNumber) == true )
		{
			return false;
		}

		if ( gDuel.AreOnDuel(lpObj, lpTargetObj) == false )
		{
			if ( gDuel.IsOnDuel(lpTargetObj) == true )
			{
				return false;
			}
		}

		if ( gLanguage == 0 || gLanguage == 2 )
		{
			if (lpObj->m_PK_Level >= 6 )
			{
				if ( lpObj->m_PK_Count >= 3 )
				{
					/*if ( gPkLimitFree == FALSE )
					{
						return false;
					}*/
				}
			}
			
			if (lpTargetObj->m_PK_Level >= 6 )
			{
				if ( lpTargetObj->m_PK_Count >= 3 )
				{
					/*if ( gPkLimitFree == FALSE )
					{
#if(GS_CASTLE)
						if(g_CastleSiege.GetCastleState() != 7)
						{
							return false;
						}
#else
						return false;
#endif
					}*/
				}
			}

		}
	}
	return true;
}

bool CObjBaseAttack::ResistanceCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill)
{
	bool bResult = true;
	// ----
	switch ( skill )
	{
	// ---- Poison
	case 1:
		if ( lpTargetObj->m_PoisonType == 0 )
		{
			if ( retResistance(lpTargetObj, 1) == 0 )
			{
				lpTargetObj->m_PoisonType = 1;
				lpTargetObj->m_PoisonBeattackCount = 20;
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->m_ViewSkillState[eVS_POISON] = 1;
			}
			else
			{
				bResult = false;
			}	
		}
		else
		{
			bResult = false;
		}
		break;
	// ---- Lightning
	case 3:
		if ( retResistance(lpTargetObj, 2) == 0 )
		{
			gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150, 0);
		}
		else
		{
			bResult = false;
		}
		break;
		// ---- Decay
	case 38:
		if ( lpTargetObj->m_PoisonType == 0 || lpTargetObj->m_PoisonType == 1 )
		{
			if ( retResistance(lpTargetObj, 1) == 0 )
			{
				lpTargetObj->m_PoisonType = 38;
				lpTargetObj->m_PoisonBeattackCount = 10;
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->m_ViewSkillState[eVS_POISON] = 1;
				GCStateInfoSend(lpTargetObj, 1, eVS_POISON);				
			}
			else
			{
				bResult = false;
			}
		}
		else
		{
			bResult = false;
		}
		break;
	// ---- Earthquake
	case 62:
		gObjBackSpring2(lpTargetObj, lpObj, 3);
		break;
	// ---- Ice, Ice Storm, Chain Drive
	case 7:
	case 39:
	case 262:
		if (lpTargetObj->m_ColdBeattackCount == 0 )
		{
			if ( retResistance(lpTargetObj, 0) == 0 )
			{
				lpTargetObj->m_ColdBeattackCount = 10;
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->DelayActionTime = 800;
				lpTargetObj->DelayLevel = 1;
				lpTargetObj->m_IceType = skill;
				GCStateInfoSend(lpTargetObj, 1, eVS_ICE);
			}
			else
			{
				bResult = false;
			}
		}
		else
		{
			bResult = false;
		}
		break;
	// ---- Weapon Skills
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 260:
	case 261:
		gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150, 0);
		break;
	// ---- Ice Arrow
	case 51:
		bResult = false;
		break;
	}
	// ----
	return bResult;
}
//Done 100%
BOOL CObjBaseAttack::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL& bAllMiss)
{
	__try
	{
		int iAttackRate = 0;
		int iDefenseRate = lpTargetObj->m_SuccessfulBlocking;
		int iMSBDamage = 0;

		if ( lpObj->Type == OBJ_USER )
		{
			switch ( lpObj->Class )
			{
			case CLASS_WIZARD:
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
				break;
			case CLASS_KNIGHT:
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
				break;
			case CLASS_ELF:
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
				break;
			case CLASS_MAGUMSA:
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
				break;
			case CLASS_DARKLORD:
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 6) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4  + ((lpObj->Leadership + lpObj->AddLeadership)/10);
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
				break;
			case CLASS_SUMMONER:
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
				break;
			case CLASS_FIGHTER:
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
				break;
			}
		}
		else
		{
			iAttackRate = lpObj->m_AttackRating;
		}

		if ( iAttackRate < iDefenseRate )
		{
			bAllMiss = TRUE;
		}

		if ( bAllMiss != FALSE )
		{
			if ( (rand()%100) >= 5 )
			{
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, iMSBDamage, 0);

				if (magicsend != 0 )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
				}

				return FALSE;
			}
		}
		else
		{
			if ( (rand()%iAttackRate) < iDefenseRate)
			{
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, iMSBDamage, 0);

				if ( magicsend != 0 )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index,  skillSuccess);
				}

				return FALSE;
			}
		}
	}
	__except (lpObj->Level=1,-1)
	{
		CLog.LogAdd(lMsg.Get(MSGGET(2, 12)), lpObj->Name);
		return FALSE;
	}

	return TRUE;
}
//Done 100%
bool CObjBaseAttack::MissCheckPvP(LPOBJ lpObj , LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL & bAllMiss)
{
	float iAttackRate = 0;
	float iDefenseRate = 0;
	int iAttackSuccessRate = 0;

	// --------------------------------------------
	// Attack Rate
	// --------------------------------------------
	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 4 );		break;
	case CLASS_KNIGHT:		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 45) / 10;	break;
	case CLASS_ELF:			iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 6 ) / 10;	break;
	case CLASS_MAGUMSA:		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 35) / 10;	break;
	case CLASS_DARKLORD:	iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 4 );		break;
	case CLASS_SUMMONER:	iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 4 );		break;
	case CLASS_FIGHTER:		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 45) / 10;	break;
	}
	
	// --------------------------------------------
	// Defense Rate
	// --------------------------------------------
	switch ( lpTargetObj->Class )
	{
	case CLASS_WIZARD:		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 4;	break;
	case CLASS_KNIGHT:		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 2;	break;
	case CLASS_ELF:			iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 10;break;
	case CLASS_MAGUMSA:		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 4;	break;
	case CLASS_DARKLORD:	iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 2;	break;
	case CLASS_SUMMONER:	iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 4;	break;
	case CLASS_FIGHTER:		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 2;	break;
	}
	// ----
	if ( iAttackRate <= 0.0f || iDefenseRate <= 0.0f || lpObj->Level <= 0 || (DWORD)lpTargetObj <= 0 )
	{
		return false;
	}
	// --------------------------------------------
	// 380 & Jewel of Harmony PVP Options
	// --------------------------------------------
	iAttackRate += lpObj->m_ItemOptionExFor380.OpAddAttackSuccessRatePVP;
	iDefenseRate += lpTargetObj->m_ItemOptionExFor380.OpAddDefenseSuccessRatePvP;

	iAttackRate += lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP;
	iDefenseRate += lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddDefenseSuccessRatePvP;

	float iExpressionA = ( iAttackRate * 10000.0f ) / ( iAttackRate + iDefenseRate );
	float iExpressionB = ( lpObj->Level * 10000 ) / ( lpObj->Level + lpTargetObj->Level );

	iExpressionA /= 10000.0f;
	iExpressionB /= 10000.0f;

	iAttackSuccessRate = 100.0f * iExpressionA * g_fSuccessAttackRateOption * iExpressionB;

	if ( (lpTargetObj->Level - lpObj->Level) >= 100 )
	{
		iAttackSuccessRate -= 5;
	}
	else if ( (lpTargetObj->Level - lpObj->Level) >= 200 )
	{
		iAttackSuccessRate -= 10;
	}
	else if ( (lpTargetObj->Level - lpObj->Level) >= 300 )
	{
		iAttackSuccessRate -= 15;
	}

	int dwRate = rand() % 100;

	if ( dwRate > iAttackSuccessRate )
	{
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);

		if ( g_bShieldComboMissOptionOn == TRUE )
		{
			if ( lpObj->comboSkill.ProgressIndex >= 0 )
			{
				CLog.LogAdd("[Shield] ComboSkill Cancel! [%s][%s]", lpObj->AccountID, lpObj->Name);

				lpObj->comboSkill.Init();
			}
		}

		return false;
	}

	return true;
}
//Done 100%
int  CObjBaseAttack::GetTargetDefense(LPOBJ lpObj, LPOBJ lpTargetObj, unsigned char& MsgDamage)
{
	int skilldefense = lpTargetObj->m_SkillDefense;
	int targetdefense = lpTargetObj->m_Defense + skilldefense;

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		targetdefense += lpTargetObj->m_ItemOptionExFor380.OpAddDefense / 2;
	}

	targetdefense -= (targetdefense * lpTargetObj->m_SkillMagumReduceDefense)/100;
	
	if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefenseTime > 0 )
	{
		targetdefense += lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefense;

		if ( targetdefense <0 )
			targetdefense = 0;

	}

	int percentdamage = 0;

	if ( lpObj->pInventory[7].IsItem() != FALSE )
	{
		percentdamage = lpObj->pInventory[7].IsWingOpGetOnePercentDamage();
	}

	if ( lpObj->pInventory[7].IsItem() != FALSE &&
		((lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <= ITEMGET(12,40)) ||  
		lpObj->pInventory[7].m_Type == ITEMGET(12,43) || 
		lpObj->pInventory[7].m_Type == ITEMGET(12,50)) &&
		lpObj->pInventory[7].CheckThirdWingsSuccessfulBlocking() )
	{
		percentdamage += 5;
	}

	percentdamage += lpObj->SetOpIgnoreDefense;

	if ( percentdamage != 0)
	{
		if ( (rand()%100) <= percentdamage)
		{
			targetdefense = 0;
			MsgDamage = 1;
		}
	}

	return targetdefense;
}
//Done 100%
int  CObjBaseAttack::GetPartyMemberCount(LPOBJ lpObj)
{
	LPOBJ lpPartyObj;
	int partynum = lpObj->PartyNumber;

	if ( OBJMAX_RANGE(partynum) == FALSE )
	{
		CLog.LogAdd("Error : DecPartyMemberHPandMP %s %d", __FILE__, __LINE__);
		return 0;
	}

	int partycount = gParty.m_PartyInfo[partynum].Count;
	int retcount = 0;

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		int memberindex = gParty.m_PartyInfo[partynum].Number[n];

		if ( memberindex >= 0 )
		{
			lpPartyObj = &gObj[memberindex];

			if ( lpObj->MapNumber == lpPartyObj->MapNumber )
			{
				int dis = gObjCalDistance(lpObj, &gObj[memberindex]);
				
				if ( dis < MAX_PARTY_DISTANCE_EFFECT )
				{
					retcount++;
				}
			}
		}
	}

	return retcount;
}