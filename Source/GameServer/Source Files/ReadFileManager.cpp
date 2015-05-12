#include "../Header Files/stdafx.h"
#include "../Header Files/GameMain.H"
#include "../Header Files/LogProc.H"
#include "../Header Files/Functions.H"

CRFileManager	RFile;
// ------------------------------------------------------------------------------------------------------------------------------------------------------
CRFileManager :: CRFileManager()
{
	return;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
CRFileManager :: ~CRFileManager()
{
	return;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CRFileManager	::	RFSetInfo(int iCountryCode)
{
	switch ( iCountryCode )
	{
		case PROTO_KOREA:
		{
			szLanguage = "Kor";
		} break;
		// ----
		case PROTO_JAPAN:
		{
			szLanguage = "Jpn";
		} break;
		// ----
		case PROTO_ENGLISH:
		{
			szLanguage = "Eng";
		} break;
		// ----
		case PROTO_CHINA:
		{
			szLanguage = "Chs";
		} break;
		// ----
		default:
		{
			CLog.MsgBox("Protocol Selected is not Supported\nChange Protocol in Connect.ini");
		} break;
	}
	wsprintf( szRFileName[0],  ".\\Connect.ini" );
	wsprintf( szRFileName[1],  "..\\Data\\Local\\CheckSum.dat" );
	wsprintf( szRFileName[2],  "..\\Data\\Local\\Message.wtf" );
	wsprintf( szRFileName[3],  "..\\Data\\Local\\Items.txt" );
	wsprintf( szRFileName[4],  "..\\Data\\Local\\Skills.txt" );
	wsprintf( szRFileName[5],  "..\\Data\\Local\\MoveSystem.txt" );
	wsprintf( szRFileName[6],  "..\\Data\\Local\\Gate.txt" );
	wsprintf( szRFileName[7],  "..\\Data\\Local\\Quests.txt" );
	wsprintf( szRFileName[8],  "..\\Data\\Local\\ItemSetOption.txt" );
	wsprintf( szRFileName[9],  "..\\Data\\Local\\ItemSetType.txt" );
	wsprintf( szRFileName[10], "..\\Data\\Local\\SkillSpear.hit" );
	wsprintf( szRFileName[11], "..\\Data\\Local\\SkillElect.hit" );
	wsprintf( szRFileName[12], "..\\Data\\Local\\JewelOfHarmonyOpt.txt" );
	wsprintf( szRFileName[13], "..\\Data\\Local\\JewelOfHarmonySmelt.txt" );
	wsprintf( szRFileName[14], "..\\Data\\Local\\ItemAddOption.txt" );
	wsprintf( szRFileName[15], "..\\Data\\Local\\ConnectMember.txt" );
	wsprintf( szRFileName[16], "..\\Data\\Local\\MapServerInfo.dat" );
	wsprintf( szRFileName[17], "..\\Data\\Local\\Monster.txt" );
	wsprintf( szRFileName[18], "..\\Data\\Local\\MonsterSetBase.txt" );
	wsprintf( szRFileName[19], "..\\Data\\Local\\Iplist.dat" );
	wsprintf( szRFileName[20], "..\\Data\\Local\\Dec1.dat" );
	wsprintf( szRFileName[21], "..\\Data\\Local\\Enc2.dat" );
	wsprintf( szRFileName[22], "..\\Data\\Local\\MonsterSkillElement.txt" );
	wsprintf( szRFileName[23], "..\\Data\\Local\\MonsterSkillUnit.txt" );
	wsprintf( szRFileName[24], "..\\Data\\Local\\MonsterSkill.txt" );
	wsprintf( szRFileName[25], "..\\Data\\Local\\MonsterAIElement.txt" );
	wsprintf( szRFileName[26], "..\\Data\\Local\\MonsterAutomata.txt" );
	wsprintf( szRFileName[27], "..\\Data\\Local\\MonsterAIUnit.txt" );
	wsprintf( szRFileName[28], "..\\Data\\Local\\MonsterAIRule.txt" );
	wsprintf( szRFileName[29], "..\\Data\\Local\\MonsterAIGroup.txt" );
	wsprintf( szRFileName[30], "..\\Data\\Local\\Crywolf.dat" );
	wsprintf( szRFileName[31], "..\\Data\\Local\\SkillAdditionInfo.dat" );
	wsprintf( szRFileName[32], "..\\Data\\Local\\Kanturu.dat" );
	wsprintf( szRFileName[33], "..\\Data\\Local\\KanturuMonsterSetBase.txt" );
	wsprintf( szRFileName[34], "..\\Data\\Shops\\HanzoBlacksmith.txt" );
	wsprintf( szRFileName[35], "..\\Data\\Shops\\LumenBarmaid.txt" );
	wsprintf( szRFileName[36], "..\\Data\\Shops\\PasiTheMage.txt" );
	wsprintf( szRFileName[37], "..\\Data\\Shops\\MartinMerchant.txt" );
	wsprintf( szRFileName[38], "..\\Data\\Shops\\HaroldWandering.txt" );
	wsprintf( szRFileName[39], "..\\Data\\Shops\\AmyPotionGirl.txt" );
	wsprintf( szRFileName[40], "..\\Data\\Shops\\LiamBarmaid.txt" );
	wsprintf( szRFileName[41], "..\\Data\\Shops\\IsabelWizard.txt" );
	wsprintf( szRFileName[42], "..\\Data\\Shops\\ZiennaWeaponMerchant.txt" );
	wsprintf( szRFileName[43], "..\\Data\\Shops\\Crafstman.txt" );
	wsprintf( szRFileName[44], "..\\Data\\Shops\\ElfLala.txt" );
	wsprintf( szRFileName[45], "..\\Data\\Shops\\Alex.txt" );
	wsprintf( szRFileName[46], "..\\Data\\Shops\\ThompsonKenel.txt" );
	wsprintf( szRFileName[47], "..\\Data\\Shops\\LeoTheHelper.txt" );
	wsprintf( szRFileName[48], "..\\Data\\Shops\\PamelaTheSupplier.txt" );
	wsprintf( szRFileName[49], "..\\Data\\Shops\\AngelaTheSupplier.txt" );
	wsprintf( szRFileName[50], "..\\Data\\Shops\\FireworksGirl.txt" );
	wsprintf( szRFileName[51], "..\\Data\\Shops\\EleganceAllen.txt" );
	wsprintf( szRFileName[52], "..\\Data\\Shops\\Sylvia.txt" );
	wsprintf( szRFileName[53], "..\\Data\\Shops\\Leah.txt" );
	wsprintf( szRFileName[54], "..\\Data\\Shops\\Marseille.txt" );
	wsprintf( szRFileName[55], "..\\Data\\Shops\\StadiumGuard.txt" );
	wsprintf( szRFileName[56], "..\\Data\\Shops\\OracleLayla.txt" );
	wsprintf( szRFileName[57], "..\\Data\\Shops\\Christine.txt" );
	wsprintf( szRFileName[58], "..\\Data\\Shops\\Raul.txt" );
	wsprintf( szRFileName[59], "..\\Data\\Shops\\ReinaHaberdasher.txt" );
	wsprintf( szRFileName[60], "..\\Data\\Shops\\PorterMercenary.txt" );
	wsprintf( szRFileName[61], "..\\Data\\Shops\\David.txt" );
	wsprintf( szRFileName[62], "..\\Data\\Shops\\Shop29.txt" );
	// -----
	wsprintf( szRFileName[63], "..\\Data\\Common.ini" );
	// -----
	wsprintf( szRFCountryName, "%s", szLanguage );
	// -----
	RFCheckFile();
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CRFileManager	::	RFCheckFile()
{
	WIN32_FIND_DATAA wfd;
	// ------
	for( int n=0; n<MAX_FILE_NAME; n++ )
	{
		if( FindFirstFile ( szRFileName[n] , &wfd ) == INVALID_HANDLE_VALUE )
		{
			wsprintf( szTemp, FileMissed(n), szLanguage );
			CLog.MsgBox("Read Error", "Failed to Load File: %s", szTemp);
			ExitProcess(0);
		}
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CRFileManager	::	GetInt(char* Selection, char* Key, char* szFileName)
{
	return GetPrivateProfileIntA(Selection, Key, NULL, szFileName);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CRFileManager	::	GetString(char* Dest, char* Selection, char* Key, char* szFileName)
{
	GetPrivateProfileStringA( Selection, Key, NULL, this->String, sizeof(this->String), szFileName );
	// -----
	memcpy( Dest,this->String, sizeof(this->String) );
	// -----
	ZeroMemory( this->String, sizeof(this->String) );
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------