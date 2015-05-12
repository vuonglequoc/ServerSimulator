//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Dark Spirit Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef DARKSPIRIT_H
#define DARKSPIRIT_H

#include "user.h"
#include "MagicInf.h"
#include "zzzitem.h"
#include "ObjBaseAttack.h"


#define RAVEN_ATTACK_DISTANCE		7


struct PMSG_SEND_PET_ITEMINFO
{
	PBMSG_HEAD h;
	BYTE PetType;
	BYTE InvenType;
	BYTE nPos;
	BYTE Level;
	int Exp;
};

class CDarkSpirit : public CObjBaseAttack
{
public:
	int m_AttackDamageMin;
	int m_AttackDamageMax;
	int m_AttackSpeed;
	int m_SuccessAttackRate;
	int m_iMasterIndex;
	int m_iTargetIndex;
	DWORD m_dwLastAttackTime;

	enum ePetItem_Mode
	{
		PetItem_Mode_Normal = 0,
		PetItem_Mode_Attack_Random = 1,
		PetItem_Mode_Attack_WithMaster = 2,
		PetItem_Mode_Attack_Target = 3,

	} m_ActionMode;
	CItem * m_pPetItem;

public:
  CDarkSpirit();
  virtual ~CDarkSpirit();
  void Init();
  void Run();
  void ModeNormal();
  void ModeAttackRandom();
  void ModeAttackWithMaster();
  void ModeAttakTarget();
  void SetTarget(int aTargetIndex);
  void ReSetTarget(int aTargetIndex);
  void Set(int aIndex, CItem* pPetItem);
  void SetMode(ePetItem_Mode mode, int iTargetindex);
  int Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, int criticaldamage, int iActionType);
  int GetAttackDamage(LPOBJ lpObj, int targetDefense, int bIsOnDuel,  int criticaldamage);
  int MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, int& bAllMiss);
  int MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, int& bAllMiss);
  void ChangeCommand(int command, int targetindex);
  void RangeAttack(int aIndex, int aTargetIndex);
  void SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage, int iActionType);
  int GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
  static void __cdecl CDarkSpirit::SendLevelmsg(int aIndex, int nPos, int PetType, int InvenType);
};
extern CDarkSpirit gDarkSpirit[OBJMAX];


#endif