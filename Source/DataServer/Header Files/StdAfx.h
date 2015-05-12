/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [08.08.2011] This Script For Pre-Compiled Type Information.
#
#############################################################################*/
#pragma once
// -----
#define		WINVER			0x0500
#define		_WIN32_WINNT	0x0500
// -----
#define		_CRT_SECURE_NO_WARNINGS
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
#include <conio.h>
#include <SQL.h>
#include <sqlext.h>
#include <assert.h>
#include <fstream>
#include <SDKDDKVer.h>
// -----
using namespace std;

#include "Resource.H"
#include "Defines.H"
#include "Colors.H"
#include "GPackets.H"
#include "JoinProtocol.H"
#include "Structure.H"
#include "TSync.H"
#include "IOCP.H"
#include "Functions.H"
#include "JoinSrvManager.H"
#include "DataSrvManager.H"
#include "JSProtocol.H"
#include "DSProtocol.H"
// -----
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "rpcrt4.lib")
// -----