//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Character Info Class.
//----------------------------------------------------------------------------------
#ifndef CHARACTER_INFO_H
#define CHARACTER_INFO_H

#include "zzzitem.H"
#include "Defines.H"


enum CLASS_TYPE
{
	CLASS_WIZARD	= 0x0,
	CLASS_KNIGHT	= 0x1,
	CLASS_ELF		= 0x2,
	CLASS_MAGUMSA	= 0x3,
	CLASS_DARKLORD	= 0x4,
	CLASS_SUMMONER	= 0x5,
	CLASS_FIGHTER	= 0x6
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct DEFAULTCLASSTYPE
{
	WORD	Strength;
	WORD	Dexterity;
	WORD	Vitality;
	WORD	Energy;
	float	LevelLife;
	float	Life;
	float	MaxLife;
	float	LevelMana;
	float	Mana;
	float	MaxMana;
	float	VitalityToLife;
	float	EnergyToMana;
	CItem	Equipment[MAX_PLAYER_EQUIPMENT+2];
	WORD	Leadership;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class CCharacterInfo
{
public:
	CCharacterInfo();
	virtual ~CCharacterInfo();
	// -----
	void Initialize();
	void SetCharInfo(int ClassType, int Str, int Dex, int Vit, int Ene, int Led, float Life, float Mana, float LevelLife, float LevelMana, float VitToLife, float EneToMana);
	void SetEquipment(int Class);
	int  GetTotalPoint(int Class);
	// -----
	int CLASS_WIZARD_POINTS;
	int CLASS_KNIGHT_POINTS;
	int CLASS_ELF_POINTS;
	int CLASS_MAGUMSA_POINTS;
	int CLASS_DARKLORD_POINTS;
	int CLASS_SUMMONER_POINTS;
	int CLASS_FIGHTER_POINTS;
	// -----
	int CLASS_WIZARD_POINTS_PLUS;
	int CLASS_KNIGHT_POINTS_PLUS;
	int CLASS_ELF_POINTS_PLUS;
	int CLASS_SUMMONER_POINTS_PLUS;
	// -----
	int CLASS_WIZARD_MPOINTS;
	int CLASS_KNIGHT_MPOINTS;
	int CLASS_ELF_MPOINTS;
	int CLASS_MAGUMSA_MPOINTS;
	int CLASS_DARKLORD_MPOINTS;
	int CLASS_SUMMONER_MPOINTS;
	int CLASS_FIGHTER_MPOINTS;
	// -----
	bool WizardRingCreate;
	// -----
	DEFAULTCLASSTYPE sCharInfo[MAX_CLASS_TYPE];
};
extern CCharacterInfo gCharInfo;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* CHARACTER_INFO_H */
// -------------------------------------------------------------------------------------------------------------------------------------------------------
