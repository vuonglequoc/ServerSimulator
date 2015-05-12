/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [19.08.2011] This Script For JoinServer Manager Class.
#
#############################################################################*/
#ifndef JS_SRV_MANG_HEADER
#define JS_SRV_MANG_HEADER

#include "StdAfx.H"


class JServerManager : public IOCP
{
public:
	JServerManager(void);
	virtual ~JServerManager(void);
	// -----
	void				Initialize();
	// -----
	void				OnConnect		();
	void				OnDisconnect	(int aIndex);
	void				OnRecv			(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	// -----
	void				DelServer		(int ServerCode);
	void				DelUser			(const char * szAccountID);
	void				DelUserByIndex	(int nIndex);
	// -----
	bool				AddUser			(int ServerIndex, char * szAccountID, char * szIP, short nNumber);
	bool				CheckUser		(const char * szAccountID); 
	bool				IsCodeNotExist	(int ServerCode);
	bool				AddServer		(int ServerIndex,int ServerCode, char * szServerName, int Port);
	// -----
	int					GetUserIndexByID(const char * szAccountID);
	// -----
	int					m_SrvConnectedCount;
	int					m_UserConnectedCount;
	// -----
	tagSERVER_DATA		m_ServObj[MAX_OBJECT];
	tagUSER_DATA		m_UserObj[MAX_USER];
	TSyncCSection		TSync;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
extern JServerManager JSrvManager;
// ------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* JS_SRV_MANG_HEADER */
// ------------------------------------------------------------------------------------------------------------------------------------------------------