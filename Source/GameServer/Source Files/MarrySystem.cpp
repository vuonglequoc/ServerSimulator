#include "../Header Files/stdafx.h"
#include "../Header Files/MarrySystem.h"
#include "../Header Files/Defines.h"
#include "../Header Files/Notice.H"
// ----------------------------------------------------------------------------------------
CMarry gMarry;
// ----------------------------------------------------------------------------------------
CMarry::CMarry(void)
{

}
// ----------------------------------------------------------------------------------------
CMarry::~CMarry(void)
{

}
// ----------------------------------------------------------------------------------------
void CMarry::Init()
{
	gMarry.Enabled					= GetPrivateProfileInt(MARRY_HDR,"Enabled",0,MARRY_FILE_PATH);
	gMarry.OnlyMarryDifGender		= GetPrivateProfileInt(MARRY_HDR,"OnlyMarryWithDifferentGender",0,MARRY_FILE_PATH);
	MarryX[0]						= 209;
	MarryY[0]						= 16;
	MarryX[1]						= 210;
	MarryY[1]						= 16;
}
// ----------------------------------------------------------------------------------------
bool CMarry::IsMarried(LPOBJ lpObj)
{
	if ( lpObj->IsMarried == 1 )
	{
		return true;
	}
	return false;
}
// ----------------------------------------------------------------------------------------
bool CMarry::IsSameGender(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	if ( ( lpObj->Class == CLASS_ELF || lpObj->Class == CLASS_SUMMONER ) && ( lpTargetObj->Class == CLASS_ELF || lpTargetObj->Class == CLASS_SUMMONER ) )
	{
		return true;
	}
	else if ( ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_FIGHTER) &&
		      ( lpTargetObj->Class == CLASS_WIZARD || lpTargetObj->Class == CLASS_KNIGHT || lpTargetObj->Class == CLASS_MAGUMSA || lpTargetObj->Class == CLASS_DARKLORD || lpTargetObj->Class == CLASS_FIGHTER) )
	{
		return true;
	}
	return false;
}
// ----------------------------------------------------------------------------------------
bool CMarry::AreMarried(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	if ( lpObj->MarryName == lpTargetObj->Name )
	{
		return true;
	}
	return false;
}
// ----------------------------------------------------------------------------------------
void CMarry::MarryRequest(LPOBJ lpObj,LPOBJ lpTargetObj)
{
	// ---- Can't marry with yourself
	if ( memcmp(lpObj->Name,lpTargetObj->Name,sizeof(lpTargetObj->Name)) == 0 )
	{
		return;
	}
	// ---- Check if have Sent Request
	if ( lpObj->SendRequest == 1 )
	{
		if ( memcmp(lpObj->SendRequestName,lpTargetObj->Name,sizeof(lpTargetObj->Name)) == 0 )
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You have already sent request to %s",lpObj->Name);
			return;
		}
		else
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You can't request marriage to %s, you have requested marriage to %s",lpTargetObj->Name,lpObj->SendRequestName);
			return;
		}
	}
	if ( lpTargetObj->SendRequest == 1 )
	{
		if ( memcmp(lpTargetObj->SendRequestName,lpObj->Name,sizeof(lpTargetObj->Name)) == 0 )
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: Requested player have already sent marriage request to you");
			return;
		}
		else
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: Requested player have already sent marriage request to %s",lpTargetObj->SendRequestName);
			return;
		}
	}
	// ---- Check if have Recv Request
	if ( lpObj->RecvRequest == 1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You already have marriage request from %s",lpObj->RecvRequestName);
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: /MarryAccept to agree Marry with %s",lpObj->RecvRequestName);
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: /MarryRefuse to refuse Marry with %s",lpObj->RecvRequestName);
		return;
	}
	if ( lpTargetObj->RecvRequest == 1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: Requested player have already received a marriage request from %s",lpTargetObj->RecvRequestName);
		return;
	}
	// ---- Check Player Is Married
	if ( gMarry.IsMarried(lpObj) == true )
	{
		if ( gMarry.AreMarried(lpObj,lpTargetObj) == true )
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You are already married with %s",lpTargetObj->Name);
		}
		else
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You can't marry again, you are married with %s",lpObj->MarryName);
		}
		return;
	}
	// ---- Check Target Player Is Married
	if ( gMarry.IsMarried(lpTargetObj) == true )
	{
		if ( lpTargetObj->Class == CLASS_ELF || lpTargetObj->Class == CLASS_SUMMONER )
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You can't marry her, she is married with %s",lpTargetObj->MarryName);
		}
		else
		{
			SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You can't marry him, he is married with %s",lpTargetObj->MarryName);
		}
		return;
	}
	// ---- Check if are same Gender
	if ( gMarry.OnlyMarryDifGender == 1 && gMarry.IsSameGender(lpObj,lpTargetObj) == true )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You can't marry a player with same gender");
		return;
	}
	// ---- Check Players Map Number
	if ( lpObj->MapNumber != MAP_INDEX_DEVIAS || lpTargetObj->MapNumber != MAP_INDEX_DEVIAS )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You and your future partner can only get married in Devias Church");
		return;
	}
	// ---- Check Players Coords
	if ( lpObj->X != gMarry.MarryX[0] || lpObj->Y != gMarry.MarryY[0] )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You must be in this coords (X:%d, Y:%d) looking towards the altar",gMarry.MarryX[0],gMarry.MarryY[0]);
		return;
	}
	if ( lpTargetObj->X != gMarry.MarryX[1] || lpTargetObj->Y != gMarry.MarryY[1] )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: Your future partner must be in this coords (X:%d, Y:%d) looking towards the altar",gMarry.MarryX[1],gMarry.MarryY[1]);
		return;
	}
	// ---- Check Players Dir
	if ( lpObj->Dir != 1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You must be looking towards the altar");
		return;
	}
	if ( lpTargetObj->Dir != 1 )
	{
		SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: Your future partner must be looking towards the altar");
		return;
	}
	lpObj->SendRequest = 1;
	lpTargetObj->RecvRequest = 1;
	memcpy(lpObj->SendRequestName,lpTargetObj->Name,sizeof(lpTargetObj->Name));
	memcpy(lpTargetObj->RecvRequestName,lpObj->Name,sizeof(lpObj->Name));

	SendMsg.MessageOut(lpObj->m_Index, 0x01, "[Marry]: You have sent marriage request to %s",lpTargetObj->Name);
	SendMsg.MessageOut(lpTargetObj->m_Index, 0x01, "[Marry]: %s have sent you a marriage request",lpObj->Name);
	SendMsg.MessageOut(lpTargetObj->m_Index, 0x01, "[Marry]: /MarryAccept to agree Marry with %s",lpObj->Name);
	SendMsg.MessageOut(lpTargetObj->m_Index, 0x01, "[Marry]: /MarryRefuse to refuse Marry with %s",lpObj->Name);
}
// ----------------------------------------------------------------------------------------
void CMarry::MarryAgree(LPOBJ lpObj)
{

}
// ----------------------------------------------------------------------------------------
void CMarry::MarryRefuse(LPOBJ lpObj)
{

}
// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------