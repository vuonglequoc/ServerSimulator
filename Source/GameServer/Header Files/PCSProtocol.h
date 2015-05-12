//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU PCS Protocol Structures								//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef PCSPROTOCOL_H__
#define PCSPROTOCOL_H__

#define MAX_NTF_TRANSACTIONS	16

#pragma pack(1)

struct sellItemPrice
{
	DWORD dwPriceGuid;
	DWORD dwItemGuid;
	DWORD dwUseTime;
	DWORD dwAmount;
	DWORD dwPrice;
	DWORD dwSellRate;
};

struct sellItem
{
	DWORD dwItemGuid;
	DWORD dwItemCODE;
	DWORD dwCategoryID;
	DWORD dwUseType;
	DWORD dwBuyType;
	DWORD dwCoolTime;
	double dbVarEndDate;
	DWORD dwLimitSellCount;
	DWORD dwState;
	char szItemName[64];
	char szItemDesc[128];
	DWORD dwPriceCount;
	sellItemPrice itemPrice[8];
};

struct sellPackage
{
	DWORD dwPackageGuid;
	sellItemPrice itemPrice;
	DWORD dwImageCode;
	DWORD dwCategory;
	DWORD dwBuyType;
	DWORD dwLimitSellCount;
	DWORD dwInItemCount;
	double dbVarEndDate;
	DWORD dwState;
	sellItemPrice inItemPrices[16];
	char szPackageName[64];
	char szPackageDesc[128];
};

struct buyItemTransaction
{
	DWORD dwTransactionGuid;
	DWORD dwPriceGuid;
	DWORD dwResult;
};

struct itemBranch
{
	DWORD dwBranchID;
	DWORD dwGuid;
	DWORD dwItemType;
};

#pragma pack()

namespace protocol
{
	class HDRMessageID
	{
	public:
		WORD category;
		WORD messageID;
	};

	class MSG_HEADER
	{
	public:

		DWORD size;
		union
		{
			DWORD id;
			HDRMessageID hdrid;
		};
	};

	class MSG_GTOS_USER_CASH_REQ : public MSG_HEADER
	{
	public:
		DWORD dwUserGuid;

		MSG_GTOS_USER_CASH_REQ()
		{
			this->size = sizeof(MSG_GTOS_USER_CASH_REQ);
			this->id = 0x100;
		};

		~MSG_GTOS_USER_CASH_REQ()
		{
		};
	};

	class MSG_STOG_USER_CASH_ANS : public MSG_HEADER
	{
	public:
		DWORD dwUserGuid;
		DWORD dwUserCash;

		MSG_STOG_USER_CASH_ANS()
		{
			this->size = sizeof(MSG_STOG_USER_CASH_ANS);
			this->id = 0x200;
		};

		~MSG_STOG_USER_CASH_ANS()
		{
		};
	};

	class MSG_GTOS_ITEM_LIST_REQ : public MSG_HEADER
	{
	public:
		MSG_GTOS_ITEM_LIST_REQ()
		{
			this->size = sizeof(MSG_GTOS_ITEM_LIST_REQ);
			this->id = 0x300;
		};

		~MSG_GTOS_ITEM_LIST_REQ()
		{
		};
	};

	class MSG_STOG_ITEM_LIST_ANS : public MSG_HEADER
	{
	public:
		MSG_STOG_ITEM_LIST_ANS()
		{
			this->size = sizeof(MSG_STOG_ITEM_LIST_ANS);
			this->id = 0x400;
		};
		~MSG_STOG_ITEM_LIST_ANS()
		{
			return;
		};

		void resize();

		DWORD dwItemCount;
		sellItem sellItems[128];
	};

	class MSG_GTOS_PACKAGE_LIST_REQ : public MSG_HEADER
	{
	public:
		MSG_GTOS_PACKAGE_LIST_REQ()
		{
			this->size = sizeof(MSG_GTOS_PACKAGE_LIST_REQ);
			this->id = 0x500;
		};

		~MSG_GTOS_PACKAGE_LIST_REQ()
		{
		};
	};

	class MSG_STOG_PACKAGE_LIST_ANS : public MSG_HEADER
	{
	public:
		MSG_STOG_PACKAGE_LIST_ANS()
		{
			this->size = sizeof(MSG_STOG_PACKAGE_LIST_ANS);
			this->id = 0x600;
		};

		~MSG_STOG_PACKAGE_LIST_ANS()
		{
		};
		void resize();

		DWORD dwPackageCount;
		sellPackage sellPackages[96];
	};

	class MSG_GTOS_BRANCH_ITEM_LIST_REQ : public MSG_HEADER
	{
	public:
		MSG_GTOS_BRANCH_ITEM_LIST_REQ()
		{
			this->size = sizeof(MSG_GTOS_BRANCH_ITEM_LIST_REQ);
			this->id = 0x700;
		};

		~MSG_GTOS_BRANCH_ITEM_LIST_REQ()
		{
		};

	};

	class MSG_STOG_BRANCH_ITEM_LIST_ANS : public MSG_HEADER
	{
	public:
		DWORD dwItemCount;
		itemBranch branchItems[512];

		MSG_STOG_BRANCH_ITEM_LIST_ANS()
		{
			this->size = sizeof(MSG_STOG_BRANCH_ITEM_LIST_ANS);
			this->id = 0x800;
		};


		~MSG_STOG_BRANCH_ITEM_LIST_ANS()
		{
		};

		void resize();
	};

	class MSG_GTOS_BUY_ITEM_REQ : public MSG_HEADER
	{
	public:
		DWORD dwServerGuid;
		DWORD dwUserGuid;
		DWORD dwCharGuid;
		char szCharName[96];
		DWORD dwCount;
		DWORD dwPriceGuids[16];

		MSG_GTOS_BUY_ITEM_REQ()
		{
			this->size = sizeof(MSG_GTOS_BUY_ITEM_REQ);
			this->id = 0x900;
		};

		~MSG_GTOS_BUY_ITEM_REQ()
		{
		};

		void resize();
	};

	class MSG_STOG_BUY_ITEM_ANS : public MSG_HEADER
	{
	public:
		DWORD dwServerGuid;
		DWORD dwUserGuid;
		DWORD dwCharGuid;
		DWORD dwUserCash;
		DWORD dwResult;
		DWORD dwCount;
		buyItemTransaction transactions[16];

		MSG_STOG_BUY_ITEM_ANS()
		{
			this->size = sizeof(MSG_STOG_BUY_ITEM_ANS);
			this->id = 0xA00;
		};

		~MSG_STOG_BUY_ITEM_ANS()
		{
		};

		void resize();
	};

	class MSG_GTOS_BUY_ITEM_NTF : public MSG_HEADER
	{
	public:
		MSG_GTOS_BUY_ITEM_NTF()
		{
			this->size = sizeof(MSG_GTOS_BUY_ITEM_NTF);
			this->id = 0xB00;
		};


		~MSG_GTOS_BUY_ITEM_NTF()
		{
		};

		void resize()
		{
			this->size -= (16 - (DWORD)((WORD)(this->dwCount))) * sizeof(buyItemTransaction);
		};

		DWORD dwServerGuid;
		DWORD dwUserGuid;
		DWORD dwCharGuid;
		DWORD dwCount;
		buyItemTransaction transactions[16];

	};
}


#endif