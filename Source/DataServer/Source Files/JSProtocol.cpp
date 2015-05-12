#include "../Header Files/StdAfx.H"
#include "../Header Files/Main.h"
#include "../Header Files/Database.H"
#include "../Header Files/LogProc.H"
#include "../Header Files/MD5.H"


JSProtocol JSProto;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void JSProtocol::JSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	switch (HeadCode)
	{
		case JS_PROTO_CORE_SRV_LOGIN:
		{
			this->JoinServerLogin(aIndex, (JOIN_SERVER_INFO *)aRecv);				// Done & Revised By WolF	[24/12/2011]
			// -----
		}	break;
		// -----
		case JS_PROTO_CORE_ACC_REQUEST:
		{
			this->JGPAccountRequest(aIndex, (JOIN_SERVER_ACC_REQUEST *)aRecv);			// Done & Revised By WolF	[24/12/2011]
			// -----
		}	break;
		// -----
		case JS_PROTO_CORE_JOIN_FAIL:
		{
			this->GJPAccountFail(aIndex, (JOIN_SERVER_JOIN_FAIL *)aRecv);				// Done & Revised By WolF	[24/12/2011]
			// -----
		}	break;
		// -----
		case JS_PROTO_CORE_ACC_CLOSE:
		{
			GJPUserClose(aIndex, (JOIN_SERVER_USER_CLOSE *)aRecv);						// Done & Revised By WolF	[24/12/2011]
			// -----
		}	break;
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void JSProtocol::JoinServerLogin(int aIndex, JOIN_SERVER_INFO * lpMsg)
{
	JOIN_SERVER_RESULT JSResult	= {0};
	// -----
	JSResult.h.Set(JS_PROTO_CORE_SRV_LOGIN, sizeof(JOIN_SERVER_RESULT));
	// -----
	JSResult.Result	= 0x00;
	// -----
	if(JSrvManager.AddServer(aIndex, lpMsg->ServerCode, lpMsg->ServerName, lpMsg->Port) == true)
	{
		JSResult.Result	= 0x01;
		// -----
		CLog.Connect("[JoinServerLogin]: GS(%s) SrvCode(%d) Port(%d) Connected.", lpMsg->ServerName, lpMsg->ServerCode, lpMsg->Port);
	}
	// -----
	JSrvManager.DataSend(aIndex, (LPBYTE)&JSResult, JSResult.h.Size);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void JSProtocol::JGPAccountRequest(int aIndex, JOIN_SERVER_ACC_REQUEST * lpMsg)
{
	char	szAccountID[11]		= {0};
	char	szIp[16]			= {0};
	char	szPass[11]			= {0};
	char	szTemp[100]			= {0};
	char	JoominNumber[14]	= {0};
	DWORD	dwAccKey			= {0};
	// -----
	int		Number				= lpMsg->Number;
	int		DBNumber			= 0;
	int		UserNumber			= 0;
	int		BlocCode			= 0;
	UCHAR	Result				= ACC_REQ_SUCCESS;
	// -----
	MsgDigestV5					pMD5Hash;
	// -----
	gFunc.BuxConvert(lpMsg->AccountID, 10);
	gFunc.BuxConvert(lpMsg->Password, 10);
	// -----
	memcpy(szAccountID, lpMsg->AccountID , 10);
	memcpy(szPass, lpMsg->Password , 10);
	memcpy(szIp, lpMsg->IPAddress, 15);
	// -----
	if (( gFunc.CheckSQLSyntex   ( szAccountID )	== true ) &&
		( gFunc.CheckSQLSyntex   ( szPass )			== true ) &&
		( gFunc.CheckSpaceSyntex ( szAccountID)		== true ) )
	{
		if ( JSrvManager.CheckUser(szAccountID) == false )
		{
			if ( gDataBase.ExecFormat("SELECT * FROM MEMB_INFO WHERE memb___id='%s'", szAccountID) == true )
			{
				if ( gDataBase.Fetch() == SQL_SUCCESS )
				{
					DBNumber = gDataBase.GetInt("memb_guid");
					BlocCode = gDataBase.GetInt("bloc_code");
					// -----
					gDataBase.GetStr("sno__numb", JoominNumber);
					// -----
					if(gMainApp.MSTR.JoinSrv_UseMD5 == TRUE) // -- MD5 Login
					{
						BYTE btBinaryPass[16] = {0};
						// -----
						sprintf(szTemp, "SELECT memb__pwd from MEMB_INFO where memb___id='%s'", szAccountID);
						// -----
						if(gDataBase.GetAsBinary(szTemp, btBinaryPass) > 0)
						{
							dwAccKey = gFunc.CreateAccountKey(szAccountID);
							// -----
							if(pMD5Hash.MD5_CheckValue(szPass, (char*)btBinaryPass, dwAccKey) == true)
							{
								Result = ACC_REQ_SUCCESS;
							}
							else
							{
								Result = ACC_REQ_WRONG_PASS;
								// -----
								CLog.Error("[Protocol][MD5] AccountRequest() :: Wrong Password - ID: (%s).", szAccountID);
							}
						}
						else
						{
							Result = ACC_REQ_WRONG_PASS;
							// ----
							CLog.Error("[Protocol][MD5] AccountRequest() :: Password is Blank - ID: (%s).", szAccountID);
						}
					}
					else // -- Normal Login
					{
						char szPassword[11] = {0};
						// -----
						gDataBase.GetStr("memb__pwd", szPassword);
						// -----
						if(strcmp(szPass, szPassword) == NULL)
						{
							Result = ACC_REQ_SUCCESS;
						}
						else
						{
							CLog.Error("[Protocol][NON-MD5] AccountRequest() :: Wrong Password - ID: (%s).", szAccountID);
							// -----
							Result = ACC_REQ_WRONG_PASS;
						}
					}
					// -----
					gDataBase.Clear();
					// -----
					if(Result == ACC_REQ_SUCCESS)
					{
						if(BlocCode > 0)
						{
							Result = ACC_REQ_ACC_BANNED;
						}
						else
						{
							if(JSrvManager.AddUser(aIndex, szAccountID, szIp, lpMsg->Number) == false)
							{
								Result = ACC_REQ_MAX_USER;
							}
						}
					}
				}
				else
				{
					CLog.Error("[Protocol] AccountRequest() :: Account doesn't exist - ID: (%s).", szAccountID);
					// -----
					Result = ACC_REQ_WRONG_ID;
				}
			}
			else
			{
				CLog.Error("[Protocol] AccountRequest() :: Fail ExecFormat - ID: (%s).", szAccountID);
				// -----
				Result = ACC_REQ_TEMP_BLOCK;
			}
			// ----
			gDataBase.Clear();
		}
		else
		{
			CLog.Error("[Protocol] AccountRequest() :: Account already connected - ID: (%s).", szAccountID);
			// -----
			Result = ACC_REQ_ALREADY_CONNECT;
		}
	}
	else
	{
		CLog.Error("[Protocol] AccountRequest() :: (ANTIHACK) Special characters in ID: (%s).", szAccountID);
		// -----
		Result = ACC_REQ_TEMP_BLOCK;
	}
	// -----
	JOIN_SERVER_ACC_REQ_RESULT	Msg(Result, Number, UserNumber, DBNumber, szAccountID, JoominNumber);
	// -----
	JSrvManager.DataSend(aIndex, (LPBYTE)&Msg, Msg.h.Size);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void JSProtocol::GJPAccountFail(int aIndex, JOIN_SERVER_JOIN_FAIL * lpMsg)
{
	char szAccountID[11]	= {0};
	// ----
	memcpy(szAccountID, lpMsg->AccountID, 10);
	// ----
	CLog.Error("[Protocol] GJPAccountFail() - ID: (%s).", szAccountID);
	// ----
	JSrvManager.DelUser(szAccountID);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void JSProtocol::GJPUserClose(int aIndex, JOIN_SERVER_USER_CLOSE * lpMsg)
{
	char szAccountID[11] = {0};
	// ----
	memcpy(szAccountID, lpMsg->AccountID, 10);
	// ----
	JSrvManager.DelUser(szAccountID);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------