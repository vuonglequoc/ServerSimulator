//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Master Skill Tree System
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
#ifndef SKILLTREE_H__
#define SKILLTREE_H__
// -----------------------------------------------------------------------------------------------------------------------------------------
#include "user.h"
#include "MagicInf.h"
#include "prodef.h"
// -----------------------------------------------------------------------------------------------------------------------------------------
#define MAX_SKILL_IN_TREE	600
#define MSSys_Module		"[MasterSkillTree]"
#pragma pack(1)
// -----------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_SKILL_ADD_RECV
{
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char subcode;
	unsigned char SkillL;
	unsigned char SkillH;
	unsigned char PosL;
	unsigned char PosH;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_SKILL_ADD_SEND
{
	//PBMSG_HEAD2 h;		//0xc1 0xXX 0xf3 0x52
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char subcode;
	char Enabled;
	char _Unk1;
	unsigned char MasterPointL;
	unsigned char MasterPointH;
	short SkillPos;
	short Skill;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_MASTERLEVEL_SEND
{
	PBMSG_HEAD2 h;		//0xC1,0x20,0xF3,0x50
	short MasterLevel;
	BYTE MasterExp[8];
	BYTE MasterNextExp[8];
	short MasterPoints;
	short MaxLife;
	short MaxMana;
	short iMaxShield;
	short MaxBP;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_MASTER_LEVEL_UP_SEND
{
    PBMSG_HEAD2 h; //C1:F3:51
	WORD MasterLevel;
	WORD MinusPoint;
	WORD MasterPoint;
	WORD MaxMinusPoint;
	WORD MaxLife;
	WORD MaxMana;
	WORD MaxShield;
    WORD MaxBP;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
#pragma pack()
// -----------------------------------------------------------------------------------------------------------------------------------------
class CSkillTree  
{
public:
	CSkillTree(void);
	~CSkillTree(void);

	void LoadSkillTreeSkills();
	void MasterSkillUse1 (int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	void MasterSKillUse2 (int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL bCombo);
	void SendMasterData(int iIndex);
	void SendMasterSkills(int aIndex);
	bool IsMasterMagic(int SkillNumber);
	void SkillAddRecv(PMSG_SKILL_ADD_RECV * aRecv, int iIndex);
	void InitEffect(MASTER_SKILL_TREE_EFFECT* lpEffect);
	void ApplyEffect(LPOBJ lpObj);
	void CalcEffect(int type,int bonus,MASTER_SKILL_TREE_EFFECT* lpEffect);
	void GCMasterLevelUpSend(LPOBJ lpObj,bool effect);
	bool IsMasterMagicExist(LPOBJ lpObj,int SkillNumber);
	bool CanAddSkill(LPOBJ lpObj,int MagicNumber);
	int MasterMagicAdd(LPOBJ lpObj,int SkillNumber);
	int GetMasterSkillDistance(int SkillNumber);
	int GetMasterSkillMana(LPOBJ lpObj, int SkillNumber);
	int GetMasterSkillBP(LPOBJ lpObj, int SkillNumber);
	int GetMasterSkillAttr2(int SkillNumber);
	int GetMasterSkillDamage(int SkillNumber);
	bool MasterSkillClassCheck(LPOBJ lpObj,int SkillNumber);
	bool MasterSkillEnergyCheck(LPOBJ lpObj,int SkillNumber);
	int GetMasterSkillType(int SkillNumber);
	bool CheckMasterSkillCanAdd(LPOBJ lpObj, int SkillNumber);
private:
	int m_Damage[MAX_SKILL_IN_TREE];
	char m_Name[MAX_SKILL_IN_TREE][50];
	unsigned char m_rLevel[MAX_SKILL_IN_TREE];
	unsigned short m_Mana[MAX_SKILL_IN_TREE];
	unsigned char m_Distance[MAX_SKILL_IN_TREE];
	short m_RequireEnergy[MAX_SKILL_IN_TREE];
	unsigned char m_RequireClass[MAX_SKILL_IN_TREE][MAX_CLASS_TYPE];
	short m_RequireLevel[MAX_SKILL_IN_TREE];
	short m_RequireLeadership[MAX_SKILL_IN_TREE];
	short m_Delay[MAX_SKILL_IN_TREE];
	short m_BrainPower[MAX_SKILL_IN_TREE];
	char m_Attr1[MAX_SKILL_IN_TREE];
	char m_Attr2[MAX_SKILL_IN_TREE];
	char m_iSkillUseType[MAX_SKILL_IN_TREE];
	unsigned short m_iSkillBrand[MAX_SKILL_IN_TREE];
	char m_iKillCount[MAX_SKILL_IN_TREE];
	char m_iRequireStatus[MAX_SKILL_IN_TREE][MAX_REQ_SKILL_STATUS];
	char m_Rank[MAX_SKILL_IN_TREE];
	char m_Group[MAX_SKILL_IN_TREE];
	char m_MPoints[MAX_SKILL_IN_TREE];
	char m_Icon[MAX_SKILL_IN_TREE];
	char m_iSkillType[MAX_SKILL_IN_TREE];
};
// -----------------------------------------------------------------------------------------------------------------------------------------
extern CSkillTree gSkillTree;
// -----------------------------------------------------------------------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------