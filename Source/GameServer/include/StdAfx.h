#pragma once
// -----
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
// -----
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x601
#endif
// -----
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
// -----
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4244 )
// -----
/* System Headers */
#include <afx.h>
#include <windows.h>
#include <winbase.h>
#include <winsock2.h>
#include "COMMCTRL.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <process.h>
#include <map>
#include <string>
#include <ios>
#include <algorithm>
#include <vector>
#include <set>
#include <rpcdce.h>
#include "windef.h"
#include <conio.h>
#include <omp.h>

//##############################################
//# SCRIPT SYSTEM OPTIONS					   #
//##############################################
#define SCRIPT_ENABLED 0	// 1:YES | 0:NO

#if (SCRIPT_ENABLED == 1)

	#pragma comment(lib, "./RCZ_LUA/lua5.1.lib") 
	#include "./RCZ_LUA/lua.hpp"

#endif

// -----
/* Zodea GameServer Headers */
#include "SimpleModulus.H"
#include "Protection.h"
#include "TColors.H"
#include "MuMsg.H"
#include "ReadFileManager.H"
#include "GMManager.H"
#include "ChatHandler.H"
// -----
/* Protocol Headers */
#include "JoinProtocol.H"
// -----
using namespace std;
// -----
#pragma comment(lib , "rpcrt4.lib")
// -----
/* Customers Serial */
//#define Unk120		312821150	/* RCT-WolF */
//#define Unk120		435697218	/* RCT-M.E.S */
#define Unk120		163785440	/* RCT-EmaLeto */
//#define Unk120		237999980	/* Chris05 */
//#define Unk120		79754450	/* MasterBG */
//#define Unk120		49480075	/* Matt */
// -----
#define PROTO_KOREA		0
#define PROTO_JAPAN		1
#define PROTO_ENGLISH	2
#define PROTO_CHINA		3
// -----
#define SRV_PROTOCOL (PROTO_KOREA)
// -----
#if ( SRV_PROTOCOL == PROTO_KOREA )
#	define PMSG_BEATTACK_HEADCODE		0x10
#	define PMSG_MOVE_HEADCODE			0xD3
#	define PMSG_POSISTION_SET_HEADCODE	0xDF
#	define PMSG_ATTACK_HEADCODE			0xD7
// -----
#elif ( SRV_PROTOCOL == PROTO_JAPAN )
#	define PMSG_BEATTACK_HEADCODE		0xD7
#	define PMSG_MOVE_HEADCODE			0x1D
#	define PMSG_POSISTION_SET_HEADCODE	0xD6
#	define PMSG_ATTACK_HEADCODE			0xDC
// -----
#elif ( SRV_PROTOCOL == PROTO_ENGLISH )
#	define PMSG_BEATTACK_HEADCODE		0xDB
#	define PMSG_MOVE_HEADCODE			0xD4
#	define PMSG_POSISTION_SET_HEADCODE	0x11
#	define PMSG_ATTACK_HEADCODE			0x15 
// -----
#elif ( SRV_PROTOCOL == PROTO_CHINA )
#	define PMSG_BEATTACK_HEADCODE		0x1D
#	define PMSG_MOVE_HEADCODE			0xD9
#	define PMSG_POSISTION_SET_HEADCODE	0xD0
#	define PMSG_ATTACK_HEADCODE			0xD7
// -----
#endif