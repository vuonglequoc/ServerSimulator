//=====================================================================================
//Project: MMTeam - Season II - Emulator
//Version: GameServer 2.00.05
//Information: ZZZPATH reworked fixing memory leaks
//=====================================================================================
#include "stdafx.h"
#include "zzzpath.h"

PATH::PATH()
: HitMap(0)
, Map(0)
, Width(0)
, Height(0)
, NumPath(0)
, LastDir(0)
, NumFails(0)
, stx(0)
, sty(0)
, edx(0)
, edy(0)
{
	// Initialise the dir
	int dir[16] = {-1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0} ;
	memcpy(this->Dir, dir, sizeof(dir));
}

PATH::~PATH()
{
	if (HitMap)
	{
		delete[] HitMap;
		HitMap = 0;
	}
}

bool PATH::FindPath(int startx, int starty, int endx,  int endy, bool ErrorCheck)
{
	this->NumPath = 0;

	int WhichDir = 0;
	int NumFails = 0;
	bool First;

	memset(this->HitMap, 0, this->Width * this->Height);
	First = true;

	while ((startx != endx) || (starty != endy))
	{
		WhichDir = this->FindNextDir(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
		First = 0;
		
		if (WhichDir >= 0)
		{
			this->PathX[this->NumPath] = startx;
			this->PathY[this->NumPath] = starty;
			startx += this->Dir[WhichDir*2];
			starty += this->Dir[WhichDir*2+1];
			this->NumPath++;

			if (this->NumPath >= 15)
			{
				return false;
			}
		}
		else if (ErrorCheck != false)
		{
			this->NumPath--;

			if (this->NumPath < 0)
			{
				return false;
			}

			startx = this->PathX[this->NumPath];
			starty = this->PathY[this->NumPath];
			NumFails++;

			if (NumFails >= 10)
			{
				return false;
			}
		}
		else
		{
			break;
		}
	}

	this->PathX[this->NumPath] = startx;
	this->PathY[this->NumPath] = starty;
	this->NumPath++;

	if (this->NumPath >= 15)
	{
		return false;
	}

	return true;
}

bool PATH::FindPath2(int startx, int starty, int endx,  int endy, bool ErrorCheck)
{
	this->NumPath = 0;

	int WhichDir = 0;
	int NumFails = 0;
	bool First;

	memset(this->HitMap, 0, this->Width * this->Height);
	First = true;

	while ((startx != endx) || (starty != endy))
	{
		WhichDir = this->FindNextDir2(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
		First = 0;
		
		if (WhichDir >= 0)
		{
			this->PathX[this->NumPath] = startx;
			this->PathY[this->NumPath] = starty;
			startx += this->Dir[WhichDir*2];
			starty += this->Dir[WhichDir*2+1];
			this->NumPath++;

			if (this->NumPath >= 14)
			{
				this->NumPath--;
				break;
			}
		}
		else if (ErrorCheck != FALSE)
		{
			this->NumPath--;

			if (this->NumPath < 0)
			{
				return false;
			}

			startx = this->PathX[this->NumPath];
			starty = this->PathY[this->NumPath];
			NumFails++;

			if (NumFails >= 10)
			{
				return false;
			}
		}
		else
		{
			break;
		}
		
	}

	this->PathX[this->NumPath] = startx;
	this->PathY[this->NumPath] = starty;
	this->NumPath++;

	if (this->NumPath >= 15)
	{
		return false;
	}

	return true;
}

bool PATH::FindPath3(int startx, int starty, int endx,  int endy, bool ErrorCheck)
{
	this->NumPath = 0;

	int WhichDir = 0;
	int NumFails = 0;
	bool First = true;

	memset(this->HitMap, 0, this->Width * this->Height);

	while ((startx != endx) || (starty != endy))
	{
		WhichDir = this->FindNextDir3(startx, starty, endx, endy, WhichDir, First, ErrorCheck);
		First = 0;
		
		if (WhichDir >= 0)
		{
			this->PathX[this->NumPath] = startx;
			this->PathY[this->NumPath] = starty;
			startx += this->Dir[WhichDir*2];
			starty += this->Dir[WhichDir*2+1];
			this->NumPath++;

			if (this->NumPath >= 14)
			{
				this->NumPath--;
				break;
			}
		}
		else if (ErrorCheck != FALSE)
		{
			this->NumPath--;

			if (this->NumPath < 0)
			{
				return false;
			}

			startx = this->PathX[this->NumPath];
			starty = this->PathY[this->NumPath];
			NumFails++;

			if (NumFails >= 10)
			{
				return false;
			}
		}
		else
		{
			break;
		}
		
	}

	this->PathX[this->NumPath] = startx;
	this->PathY[this->NumPath] = starty;
	this->NumPath++;

	if (this->NumPath >= 15)
	{
		return false;
	}

	return true;
}

void PATH::SetMapDimensions(int w, int h, unsigned char* map)
{
	this->Width = w;
	this->Height = h;
	this->Map = map;
	this->HitMap = new unsigned char[this->Width * this->Height];
}

int PATH::GetPath()
{
	return this->NumPath;
}

unsigned char* PATH::GetPathX()
{
	return this->PathX;
}

unsigned char* PATH::GetPathY()
{
	return this->PathY;
}