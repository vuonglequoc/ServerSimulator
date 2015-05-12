//=====================================================================================
//Project: MMTeam - Season II - Emulator
//Version: GameServer 2.00.05
//Information: ZZZPATH reworked fixing memory leaks
//=====================================================================================
#ifndef ZZZPATH_H
#define ZZZPATH_H

struct PATH_t
{
	int PathNum;
	int PathX[15];
	int PathY[15];
	int CurrentPath;
};

class PATH
{
private:

	int GetDist(int x1, int y1, int x2, int y2) // line : 20
	{
		int distx = x2 - x1;
		int disty = y2 - y1;
		return (distx * distx) + (disty * disty);
	}	// line : 23

	int VerifyThatOnPath(int x, int y)
	{
		for ( int i = 0 ; i < this->NumPath ; i++ )
		{
			if ( x ==  this->PathX[i] && y == this->PathY[i] )
			{
				return i;
			}
		}
		return -1;
	}

	BOOL CanWeMoveForward(int x, int y) // line : 33
	{
		int which = x+ y * this->Width  ;
		
		if ( x >= this->Width || x < 0 || y >= this->Height || y < 0 )
		{
			return false;
		}

		if ( this->Map[which] > 1 || this->HitMap[which] != 0 )
		{
			this->HitMap[which] = 1;
			return false;
		}
		return TRUE;
	};	// line : 43

	BOOL CanWeMoveForward2(int x, int y) // line : 46
	{
		int which = x +  y* this->Width ;
		
		if ( x >= this->Width || x < 0 || y >= this->Height || y < 0 )
		{
			return false;
		}

		if ( this->Map[which] >= 1 || this->HitMap[which] != 0 )
		{
			this->HitMap[which] = 1;
			return false;
		}
		return TRUE;
	};
	
	BOOL CanWeMoveForward3(int x, int y) // line : 46
	{
		int which = x +  y* this->Width ;
		
		if ( x >= this->Width || x < 0 || y >= this->Height || y < 0 )
		{
			return false;
		}

		if ( this->Map[which] >= 1 || this->HitMap[which] != 0 )
		{
			this->HitMap[which] = 1;
			return false;
		}
		return TRUE;
	};

	// line : 56


	BOOL IsThisSpotOK(int x, int y) // line : 59
	{
		int pos;

		pos = x+ y * this->Width ;

		if ( x < 0 || x >= this->Width || y < 0 || y >= this->Width )	// Make Deathway fix here maybe
		{
			return false;
		}
		if ( this->HitMap[pos] != 0 )
		{
			return false;
		}
		return true;
	};	// line : 68

private:

	int FindNextDir(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // line : 70
	{
		int MinDist = 10000000;
		long ldir;
		int WhichDir = 0;
		int i;

		if ( First != 0 )
		{
			for ( i = 0 ; i<8 ; i++ )
			{
				ldir = i % 8;
				int endx = sx + this->Dir[ldir*2];
				int endy = sy + this->Dir[ldir*2+1];
				int dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
		else
		{
			for ( i = dirstart+7 ; i <= dirstart+9 ; i++ )
			{
				ldir = i % 8;
				int endx = sx + this->Dir[ldir*2];
				int endy = sy + this->Dir[ldir*2+1];
				int dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}

			if ( MinDist == 10000000 )
			{
				for ( i = dirstart+2 ; i <= dirstart+6 ; i++ )
				{
					ldir = i % 8;
					int endx = sx + this->Dir[ldir*2];
					int endy = sy + this->Dir[ldir*2+1];
					int dist = this->GetDist(endx, endy, dx, dy);

					if ( MinDist > dist )
					{
						if ( this->IsThisSpotOK(endx, endy) != FALSE )
						{
							MinDist = dist;
							WhichDir = ldir;
						}
					}
				}
			}
		}
		int LastDist;

		this->LastDir = dirstart;
		LastDist = this->GetDist(sx, sy, sx + this->Dir[this->LastDir*2], sy + this->Dir[this->LastDir*2+1]);

		if ( ( LastDist > MinDist - 6 ) && ( LastDist < MinDist + 6 ) && ( MinDist >= 25 ) )
		{
			WhichDir = this->LastDir;
		}

		if ( MinDist == 10000000 )
		{
			return -1;
		}
		
		if ( this->CanWeMoveForward(sx+this->Dir[((WhichDir*2))], sy+this->Dir[((WhichDir*2))+1]) != FALSE )
		{
			int path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->HitMap[sx + sy * this->Width ] = 1;
				this->NumPath = path;
			}

			return WhichDir;
		}

		if ( ErrorCheck != 0 )
		{
			int dir = WhichDir+7;

			dir %= 8;

			if (this->CanWeMoveForward(sx + this->Dir[dir*2], sy + this->Dir[dir*2+1]) != FALSE )
			{
				int path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx  + sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir;
			}

			int dir2 = WhichDir+9;
			dir2 %= 8;
			if (CanWeMoveForward(sx+this->Dir[dir2*2], sy+this->Dir[dir2*2+1]) != FALSE )
			{
				int path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx + sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir2;
			}
		}
		
		this->HitMap[sx + sy*this->Width]=1;
		return -1;
	};	// line : 155
		
	int FindNextDir2(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // line : 158
	{
		int MinDist = 10000000;
		long ldir;
		int WhichDir = 0;
		int i;

		if ( First != 0 )
		{
			for ( i = 0 ; i<8 ; i++ )
			{
				ldir = i % 8 ;
				int endx = sx + this->Dir[ldir*2];
				int endy = sy + this->Dir[ldir*2+1];
				int dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
		else
		{
			for ( i = dirstart+7 ; i <= dirstart+9 ; i++ )
			{
				ldir = i % 8;
				int endx = sx + this->Dir[ldir*2];
				int endy = sy + this->Dir[ldir*2+1];
				int dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE )
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}

			if ( MinDist == 10000000 )
			{
				for ( i = dirstart+2 ; i <= dirstart+6 ; i++ )
				{
					ldir = i % 8;
					int endx = sx + this->Dir[ldir*2];
					int endy = sy + this->Dir[ldir*2+1];
					int dist = this->GetDist(endx, endy, dx, dy);

					if ( MinDist > dist )
					{
						if ( this->IsThisSpotOK(endx, endy) != FALSE )
						{
							MinDist = dist;
							WhichDir = ldir;
						}
					}
				}
			}
		}
		int LastDist;

		this->LastDir = dirstart;
		LastDist = this->GetDist(sx, sy, sx + this->Dir[this->LastDir*2], sy + this->Dir[this->LastDir*2+1]);

		if ( ( LastDist > MinDist - 6 ) && ( LastDist < MinDist + 6 ) && ( MinDist >= 25 ) )
		{
			WhichDir = this->LastDir;
		}

		if ( MinDist == 10000000 )
		{
			return -1;
		}
		
		if ( this->CanWeMoveForward2(sx+this->Dir[WhichDir*2],sy+ this->Dir[WhichDir*2+1]) != FALSE )
		{
			int path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->HitMap[sx+ sy * this->Width ] = 1;
				this->NumPath = path;
			}

			return WhichDir;
		}

		if ( ErrorCheck != 0 )
		{
			int dir = WhichDir+7;

			dir %= 8;

			if (this->CanWeMoveForward2(sx + this->Dir[dir*2], sy + this->Dir[dir*2+1]) != FALSE )
			{
				int path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx+ sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir;
			}

			int dir2 = WhichDir+9;
			dir2 %= 8;

			if (CanWeMoveForward2(sx+this->Dir[dir2*2], sy+this->Dir[dir2*2+1]) != FALSE )
			{
				int path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx + sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir2;
			}
		}
		
		this->HitMap[sx + sy*this->Width]=1;
		return -1;
	};
	
	int FindNextDir3(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // line : 158
	{
		int MinDist = 10000000;
		long ldir;
		int WhichDir = 0;
		int i;

		if ( First != 0 )
		{
			for ( i = 0 ; i<8 ; i++ )
			{
				ldir = i % 8 ;
				int endx = sx + this->Dir[ldir*2];
				int endy = sy + this->Dir[ldir*2+1];
				int dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE && this->CanWeMoveForward3(endx, endy) != FALSE)
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
		else
		{
			for ( i = dirstart+7 ; i <= dirstart+9 ; i++ )
			{
				ldir = i % 8;
				int endx = sx + this->Dir[ldir*2];
				int endy = sy + this->Dir[ldir*2+1];
				int dist = this->GetDist(endx, endy, dx, dy);

				if ( MinDist > dist )
				{
					if ( this->IsThisSpotOK(endx, endy) != FALSE && this->CanWeMoveForward3(endx, endy) != FALSE)
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}

			if ( MinDist == 10000000 )
			{
				for ( i = dirstart+2 ; i <= dirstart+6 ; i++ )
				{
					ldir = i % 8;
					int endx = sx + this->Dir[ldir*2];
					int endy = sy + this->Dir[ldir*2+1];
					int dist = this->GetDist(endx, endy, dx, dy);

					if ( MinDist > dist )
					{
						if ( this->IsThisSpotOK(endx, endy) != FALSE && this->CanWeMoveForward3(endx, endy) != FALSE)
						{
							MinDist = dist;
							WhichDir = ldir;
						}
					}
				}
			}
		}
		
		if ( MinDist == 10000000 )
		{
			return -1;
		}
		
		if ( this->CanWeMoveForward3(sx+this->Dir[WhichDir*2],sy+ this->Dir[WhichDir*2+1]) != FALSE )
		{
			int path = this->VerifyThatOnPath(sx, sy);

			if ( path != -1 )
			{
				this->HitMap[sx+ sy * this->Width ] = 1;
				this->NumPath = path;
			}

			return WhichDir;
		}

		if ( ErrorCheck != 0 )
		{
			int dir = WhichDir+7;

			dir %= 8;

			if (this->CanWeMoveForward3(sx + this->Dir[dir*2], sy + this->Dir[dir*2+1]) != FALSE )
			{
				int path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx+ sy * this->Width] = 1;
					this->NumPath = path;
				}
				return dir;
			}

			int dir2 = WhichDir+9;
			dir2 %= 8;

			if (CanWeMoveForward3(sx+this->Dir[dir2*2], sy+this->Dir[dir2*2+1]) != FALSE )
			{
				int path = this->VerifyThatOnPath(sx, sy);

				if ( path != -1 )
				{
					this->HitMap[sx + sy * this->Width] = 1;
					this->NumPath = path;
				}

				return dir2;
			}
		}
		
		this->HitMap[sx + sy*this->Width]=1;
		return -1;
	};

	// line : 245

public:
	PATH();
	~PATH();

	bool FindPath(int startx, int starty, int endx,  int endy, bool ErrorCheck);
	bool FindPath2(int startx, int starty, int endx,  int endy, bool ErrorCheck);
	bool FindPath3(int startx, int starty, int endx,  int endy, bool ErrorCheck);
	void SetMapDimensions(int w, int h, unsigned char* map);

	int GetPath();
	unsigned char* GetPathX();
	unsigned char* GetPathY();

private:
	int Width;
	int Height;
	int NumPath;
	BYTE PathX[500];
	BYTE PathY[500];
	int Dir[16];
	int LastDir;
	int NumFails;
	unsigned char* Map;
	unsigned char* HitMap;
	int stx;
	int sty;
	int edx;
	int edy;
};

#endif // ZZZPATH_H_