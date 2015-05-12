//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Golden Archer Event
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#include "../Header Files/stdafx.h"
#include "../Header Files/GoldenArcher.H"
#include "../Header Files/User.h"
#include "../Header Files/Notice.H"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/Functions.h"
#include "../Header Files/SendProto.H"

// --------------------------------------------------------------------------------------------------------------------------------------------------
CGoldenArcher gGAEvent;
// --------------------------------------------------------------------------------------------------------------------------------------------------
CGoldenArcher::CGoldenArcher()
{
	// ----
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
CGoldenArcher::~CGoldenArcher()
{
	// ----
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGoldenArcher::Init(LPSTR szFileName)
{
	this->ReadFile(this->ReadFiles, szFileName);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
int CGoldenArcher::GetRenaCount(int PlayerID)
{
	LPOBJ lpObj = &gObj[PlayerID];
	// ----
	int count = 0;
	// ----
	for(unsigned i = 0; i < 76; i++)
	{
		if(lpObj->pInventory[i].m_Type == 0x1C15 && lpObj->pInventory[i].m_Level == 0)
			count++;
	}
	// ----
	return count;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGoldenArcher::PlayerDeleteQuestItem(int iIndex)
{
	int count = 0;
	// ----
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}
	// ----
	for ( int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == 0x1C15 && gObj[iIndex].pInventory[x].m_Level == 0 )
			{
				gObjInventoryItemSet(iIndex, x, -1);
				gObjInventoryDeleteItem(iIndex, x);
				GCInventoryItemDeleteSend(iIndex, x, TRUE);
				count++;

				if(count == count)
					break;
			}
		}
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGoldenArcher::NPCTalk_GoldenArcher(int PlayerID) 
{
	if(this->GetRenaCount(PlayerID) > 0)
	{
		LPOBJ lpObj = &gObj[PlayerID];
		
		unsigned short *ActualPts = (unsigned short*)&lpObj->m_Quest[29];
		
		if(*ActualPts == 0xFFFF)
		{
			*ActualPts = 0;
		}
		
		if(*ActualPts > this->ReqItem)
		{
			*ActualPts = 0;
		}
			
		*ActualPts += 1;
		
		this->PlayerDeleteQuestItem(PlayerID);
	
		if(*ActualPts == this->ReqItem)
		{
			this->RewardItem(PlayerID);
			MsgOutput(PlayerID,"[GoldenArcher] Enjoy Reward");
			*ActualPts = *ActualPts - this->ReqItem;
		}
		else
		{
			int RenasCount = 0;
			RenasCount = RenasCount + *ActualPts;
			MsgOutput(PlayerID,"[%s]: Your 'Rena' accepted, You Sent (%d) Rena(s), For Reward need (%d).",lpObj->Name,RenasCount,this->ReqItem);
		}
		
		unsigned char GAMsg[7] = {0xc1, 0x07, 0x94, 0x00};
		memcpy(&GAMsg[4], ActualPts, 2);
		gSendProto.DataSend(lpObj->m_Index, GAMsg, 7);
	}	
	else
	{
		SendMsg.MessageOut(PlayerID, 0x01, "[Golden Archer] You don't have the required Renas");
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGoldenArcher::RewardItem(int aIndex) 
{
	int ItemLine = Role(static_cast<int>(gGAEvent.ReadFiles.size() - 1));

	LPOBJ lpObj = &gObj[aIndex];

	int ItemType, ItemIndex, Level, Durability, Luck, Skill, Option, Exc, Ancient;
	unsigned char Socket[5];

	sscanf(this->ReadFiles.at(ItemLine).c_str(), "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &ItemType, &ItemIndex, &Level, &Durability, &Luck,
		&Skill, &Option, &Exc, &Ancient, &Socket[0], &Socket[1], &Socket[2],&Socket[3],&Socket[4]);

	UINT  RewardItem = ItemType *512 + ItemIndex;

	ItemSerialCreateSend(aIndex, lpObj->MapNumber, lpObj->X, lpObj->Y, RewardItem, Level, Durability, Luck, Skill, Option, aIndex, Exc, Ancient, Socket);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
void CGoldenArcher::ReadFile(std::vector<std::string> &buffer, const char *szFilename)
{
	char Line[1024];

	SMDFile = fopen(szFilename, "r");

	buffer.clear();
    while(!feof(SMDFile))
    {
        fgets(Line, 1024, SMDFile);
		if(strcmp(Line, "end") == 0)
			break;

        if(Line[strlen(Line) - 1] == '\n' || Line[strlen(Line) - 1] == '\r')
        {
            if(Line[0] != '/' || Line[1] != '/')
				buffer.push_back(Line);
        }
    }
    fclose(SMDFile);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------