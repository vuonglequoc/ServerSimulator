//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Notice Class.
//----------------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/Notice.H"
#include "../Header Files/SendProto.H"
#include "../Header Files/Functions.H"
#include "../Header Files/LogProc.H"


CNotice SendMsg;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CNotice::CNotice()
{
	this->m_Notice.btCount	= 1;
	this->m_Notice.btSpeed	= 20;
	this->m_Notice.dwColor	= _ARGB(255, 255, 200, 80);
	this->m_Notice.wDelay	= 0;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CNotice::~CNotice()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
_inline DWORD CNotice::_ARGB(BYTE Alfa, BYTE Red, BYTE Green, BYTE Blue)
{
	return Alfa*16777216 + Blue*65536 + Green*256 + Red;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::MakeNotice(LPVOID lpMessage, BYTE SubCode, BYTE Type, LPSTR Message)
{
	int NoticeLen			= NULL;
	// -----
	PMSG_NOTICE * pNotice	= (PMSG_NOTICE *)lpMessage;
	// -----
	pNotice->type  = Type;
	// -----
	wsprintf(pNotice->Notice, Message);
	// -----
	NoticeLen = (strlen(pNotice->Notice) + sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + 1);
	// -----
	C1HeadSet((LPBYTE)pNotice, SubCode, NoticeLen);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::MessageOut(int aIndex, BYTE Type, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		vsprintf		( &m_Msg[0], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x0D, Type, m_Msg);
		// -----
		gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
	}
	catch(...)
	{
		CLog.Error("[NOTICE] MessageOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::MessageOutAll(BYTE Type, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		vsprintf		( &m_Msg[0], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x0D, Type, m_Msg);
		// -----
		gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
	}
	catch(...)
	{
		CLog.Error("[NOTICE] MessageOutAll() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::MessageOutMap(int MapNumber, BYTE Type, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		vsprintf		( &m_Msg[0], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x0D, Type, m_Msg);
		// -----
		gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
	}
	catch(...)
	{
		CLog.Error("[NOTICE] MessageOutMap() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::MessageOutGM(BYTE Type, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		vsprintf		( &m_Msg[0], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x0D, Type, m_Msg);
		// -----
		gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
	}
	catch(...)
	{
		CLog.Error("[NOTICE] MessageOutGM() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::NormalMsgOut(int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		vsprintf		( &m_Msg[0], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x00, 0x00, m_Msg);
		// -----
		switch ( ObjectSend )
		{
			case OBJ_SEND_NORMAL:
			{
				if ( aIndex >= OBJMIN &&  aIndex <= OBJMAX )
					gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_ALL_PLYRS:
			{
				gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
			// -----
			case OBJ_SEND_SPC_MAP:
			{
				if ( MapNumber >= 0 && MapNumber <= 81 )
					gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_GM:
			{
				gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
		}
	}
	catch(...)
	{
		CLog.Error("[NOTICE] NormalMsgOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::WhisperOut(int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		vsprintf		( &m_Msg[0], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x02, 0x00, m_Msg);
		// -----
		switch ( ObjectSend )
		{
			case OBJ_SEND_NORMAL:
			{
				if ( aIndex >= OBJMIN &&  aIndex <= OBJMAX )
					gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_ALL_PLYRS:
			{
				gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
			// -----
			case OBJ_SEND_SPC_MAP:
			{
				if ( MapNumber >= 0 && MapNumber <= 81 )
					gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_GM:
			{
				gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
		}
	}
	catch(...)
	{
		CLog.Error("[NOTICE] WhisperOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::PartyMsgOut(int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		m_Msg[0]		= '~';
		vsprintf		( &m_Msg[1], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x00, 0x00, m_Msg);
		// -----
		switch ( ObjectSend )
		{
			case OBJ_SEND_NORMAL:
			{
				if ( aIndex >= OBJMIN &&  aIndex <= OBJMAX )
					gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_ALL_PLYRS:
			{
				gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
			// -----
			case OBJ_SEND_SPC_MAP:
			{
				if ( MapNumber >= 0 && MapNumber <= 81 )
					gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_GM:
			{
				gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
		}
	}
	catch(...)
	{
		CLog.Error("[NOTICE] PartyMsgOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::GuildMsgOut(int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		m_Msg[0]		= '@';
		vsprintf		( &m_Msg[1], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x00, 0x00, m_Msg);
		// -----
		switch ( ObjectSend )
		{
			case OBJ_SEND_NORMAL:
			{
				if ( aIndex >= OBJMIN &&  aIndex <= OBJMAX )
					gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_ALL_PLYRS:
			{
				gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
			// -----
			case OBJ_SEND_SPC_MAP:
			{
				if ( MapNumber >= 0 && MapNumber <= 81 )
					gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_GM:
			{
				gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
		}
	}
	catch(...)
	{
		CLog.Error("[NOTICE] GuildMsgOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::GMGuildMsgOut(int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		vsprintf		( &m_Msg[0], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x0D, 0x02, m_Msg);
		// -----
		switch ( ObjectSend )
		{
			case OBJ_SEND_NORMAL:
			{
				if ( aIndex >= OBJMIN &&  aIndex <= OBJMAX )
					gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_ALL_PLYRS:
			{
				gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
			// -----
			case OBJ_SEND_SPC_MAP:
			{
				if ( MapNumber >= 0 && MapNumber <= 81 )
					gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_GM:
			{
				gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
		}
	}
	catch(...)
	{
		CLog.Error("[NOTICE] GMGuildMsgOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::AllianceMsgOut(int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		m_Msg[0]		= '@';
		m_Msg[1]		= '@';
		vsprintf		( &m_Msg[2], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x00, 0x00, m_Msg);
		// -----
		switch ( ObjectSend )
		{
			case OBJ_SEND_NORMAL:
			{
				if ( aIndex >= OBJMIN &&  aIndex <= OBJMAX )
					gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_ALL_PLYRS:
			{
				gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
			// -----
			case OBJ_SEND_SPC_MAP:
			{
				if ( MapNumber >= 0 && MapNumber <= 81 )
					gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_GM:
			{
				gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
		}
	}
	catch(...)
	{
		CLog.Error("[NOTICE] AllianceMsgOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CNotice::GensMsgOut(int aIndex, BYTE ObjectSend, int MapNumber, LPSTR szNotice, ...)
{
	try
	{
		VA_START		( m_Args, szNotice );
		m_Msg[0]		= '$';
		vsprintf		( &m_Msg[1], szNotice, m_Args );
		VA_END			( m_Args );
		// -----
		this->MakeNotice(&m_Notice, 0x00, 0x00, m_Msg);
		// -----
		switch ( ObjectSend )
		{
			case OBJ_SEND_NORMAL:
			{
				if ( aIndex >= OBJMIN &&  aIndex <= OBJMAX )
					gSendProto.DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_ALL_PLYRS:
			{
				gSendProto.DataSendAll((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
			// -----
			case OBJ_SEND_SPC_MAP:
			{
				if ( MapNumber >= 0 && MapNumber <= 81 )
					gSendProto.DataSendMap(MapNumber, (LPBYTE)&m_Notice, m_Notice.h.size);
				else return;
			} break;
			// -----
			case OBJ_SEND_GM:
			{
				gSendProto.DataSendGM((LPBYTE)&m_Notice, m_Notice.h.size);
			} break;
		}
	}
	catch(...)
	{
		CLog.Error("[NOTICE] GensMsgOut() Could not be sent - Error:[%d]");
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------