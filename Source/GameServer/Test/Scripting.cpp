#include "../Header Files/StdAfx.h"
#include "Scripting.h"

#if (SCRIPT_ENABLED == 1)

//########################################################
//# LoadLibraryA(LibName,CoreFunc)
//########################################################
int ScriptLoadLibraryA(lua_State *L)
{
	char Temp[200] = {0};
	sprintf(Temp,"../data/Addon/%s",lua_tostring(L,1));
	HMODULE dll = LoadLibraryA(Temp);
	FARPROC ProcAddress = GetProcAddress(dll,lua_tostring(L,2));
	if(ProcAddress != 0)
		ProcAddress();
	lua_pushnumber(L,(int)ProcAddress);
	return 1;
}
//########################################################
//# ExitProcess()
//########################################################
int ScriptExitProcess(lua_State *L)
{
	ExitProcess(0);
	return 1;
}
//########################################################
//# MessageBoxYesNo(Message,Title)
//# Responses:
//#	 - YES: 6
//########################################################
int ScriptMessageBoxYesNo(lua_State *L)
{
	int Result = MessageBoxA(0,lua_tostring(L,1),lua_tostring(L,2),MB_YESNO | MB_ICONQUESTION);
	lua_pushnumber(L,Result);
	return 1;
}
//########################################################
//# MessageBoxA(Message,Title)
//########################################################
int ScriptMessageBoxA(lua_State *L)
{
	MessageBoxA(0,lua_tostring(L,1),lua_tostring(L,2),MB_OK);
	return 0;
}
//########################################################
//# Sleep(inMiliSec)
//########################################################
int ScriptSleep(lua_State *L)
{
	Sleep(lua_tonumber(L,1));
	return 0;
}
//########################################################
//# SetByte(Offset,Value)
//#	Standart Values:
//#		- JNZ: 0x75
//#		- JE: 0x74
//#		- JMP: 0xEB
//#		- RETN: 0xC3
//########################################################
int ScriptSetByte(lua_State *L)
{
	*(BYTE*)((BYTE)lua_tonumber(L,1)) = (BYTE)lua_tonumber(L,2);
	return 0;
}
//########################################################
//# SetNop(Offset,Size)
//########################################################
int ScriptSetNop(lua_State *L)
{
	for(int n = 0; n < (int)lua_tonumber(L,2); n++)
	{
		*(BYTE*)((BYTE)lua_tonumber(L,1)+n) = 0x90;
	}
	return 0;
}

//########################################################
//# ScriptExec(File)
//########################################################
int ScriptExec(lua_State *L)
{
	ExecCommonFile((char*)lua_tostring(L, -1));
	return 0;
}

void ReportErrors(lua_State *L, int status)
{
  if (status !=0) 
  {
    MessageBoxA(0,lua_tostring(L, -1),"RC Scripting Error",MB_OK);
    lua_pop(L, 1);
  }
}

void RegisterBasicFunctions(lua_State *L)
{
	lua_register(L,"LoadLibraryA",			ScriptLoadLibraryA);
	lua_register(L,"ExitProcess",			ScriptExitProcess);
	lua_register(L,"Sleep",					ScriptSleep);

	lua_register(L,"MessageBoxYesNo",		ScriptMessageBoxYesNo);
	lua_register(L,"MsgBox",				ScriptMessageBoxA);

	lua_register(L,"SetByte",				ScriptSetByte);
	lua_register(L,"SetNop",				ScriptSetNop);

	lua_register(L,"ScriptExec",			ScriptExec);
}

void RegisterPlayerFunctions(lua_State *L)
{
}

void ExecCommonFile(char *File) // WORK
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	RegisterBasicFunctions(L);
	int s = luaL_loadfile(L, File);
	if (s == 0) 
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	ReportErrors(L, s);
	lua_close(L);
}

#endif