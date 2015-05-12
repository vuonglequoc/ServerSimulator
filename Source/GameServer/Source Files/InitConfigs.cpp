#include "../Header Files/stdafx.h"
#include "../Header Files/InitConfigs.h"
#include "../Header Files/Defines.h"

CInitConfigs gInitConfigs;

CInitConfigs::CInitConfigs()
{
	return;
}

CInitConfigs::~CInitConfigs()
{
	return;
}

void CInitConfigs::Init()
{
	gInitConfigs.BoxOfLuckEnabled				= GetPrivateProfileInt("EventItemBags","BoxOfLuckEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.StarOfXMasEnabled				= GetPrivateProfileInt("EventItemBags","StarOfXMasEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.FireCrackerEnabled				= GetPrivateProfileInt("EventItemBags","FireCrackerEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.HeartOfLoveEnabled				= GetPrivateProfileInt("EventItemBags","HeartOfLoveEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.SilverMedalEnabled				= GetPrivateProfileInt("EventItemBags","SilverMedalEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.GoldMedalEnabled				= GetPrivateProfileInt("EventItemBags","GoldMedalEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.BoxOfHeavenEnabled				= GetPrivateProfileInt("EventItemBags","BoxOfHeavenEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.DarkLordHeartEnabled			= GetPrivateProfileInt("EventItemBags","DarkLordHeartEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.RedRibbonBoxEnabled			= GetPrivateProfileInt("EventItemBags","RedRibbonBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.GreenRibbonBoxEnabled			= GetPrivateProfileInt("EventItemBags","GreenRibbonBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.BlueRibbonBoxEnabled			= GetPrivateProfileInt("EventItemBags","BlueRibbonBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.PinkChocolateBoxEnabled		= GetPrivateProfileInt("EventItemBags","PinkChocolateBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.RedChocolateBoxEnabled			= GetPrivateProfileInt("EventItemBags","RedChocolateBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.BlueChocolateBoxEnabled		= GetPrivateProfileInt("EventItemBags","BlueChocolateBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.LightPurpleCandyBoxEnabled		= GetPrivateProfileInt("EventItemBags","LightPurpleCandyBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.VermilionCandyBoxEnabled		= GetPrivateProfileInt("EventItemBags","VermilionCandyBoxEnabled",0,EVENTBAG_FILE_PATH);
	gInitConfigs.DeepBlueCandyBoxEnabled		= GetPrivateProfileInt("EventItemBags","DeepBlueCandyBoxEnabled",0,EVENTBAG_FILE_PATH);
}