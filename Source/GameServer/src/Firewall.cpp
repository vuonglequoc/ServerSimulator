//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Firewall Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "Firewall.h"
#include "ReadScript.h"
#include "User.h"
#include "Defines.h"


GCFirewall FirewallC;
IPBLOCK IPBlockInfo[255];
//-----------------------------------------------------------------------------------------------------------------------
GCFirewall::GCFirewall()
{
	// ----
}
// ----------------------------------------------------------------------------------------------------------------------
GCFirewall::~GCFirewall()
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------
void GCFirewall::Init()
{
	Enabled = GetPrivateProfileIntA(COMMON_HDR,"SwitchFirewall",1,COMMON_FILE_PATH);

	//Read Firewall File
	this->Firewall__ReadFile();
}
//-----------------------------------------------------------------------------------------------------------------------
void GCFirewall::Firewall__ReadFile()
{
	BOOL bRead = FALSE;
	DWORD dwArgv = 0;
	char sLineTxt[255] = {0};
	IPBlockCount = 1;

	SMDFile = fopen(FIREWALL_FILE_PATH,"r");

	rewind(SMDFile);
	
	while(fgets(sLineTxt, 255, SMDFile) != NULL)
	{
		if(sLineTxt[0] == '/')continue;
		if(sLineTxt[0] == ';')continue;

		sscanf(sLineTxt, "IpAddress: %s", &IpAddress);
		sprintf(IPBlockInfo[IPBlockCount].IpAddress,"%s",IpAddress);
		IPBlockCount++;
	}

	rewind(SMDFile);
	fclose(SMDFile);
}
//-----------------------------------------------------------------------------------------------------------------------
bool GCFirewall::CheckIP(DWORD aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	for(int x=1; x<IPBlockCount; x++)
	{
		if(!strcmp(IPBlockInfo[x].IpAddress,lpObj->Ip_addr))
		{
			gNet.CloseClient(aIndex);
			return FALSE;
		}
	}
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------