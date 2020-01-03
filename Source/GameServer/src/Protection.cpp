/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Protection Class For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#include "stdafx.h"
#include "Protection.H"
#include "GameMain.H"


UnkownC00 CUnknown;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
UnkownC00::UnkownC00(void)
{
	/*Unknown382();
	//// -----
	Unknown193();
	//// -----
	Unknown726();*/
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
UnkownC00::~UnkownC00(void)
{
	// -----
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
static char * Unknown648[Unk238] = 
{
	"[RCT] WolF",
	"[RCT] Maxi",
	"[RCT] EmaLeto",
	"Chris",
	"Gierek",
	"CrashOveride"
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void UnkownC00::Unknown872()
{
	/*Unknown382();
	//// -----
	Unknown726();*/
	//// -----
	Unknown592 = true;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool UnkownC00::Unknown193()
{
	IsMainFunction = true;
	// ------
	UuidCreateSequential( &Unk645 );
	// ------
    for (int U = 2 ; U < 8 ; U++)
	{
		Unk129[U - 2] = Unk645.Data4[U];
	}
	// ------
	wsprintfA(Unk291, "%02X-%02X-%02X-%02X-%02X-%02X", Unk129[0], Unk129[1], Unk129[2], Unk129[3], Unk129[4], Unk129[5]);
	// ------
	Unk147 = (((Unk129[1] * 9) + (Unk129[2] * 4) + (Unk129[3] * 6) * (Unk129[4] * 3) + (Unk129[5] * 8))) * 3;
	if (Unk147 <= 0) { Unk147 = Unk147 * -1; }
	// ------
	Unk148 = (((Unk129[1] * 5) + (Unk129[2] * 2) + (Unk129[3] * 7) * (Unk129[4] * 1) + (Unk129[5] * 4))) * 14;
	if (Unk148 <= 0) { Unk148 = Unk148 * -1; }
	//// ------
	Unk149 = (((Unk129[1] * 6) + (Unk129[2] * 1) + (Unk129[3] * 9) * (Unk129[4] * 3) + (Unk129[5] * 7))) * 25;
	if (Unk149 <= 0) { Unk149 = Unk149 * -1; }
	// ------
	Unk189	= ( ( (Unk147 + Unk148) * 35 ) ) + Unk149;
	if (Unk189 <= 0) { Unk189 = Unk189 * -1; }
	//// ------
	sprintf(Unk739, "Your Serial Number :: (%d) ", Unk189);
	// ------
	//MessageBoxA(NULL,Unk739,"Serial Key Generator!",MB_OK);
	// ------
	if(!Unknown172(Unk291))
	{
		Sleep(1000);
		// ------
		//MessageBox(NULL,"                              Contact Service Provider\n          You don't have license to use Zodea GameServer",
		MessageBox(NULL,"                              Contact Service Provider\n          You don't have license to use MuBK GameServer",
						"Error() Policy License",MB_OK);
		// ------
		ExitProcess(0);
		return true;
	}
	// ------
	else if( Unk189 != Unk120 )
	{
		Sleep(1000);
		// ------
		MessageBox(NULL,"                              Contact Service Provider\n          In Order to Register Your License & To Get The Serial.",
						//"Error() Zodea License",MB_OK);
						"Error() MuBK License",MB_OK);
		// ------
		ExitProcess(0);
		return true;
	}
	// -----
	return false;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool UnkownC00::Unknown172(LPCSTR Unk183)
{
	char Unk983[Unk238][18] =
	{
		"00-23-18-F0-D9-95",	// WolF	  ~ CENetwork © 2011
		"00-1F-D0-15-D2-6C",	// M.E.S  ~ CENetwork © 2011
		"00-E0-4D-B8-93-F5",	// Ema Leto ~ CENetwork © 2011
		"00-40-F4-9B-FF-9F",	// Chris
		"02-00-00-95-37-76",	// Mate Morello
		"18-03-73-92-18-82"		// CrashOveride
	};
	// ------
	if( Unk189 == Unk120 )
	{
		for(int Y = 0 ; Y < Unk238 ; Y++)
		{
			if(!strcmp ( Unk183 , Unk983[Y] ))
			{
				wsprintf(PRT_MSG,"Files are licensed to : %s", Unknown648[Y]);
				// -----
				if ( IsMainFunction == TRUE )
				{
					//if (MessageBox(NULL,PRT_MSG,"Zodea GameServer License", MB_YESNO|MB_APPLMODAL) == IDNO)
					if (MessageBox(NULL,PRT_MSG,"MuBK GameServer License", MB_YESNO|MB_APPLMODAL) == IDNO)
					{
						ExitProcess(0);
					}
				}
				// -----
				Unk449 = Unknown648[Y];
				// -----
				return true;
			}
		}
	}
	// ------
	return false;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL CALLBACK cEnum ( HWND hWnd , LPARAM p )
{
	AnimateWindow( hWnd , 0x1 , 0x90000  );
	// ------
	return TRUE;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void UnkownC00::Unknown382()
{
	POINT	p;
	Unk297	= 0;
	Unk349	= 0;
	// ------
	for ( qy = 0 ; qy < 100 ; qy++)
	{
		for ( qx = 0 ; qx < 100 ; qx++)
		{
			p.x = qx * 20;
			p.y = qy * 20;
			
			Unk297 = WindowFromPoint(p);

			GetWindowText( Unk297 , t , 255);

			if ( strstr( t , "Shadow") || strstr( t , "Olly") || strstr( t , "Debug") || strstr( t , "CPU - main") )
			{
				unsigned char * hack = (unsigned char*) GetProcAddress(GetModuleHandle("kernel32.dll") , "OpenProcess");

				if ( *(hack+6) == 0xEA )
				{
					//strcpy(txt , "Hide :: Debugger not worked! ... Don't try to Crack Zodea GameServer.");
					strcpy(txt , "Hide :: Debugger not worked! ... Don't try to Crack MuBK GameServer.");
				}
				else
				{
					//strcpy(txt, "Don't try to Crack Zodea GameServer.");
					strcpy(txt, "Don't try to Crack MuBK GameServer.");
				}


				LONG a = GetWindowLong ( Unk297 , GWL_HWNDPARENT );
				
				while ( a > 0 )
				{
					if ( a > 0 ) Unk297 = (HWND) a;
					a = GetWindowLong ( Unk297 , GWL_HWNDPARENT );
				}

				EnumChildWindows( Unk297 , cEnum , 0 );
				Rectangle ( GetDC(Unk297) , 0 , 0 , 2000 , 2000 );
				
				for ( int f = 0 ; f < 64 ; f++ )
				{
					TextOut( GetDC(Unk297) , 40 , 40 + (f*16) , txt , strlen(txt) ); 
				}
				
				SetWindowLong( Unk297 , GWL_STYLE , 0x0 );
				LockWindowUpdate( Unk297 );

				TOKEN_PRIVILEGES tp;
				HANDLE hToken;
				if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
				{
					LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
					tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					tp.PrivilegeCount = 1;
					AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
					
					CloseHandle(hToken);
				}
				Unk349++;				
			}
		}
	}
	// -----
	if ( Unk349 > 0 ) 
	// -----	
	ExitProcess ( 0 );
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void UnkownC00::Unknown726()
{
	IsMainFunction = false;
	// ------
	if( Unk189 != Unk120 ) ExitProcess(0);
	// ------
	if ( !Unknown172(Unk291) )	ExitProcess(0);
	// ------
	Unknown382();
}