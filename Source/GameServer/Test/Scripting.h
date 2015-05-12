#include "../Header Files/StdAfx.h"

#if (SCRIPT_ENABLED == 1)

void ReportErrors(lua_State *L, int status);
void RegisterBasicFunctions(lua_State *L);
void RegisterPlayerFunctions(lua_State *L);
void ExecCommonFile(char *File);

#endif