#include "stdafx.h"
#include "SkyEvent.h"
#include "Defines.h"

CSkyEvent gSkyEvent;

CSkyEvent::CSkyEvent()
{
	gSkyEvent.Init();
}

CSkyEvent::~CSkyEvent()
{

}

void CSkyEvent::Init()
{
	gSkyEvent.Enabled				= GetPrivateProfileInt("SkyEvent","Enabled",0,SKYEVENT_FILE_PATH);
}