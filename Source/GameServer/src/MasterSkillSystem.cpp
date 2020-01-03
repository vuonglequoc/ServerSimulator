//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Master Skill Tree System
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "MasterSkillSystem.h"
#include "protocol.h"
#include "user.h"
#include "LogProc.H"
#include "Main.H"
#include "GameMain.h"
#include "ObjCalCharacter.h"
#include "ReadScript.h"
#include "WzMemScript.h"
#include "Defines.h"
#include "MagicInf.h"
#include "SendProto.H"


// -----------------------------------------------------------------------------------------------------------------------------------------
CSkillTree gSkillTree;
// -----------------------------------------------------------------------------------------------------------------------------------------
CSkillTree::CSkillTree(void)
{
	LoadSkillTreeSkills();
}
// -----------------------------------------------------------------------------------------------------------------------------------------
CSkillTree::~CSkillTree(void)
{
	// ------
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::LoadSkillTreeSkills()
{
	SMDFile = fopen("..\\Data\\Lang\\Kor\\MasterSkill(Kor).txt", "r");

	if ( SMDFile == NULL )
	{
		return;
	}

	int Token;
	int number;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		if ( Token == 1 )
		{
			number = TokenNumber;

			Token = GetToken();
			strcpy(gSkillTree.m_Name[number], TokenString );

			Token = GetToken();
			gSkillTree.m_rLevel[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Damage[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Mana[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_BrainPower[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Distance[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Delay[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireEnergy[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireLevel[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Attr1[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Attr2[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_iSkillUseType[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_iSkillBrand[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_iKillCount[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_iRequireStatus[number][0] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_iRequireStatus[number][1] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_iRequireStatus[number][2] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireClass[number][0] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireClass[number][1] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireClass[number][2] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireClass[number][3] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireClass[number][4] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireClass[number][5] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_RequireClass[number][6] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Rank[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Group[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_MPoints[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_Icon[number] = TokenNumber;

			Token = GetToken();
			gSkillTree.m_iSkillType[number] = TokenNumber;
		}
	}
	fclose(SMDFile);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::SendMasterData(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( gUser.gObjIsMasteringLevel(lpObj) == false )
	{
		return;
	}

	PMSG_MASTERLEVEL_SEND pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0xF3,0x50,sizeof(pMsg));
	pMsg.MasterLevel = lpObj->MasterLevel;
	pMsg.MasterExp[0] = HIBYTE(HIWORD(HIDWORD(lpObj->MLExperience))); // Highest BYTE
	pMsg.MasterExp[1] = LOBYTE(HIWORD(HIDWORD(lpObj->MLExperience)));
	pMsg.MasterExp[2] = HIBYTE(LOWORD(HIDWORD(lpObj->MLExperience)));
	pMsg.MasterExp[3] = LOBYTE(LOWORD(HIDWORD(lpObj->MLExperience)));
	pMsg.MasterExp[4] = HIBYTE(HIWORD(LODWORD(lpObj->MLExperience)));
	pMsg.MasterExp[5] = LOBYTE(HIWORD(LODWORD(lpObj->MLExperience)));
	pMsg.MasterExp[6] = HIBYTE(LOWORD(LODWORD(lpObj->MLExperience)));
	pMsg.MasterExp[7] = LOBYTE(LOWORD(LODWORD(lpObj->MLExperience))); // Lowest BYTE
	pMsg.MasterNextExp[0] = HIBYTE(HIWORD(HIDWORD(lpObj->MLNextExp))); // Highest BYTE
	pMsg.MasterNextExp[1] = LOBYTE(HIWORD(HIDWORD(lpObj->MLNextExp)));
	pMsg.MasterNextExp[2] = HIBYTE(LOWORD(HIDWORD(lpObj->MLNextExp)));
	pMsg.MasterNextExp[3] = LOBYTE(LOWORD(HIDWORD(lpObj->MLNextExp)));
	pMsg.MasterNextExp[4] = HIBYTE(HIWORD(LODWORD(lpObj->MLNextExp)));
	pMsg.MasterNextExp[5] = LOBYTE(HIWORD(LODWORD(lpObj->MLNextExp)));
	pMsg.MasterNextExp[6] = HIBYTE(LOWORD(LODWORD(lpObj->MLNextExp)));
	pMsg.MasterNextExp[7] = LOBYTE(LOWORD(LODWORD(lpObj->MLNextExp))); // Lowest BYTE
	pMsg.MasterPoints = lpObj->MasterPoints;
	pMsg.MaxLife = lpObj->MaxLife+lpObj->AddLife;
	pMsg.MaxMana = lpObj->MaxMana+lpObj->AddMana;
	pMsg.iMaxShield = lpObj->iMaxShield+lpObj->iAddShield;
	pMsg.MaxBP = lpObj->MaxBP+lpObj->AddBP;

	gSendProto.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CSkillTree::IsMasterMagic(int SkillNumber)
{
	if ( SkillNumber >= 300 && SkillNumber <= 554 )
	{
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::SendMasterSkills(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( gUser.gObjIsMasteringLevel(lpObj) == false )
	{
		return;
	}

	BYTE ExistSkills[512]={0};

	for(int i=0;i<MAX_MAGIC;i++)
	{
		if ( IsMasterMagic(lpObj->Magic[i].m_Skill) == true )
		{
			ExistSkills[0]=0xC1;
			ExistSkills[1]=0x0C;
			ExistSkills[2]=0xF3;
			ExistSkills[3]=0x52;
			ExistSkills[4]=0x01;
			ExistSkills[5]=0x00;
			ExistSkills[6]=LOBYTE(lpObj->MasterPoints);
			ExistSkills[7]=HIBYTE(lpObj->MasterPoints);
			ExistSkills[8]=LOBYTE(lpObj->Magic[i].m_Skill);
			ExistSkills[9]=HIBYTE(lpObj->Magic[i].m_Skill);
			ExistSkills[10]=0;
			ExistSkills[11]=1;
			gSendProto.DataSend(aIndex, (PBYTE)ExistSkills, ExistSkills[1]);
			// ----
			/*if ( lpObj->Magic[i].m_Skill >= 435 && lpObj->Magic[i].m_Skill <= 554 )
			{
				GCMagicListOneDelSend(aIndex, i, lpObj->Magic[i].m_Skill, 0, 0, 0);
			}*/
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CSkillTree::IsMasterMagicExist(LPOBJ lpObj,int SkillNumber)
{
	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == true)
		{
			if(lpObj->Magic[n].m_Skill == SkillNumber)
			{
				GCServerMsgStringSend("[MasterSkillTree]: Same Magic already exists",lpObj->m_Index,1);
				return false;
			}
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CSkillTree::CanAddSkill(LPOBJ lpObj,int MagicNumber)
{
	bool bResult = false;
	// -- Check if Have Points
	if ( lpObj->MasterPoints < gSkillTree.m_MPoints[MagicNumber] )
	{
		GCServerMsgStringSend("Don't have points",lpObj->m_Index,1);
		return false;
	}
	// -- Check if Right Class
	else if ( gSkillTree.m_RequireClass[MagicNumber][lpObj->Class] == 0 )
	{
		return false;
	}
	// -- Check if magic exist
	else if ( gSkillTree.IsMasterMagicExist(lpObj,MagicNumber) == false )
	{
		return false;
	}

	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CSkillTree::MasterMagicAdd(LPOBJ lpObj,int MagicNumber)
{
	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if ( lpObj->Magic[n].m_Skill == MagicNumber )
		{
			return -1;
		}

		else if( lpObj->Magic[n].IsMagic() == false )
		{
			lpObj->Magic[n].m_Skill = MagicNumber;
			lpObj->Magic[n].m_Level = gSkillTree.m_Rank[MagicNumber];
			int damage = gSkillTree.m_Damage[MagicNumber];
			lpObj->Magic[n].m_DamageMin = damage;
			lpObj->Magic[n].m_DamageMax = damage + damage/2;
			return n;
		}
	}
	return -1;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::SkillAddRecv( PMSG_SKILL_ADD_RECV * aRecv, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	//int SkillNumber = (aRecv->SkillH * 256) + aRecv->SkillL;
	PMSG_SKILL_ADD_SEND pMsg;

	pMsg.c = 0xC1;
	pMsg.size = sizeof(pMsg);
	pMsg.headcode = 0xF3;
	pMsg.subcode = 0x52;
	pMsg._Unk1 = 0x00;
	pMsg.Enabled = 0x01;
	pMsg.MasterPointL = LOBYTE(lpObj->MasterPoints);
	pMsg.MasterPointH = HIBYTE(lpObj->MasterPoints);
	//pMsg.PosL = 1;
	//pMsg.PosH = 0;
	pMsg.Skill = aRecv->SkillL + ( aRecv->SkillH * 256 );
	pMsg.SkillPos = 0;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.size);

	/*LPOBJ lpObj = &gObj[aIndex];
	int SkillNumber = (aRecv->SkillH * 256) + aRecv->SkillL;
	int SkillRemoved;
	int SkillPosition;

	if ( CanAddSkill(lpObj,SkillNumber) == false )
	{
		return;
	}
	PMSG_SKILL_ADD_SEND pMsg;
	pMsg.h.set((LPBYTE)&pMsg,0xF3,0x52,sizeof(pMsg));

	
	if ( SkillNumber >= 300 && SkillNumber <= 434 )
	{
		// Decrease Master Points
		lpObj->MasterPoints -= gSkillTree.m_MPoints[SkillNumber];

		if ( SkillNumber != 300 && 
			 SkillNumber != 305 && 
			 SkillNumber != 310 && 
			 SkillNumber != 315 &&
			 SkillNumber != 320 &&
			 SkillNumber != 325 &&
			 SkillNumber != 330 &&
			 SkillNumber != 335 &&
			 SkillNumber != 340 && 
			 SkillNumber != 345 && 
			 SkillNumber != 350 && 
			 SkillNumber != 355 && 
			 SkillNumber != 360 &&
			 SkillNumber != 365 &&
			 SkillNumber != 370 &&
			 SkillNumber != 375 &&
			 SkillNumber != 380 &&
			 SkillNumber != 385 &&
			 SkillNumber != 390 &&
			 SkillNumber != 395 &&
			 SkillNumber != 400 &&
			 SkillNumber != 405 &&
			 SkillNumber != 410 &&
			 SkillNumber != 415 &&
			 SkillNumber != 420 &&
			 SkillNumber != 425 &&
			 SkillNumber != 430 )
		{
			SkillRemoved = gObjMagicDel(lpObj,SkillNumber-1,0);
		}			
		// Add New Skill
		gSkillTree.MasterMagicAdd(lpObj,SkillNumber);

		// Cal Character Function
		gCalCharacter.gObjCalCharacter(aIndex);

		pMsg.Enabled = 0x01;
		pMsg._Unk1 = 0x00;
		pMsg.MasterPoints[0] = LOBYTE(lpObj->MasterPoints);
		pMsg.MasterPoints[1] = HIBYTE(lpObj->MasterPoints);
		pMsg.Skill = SkillNumber;
		pMsg.SkillPos = 0;

		gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
	}
	else if ( SkillNumber >= 435 && SkillNumber <= 554 )
	{
		// Check if old skill exists
		SkillRemoved = gObjMagicDel(lpObj,gSkillTree.m_iSkillBrand[SkillNumber],0);
		// ----
		if ( SkillRemoved >= 0 )
		{
			// Remove old skill
			GCMagicListOneDelSend(aIndex, SkillRemoved, gSkillTree.m_iSkillBrand[SkillNumber], 0, 0, 0);
			// Decrease Master Points
			lpObj->MasterPoints -= gSkillTree.m_MPoints[SkillNumber];
			// Add new magic
			SkillPosition = MasterMagicAdd(lpObj, SkillNumber);

			pMsg.Enabled = 0x01;
			pMsg._Unk1 = 0x00;
			pMsg.MasterPoints[0] = LOBYTE(lpObj->MasterPoints);
			pMsg.MasterPoints[1] = HIBYTE(lpObj->MasterPoints);
			pMsg.Skill = SkillNumber;
			pMsg.SkillPos = 0;

			gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
		}
		else
		{
			GCServerMsgStringSend("[MasterSkillTree]: You need to learn skill to upgrade it",aIndex,1);
		}
	}*/
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::InitEffect(MASTER_SKILL_TREE_EFFECT* lpEffect)
{
    lpEffect->RiseAttackSuccessRate = 0;
	lpEffect->AdvancedAttackSuccessRate = 0;
	lpEffect->AdvancedDefenseSuccessRate = 0;
	lpEffect->RepairLevel1 = 0;
	lpEffect->RepairLevel2 = 0;
	lpEffect->PoisonResistance = 0;
	lpEffect->LightningResistance = 0;
	lpEffect->IceResistance = 0;
	lpEffect->IncreaseAutoRegeneration = 0;
	lpEffect->IncreaseZen = 0;
	lpEffect->IncreaseDefense = 0;
	lpEffect->IncreaseMaxHP = 0;
	lpEffect->IncreaseMaxAG = 0;
	lpEffect->IncreaseRecoverMana = 0;
	lpEffect->IncreaseRecoverLife = 0;
	lpEffect->IncreaseRecoverSD = 0;
	lpEffect->IncreaseExperience = 0;
	lpEffect->IncreaseMaxSD = 0;
	lpEffect->IncreaseSDRecoveryRate = 0;
	lpEffect->IncreaseMaxDamage = 0;
	lpEffect->IncreaseMinDamage = 0;
	lpEffect->IncreaseManaReduction = 0;
	lpEffect->IncreaseMaxSkillPower = 0;
	lpEffect->IncreaseMinSkillPower = 0;
	lpEffect->DecreasePetDurabilityWane = 0;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::ApplyEffect(LPOBJ lpObj)
{
	MASTER_SKILL_TREE_EFFECT* lpEffect = &lpObj->MasterSkillTreeEffect;

	int MagicNumber = -1;
	int MagicBonus = 0;

	this->InitEffect(lpEffect);

    for(int n=0;n < 60;n++)
	{
	    if(lpObj->Magic[n].IsMagic() == false || lpObj->Magic[n].m_Skill < MAX_SKILL)
		{
		    continue;
		}

		MagicNumber = lpObj->Magic[n].m_Skill;
		MagicBonus = gSkillTree.m_Damage[MagicNumber];

		if(MagicBonus != 0)
		{
		    this->CalcEffect(MagicNumber,MagicBonus,lpEffect);
		}
	}
	// ----
	lpObj->m_Resistance[0] += (lpObj->m_Resistance[0]*lpEffect->IceResistance)/100;
	lpObj->m_Resistance[1] += (lpObj->m_Resistance[1]*lpEffect->PoisonResistance)/100;
	lpObj->m_Resistance[2] += (lpObj->m_Resistance[2]*lpEffect->LightningResistance)/100;
	// ---- 
	lpObj->MonsterDieGetMoney += lpEffect->IncreaseZen;
	// ----
	lpObj->m_AttackDamageMinLeft += lpEffect->IncreaseMinDamage;
	lpObj->m_AttackDamageMaxLeft += lpEffect->IncreaseMaxDamage;
	lpObj->m_AttackDamageMinRight += lpEffect->IncreaseMinDamage;
	lpObj->m_AttackDamageMaxRight += lpEffect->IncreaseMaxDamage;
	// ----
	lpObj->m_MagicDamageMin += lpEffect->IncreaseMinSkillPower;
	lpObj->m_MagicDamageMax += lpEffect->IncreaseMaxSkillPower;
	// ----
	lpObj->m_Defense += lpEffect->IncreaseDefense;
	// ----
	lpObj->AddLife += lpEffect->IncreaseMaxHP;
	// ----
	lpObj->AddBP += lpEffect->IncreaseMaxAG;
	// ----
	lpObj->iAddShield += lpEffect->IncreaseMaxSD;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::CalcEffect(int type,int bonus,MASTER_SKILL_TREE_EFFECT* lpEffect)
{
	if ( type >= 300 && type <= 304 )
	{
		lpEffect->RiseAttackSuccessRate = bonus;
	}
	if ( type >= 306 && type <= 309 )
	{
		lpEffect->AdvancedAttackSuccessRate = bonus;
	}
	if ( type >= 310 && type <= 314 )
	{
		lpEffect->AdvancedDefenseSuccessRate = bonus;
	}
	if ( type >= 315 && type <= 319 )
	{
		lpEffect->RepairLevel1 = bonus;
	}
	if ( type >= 320 && type <= 324 )
	{
		lpEffect->RepairLevel2 = bonus;
	}
	if ( type >= 325 && type <= 329 )
	{
		lpEffect->PoisonResistance = bonus;
	}
	if ( type >= 330 && type <= 334 )
	{
		lpEffect->LightningResistance = bonus;
	}
	if ( type >= 335 && type <= 339 )
	{
		lpEffect->IceResistance = bonus;
	}
	if ( type >= 340 && type <= 344 )
	{
		lpEffect->IncreaseAutoRegeneration = bonus;
	}
	if ( type >= 345 && type <= 349 )
	{
		lpEffect->IncreaseZen = bonus;
	}
	if ( type >= 350 && type <= 354 )
	{
		lpEffect->IncreaseDefense = bonus;
	}
	if ( type >= 355 && type <= 359 )
	{
		lpEffect->IncreaseMaxHP = bonus;
	}
	if ( type >= 360 && type <= 364 )
	{
		lpEffect->IncreaseMaxAG = bonus;
	}
	if ( type >= 365 && type <= 369 )
	{
		lpEffect->IncreaseRecoverMana = bonus;
	}
	if ( type >= 370 && type <= 374 )
	{
		lpEffect->IncreaseRecoverLife = bonus;
	}
	if ( type >= 375 && type <= 379 )
	{
		lpEffect->IncreaseRecoverSD = bonus;
	}
	if ( type >= 380 && type <= 384 )
	{
		lpEffect->IncreaseExperience = bonus;
	}
	if ( type >= 385 && type <= 389 )
	{
		lpEffect->IncreaseMaxSD = bonus;
	}
	if ( type >= 390 && type <= 394 )
	{
		lpEffect->IncreaseSDRecoveryRate = bonus;
	}
	if ( type >= 395 && type <= 399 )
	{
		lpEffect->IncreaseMaxDamage = bonus;
	}
	if ( type >= 400 && type <= 404 )
	{
		lpEffect->IncreaseMinDamage = bonus;
	}
	if ( type >= 405 && type <= 409 )
	{
		lpEffect->IncreaseManaReduction = bonus;
	}
	if ( type >= 410 && type <= 414 )
	{
		lpEffect->IncreaseMaxSkillPower = bonus;
	}
	if ( type >= 415 && type <= 419 )
	{
		lpEffect->IncreaseMinSkillPower = bonus;
	}
	if ( type >= 420 && type <= 424 )
	{
		lpEffect->DecreasePetDurabilityWane = bonus;
	}
	if ( type >= 425 && type <= 429 )
	{
		lpEffect->IncreaseMaxDamage = bonus;
		lpEffect->IncreaseMaxSkillPower = bonus;
	}
	if ( type >= 430 && type <= 434 )
	{
		lpEffect->IncreaseMinDamage = bonus;
		lpEffect->IncreaseMinSkillPower = bonus;
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CSkillTree::GCMasterLevelUpSend(LPOBJ lpObj,bool effect)
{
    PMSG_MASTER_LEVEL_UP_SEND pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0xF3,0x51,sizeof(pMsg));

	pMsg.MasterLevel	= (WORD)lpObj->MasterLevel;
	pMsg.MinusPoint		= 0;
	pMsg.MasterPoint	= (WORD)lpObj->MasterPoints;
	pMsg.MaxMinusPoint	= 0;
	pMsg.MaxLife		= lpObj->MaxLife+lpObj->AddLife;
	pMsg.MaxMana		= lpObj->MaxMana+lpObj->AddMana;
	pMsg.MaxShield		= lpObj->iMaxShield+lpObj->iAddShield;
	pMsg.MaxBP			= lpObj->MaxBP+lpObj->AddBP;

    gSendProto.DataSend(lpObj->m_Index,(BYTE*)&pMsg,pMsg.h.size);

	if(effect == 1)
	{
	    GCSendEffectInfo(lpObj->m_Index,16);
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CSkillTree::GetMasterSkillDistance(int SkillNumber)
{
	return gSkillTree.m_Distance[SkillNumber];
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CSkillTree::GetMasterSkillMana(LPOBJ lpObj, int SkillNumber)
{

	if ( lpObj->Type != OBJ_USER )
	{
		return 0;
	}

	float mana = lpObj->Mana;

	mana -= gSkillTree.m_Mana[SkillNumber];

	if(mana < 0.0f)
	{
		return -1;
	}
	return mana;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CSkillTree::GetMasterSkillBP(LPOBJ lpObj, int SkillNumber)
{
	if(lpObj->Type != OBJ_USER)
	{
		return 0;
	}

	int bp = lpObj->BP;
	
	int usingbp = gSkillTree.m_BrainPower[SkillNumber];

	usingbp -= usingbp * lpObj->SetOpDecreaseAG / 100;

	bp -= usingbp;

	if(bp < 0)
	{
		return -1;
	}

	return bp;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CSkillTree::GetMasterSkillAttr2(int SkillNumber)
{
	return gSkillTree.m_Attr2[SkillNumber];
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CSkillTree::GetMasterSkillDamage(int SkillNumber)
{
	int damage;
	
	damage = gSkillTree.m_Damage[SkillNumber] + (rand() % 1);
	
	return damage;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CSkillTree::MasterSkillClassCheck(LPOBJ lpObj,int SkillNumber)
{
	if ( gUser.gObjIsMasteringLevel(lpObj) == false )
	{
		return false;
	}
	if ( gSkillTree.m_RequireClass[SkillNumber][lpObj->Class] == 0 )
	{
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CSkillTree::MasterSkillEnergyCheck(LPOBJ lpObj,int SkillNumber)
{
	int reqeng;
	// ----
	reqeng = gSkillTree.m_RequireEnergy[SkillNumber];

	if(reqeng < 0)
	{
		return false;
	}

	if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
	{
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int CSkillTree::GetMasterSkillType(int SkillNumber)
{
	return gSkillTree.m_Attr2[SkillNumber];
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool CSkillTree::CheckMasterSkillCanAdd(LPOBJ lpObj, int SkillNumber)
{
	for (int i=0;i<MAX_SKILL_IN_TREE;i++)
	{
		if ( lpObj->Magic[i].IsMagic() == true )
		{
			if ( lpObj->Magic[i].m_Skill > 300 )
			{
				// Evil Spirit
				if ( SkillNumber == 9 && ( ( lpObj->Magic[i].m_Skill >= 445 && lpObj->Magic[i].m_Skill <= 449 ) || ( lpObj->Magic[i].m_Skill >= 530 && lpObj->Magic[i].m_Skill <= 534 ) ) )
				{
					return false;
				}
				// Twisting Slash
				else if ( SkillNumber == 41 && ( ( lpObj->Magic[i].m_Skill >= 455 && lpObj->Magic[i].m_Skill <= 459 ) || ( lpObj->Magic[i].m_Skill >= 495 && lpObj->Magic[i].m_Skill <= 499 ) ) )
				{
					return false;
				}
				// Mana Shield
				else if ( SkillNumber == 16 && ( lpObj->Magic[i].m_Skill >= 435 && lpObj->Magic[i].m_Skill <= 439 ) )
				{
					return false;
				}
				// Hell Fire
				else if ( SkillNumber == 10 && ( lpObj->Magic[i].m_Skill >= 440 && lpObj->Magic[i].m_Skill <= 444 ) )
				{
					return false;
				}
				// Ice Storm
				else if ( SkillNumber == 39 && ( lpObj->Magic[i].m_Skill >= 450 && lpObj->Magic[i].m_Skill <= 454 ) )
				{
					return false;
				}
				// Death Stab
				else if ( SkillNumber == 43 && ( lpObj->Magic[i].m_Skill >= 460 && lpObj->Magic[i].m_Skill <= 464 ) )
				{
					return false;
				}
				// Rageful Blow
				else if ( SkillNumber == 42 && ( lpObj->Magic[i].m_Skill >= 465 && lpObj->Magic[i].m_Skill <= 469 ) )
				{
					return false;
				}
				// Greater Fortitude
				else if ( SkillNumber == 48 && ( lpObj->Magic[i].m_Skill >= 470 && lpObj->Magic[i].m_Skill <= 474 ) )
				{
					return false;
				}
				// Heal
				else if ( SkillNumber == 26 && ( lpObj->Magic[i].m_Skill >= 475 && lpObj->Magic[i].m_Skill <= 479 ) )
				{
					return false;
				}
				// Greater Defense
				else if ( SkillNumber == 27 && ( lpObj->Magic[i].m_Skill >= 480 && lpObj->Magic[i].m_Skill <= 484 ) )
				{
					return false;
				}
				// Greater Damage
				else if ( SkillNumber == 28 && ( lpObj->Magic[i].m_Skill >= 485 && lpObj->Magic[i].m_Skill <= 489 ) )
				{
					return false;
				}
				// Triple Shot
				else if ( SkillNumber == 24 && ( lpObj->Magic[i].m_Skill >= 490 && lpObj->Magic[i].m_Skill <= 494 ) )
				{
					return false;
				}
				// Fire Slash
				else if ( SkillNumber == 55 && ( lpObj->Magic[i].m_Skill >= 500 && lpObj->Magic[i].m_Skill <= 504 ) )
				{
					return false;
				}
				// Power Slash
				else if ( SkillNumber == 56 && ( lpObj->Magic[i].m_Skill >= 505 && lpObj->Magic[i].m_Skill <= 509 ) )
				{
					return false;
				}
				// Blast
				else if ( SkillNumber == 13 && ( lpObj->Magic[i].m_Skill >= 510 && lpObj->Magic[i].m_Skill <= 514 ) )
				{
					return false;
				}
				// EarthQuake
				else if ( SkillNumber == 62 && ( lpObj->Magic[i].m_Skill >= 515 && lpObj->Magic[i].m_Skill <= 519 ) )
				{
					return false;
				}
				// Fire Burst
				else if ( SkillNumber == 61 && ( lpObj->Magic[i].m_Skill >= 520 && lpObj->Magic[i].m_Skill <= 524 ) )
				{
					return false;
				}
				// Fire Scream
				else if ( SkillNumber == 78 && ( lpObj->Magic[i].m_Skill >= 525 && lpObj->Magic[i].m_Skill <= 529 ) )
				{
					return false;
				}
				// Sleep
				else if ( SkillNumber == 219 && ( lpObj->Magic[i].m_Skill >= 535 && lpObj->Magic[i].m_Skill <= 539 ) )
				{
					return false;
				}
				// Chain Lightning
				else if ( SkillNumber == 215 && ( lpObj->Magic[i].m_Skill >= 540 && lpObj->Magic[i].m_Skill <= 544 ) )
				{
					return false;
				}
				// Lightning Shock
				else if ( SkillNumber == 230 && ( lpObj->Magic[i].m_Skill >= 545 && lpObj->Magic[i].m_Skill <= 549 ) )
				{
					return false;
				}
				// Drain Life
				else if ( SkillNumber == 214 && ( lpObj->Magic[i].m_Skill >= 550 && lpObj->Magic[i].m_Skill <= 554 ) )
				{
					return false;
				}
			}
		}
	}
	// ----
	return true;
}