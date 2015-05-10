# mu-bk-server-sourcecode
MuServer

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

The MuServerBK project is developed from the MuServerEx project.

Mu Server BK has been tested under the following systems on 32bit and 64bit installations but should also compile 
and run under any system where .NET 3.5 are available. By default Mu Server BK dont support native x64 compilations.

    * Windows XP SP3 (32bit)
    * Windows 8 (64bit)

Thanks

Mu Server BK says thanks to following people. If some is missing please contact us and the list will be updated.

    * [BOR] Sanek, Leo123
    * [RCT] Mesosa, Wolf, Emaleto
    * [MUBK] lordoftech

Contact
    * Gmail lelordoftech@gmail.com [ https://github.com/lelordoftech/mu-bk-server-sourcecode/ ]

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Introduction
Here you will learn how to setup MuServerBK.

Downloading & Setup
Please download Github

Step 1: Download Latest Source Version.

Step 2: Compile ConnectServer, DataServer, eDataServer & GameServer with Visual Studio 2010 Ultimate.

Step 3: Put Compiled exes in Compiled folder:
C:/MuServerBK/Source/BoR Team - CS DS JS/BoR/Release/ConnectServer.exe > C://MuServerBK/Compiled/ConnectServer
C:/MuServerBK/Source/DataServer/Release/DataServer.exe                 > C://MuServerBK/Compiled/DataServer
C:/MuServerBK/Source/DataServerBK/Release/eDataServer.exe              > C://MuServerBK/Compiled/DataServerBK
C:/MuServerBK/Source/GameServer/Release/GameServer.exe                 > C://MuServerBK/Compiled/GameServer

Step 4: Change information in Server

Open C:/MuServerBK/Compiled/DataServer/DataServer.conf   - Change SQL Password
Open C:/MuServerBK/Compiled/DataServerEx/eDataServer.ini - Change SQL Password
Open C:/MuServerBK/Compiled/Data/Local/MapServerInfo.dat - Change IP

Done!
