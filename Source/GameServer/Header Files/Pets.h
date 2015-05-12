#ifndef PETS_H
#define PETS_H

class CPets
{
public:
	CPets(void);
	~CPets(void);
	// ----
	void Init();
	// ---- Angel
	// ---- Imp
	// ---- Uniria
	// ---- Dinorant
	// ---- Demon
	// ---- Spirit Guardian
	// ---- Panda
	// ---- Rudolf
	// ---- Skeleton
	// ---- Fenrir
	unsigned short FenrirRepairRate;
	unsigned char FenrirMaxDurSmall[7];
	unsigned char Fenrir01ChaosSuccessRate;
	unsigned char Fenrir02ChaosSuccessRate;
	unsigned char Fenrir03ChaosSuccessRate;

	// ---- Dark Horse
	unsigned short DarkHorseAddExperience;

	// ---- Dark Spirit
	unsigned short DarkSpiritAddExperience;

};

extern CPets gPets;

#endif