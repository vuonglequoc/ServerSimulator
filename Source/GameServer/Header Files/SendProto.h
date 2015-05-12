/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Send Protocol Class
 * Coded By:	WolF & mesosa
 */
#ifndef	SEND_PROTOCOL_H
#define	SEND_PROTOCOL_H

#include "StdAfx.H"
#include "GProtocol.H"
#include "User.H"


class CSendProtocol
{
public:
	void	DataSend		( int aIndex, LPBYTE lpMsg, DWORD dwSize );
	void	DataSendAll		( LPBYTE lpMsg, DWORD dwSize );
	void	DataSendMap		( int Map, LPBYTE lpMsg, DWORD dwSize );
	void	DataSendGM		( LPBYTE lpMsg, DWORD dwSize );
	void	VPMsgSend		( LPOBJ lpObj, LPBYTE lpMsg, int dwSize );
	void	CloseMsgSend	( int aIndex, BYTE Result );
	void	NoUsersMessage	( int aIndex );
	// -----
private:
	
};
extern CSendProtocol gSendProto;
// ---------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// ---------------------------------------------------------------------------------------------------------------------------------------------------