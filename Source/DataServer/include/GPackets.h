/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [19.08.2011] This Script For General Packets of the Application.
#
#############################################################################*/
#ifndef PACKET_HEADER
#define PACKET_HEADER

#include "StdAfx.H"


struct PBMSG_HEAD
{
public:
	BYTE	c;
	BYTE	Size;
	BYTE	HeadCode;
	// -----
	void Set(UCHAR Head, UCHAR dwSize)
	{
		this->c			= 0xC1;
		this->Size		= dwSize;
		this->HeadCode	= Head;
	};
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PWMSG_HEAD
{
public:
	BYTE	c;
	BYTE	SizeH;
	BYTE	SizeL;
	BYTE	HeadCode;
	// -----
	void Set(UCHAR head, int dwSize)
	{
		this->c			= 0xC2;
		this->SizeH		= SET_NUMBERH(dwSize);
		this->SizeL		= SET_NUMBERL(dwSize);
		this->HeadCode	= head;
	};
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PBMSG_HEAD2
{
public:
	BYTE c;
	BYTE Size;
	BYTE HeadCode;
	BYTE SubCode;
	// -----
	void Set(UCHAR dwHeadCode, UCHAR dwSubCode, UCHAR Size)
	{
		this->c			= 0xC1;
		this->Size		= Size;
		this->HeadCode	= dwHeadCode;
		this->SubCode	= dwSubCode;
	};
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_DEFAULT2
{
	PBMSG_HEAD	h;
	BYTE		subcode;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct SDHP_SERVERINFO
{
	struct	PBMSG_HEAD h;
	//BYTE	Type;
	USHORT	Port;
	char	ServerName[50];
	USHORT	ServerCode;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct SDHP_RESULT
{
	struct	PBMSG_HEAD h;
	BYTE	Result;
	ULONG	ItemCount;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* PACKET_HEADER */
// -------------------------------------------------------------------------------------------------------------------------------------------------------