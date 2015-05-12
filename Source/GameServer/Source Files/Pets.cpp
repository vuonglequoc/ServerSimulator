#include "../Header Files/stdafx.h"
#include "../Header Files/Pets.h"
#include "../Header Files/Defines.h"

CPets gPets;

CPets::CPets(void)
{
	return;
}

CPets::~CPets(void)
{
	return;
}

void CPets::Init()
{
	// ---- FENRIR
	gPets.FenrirRepairRate				= GetPrivateProfileInt(FENRIR_HDR,"FenrirRepairRate",0,PETS_FILE_PATH);
	gPets.FenrirMaxDurSmall[0]			= GetPrivateProfileInt(FENRIR_HDR,"FenrirDWMaxDurSmall",0,PETS_FILE_PATH);
	gPets.FenrirMaxDurSmall[1]			= GetPrivateProfileInt(FENRIR_HDR,"FenrirDKMaxDurSmall",0,PETS_FILE_PATH);
	gPets.FenrirMaxDurSmall[2]			= GetPrivateProfileInt(FENRIR_HDR,"FenrirELFMaxDurSmall",0,PETS_FILE_PATH);
	gPets.FenrirMaxDurSmall[3]			= GetPrivateProfileInt(FENRIR_HDR,"FenrirMGMaxDurSmall",0,PETS_FILE_PATH);
	gPets.FenrirMaxDurSmall[4]			= GetPrivateProfileInt(FENRIR_HDR,"FenrirDLMaxDurSmall",0,PETS_FILE_PATH);
	gPets.FenrirMaxDurSmall[5]			= GetPrivateProfileInt(FENRIR_HDR,"FenrirSUMaxDurSmall",0,PETS_FILE_PATH);
	gPets.FenrirMaxDurSmall[6]			= GetPrivateProfileInt(FENRIR_HDR,"FenrirRFMaxDurSmall",0,PETS_FILE_PATH);

	gPets.Fenrir01ChaosSuccessRate		= GetPrivateProfileInt(FENRIR_HDR,"Fenrir01ChaosSuccessRate",0,PETS_FILE_PATH);
	gPets.Fenrir02ChaosSuccessRate		= GetPrivateProfileInt(FENRIR_HDR,"Fenrir02ChaosSuccessRate",0,PETS_FILE_PATH);
	gPets.Fenrir03ChaosSuccessRate		= GetPrivateProfileInt(FENRIR_HDR,"Fenrir03ChaosSuccessRate",0,PETS_FILE_PATH);

	// ---- DARK HORSE
	gPets.DarkHorseAddExperience		= GetPrivateProfileInt(DARKHORSE_HDR,"DarkHorseAddExperience",0,PETS_FILE_PATH);

	// ---- DARK SPIRIT
	gPets.DarkSpiritAddExperience		= GetPrivateProfileInt(DARKHORSE_HDR,"DarkSpiritAddExperience",0,PETS_FILE_PATH);
}