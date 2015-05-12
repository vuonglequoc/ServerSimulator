//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Notice Class.
//----------------------------------------------------------------------------------
#ifndef _NOTICE_H_
#define _NOTICE_H_


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * NOTE About Function Using!
 * -----
 * void NormalMsgOut(int aIndex, BYTE ObjectSend, int MapNumber, LPCCH szNotice, ...);
 * -------------------------------------------------------------------------------------
 * If You will use for All Players, Just insert "OBJ_EMPTY" In place of aIndex.
 * Use OBJECT_SEND_TYPES In Case of ObjectSend To Define the Target.
 * If You will not use for Specific Map, Just insert NO_SPC_MAP In Place Of MapNumber.
 */

enum OBJECT_SEND_TYPES
{
	OBJ_SEND_NORMAL			= 0x00,
	OBJ_SEND_ALL_PLYRS		= 0x01,
	OBJ_SEND_SPC_MAP		= 0x02,
	OBJ_SEND_GM				= 0x03,
	// -----
	OBJ_SEND_ALL_GOLDEN		= 0x00,
	OBJ_SEND_ALL_NORMAL		= 0x01
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_NOTICE
{
	PBMSG_HEAD	h;
	BYTE		type;
	BYTE		btCount;
	WORD		wDelay;	
	int			dwColor;
	BYTE		btSpeed;
	char		Notice[256];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class CNotice
{
public:
	CNotice();
	virtual ~CNotice();
	// -----
	void	MakeNotice			( LPVOID lpMessage, BYTE SubCode, BYTE Type, LPSTR Message );
	DWORD	inline _ARGB		( BYTE Alfa, BYTE Red, BYTE Green, BYTE Blue );
	// -----
	void	MessageOut			( int aIndex, BYTE Type, LPSTR szNotice, ... );
	void	MessageOutAll		( BYTE Type, LPSTR szNotice, ... );
	void	MessageOutMap		( int MapNumber, BYTE Type, LPSTR szNotice, ... );
	void	MessageOutGM		( BYTE Type, LPSTR szNotice, ... );
	// -----
	void	NormalMsgOut		( int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ... );
	void	WhisperOut			( int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ... );
	void	PartyMsgOut			( int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ... );
	void	GuildMsgOut			( int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ... );
	void	GMGuildMsgOut		( int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ... );
	void	AllianceMsgOut		( int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ... );
	void	GensMsgOut			( int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ... );
	// -----
private:
	PMSG_NOTICE		m_Notice;
	va_list			m_Args;
	char			m_Msg[256];
};
extern CNotice SendMsg;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* _NOTICE_H_ */
// -------------------------------------------------------------------------------------------------------------------------------------------------------