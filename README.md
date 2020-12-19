# Server Simulator

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

This project is developed from the MuServerEx project.

Server has been tested under the following systems on 32bit and 64bit installations but should also compile and 
run under any system where .NET 3.5 are available. By default Server dont support native x64 compilations.

    * Windows XP SP3 (32bit)
    * Windows 8 (64bit)

Thanks to following people. If some is missing please contact us and the list will be updated.

    * [BOR] Sanek, Leo123
    * [RCT] Mesosa, Wolf, Emaleto
    * [MUBK] lordoftech

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Introduction
Here you will learn how to setup Server.

Downloading & Setup
Please download Github

Step 1: Download Latest Source Version.

Step 2: Compile with Visual Studio 2010 Ultimate.

    * ConnectServer
    * DataServer
    * eDataServer
    * GameServer

Step 3: Put Compiled exes in Compiled folder:

    * C:/Source/BoR Team - CS DS JS/BoR/Release/ConnectServer.exe > C:/Compiled/ConnectServer
    * C:/Source/DataServer/Release/DataServer.exe                 > C:/Compiled/DataServer
    * C:/Source/DataServerBK/Release/eDataServer.exe              > C:/Compiled/DataServerBK
    * C:/Source/GameServer/Release/GameServer.exe                 > C:/Compiled/GameServer

Step 4: Change information in Server

    * Open C:/Compiled/DataServer/DataServer.conf   - Change SQL Password
    * Open C:/Compiled/DataServerEx/eDataServer.ini - Change SQL Password
    * Open C:/Compiled/Data/Local/MapServerInfo.dat - Change IP

Done!
