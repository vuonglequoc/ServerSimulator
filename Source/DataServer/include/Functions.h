/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [10.08.2011] This Script For Main Functions of the Application.
#
#############################################################################*/
#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include "StdAfx.h"


class CFunction
{
public:
	LPSTR	FileMissed			( int File );
	void	BuxConvert			( LPSTR Buff, int dwSize );
	bool	CheckSQLSyntex		( LPSTR String );
	bool	CheckSpaceSyntex	( LPSTR String );
	DWORD	CreateAccountKey	( LPTSTR lpszAccountID );
	void	PHeadSetW			( LPBYTE lpBuf, BYTE Head, int Size );
	void	PHeadSubSetB		( LPBYTE lpBuf, BYTE Head, BYTE SubHead, int Size );
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
extern CFunction gFunc;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* FUNCTIONS_HEADER */
// -------------------------------------------------------------------------------------------------------------------------------------------------------