//-----------------------------------------------------------------------------------------------------------

#include "version.h"
#include "sharednames.h"

#include "../../Libs/App/TApp.h"
#include "../../Libs/DataBase/TDataBase.h"
#include "../../libs/IOCP/TIOCP.h"
#include "../../Libs/Log/TLog.h"
#include "../../Libs/MyTools/TColors.h"
#include "../../libs/MyTools/TTimer.h"
#include "../../libs/MyTools/TMiniDumper.h"
#include "../../Libs/MyTools/TXmlReader.h"
#include "../../Libs/WzaTool/WzaTool.h"

#include "TSynchro.h"

//-----------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
	#pragma comment(lib, "../BoR/Debug/App.lib")
	#pragma comment(lib, "../BoR/Debug/IOCP.lib")
	#pragma comment(lib, "../BoR/Debug/WzaTool.lib")
	#pragma comment(lib, "../BoR/Debug/Log.lib")
	#pragma comment(lib, "../BoR/Debug/MyTools.lib")
	#pragma comment(lib, "../BoR/Debug/DataBase.lib")
	//#pragma comment(lib, "../BoR/Debug/libxml2.lib")
	#pragma comment(lib, "../lib/libxml2.lib")
#else
	#pragma comment(lib, "../BoR/Release/App.lib")
	#pragma comment(lib, "../BoR/Release/IOCP.lib")
	#pragma comment(lib, "../BoR/Release/WzaTool.lib")
	#pragma comment(lib, "../BoR/Release/Log.lib")
	#pragma comment(lib, "../BoR/Release/MyTools.lib")
	#pragma comment(lib, "../BoR/Release/DataBase.lib")
	//#pragma comment(lib, "../BoR/Debug/libxml2.lib")
	#pragma comment(lib, "../lib/libxml2.lib")
#endif

//#pragma comment(lib, "../lib/libxml2.lib")
//-----------------------------------------------------------------------------------------------------------