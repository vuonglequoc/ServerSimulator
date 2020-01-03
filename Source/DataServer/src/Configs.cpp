#include "StdAfx.H"
#include "Configs.H"
#include "Main.H"
#include "LogProc.H"
#include "ReadFileManager.H"

MainConfig MCfg;

void MainConfig::Initialize()
{
	this->ReadMainInfo	( RFile.szRFileName[0] );
}
void MainConfig::ReadMainInfo( LPSTR szFileName )
{
	CLog.Load("DataServer Emulator Configuration Loading...");

	RFile.GetString ( gMainApp.MSTR.SQL_Host,  "Database",	"Server",	szFileName );
	RFile.GetString ( gMainApp.MSTR.SQL_DB,	   "Database",	"DataBase",		szFileName );
	RFile.GetString ( gMainApp.MSTR.SQL_User,  "Database",	"Username",		szFileName );
	RFile.GetString ( gMainApp.MSTR.SQL_Pass,  "Database",	"Password",		szFileName );

	gMainApp.MSTR.JoinSrv_UseMD5	= RFile.GetInt ( "Server",	"MD5Enabled",		szFileName );
	gMainApp.MSTR.DataSrv_Count		= RFile.GetInt ( "Server",	"DataServerCount",	szFileName );
	gMainApp.MSTR.JoinSrv_Port		= RFile.GetInt ( "Server",	"JoinServerPort",	szFileName );
	gMainApp.MSTR.DataSrv_Port		= RFile.GetInt ( "Server",	"DataServerPort",	szFileName );

}
