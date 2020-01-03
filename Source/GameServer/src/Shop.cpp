//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: Game Shops & NPCs Structures							//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "Main.H"
#include "Shop.H"
#include "LogProc.H"
#include "ReadScript.h"
#include "GameMain.H"
#include "WzMemScript.H"


CShop ShopC[MAX_SHOP];
// -----------------------------------------------------------------------------------------------------------------------
CShop::CShop(void)
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
CShop::~CShop(void)
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CShop::Init()
{
	this->SendItemDataLen	= 0;
	this->ItemCount			= 0;
	// -----
	memset(this->ShopInventoryMap, 0, sizeof(this->ShopInventoryMap));
}
// -----------------------------------------------------------------------------------------------------------------------
BOOL CShop::InsertItem(int Type, int Index, int Level, int Dur, int Opt1, int Opt2, int Opt3, int Exc, int Anc, BYTE Socket[5])
{
	int iTemp;
	int Width;
	int Height;
	int X;
	int Y;
	int Blank;

	Blank = -1;
	iTemp = Type * MAX_SUBTYPE_ITEMS + Index;
	
	if ( iTemp < 0 )
	{
		CLog.MsgBox("Error() Insert Item in Shop %s %d", __FILE__, __LINE__);
		return FALSE;
	}
	
	ItemGetSize(iTemp, Width, Height);

	if ( Width < 0 || Height < 0 )
	{
		CLog.MsgBox("Error() Insert Item Size in Shop %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	for ( Y=0; Y<15; Y++)
	{
		for ( X=0; X<8; X++)
		{
			if ( this->ShopInventoryMap[X + Y*8] == FALSE )
			{
				Blank = this->InentoryMapCheck(X, Y, Width, Height);

				if ( Blank >= 0 )
				{
					goto SkipLoop;
				}
			}
		}
	}

	if ( Blank < 0 )
	{
		CLog.MsgBox("Error() %s %d", __FILE__, __LINE__);
		return FALSE;
	}

SkipLoop:

	this->m_item[Blank].m_Level = Level;

	if ( Dur == 0 )
	{
		Dur = ItemGetDurability(ITEMGET(Type, Index), Level, 0, 0);
	}



	this->m_item[Blank].m_Durability = (float)Dur;
	this->m_item[Blank].Convert(iTemp, Opt1, Opt2, Opt3, Exc, Anc, 0,Socket);
	this->m_item[Blank].Value();
	this->SendItemData[this->SendItemDataLen] = Blank;
	this->SendItemDataLen++;
	ItemByteConvert((LPBYTE)&this->SendItemData[this->SendItemDataLen], this->m_item[Blank]);
	this->SendItemDataLen += MAX_ITEM_INFO;
	this->ItemCount++;

	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
int CShop::InentoryMapCheck(int CordX, int CordY, int Width, int Height)
{
	int X;
	int Y;
	int Blank = 0;

	if ( (CordX+Width) > 8 )
		return -1;

	if ( (CordY+Height) > 15 )
		return -1;

	for( Y=0; Y<Height; Y++)
	{
		for( X=0; X<Width; X++)
		{
			if ( this->ShopInventoryMap[( CordY + Y) * 8 + (CordX + X)] )
			{
				Blank++;
				break;
			}
		}
	}

	if ( Blank == 0 )
	{
		for( Y=0; Y<Height; Y++)
		{
			for( X=0; X<Width; X++ )
			{
				this->ShopInventoryMap[( CordY + Y) * 8 + (CordX + X)] = 1;
			}
		}

		return (CordX + CordY*8);
	}

	return -1;
}
// -----------------------------------------------------------------------------------------------------------------------
BOOL CShop::LoadShopItem(char* filename)
{
	int Token;
	char type;
	short index;
	char level;
	short dur;
	char op1;
	char op2;
	char op3;
	char Exc;
	char Anc;
	BYTE Socket[5];

	this->Init();

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("Error() Failed to Load Shop Data File: %s", filename);
		return FALSE;
	}

	while ( true )
	{
		Token = GetToken();

		if( Token == 2 )
			break;

		if( Token == 1 )
		{
			type = (char)TokenNumber;

			Token = GetToken();
			index = (short)TokenNumber;

			Token = GetToken();
			level = (char)TokenNumber;

			Token = GetToken();
			dur = (short)TokenNumber;

			Token = GetToken();
			op1 = (char)TokenNumber;

			Token = GetToken();
			op2 = (char)TokenNumber;

			Token = GetToken();
			op3 = (char)TokenNumber;

			Token = GetToken();
			Exc = (char)TokenNumber;

			Token = GetToken();
			Anc = (char)TokenNumber;

			Token = GetToken();
			Socket[0] = (BYTE)TokenNumber;

			Token = GetToken();
			Socket[1] = (BYTE)TokenNumber;

			Token = GetToken();
			Socket[2] = (BYTE)TokenNumber;

			Token = GetToken();
			Socket[3] = (BYTE)TokenNumber;

			Token = GetToken();
			Socket[4] = (BYTE)TokenNumber;

			if (this->InsertItem(type, index, level, dur, op1, op2, op3, Exc, Anc, Socket) == FALSE )
			{
				CLog.MsgBox("Error() %s %s %d", filename, __FILE__, __LINE__);
			}
		}
	}

	fclose(SMDFile);
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
BOOL ShopDataLoad()
{
	ShopC[0].LoadShopItem  ( RFile.szRFileName[34] );
	ShopC[1].LoadShopItem  ( RFile.szRFileName[35] );
	ShopC[2].LoadShopItem  ( RFile.szRFileName[36] );
	ShopC[3].LoadShopItem  ( RFile.szRFileName[37] );
	ShopC[4].LoadShopItem  ( RFile.szRFileName[38] );
	ShopC[5].LoadShopItem  ( RFile.szRFileName[39] );
	ShopC[6].LoadShopItem  ( RFile.szRFileName[40] );
	ShopC[7].LoadShopItem  ( RFile.szRFileName[41] );
	ShopC[8].LoadShopItem  ( RFile.szRFileName[42] );
	ShopC[9].LoadShopItem  ( RFile.szRFileName[43] );
	ShopC[10].LoadShopItem ( RFile.szRFileName[44] );
	ShopC[11].LoadShopItem ( RFile.szRFileName[45] );
	ShopC[12].LoadShopItem ( RFile.szRFileName[46] );
	ShopC[13].LoadShopItem ( RFile.szRFileName[47] );
	ShopC[14].LoadShopItem ( RFile.szRFileName[48] );
	ShopC[15].LoadShopItem ( RFile.szRFileName[49] );
	ShopC[16].LoadShopItem ( RFile.szRFileName[50] );
	ShopC[17].LoadShopItem ( RFile.szRFileName[51] );
	ShopC[18].LoadShopItem ( RFile.szRFileName[52] );
	ShopC[19].LoadShopItem ( RFile.szRFileName[53] );
	ShopC[20].LoadShopItem ( RFile.szRFileName[54] );
	ShopC[21].LoadShopItem ( RFile.szRFileName[55] );
	ShopC[22].LoadShopItem ( RFile.szRFileName[56] );
	ShopC[23].LoadShopItem ( RFile.szRFileName[57] );
	ShopC[24].LoadShopItem ( RFile.szRFileName[58] );
	ShopC[25].LoadShopItem ( RFile.szRFileName[59] );
	ShopC[26].LoadShopItem ( RFile.szRFileName[60] );
	ShopC[27].LoadShopItem ( RFile.szRFileName[61] );
	ShopC[28].LoadShopItem ( RFile.szRFileName[62] );
	
	CLog.LogAdd("Shop Data loaded Complete.");

	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------