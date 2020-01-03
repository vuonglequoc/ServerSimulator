//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development � 2010							//
// Description: MU Object Cal Character Structures & Functions			//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "ObjCalCharacter.h"
#include "Gamemain.h"
#include "DarkSpirit.h"
#include "ItemAddOption.h"
#include "SetItemOption.h"
#include "LogProc.H"
#include "MasterSkillSystem.h"

CCalCharacter gCalCharacter;

CCalCharacter::CCalCharacter(void)
{
	// ----
}

CCalCharacter::~CCalCharacter(void)
{
	// ----
}

void CCalCharacter::gObjCalCharacter(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int Strength = 0;
	int Dexterity = 0;
	int Vitality = 0;
	int Energy = 0;
	int Leadership = 0;
	// ----
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left  = &lpObj->pInventory[1];
	CItem * Helm  = &lpObj->pInventory[2];
	CItem * Armor  = &lpObj->pInventory[3];
	CItem * Pants  = &lpObj->pInventory[4];
	CItem * Gloves = &lpObj->pInventory[5];
	CItem * Boots = &lpObj->pInventory[6];
	CItem * Wings = &lpObj->pInventory[7];
	CItem * Helper = &lpObj->pInventory[8];
	CItem * Amulet = &lpObj->pInventory[9];
	CItem * Ring01 = &lpObj->pInventory[10];
	CItem * Ring02 = &lpObj->pInventory[11];
	lpObj->HaveWeaponInHand = true;
	// -------------------------------------------------------
	// Check Weapon in hand
	// -------------------------------------------------------
	if ( Right->IsItem() == false && Left->IsItem() == false )
	{
		lpObj->HaveWeaponInHand = false;
	}
	else if ( Left->IsItem() == false && Right->m_Type == ITEMGET(4,15) ) // Arrow
	{
		lpObj->HaveWeaponInHand = false;
	}
	else if ( Right->IsItem() == false )
	{
		int iType = Left->m_Type / MAX_SUBTYPE_ITEMS;

		if ( Left->m_Type == ITEMGET(4,7) ) // Bolt
		{
			lpObj->HaveWeaponInHand = false;
		}
		else if ( iType == 6 ) // Shield
		{
			lpObj->HaveWeaponInHand = false;
		}
	}
	// -------------------------------------------------------
	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife = 0;
	lpObj->MonsterDieGetMana = 0;
	lpObj->DamageReflect = 0;
	lpObj->DamageMinus = 0;
	lpObj->SkillLongSpearChange = false;

	if ( lpObj->m_iItemEffectValidTime > 0 )
	{
		g_ItemAddOption.PrevSetItemLastEffectForHallowin(lpObj);
	}

	int iItemIndex;
	BOOL bIsChangeItem;	// lc34

	for ( iItemIndex=0; iItemIndex<MAX_PLAYER_EQUIPMENT;iItemIndex++)
	{
		if ( lpObj->pInventory[iItemIndex].IsItem() != FALSE )
		{
			lpObj->pInventory[iItemIndex].m_IsValidItem = true;
		}
	}

	do
	{
		lpObj->SetOpAddMaxAttackDamage = 0;
		lpObj->SetOpAddMinAttackDamage = 0;
		lpObj->SetOpAddDamage = 0;
		lpObj->SetOpIncAGValue = 0;
		lpObj->SetOpAddCriticalDamageSuccessRate = 0;
		lpObj->SetOpAddCriticalDamage = 0;
		lpObj->SetOpAddExDamageSuccessRate = 0;
		lpObj->SetOpAddExDamage = 0;
		lpObj->SetOpAddSkillAttack = 0;
		lpObj->AddStrength = 0;
		lpObj->AddDexterity = 0;
		lpObj->AddVitality = 0;
		lpObj->AddEnergy = 0;
		lpObj->AddBP = 0;
		lpObj->iAddShield = 0;
		lpObj->SetOpAddAttackDamage = 0;
		lpObj->SetOpAddDefence = 0;
		lpObj->SetOpAddMagicPower = 0;
		lpObj->SetOpAddDefenceRate = 0;
		lpObj->SetOpIgnoreDefense = 0;
		lpObj->SetOpDoubleDamage = 0;
		lpObj->SetOpTwoHandSwordImproveDamage = 0;
		lpObj->SetOpImproveSuccessAttackRate = 0;
		lpObj->SetOpReflectionDamage = 0;
		lpObj->SetOpImproveSheldDefence = 0;
		lpObj->SetOpDecreaseAG = 0;
		lpObj->SetOpImproveItemDropRate = 0;
		lpObj->IsFullSetItem = false;
		memset(lpObj->m_AddResistance, 0, sizeof(lpObj->m_AddResistance));
		bIsChangeItem = 0;
		gObjCalcSetItemStat(lpObj);
		gObjCalcSetItemOption(lpObj);

		for (iItemIndex=0;iItemIndex<MAX_PLAYER_EQUIPMENT;iItemIndex++)
		{
			if ( lpObj->pInventory[iItemIndex].IsItem() != FALSE && lpObj->pInventory[iItemIndex].m_IsValidItem != false )
			{
				if ( gObjValidItem( lpObj, &lpObj->pInventory[iItemIndex], iItemIndex) != FALSE )
				{
					lpObj->pInventory[iItemIndex].m_IsValidItem = true;
				}
				else
				{
					lpObj->pInventory[iItemIndex].m_IsValidItem  = false;
					bIsChangeItem = TRUE;
				}
			}
		}
	}
	while ( bIsChangeItem != FALSE );

	Strength = lpObj->Strength + lpObj->AddStrength;
	Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
	Vitality = lpObj->Vitality + lpObj->AddVitality;
	Energy = lpObj->Energy + lpObj->AddEnergy;
	Leadership = lpObj->Leadership + lpObj->AddLeadership;

	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:
		lpObj->m_AttackDamageMinRight = (Strength / 8);
		lpObj->m_AttackDamageMaxRight = (Strength / 4);
		lpObj->m_AttackDamageMinLeft = (Strength / 8);
		lpObj->m_AttackDamageMaxLeft = (Strength / 4);
		break;
	case CLASS_KNIGHT:
		lpObj->m_AttackDamageMinRight = Strength / 6;
		lpObj->m_AttackDamageMaxRight = Strength / 4;
		lpObj->m_AttackDamageMinLeft = Strength / 6;
		lpObj->m_AttackDamageMaxLeft = Strength / 4;
		break;
	case CLASS_ELF:
		if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) ) ||
			 (Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7)) ||
			  Right->m_Type == ITEMGET(4,16) ||
			  Left->m_Type == ITEMGET(4,17) ||
			  Right->m_Type == ITEMGET(4,18) ||
			  Right->m_Type == ITEMGET(4,19) ||
			  Left->m_Type == ITEMGET(4,20) ||
			  Left->m_Type == ITEMGET(4,21) ||
			  Left->m_Type == ITEMGET(4,22) ||
			  Left->m_Type == ITEMGET(4,23) ||
			  Left->m_Type == ITEMGET(4,24) )
		{
			if ( (Right->IsItem() != FALSE && Right->m_IsValidItem == false) || (Left->IsItem() != FALSE && Left->m_IsValidItem == false) )
			{
				lpObj->m_AttackDamageMinRight = (Dexterity + Strength ) / 7;
				lpObj->m_AttackDamageMaxRight = (Dexterity + Strength ) / 4;
				lpObj->m_AttackDamageMinLeft = (Dexterity + Strength ) / 7;
				lpObj->m_AttackDamageMaxLeft = (Dexterity + Strength ) / 4;
			}
			else
			{
				lpObj->m_AttackDamageMinRight = (Dexterity / 7) + (Strength / 14);
				lpObj->m_AttackDamageMaxRight = (Dexterity / 4) + (Strength / 8 );
				lpObj->m_AttackDamageMinLeft = (Dexterity / 7) + (Strength / 14);
				lpObj->m_AttackDamageMaxLeft = (Dexterity / 4) + (Strength / 8 );
			}
		}
		else
		{
			lpObj->m_AttackDamageMinRight = (Dexterity + Strength) / 7;
			lpObj->m_AttackDamageMaxRight = (Dexterity + Strength) / 4;
			lpObj->m_AttackDamageMinLeft = (Dexterity + Strength) / 7;
			lpObj->m_AttackDamageMaxLeft = (Dexterity + Strength) / 4;
		}
		break;
	case CLASS_MAGUMSA:
		lpObj->m_AttackDamageMinRight = (Strength / 6) + (Energy / 12);
		lpObj->m_AttackDamageMaxRight = (Strength / 4) + (Energy / 8);
		lpObj->m_AttackDamageMinLeft = (Strength / 6) + (Energy / 12);
		lpObj->m_AttackDamageMaxLeft = (Strength / 4) + (Energy / 8);
		break;
	case CLASS_DARKLORD:
		lpObj->m_AttackDamageMinRight = (Strength / 7) + (Energy / 14);
		lpObj->m_AttackDamageMaxRight = (Strength / 5) + (Energy / 10);
		lpObj->m_AttackDamageMinLeft = (Strength / 7) + (Energy / 14);
		lpObj->m_AttackDamageMaxLeft = (Strength / 5) + (Energy / 10);
		break;
	case CLASS_SUMMONER:
		lpObj->m_AttackDamageMinRight = (Strength / 8) + (Energy / 12);
		lpObj->m_AttackDamageMaxRight = (Strength / 4) + (Energy / 8);
		lpObj->m_AttackDamageMinLeft = (Strength / 8) + (Energy / 12);
		lpObj->m_AttackDamageMaxLeft = (Strength / 4) + (Energy / 8);
		break;
	case CLASS_FIGHTER:
		lpObj->m_AttackDamageMinRight = (Strength / 6) + (Energy / 12);
		lpObj->m_AttackDamageMaxRight = (Strength / 4) + (Energy / 8);
		lpObj->m_AttackDamageMinLeft = (Strength / 6) + (Energy / 12);
		lpObj->m_AttackDamageMaxLeft = (Strength / 4) + (Energy / 8);
		break;
	}
	
	if ( Wings->IsItem() != FALSE && Wings->m_IsValidItem != false ) 
	{
		Wings->PlusSpecial(&lpObj->m_AttackDamageMinRight, 80);
		Wings->PlusSpecial(&lpObj->m_AttackDamageMaxRight, 80);
		Wings->PlusSpecial(&lpObj->m_AttackDamageMinLeft, 80);
		Wings->PlusSpecial(&lpObj->m_AttackDamageMaxLeft, 80);
	}

	int AddLeadership = 0;

	if ( Wings->IsItem() != FALSE && Wings->m_IsValidItem != false)
	{
		AddLeadership += Wings->m_Leadership;
	}

	if ( Right->m_Type != -1 )
	{
		if ( Right->m_IsValidItem != false )
		{
			if ( Right->m_Type >= ITEMGET(5,0) && Right->m_Type <= ITEMGET(6,0) ) // Check this
			{
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin / 2;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax / 2;
			}
			else
			{
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax;
			}
		}

		if ( Right->m_SkillChange != FALSE )
		{
			lpObj->SkillLongSpearChange = true;
		}

		Right->PlusSpecial(&lpObj->m_AttackDamageMinRight, 80);
		Right->PlusSpecial(&lpObj->m_AttackDamageMaxRight, 80);
	}
		
	if ( Left->m_Type != -1 )
	{
		if ( Left->m_IsValidItem != false)
		{
			lpObj->m_AttackDamageMinLeft += Left->m_DamageMin;
			lpObj->m_AttackDamageMaxLeft += Left->m_DamageMax;
		}

		Left->PlusSpecial(&lpObj->m_AttackDamageMinLeft, 80);
		Left->PlusSpecial(&lpObj->m_AttackDamageMaxLeft, 80);
	}

	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;

	if ( Wings->IsItem() != FALSE && Wings->m_IsValidItem != false )
	{
		Wings->PlusSpecial(&lpObj->m_MagicDamageMin, 81);
		Wings->PlusSpecial(&lpObj->m_MagicDamageMax, 81);
		Wings->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	if ( Right->IsItem() != FALSE && Right->m_IsValidItem != false )
	{
		Right->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	if ( Left->IsItem() != FALSE && Left->m_IsValidItem != false )
	{
		Left->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	if ( Helm->IsItem() != FALSE && Helm->m_IsValidItem != false )
	{
		Helm->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	if ( Armor->IsItem() != FALSE && Armor->m_IsValidItem != false )
	{
		Armor->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	if ( Pants->IsItem() != FALSE && Pants->m_IsValidItem != false )
	{
		Pants->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	if ( Gloves->IsItem() != FALSE && Gloves->m_IsValidItem != false )
	{
		Gloves->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	if ( Boots->IsItem() != FALSE && Boots->m_IsValidItem != false )
	{
		Boots->PlusSpecial(&lpObj->m_CriticalDamage, 84);
	}
	lpObj->m_MagicDamageMin = Energy / 9;
	lpObj->m_MagicDamageMax = Energy / 4;


	if ( Right->m_Type != -1 )
	{
		if ( Right->m_Type == ITEMGET(0,31) ||
			 Right->m_Type == ITEMGET(0,21) ||
			 Right->m_Type == ITEMGET(0,23) ||
			 Right->m_Type == ITEMGET(0,25) ||
			 Right->m_Type == ITEMGET(0,28) ) 
		{
			Right->PlusSpecial(&lpObj->m_MagicDamageMin, 80);
			Right->PlusSpecial(&lpObj->m_MagicDamageMax, 80);
		}
		else
		{
			Right->PlusSpecial(&lpObj->m_MagicDamageMin, 81);
			Right->PlusSpecial(&lpObj->m_MagicDamageMax, 81);
		}
	}

	lpObj->m_AttackRating = (Strength + Dexterity) / 2;
	if (lpObj->Class != CLASS_FIGHTER) // Rage Fighter - No Gloves
	{
		if ( Gloves->IsItem() != FALSE && Gloves->m_IsValidItem != false )
		{
			lpObj->m_AttackRating += Gloves->ItemDefense();
		}
	}
	// ----------------------------------------
	// Attack Speed
	// ---------------------------------------- 
	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:
		lpObj->m_AttackSpeed = Dexterity / 20;
		lpObj->m_MagicSpeed = Dexterity / 10;
		break;
	case CLASS_KNIGHT:
		lpObj->m_AttackSpeed = Dexterity / 15;
		lpObj->m_MagicSpeed = Dexterity / 20;
		break;
	case CLASS_ELF:
		lpObj->m_AttackSpeed = Dexterity / 50;
		lpObj->m_MagicSpeed = Dexterity / 50;
		break;
	case CLASS_MAGUMSA:
		lpObj->m_AttackSpeed = Dexterity / 15;
		lpObj->m_MagicSpeed = Dexterity / 20;
		break;
	case CLASS_DARKLORD:
		lpObj->m_AttackSpeed = Dexterity / 10;
		lpObj->m_MagicSpeed = Dexterity / 10;
		break;
	case CLASS_SUMMONER:
		lpObj->m_AttackSpeed = Dexterity / 20;
		lpObj->m_MagicSpeed = Dexterity / 20;
		break;
	case CLASS_FIGHTER:
		lpObj->m_AttackSpeed = Dexterity / 15;
		lpObj->m_MagicSpeed = Dexterity / 20;
		break;
	}
	
	bool bRight = false;
	bool bLeft = false;
	// ---------------------------------------- 
	// Check if have weapon in Right Hand
	// ---------------------------------------- 
	if ( Right->m_Type != ITEMGET(4,7) && Right->m_Type != ITEMGET(4,15) && Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(6,0) )
	{
		if ( Right->m_IsValidItem != false )
		{
			bRight = true;
		}
	}
	// ---------------------------------------- 
	// Check if have weapon in Left Hand
	// ---------------------------------------- 
	if ( Left->m_Type != ITEMGET(4,7) && Left->m_Type != ITEMGET(4,15) && Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(6,0) )
	{
		if ( Left->m_IsValidItem != false )
		{
			bLeft = true;
		}
	}

	if ( bRight != false && bLeft != false )
	{
		lpObj->m_AttackSpeed += (Right->m_AttackSpeed + Left->m_AttackSpeed)/2;
		lpObj->m_MagicSpeed += (Right->m_AttackSpeed + Left->m_AttackSpeed)/2;
	}
	else if ( bRight!= false )
	{
		lpObj->m_AttackSpeed += Right->m_AttackSpeed;
		lpObj->m_MagicSpeed += Right->m_AttackSpeed;
	}
	else if ( bLeft != false )
	{
		lpObj->m_AttackSpeed += Left->m_AttackSpeed;
		lpObj->m_MagicSpeed += Left->m_AttackSpeed;
	}

	if ( lpObj->Class != CLASS_FIGHTER ) // Rage Fighter - No Gloves
	{
		if ( Gloves->m_Type != -1 )
		{
			if ( Gloves->m_IsValidItem != false )
			{
				lpObj->m_AttackSpeed += Gloves->m_AttackSpeed;
				lpObj->m_MagicSpeed += Gloves->m_AttackSpeed;
			}
		}
	}

	if ( Amulet->m_Type != -1 ) // Pendant
	{
		if ( Amulet->m_IsValidItem != false )
		{
			lpObj->m_AttackSpeed += Amulet->m_AttackSpeed;
			lpObj->m_MagicSpeed += Amulet->m_AttackSpeed;
		}
	}
	// ----------------------------------------
	// Wizard Ring Effect
	// ----------------------------------------
	if ( (lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,20) && lpObj->pInventory[10].m_Level == 0 && lpObj->pInventory[10].m_Durability > 0.0f ) || (lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,20) && lpObj->pInventory[11].m_Level == 0 && lpObj->pInventory[11].m_Durability > 0.0f ) )
	{
		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * 10 / 100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * 10 / 100;
		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * 10 / 100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * 10 / 100;
		lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * 10 / 100;
		lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * 10 / 100;
		lpObj->m_AttackSpeed += 10;
		lpObj->m_MagicSpeed += 10;
	}
	// ----------------------------------------
	// Speed Hack Detect
	// ----------------------------------------
	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_MagicSpeed*2 * gDecTimePerAttackSpeed) );
		break;
	case CLASS_KNIGHT:
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_AttackSpeed * gDecTimePerAttackSpeed) );
		break;
	case CLASS_ELF:
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_AttackSpeed * gDecTimePerAttackSpeed) );
		break;
	case CLASS_MAGUMSA:
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_AttackSpeed * gDecTimePerAttackSpeed) );
		break;
	case CLASS_DARKLORD:
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_AttackSpeed * gDecTimePerAttackSpeed) );
		break;
	case CLASS_SUMMONER:
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_MagicSpeed*2 * gDecTimePerAttackSpeed) );
		break;
	case CLASS_FIGHTER:
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_MagicSpeed*2 * gDecTimePerAttackSpeed) );
		break;
	}
	
	if ( lpObj->m_DetectSpeedHackTime < gMinimumAttackSpeedTime )
	{
		lpObj->m_DetectSpeedHackTime = gMinimumAttackSpeedTime;
	}
	// ----------------------------------------
	// Successful Blocking
	// ----------------------------------------
	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:		lpObj->m_SuccessfulBlocking = Dexterity / 3;	break;
	case CLASS_KNIGHT:		lpObj->m_SuccessfulBlocking = Dexterity / 3;	break;
	case CLASS_ELF:			lpObj->m_SuccessfulBlocking = Dexterity / 4;	break;
	case CLASS_MAGUMSA:		lpObj->m_SuccessfulBlocking = Dexterity / 3;	break;
	case CLASS_DARKLORD:	lpObj->m_SuccessfulBlocking = Dexterity / 7;	break;
	case CLASS_SUMMONER:	lpObj->m_SuccessfulBlocking = Dexterity / 3;	break;
	case CLASS_FIGHTER:		lpObj->m_SuccessfulBlocking = Dexterity / 5;	break;
	}
	
	if ( Left->m_Type != -1 )
	{
		if ( Left->m_IsValidItem != false )
		{
			lpObj->m_SuccessfulBlocking += Left->m_SuccessfulBlocking;
			Left->PlusSpecial(&lpObj->m_SuccessfulBlocking, 82);
		}
	}
	// ----------------------------------------
	// Character Set & Succesful Blocking
	// ----------------------------------------
	bool Success = true;

	if ( lpObj->Class == CLASS_MAGUMSA ) // Magic Gladiator (No Helmet)
	{
		for ( int j=3;j<=6;j++)
		{
			if ( lpObj->pInventory[j].m_Type == -1 )
			{
				Success = false;
				break;
			}

			if ( lpObj->pInventory[j].m_IsValidItem == false )
			{
				Success = false;
				break;
			}
		}
	}
	else if ( lpObj->Class == CLASS_FIGHTER ) // Rage Fighter (No Gloves)
	{
		for ( int l=2;l<6;l++)
		{
			if ( l != 5 )
			{
				if ( lpObj->pInventory[l].m_Type == -1 )
				{
					Success = false;
					break;
				}

				if ( lpObj->pInventory[l].m_IsValidItem == false )
				{
					Success = false;
					break;
				}
			}
		}

	}
	else // Dark Wizard, Dark Knight, Dark Lord, Elf, Summoner (All Set)
	{
		for  ( int k=2;k<=6;k++)
		{
			if ( lpObj->pInventory[k].m_Type == -1 )
			{
				Success = false;
				break;
			}

			if ( lpObj->pInventory[k].m_IsValidItem == false )
			{
				Success = false;
				break;
			}
		}
	}

	int Level11Count = 0;
	int Level10Count = 0;
	int Level12Count = 0;
	int Level13Count = 0;
	int Level14Count = 0;
	int Level15Count = 0;

	if ( Success != false )
	{
		int in;

		if ( lpObj->Class == CLASS_MAGUMSA ) // Magic Gladiator (No Helmet)
		{
			in = Armor->m_Type % MAX_SUBTYPE_ITEMS;

			if ( in != ITEMGET(0,15) &&
				 in != ITEMGET(0,20) &&
				 in != ITEMGET(0,23) &&
				 in != ITEMGET(0,32) &&
				 in != ITEMGET(0,37) )
			{
				Success = false;
			}
			else
			{
				Level15Count++;

				for (int m=3;m<=6;m++)
				{
					if ( in != ( lpObj->pInventory[m].m_Type % MAX_SUBTYPE_ITEMS) )
					{
						Success = false;
					}
					if ( lpObj->pInventory[m].m_Level > 14 )
					{
						Level15Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 13 )
					{
						Level14Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 12 )
					{
						Level13Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 11 )
					{
						Level12Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 10 )
					{
						Level11Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 9 )
					{
						Level10Count++;
					}
				}
			}
		}
		if ( lpObj->Class == CLASS_FIGHTER ) // Rage Fighter (No Gloves)
		{
			in = Boots->m_Type % MAX_SUBTYPE_ITEMS;

			if ( in != ITEMGET(0,59) &&
				 in != ITEMGET(0,60) &&
				 in != ITEMGET(0,61) )
			{
				Success = false;
			}
			else
			{
				Level15Count++;

				for (int m=2;m<=6;m++)
				{
					if (m != 5)
					{
						if ( in != ( lpObj->pInventory[m].m_Type % MAX_SUBTYPE_ITEMS) )
						{
							Success = false;
						}
						if ( lpObj->pInventory[m].m_Level > 14 )
						{
							Level15Count++;
						}
						else if ( lpObj->pInventory[m].m_Level > 13 )
						{
							Level14Count++;
						}
						else if ( lpObj->pInventory[m].m_Level > 12 )
						{
							Level13Count++;
						}
						else if ( lpObj->pInventory[m].m_Level > 11 )
						{
							Level12Count++;
						}
						else if ( lpObj->pInventory[m].m_Level > 10 )
						{
							Level11Count++;
						}
						else if ( lpObj->pInventory[m].m_Level > 9 )
						{
							Level10Count++;
						}
					}
				}
			}
		}
		else // Dark Knight, Dark Wizard, Elf, Dark Lord, Summoner (All Set)
		{
			in = lpObj->pInventory[2].m_Type % MAX_SUBTYPE_ITEMS;

			for (int m=2;m<=6;m++)
			{
				if ( in !=  ( lpObj->pInventory[m].m_Type % MAX_SUBTYPE_ITEMS) )
				{
					Success = false;
				}
				if ( lpObj->pInventory[m].m_Level > 14 )
				{
					Level15Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 13 )
				{
					Level14Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 12 )
				{
					Level13Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 11 )
				{
					Level12Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 10 )
				{
					Level11Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 9 )
				{
					Level10Count++;
				}
			}
		}

		if ( Success != false )	// #warning unuseful if
		{
			lpObj->m_SuccessfulBlocking += lpObj->m_SuccessfulBlocking / 10;
		}
	}
	// ----------------------------------------
	// Character Defense
	// ----------------------------------------
	switch ( lpObj->Class )
	{
	case CLASS_WIZARD:		lpObj->m_Defense = Dexterity / 4;	break;
	case CLASS_KNIGHT:		lpObj->m_Defense = Dexterity / 3;	break;
	case CLASS_ELF:			lpObj->m_Defense = Dexterity / 10;	break;
	case CLASS_MAGUMSA:		lpObj->m_Defense = Dexterity / 4;	break;
	case CLASS_DARKLORD:	lpObj->m_Defense = Dexterity / 7;	break;
	case CLASS_SUMMONER:	lpObj->m_Defense = Dexterity / 4;	break;
	case CLASS_FIGHTER:		lpObj->m_Defense = Dexterity / 4;	break;
	}
	
	if ( lpObj->Class == CLASS_MAGUMSA ) // Magic Gladiator (No Helm)
	{
		lpObj->m_Defense += Armor->ItemDefense();
		lpObj->m_Defense += Pants->ItemDefense();
		lpObj->m_Defense += Gloves->ItemDefense();
		lpObj->m_Defense += Boots->ItemDefense();
		lpObj->m_Defense += Left->ItemDefense();
		lpObj->m_Defense += Wings->ItemDefense();
	}
	else if ( lpObj->Class == CLASS_FIGHTER ) // Rage Fighter (No Gloves)
	{
		lpObj->m_Defense += Helm->ItemDefense();
		lpObj->m_Defense += Armor->ItemDefense();
		lpObj->m_Defense += Pants->ItemDefense();
		lpObj->m_Defense += Boots->ItemDefense();
		lpObj->m_Defense += Left->ItemDefense();
		lpObj->m_Defense += Wings->ItemDefense();
	}
	else // Dark Wizard, Dark Knight, Elf, Dark Lord, Summoner (All Set)
	{
		lpObj->m_Defense += Helm->ItemDefense();
		lpObj->m_Defense += Armor->ItemDefense();
		lpObj->m_Defense += Pants->ItemDefense();
		lpObj->m_Defense += Gloves->ItemDefense();
		lpObj->m_Defense += Boots->ItemDefense();
		lpObj->m_Defense += Left->ItemDefense();
		lpObj->m_Defense += Wings->ItemDefense();
	}

	if ( Helper->IsItem() != FALSE )
	{
		if ( Helper->m_Type == ITEMGET(13,4) && Helper->m_Durability > 0.0f ) // Dark Horse
		{
			lpObj->m_Defense += INT( Dexterity / 20 + 5 + Helper->m_PetItem_Level * 2 );
		}
	}

	if ( (Level15Count + Level14Count + Level13Count + Level12Count + Level11Count + Level10Count) >= 5 )
	{
		if ( Success != false )
		{
			if ( Level15Count == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 30 / 100;
			}
			else if ( Level14Count == 5 || (Level14Count + Level15Count) == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 25 / 100;
			}
			else if ( Level13Count == 5 || (Level13Count + Level14Count + Level15Count) == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 20 / 100;
			}
			else if ( Level12Count == 5 || (Level12Count + Level13Count + Level14Count + Level15Count) == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 15 / 100;
			}
			else if ( Level11Count == 5 || (Level11Count + Level12Count + Level13Count + Level14Count + Level15Count) == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 10 / 100;
			}
			else if ( Level10Count == 5 || (Level10Count + Level11Count + Level12Count + Level13Count + Level14Count + Level15Count) == 5)
			{
				lpObj->m_Defense += lpObj->m_Defense * 5 / 100;
			}
		}
	}

	lpObj->m_Defense = lpObj->m_Defense * 10 / 20;
	// ----------------------------------------
	// Transformation Rings
	// ----------------------------------------
	if ( lpObj->m_Change == 9 ) // Thunder Lich Change
	{
		// Add Magic Power Here
	}
	else if ( lpObj->m_Change == 41 ) // Death Cow Change
	{
		lpObj->AddLife = ((int)(lpObj->MaxLife * 20.0f))/100;
	}
	else if ( lpObj->m_Change == 372 ) // Don't Know
	{
		lpObj->m_Defense += lpObj->m_Defense / 10;
		lpObj->AddLife += lpObj->Level;
	}
	// ----------------------------------------
	// Pets
	// ----------------------------------------
	if ( Helper->m_Type == ITEMGET(13,0) ) // Guardian Angel
	{
		lpObj->AddLife += 50;
	}
	else if ( Helper->m_Type == ITEMGET(13,1) ) // Imp
	{
		//Add 30% to Attack
		lpObj->m_AttackDamageMinLeft	+= (lpObj->m_AttackDamageMinLeft*30)/100;
		lpObj->m_AttackDamageMaxLeft	+= (lpObj->m_AttackDamageMaxLeft*30)/100;
		lpObj->m_AttackDamageMinRight	+= (lpObj->m_AttackDamageMinRight*30)/100;
		lpObj->m_AttackDamageMaxRight	+= (lpObj->m_AttackDamageMaxRight*30)/100;
		//Add 30% to Magic Attack
		lpObj->m_MagicDamageMin			+= (lpObj->m_MagicDamageMin*30)/100;
		lpObj->m_MagicDamageMax			+= (lpObj->m_MagicDamageMax*30)/100;
	}
	else if ( Helper->m_Type == ITEMGET(13,3) ) // Dinorant
	{
		//Helper->PlusSpecial(&lpObj->AddBP, 103 );
		//Add 15% to Attack
		lpObj->m_AttackDamageMinLeft	+= (lpObj->m_AttackDamageMinLeft*15)/100;
		lpObj->m_AttackDamageMaxLeft	+= (lpObj->m_AttackDamageMaxLeft*15)/100;
		lpObj->m_AttackDamageMinRight	+= (lpObj->m_AttackDamageMinRight*15)/100;
		lpObj->m_AttackDamageMaxRight	+= (lpObj->m_AttackDamageMaxRight*15)/100;
		//Add 15% to Magic Attack
		lpObj->m_MagicDamageMin			+= (lpObj->m_MagicDamageMin*15)/100;
		lpObj->m_MagicDamageMax			+= (lpObj->m_MagicDamageMax*15)/100;
	}
	else if ( Helper->m_Type == ITEMGET(13,64) ) // Demon
	{
		//Add 40% to Attack
		lpObj->m_AttackDamageMinLeft	+= (lpObj->m_AttackDamageMinLeft*40)/100;
		lpObj->m_AttackDamageMaxLeft	+= (lpObj->m_AttackDamageMaxLeft*40)/100;
		lpObj->m_AttackDamageMinRight	+= (lpObj->m_AttackDamageMinRight*40)/100;
		lpObj->m_AttackDamageMaxRight	+= (lpObj->m_AttackDamageMaxRight*40)/100;
		//Add +10 to Attack Speed
		lpObj->m_MagicSpeed += 10;
		lpObj->m_AttackSpeed += 10;
		//Add 40% to Magic Attack
		lpObj->m_MagicDamageMin			+= (lpObj->m_MagicDamageMin*40)/100;
		lpObj->m_MagicDamageMax			+= (lpObj->m_MagicDamageMax*40)/100;
	}
	else if ( Helper->m_Type == ITEMGET(13,65) ) // Spirit Guardian
	{
		lpObj->AddLife					+= 50;
	}
	else if ( Helper->m_Type == ITEMGET(13,80) ) // Pet Panda
	{
		lpObj->m_Defense				+= 50;
	}
	else if ( Helper->m_Type == ITEMGET(13,123) ) // Skeleton Pet
	{
		//Add 20% to Attack
		lpObj->m_AttackDamageMinLeft	+= (lpObj->m_AttackDamageMinLeft*20)/100;
		lpObj->m_AttackDamageMaxLeft	+= (lpObj->m_AttackDamageMaxLeft*20)/100;
		lpObj->m_AttackDamageMinRight	+= (lpObj->m_AttackDamageMinRight*20)/100;
		lpObj->m_AttackDamageMaxRight	+= (lpObj->m_AttackDamageMaxRight*20)/100;
		//Add +10 to Attack Speed
		lpObj->m_MagicSpeed += 10;
		lpObj->m_AttackSpeed += 10;
		//Add 20% to Magic Attack
		lpObj->m_MagicDamageMin			+= (lpObj->m_MagicDamageMin*20)/100;
		lpObj->m_MagicDamageMax			+= (lpObj->m_MagicDamageMax*20)/100;
	}
	int addlife = 0;
	int addmana = 0;

	Wings->PlusSpecial(&addlife, 100);
	Wings->PlusSpecial(&addmana, 101);
	Wings->PlusSpecial(&AddLeadership, 105);
	lpObj->AddLeadership = AddLeadership;
	lpObj->AddLife += addlife;
	lpObj->AddMana += addmana;

	if ( lpObj->Type == OBJ_USER )
	{
		gDarkSpirit[lpObj->m_Index].Set(lpObj->m_Index, &lpObj->pInventory[1]);
	}

	Amulet->PlusSpecialPercentEx(&lpObj->AddBP, lpObj->MaxBP, 173);
	Ring01->PlusSpecialPercentEx(&lpObj->AddMana, lpObj->MaxMana, 172);
	Ring02->PlusSpecialPercentEx(&lpObj->AddMana, lpObj->MaxMana, 172);

	CItem * rItem[3];
	int comparecount = 0;
	
	rItem[0] = &lpObj->pInventory[10];
	rItem[1] = &lpObj->pInventory[11];
	rItem[2] = &lpObj->pInventory[9];

#define GET_MAX_RESISTANCE(x,y,z) ( ( ( ( (x) > (y) ) ? (x) : (y) ) > (z) ) ? ( ( (x) > (y) ) ? (x) : (y) ) : (z) )	

	lpObj->m_Resistance[1] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[1], rItem[1]->m_Resistance[1], rItem[2]->m_Resistance[1]);
	lpObj->m_Resistance[0] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[0], rItem[1]->m_Resistance[0], rItem[2]->m_Resistance[0]);
	lpObj->m_Resistance[2] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[2], rItem[1]->m_Resistance[2], rItem[2]->m_Resistance[2]);
	lpObj->m_Resistance[3] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[3], rItem[1]->m_Resistance[3], rItem[2]->m_Resistance[3]);
	lpObj->m_Resistance[4] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[4], rItem[1]->m_Resistance[4], rItem[2]->m_Resistance[4]);
	lpObj->m_Resistance[5] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[5], rItem[1]->m_Resistance[5], rItem[2]->m_Resistance[5]);
	lpObj->m_Resistance[6] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[6], rItem[1]->m_Resistance[6], rItem[2]->m_Resistance[6]);


	g_Item380System.ApplyFor380Option(lpObj);
	g_kJewelOfHarmonySystem.SetApplyStrengthenItem(lpObj);
	gItemSocket.ApplyEffect(lpObj);
	gSkillTree.ApplyEffect(lpObj);

	GObjExtItemApply(lpObj);
	gObjSetItemApply(lpObj);
	gObjNextExpCal(lpObj);

	if ( (Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7) ) ||
		  Left->m_Type == ITEMGET(4,17) ||
		  Left->m_Type == ITEMGET(4,20) ||
		  Left->m_Type == ITEMGET(4,21) ||
		  Left->m_Type == ITEMGET(4,22) ||
		  Left->m_Type == ITEMGET(4,23) ||
		  Left->m_Type == ITEMGET(4,24)  )
	{
		if ( Right->m_Type == ITEMGET(4,15) && Right->m_Level == 1 )
		{
			lpObj->m_AttackDamageMinLeft += (WORD)(lpObj->m_AttackDamageMinLeft * 0.03f + 1.0f);
			lpObj->m_AttackDamageMaxLeft += (WORD)(lpObj->m_AttackDamageMaxLeft * 0.03f + 1.0f);
		}
		else if ( Right->m_Type == ITEMGET(4,15) && Right->m_Level == 2 )
		{
			lpObj->m_AttackDamageMinLeft += (WORD)(lpObj->m_AttackDamageMinLeft * 0.05f + 1.0f);
			lpObj->m_AttackDamageMaxLeft += (WORD)(lpObj->m_AttackDamageMaxLeft * 0.05f + 1.0f);
		}
	}
	else if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) ) ||
		      (Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0)) )
	{
		if ( Left->m_Type == ITEMGET(4,7) && Left->m_Level == 1 )
		{
				lpObj->m_AttackDamageMinRight += (WORD)(lpObj->m_AttackDamageMinRight * 0.03f + 1.0f);
				lpObj->m_AttackDamageMaxRight += (WORD)(lpObj->m_AttackDamageMaxRight * 0.03f + 1.0f);
		}
		else if ( Left->m_Type == ITEMGET(4,7) && Left->m_Level == 2 )
		{
				lpObj->m_AttackDamageMinRight += (WORD)(lpObj->m_AttackDamageMinRight * 0.05f + 1.0f);
				lpObj->m_AttackDamageMaxRight += (WORD)(lpObj->m_AttackDamageMaxRight * 0.05f + 1.0f);
		}
	}

	if ( lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_FIGHTER )
	{
		if ( Right->m_Type != -1 && Left->m_Type != -1 )
		{
			if ( Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) && Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0) )
			{
				lpObj->m_AttackDamageMinRight = lpObj->m_AttackDamageMinRight * 55 / 100;
				lpObj->m_AttackDamageMaxRight = lpObj->m_AttackDamageMaxRight * 55 / 100;
				lpObj->m_AttackDamageMinLeft = lpObj->m_AttackDamageMinLeft * 55 / 100;
				lpObj->m_AttackDamageMaxLeft = lpObj->m_AttackDamageMaxLeft * 55 / 100;
			}
		}
	}

	if ( lpObj->m_iItemEffectValidTime > 0 )
	{
		g_ItemAddOption.NextSetItemLastEffectForHallowin(lpObj);
	}

	gObjCalcShieldPoint(lpObj);

	if ( lpObj->iShield > ( lpObj->iMaxShield + lpObj->iAddShield ) )
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield ;
		GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	}

	GCReFillSend(lpObj->m_Index, lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
	GCManaSend(lpObj->m_Index, lpObj->MaxMana + lpObj->AddMana, 0xFE, 0, lpObj->MaxBP + lpObj->AddBP);
}

void GObjExtItemApply(LPOBJ lpObj)
{

	int n;
	int i;
	int max_count = 0;
	int temp_n[3] = {0, 1, 9};
	int temp_n2[10] = {2, 3, 4, 5, 6, 7, 8, 10, 11, 1};
	int maxn2_loop = 9;

	for ( i=0;i<3;i++ )
	{
		n = temp_n[i];

		if ( lpObj->pInventory[n].m_Type >= ITEMGET(6, 0) && lpObj->pInventory[n].m_Type < ITEMGET(7, 0) )
			continue;

		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Durability != 0.0f )
			{
				if ( lpObj->pInventory[n].m_IsValidItem == true )
				{
					if ( lpObj->pInventory[n].IsExtAttackRate() )
					{
						if ( ( lpObj->pInventory[n].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[n].m_Type < ITEMGET(6, 0) ) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,12) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,25) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,27) )
						{
							lpObj->m_MagicDamageMax += lpObj->Level / 20;
							lpObj->m_MagicDamageMin += lpObj->Level / 20;
						}
						else 
						{
							if ( n == 0 || n == 9 )
							{
								lpObj->m_AttackDamageMinRight += lpObj->Level / 20;
								lpObj->m_AttackDamageMaxRight += lpObj->Level / 20;
							}
							
							if ( n == 1 || n == 9 )
							{
								lpObj->m_AttackDamageMinLeft += lpObj->Level / 20;
								lpObj->m_AttackDamageMaxLeft += lpObj->Level / 20;
							}
						}
					}

					if ( lpObj->pInventory[n].IsExtAttackRate2() )
					{
						if ( ( lpObj->pInventory[n].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[n].m_Type < ITEMGET(6, 0) ) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,12) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,25) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,27) )
						{
							lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * lpObj->pInventory[n].IsExtAttackRate2() / 100;
							lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * lpObj->pInventory[n].IsExtAttackRate2() / 100;
						}
						else 
						{
							if ( n == 0 || n == 9 )
							{
								lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * lpObj->pInventory[n].IsExtAttackRate2() / 100;
								lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * lpObj->pInventory[n].IsExtAttackRate2() / 100;
							}
							
							if ( n == 1 || n == 9 )
							{
								lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * lpObj->pInventory[n].IsExtAttackRate2() / 100;
								lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * lpObj->pInventory[n].IsExtAttackRate2() / 100;
							}
						}
					}

					lpObj->m_ExcelentDamage += lpObj->pInventory[n].IsExtExcellentDamage();
					lpObj->m_AttackSpeed += lpObj->pInventory[n].IsExtAttackSpeed();
					lpObj->m_MagicSpeed += lpObj->pInventory[n].IsExtAttackSpeed();
					lpObj->MonsterDieGetLife += lpObj->pInventory[n].IsExtMonsterDieLife();
					lpObj->MonsterDieGetMana += lpObj->pInventory[n].IsExtMonsterDieMana();
				}
			}
		}
	}
		
	if ( lpObj->pInventory[1].IsItem() == TRUE )
	{
		if ( lpObj->pInventory[1].m_Type >= ITEMGET(6, 0) && lpObj->pInventory[1].m_Type <= ITEMGET(6, 21) )
		{
			maxn2_loop = 10;
		}
	}

	for ( i=0;i<maxn2_loop;i++)
	{
		n = temp_n2[i];

		if ( max_count < 8 )
		{
			if ( lpObj->pInventory[n].IsItem() == TRUE &&
				 lpObj->pInventory[n].IsExtItem() &&
				 lpObj->pInventory[n].m_Durability != 0.0f &&
				 lpObj->pInventory[n].m_IsValidItem == true )
			{
				if ( n != 7 )	// #warning this will unselect ONLY wings
				{
					lpObj->AddLife += (short)(lpObj->MaxLife * lpObj->pInventory[n].IsExtLifeAdd()  / 100.0f);	// #stat
					lpObj->AddMana += (short)(lpObj->MaxMana * lpObj->pInventory[n].IsExtManaAdd() / 100.0f);	// #stat
					lpObj->m_SuccessfulBlocking += lpObj->m_SuccessfulBlocking * lpObj->pInventory[n].IsExtDefenseSuccessfull() / 100;
					lpObj->MonsterDieGetMoney += lpObj->pInventory[n].IsExtMonsterMoney();
					lpObj->DamageMinus += lpObj->pInventory[n].IsExtDamageMinus();
					lpObj->DamageReflect += lpObj->pInventory[n].IsExtDamageReflect();
					max_count++;
				}
			}
		}
	}

	lpObj->AddLife += lpObj->m_SkillAddLife;
	lpObj->AddLife += lpObj->m_RageFighterSkillFitnessAddLife;
}

void gObjCalcSetItemStat(LPOBJ lpObj)
{
	for ( int i=0;i<INVETORY_WEAR_SIZE;i++)
	{
		if ( lpObj->pInventory[i].IsSetItem() && lpObj->pInventory[i].m_IsValidItem && lpObj->pInventory[i].m_Durability != 0.0f )
		{
			int AddStatType = lpObj->pInventory[i].GetAddStatType();

			switch ( AddStatType )
			{
				case 1:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddStrength, 196);
					break;
				case 2:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddDexterity, 197);
					break;
				case 3:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddEnergy, 198);
					break;
				case 4:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddVitality, 199);
					break;
			}
		}
	}
}

void gObjGetSetItemOption(LPOBJ lpObj, LPBYTE pSetOptionTable , LPBYTE pSetOptionCountTable , int * pSetOptionCount)
{
	*pSetOptionCount = 0;
	int RightSetOptionIndex = -1;
	int RightRingSetOptionIndex = -1;

	for ( int i=0;i<INVETORY_WEAR_SIZE;i++)
	{
		int iSetItemType = lpObj->pInventory[i].IsSetItem();

		if ( iSetItemType )
		{
			if ( lpObj->pInventory[i].m_IsValidItem )
			{
				int iSetItemOption = gSetItemOption.GetOptionTableIndex(lpObj->pInventory[i].m_Type, iSetItemType );

				if ( iSetItemOption != -1 )
				{
					if ( i == 0 )
					{
						RightSetOptionIndex = iSetItemOption;
					}
					else if ( i == 1  )
					{
						if (  RightSetOptionIndex == iSetItemOption )
						{
							if ( lpObj->pInventory[0].GetWeaponType() == lpObj->pInventory[1].GetWeaponType() && lpObj->pInventory[0].IsSetItem() )
							{
								//CLog.LogAdd("양손에 같은 세트가 적용되었다 무시...");
								continue;
							}
						}
					}
					
					if ( i == 10 )
					{
						RightRingSetOptionIndex = iSetItemOption;
					}
					else if ( i == 11   )
					{
						if ( lpObj->pInventory[10].m_Type == lpObj->pInventory[11].m_Type && lpObj->pInventory[10].IsSetItem() )
						{
							//CLog.LogAdd("반지에 같은 세트가 적용되었다 무시...");
							continue;
						}
					}

					BOOL bFound = FALSE;

					for ( int n=0;n<*pSetOptionCount;n++)
					{
						if ( pSetOptionTable[n] == iSetItemOption )
						{
							pSetOptionCountTable[n]++;
							bFound = TRUE;

							//CLog.LogAdd("옵션 찾았다 %s, Count = %d",
								//gSetItemOption.GetSetOptionName(iSetItemOption), pSetOptionCountTable[n]);

							break;
						}
					}

					if ( bFound == FALSE )
					{
						pSetOptionTable[*pSetOptionCount] = iSetItemOption;
						pSetOptionCountTable[*pSetOptionCount]++;

						//CLog.LogAdd("옵션 찾았다 %s, Count = %d",
							//gSetItemOption.GetSetOptionName(iSetItemOption), pSetOptionCountTable[*pSetOptionCount]);

						*pSetOptionCount += 1;
					}
				}
			}
		}
	}
}

void gObjCalcSetItemOption(LPOBJ lpObj)
{
	BYTE SetOptionTable[29];
	BYTE SetOptionCountTable[29];
	int SetOptionCount = 0;
	int op1;
	int op2;
	int op3;
	int op4;
	int op5;
	int opvalue1;
	int opvalue2;
	int opvalue3;
	int opvalue4;
	int opvalue5;

	memset(SetOptionTable, 0, sizeof(SetOptionTable));
	memset(SetOptionCountTable, 0, sizeof(SetOptionCountTable));
	gObjGetSetItemOption(lpObj, SetOptionTable, SetOptionCountTable, &SetOptionCount);

	for ( int optioncount=0;optioncount<SetOptionCount;optioncount++)
	{
		int OptionTableIndex = SetOptionTable[optioncount];
		int OptionTableCount = SetOptionCountTable[optioncount];

		if ( OptionTableCount >= 2 )
		{
			gSetItemOption.IsRequireClass(OptionTableIndex, lpObj->Class, lpObj->ChangeUP, lpObj->ChangeUP2);

			for ( int tablecnt=0;tablecnt<(OptionTableCount-1);tablecnt++)
			{
				if ( gSetItemOption.GetSetOption(OptionTableIndex, tablecnt,
					op1, op2, opvalue1, opvalue2, lpObj->Class, lpObj->ChangeUP, lpObj->ChangeUP2) )
				{
					gObjSetItemStatPlusSpecial(lpObj, op1, opvalue1);
					gObjSetItemStatPlusSpecial(lpObj, op2, opvalue2);
				}
			}

			if ( gSetItemOption.GetMaxSetOptionCount(OptionTableIndex) < OptionTableCount )
			{
				lpObj->IsFullSetItem = true;

				gSetItemOption.GetGetFullSetOption(OptionTableIndex, op1, op2, op3, op4, op5, opvalue1, opvalue2, opvalue3, opvalue4, opvalue5, lpObj->Class, lpObj->ChangeUP, lpObj->ChangeUP2);
				gObjSetItemStatPlusSpecial(lpObj, op1, opvalue1);
				gObjSetItemStatPlusSpecial(lpObj, op2, opvalue2);
				gObjSetItemStatPlusSpecial(lpObj, op3, opvalue3);
				gObjSetItemStatPlusSpecial(lpObj, op4, opvalue4);
				gObjSetItemStatPlusSpecial(lpObj, op5, opvalue5);
			}		
		}
	}

	for ( int optioncount=0;optioncount<SetOptionCount;optioncount++)
	{
		int OptionTableIndex = SetOptionTable[optioncount];
		int OptionTableCount = SetOptionCountTable[optioncount];

		if ( OptionTableCount >= 2 )
		{

			for ( int tablecnt=0;tablecnt<(OptionTableCount-1);tablecnt++)
			{
				if ( gSetItemOption.GetSetOption(OptionTableIndex, tablecnt,
					op1, op2, opvalue1, opvalue2, lpObj->Class, lpObj->ChangeUP, lpObj->ChangeUP2) )
				{
					gObjSetItemPlusSpecial(lpObj, op1, opvalue1);
					gObjSetItemPlusSpecial(lpObj, op2, opvalue2);
				}
			}
				
			if ( gSetItemOption.GetMaxSetOptionCount(OptionTableIndex) < OptionTableCount )
			{
				lpObj->IsFullSetItem = true;

				gSetItemOption.GetGetFullSetOption(OptionTableIndex,
					op1, op2, op3, op4, op5,
					opvalue1, opvalue2, opvalue3, opvalue4, opvalue5,
					lpObj->Class, lpObj->ChangeUP, lpObj->ChangeUP2);

				gObjSetItemPlusSpecial(lpObj, op1, opvalue1);
				gObjSetItemPlusSpecial(lpObj, op2, opvalue2);
				gObjSetItemPlusSpecial(lpObj, op3, opvalue3);
				gObjSetItemPlusSpecial(lpObj, op4, opvalue4);
				gObjSetItemPlusSpecial(lpObj, op5, opvalue5);
			}		
		}
	}
}

void gObjSetItemStatPlusSpecial(LPOBJ lpObj, int option, int ivalue)
{
	if ( option == -1 )
		return;

	switch ( option )
	{
		case AT_SET_OPTION_IMPROVE_STRENGTH:
			lpObj->AddStrength += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_DEXTERITY:
			lpObj->AddDexterity += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_ENERGY:
			lpObj->AddEnergy += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_VITALITY:
			lpObj->AddVitality += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_LEADERSHIP:
			lpObj->Leadership += ivalue;
			break;
	}
}

void gObjSetItemPlusSpecial(LPOBJ lpObj, int option, int ivalue)
{
	if ( option == -1 )
		return;

	switch ( option )
	{
		case AT_SET_OPTION_DOUBLE_DAMAGE:
			lpObj->SetOpDoubleDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE:
			lpObj->SetOpImproveSheldDefence += ivalue;
			break;
		case AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE:
			lpObj->SetOpTwoHandSwordImproveDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE:
			lpObj->SetOpAddMinAttackDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE:
			lpObj->SetOpAddMaxAttackDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAGICDAMAGE:
			lpObj->SetOpAddMagicPower += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_DAMAGE:
			lpObj->SetOpAddDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_ATTACKRATE:
			lpObj->SetOpImproveSuccessAttackRate += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_DEFENCE:
			lpObj->SetOpAddDefence += ivalue;
			break;
		case AT_SET_OPTION_DEFENCE_IGNORE:
			lpObj->SetOpIgnoreDefense += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXLIFE:
			lpObj->AddLife += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXMANA:
			lpObj->AddMana += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXAG:
			lpObj->AddBP += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_AG_VALUE:
			lpObj->SetOpIncAGValue += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_CRITICALDAMAGE:
			lpObj->SetOpAddCriticalDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS:
			lpObj->SetOpAddCriticalDamageSuccessRate += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_EX_DAMAGE:
			lpObj->SetOpAddExDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS:
			lpObj->SetOpAddExDamageSuccessRate += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_SKILLDAMAGE:
			lpObj->SetOpAddSkillAttack += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX:
			lpObj->SetOpAddAttackDamage += ( lpObj->Dexterity + lpObj->AddDexterity ) / ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR:
			lpObj->SetOpAddAttackDamage += ( lpObj->Strength + lpObj->AddStrength ) / ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY:
			lpObj->SetOpAddMagicPower += ( lpObj->Energy + lpObj->AddEnergy ) / ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX:
			lpObj->SetOpAddDefence += lpObj->Dexterity + ( lpObj->AddDexterity  / ivalue );
			break;
		case AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT:
			lpObj->SetOpAddDefence += ( lpObj->Vitality + lpObj->AddVitality ) / ivalue;
			break;
		case AT_SET_OPTION_FIRE_MASTERY:
			lpObj->m_AddResistance[R_FIRE] += ivalue;
			break;
		case AT_SET_OPTION_ICE_MASTERY:
			lpObj->m_AddResistance[R_ICE] += ivalue;
			break;
		case AT_SET_OPTION_THUNDER_MASTERY:
			lpObj->m_AddResistance[R_LIGHTNING] += ivalue;
			break;
		case AT_SET_OPTION_POSION_MASTERY:
			lpObj->m_AddResistance[R_POISON] += ivalue;
			break;
		case AT_SET_OPTION_WATER_MASTERY:
			lpObj->m_AddResistance[R_WATER] += ivalue;
			break;
		case AT_SET_OPTION_WIND_MASTERY:
			lpObj->m_AddResistance[R_WIND] += ivalue;
			break;
		case AT_SET_OPTION_EARTH_MASTERY:
			lpObj->m_AddResistance[R_EARTH] += ivalue;
			break;
	}
}

void gObjSetItemApply(LPOBJ lpObj)
{
	lpObj->m_AttackDamageMinLeft += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMaxLeft += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMinRight += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMaxRight += lpObj->SetOpAddAttackDamage;

	lpObj->m_AttackDamageMinLeft += lpObj->SetOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinRight += lpObj->SetOpAddMinAttackDamage;

	lpObj->m_AttackDamageMaxLeft += lpObj->SetOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxRight += lpObj->SetOpAddMaxAttackDamage;

	lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * lpObj->SetOpAddMagicPower / 100;
	lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * lpObj->SetOpAddMagicPower / 100;

	lpObj->AddLife += INT(lpObj->AddVitality * gCharInfo.sCharInfo[lpObj->Class].VitalityToLife);
	lpObj->AddMana += INT(lpObj->AddEnergy * gCharInfo.sCharInfo[lpObj->Class].EnergyToMana);

	lpObj->m_CriticalDamage += lpObj->SetOpAddCriticalDamageSuccessRate;
	lpObj->m_ExcelentDamage += lpObj->SetOpAddExDamageSuccessRate;

	if ( lpObj->pInventory[10].IsSetItem() )
	{
		lpObj->pInventory[10].PlusSpecialSetRing((LPBYTE)lpObj->m_AddResistance);
	}

	if ( lpObj->pInventory[11].IsSetItem() )
	{
		lpObj->pInventory[11].PlusSpecialSetRing((LPBYTE)lpObj->m_AddResistance);
	}

	if ( lpObj->pInventory[9].IsSetItem() )
	{
		lpObj->pInventory[9].PlusSpecialSetRing((LPBYTE)lpObj->m_AddResistance);
	}

	if ( lpObj->SetOpTwoHandSwordImproveDamage )
	{
		if ( !lpObj->pInventory[1].IsItem() && !lpObj->pInventory[0].IsItem() )
		{
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}

		if ( lpObj->pInventory[1].IsItem() && lpObj->pInventory[1].m_TwoHand == FALSE )
		{
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}

		if ( lpObj->pInventory[0].IsItem() && lpObj->pInventory[0].m_TwoHand == FALSE )
		{
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}
	}

	if ( (lpObj->MaxLife + lpObj->AddLife ) < lpObj->Life )
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	}

	gObjSetBP(lpObj->m_Index);

	if ( (lpObj->MaxMana + lpObj->AddMana ) < lpObj->Mana )
	{
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
	}

	lpObj->m_Defense += lpObj->SetOpAddDefence * 10 / 20;
	lpObj->m_Defense += lpObj->m_Defense * lpObj->SetOpAddDefenceRate / 100;

	if ( lpObj->pInventory[1].m_Type >= ITEMGET(6,0) && lpObj->pInventory[1].m_Type < ITEMGET(7,0) )
	{
		lpObj->m_Defense += lpObj->m_Defense * lpObj->SetOpImproveSheldDefence / 100;
	}
}

BOOL gObjValidItem(LPOBJ lpObj, CItem * lpItem, int pos)
{
	LPITEM_ATTRIBUTE p = &ItemAttribute[lpItem->m_Type];
	
	if ( p->RequireStrength != 0 )
	{
		if ( (lpObj->Strength + lpObj->AddStrength) < ( lpItem->m_RequireStrength - lpItem->m_HJOpStrength ) )
		{
			return FALSE;
		}
	}

	if ( p->RequireDexterity != 0 )
	{
		if ( (lpObj->Dexterity + lpObj->AddDexterity) < ( lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity ) )
		{
			return FALSE;
		}
	}

	if ( p->RequireEnergy != 0 )
	{
		if ( (lpObj->Energy + lpObj->AddEnergy) < lpItem->m_RequireEnergy )
		{
			return FALSE;
		}
	}

	if ( p->RequireLevel != 0 )
	{
		if ( (lpObj->Level) < lpItem->m_RequireLevel )
		{
			return FALSE;
		}
	}

	if ( lpItem->m_Type >= ITEMGET(0,0) && lpItem->m_Type <= ITEMGET(11, 0) )
	{
		if ( p->RequireVitality )
		{
			if ( (lpObj->Vitality + lpObj->AddVitality ) < lpItem->m_RequireVitality )
			{
				return FALSE;
			}
		}

		if ( p->RequireLeadership )
		{
			if ( (lpObj->Leadership + lpObj->AddLeadership ) < lpItem->m_RequireLeaderShip )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

void gObjCalcShieldPoint(LPOBJ lpObj)
{
	int iMaxShieldPoint = 0;
	int iExpressionA = ( lpObj->Strength + lpObj->AddStrength ) + ( lpObj->Dexterity + lpObj->AddDexterity ) + ( lpObj->Vitality + lpObj->AddVitality ) + (lpObj->Energy + lpObj->AddEnergy );

	if ( lpObj->Class == CLASS_DARKLORD )
	{
		iExpressionA += lpObj->Leadership + lpObj->AddLeadership;
	}

	if ( g_iShieldGageConstB == 0 )
		g_iShieldGageConstB = 30;

	int iExpressionB = ( lpObj->Level * lpObj->Level) / g_iShieldGageConstB;

	if ( g_iShieldGageConstA == 0 )
		g_iShieldGageConstA = 12;

	iMaxShieldPoint = ( iExpressionA * g_iShieldGageConstA ) / 10 + iExpressionB  + lpObj->m_Defense;
	lpObj->iMaxShield = iMaxShieldPoint;
}