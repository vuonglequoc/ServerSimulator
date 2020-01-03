#ifndef _INITCONFIGS_H_
#define _INITCONFIGS_H_

class CInitConfigs
{
public:
	CInitConfigs();
	~CInitConfigs();
	// ----
	void Init();
	// ---- Box
	char BoxOfLuckEnabled;
	char StarOfXMasEnabled;
	char FireCrackerEnabled;
	char HeartOfLoveEnabled;
	char SilverMedalEnabled;
	char GoldMedalEnabled;
	char BoxOfHeavenEnabled;
	char DarkLordHeartEnabled;
	char RedRibbonBoxEnabled;
	char GreenRibbonBoxEnabled;
	char BlueRibbonBoxEnabled;
	char PinkChocolateBoxEnabled;
	char RedChocolateBoxEnabled;
	char BlueChocolateBoxEnabled;
	char LightPurpleCandyBoxEnabled;
	char VermilionCandyBoxEnabled;
	char DeepBlueCandyBoxEnabled;
	// ---- 
};

extern CInitConfigs gInitConfigs;

#endif