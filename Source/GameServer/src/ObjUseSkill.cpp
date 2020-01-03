//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Object Use Skills Structures							//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "GameMain.h"
#include "ChaosCastle.h"
#include "Main.H"
#include "user.h"
#include "SkillHitBox.h"
#include "MoveSystem.h"
#include "SkillAdditionInfo.h"
#include "LogProc.H"
#include "MasterSkillSystem.h"
#include "Functions.H"
#include "GenSystem.h"
#include "Party.h"
#include "DuelSystem.h"
#include "CastleSiege.h"
#include "SendProto.H"


CObjUseSkill gObjUseSkill;
// -----------------------------------------------------------------------------------------------------------------------
CObjUseSkill::CObjUseSkill(void)
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
CObjUseSkill::~CObjUseSkill(void)
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::CheckViewportTarget(LPOBJ lpObj,int aIndex,int bIndex,int type)
{
	if(OBJMON_RANGE(aIndex) == 0)
	{
	    return false;
	}

    if(type == OBJ_MONSTER && gObj[aIndex].m_RecallMon < 0)
	{
	    return true;
	}

    if(aIndex == bIndex || aIndex == lpObj->m_iDuelUser)
	{
	    return true;
	}

	int summon = aIndex;

	if(gObj[aIndex].Type == OBJ_MONSTER && gObj[aIndex].m_RecallMon >= 0)
	{
	    summon = gObj[aIndex].m_RecallMon;
	}

	if(gObjTargetGuildWarCheck(&gObj[summon],lpObj) != 0)
	{
	    return true;
	}

    return false;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::CheckAngle(LPOBJ lpObj,int view,int angle,int x,int y,int skill)
{
    int a_angle = angle/2;

	int x_angle = view - a_angle + 360;
	
	int y_angle = view + a_angle + 360;

	int m_angle = this->GetAngle(lpObj->X,lpObj->Y,x,y) + 360;

	if(m_angle > x_angle && m_angle < y_angle)
	{
	    return true;
	}

	return false;
}
// -----------------------------------------------------------------------------------------------------------------------
int  CObjUseSkill::GetTargetLinePath(int sx, int sy, int tx, int ty, int * PathX, int * PathY, int distance)
{
	int x,y;
	int delta_x,delta_y;
	int half;

	int error = 0;

	int incx = 1,incy = 1;
	int dis;
	int count;

	if(sx > tx)
	{
		incx = -1;
		delta_x = sx - tx;
	}
	else
	{
		delta_x = tx - sx;
	}

	if(sy > ty)
	{
		incy = -1;
		delta_y = sy - ty;
	}
	else
	{
		delta_y = ty - sy;
	}

	dis = sqrt((float)(delta_x * delta_x + delta_y * delta_y));
	dis = distance - dis;

	count = 0;
	PathX[count] = sx;
	PathY[count] = sy;
	count++;

	if(delta_x > delta_y)
	{
		y = sy;
		half = delta_x / 2;

		if(incx > 0)
		{
			for(x = (sx+1); x <= (tx + dis); x++)
			{
				error += delta_y;

				if(error > half)
				{
					y += incy;
					error -= delta_x;
				}

				PathX[count] = x;
				PathY[count] = y;

				count++;
			}
		}
		else
		{
			for(x = (sx-1); x >= (tx - dis);x--)
			{
				error += delta_y;

				if(error > half)
				{
					y += incy;
					error -= delta_x;
				}

				PathX[count] = x;
				PathY[count] = y;

				count++;
			}
		}
	}
	else
	{
		x = sx;
		half = delta_y / 2;

		if(incy > 0)
		{
			for(y = (sy+1); y <= (ty + dis);y++)
			{
				error += delta_x;

				if(error > half)
				{
					x += incx;
					error -= delta_y;
				}

				PathX[count] = x;
				PathY[count] = y;
				count++;
			}
		}
		else
		{
			for(y = (sy-1);y >= (ty - dis);y--)
			{
				error += delta_x;

				if(error > half)
				{
					x += incx;
					error -= delta_y;
				}

				PathX[count] = x;
				PathY[count] = y;
				count++;
			}
		}
	}
	return count;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SpeedHackCheck(int aIndex)
{
	/*LPOBJ lpObj = &gObj[aIndex];
	int iTimeCalc = GetTickCount() - lpObj->m_LastAttackTime;

	if(iTimeCalc < lpObj->m_DetectSpeedHackTime && iTimeCalc != 0)
	{
		lpObj->m_DetectCount++;
		lpObj->m_SumLastAttackTime += iTimeCalc;

		if(lpObj->m_DetectCount > gHackCheckCount)
		{
			lpObj->m_DetectedHackKickCount++;

			if(gIsKickDetecHackCountLimit != 0 && lpObj->m_DetectedHackKickCount > gDetectedHackKickCount)
			{
				CLog.LogAdd("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)", lpObj->AccountID,lpObj->Name,lMsg.Get(lpObj->Class+1900),lpObj->m_DetectedHackKickCount);
				CloseClient(aIndex);
				return false;
			}

			lpObj->m_SpeedHackPenalty = gSpeedHackPenalty;
			CLog.LogAdd("[%s][%s] %s Attack Speed Is Wrong MagicSkill (%d)(%d) Penalty %d", lpObj->AccountID,lpObj->Name,lMsg.Get(lpObj->Class+1900),lpObj->m_DetectSpeedHackTime,lpObj->m_SumLastAttackTime / lpObj->m_DetectCount,lpObj->m_SpeedHackPenalty);
		}
	}
	else
	{
		lpObj->m_SumLastAttackTime = 0;
		lpObj->m_DetectCount = 0;
	}

	lpObj->m_LastAttackTime = GetTickCount();

	if(bIsIgnorePacketSpeedHackDetect != 0 && lpObj->m_SpeedHackPenalty > 0)
	{
		lpObj->m_SpeedHackPenalty--;
		CLog.LogAdd("[%s][%s] %s Apply Attack Speed Penalty (%d left)", lpObj->AccountID,lpObj->Name,lMsg.Get(lpObj->Class + 1900),lpObj->m_SpeedHackPenalty);
		return false;
	}*/
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::EnableSkill(WORD Skill)
{
	if(    Skill != AT_SKILL_TWISTER
		&& Skill != AT_SKILL_EVIL
		&& Skill != AT_SKILL_FLAME
		&& Skill != AT_SKILL_HELL_FIRE
		&& Skill != AT_SKILL_BLAST
		&& Skill != AT_SKILL_INFERNO
		&& Skill != AT_SKILL_CROSSBOW
		&& Skill != AT_SKILL_DEVILFIRE
		&& Skill != AT_SKILL_AQUA_BEAM
		&& Skill != AT_SKILL_TWISTING_SLASH
		&& Skill != AT_SKILL_POWER_SLASH
		&& Skill != AT_SKILL_KNIGHTSPEAR
		&& Skill != AT_SKILL_KNIGHTADDLIFE
		&& Skill != AT_SKILL_DEATH_STAB
		&& Skill != AT_SKILL_KNIGHTDINORANT
		&& Skill != AT_SKILL_ELFHARDEN
		&& Skill != AT_SKILL_PENETRATION
		&& Skill != AT_SKILL_DEFENSEDOWN
		&& Skill != AT_SKILL_MAGICDEFENSE
		&& Skill != AT_SKILL_RAGEFUL_BLOW
		&& Skill != AT_SKILL_DECAY
		&& Skill != AT_SKILL_ICE_STORM
		&& Skill != AT_SKILL_NOVA
		&& Skill != AT_SKILL_ELECTRICSPARK
		&& Skill != AT_SKILL_DARKHORSE_ATTACK
		&& Skill != AT_SKILL_BRAND_OF_SKILL
		&& Skill != AT_SKILL_STUN
		&& Skill != AT_SKILL_REMOVAL_STUN
		&& Skill != AT_SKILL_ADD_MANA
		&& Skill != AT_SKILL_INVISIBLE 
		&& Skill != AT_SKILL_REMOVAL_INVISIBLE
		&& Skill != AT_SKILL_REMOVAL_MAGIC
		&& Skill != AT_SKILL_FENRIR_ATTACK
		&& Skill != AT_SKILL_INFINITY_ARROW
		&& Skill != AT_SKILL_FIRESCREAM
		&& Skill != AT_SKILL_DRAIN_LIFE
		&& Skill != AT_SKILL_CHAIN_LIGHTING
		&& Skill != AT_SKILL_ELECTRIC_SURGE
		&& Skill != AT_SKILL_REFLEX
		&& Skill != AT_SKILL_SWORD_POWER
		&& Skill != AT_SKILL_SLEEP
		&& Skill != AT_SKILL_NIGHT
		&& Skill != AT_SKILL_WEAKNESS
		&& Skill != AT_SKILL_INNOVATION
		&& Skill != AT_SKILL_SAHAMUTT
		&& Skill != AT_SKILL_NEIL
		&& Skill != AT_SKILL_GHOST_PHANTOM
		&& Skill != AT_SKILL_RED_STORM
		&& Skill != AT_SKILL_EXPLOTION
		&& Skill != AT_SKILL_MAGIC_CIRCLE
		&& Skill != AT_SKILL_REDUCE_DAMAGE_SHIELD
		&& Skill != AT_SKILL_FIVE_SHOT
		&& Skill != AT_SKILL_SWORD_SLASH
		&& Skill != AT_SKILL_LIGHTING_STORM
		&& Skill != AT_SKILL_BIRDS
		&& Skill != AT_SKILL_LARGE_RING_BLOWER
		&& Skill != AT_SKILL_UPPER_BEAST
		&& Skill != AT_SKILL_CHAIN_DRIVER
		&& Skill != AT_SKILL_DARK_SIDE
		&& Skill != AT_SKILL_DRAGON_LORE
		&& Skill != AT_SKILL_DRAGON_SLAYER
		&& Skill != AT_SKILL_RED_ARMOR_IGNORE
		&& Skill != AT_SKILL_FITNESS
		&& Skill != AT_SKILL_DEFENSE_RATE_INCREASE
		&& Skill != AT_SKILL_CHARGE
		&& Skill != AT_SKILL_NEW
		&& gSkillTree.IsMasterMagic(Skill) == 0
		// Master Skills
		)
	{
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
int  CObjUseSkill::GetUseMana(int aIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	float mana = lpObj->Mana;

	if ( lpObj->Type != OBJ_USER )
	{
		return 0;
	}

	if ( lpMagic == NULL )
	{
		return 0;
	}

	mana -= MagicDamageC.SkillGetMana(lpMagic->m_Skill);

	mana -= this->GetAddUseMana(lpObj,lpMagic);

	if(mana < 0.0f)
	{
		return -1;
	}
	return mana;
}
// -----------------------------------------------------------------------------------------------------------------------
int  CObjUseSkill::GetUseBP(int aIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int bp = lpObj->BP;

	if(lpObj->Type != OBJ_USER)
	{
		return 0;
	}

	if(lpMagic == 0)
	{
		return 0;
	}

	int usingbp = MagicDamageC.SkillGetBP(lpMagic->m_Skill);

	usingbp -= usingbp * lpObj->SetOpDecreaseAG / 100;

	bp -= usingbp;

	if(bp < 0)
	{
		return -1;
	}

	return bp;
}
// -----------------------------------------------------------------------------------------------------------------------
// 0x1E Skills
void CObjUseSkill::UseSkill(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int bCombo = 0;

	if(gComboAttack.CheckCombo(aIndex,lpMagic->m_Skill))
	{
		bCombo = 1;
	}

	if(lpObj->SkillHellFire2State && lpMagic->m_Skill != AT_SKILL_NOVA)
	{
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,AT_SKILL_NOVA);

		if(lpMagic != 0)
		{
			gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
		}

		return;
	}

	int usemana;
	// ----
	if ( lpMagic->m_Skill < 300 )
	{
		usemana = this->GetUseMana(aIndex,lpMagic);
	}
	else
	{
		usemana = gSkillTree.GetMasterSkillMana(lpObj,lpMagic->m_Skill);
	}

	PMSG_DURATION_MAGIC_SEND pMsg;

	if(usemana >= 0)
	{
		int usebp;
		// ----
		if ( lpMagic->m_Skill < 300 )
		{
			usebp = gObjMagicBPUse(&gObj[aIndex],lpMagic);
		}
		else
		{
			usebp = gSkillTree.GetMasterSkillBP(lpObj,lpMagic->m_Skill);
		}

		if(usebp >= 0)
		{
			lpObj->UseMagicNumber = lpMagic->m_Skill;
			lpObj->UseMagicTime = GetTickCount();
			lpObj->UseMagicCount = 0;

			lpObj->Mana = usemana;
			lpObj->BP = usebp;

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);

			C3HeadSet((PBYTE)&pMsg,0x1E,sizeof(pMsg));

			pMsg.MagicNumberH = SET_NUMBERH(lpMagic->m_Skill);
			pMsg.MagicNumberL = SET_NUMBERL(lpMagic->m_Skill);
			pMsg.X = x;
			pMsg.Y = y;
			pMsg.Dir = dir;
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);

			if(lpObj->Type == OBJ_USER)
			{
				gSendProto.DataSend(lpObj->m_Index,(PBYTE)&pMsg,pMsg.h.size);
			}

			if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
			{
				switch(lpMagic->m_Skill)
				{
					case AT_SKILL_MAGICDEFENSE:
					case MASTER_SKILL_SOUL_BARRIER_LVL1:	// New Added
					case MASTER_SKILL_SOUL_BARRIER_LVL2:	// New Added
					case MASTER_SKILL_SOUL_BARRIER_LVL3:	// New Added
					case MASTER_SKILL_SOUL_BARRIER_LVL4:	// New Added
					case MASTER_SKILL_SOUL_BARRIER_LVL5:	// New Added
					case AT_SKILL_HEALING:
					case MASTER_SKILL_HEAL_LVL1:	// New Added
					case MASTER_SKILL_HEAL_LVL2:	// New Added
					case MASTER_SKILL_HEAL_LVL3:	// New Added
					case MASTER_SKILL_HEAL_LVL4:	// New Added
					case MASTER_SKILL_HEAL_LVL5:	// New Added
					case AT_SKILL_DEFENSE:
					case MASTER_SKILL_GREATER_DEFENSE_LVL1:	// New Added
					case MASTER_SKILL_GREATER_DEFENSE_LVL2:	// New Added
					case MASTER_SKILL_GREATER_DEFENSE_LVL3:	// New Added
					case MASTER_SKILL_GREATER_DEFENSE_LVL4:	// New Added
					case MASTER_SKILL_GREATER_DEFENSE_LVL5:	// New Added
					case AT_SKILL_ATTACK:
					case MASTER_SKILL_GREATER_DAMAGE_LVL1:	// New Added
					case MASTER_SKILL_GREATER_DAMAGE_LVL2:	// New Added
					case MASTER_SKILL_GREATER_DAMAGE_LVL3:	// New Added
					case MASTER_SKILL_GREATER_DAMAGE_LVL4:	// New Added
					case MASTER_SKILL_GREATER_DAMAGE_LVL5:	// New Added
					case AT_SKILL_KNIGHTADDLIFE:
					case MASTER_SKILL_GREATER_FORTITUDE_LVL1:	// New Added
					case MASTER_SKILL_GREATER_FORTITUDE_LVL2:	// New Added
					case MASTER_SKILL_GREATER_FORTITUDE_LVL3:	// New Added
					case MASTER_SKILL_GREATER_FORTITUDE_LVL4:	// New Added
					case MASTER_SKILL_GREATER_FORTITUDE_LVL5:	// New Added
					case AT_SKILL_FITNESS:
					case AT_SKILL_DEFENSE_RATE_INCREASE:
					case AT_SKILL_RED_ARMOR_IGNORE:
					case AT_SKILL_MAGIC_CIRCLE:
					case AT_SKILL_REDUCE_DAMAGE_SHIELD:
					case AT_SKILL_WEAKNESS:
					case AT_SKILL_INNOVATION:
					case AT_SKILL_SLEEP:
					case AT_SKILL_SWORD_POWER:
						break;
					default: 
					gSendProto.VPMsgSend(lpObj,(PBYTE)&pMsg,pMsg.h.size);
				}
			}
			else
			{
				gSendProto.VPMsgSend(lpObj,(PBYTE)&pMsg,pMsg.h.size);
			}
		}
		// ----------------------------
		// Skills Attack Functions Call
		// ----------------------------
		switch ( lpMagic->m_Skill )
		{
		// ---- Evil Spirit
		case AT_SKILL_EVIL:							this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_SM_LVL1:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_SM_LVL2:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_SM_LVL3:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_SM_LVL4:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_SM_LVL5:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_MG_LVL1:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_MG_LVL2:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_MG_LVL3:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_MG_LVL4:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;
		case MASTER_SKILL_EVIL_SPIRIT_MG_LVL5:		this->SkillEvilSpirit(aIndex,aTargetIndex,lpMagic);	break;

		// ---- Flame
		case AT_SKILL_FLAME:	this->SkillFlame(aIndex,aTargetIndex,lpMagic,x,y);break;

		// ---- Hell Fire
		case AT_SKILL_HELL_FIRE:			this->SkillHellFire(aIndex,lpMagic,aTargetIndex);break;
		case MASTER_SKILL_HELLFIRE_LVL1:	this->SkillHellFire(aIndex,lpMagic,aTargetIndex);break;
		case MASTER_SKILL_HELLFIRE_LVL2:	this->SkillHellFire(aIndex,lpMagic,aTargetIndex);break;
		case MASTER_SKILL_HELLFIRE_LVL3:	this->SkillHellFire(aIndex,lpMagic,aTargetIndex);break;
		case MASTER_SKILL_HELLFIRE_LVL4:	this->SkillHellFire(aIndex,lpMagic,aTargetIndex);break;
		case MASTER_SKILL_HELLFIRE_LVL5:	this->SkillHellFire(aIndex,lpMagic,aTargetIndex);break;

		// ---- Inferno
		case AT_SKILL_INFERNO:	this->SkillInferno(aIndex,lpMagic,aTargetIndex);break;

		// ---- Power Slash
		case AT_SKILL_POWER_SLASH:				this->SkillPowerSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_POWER_SLASH_LVL1:		this->SkillPowerSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_POWER_SLASH_LVL2:		this->SkillPowerSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_POWER_SLASH_LVL3:		this->SkillPowerSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_POWER_SLASH_LVL4:		this->SkillPowerSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_POWER_SLASH_LVL5:		this->SkillPowerSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;

		// ---- Decay
		case AT_SKILL_DECAY:	this->SkillDeathPoison(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;

		// ---- Ice Storm
		case AT_SKILL_ICE_STORM:			this->SkillIceStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_ICE_STORM_LVL1:	this->SkillIceStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_ICE_STORM_LVL2:	this->SkillIceStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_ICE_STORM_LVL3:	this->SkillIceStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_ICE_STORM_LVL4:	this->SkillIceStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;
		case MASTER_SKILL_ICE_STORM_LVL5:	this->SkillIceStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;

		// ---- Rageful Blow
		case AT_SKILL_RAGEFUL_BLOW:				this->SkillRagefulBlow(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,bCombo);break;
		case MASTER_SKILL_RAGEFUL_BLOW_LVL1:	this->SkillRagefulBlow(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,bCombo);break;
		case MASTER_SKILL_RAGEFUL_BLOW_LVL2:	this->SkillRagefulBlow(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,bCombo);break;
		case MASTER_SKILL_RAGEFUL_BLOW_LVL3:	this->SkillRagefulBlow(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,bCombo);break;
		case MASTER_SKILL_RAGEFUL_BLOW_LVL4:	this->SkillRagefulBlow(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,bCombo);break;
		case MASTER_SKILL_RAGEFUL_BLOW_LVL5:	this->SkillRagefulBlow(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,bCombo);break;

		// ---- Twisting Slash
		case AT_SKILL_TWISTING_SLASH:				this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_BM_LVL1:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_BM_LVL2:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_BM_LVL3:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_BM_LVL4:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_BM_LVL5:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_MG_LVL1:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_MG_LVL2:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_MG_LVL3:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_MG_LVL4:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;
		case MASTER_SKILL_TWISTING_SLASH_MG_LVL5:	this->SkillTwistingSlash(aIndex,lpMagic,aTargetIndex,bCombo);break;

		// ---- Electric Spark
		case AT_SKILL_ELECTRICSPARK:	this->SkillElectricSpark(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);break;

		// ---- Earthquake
		case AT_SKILL_DARKHORSE_ATTACK:		this->SkillDarkHorseAttack(lpObj->m_Index,aTargetIndex,lpMagic);break;

		// ---- Stern
		case AT_SKILL_STUN:		this->SkillStun(aIndex,aTargetIndex,lpMagic,x,y,dir,TargetPos);break;

		// ---- Plasma Storm
		case AT_SKILL_FENRIR_ATTACK:	this->SkillFenrirAttack(aIndex,aTargetIndex,lpMagic);break;

		// ---- Electric Surge
		case AT_SKILL_ELECTRIC_SURGE:	this->SkillElectricSurge(aIndex,aTargetIndex,lpMagic);break;

		// ---- Drain Life
		case AT_SKILL_DRAIN_LIFE:				this->SkillDrainLife(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_DRAIN_LIFE_LVL1:		this->SkillDrainLife(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_DRAIN_LIFE_LVL2:		this->SkillDrainLife(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_DRAIN_LIFE_LVL3:		this->SkillDrainLife(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_DRAIN_LIFE_LVL4:		this->SkillDrainLife(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_DRAIN_LIFE_LVL5:		this->SkillDrainLife(aIndex,aTargetIndex,lpMagic);break;

		// ---- Chain Lightning
		case AT_SKILL_CHAIN_LIGHTING:				this->SkillChainLightning(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_CHAIN_LIGHTNING_LVL1:		this->SkillChainLightning(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_CHAIN_LIGHTNING_LVL2:		this->SkillChainLightning(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_CHAIN_LIGHTNING_LVL3:		this->SkillChainLightning(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_CHAIN_LIGHTNING_LVL4:		this->SkillChainLightning(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_CHAIN_LIGHTNING_LVL5:		this->SkillChainLightning(aIndex,aTargetIndex,lpMagic);break;

		// ---- Sahamutt
		case AT_SKILL_SAHAMUTT:		this->SkillBookNumber12(aIndex,aTargetIndex,lpMagic);break;

		// ---- Neil
		case AT_SKILL_NEIL:		this->SkillBookNumber12(aIndex,aTargetIndex,lpMagic);break;

		// ---- Weakness
		case AT_SKILL_WEAKNESS:	this->SkillWeakness(aIndex,0);break;

		// ---- Innovation
		case AT_SKILL_INNOVATION:	this->SkillInnovation(aIndex,0);break;

		// ---- Ghost Phantom
		case AT_SKILL_GHOST_PHANTOM:	this->SkillGhostPhantomAttack(aIndex,aTargetIndex,lpMagic);break;

		// ---- Lightning Shock
		case AT_SKILL_RED_STORM:					this->SkillRedStorm(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_LIGHTNING_SHOCK_LVL1:		this->SkillRedStorm(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_LIGHTNING_SHOCK_LVL2:		this->SkillRedStorm(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_LIGHTNING_SHOCK_LVL3:		this->SkillRedStorm(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_LIGHTNING_SHOCK_LVL4:		this->SkillRedStorm(aIndex,aTargetIndex,lpMagic);break;
		case MASTER_SKILL_LIGHTNING_SHOCK_LVL5:		this->SkillRedStorm(aIndex,aTargetIndex,lpMagic);break;

		// ---- Explotion
		case AT_SKILL_EXPLOTION:	this->SkillExplotion(aIndex,aTargetIndex,lpMagic,x,y,bCombo);break;

		// ---- Sword Slash
		case AT_SKILL_SWORD_SLASH:	this->SkillSwordSlash(aIndex, aTargetIndex, lpMagic, TargetPos);break;

		// ---- Lightning Storm
		case AT_SKILL_LIGHTING_STORM:	this->SkillLightingStorm(aIndex,aTargetIndex,lpMagic);break;

		// ---- Birds
		case AT_SKILL_BIRDS:	this->SkillBirdsAttack(aIndex,aTargetIndex,lpMagic,TargetPos);	break;

		// ---- Dragon Lore
		case AT_SKILL_DRAGON_LORE:	this->SkillDragonLore(aIndex,aTargetIndex,lpMagic);break;

		case AT_SKILL_NEW:			this->SkillNew(aIndex,aTargetIndex,lpMagic);

		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::UseSkill(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int bCombo = 0;

	if(gComboAttack.CheckCombo(aIndex,lpMagic->m_Skill))
	{
		bCombo = 1;
	}

	if(lpObj->SkillHellFire2State && lpMagic->m_Skill != AT_SKILL_NOVA)
	{
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,AT_SKILL_NOVA);

		if(lpMagic != 0)
		{
			gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
		}

		return;
	}

	if(lpMagic->m_Skill == AT_SKILL_NOVA && aTargetIndex != 58)
	{
		this->RunningSkill(aIndex,aTargetIndex,lpMagic,bCombo);
		lpObj->UseMagicNumber = lpMagic->m_Skill;
		lpObj->UseMagicTime = GetTickCount();
		lpObj->UseMagicCount = 0;
		return;
	}
	int usemana;
	if ( lpMagic->m_Skill < 300 )
	{
		usemana = this->GetUseMana(aIndex,lpMagic);
	}
	else
	{
		usemana = gSkillTree.GetMasterSkillMana(lpObj,lpMagic->m_Skill);
	}

	if(usemana >= 0)
	{
		int usebp;
		if ( lpMagic->m_Skill < 300 )
		{
			usebp = this->GetUseBP(aIndex,lpMagic);
		}
		else
		{
			usebp = gSkillTree.GetMasterSkillBP(lpObj,lpMagic->m_Skill);
		}

		if(usebp >= 0)
		{
			if( this->RunningSkill(aIndex,aTargetIndex,lpMagic,bCombo) == true )
			{
				lpObj->UseMagicNumber = lpMagic->m_Skill;
				lpObj->UseMagicTime = GetTickCount();
				lpObj->UseMagicCount = 0;

				lpObj->Mana = usemana;
				lpObj->BP = usebp;

			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// 0x19 Skills
bool CObjUseSkill::RunningSkill(int aIndex, int aTargetIndex, CMagicInf * lpMagic, BOOL bCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	WORD MagicNumber = lpMagic->m_Skill;

	lpObj->m_SkillNumber = MagicNumber;

	if(lpObj->m_iSkillInvisibleTime > 0)
	{
		this->RemoveCloakingEffect(lpObj->m_Index);
	}

	switch(MagicNumber)
	{
	case AT_SKILL_BLOCKING:
		if(lpObj->pInventory[1].IsItem() == 1)
		{
			if(lpObj->pInventory[1].m_Type >= ITEMGET(6,4) && lpObj->pInventory[1].m_Type < ITEMGET(7,0))
			{
				if(lpObj->pInventory[1].m_Option1 != 0)
				{
					lpObj->m_SkillTime = GetTickCount() + 4000;
					GCActionSend(lpObj,18,lpObj->m_Index,aTargetIndex);
				}
			}
		}
		break;
	case AT_SKILL_POISON:
	case AT_SKILL_METEORITE:
	case AT_SKILL_THUNDER:
	case AT_SKILL_FIREBALL:
	case AT_SKILL_ICE:
	case AT_SKILL_ENERGYBALL:
	case AT_SKILL_SWORD1:
	case AT_SKILL_SWORD2:
	case AT_SKILL_SWORD3:
	case AT_SKILL_SWORD4:
	case AT_SKILL_SWORD5:
	case AT_SKILL_CROSSBOW:
	case MASTER_SKILL_TRIPLE_SHOT_LVL1:
	case MASTER_SKILL_TRIPLE_SHOT_LVL2:
	case MASTER_SKILL_TRIPLE_SHOT_LVL3:
	case MASTER_SKILL_TRIPLE_SHOT_LVL4:
	case MASTER_SKILL_TRIPLE_SHOT_LVL5:
	case AT_SKILL_BOW:
	case AT_SKILL_KNIGHTSPEAR:
	case AT_SKILL_PENETRATION:
	case AT_SKILL_POWER_SLASH:
	case MASTER_SKILL_POWER_SLASH_LVL1:
	case MASTER_SKILL_POWER_SLASH_LVL2:
	case MASTER_SKILL_POWER_SLASH_LVL3:
	case MASTER_SKILL_POWER_SLASH_LVL4:
	case MASTER_SKILL_POWER_SLASH_LVL5:
	case AT_SKILL_DRAGON_SLAYER:
		gObjAttack(lpObj,lpTargetObj,lpMagic,1,0,0,0);
		break;

	// ---- Healing
	case AT_SKILL_HEALING:					this->SkillHealing(aIndex,aTargetIndex,0);break;
	case MASTER_SKILL_HEAL_LVL1:			this->SkillHealing(aIndex,aTargetIndex,1);break;
	case MASTER_SKILL_HEAL_LVL2:			this->SkillHealing(aIndex,aTargetIndex,2);break;
	case MASTER_SKILL_HEAL_LVL3:			this->SkillHealing(aIndex,aTargetIndex,3);break;
	case MASTER_SKILL_HEAL_LVL4:			this->SkillHealing(aIndex,aTargetIndex,4);break;
	case MASTER_SKILL_HEAL_LVL5:			this->SkillHealing(aIndex,aTargetIndex,5);break;

	case AT_SKILL_REDUCE_DAMAGE_SHIELD:		this->SkillRecovery(aIndex,aTargetIndex,lpMagic);break;

	// ---- Soul Barrier
	case AT_SKILL_MAGICDEFENSE:				this->SkillSoulBarrier(aIndex,aTargetIndex,0);break;
	case MASTER_SKILL_SOUL_BARRIER_LVL1:	this->SkillSoulBarrier(aIndex,aTargetIndex,1);break;
	case MASTER_SKILL_SOUL_BARRIER_LVL2:	this->SkillSoulBarrier(aIndex,aTargetIndex,2);break;
	case MASTER_SKILL_SOUL_BARRIER_LVL3:	this->SkillSoulBarrier(aIndex,aTargetIndex,3);break;
	case MASTER_SKILL_SOUL_BARRIER_LVL4:	this->SkillSoulBarrier(aIndex,aTargetIndex,4);break;
	case MASTER_SKILL_SOUL_BARRIER_LVL5:	this->SkillSoulBarrier(aIndex,aTargetIndex,5);break;

	// ---- Greater Fortitude
	case AT_SKILL_KNIGHTADDLIFE:				this->KnightSkillAddLife(aIndex,0);break;
	case MASTER_SKILL_GREATER_FORTITUDE_LVL1:	this->KnightSkillAddLife(aIndex,1);break;
	case MASTER_SKILL_GREATER_FORTITUDE_LVL2:	this->KnightSkillAddLife(aIndex,2);break;
	case MASTER_SKILL_GREATER_FORTITUDE_LVL3:	this->KnightSkillAddLife(aIndex,3);break;
	case MASTER_SKILL_GREATER_FORTITUDE_LVL4:	this->KnightSkillAddLife(aIndex,4);break;
	case MASTER_SKILL_GREATER_FORTITUDE_LVL5:	this->KnightSkillAddLife(aIndex,5);break;
		
	// ---- Increase Defense
	case AT_SKILL_DEFENSE:					this->SkillDefense(aIndex,aTargetIndex,0);break;
	case MASTER_SKILL_GREATER_DEFENSE_LVL1:	this->SkillDefense(aIndex,aTargetIndex,1);break;
	case MASTER_SKILL_GREATER_DEFENSE_LVL2:	this->SkillDefense(aIndex,aTargetIndex,2);break;
	case MASTER_SKILL_GREATER_DEFENSE_LVL3:	this->SkillDefense(aIndex,aTargetIndex,3);break;
	case MASTER_SKILL_GREATER_DEFENSE_LVL4:	this->SkillDefense(aIndex,aTargetIndex,4);break;
	case MASTER_SKILL_GREATER_DEFENSE_LVL5:	this->SkillDefense(aIndex,aTargetIndex,5);break;

	// ---- Increase Attack
	case AT_SKILL_ATTACK:					this->SkillAttack(aIndex,aTargetIndex,0);break;
	case MASTER_SKILL_GREATER_DAMAGE_LVL1:	this->SkillAttack(aIndex,aTargetIndex,1);break;
	case MASTER_SKILL_GREATER_DAMAGE_LVL2:	this->SkillAttack(aIndex,aTargetIndex,2);break;
	case MASTER_SKILL_GREATER_DAMAGE_LVL3:	this->SkillAttack(aIndex,aTargetIndex,3);break;
	case MASTER_SKILL_GREATER_DAMAGE_LVL4:	this->SkillAttack(aIndex,aTargetIndex,4);break;
	case MASTER_SKILL_GREATER_DAMAGE_LVL5:	this->SkillAttack(aIndex,aTargetIndex,5);break;

	// ---- Defense Success Rate Increase
	case AT_SKILL_DEFENSE_RATE_INCREASE:	this->SkillDefenseSuccessRate(aIndex,lpMagic);break;

	// ---- Fitness
	case AT_SKILL_FITNESS:					this->SkillFitness(aIndex,lpMagic);break;

	// ---- Red Armor Ignore
	case AT_SKILL_RED_ARMOR_IGNORE:			this->SkillRedArmorIgnore(aIndex,lpMagic);break;

	// ---- Magic Circle
	case AT_SKILL_MAGIC_CIRCLE:				this->SkillMagicCircle(aIndex,lpMagic);break;

	// ---- Large Ring Blower
	case AT_SKILL_LARGE_RING_BLOWER:		this->SkillLargeRingBlower(aIndex,aTargetIndex,lpMagic);break;

	// ---- Upper Beast
	case AT_SKILL_UPPER_BEAST:				this->SkillUpperBeast(aIndex,aTargetIndex,lpMagic);break;

	// ---- Chain Driver
	case AT_SKILL_CHAIN_DRIVER:				this->SkillChainDriver(aIndex,aTargetIndex,lpMagic);break;
	
	// ---- Dark Side
	case AT_SKILL_DARK_SIDE:				this->SkillDarkSide(aIndex,aTargetIndex,lpMagic);	break;

	// ---- Charge
	case AT_SKILL_CHARGE:					return this->SkillCharge(aIndex,aTargetIndex,lpMagic);break;

	// ---- Night
	case AT_SKILL_NIGHT:					this->SkillNight(aIndex,aTargetIndex,lpMagic);break;

	case AT_SKILL_CALLMON1:					return this->SkillMonsterCall(lpObj->m_Index,26,lpObj->X-1,lpObj->Y+1);break;
	case AT_SKILL_CALLMON2:					return this->SkillMonsterCall(lpObj->m_Index,32,lpObj->X-1,lpObj->Y+1);break;
	case AT_SKILL_CALLMON3:					return this->SkillMonsterCall(lpObj->m_Index,21,lpObj->X-1,lpObj->Y+1);break;
	case AT_SKILL_CALLMON4:					return this->SkillMonsterCall(lpObj->m_Index,20,lpObj->X-1,lpObj->Y+1);break;
	case AT_SKILL_CALLMON5:					return this->SkillMonsterCall(lpObj->m_Index,10,lpObj->X-1,lpObj->Y+1);break;
	case AT_SKILL_CALLMON6:					return this->SkillMonsterCall(lpObj->m_Index,150,lpObj->X-1,lpObj->Y+1);break;
	case AT_SKILL_CALLMON7:					return this->SkillMonsterCall(lpObj->m_Index,151,lpObj->X-1,lpObj->Y+1);break;

	// ---- Nova
	case AT_SKILL_NOVA:						return this->SkillNova(lpObj->m_Index,aTargetIndex,lpMagic);break;

	// ---- Death Stab
	case AT_SKILL_DEATH_STAB:				return this->SkillDeathStab(lpObj->m_Index,aTargetIndex,lpMagic,bCombo,0);break;
	case MASTER_SKILL_DEATH_STAB_LVL1:		return this->SkillDeathStab(lpObj->m_Index,aTargetIndex,lpMagic,bCombo,1);break;
	case MASTER_SKILL_DEATH_STAB_LVL2:		return this->SkillDeathStab(lpObj->m_Index,aTargetIndex,lpMagic,bCombo,2);break;
	case MASTER_SKILL_DEATH_STAB_LVL3:		return this->SkillDeathStab(lpObj->m_Index,aTargetIndex,lpMagic,bCombo,3);break;
	case MASTER_SKILL_DEATH_STAB_LVL4:		return this->SkillDeathStab(lpObj->m_Index,aTargetIndex,lpMagic,bCombo,4);break;
	case MASTER_SKILL_DEATH_STAB_LVL5:		return this->SkillDeathStab(lpObj->m_Index,aTargetIndex,lpMagic,bCombo,5);break;
	
	case AT_SKILL_SPEAR:					return this->SkillSpear(lpObj->m_Index,aTargetIndex,lpMagic);break;

	// ---- Fire Burst
	case AT_SKILL_FIREBURST:				return this->SkillFireBurst(lpObj->m_Index,aTargetIndex,lpMagic,0);break;
	case MASTER_SKILL_FIRE_BURST_LVL1:		return this->SkillFireBurst(lpObj->m_Index,aTargetIndex,lpMagic,1);break;
	case MASTER_SKILL_FIRE_BURST_LVL2:		return this->SkillFireBurst(lpObj->m_Index,aTargetIndex,lpMagic,2);break;
	case MASTER_SKILL_FIRE_BURST_LVL3:		return this->SkillFireBurst(lpObj->m_Index,aTargetIndex,lpMagic,3);break;
	case MASTER_SKILL_FIRE_BURST_LVL4:		return this->SkillFireBurst(lpObj->m_Index,aTargetIndex,lpMagic,4);break;
	case MASTER_SKILL_FIRE_BURST_LVL5:		return this->SkillFireBurst(lpObj->m_Index,aTargetIndex,lpMagic,5);break;

	// ---- Earth Quake
	case AT_SKILL_DARKHORSE_ATTACK:			return this->SkillDarkHorseAttack(lpObj->m_Index,aTargetIndex,lpMagic);break;


	//case AT_SKILL_RECALL_PARTY:				this->SkillRecallParty(aIndex,lpMagic->m_Level);break;

	// ---- Add Critical Damage
	case AT_SKILL_ADD_CRITICALDAMAGE:		this->SkillAddCriticalDamage(aIndex,lpMagic->m_Level);break;
			
	case AT_SKILL_DEFENSEDOWN:				this->MaGumSkillDefenseDown(lpObj->m_Index,aTargetIndex,1);break;
	case MASTER_SKILL_BLOOD_ATTACK_LVL1:	this->MaGumSkillDefenseDown(lpObj->m_Index,aTargetIndex,2);break;	// New Added
	case MASTER_SKILL_BLOOD_ATTACK_LVL2:	this->MaGumSkillDefenseDown(lpObj->m_Index,aTargetIndex,3);break;	// New Added
	case MASTER_SKILL_BLOOD_ATTACK_LVL3:	this->MaGumSkillDefenseDown(lpObj->m_Index,aTargetIndex,4);break;	// New Added
	case MASTER_SKILL_BLOOD_ATTACK_LVL4:	this->MaGumSkillDefenseDown(lpObj->m_Index,aTargetIndex,5);break;	// New Added
	case MASTER_SKILL_BLOOD_ATTACK_LVL5:	this->MaGumSkillDefenseDown(lpObj->m_Index,aTargetIndex,6);break;	// New Added

	case AT_SKILL_SUMMON:					this->SkillSummon(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_IMMUNE_TO_MAGIC:			this->SkillImmuneToMagic(lpObj->m_Index,lpMagic);break;
	case AT_SKILL_IMMUNE_TO_HARM:			this->SkillImmuneToHarm(lpObj->m_Index,lpMagic);break;

	case AT_SKILL_RUSH:						this->SkillKnightRush(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_JAVALIN:					this->SkillWizardJavalin(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_DEEPIMPACT:				this->SkillElfRush(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_ONE_FLASH:				this->SkillMagumOneFlash(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_DEATH_CANNON:				this->SkillMagumDeathCannon(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_SPACE_SPLIT:				this->SkillDarklordSpaceSplit(lpObj->m_Index,aTargetIndex,lpMagic);break;

	case AT_SKILL_BRAND_OF_SKILL:			this->SkillBrandOfSkill(aIndex,aTargetIndex,lpMagic);break;

	case AT_SKILL_REMOVAL_STUN:				this->SkillRemoveStun(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_ADD_MANA:					this->SkillAddMana(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_INVISIBLE:				this->SkillCloaking(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_REMOVAL_INVISIBLE:		this->SkillRemoveCloaking(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_REMOVAL_MAGIC:			this->SkillRemoveMagic(aIndex,aTargetIndex,lpMagic);break;

	case AT_SKILL_INFINITY_ARROW:			this->SkillInfinityArrow(aIndex,aTargetIndex,lpMagic);break;

	case AT_SKILL_REFLEX:					this->SkillReflex(aIndex,0);break;

	// ---- Sleep
	case AT_SKILL_SLEEP:					this->SkillSleep(aIndex,lpMagic,aTargetIndex);break;
	case MASTER_SKILL_SLEEP_LVL1:			this->SkillSleep(aIndex,lpMagic,aTargetIndex);break;
	case MASTER_SKILL_SLEEP_LVL2:			this->SkillSleep(aIndex,lpMagic,aTargetIndex);break;
	case MASTER_SKILL_SLEEP_LVL3:			this->SkillSleep(aIndex,lpMagic,aTargetIndex);break;
	case MASTER_SKILL_SLEEP_LVL4:			this->SkillSleep(aIndex,lpMagic,aTargetIndex);break;
	case MASTER_SKILL_SLEEP_LVL5:			this->SkillSleep(aIndex,lpMagic,aTargetIndex);break;

	case AT_SKILL_SWORD_POWER:				this->SkillBerserker(aIndex,aTargetIndex,0);break;

	default:
		gObjAttack(lpObj,lpTargetObj,lpMagic,1,0,0,0);break;
	}

	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// 0x4A
void CObjUseSkill::UseSkill2(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int bCombo = 0;

	if(gComboAttack.CheckCombo(aIndex,lpMagic->m_Skill))
	{
		bCombo = 1;
	}

	if(lpObj->SkillHellFire2State && lpMagic->m_Skill != AT_SKILL_NOVA)
	{
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,AT_SKILL_NOVA);

		if(lpMagic != 0)
		{
			gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
		}

		return;
	}

	if(lpMagic->m_Skill == AT_SKILL_NOVA && aTargetIndex != 58)
	{
		this->RunningSkill(aIndex,aTargetIndex,lpMagic,bCombo);
		lpObj->UseMagicNumber = lpMagic->m_Skill;
		lpObj->UseMagicTime = GetTickCount();
		lpObj->UseMagicCount = 0;
		return;
	}
	int usemana;
	if ( lpMagic->m_Skill < 300 )
	{
		usemana = this->GetUseMana(aIndex,lpMagic);
	}
	else
	{
		usemana = gSkillTree.GetMasterSkillMana(lpObj,lpMagic->m_Skill);
	}

	if(usemana >= 0)
	{
		int usebp;
		if ( lpMagic->m_Skill < 300 )
		{
			usebp = this->GetUseBP(aIndex,lpMagic);
		}
		else
		{
			usebp = gSkillTree.GetMasterSkillBP(lpObj,lpMagic->m_Skill);
		}

		if(usebp >= 0)
		{
			if( this->RunningSkill(aIndex,aTargetIndex,lpMagic,bCombo) == true )
			{
				lpObj->UseMagicNumber = lpMagic->m_Skill;
				lpObj->UseMagicTime = GetTickCount();
				lpObj->UseMagicCount = 0;

				lpObj->Mana = usemana;
				lpObj->BP = usebp;

				GCManaSend(aIndex,lpObj->Mana,0xFF,0,lpObj->BP);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// 0x4B

// -----------------------------------------------------------------------------------------------------------------------
// Magic Gladiator Defense Down Skill - Checked Function
void CObjUseSkill::MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = 1;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if( lpObj->Type == OBJ_USER && lpObj->Class != CLASS_MAGUMSA )
	{
		return;
	}

	if(retCalcSkillResistance(lpTargetObj,1))
	{
		if ( lpTargetObj->m_SkillMagumReduceDefenseTime <= 0)
		{
			lpTargetObj->m_SkillMagumReduceDefense = 10*skill_level;
		}
		lpTargetObj->m_SkillMagumReduceDefenseTime = 10+skill_level;
		GCStateInfoSend(lpTargetObj,1,eVS_DEFENSE_DOWN);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_DEFENSEDOWN,lpTargetObj->m_Index,skillSuccess);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Knight Add Life Skill - Checked Function
void CObjUseSkill::KnightSkillAddLife(int aIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];

	if( lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1 )
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	int viewplayer = 0;
	int viewpercent = 0;

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}

	if(viewplayer > 1)
	{
		if(viewplayer == 2)
		{
			viewpercent = 1;
		}
		else if(viewplayer == 3)
		{
			viewpercent = 2;
		}
		else if(viewplayer == 4)
		{
			viewpercent = 3;
		}
		else
		{
			viewpercent = 4;
		}
	}

	int iaddLifepower = 12 + (lpObj->Vitality + lpObj->AddVitality) / (100-skill_level) + (lpObj->Energy + lpObj->AddEnergy)/20 + viewpercent;
	int iLifeTime = 60 + (lpObj->Energy + lpObj->AddEnergy) / (10-skill_level);
	int iaddlife;

	if(partynum == -1)
	{
		iaddlife = int(lpObj->MaxLife) * iaddLifepower / 100;
		lpObj->AddLife -= lpObj->m_SkillAddLife;
		lpObj->AddLife += iaddlife;
		lpObj->m_SkillAddLife = iaddlife;

		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
		lpObj->m_SkillAddLifeTime = iLifeTime;

		GCStateInfoSend(lpObj,1,eVS_SWELL_LIFE);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_KNIGHTADDLIFE,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			if(ApplyPartyIndex[n]!= -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				iaddlife = int(lpPartyObj->MaxLife) * iaddLifepower / 100;
				lpPartyObj->AddLife -= lpPartyObj->m_SkillAddLife;
				lpPartyObj->AddLife += iaddlife;
				lpPartyObj->m_SkillAddLife = iaddlife;
				
				GCReFillSend(lpPartyObj->m_Index,lpPartyObj->MaxLife + lpPartyObj->AddLife,0xFE,0,lpPartyObj->iMaxShield + lpPartyObj->iAddShield);
				lpPartyObj->m_SkillAddLifeTime = iLifeTime;

				GCStateInfoSend(lpPartyObj,1,eVS_SWELL_LIFE);
				GCMagicAttackNumberSend(lpObj,AT_SKILL_KNIGHTADDLIFE,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Wizard Mana Shield Skill - Checked Function
void CObjUseSkill::SkillSoulBarrier(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon== -1)
	{
		return;
	}

	if(lpObj->PartyNumber != lpTargetObj->PartyNumber)
	{
		return;
	}
	if(gUser.ChaosCastleMapRange(lpObj->MapNumber) == true && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	lpTargetObj->m_WizardSkillDefense = 10 + (lpObj->Dexterity + lpObj->AddDexterity) / (50-skill_level) + (lpObj->Energy + lpObj->AddEnergy) / 200;
	lpTargetObj->m_WizardSkillDefenseTime = 60 + (lpObj->Energy + lpObj->AddEnergy) / (40-skill_level);
	
	GCStateInfoSend(lpTargetObj, 1, eVS_SOUL_BARRIER);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_MAGICDEFENSE,lpTargetObj->m_Index,skillSuccess);
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Defense Increase Skill - Checked Function
void CObjUseSkill::SkillDefense(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(gUser.ChaosCastleMapRange(lpObj->MapNumber) == true && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}
	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	float skill_defense = float(skill_level + 1)*2 + (lpObj->Energy + lpObj->AddEnergy) / 8;

	if(lpTargetObj->Class == CLASS_KNIGHT || lpTargetObj->Class == CLASS_FIGHTER)
	{
		skill_defense += skill_defense/10.0f;
	}

	lpTargetObj->m_SkillDefense = skill_defense;
	lpTargetObj->m_SkillDefenseTime = 60;
	GCStateInfoSend(lpTargetObj,1,eVS_DEFENSE_AURA);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro.IncAgro(lpObj->m_Index,int(skill_defense)/10);
			}
		}
	}
	GCMagicAttackNumberSend(lpObj,AT_SKILL_DEFENSE,lpTargetObj->m_Index,skillSuccess);
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Damage Increase Skill - Checked Function
void CObjUseSkill::SkillAttack(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(gUser.ChaosCastleMapRange(lpObj->MapNumber) == true && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}
	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	float skill_attack = float((skill_level + 1) * 3) + (lpObj->Energy + lpObj->AddEnergy) / 7.0f;

	if(lpTargetObj->Class == CLASS_KNIGHT || lpTargetObj->Class == CLASS_FIGHTER)
	{
		skill_attack += skill_attack/10.0f;
	}


	lpTargetObj->m_SkillAttack = skill_attack;
	lpTargetObj->m_SkillAttackTime = 60;
	GCStateInfoSend(lpTargetObj,1,eVS_ATTACK_AURA);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro.IncAgro(lpObj->m_Index,int(skill_attack)/10);
			}
		}
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_ATTACK,lpTargetObj->m_Index,skillSuccess);
}
// -----------------------------------------------------------------------------------------------------------------------
// Transformation Rings Skill - Checked Function
bool CObjUseSkill::SkillChangeUse(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int skill_level = -1;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,10))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = lpObj->pInventory[10].m_Level;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,10))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = lpObj->pInventory[11].m_Level;
		}
	}

	if(skill_level > 7)
	{
		return false;
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,39))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 372;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,39))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 372;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,40))	//Halloween Ring
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 373;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,40))	//Halloween Ring
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 373;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,41))	// X-Mas Ring
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 374;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,41))	// X-Mas Ring
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 374;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,42))	// GM Ring
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 378;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,42))	// GM Ring
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 378;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,68))	// Snowman Ring
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 477;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && skill_level == -1 && lpObj->pInventory[10].m_Type == ITEMGET(13,68))	// Snowman Ring
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 477;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,76))	// Panda Ring
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 503;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && skill_level == -1 && lpObj->pInventory[10].m_Type == ITEMGET(13,76))	// Panda Ring
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 503;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,122))	// Skeleton Ring
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 548;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && skill_level == -1 && lpObj->pInventory[10].m_Type == ITEMGET(13,122))	// Skeleton Ring
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 548;
		}
	}

	int change = -1;

	switch(skill_level)
	{
	case 0: change = 2;	break;
	case 1: change = 7; break;
	case 2: change = 14;	break;
	case 3: change = 8;	break;
	case 4:	change = 9;	break;
	case 5:	change = 41;	break;
	case 372:	change = skill_level;	break;
	case 373:	change = skill_level;	break;
	case 374:	change = skill_level;	break;
	case 378:	change = skill_level;	break;
	case 477:   change = skill_level;   break;
	case 503:   change = skill_level;   break;
	case 548:   change = skill_level;   break;
	default: change = -1;	break;
	}

	if(lpObj->m_Change >= 0 && lpObj->m_Change == change)
	{
		return false;
	}

	if(lpObj->m_Change == 41 && change != 41)
	{
		lpObj->AddLife = 0;

		GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
	}

	if(lpObj->m_Change == 372 && change != 372)
	{
		lpObj->AddLife = 0;
		gCalCharacter.gObjCalCharacter(lpObj->m_Index);

		GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
	}

	if(lpObj->m_Change == 378 && change != 378)
	{
		lpObj->AddLife = 0;
		gCalCharacter.gObjCalCharacter(lpObj->m_Index);

		GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
	}

	if(lpObj->m_Change == 477 && change != 477)
	{
		lpObj->AddLife = 0;
		gCalCharacter.gObjCalCharacter(lpObj->m_Index);

		GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
	}

	lpObj->m_Change = change;
	gObjViewportListProtocolCreate(lpObj);

	PMSG_KILLCOUNT pMsg = {0};

	C1SubHeadSet((LPBYTE)&pMsg,0xB8,0x01,sizeof(pMsg));

	pMsg.btKillCount = lpObj->m_btKillCount;

	gSendProto.DataSend(aIndex,(PBYTE)&pMsg,sizeof(pMsg));
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Healing Skill - Checked Function
void CObjUseSkill::SkillHealing(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(gUser.ChaosCastleMapRange(lpObj->MapNumber) == true && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}
	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	lpTargetObj->Life = ((skill_level + 1) * 5 + (lpObj->Energy + lpObj->AddEnergy) / 5) + lpTargetObj->Life;

	if((lpTargetObj->MaxLife + lpTargetObj->AddLife) < lpTargetObj->Life)
	{
		lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
	}

	if(lpTargetObj->Type == OBJ_USER)
	{
		GCReFillSend(lpTargetObj->m_Index,lpTargetObj->Life,0xFF,0,lpTargetObj->iShield);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_HEALING,lpTargetObj->m_Index,skillSuccess);

		for(int n = 0; n < MAX_VIEWPORT; n++)
		{
			if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
			{
				LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
				if(lpMonster->m_iCurrentAI != 0)
				{
					lpMonster->m_Agro.IncAgro(lpObj->m_Index,(lpObj->Energy / 5)/40);
				}
			}
		}
	}
	else if(lpTargetObj->m_RecallMon >= 0)
	{
		GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Monster Call Skill - Checked Function
bool CObjUseSkill::SkillMonsterCall(int aIndex, int MonsterType, int x, int y)
{
	if(gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		return false;
	}
	if(gUser.ChaosCastleMapRange(gObj[aIndex].MapNumber) == true)
	{
		return false;
	}
	if(gObj[aIndex].m_RecallMon >= 0)
	{
		GCRecallMonLife(aIndex,60,0);
		gObjMonsterCallKill(aIndex);
		return false;
	}
	int result = gObjAddCallMon();

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;
		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;
		gObj[result].MapNumber = gObj[aIndex].MapNumber;
		gObjSetMonster(result,MonsterType);
		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;
		gObj[result].TargetNumber = (WORD)-1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;
		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Knight Rageful Blow Skill - Checked Function
void CObjUseSkill::SkillRagefulBlow(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int HitCount = 0;
	int bAttack = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 4)
					{
						bAttack = 0;

						if(HitCount > 15)
						{
							bAttack = 0;
						}

						if(HitCount >= 10)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,isCombo);

							if(isCombo != 0)
							{
								GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,aIndex,1);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Knight & Magic Gladiator Twisting Slash Skill - Checked Function
void CObjUseSkill::SkillTwistingSlash(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					int Distance;
					// ----
					if ( lpMagic->m_Skill < 300 )
					{
						Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
					}
					else
					{
						Distance = gSkillTree.GetMasterSkillDistance(lpMagic->m_Skill);
					}
					// ----
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < Distance)
					{
						bAttack = 0;

						if(HitCount > 15)
						{
							bAttack = 0;
						}

						if(HitCount >= 10)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{

							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,isCombo);

							if(isCombo != 0)
							{
								GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,gObj[tObjNum].m_Index,1);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Magic Gladiator Power Slash Skill - Checked Function
void CObjUseSkill::SkillPowerSlash(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE targetangle, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int StartDis = 1;
	int tObjNum;
	::SkillFrustrum(targetangle,aIndex);
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if( SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex) == true )
					{
						bAttack = 0;

						if(HitCount > 10)
						{
							bAttack = 0;
						}
						
						if(HitCount >= 5)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
						}

						HitCount++;
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Wizard Decay Skill - Checked Function
bool CObjUseSkill::SkillDeathPoison(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int FirstHit = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
					{
						bAttack = 0;

						if(HitCount > 10)
						{
							bAttack = 0;
						}
						
						if(HitCount >= 5)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							int delay = rand()%500;
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delay,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SkillIceStorm(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int FirstHit = 0;
	int HitCount = 0;
	int bAttack;
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
					{
						bAttack = 0;

						if(HitCount > 10)
						{
							bAttack = 0;
						}
						
						if(HitCount >= 5)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							int delay = rand()%500;
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Wizard Nova Skill - Checked Function
bool CObjUseSkill::SkillNova(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(aTargetIndex == 58)
	{
		return this->SkillHellFire2Start(aIndex,lpMagic);
	}

	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;

	if(lpObj->SkillHellFire2State == 0)
	{
		return false;
	}

	lpObj->SkillHellFire2State = 0;
	int count = 0;
	lpObj->SkillHellFire2Time = 0;

	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(aTargetIndex == tObjNum || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= 6)
					{
						gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,600,lpMagic->m_Skill,0);
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_NOVA,lpObj->m_Index,1);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Wizard Nova Skill Charge - Checked Function
bool CObjUseSkill::SkillHellFire2Start(int aIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->SkillHellFire2State != 0)
	{
		return false;
	}

	lpObj->SkillHellFire2Time = GetTickCount();
	lpObj->SkillHellFire2State = 1;
	lpObj->SkillHellFire2Count = 0;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_EXPHELL_START,aIndex,1);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Knight Combo Skill - Checked Function
bool CObjUseSkill::SkillDeathStab(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo, int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	int StartDis = 1;
	int tObjNum;

	float fangle = this->GetAngle(lpObj->X,lpObj->Y,gObj[aTargetIndex].X,gObj[aTargetIndex].Y);

	this->SkillFrustrum(aIndex,fangle,1.5f,3.0f);

	int count = 0;
	int loopcount = 0;
	int attackcheck;
	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					attackcheck = 0;

					if(loopcount == 0)
					{
						attackcheck = 1;
					}
					else if(rand()%3==0)
					{
						attackcheck = 1;
					}

					if(attackcheck != 0)
					{
						if(::SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex))
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,isCombo);
							if(isCombo != 0)
							{
								GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,gObj[tObjNum].m_Index,1);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			loopcount++;
			count = 0;
			if(loopcount >= 3)
			{
				break;
			}
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Skills Distance and Angle - Checked Function
void CObjUseSkill::SkillFrustrum(int aIndex, float fangle, float tx, float ty)
{
	LPOBJ lpObj = &gObj[aIndex];

	float p[MAX_ARRAY_FRUSTRUM][3];

	p[0][0] = -tx;
	p[0][1] = ty;
	p[0][2] = 0;
	p[1][0] = tx;
	p[1][1] = ty;
	p[1][2] = 0;
	p[2][0] = 1.0f;
	p[2][1] = 0;
	p[2][2] = 0;
	p[3][0] = -1.0f;
	p[3][1] = 0;
	p[3][2] = 0;

	float Angle[3];

	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = fangle;

	float Matrix[3][MAX_ARRAY_FRUSTRUM];
	float vFrustrum[MAX_ARRAY_FRUSTRUM][3];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Skills Distance and Angle - Checked Function
void CObjUseSkill::SkillFrustrum2(int aIndex, float fangle, float tx, float ty)
{
	LPOBJ lpObj = &gObj[aIndex];


	float p[MAX_ARRAY_FRUSTRUM][3];

	p[0][0] = 0;
	p[0][1] = ty;
	p[0][2] = 0;
	p[1][0] = 0;
	p[1][1] = ty;
	p[1][2] = 0;
	p[2][0] = tx;
	p[2][1] = 1.0f;
	p[2][2] = 0;
	p[3][0] = -tx;
	p[3][1] = 1.0f;
	p[3][2] = 0;

	float Angle[3];

	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = fangle;

	float Matrix[3][MAX_ARRAY_FRUSTRUM];
	float vFrustrum[MAX_ARRAY_FRUSTRUM][3];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Get Angle Function - Checked Function
int  CObjUseSkill::GetAngle(int x, int y, int tx, int ty)
{
	double diffX = x - tx;
	double diffY = y - ty;

	double rad = atan2(abs(diffY),abs(diffX));

	int angle = rad * 180 / 3.14159265358 + 90;

	if(angle < 0)
	{
		angle += 360;
	}
	return angle;
}
// -----------------------------------------------------------------------------------------------------------------------
// Get Distance Function - Checked Function
int  CObjUseSkill::CalDistance(int x, int y, int x1, int y1)
{
	if(x == x1 && y == y1)
	{
		return 0;
	}

	float tx = x - x1;
	float ty = y - y1;

	return sqrt(tx*tx + ty*ty);
}
// -----------------------------------------------------------------------------------------------------------------------
// Skill Attack Area Check - Checked Function
bool CObjUseSkill::SkillAreaCheck(int angel, int x, int y, int tx, int ty, int dis, int arc)
{
	int minangel = angel - arc;
	int maxangel = angel + arc;

	if(this->CalDistance(x,y,tx,ty) > dis)
	{
		return false;
	}

	int targetangle = this->GetAngle(x,y,tx,ty);

	if(minangel < 0)
	{
		if(targetangle > maxangel)
		{
			if(targetangle > minangel + 360)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(maxangel < targetangle)
		{
			return false;
		}
	}
	else if(maxangel > 360)
	{
		if(targetangle < minangel)
		{
			if(targetangle < maxangel - 360)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(maxangel < targetangle)
		{
			return false;
		}
	}
	else if(minangel > targetangle)
	{
		return false;
	}
	else if(maxangel < targetangle)
	{
		return false;
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Force Skill - Checked Function
bool CObjUseSkill::SkillSpear(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int StartDis = 1;
	int tObjNum;
	int iangle = this->GetAngle(lpObj->X,lpObj->Y,gObj[aTargetIndex].X,gObj[aTargetIndex].Y);
	int count = 0;
	int loopcount = 0;
	int attackcheck;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	if(lpObj->SkillLongSpearChange == 0)
	{
		gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
		return true;
	}

	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					attackcheck = 0;

					if(loopcount == 0)
					{
						attackcheck = 1;
					}
					else if(rand()%3==0)
					{
						attackcheck = 1;
					}

					if(attackcheck != 0)
					{
						if(SkillSpearHitBox.HitCheck(iangle,lpObj->X,lpObj->Y,gObj[tObjNum].X,gObj[tObjNum].Y))
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Fire Burst Skill - Checked Function
bool CObjUseSkill::SkillFireBurst(int aIndex, int aTargetIndex, CMagicInf * lpMagic,int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	int StartDis = 1;
	int tObjNum;
	int count = 0;
	int loopcount = 0;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);

	int attackcheck;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int delaytime;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0 && aTargetIndex != tObjNum)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					attackcheck = 0;

					if(loopcount == 0)
					{
						attackcheck = 1;
					}
					else if(rand()%3==0)
					{
						attackcheck = 1;
					}

					if(attackcheck != 0)
					{
						if(gObjCalDistance(&gObj[aTargetIndex],&gObj[tObjNum]) < 3)
						{
							delaytime = (rand()*17)%300 + 500;

							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Dark Horse Skill - Checked Function
bool CObjUseSkill::SkillDarkHorseAttack(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_DARKHORSE_ATTACK,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
	
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;
	
						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
	
						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}
	
					if(EnableAttack != 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
						{
							bAttack = 0;
	
							if(HitCount > 15)
							{
								bAttack = 0;
							}
	
							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}
	
							if(bAttack != 0)
							{
								gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,0);
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Summon Skill - Checked Function
BOOL CObjUseSkill::RecallPartyCheck(int aIndex, int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	int number;
	int partynum = 0;

	partynum = lpObj->PartyNumber;

	int partycount;
	LPOBJ lpPartyObj;
	int recallcount = 0;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpPartyObj->m_Index != lpObj->m_Index)
				{
					if(lpPartyObj->MapNumber == lpObj->MapNumber)
					{
						if(lpPartyObj->SkillRecallParty_Time != 0)
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Summon Skill - Make new function
void CObjUseSkill::SkillRecallParty(int aIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];

	if( lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1 )
	{
		return;
	}

	if(this->RecallPartyCheck(aIndex,skill_level) == 0)
	{
		MsgOutput(aIndex,lMsg.Get(1238));
		return;
	}

	/*if(gMoveCommand.CheckMainToMove(lpObj) == 0)
	{
		char msg[255];
		wsprintf(msg,lMsg.Get(1249));
		GCServerMsgStringSend(msg,lpObj->m_Index,1);
		return;
	}

	if(gMoveCommand.CheckInterfaceToMove(lpObj) == 0)
	{
		char msg[255];
		wsprintf(msg,lMsg.Get(1239));
		GCServerMsgStringSend(msg,lpObj->m_Index,1);
		return;
	}*/

	//int number;
	int partynum;
	//int partycount;
	//LPOBJ lpPartyObj;
	int recallcount;

	partynum = 0;
	partynum = lpObj->PartyNumber;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_RECALL_PARTY,lpObj->m_Index,skillSuccess);

	recallcount = 0;

	/*if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY;n ++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpPartyObj->m_Index != lpObj->m_Index)
				{
					int movelevel = gMoveCommand.GetMoveLevel(lpObj->MapNumber,lpObj->X,lpObj->Y,lpPartyObj->Class);
					int bCheckMainToMove = gMoveCommand.CheckMainToMove(lpPartyObj);
					int bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove(lpPartyObj,lpObj->MapNumber);
					int bCheckInterfaceToMove = gMoveCommand.CheckInterfaceToMove(lpPartyObj);

					if(lpPartyObj->Level >= movelevel
						&& movelevel != -1
						&& bCheckMainToMove != false
						&& bCheckEquipmentToMove != false
						&& bCheckInterfaceToMove != false)
					{
						int addx;
						int addy;
						int count = 50;
						int Find = 0;

						while(count--)
						{
							addx = lpObj->X + rand()%9 - 4;
							addy = lpObj->Y + rand()%9 - 4;

							if(gObjCheckTeleportArea(lpObj->m_Index,addx,addy))
							{
								Find = 1;
								break;
							}
						}

						if(Find == 0)
						{
							addx = lpObj->X;
							addy = lpObj->Y;
						}

						lpPartyObj->SkillRecallParty_Time = 7;
						lpPartyObj->SkillRecallParty_MapNumber = lpObj->MapNumber;
						lpPartyObj->SkillRecallParty_X = addx;
						lpPartyObj->SkillRecallParty_Y = addy;

						char msg[255];

						wsprintf(msg,lMsg.Get(1251));
						GCServerMsgStringSend(msg,lpPartyObj->m_Index,1);
						recallcount++;
					}
					else
					{
						char msg[255];
						wsprintf(msg,lMsg.Get(1252));
						GCServerMsgStringSend(msg,lpPartyObj->m_Index,1);
					}
				}
			}
		}
	}*/

	if(recallcount != 0)
	{
		char msg[255];
		wsprintf(msg,lMsg.Get(1244),recallcount);
		GCServerMsgStringSend(msg,lpObj->m_Index,1);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Add Critical Damage - Checked Function
void CObjUseSkill::SkillAddCriticalDamage(int aIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	int number;

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);
					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int addcriticaldamagevalue = (lpObj->Leadership + lpObj->AddLeadership)/25 + (lpObj->Energy + lpObj->AddEnergy)/30;
	int SkillTime = (lpObj->Energy + lpObj->AddEnergy)/10 + 60;

	if(partynum == -1)
	{
		lpObj->SkillAddCriticalDamageTime = SkillTime;
		lpObj->SkillAddCriticalDamage = addcriticaldamagevalue;
		lpObj->m_CriticalDamage += addcriticaldamagevalue;
		GCStateInfoSend(lpObj,1,eVS_CRITICAL_DAMAGE);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_CRITICALDAMAGE,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0;n < MAX_USER_IN_PARTY;n++)
		{
			if(ApplyPartyIndex[n] != -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				lpPartyObj->SkillAddCriticalDamageTime = SkillTime;
				lpPartyObj->SkillAddCriticalDamage = addcriticaldamagevalue;
				lpPartyObj->m_CriticalDamage += addcriticaldamagevalue;
				GCStateInfoSend(lpPartyObj,1,eVS_CRITICAL_DAMAGE);
				GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_CRITICALDAMAGE,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Electric Spark - Checked Function
void CObjUseSkill::SkillElectricSpark(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int TargePosx = lpObj->X - (8 - (TargetPos & 15));
	int TargePosy = lpObj->Y - (8 - ((TargetPos & 240) >> 4));

	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int iangle = this->GetAngle(lpObj->X,lpObj->Y,TargePosx,TargePosy);
	int delaytime;
	int Hit = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(SkillElectricSparkHitBox.HitCheck(iangle,gObj[aIndex].X,gObj[aIndex].Y,gObj[tObjNum].X,gObj[tObjNum].Y))
					{
						delaytime = 500;
						Hit = 1;

						gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	if(Hit != 0)
	{
		DecPartyMemberHPandMP(lpObj);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Decrease Party Members HP and MP because of Electric Spark - Checked Function
int  DecPartyMemberHPandMP(LPOBJ lpObj)
{
	LPOBJ lpPartyObj;
	int partynum = lpObj->PartyNumber;

	if( partynum == -1 )
		return false;

	if(OBJMAX_RANGE(partynum)==false)
	{
		CLog.LogAdd("error : DecPartyMemberHPandMP %s %d",__FILE__,__LINE__);
		return false;
	}

	int partycount = gParty.m_PartyInfo[partynum].Count;
	int retcount = 0;

	for(int n = 0; n < MAX_USER_IN_PARTY;n++)
	{
		int number = gParty.m_PartyInfo[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpObj->MapNumber == lpPartyObj->MapNumber && lpObj->m_Index != number)
			{
				int dis = gObjCalDistance(lpObj,&gObj[number]);
				if(dis < 10)
				{
					lpPartyObj = &gObj[number];

					int declife = lpPartyObj->Life * 20.0f / 100.0f;
					int decmana = lpPartyObj->Mana * 5.0f / 100.0f;

					lpPartyObj->Life = lpPartyObj->Life - declife;
					lpPartyObj->Mana = lpPartyObj->Mana - decmana;

					GCReFillSend(lpPartyObj->m_Index,lpPartyObj->Life,0xFF,0,lpPartyObj->iShield);
					GCManaSend(lpPartyObj->m_Index,lpPartyObj->Mana,0xFF,0,lpPartyObj->BP);
					retcount++;
				}
			}
		}
	}

	return retcount;
}
// -----------------------------------------------------------------------------------------------------------------------
// Summon Monster Skill - Checked Function
BOOL CObjUseSkill::SkillSummon(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex)==false)
	{
		return false;
	}

	int iSummonCount = 1;
	int iSummonMonterType = 0;
	BYTE bDoSummon = 1;
	BYTE bSummonSuccess = 0;
	BYTE cSummonX = 0;
	BYTE cSummonY = 0;

	LPOBJ lpObj = &gObj[aIndex];

	switch(lpObj->Class)
	{
	case 161:
		iSummonMonterType = rand()%2+ 147;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 181:
		iSummonMonterType = rand()%2+ 177;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 189:
		iSummonMonterType = rand()%2+ 185;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 197:
		iSummonMonterType = rand()%2+ 193;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 267:
		iSummonMonterType = rand()%2+ 263;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 275:
		iSummonMonterType = rand()%2+ 271;
		bDoSummon = (rand()%15) ? false : true;
		break;
	}

	if(bDoSummon == false)
	{
		return false;
	}

	for(int iSC = 0; iSC < iSummonCount; iSC++)
	{
		BYTE cSX = lpObj->X;
		BYTE cSY = lpObj->Y;

		if(gObjGetRandomFreeLocation(lpObj->MapNumber,(BYTE &)cSX,(BYTE &)cSY,2,2,30)==1)
		{
			int result = gObjAddMonster(lpObj->MapNumber);

			if(result >= 0)
			{
				gObj[result].m_PosNum = (WORD)-1;
				gObj[result].X = cSX;
				gObj[result].Y = cSY;
				gObj[result].MapNumber = lpObj->MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = 1;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result,iSummonMonterType);
				gObj[result].m_Attribute = 60;
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = rand()%8;
				bSummonSuccess = 1;
				cSummonX = gObj[result].StartX;
				cSummonY = gObj[result].StartY;
			}
		}
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_SUMMON,(((cSummonX & 0xFF)&0xFF) | ((cSummonY & 0xFF)&0xFF) << 8) & 0xFFFF,bSummonSuccess);
	return bSummonSuccess;
}
// -----------------------------------------------------------------------------------------------------------------------
// Magic Attack Inmune Skill - Checked Function
BOOL CObjUseSkill::SkillImmuneToMagic(int aIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_ImmuneToMagicCount <= 0 && lpObj->m_ImmuneToHarmCount <= 0)
	{
		lpObj->m_ImmuneToMagicCount = 10;
		GCStateInfoSend(lpObj,1,eVS_INMUNE_TO_MAGIC);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_IMMUNE_TO_MAGIC,lpObj->m_Index,1);
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------------------------------------------------------
// Physical Attack Inmune Skill - Checked Function
BOOL CObjUseSkill::SkillImmuneToHarm(int aIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_ImmuneToMagicCount <= 0 && lpObj->m_ImmuneToHarmCount <= 0)
	{
		lpObj->m_ImmuneToHarmCount = 10;
		GCStateInfoSend(lpObj,1,eVS_DEFENSE_AURA);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_IMMUNE_TO_HARM,lpObj->m_Index,1);
		return true;
	}
	return false;
}
// -----------------------------------------------------------------------------------------------------------------------
// Remove Character Invalid Magics And Skills States - Checked Function
BOOL CObjUseSkill::RemoveAllCharacterInvalidMagicAndSkillState(LPOBJ lpObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}
	
	lpObj->m_SkillAttackTime = 0;
	lpObj->m_SkillAttack = 0;

	// ---- Increase Defense
	lpObj->m_SkillDefenseTime = 0;
	lpObj->m_SkillDefense = 0;
	GCStateInfoSend(lpObj,0,eVS_DEFENSE_AURA);

	// ---- Increase Attack
	lpObj->m_SkillAttackTime2 = 0;
	lpObj->m_SkillAttack2 = 0;
	GCStateInfoSend(lpObj,0,eVS_ATTACK_AURA);

	// ---- Mana Shield
	lpObj->m_WizardSkillDefenseTime = 0;
	lpObj->m_WizardSkillDefense = 0;
	GCStateInfoSend(lpObj,0,eVS_SOUL_BARRIER);

	// ---- Greater Fortitude
	lpObj->m_SkillAddLifeTime = 0;
	lpObj->m_SkillAddLife = 0;
	GCStateInfoSend(lpObj,0,eVS_SWELL_LIFE);

	// ---- Ice Arrow
	lpObj->m_SkillHardenTime = 0;
	lpObj->m_SkillHarden = 0;
	GCStateInfoSend(lpObj,0,eVS_ICE_ARROW);

	// ---- Defense Down
	lpObj->m_SkillMagumReduceDefenseTime = 0;
	lpObj->m_SkillMagumReduceDefense = 0;
	GCStateInfoSend(lpObj,0,eVS_DEFENSE_DOWN);

	// ---- Add Critical Damage
	lpObj->SkillRecallParty_Time = 0;
	lpObj->SkillAddCriticalDamageTime = 0;
	lpObj->SkillAddCriticalDamage = 0;
	GCStateInfoSend(lpObj,0,eVS_CRITICAL_DAMAGE);

	lpObj->m_iSkillStunTime = 0;
	GCStateInfoSend(lpObj,0,eVS_STUN);

	lpObj->m_iSkillBrandOfSkillTime = 0;

	lpObj->m_ViewSkillState[eVS_SACRIFICE] = 0;

	lpObj->m_iSkillInvisibleTime = 0;

	lpObj->m_ViewSkillState[eVS_TRANSPARENCY] = 0;

	lpObj->m_iSkillManaSwellTime = 0;
	lpObj->m_iSkillManaSwell = 0;

	lpObj->m_ViewSkillState[eVS_SWELL_MANA] = 0;

	// ---- Poison
	lpObj->m_PoisonBeattackCount = 0;
	lpObj->m_PoisonType = 0;
	GCStateInfoSend(lpObj,0,eVS_POISON);

	// ---- Ice
	lpObj->m_ColdBeattackCount = 0;
	lpObj->m_IceType = 0;
	GCStateInfoSend(lpObj,0,eVS_ICE);

	lpObj->m_ImmuneToMagicCount = 0;
	lpObj->m_ImmuneToHarmCount = 0;

	// ---- Infinity Arrow
	lpObj->m_iMuseElfInfinityArrowSkillTime = 0;
	GCStateInfoSend(lpObj,0,eVS_INFINITY_ARROW);

	// ---- Increase Defense Success Rate
	lpObj->m_RageFighterSkillDefenseRateTime = 0;
	lpObj->m_RageFighterSkillDefenseRate = 0;
	GCStateInfoSend(lpObj,0,eVS_DEFENSE_SUCCESS_RATE);

	// ---- Fitness
	lpObj->m_RageFighterSkillFitnessAddLife = 0;
	lpObj->m_RageFighterSkillFitnessTime = 0;
	GCStateInfoSend(lpObj,0,eVS_FITNESS);

	// ---- Red Armor Ignore
	lpObj->m_RageFighterSkillRedArmorIgnore = 0;
	lpObj->m_RageFighterSkillRedArmorIgnoreTime = 0;
	GCStateInfoSend(lpObj,0,eVS_IGNORE_DEFENSE);

	// ---- Weakness
	lpObj->m_SkillWeakness = 0;
	lpObj->m_SkillWeaknessTime = 0;
	GCStateInfoSend(lpObj,0,eVS_SKILL_NICEWEEK);
	
	// ---- Innovation
	lpObj->m_SkillInnovation = 0;
	lpObj->m_SkillInnovationTime = 0;
	GCStateInfoSend(lpObj,0,eVS_SKILL_INNERBEIYSEYON);

	// ---- Night
	lpObj->m_SkillNight = 0;
	lpObj->m_SkillNightTime = 0;
	GCStateInfoSend(lpObj,0,eVS_SKILL_BLIDNESS);

	// ---- Sleep
	lpObj->m_SkillSleepTime = 0;
	GCStateInfoSend(lpObj,0,eVS_SKILL_SLEEP);
	
	// ---- Reflex
	lpObj->m_SkillReflex = 0;
	lpObj->m_SkillReflexTime = 0;
	GCStateInfoSend(lpObj,0,eVS_SKILL_DAMAGEREFLECTION);

	// ---- Bless Potion
	lpObj->m_iPotionBlessTime = 0;
	lpObj->m_ViewSkillState[eVS_POTION_BLESS] = 0;

	// ---- Soul Potion
	lpObj->m_iPotionSoulTime = 0;
	lpObj->m_ViewSkillState[eVS_POTION_SOUL] = 0;

	lpObj->SkillMagicCircleTime = 0;
	lpObj->SkillMagicCircle = 0;
	GCStateInfoSend(lpObj,0,eVS_SKILL_MAGICCIRCLE);

	return true;

}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Knight Crescent Moon Slash (Siege Skill) - Checked Function
BOOL CObjUseSkill::SkillKnightRush(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Wizard & Summoner Mana Glaive (Siege Skill) - Checked Function
BOOL CObjUseSkill::SkillWizardJavalin(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int iDelay = 500;

	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,iDelay,lpMagic->m_Skill,0);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,iDelay+200,lpMagic->m_Skill,0);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Starfall (Siege Skill) - Checked Function
BOOL CObjUseSkill::SkillElfRush(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,800,lpMagic->m_Skill,0);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Magic Gladiator Spiral Slash (Siege Skill) - Checked Function
BOOL CObjUseSkill::SkillMagumOneFlash(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,100,lpMagic->m_Skill,0);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Magic Gladiator Mana Rays (Siege Skill) - Checked Function
BOOL CObjUseSkill::SkillMagumDeathCannon(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Fire Blast (Siege Skill) - Checked Function
BOOL CObjUseSkill::SkillDarklordSpaceSplit(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,500,lpMagic->m_Skill,0);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Siege Skill - Checked Function
BOOL CObjUseSkill::SkillBrandOfSkill(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int count = 0;

	lpObj->m_iSkillBrandOfSkillTime = 10;
	lpObj->m_ViewSkillState[eVS_SACRIFICE] = 1;
	GCStateInfoSend(lpObj,1,eVS_SACRIFICE);

	GCMagicAttackNumberSend(lpObj,AT_SKILL_BRAND_OF_SKILL,lpObj->m_Index,1);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum))
			{
				if(lpObj->VpPlayer2[count].type == OBJ_USER)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
					{
						gObj[tObjNum].m_iSkillBrandOfSkillTime = 10;
						GCStateInfoSend(&gObj[tObjNum],1,eVS_SACRIFICE);
						GCMagicAttackNumberSend(lpObj,AT_SKILL_BRAND_OF_SKILL,tObjNum,1);
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Siege Skill - Checked Function
BOOL CObjUseSkill::SkillStun(int aIndex, int aTargetIndex, CMagicInf * lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int TargePosx = lpObj->X - (8 - (btTargetPos & 15));
	int TargePosy = lpObj->Y - (8 - ((btTargetPos & 240) >> 4));

	int iangle = this->GetAngle(lpObj->X,lpObj->Y,TargePosx,TargePosy);

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			if(lpObj->VpPlayer2[count].type == OBJ_USER || lpObj->VpPlayer2[count].type == OBJ_MONSTER)
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if(OBJMAX_RANGE(tObjNum))
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= 4)
					{
						if(SkillElectricSparkHitBox.HitCheck(iangle,lpObj->X,lpObj->Y,gObj[tObjNum].X,gObj[tObjNum].Y))
						{
							gObj[tObjNum].m_iSkillStunTime = 10;
							GCStateInfoSend(&gObj[tObjNum],1,eVS_STUN);
							GCMagicAttackNumberSend(lpObj,AT_SKILL_STUN,tObjNum,1);
							gObjSetPosition(tObjNum,gObj[tObjNum].X,gObj[tObjNum].Y);
						}
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Siege Skill - Checked Function
BOOL CObjUseSkill::SkillRemoveStun(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{

	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_STUN,lpObj->m_Index,1);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{

			tObjNum = lpObj->VpPlayer2[count].number;
			if(OBJMAX_RANGE(tObjNum))
			{
				if(lpObj->VpPlayer2[count].type == OBJ_USER)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 6)
					{
						gObj[tObjNum].m_iSkillStunTime = 0;
						GCStateInfoSend(&gObj[tObjNum],0,eVS_STUN);
						GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_STUN,tObjNum,1);
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Siege Skill - Checked Function
BOOL CObjUseSkill::SkillAddMana(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;
	gObjSetKillCount(aIndex,0);

	lpObj->AddMana -= lpObj->m_iSkillManaSwell;

	int iSwelledMana = short(lpObj->MaxMana + lpObj->AddMana) * 50 / 100;
	lpObj->AddMana += iSwelledMana;
	lpObj->m_iSkillManaSwellTime = 60;
	lpObj->m_iSkillManaSwell = iSwelledMana;

	GCManaSend(lpObj->m_Index,lpObj->MaxMana + lpObj->AddMana,0xFE,0,lpObj->MaxBP + lpObj->AddBP);

	if( (lpObj->MaxMana + lpObj->AddMana) < lpObj->Mana)
	{
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	}

	GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_MANA,lpObj->m_Index,1);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum) && lpObj->VpPlayer2[count].type == OBJ_USER)
			{
				if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
				{
					if(lpObj->GuildNumber > 0 && gObj[tObjNum].GuildNumber > 0)
					{
						if(lpObj->GuildNumber != gObj[tObjNum].GuildNumber)
						{
							count++;
							continue;
						}

						gObj[tObjNum].AddMana -= gObj[tObjNum].m_iSkillManaSwell;
						iSwelledMana = short(gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana) * 50 / 100;
						gObj[tObjNum].AddMana += iSwelledMana;

						gObj[tObjNum].m_iSkillManaSwellTime = 60;
						gObj[tObjNum].m_iSkillManaSwell = iSwelledMana;


						GCManaSend(tObjNum,gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana,0xFE,0,gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP);

						if(gObj[tObjNum].Mana > gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana)
						{
							gObj[tObjNum].Mana = gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana;
						}

						GCManaSend(tObjNum,gObj[tObjNum].Mana,0xFF,0,gObj[tObjNum].BP);

						GCStateInfoSend(&gObj[tObjNum],1,eVS_SWELL_MANA);
						GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_MANA,tObjNum,1);
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Siege Skill - Checked Function
BOOL CObjUseSkill::SkillCloaking(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	if(lpTargetObj->Type != OBJ_USER)
	{
		CLog.LogAdd("[ANTI-HACK][Cloaking Skill] [%s][%s] Invalid Targeting  TargetType [%d] TargetClass [%d]", lpObj->AccountID,lpObj->Name,lpTargetObj->Type,lpTargetObj->Class);
		return false;
	}

	gObjSetKillCount(aIndex,0);

	GCMagicAttackNumberSend(lpObj,AT_SKILL_INVISIBLE,lpObj->m_Index,1);
	lpTargetObj->m_iSkillInvisibleTime = 60;
	GCStateInfoSend(lpTargetObj,1,eVS_TRANSPARENCY);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_INVISIBLE,aTargetIndex,1);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Siege Skill - Checked Function
BOOL CObjUseSkill::SkillRemoveCloaking(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_INVISIBLE,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum) && lpObj->VpPlayer2[count].type == OBJ_USER)
			{
				if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
				{
					gObj[tObjNum].m_iSkillInvisibleTime = 0;
					GCStateInfoSend(&gObj[tObjNum],0,eVS_TRANSPARENCY);
					GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_INVISIBLE,tObjNum,1);
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Siege Skill - Checked Function
BOOL CObjUseSkill::SkillRemoveMagic(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_MAGIC,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum) && lpObj->VpPlayer2[count].type == OBJ_USER)
			{
				if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
				{
					this->RemoveAllCharacterInvalidMagicAndSkillState(&gObj[tObjNum]);
					GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_MAGIC,tObjNum,1);
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Fire Scream Skill - Checked Function
BOOL CObjUseSkill::SkillFireScream(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if( g_SkillAdditionInfo.GetFireScreamSkill() == false )
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if( gObjIsConnected(lpObj->m_Index) == false && gObjIsConnected(lpTargetObj->m_Index) == false )
	{
		return false;
	}

	if( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	gObjAttack( lpObj,lpTargetObj,lpMagic,0,0,0,0 );
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Fire Scream Skill - Checked Function
BOOL CObjUseSkill::FireScreamExplosionAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{

	if(gObjIsConnected(lpObj->m_Index) == false && gObjIsConnected(lpTargetObj->m_Index) == false)
	{
		return false;
	}

	if(rand()%10000 > g_SkillAdditionInfo.GetFireScreamExplosionRate())
	{
		return false;
	}

	int iExplosionDamage = iAttackDamage / 10;
	int iTarObjNum;

	if(iExplosionDamage < 1)
	{
		return true;
	}

	int iCount = 0;
	int bEnableAttack;

	while( true )
	{
		if(lpObj->VpPlayer2[iCount].state != 0)
		{
			iTarObjNum = lpObj->VpPlayer2[iCount].number;

			if(iTarObjNum >= 0)
			{
				bEnableAttack = 0;

				if(gObj[iTarObjNum].Type == OBJ_MONSTER && gObj[iTarObjNum].m_RecallMon < 0)
				{
					bEnableAttack = 1;
				}
				else if(lpTargetObj->Type == OBJ_USER && lpObj->Type == OBJ_USER && gObj[iTarObjNum].Type != OBJ_NPC)
				{
					bEnableAttack = 1;
				}
				else if(lpTargetObj->m_cChaosCastleIndex == gObj[iTarObjNum].m_cChaosCastleIndex && lpTargetObj->m_cChaosCastleIndex != -1)
				{
					bEnableAttack = 1;
				}

				if(bEnableAttack != 0)
				{
					if(lpObj->m_Index != gObj[iTarObjNum].m_Index)
					{
						int iSkillDistance = gObjCalDistance(lpTargetObj,&gObj[iTarObjNum]);

						if(iSkillDistance <= g_SkillAdditionInfo.GetFireScreamExplosionAttackDistance())
						{
							gObjAddMsgSendDelay(lpObj,54,iTarObjNum,100,iExplosionDamage);
						}
					}
				}
			}
		}

		iCount++;

		if(iCount > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
BOOL CObjUseSkill::RemoveCloakingEffect(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	gObj[aIndex].m_iSkillInvisibleTime = 0;
	GCStateInfoSend(&gObj[aIndex],0,eVS_TRANSPARENCY);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Fire Scream - Checked Function
void CObjUseSkill::SpecificSkillAdditionTreat(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, int iAttackDamage)
{
	WORD bySkillNum = lpMagic->m_Skill;

	if(bySkillNum == AT_SKILL_FIRESCREAM)
	{
		gObjUseSkill.FireScreamExplosionAttack(lpObj,lpTargetObj,iAttackDamage);
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Summoned Monster Skill Remove - Checked Function
int CObjUseSkill::RemoveSpecificSkillState(LPOBJ lpObj, int iSkillNumber)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

    if(iSkillNumber == AT_SKILL_ATTACK)
	{
		lpObj->m_SkillAttackTime = 0;
		lpObj->m_SkillAttack = 0;
		GCStateInfoSend(lpObj,0,eVS_ATTACK_AURA);
	}
	else if(iSkillNumber == AT_SKILL_DEFENSE)
	{
		lpObj->m_SkillDefenseTime = 0;
		lpObj->m_SkillDefense = 0;
		GCStateInfoSend(lpObj,0,eVS_DEFENSE_AURA);
	}
	
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Fenrir Skill - Checked Function
int CObjUseSkill::SkillFenrirAttack(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	if(lpObj->Level < 300)
	{
		return false;
	}

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;


						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == 1)
						{
							EnableAttack = 1;
						}
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 6)
					{
						bAttack = 0;

						if(HitCount >= 5)
						{
							bAttack = 0;
						}
						else
						{
							bAttack = 1;
						}

						if(tObjNum == aTargetIndex)
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,53,tObjNum,300,lpMagic->m_Skill,0);
							HitCount++;
						}
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Infinity Arrow Skill - Checked Function
int CObjUseSkill::SkillInfinityArrow(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if( lpObj->ChangeUP == 0)
	{
		return false;
	}

	if(lpObj->m_Index != aTargetIndex)
	{
		return false;
	}

	if(lpObj->m_iMuseElfInfinityArrowSkillTime > 0)
	{
		return false;
	}

	lpObj->m_iMuseElfInfinityArrowSkillTime = g_SkillAdditionInfo.GetInfinityArrowSkillTime();
	GCStateInfoSend(lpObj,1,eVS_INFINITY_ARROW);
	GCMagicAttackNumberSend(lpObj,0x4D,lpObj->m_Index,1);
	CLog.LogAdd("[%s][%s] Use Infinity Arrow Skill (Time:%d)(Character Level : %d)(ChangeUp: %d)", lpObj->AccountID,lpObj->Name,lpObj->m_iMuseElfInfinityArrowSkillTime,lpObj->Level,lpObj->ChangeUP);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Elf Infinity Arrow Skill - Checked Function
int CObjUseSkill::GetAddUseMana(LPOBJ lpObj,CMagicInf * lpMagic)
{
	int iRetValue = 0;

	if( lpObj->Type == OBJ_USER && lpObj->ChangeUP == 1 && lpObj->m_iMuseElfInfinityArrowSkillTime > 0)
	{
		if(lpMagic->m_Skill == AT_SKILL_CROSSBOW || lpMagic->m_Skill == AT_SKILL_BOW || lpMagic->m_Skill == AT_SKILL_ELFHARDEN || lpMagic->m_Skill == AT_SKILL_PENETRATION || lpMagic->m_Skill == AT_SKILL_5CROSSBOW)
		{
			int iArrowLevel = 0;

			CItem * Right = &lpObj->pInventory[0];
			CItem * Left = &lpObj->pInventory[1];

			if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type <= ITEMGET(4,14) || Right->m_Type == ITEMGET(4,16) || Right->m_Type == ITEMGET(4,18) || Right->m_Type == ITEMGET(4,19))
			{
				if(Left->m_Type == ITEMGET(4,7))
				{
					iArrowLevel = Left->m_Level;
				}
			}
			else if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type <= ITEMGET(4,6) || Left->m_Type == ITEMGET(4,17) || ( Left->m_Type >= ITEMGET(4,20) && Left->m_Type <= ITEMGET(4,24) ))
			{
				if(Right->m_Type == ITEMGET(4,15))
				{
					iArrowLevel = Right->m_Level;
				}
			}

			if(iArrowLevel == 0)
			{
				iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus0();
			}
			else if(iArrowLevel == 1)
			{
				iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus1();
			}
			else if(iArrowLevel == 2)
			{
				iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus2();
			}
		}
	}
	return iRetValue;
}
// -----------------------------------------------------------------------------------------------------------------------
int CObjUseSkill::SkillDrainLife(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int drainLife = (((lpObj->Energy * 10)/100) * lpObj->MaxLife)/100;
	

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,1,1,0,0);

	if((lpObj->Life + drainLife) > lpObj->MaxLife)
	{
		lpObj->Life = lpObj->MaxLife;
	}
	else
	{
		lpObj->Life += drainLife;
	}

	return true;

}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillSleep(int aIndex,CMagicInf * lpMagic, int aTargetIndex)
{
    int skillSuccess = 1;
	LPOBJ lpObj = &gObj[aIndex];
    LPOBJ lpTargetObj = &gObj[aTargetIndex];
	int skill_level;

	if (lpMagic->m_Skill == 219 )
	{
		skill_level = 0;
	}
	else
	{
		skill_level = lpMagic->m_Level;
	}

	if(gUser.ChaosCastleMapRange(lpObj->MapNumber) == true)
	{
		return;
	}

	int sleeptime = ((lpObj->AddEnergy+lpObj->Energy)/10+2)*(1000+skill_level);

	if(sleeptime > 15*1000)
	{
		sleeptime = 15*1000;
	}
	
	lpTargetObj->NextActionTime = sleeptime;
	lpTargetObj->m_SkillSleepTime = sleeptime / 1000;
	GCStateInfoSend(lpTargetObj,1, eVS_SKILL_SLEEP );
	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpTargetObj->m_Index,skillSuccess);
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillReflex(int aIndex,int skill_level)
{
    int skillSuccess = 1;
	LPOBJ lpObj = &gObj[aIndex];
	int number;

	if(lpObj->Type != 1 && lpObj->m_RecallMon == -1)
	{
		return;
	}

	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;
	
	int ApplyPartyIndex[5];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));
	
	partynum = lpObj->PartyNumber;
	
	if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >=0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);
					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}
	
	int reflectvalue = ((lpObj->Energy + lpObj->AddEnergy)/30+20);
	int skillTime = ((lpObj->Energy + lpObj->AddEnergy)/25+10);

	if(reflectvalue > 100)
	{
		reflectvalue = 100;
	}

	if(partynum == -1)
	{
		if ( lpObj->m_SkillReflexTime <= 0 )
		{
			lpObj->DamageReflect += reflectvalue;
			lpObj->m_SkillReflex = reflectvalue;
		}
		lpObj->m_SkillReflexTime = skillTime;
		GCStateInfoSend(lpObj,1,eVS_SKILL_DAMAGEREFLECTION);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_REFLEX,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n=0;n<5;n++)
		{
			if(ApplyPartyIndex[n] != -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];
				if ( lpPartyObj->m_SkillReflexTime <= 0 )
				{
					lpPartyObj->DamageReflect += reflectvalue;
					lpPartyObj->m_SkillReflex = reflectvalue;
				}
				lpPartyObj->m_SkillReflexTime = skillTime;
				GCStateInfoSend(lpPartyObj,1,eVS_SKILL_DAMAGEREFLECTION);
				GCMagicAttackNumberSend(lpObj,0xD9,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
BOOL CObjUseSkill::SkillGhostPhantomAttack(int aIndex,int aTargetIndex,CMagicInf* lpMagic)
{
    LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int delay = rand()%300;

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
					if(lpObj->VpPlayer2[count].type == 2 && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;


						if(gObj[tObjNum].Type == 2 && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == 1)
						{
							EnableAttack = 1;
						}
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 7)
					{
						bAttack = 0;

						if(HitCount >= 11)
						{
							bAttack = 0;
						}
						else
						{
							bAttack = 1;
						}

						if(tObjNum == aTargetIndex)
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delay,lpMagic->m_Skill,0);
							HitCount++;
						}
					}
				}
			}
		}

		count++;

		if(count > 74)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillRedStorm(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
    LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int delay = rand()%300;

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
					if(lpObj->VpPlayer2[count].type == 2 && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;


						if(gObj[tObjNum].Type == 2 && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == 1)
						{
							EnableAttack = 1;
						}
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 7)
					{
						bAttack = 0;

						if(HitCount >= 11)
						{
							bAttack = 0;
						}
						else
						{
							bAttack = 1;
						}

						if(tObjNum == aTargetIndex)
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delay,lpMagic->m_Skill,0);
							HitCount++;
						}
					}
				}
			}
		}

		count++;

		if(count > 74)
		{
			break;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SkillBookNumber12(int aIndex,int aTargetIndex,CMagicInf* lpMagic)
{
    LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	
	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
					if(lpObj->VpPlayer2[count].type == 2 && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;


						if(gObj[tObjNum].Type == 2 && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}

						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == 1)
						{
							EnableAttack = 1;
						}
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 7)
					{
						bAttack = 0;

						if(HitCount >= 11)
						{
							bAttack = 0;
						}
						else
						{
							bAttack = 1;
						}

						if(tObjNum == aTargetIndex)
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,300,lpMagic->m_Skill,0);
							HitCount++;
						}
					}
				}
			}
		}

		count++;

		if(count > 74)
		{
			break;
		}
	}
	return true;

}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SkillLightingStorm(int aIndex,int bIndex,CMagicInf* lpMagic)
{
    LPOBJ lpObj = &gObj[aIndex];

    for(int n=0;n < MAX_VIEWPORT;n++)
	{
        if(lpObj->VpPlayer2[n].state <= 0)
		{
		    continue;
		}

        int index = lpObj->VpPlayer2[n].index;

		if(this->CheckViewportTarget(lpObj,index,bIndex,lpObj->VpPlayer2[n].type) == 0)
		{
		    continue;
		}

		if(gObjCalDistance(lpObj,&gObj[index]) < 4)
		{
            gObjAddAttackProcMsgSendDelay(lpObj,50,index,500,lpMagic->m_Skill,0);
		}
	}

	return 1;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SkillSwordSlash(int aIndex, int bIndex, CMagicInf* lpMagic, BYTE angle)
{
	LPOBJ lpObj = &gObj[aIndex];

	int nangle = (angle*360)/255;

    for(int n=0;n < MAX_VIEWPORT;n++)
	{
        if(lpObj->VpPlayer2[n].state <= 0)
		{
		    continue;
		}

        int index = lpObj->VpPlayer2[n].index;

		if(this->CheckViewportTarget(lpObj,index,bIndex,lpObj->VpPlayer2[n].type) == 0)
		{
		    continue;
		}

		if(gObjCalDistance(lpObj,&gObj[index]) >= 4)
		{
		    continue;
		}

        if(this->CheckAngle(lpObj,nangle,150,gObj[index].X,gObj[index].Y,lpMagic->m_Skill) != 0)
		{
		    gObjAttack(lpObj,&gObj[index],lpMagic,0,1,0,0);
		}
	}

	return 1;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SkillChainLightning(int aIndex,int bIndex,CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

    if(OBJMAX_RANGE(bIndex) == 0)
	{
	    return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,bIndex,1);

	LPOBJ lpTarget = &gObj[bIndex];

    gObjAttack(lpObj,lpTarget,lpMagic,1,1,0,0);

	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SkillElectricSurge(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

    if( OBJMAX_RANGE(aTargetIndex) == 0 )
	{
	    return false;
	}

	LPOBJ lpTarget = &gObj[aTargetIndex];

    gObjAttack(lpObj,lpTarget,lpMagic,1,1,0,0);

	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillWeakness(int aIndex,int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
	
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if( DuelIndex == tObjNum )
					{
						EnableAttack = 1;
					}
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;
	
						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
	
						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}
	
					if(EnableAttack != 0)
					{
						int Distance;
						int DamageReduce;
						// ----
						Distance = MagicDamageC.GetSkillDistance(AT_SKILL_WEAKNESS);
						DamageReduce = MagicDamageC.SkillGet(AT_SKILL_WEAKNESS);
						// ----
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= Distance)
						{
							bAttack = 0;
	
							if(HitCount > 15)
							{
								bAttack = 0;
							}
	
							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}
	
							if(bAttack != 0)
							{
								gObjUseSkill.ApplyWeaknessSkill(&gObj[tObjNum]);
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
return;

	return;
}


// -----------------------------------------------------------------------------------------------------------------------
// - UNDER DEV..!
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillInnovation(int aIndex,int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];

	CLog.Debug("[CObjUseSkill::SkillInnovation] [%s/%s] Skill Not Finished",lpObj->AccountID,lpObj->Name);
}
void CObjUseSkill::SkillBerserker(int aIndex, int aTargetIndex, int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	CLog.Debug("[CObjUseSkill::SkillBerserker] [%s/%s] Skill Not Finished",lpObj->AccountID,lpObj->Name);
	return;
}
void CObjUseSkill::SkillNight(int aIndex,int aTargetIndex,CMagicInf* lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	CLog.Debug("[CObjUseSkill::SkillNight] [%s/%s] Skill Not Finished",lpObj->AccountID,lpObj->Name);
	return;
}
// -----------------------------------------------------------------------------------------------------------------------



// Dark Knight Explotion Skill - Checked Function
bool CObjUseSkill::SkillExplotion(int aIndex,int aTargetIndex,CMagicInf *lpMagic,BYTE x,BYTE y,BOOL isCombo) //GOOD 232
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int HitCount = 0;
	int bAttack = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 4)
					{
						bAttack = 0;

						if(HitCount > 15)
						{
							bAttack = 0;
						}

						if(HitCount >= 10)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,isCombo);

							if(isCombo != 0)
							{
								GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,aIndex,1);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
// Dark Lord Birds Skill - Checked Function
bool CObjUseSkill::SkillBirdsAttack(int aIndex,int bIndex,CMagicInf* lpMagic,BYTE angle) //GOOD 238
{
	LPOBJ lpObj = &gObj[aIndex];

	int nangle = (angle*360)/255;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
        if(lpObj->VpPlayer2[n].state <= 0)
		{
		    continue;
		}

        int index = lpObj->VpPlayer2[n].index;

		if(this->CheckViewportTarget(lpObj,index,bIndex,lpObj->VpPlayer2[n].type) == 0)
		{
		    continue;
		}
		GCServerMsgStringSend("Birds Attack 1",aIndex,1);
		//if(SkillBirdsHitBox.HitCheck(nangle,lpObj->X,lpObj->Y,gObj[index].X,gObj[index].Y) != 0)
		//{
			GCServerMsgStringSend("Birds Attack 2",aIndex,1);
		    //gObjAddAttackProcMsgSendDelay(lpObj,50,index,200,lpMagic->m_Skill,0);
			gObjAttack(lpObj,&gObj[index],lpMagic,1,0,0,0);
		//}
	}

    return 1;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillDragonLore(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	//GCMagicAttackNumberSend(lpObj,AT_SKILL_DRAGON_LORE,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
	
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;
	
						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
	
						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}
	
					if(EnableAttack != 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 6)
						{
							bAttack = 0;
	
							if(HitCount > 15)
							{
								bAttack = 0;
							}
	
							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}
	
							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillDarkSide(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
	
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if( tObjNum == aTargetIndex || DuelIndex == tObjNum )
					{
						EnableAttack = 1;
					}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;
	
						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
	
						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}
	
					if(EnableAttack != 0)
					{
						int Distance;
						// ----
						if ( lpMagic->m_Skill < 300 )
						{
							Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
						}
						else
						{
							Distance = gSkillTree.GetMasterSkillDistance(lpMagic->m_Skill);
						}
						// ----
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= Distance)
						{
							bAttack = 0;
	
							if(HitCount > 15)
							{
								bAttack = 0;
							}
	
							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}
	
							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillLargeRingBlower(int aIndex,int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];


	gObjAttack(lpObj,lpTargetObj,lpMagic,1,1,0,0);
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillUpperBeast(int aIndex,int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	gObjAttack(lpObj,lpTargetObj,lpMagic,1,1,0,0);
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillChainDriver(int aIndex,int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	gObjAttack(lpObj,lpTargetObj,lpMagic,1,1,0,0);
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillDefenseSuccessRate(int aIndex, CMagicInf * lpMagic)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int iDefense;
	int iDefenseTime = 60 + (lpObj->Energy + lpObj->AddEnergy) / 10;

	if(partynum == -1)
	{
		if (lpObj->m_RageFighterSkillDefenseRate == 0)
		{
			iDefense = (lpObj->m_Defense*12)/100;
			lpObj->m_Defense += (lpObj->m_Defense*12)/100;
			lpObj->m_RageFighterSkillDefenseRate = iDefense;
		}
		lpObj->m_RageFighterSkillDefenseRateTime = iDefenseTime;
	
		GCStateInfoSend(lpObj,1,eVS_DEFENSE_SUCCESS_RATE);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_DEFENSE_RATE_INCREASE,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			if(ApplyPartyIndex[n]!= -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				if (lpPartyObj->m_RageFighterSkillDefenseRate == 0)
				{
					iDefense = (lpPartyObj->m_Defense*12)/100;
					lpPartyObj->m_Defense+= (lpPartyObj->m_Defense*12)/100;
					lpPartyObj->m_RageFighterSkillDefenseRate = iDefense;
				}
				lpPartyObj->m_RageFighterSkillDefenseRateTime = iDefenseTime;
				GCStateInfoSend(lpPartyObj,1,eVS_DEFENSE_SUCCESS_RATE);
				GCMagicAttackNumberSend(lpObj,AT_SKILL_DEFENSE_RATE_INCREASE,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillFitness(int aIndex, CMagicInf * lpMagic)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];

	if( lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1 )
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	int viewplayer = 0;
	int viewpercent = 0;

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}

	if(viewplayer > 1)
	{
		if(viewplayer == 2)
		{
			viewpercent = 1;
		}
		else if(viewplayer == 3)
		{
			viewpercent = 2;
		}
		else if(viewplayer == 4)
		{
			viewpercent = 3;
		}
		else
		{
			viewpercent = 4;
		}
	}

	int iFitnessPower = 12 + (lpObj->Vitality + lpObj->AddVitality) / 100 + (lpObj->Energy + lpObj->AddEnergy)/20 + viewpercent;
	int iLifeTime = 60 + (lpObj->Energy + lpObj->AddEnergy) / 10;
	int iaddlife;

	if(partynum == -1)
	{
		iaddlife = int(lpObj->MaxLife) * iFitnessPower / 100;
		lpObj->AddLife -= lpObj->m_RageFighterSkillFitnessAddLife;
		lpObj->AddLife += iaddlife;
		lpObj->m_RageFighterSkillFitnessAddLife = iaddlife;

		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
		lpObj->m_RageFighterSkillFitnessTime = iLifeTime;

		GCStateInfoSend(lpObj,1,eVS_FITNESS);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_FITNESS,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			if(ApplyPartyIndex[n]!= -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				iaddlife = int(lpPartyObj->MaxLife) * iFitnessPower / 100;
				lpPartyObj->AddLife -= lpPartyObj->m_RageFighterSkillFitnessAddLife;
				lpPartyObj->AddLife += iaddlife;
				lpPartyObj->m_RageFighterSkillFitnessAddLife = iaddlife;
				
				GCReFillSend(lpPartyObj->m_Index,lpPartyObj->MaxLife + lpPartyObj->AddLife,0xFE,0,lpPartyObj->iMaxShield + lpPartyObj->iAddShield);
				lpPartyObj->m_RageFighterSkillFitnessTime = iLifeTime;

				GCStateInfoSend(lpPartyObj,1,eVS_FITNESS);
				GCMagicAttackNumberSend(lpObj,AT_SKILL_FITNESS,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillRedArmorIgnore(int aIndex, CMagicInf * lpMagic)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	int number;
	int partynum = 0;
	int totallevel = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyInfo[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyInfo[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis <= MagicDamageC.GetSkillDistance(lpMagic->m_Skill) )
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int iRedArmorIgnoreTime = 60 + (lpObj->Energy + lpObj->AddEnergy) / 10;
	
	if(partynum == -1)
	{
		lpObj->SetOpIgnoreDefense -= lpObj->m_RageFighterSkillRedArmorIgnore;
		lpObj->m_RageFighterSkillRedArmorIgnore = 5;
		lpObj->SetOpIgnoreDefense += lpObj->m_RageFighterSkillRedArmorIgnore;

		lpObj->m_RageFighterSkillRedArmorIgnoreTime = iRedArmorIgnoreTime;
	
		GCStateInfoSend(lpObj,1,eVS_IGNORE_DEFENSE);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_RED_ARMOR_IGNORE,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			if(ApplyPartyIndex[n]!= -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				lpPartyObj->SetOpIgnoreDefense -= lpPartyObj->m_RageFighterSkillRedArmorIgnore;
				lpPartyObj->m_RageFighterSkillRedArmorIgnore = 5;
				lpPartyObj->SetOpIgnoreDefense += lpPartyObj->m_RageFighterSkillRedArmorIgnore;

				lpPartyObj->m_RageFighterSkillRedArmorIgnoreTime = iRedArmorIgnoreTime;

				GCStateInfoSend(lpPartyObj,1,eVS_IGNORE_DEFENSE);
				GCMagicAttackNumberSend(lpObj,AT_SKILL_RED_ARMOR_IGNORE,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
bool CObjUseSkill::SkillCharge(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	gObjAttack(lpObj,lpTargetObj,lpMagic,1,0,0,0);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillEvilSpirit(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
	
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if( tObjNum == aTargetIndex || DuelIndex == tObjNum )
					{
						EnableAttack = 1;
					}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;
	
						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
	
						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}
	
					if(EnableAttack != 0)
					{
						int Distance;
						// ----
						if ( lpMagic->m_Skill < 300 )
						{
							Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
						}
						else
						{
							Distance = gSkillTree.GetMasterSkillDistance(lpMagic->m_Skill);
						}
						// ----
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= Distance)
						{
							bAttack = 0;
	
							if(HitCount > 15)
							{
								bAttack = 0;
							}
	
							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}
	
							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillInferno(int aIndex, CMagicInf* lpMagic, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					int Distance;
					// ----
					if ( lpMagic->m_Skill < 300 )
					{
						Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
					}
					else
					{
						Distance = gSkillTree.GetMasterSkillDistance(lpMagic->m_Skill);
					}
					// ----
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= Distance)
					{
						bAttack = 0;

						if(HitCount > 15)
						{
							bAttack = 0;
						}

						if(HitCount >= 10)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillHellFire(int aIndex,CMagicInf* lpMagic, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				if(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)
				{
	
					if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = 1;
					}
					else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
					{
						EnableAttack = 1;
					}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
					else
					{
						int CallMonIndex = gObj[tObjNum].m_Index;
	
						if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
	
						if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
						{
							EnableAttack = 1;
						}
					}
	
					if(EnableAttack != 0)
					{
						int Distance;
						// ----
						if ( lpMagic->m_Skill < 300 )
						{
							Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
						}
						else
						{
						Distance = gSkillTree.GetMasterSkillDistance(lpMagic->m_Skill);
						}
						// ----
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= Distance)
						{
							bAttack = 0;
	
							if(HitCount > 15)
							{
								bAttack = 0;
							}
	
							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}
	
							if(bAttack != 0)
							{
								//gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,300,lpMagic->m_Skill,0);
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillFlame(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BYTE x, BYTE y)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int FirstHit = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#if (GS_CASTLE)
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == 7)
					{
						EnableAttack = 1;
					}
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					int Distance;
					// ----
					if ( lpMagic->m_Skill < 300 )
					{
						Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
					}
					else
					{
						Distance = gSkillTree.GetMasterSkillDistance(lpMagic->m_Skill);
					}
					// ----
					if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) <= Distance)
					{
						bAttack = 0;

						if(HitCount > 10)
						{
							bAttack = 0;
						}
						
						if(HitCount >= 5)
						{
							if(rand()%2)
							{
								bAttack = 1;
							}
						}
						else
						{
							bAttack = 1;
						}

						if(bAttack != 0)
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CObjUseSkill::SkillMagicCircle(int aIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if ( lpObj->SkillMagicCircleTime <= 0 )
	{
		lpObj->SkillMagicCircle = (lpObj->m_MagicDamageMin*20)/100;
		lpObj->m_MagicDamageMin += lpObj->SkillMagicCircle;
		GCStateInfoSend(lpObj,1,eVS_SKILL_MAGICCIRCLE);
		lpObj->SkillMagicCircleTime = 60 + (lpObj->Energy + lpObj->AddEnergy) / 40;
	}
	else
	{
		lpObj->m_MagicDamageMin -= lpObj->SkillMagicCircle;
		lpObj->SkillMagicCircle = 0;
		lpObj->SkillMagicCircle = (lpObj->m_MagicDamageMin*20)/100;
		lpObj->m_MagicDamageMin += lpObj->SkillMagicCircle;
		lpObj->SkillMagicCircleTime = 60 + (lpObj->Energy + lpObj->AddEnergy) / 40;
		GCStateInfoSend(lpObj,1,eVS_SKILL_MAGICCIRCLE);
	}
	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpObj->m_Index,1);
	return;
}

void CObjUseSkill::SkillRecovery(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if( lpTargetObj->Type != OBJ_USER )
	{
		return;
	}
	if( lpTargetObj->m_RecallMon == -1 )
	{
		return;
	}
	if( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true && lpObj->m_Index != lpTargetObj->m_Index )
	{
		return;
	}

	lpTargetObj->iShield = ((lpObj->Energy + lpObj->AddEnergy) / 5) + lpTargetObj->iShield;

	if((lpTargetObj->iMaxShield + lpTargetObj->iAddShield) < lpTargetObj->iShield)
	{
		lpTargetObj->iShield = lpTargetObj->iMaxShield + lpTargetObj->iAddShield;
	}

	if(lpTargetObj->Type == OBJ_USER)
	{
		GCReFillSend(lpTargetObj->m_Index,lpTargetObj->Life,0xFF,0,lpTargetObj->iShield);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_REDUCE_DAMAGE_SHIELD,lpTargetObj->m_Index,skillSuccess);

		for(int n = 0; n < MAX_VIEWPORT; n++)
		{
			if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
			{
				LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
				if(lpMonster->m_iCurrentAI != 0)
				{
					lpMonster->m_Agro.IncAgro(lpObj->m_Index,(lpObj->Energy / 5)/40);
				}
			}
		}
	}
	else if(lpTargetObj->m_RecallMon >= 0)
	{
		GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
	}

}

void CObjUseSkill::ApplyWeaknessSkill(LPOBJ lpObj)
{

	lpObj->m_AttackDamageLeft += lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageRight += lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMin += lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMax += lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMinLeft += lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMaxLeft += lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMinRight += lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMaxRight += lpObj->m_SkillWeakness;

	

	lpObj->m_AttackDamageLeft -= lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageRight -= lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMin -= lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMax -= lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMinLeft -= lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMaxLeft -= lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMinRight -= lpObj->m_SkillWeakness;
	lpObj->m_AttackDamageMaxRight -= lpObj->m_SkillWeakness;

	return;
}

void CObjUseSkill::SkillNew(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	/*gObjAttack(&gObj[aIndex],&gObj[aTargetIndex],lpMagic,1,0,0,0);
	return;*/

	LPOBJ lpObj = &gObj[aIndex];
	int StartDis = 1;
	int tObjNum;
	int count = 0;
	int loopcount = 0;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);

	int attackcheck;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int delaytime;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0 && aTargetIndex != tObjNum)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					attackcheck = 0;

					if(loopcount == 0)
					{
						attackcheck = 1;
					}
					else if(rand()%3==0)
					{
						attackcheck = 1;
					}

					if(attackcheck != 0)
					{
						if(gObjCalDistance(&gObj[aTargetIndex],&gObj[tObjNum]) < 3)
						{
							delaytime = (rand()*17)%300 + 500;

							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return;
}