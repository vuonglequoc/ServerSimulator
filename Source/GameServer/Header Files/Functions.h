#include "StdAfx.h"
#include "MapClass.h"


#define Func_Module		"FUNCTION"

char *	GetMonthName(int Month);
char *	GetDayName(int Day);
void	GCFireWorkSend(int aIndex, int Amount);
char *	GetMapName(int MapId);
void	SummonMonster(DWORD PlayerID);
void	DisconnectAllPlayers();
void	LogOutAllUsers();
void	RemoveAllMonstersAndReLoad();
void	ReadMapAttrName();
int		GetWarehouseUsedHowMuch(int UserLevel, BOOL IsLock);
void	AddAllMonsters();
char *	FileMissed(int File);
bool	gObjIsConnected(int aIndex);
bool	gObjIsConnected(LPOBJ lpObj);
unsigned char		GetRandomExeOption(unsigned char MaxOption);
unsigned char Role(unsigned char range);
char * GetStatus(int Status);

// -----
void BuxConvert(char* buf, int size);
void C1HeadSet(LPBYTE lpBuf, BYTE head, int size);
void C1SubHeadSet(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void C2HeadSet( LPBYTE lpBuf, BYTE head, int size) ;
void C2SubHeadSet(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void C3HeadSet(LPBYTE lpBuf, BYTE head,int size);
void C3SubPHeadSet(LPBYTE lpBuf, BYTE head,BYTE sub, int size);
// -----
class char_ID
{
public:
	char_ID(LPSTR szName)
	{
		memset(this->Name, 0, sizeof(this->Name));
		// -----
		if ( szName != NULL )
		{
			memcpy(this->Name, szName, MAX_ACCOUNT_LEN);
		}
	}
	// -----
	LPSTR GetBuffer() { return this->Name; }
	// -----
	int  GetLength() { return strlen(this->Name); }
	// -----
private:
	char Name[MAX_ACCOUNT_LEN+1];
};

bool FolderExists(std::string file);