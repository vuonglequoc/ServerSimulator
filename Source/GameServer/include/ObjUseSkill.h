//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Object Use Skills Structures							//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef OBJUSESKILL_H
#define OBJUSESKILL_H

#include "MagicInf.h"
#include "user.h"

struct PMSG_DURATION_MAGIC_SEND
{
	PBMSG_HEAD h;
	unsigned char MagicNumberH;
	unsigned char MagicNumberL;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char X;
	unsigned char Y;
	unsigned char Dir;
};


class CObjUseSkill
{
public:
	CObjUseSkill(void);
	~CObjUseSkill(void);
	bool CheckViewportTarget(LPOBJ lpObj,int aIndex,int bIndex,int type);
	bool CheckAngle(LPOBJ lpObj,int view,int angle,int x,int y,int skill);
	bool SpeedHackCheck(int aIndex);
	bool EnableSkill(WORD Skill);
	int GetUseMana(int aIndex, CMagicInf* lpMagic);
	int GetAddUseMana(LPOBJ lpObj, CMagicInf* lpMagic);
	int GetUseBP(int aIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void UseSkill2(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	bool RunningSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL bCombo);
	void SkillFrustrum(int aIndex, float fangle, float tx, float ty);
	int GetTargetLinePath(int sx, int sy, int tx, int ty,  int* PathX,int* PathY, int distance);
	int GetAngle(int x, int y, int tx,  int ty);
	int CalDistance(int x, int y, int x1, int y1);
	void SkillFrustrum2(int aIndex, float fangle, float tx, float ty);
	bool SkillAreaCheck(int angel, int x, int y,  int tx, int ty,  int dis,  int arc);
	void ApplyWeaknessSkill(LPOBJ lpObj);
	// ----
	void MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level);
	// ---- Buffs
	void KnightSkillAddLife(int aIndex, int skill_level);
	void SkillSoulBarrier(int aIndex, int aTargetIndex, int skill_level);
	void SkillDefense(int aIndex, int aTargetIndex, int skill_level);
	void SkillAttack(int aIndex, int aTargetIndex, int skill_level);
	void SkillHealing(int aIndex, int aTargetIndex, int skill_level);
	void SkillWeakness(int aIndex,int skill_level);
	void SkillInnovation(int aIndex,int skill_level);
	void SkillBerserker(int aIndex, int aTargetIndex, int skill_level);
	void SkillDefenseSuccessRate(int aIndex, CMagicInf * lpMagic);
	void SkillFitness(int aIndex,CMagicInf * lpMagic);
	void SkillRedArmorIgnore(int aIndex, CMagicInf * lpMagic);
	void SkillMagicCircle(int aIndex, CMagicInf * lpMagic);
	void SkillRecovery(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillReflex(int aIndex,int skill_level);
	// ----
	bool SkillChangeUse(int aIndex);
	bool SkillMonsterCall(int aIndex, int MonsterType, int x, int y);
	bool SkillDeathPoison(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	bool SkillIceStorm(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	bool SkillHellFire2Start(int aIndex, CMagicInf* lpMagic);
	bool SkillNova(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void SkillRagefulBlow(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo);
	void SkillTwistingSlash(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL isCombo);
	bool SkillDeathStab(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo, int skill_level);
	void SkillPowerSlash(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);
	bool SkillSpear(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	bool SkillFireBurst(int aIndex, int aTargetIndex, CMagicInf* lpMagic,int skill_level);
	bool SkillDarkHorseAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int RecallPartyCheck(int aIndex, int skill_level);
	void SkillRecallParty(int aIndex, int skill_level);
	void SkillAddCriticalDamage(int aIndex, int skill_level);
	void SkillElectricSpark(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSummon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillImmuneToMagic(int aIndex, CMagicInf* lpMagic);
	int SkillImmuneToHarm(int aIndex, CMagicInf* lpMagic);
	int SkillKnightRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillWizardJavalin(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillElfRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumOneFlash(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumDeathCannon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillDarklordSpaceSplit(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillBrandOfSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillAddMana(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveMagic(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int RemoveCloakingEffect(int aIndex);
	int SkillStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos);
	int RemoveAllCharacterInvalidMagicAndSkillState(LPOBJ lpObj);
	int RemoveSpecificSkillState(LPOBJ lpObj, int iSkillNumber);
	int SkillFenrirAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillInfinityArrow(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillFireScream(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int FireScreamExplosionAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
	void SpecificSkillAdditionTreat(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, int iAttackDamage);
	void SkillSleep(int aIndex, CMagicInf * lpMagic, int aTargetIndex);
	int  SkillGhostPhantomAttack(int aIndex,int bIndex,CMagicInf* lpMagic);
	void SkillRedStorm(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillNight(int aIndex,int aTargetIndex,CMagicInf* lpMagic);
	bool SkillBookNumber12(int aIndex,int bIndex,CMagicInf* lpMagic);
	bool SkillLightingStorm(int aIndex, int bIndex, CMagicInf* lpMagic);
	bool SkillSwordSlash(int aIndex, int bIndex, CMagicInf* lpMagic, BYTE angle);
	bool SkillChainLightning(int aIndex,int bIndex,CMagicInf* lpMagic);
	bool SkillExplotion(int aIndex,int bIndex,CMagicInf *lpMagic,BYTE x,BYTE y,BOOL combo);
	bool SkillBirdsAttack(int aIndex,int bIndex,CMagicInf* lpMagic,BYTE angle);
	int  SkillDrainLife(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	bool SkillElectricSurge(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void SkillEvilSpirit(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void SkillInferno(int aIndex, CMagicInf* lpMagic, int aTargetIndex);
	void SkillHellFire(int aIndex,CMagicInf* lpMagic, int aTargetIndex);
	void SkillFlame(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BYTE x, BYTE y);
	// ---- Season 6 Rage Fighter Skills
	void SkillDragonLore(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillDarkSide(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillLargeRingBlower(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void SkillUpperBeast(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void SkillChainDriver(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	bool SkillCharge(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void SkillNew(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	// ----
	bool CheckSkillDistance(LPOBJ lpObj,LPOBJ lpTargetObj,int Distance);
};

int DecPartyMemberHPandMP(LPOBJ lpObj);
extern CObjUseSkill gObjUseSkill;


#endif