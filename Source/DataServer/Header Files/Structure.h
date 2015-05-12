/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [09.08.2011] This Script For Structures Of The Application.
#
#############################################################################*/
#ifndef STRUCTURE_HEADER
#define STRUCTURE_HEADER

#include "StdAfx.H"


struct MAIN_STRUCT
{
	char			SQL_Host[256];
	char			SQL_DB[256]; 
	char			SQL_User[256];
	char			SQL_Pass[256];
	int				MAX_COLUMN;
	// -----
	WORD			JoinSrv_Port;
	WORD			DataSrv_Port;
	int				JoinSrv_UseMD5;
	int				DataSrv_Count;
};

// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct FONT_INFO
{
	HFONT			Font;
	int				Height;
	int				Width;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct COLOR_INFO
{
	HBRUSH			ActiveBG;
	COLORREF		StandByBG;
	COLORREF		ActiveText;
	COLORREF		StandByText;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct LOG_INFO
{
	int				Len;
	COLORREF		Color;
	char			Text[LOG_LINE_LENGTH];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
enum SERVER_STATUS
{
	SRVS_OFFLINE,
	SRVS_ONLINE,
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct OBJECTSTRUCT
{
	short					m_Index;
	char					m_ServerIp[16];
	// ----
	SERVER_STATUS			m_State;
	SOCKET					m_Socket;
	// ----
	struct _PER_SOCKET_CONTEXT * PerSocketContext;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct _PER_IO_CONTEXT
{ 
	WSAOVERLAPPED			Overlapped;
	WSABUF					wsabuf; 
	// ----
	UCHAR					Buffer[MAX_IO_BUFFER_SIZE];
	UCHAR					BufferSecond[MAX_IO_BUFFER_SIZE];
	// ----
	int						nSecondOfs;
	int						nTotalBytes;
	int						nSentBytes;
	BYTE					IOOperation;
	BYTE					nWaitIO;
}; 
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct _PER_SOCKET_CONTEXT
{
	int						nIndex;
	int						dwIOCount;
	// ----
	SOCKET					m_socket;
	_PER_IO_CONTEXT			IOContext[2];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
enum eSRV_TYPE
{
	NONE,
	// -----
	ST_JOIN_SERVER,
	// -----
	ST_DATA_SERVER,
	// -----
	MAX_SRV_TYPES
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct tagSERVER_DATA
{
	void	Clear();
	char	m_szServerName[50];
	int		m_Index;
	int		m_ServerCode;
	int		m_Port;
	int		m_ConnectedCount;
	int		m_MaxUser;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct tagUSER_DATA
{
	void	Clear();
	char	m_szAccountID[10];
	int		m_ServerIndex;
	int		m_ServerCode;
	int		m_nMoveMapTime;
	int		iUserNumber;
	short	nNumber;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
enum QUERY_PROCESS_TYPES
{
	QUERY_NULL			= -100,
	QUERY_EOF			= -101,
	QUERY_NO_COLUMN		= -102,
	QUERY_ERROR			= -103,
	// -----
	MAX_COLUMN			= 100
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* STRUCTURE_HEADER */
// -------------------------------------------------------------------------------------------------------------------------------------------------------