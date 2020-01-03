//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Enterance NPC Structure & Functions			//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_ENTERANCE_NPC_H
#define KANTURU_ENTERANCE_NPC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKanturuEntranceNPC
{
public:
  CKanturuEntranceNPC();
  virtual ~CKanturuEntranceNPC();
  void NotifyEntranceInfo(int iIndex);
  void NotifyResultEnterKanturuBossMap(int iIndex);
};
extern CKanturuEntranceNPC g_KanturuEntranceNPC;


#endif