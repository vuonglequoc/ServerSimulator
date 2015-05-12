/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Decrypted Message (MUMSG) For Zodea GameServer
 * Coded By:	WolF & M.E.S
 */
#ifndef MU_MSG_HEADER
#define MU_MSG_HEADER


#define MSGGET(x, y)	((x)*256 + (y))
// -------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct MSG_STRUCT
{
	MSG_STRUCT* next;
	int number;
	LPBYTE msg;

} MSG_STRUCT, * LPMSG_STRUCT;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct TCHeader
{
	BYTE headcode;
	BYTE version;
	char caption[21];
	int count;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class CMsg
{
public:
	CMsg();
	virtual ~CMsg();
	// -----
	void	LoadWTF(char* filename);
	LPSTR	Get(int);
	// -----
private:
	bool	lMsgListInit();
	bool	lMsgListAdd(int, LPBYTE);
	void	XorBuffer(LPSTR, int);
	void	DataFileLoad(struct _iobuf*);
	void	lMsgFree();
	void	lMsgListPrint();
	// -----
	MSG_STRUCT * lMsgListNew();

protected:
	MSG_STRUCT * Msghead;
	MSG_STRUCT * Msgcur;
	MSG_STRUCT * MsgIndex[32768];
	TCHeader	 LoadHeader;
	// -----
	char szDefaultMsg[50];
};
extern CMsg lMsg;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------------------------------------------------------------------