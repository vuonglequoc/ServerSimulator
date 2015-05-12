#include "../Header Files/stdafx.h"
#include "../Header Files/CherryBlossom.h"
#include "../Header Files/Defines.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/LogProc.H"

CCherryBlossom gCherryBlossom;

CCherryBlossom::CCherryBlossom(void)
{
	gCherryBlossom.Init();
}

CCherryBlossom::~CCherryBlossom(void)
{

}

void CCherryBlossom::Init()
{
	gCherryBlossom.Enabled					= GetPrivateProfileInt(CHERRY_HDR,"Enabled",0,CHERRY_FILE_PATH);

	gCherryBlossom.ReadFile();
}

void CCherryBlossom::ReadFile()
{
	SMDFile = fopen(CHERRY_MIX_PATH, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("[CherryBlossom] Info file Load Fail [%s]", CHERRY_MIX_PATH);
		return;
	}

	int Token;
	char type = -1;

	while ( true )
	{
	    Token = GetToken();

        if( Token == 2)
		{
            break;
		}

		type = TokenNumber;

		while ( true )
		{
			Token = GetToken();

			if ( strcmp("end", TokenString ) == 0)
			{
				break;
			}

			gCherryBlossom.ItemType[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemIndex[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemLevel[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.Durability[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemSkill[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemLuck[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemOpt[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemExe[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemAnc[type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemSocket[0][type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemSocket[1][type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemSocket[2][type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemSocket[3][type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			Token = GetToken();
			gCherryBlossom.ItemSocket[4][type][gCherryBlossom.ItemCount[type]] = TokenNumber;

			gCherryBlossom.ItemCount[type]++;
		}
	}
}