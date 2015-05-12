#include "../Header Files/stdafx.h"
#include "../Header Files/SpecialItems.h"
#include "../Header Files/User.h"
#include "../Header Files/Defines.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/LogProc.H"

// ----------------------------------------------------------------------------------------------
CSpecialItem gSpecialItem;
// ----------------------------------------------------------------------------------------------
CSpecialItem::CSpecialItem(void)
{
	return;
}
// ----------------------------------------------------------------------------------------------
CSpecialItem::~CSpecialItem(void)
{
	return;
}
// ----------------------------------------------------------------------------------------------
void CSpecialItem::Init()
{
	gSpecialItem.ItemCount[0] = 0;
	gSpecialItem.ItemCount[1] = 0;
	gSpecialItem.ItemCount[2] = 0;
	gSpecialItem.ItemCount[3] = 0;
	gSpecialItem.ItemCount[4] = 0;
	gSpecialItem.ItemCount[5] = 0;
	gSpecialItem.ItemCount[6] = 0;

	gSpecialItem.ReadFile();

	return;
}
// ----------------------------------------------------------------------------------------------
void CSpecialItem::ReadFile()
{
	SMDFile = fopen(BUFF_EFFECT_FILE, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[BuffEffect] Info file Load Fail [%s]", BUFF_EFFECT_FILE);
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
		    Token = GetToken();

			if ( strcmp("end", TokenString ) == 0)
			{
				break;
			}

			gSpecialItem.ItemType[type][gSpecialItem.ItemCount[type]] = TokenNumber;
				
			Token = GetToken();
			gSpecialItem.ItemIndex[type][gSpecialItem.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gSpecialItem.EffectType1[type][gSpecialItem.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gSpecialItem.EffectValue1[type][gSpecialItem.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gSpecialItem.EffectType2[type][gSpecialItem.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gSpecialItem.EffectValue2[type][gSpecialItem.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gSpecialItem.BuffTime[type][gSpecialItem.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gSpecialItem.BuffNumber[type][gSpecialItem.ItemCount[type]] = TokenNumber;

			gSpecialItem.ItemCount[type]++;
		        
		}
    }

	fclose(SMDFile);
	return;
}
// ----------------------------------------------------------------------------------------------
void CSpecialItem::UseItem(LPOBJ lpObj, char Type, int Item)
{
	//gSpecialItem.RemoveEffect(lpObj,Type,Item);
	// ----
	gSpecialItem.ApplyEffect(lpObj,Type,Item);
}
// ----------------------------------------------------------------------------------------------
void CSpecialItem::RemoveEffect(LPOBJ lpObj, char Type, int Item)
{
	if ( lpObj->SpecialBuff[Type] == 0 )
	{
		return;
	}

	int CountPos = -1;

	CountPos = gSpecialItem.SearchItemPos(lpObj->SpecialBuffItem[Type],Type);

	if ( CountPos == -1 )
	{
		return;
	}

	if ( lpObj->SpecialBuffEffectType1[Type] != -1 )
	{
		switch ( lpObj->SpecialBuffEffectType1[Type] )
		{
		case INCREASE_MAX_MANA:
			lpObj->AddMana -= lpObj->SpecialBuffEffectValue1[Type];
			if ( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
			{
				lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
				GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
			}
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_AG:
			lpObj->AddBP -= lpObj->SpecialBuffEffectValue1[Type];
			if ( lpObj->BP > (lpObj->MaxBP+lpObj->AddBP) )
			{
				lpObj->BP = lpObj->MaxBP+lpObj->AddBP;
				GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
			}
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_LIFE:
			lpObj->AddLife -= lpObj->SpecialBuffEffectValue1[Type];
			if ( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife) )
			{
				lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
			}
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
			break;
		case INCREASE_MAX_SD:
			lpObj->iAddShield -= lpObj->SpecialBuffEffectValue1[Type];
			if ( lpObj->iShield > (lpObj->iMaxShield+lpObj->iAddShield) )
			{
				lpObj->iShield = lpObj->iMaxShield+lpObj->iAddShield;
				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
			}
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
			break;
		case INCREASE_DROP_RATE:
			break;
		case INCREASE_EXE_DMG:
			lpObj->m_ExcelentDamage -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_CRITICAL_DMG:
			lpObj->m_CriticalDamage -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_LIFE_AUTORECOVERY:
			break;
		case INCREASE_EXP_RATE:
			break;
		case INCREASE_ATTACK_DMG:
			lpObj->m_AttackDamageLeft -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_AttackDamageRight -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_AttackDamageMin -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_AttackDamageMax -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_AttackDamageMinLeft -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_AttackDamageMaxLeft -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_AttackDamageMinRight -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_AttackDamageMaxRight -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_DEFENSE:
			lpObj->m_Defense -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_MANA_AUTORECOVERY:
			break;
		case INCREASE_ATTACK_SPEED:
			lpObj->m_AttackSpeed -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_MagicSpeed -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case DISABLE_EXP:
			break;
		case ENABLE_MOBILITY:
			break;
		case INCREASE_WIZARDY_DMG:
			lpObj->m_MagicDamageMax -= lpObj->SpecialBuffEffectValue1[Type];
			lpObj->m_MagicDamageMin -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_STRENGHT:
			lpObj->AddStrength -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_AGILITY:
			lpObj->AddDexterity -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_HEALTH:
			lpObj->AddVitality -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_ENERGY:
			lpObj->AddEnergy -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		case INCREASE_LEADERSHIP:
			lpObj->AddLeadership -= lpObj->SpecialBuffEffectValue1[Type];
			break;
		}
		lpObj->SpecialBuffEffectType2[Type] = -1;
	}

	if ( lpObj->SpecialBuffEffectType2[Type] != -1 )
	{
		switch ( lpObj->SpecialBuffEffectType2[Type] )
		{
		case INCREASE_MAX_MANA:
			lpObj->AddMana -= lpObj->SpecialBuffEffectValue2[Type];
			if ( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
			{
				lpObj->Mana = lpObj->MaxMana+lpObj->AddMana;
				GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
			}
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_AG:
			lpObj->AddBP -= lpObj->SpecialBuffEffectValue2[Type];
			if ( lpObj->BP > (lpObj->MaxBP+lpObj->AddBP) )
			{
				lpObj->BP = lpObj->MaxBP+lpObj->AddBP;
				GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
			}
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_LIFE:
			lpObj->AddLife -= lpObj->SpecialBuffEffectValue2[Type];
			if ( lpObj->Life > (lpObj->MaxLife+lpObj->AddLife) )
			{
				lpObj->Life = lpObj->MaxLife+lpObj->AddLife;
				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
			}
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
			break;
		case INCREASE_MAX_SD:
			lpObj->iAddShield -= lpObj->SpecialBuffEffectValue2[Type];
			if ( lpObj->iShield > (lpObj->iMaxShield+lpObj->iAddShield) )
			{
				lpObj->iShield = lpObj->iMaxShield+lpObj->iAddShield;
				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
			}
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
			break;
		case INCREASE_DROP_RATE:
			break;
		case INCREASE_EXE_DMG:
			lpObj->m_ExcelentDamage -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_CRITICAL_DMG:
			lpObj->m_CriticalDamage -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_LIFE_AUTORECOVERY:
			break;
		case INCREASE_EXP_RATE:
			break;
		case INCREASE_ATTACK_DMG:
			lpObj->m_AttackDamageLeft -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_AttackDamageRight -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_AttackDamageMin -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_AttackDamageMax -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_AttackDamageMinLeft -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_AttackDamageMaxLeft -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_AttackDamageMinRight -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_AttackDamageMaxRight -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_DEFENSE:
			lpObj->m_Defense -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_MANA_AUTORECOVERY:
			break;
		case INCREASE_ATTACK_SPEED:
			lpObj->m_AttackSpeed -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_MagicSpeed -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case DISABLE_EXP:
			break;
		case ENABLE_MOBILITY:
			break;
		case INCREASE_WIZARDY_DMG:
			lpObj->m_MagicDamageMax -= lpObj->SpecialBuffEffectValue2[Type];
			lpObj->m_MagicDamageMin -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_STRENGHT:
			lpObj->AddStrength -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_AGILITY:
			lpObj->AddDexterity -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_HEALTH:
			lpObj->AddVitality -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_ENERGY:
			lpObj->AddEnergy -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		case INCREASE_LEADERSHIP:
			lpObj->AddLeadership -= lpObj->SpecialBuffEffectValue2[Type];
			break;
		}
	}
	lpObj->SpecialBuffEffectType1[Type] = -1;
	lpObj->SpecialBuffEffectValue1[Type] = -1;
	lpObj->SpecialBuffEffectType2[Type] = -1;
	lpObj->SpecialBuffEffectValue2[Type] = -1;

	lpObj->SpecialBuff[Type] = 0;

	if ( gSpecialItem.BuffNumber[Type][CountPos] != -1 )
	{
		GCStateInfoSend(lpObj,0,gSpecialItem.BuffNumber[Type][CountPos]);
	}

	lpObj->SpecialBuffEffectTime[Type] = 0;

	lpObj->SpecialBuffItem[Type] = -1;
	
}
// ----------------------------------------------------------------------------------------------
void CSpecialItem::ApplyEffect(LPOBJ lpObj,char Type,int Item)
{
	int CountPos = -1;

	CountPos = gSpecialItem.SearchItemPos(Item,Type);

	if ( CountPos == -1 )
	{
		return;
	}

	if ( lpObj->SpecialBuff[Type] == 1 )
	{
		gSpecialItem.RemoveEffect(lpObj,Type,Item);
	}

	if ( gSpecialItem.EffectType1[Type][CountPos] != -1 )
	{
		switch ( gSpecialItem.EffectType1[Type][CountPos] )
		{
		case INCREASE_MAX_MANA:
			lpObj->AddMana += gSpecialItem.EffectValue1[Type][CountPos];
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_AG:
			lpObj->AddBP += gSpecialItem.EffectValue1[Type][CountPos];
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_LIFE:
			lpObj->AddLife += gSpecialItem.EffectValue1[Type][CountPos];
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
			break;
		case INCREASE_MAX_SD:
			lpObj->iAddShield += gSpecialItem.EffectValue1[Type][CountPos];
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
			break;
		case INCREASE_DROP_RATE:
			break;
		case INCREASE_EXE_DMG:
			lpObj->m_ExcelentDamage += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_CRITICAL_DMG:
			lpObj->m_CriticalDamage += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_LIFE_AUTORECOVERY:
			break;
		case INCREASE_EXP_RATE:
			break;
		case INCREASE_ATTACK_DMG:
			lpObj->m_AttackDamageLeft += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_AttackDamageRight += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_AttackDamageMin += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_AttackDamageMax += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_AttackDamageMinLeft += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_AttackDamageMaxLeft += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_AttackDamageMinRight += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_AttackDamageMaxRight += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_DEFENSE:
			lpObj->m_Defense += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_MANA_AUTORECOVERY:
			break;
		case INCREASE_ATTACK_SPEED:
			lpObj->m_AttackSpeed += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_MagicSpeed += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case DISABLE_EXP:
			break;
		case ENABLE_MOBILITY:
			break;
		case INCREASE_WIZARDY_DMG:
			lpObj->m_MagicDamageMax += gSpecialItem.EffectValue1[Type][CountPos];
			lpObj->m_MagicDamageMin += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_STRENGHT:
			lpObj->AddStrength += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_AGILITY:
			lpObj->AddDexterity += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_HEALTH:
			lpObj->AddVitality += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_ENERGY:
			lpObj->AddEnergy += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		case INCREASE_LEADERSHIP:
			lpObj->AddLeadership += gSpecialItem.EffectValue1[Type][CountPos];
			break;
		}
	}

	if ( gSpecialItem.EffectType2[Type][CountPos] != -1 )
	{
		switch ( gSpecialItem.EffectType2[Type][CountPos] )
		{
		case INCREASE_MAX_MANA:
			lpObj->AddMana += gSpecialItem.EffectValue2[Type][CountPos];
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_AG:
			lpObj->AddBP += gSpecialItem.EffectValue2[Type][CountPos];
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP+lpObj->AddBP);
			break;
		case INCREASE_MAX_LIFE:
			lpObj->AddLife += gSpecialItem.EffectValue2[Type][CountPos];
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
			break;
		case INCREASE_MAX_SD:
			lpObj->iAddShield += gSpecialItem.EffectValue2[Type][CountPos];
			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
			break;
		case INCREASE_DROP_RATE:
			break;
		case INCREASE_EXE_DMG:
			lpObj->m_ExcelentDamage += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_CRITICAL_DMG:
			lpObj->m_CriticalDamage += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_LIFE_AUTORECOVERY:
			break;
		case INCREASE_EXP_RATE:
			break;
		case INCREASE_ATTACK_DMG:
			lpObj->m_AttackDamageLeft += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_AttackDamageRight += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_AttackDamageMin += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_AttackDamageMax += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_AttackDamageMinLeft += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_AttackDamageMaxLeft += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_AttackDamageMinRight += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_AttackDamageMaxRight += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_DEFENSE:
			lpObj->m_Defense += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_MANA_AUTORECOVERY:
			break;
		case INCREASE_ATTACK_SPEED:
			lpObj->m_AttackSpeed += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_MagicSpeed += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case DISABLE_EXP:
			break;
		case ENABLE_MOBILITY:
			break;
		case INCREASE_WIZARDY_DMG:
			lpObj->m_MagicDamageMax += gSpecialItem.EffectValue2[Type][CountPos];
			lpObj->m_MagicDamageMin += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_STRENGHT:
			lpObj->AddStrength += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_AGILITY:
			lpObj->AddDexterity += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_HEALTH:
			lpObj->AddVitality += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_ENERGY:
			lpObj->AddEnergy += gSpecialItem.EffectValue2[Type][CountPos];
			break;
		case INCREASE_LEADERSHIP:
			lpObj->AddLeadership += gSpecialItem.EffectValue2[Type][CountPos];
			break;

		}
	}
	lpObj->SpecialBuff[Type] = 1;
	lpObj->SpecialBuffEffectType1[Type] = gSpecialItem.EffectType1[Type][CountPos];
	lpObj->SpecialBuffEffectValue1[Type] = gSpecialItem.EffectValue1[Type][CountPos];
	lpObj->SpecialBuffEffectType2[Type] = gSpecialItem.EffectType2[Type][CountPos];
	lpObj->SpecialBuffEffectValue2[Type] = gSpecialItem.EffectValue2[Type][CountPos];
	if ( gSpecialItem.BuffTime[Type][CountPos] != 0 )
	{
		lpObj->SpecialBuffEffectTime[Type] = gSpecialItem.BuffTime[Type][CountPos];
	}
	if ( gSpecialItem.BuffNumber[Type][CountPos] != -1 )
	{
		GCStateInfoSend(lpObj,1,gSpecialItem.BuffNumber[Type][CountPos]);
	}
	lpObj->SpecialBuffItem[Type] = Item;
	
}
// ----------------------------------------------------------------------------------------------
int CSpecialItem::SearchItemPos(int Item, char Type)
{
	for ( int i=0;i<gSpecialItem.ItemCount[Type];i++)
	{
		if ( ITEMGET(gSpecialItem.ItemType[Type][i],gSpecialItem.ItemIndex[Type][i]) == Item )
		{
			return i;
		}
	}

	return -1;
}
// ----------------------------------------------------------------------------------------------
bool CSpecialItem::CheckNoExp(LPOBJ lpObj)
{
	for (int i=0;i<7;i++)
	{
		if ( lpObj->SpecialBuff[i] != -1 )
		{
			if ( lpObj->SpecialBuffEffectType1[i] == DISABLE_EXP )
			{
				return true;
			}
			else if ( lpObj->SpecialBuffEffectType2[i] == DISABLE_EXP )
			{
				return true;
			}
		}
	}
	return false;
}
// ----------------------------------------------------------------------------------------------
void CSpecialItem::TimerEffect(LPOBJ lpObj)
{
	for ( int i=0;i<7;i++ )
	{
		if ( lpObj->SpecialBuff[i] == 1 )
		{
			if ( lpObj->SpecialBuffEffectTime[i] > 0 )
			{
				lpObj->SpecialBuffEffectTime[i]--;

				if ( lpObj->SpecialBuffEffectTime[i] <= 0 )
				{
					gSpecialItem.RemoveEffect(lpObj,i,lpObj->SpecialBuffItem[i]);
				}
			}
		}
	}
}
// ----------------------------------------------------------------------------------------------
