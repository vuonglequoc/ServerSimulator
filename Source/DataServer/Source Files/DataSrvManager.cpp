#include "../Header Files/StdAfx.H"


DServerManager DS_SrvManager[];

void DServerManager::OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	DSProtocolCore(this, aIndex, HeadCode , aRecv , iSize);
}