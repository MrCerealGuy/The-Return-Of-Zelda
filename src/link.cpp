/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: link.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"
#include "..\inc\portal.h"

ZeldaLink::ZeldaLink()
{
	pLiftObject = NULL;

	x = y = nAnim = 0; 

	link_dir = LINK_DOWN;
	bSwingSword = false;

	bStandInHighGras = false;
	nHighGrasCount   = 0;
	nHighGrasFrame   = 0;

	nCurrentLifes = 5;
	nMaxLifes     = 5;

	LinkRessources.pSurfRight[0]        = LoadSurface("dat\\gfx\\link1right.bmp");
	LinkRessources.pSurfLeft[0]         = LoadSurface("dat\\gfx\\link1left.bmp");
	LinkRessources.pSurfUp[0]           = LoadSurface("dat\\gfx\\link1up.bmp");
	LinkRessources.pSurfDown[0]         = LoadSurface("dat\\gfx\\link1down.bmp");
	LinkRessources.pSurfRight[1]        = LoadSurface("dat\\gfx\\link1right0.bmp");
	LinkRessources.pSurfLeft[1]         = LoadSurface("dat\\gfx\\link1left0.bmp");
	LinkRessources.pSurfUp[1]           = LoadSurface("dat\\gfx\\link1up0.bmp");
	LinkRessources.pSurfDown[1]         = LoadSurface("dat\\gfx\\link1down0.bmp");
	LinkRessources.pSurfLeftFight       = LoadSurface("dat\\gfx\\link1left_fight.bmp");
	LinkRessources.pSurfLeftSword[0]    = LoadSurface("dat\\gfx\\sword_l0.bmp");
	LinkRessources.pSurfLeftSword[1]    = LoadSurface("dat\\gfx\\sword_l1.bmp");
	LinkRessources.pSurfLeftSword[2]    = LoadSurface("dat\\gfx\\sword_l2.bmp");
	LinkRessources.pSurfLeftSword[3]    = LoadSurface("dat\\gfx\\sword_l3.bmp");
	LinkRessources.pSurfRightFight      = LoadSurface("dat\\gfx\\link1right_fight.bmp");
	LinkRessources.pSurfRightSword[0]   = LoadSurface("dat\\gfx\\sword_r0.bmp");
	LinkRessources.pSurfRightSword[1]   = LoadSurface("dat\\gfx\\sword_r1.bmp");
	LinkRessources.pSurfRightSword[2]   = LoadSurface("dat\\gfx\\sword_r2.bmp");
	LinkRessources.pSurfRightSword[3]   = LoadSurface("dat\\gfx\\sword_r3.bmp");
	LinkRessources.pSurfUpFight         = LoadSurface("dat\\gfx\\link1up_fight.bmp");
	LinkRessources.pSurfUpSword[0]      = LoadSurface("dat\\gfx\\sword_u0.bmp");
	LinkRessources.pSurfUpSword[1]      = LoadSurface("dat\\gfx\\sword_u1.bmp");
	LinkRessources.pSurfUpSword[2]      = LoadSurface("dat\\gfx\\sword_u2.bmp");
	LinkRessources.pSurfUpSword[3]      = LoadSurface("dat\\gfx\\sword_u3.bmp");
	LinkRessources.pSurfDownFight       = LoadSurface("dat\\gfx\\link1down_fight.bmp");
	LinkRessources.pSurfDownSword[0]    = LoadSurface("dat\\gfx\\sword_d0.bmp");
	LinkRessources.pSurfDownSword[1]    = LoadSurface("dat\\gfx\\sword_d1.bmp");
	LinkRessources.pSurfDownSword[2]    = LoadSurface("dat\\gfx\\sword_d2.bmp");
	LinkRessources.pSurfDownSword[3]    = LoadSurface("dat\\gfx\\sword_d3.bmp");
	LinkRessources.pSurfStandingGras[0] = LoadSurface("dat\\gfx\\gras5.bmp");
	LinkRessources.pSurfStandingGras[1] = LoadSurface("dat\\gfx\\gras6.bmp");

	LinkRessources.pSoundLift           = LoadSound("dat\\snd\\link\\lift.wav");
	LinkRessources.pSoundThrow          = LoadSound("dat\\snd\\link\\throw.wav");
	LinkRessources.pSoundSword          = LoadSound("dat\\snd\\sword\\stroke.wav");
	LinkRessources.pSoundHurt           = LoadSound("dat\\snd\\link\\hurt.wav");
	LinkRessources.pSoundHealthWarning  = LoadSound("dat\\snd\\link\\health_warning.wav");
	LinkRessources.pSoundStepGras       = LoadSound("dat\\snd\\step\\grass.wav");

	DEBUG("Link successfully loaded.\n");
}

ZeldaLink::~ZeldaLink()
{
	// Grafiken freigeben
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfRight[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfLeft[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfUp[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfDown[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfRight[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfLeft[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfUp[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfDown[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfRightFight);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfLeftFight);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfUpFight);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfDownFight);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfLeftSword[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfLeftSword[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfLeftSword[2]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfLeftSword[3]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfRightSword[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfRightSword[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfRightSword[2]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfRightSword[3]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfUpSword[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfUpSword[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfUpSword[2]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfUpSword[3]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfDownSword[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfDownSword[1]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfDownSword[2]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfDownSword[3]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfStandingGras[0]);
	Game->Screen->ReleaseSurfaceFromList(LinkRessources.pSurfStandingGras[1]);

	LinkRessources.pSurfRight[0] = LinkRessources.pSurfLeft[0]   = NULL;
	LinkRessources.pSurfUp[0]    = LinkRessources.pSurfDown[0]   = NULL;
	LinkRessources.pSurfRight[1] = LinkRessources.pSurfLeft[1]   = NULL;
	LinkRessources.pSurfUp[1]    = LinkRessources.pSurfDown[1]   = NULL;

	LinkRessources.pSurfStandingGras[0] = LinkRessources.pSurfStandingGras[1] = NULL;


	// Sound Buffer freigeben
	if (LinkRessources.pSoundLift != NULL)
		delete LinkRessources.pSoundLift;
	LinkRessources.pSoundLift  = NULL;
	
	if (LinkRessources.pSoundThrow != NULL)
		delete LinkRessources.pSoundThrow; 
	LinkRessources.pSoundThrow = NULL;

	if (LinkRessources.pSoundSword != NULL)
		delete LinkRessources.pSoundSword; 
	LinkRessources.pSoundSword = NULL;
	
	if (LinkRessources.pSoundHurt != NULL)
		delete LinkRessources.pSoundHurt; 
	LinkRessources.pSoundHurt  = NULL;

	if (LinkRessources.pSoundStepGras != NULL)
		delete LinkRessources.pSoundStepGras;
	LinkRessources.pSoundStepGras = NULL;

	if (LinkRessources.pSoundHealthWarning != NULL)
	{
		if (GetCurrentLifes() == 1)
			LinkRessources.pSoundHealthWarning->Stop();

		delete LinkRessources.pSoundHealthWarning; 
		LinkRessources.pSoundHealthWarning = NULL;
	}
}

bool ZeldaLink::SetLocation(const int x, const int y)
{
	if (!CheckSolid(x,y) && !CheckSolid(x+GetWidth(),y) &&
		!CheckSolid(x,y+GetHeight()) && !CheckSolid(x+GetWidth(),y+GetHeight()))
	{
		this->x = x;
		this->y = y;

		return true;
	}

	return false;
}

int ZeldaLink::GetWidth(void)
{
	switch (GetDirection())
	{
	case LINK_LEFT:
		return LinkRessources.pSurfLeft[0]->GetWidth();

	case LINK_RIGHT:
		return LinkRessources.pSurfRight[0]->GetWidth();
		
	case LINK_UP:
		return LinkRessources.pSurfUp[0]->GetWidth();
	
	case LINK_DOWN:
		return LinkRessources.pSurfDown[0]->GetWidth();
	}

	return 0;
}

int ZeldaLink::GetHeight(void)
{
	switch (GetDirection())
	{
	case LINK_LEFT:
		return LinkRessources.pSurfLeft[0]->GetHeight();

	case LINK_RIGHT:
		return LinkRessources.pSurfRight[0]->GetHeight();
		
	case LINK_UP:
		return LinkRessources.pSurfUp[0]->GetHeight();
	
	case LINK_DOWN:
		return LinkRessources.pSurfDown[0]->GetHeight();
	}

	return 0;
}

RECT ZeldaLink::GetRect(void)
{
	RECT rc;

	rc.left   = x;
	rc.top    = y;
	rc.right  = x+GetWidth();
	rc.bottom = y+GetHeight();

	return rc;
}

RECT ZeldaLink::GetSolidRect(void)
{
	static int width  = GetWidth();
	static int height = GetHeight();

	RECT rc;

	rc.left   = x+4;
	rc.top    = y+20;
	rc.right  = x+width-4;
	rc.bottom = y+height;

	return rc;
}

void ZeldaLink::UpdateSword(void)
{
	static int nFightCount = 0;
	static int nFightFrame = 0;

	if (!bSwingSword) return;

	CDXSurface *pSurf = NULL;

	RECT rc;
	rc.left   = 0; rc.top = 0;

	if (nFightFrame == 4)
	{
		bSwingSword = false;
		nFightCount = 0;
		nFightFrame = 0;
	}
	else 
	{
		nFightFrame++;
		nFightCount = 0;
	}

	// Zeichne Schwert
	if (bSwingSword && nFightFrame != 0)
	{
		switch (GetDirection())
		{
		case LINK_LEFT:
			if (LinkRessources.pSurfLeftSword[nFightFrame-1] != NULL)
			{
				rc.right  = LinkRessources.pSurfLeftSword[nFightFrame-1]->GetWidth();
				rc.bottom = LinkRessources.pSurfLeftSword[nFightFrame-1]->GetHeight();

				int x, y;
			
				switch(nFightFrame)
				{
				case 1:
					x = X_KOORD(GetXKoord())-10;
					y = Y_KOORD(GetYKoord());
				break;

				case 2:
				case 3:
					x = X_KOORD(GetXKoord())-20;
					y = Y_KOORD(GetYKoord())+17;
				break;

				case 4:
					x = X_KOORD(GetXKoord())-20;
					y = Y_KOORD(GetYKoord())+20;
				break;

				}

				LinkRessources.pSurfLeftSword[nFightFrame-1]->DrawTrans(Game->Screen->GetBack(),x, y, &rc);
			}
		break;

		case LINK_RIGHT:
			if (LinkRessources.pSurfRightSword[nFightFrame-1] != NULL)
			{
				rc.right  = LinkRessources.pSurfRightSword[nFightFrame-1]->GetWidth();
				rc.bottom = LinkRessources.pSurfRightSword[nFightFrame-1]->GetHeight();

				int x, y;
			
				switch(nFightFrame)
				{
				case 1:
					x = X_KOORD(GetXKoord())+20;
					y = Y_KOORD(GetYKoord());
				break;

				case 2:
				case 3:
					x = X_KOORD(GetXKoord())+30;
					y = Y_KOORD(GetYKoord())+17;
				break;

				case 4:
					x = X_KOORD(GetXKoord())+30;
					y = Y_KOORD(GetYKoord())+20;
				break;

				}

				LinkRessources.pSurfRightSword[nFightFrame-1]->DrawTrans(Game->Screen->GetBack(),x, y, &rc);
			}
		break;

		case LINK_UP:
			if (LinkRessources.pSurfUpSword[nFightFrame-1] != NULL)
			{
				rc.right  = LinkRessources.pSurfUpSword[nFightFrame-1]->GetWidth();
				rc.bottom = LinkRessources.pSurfUpSword[nFightFrame-1]->GetHeight();

				int x, y;
			
				switch(nFightFrame)
				{
				case 1:
					x = X_KOORD(GetXKoord())+27;
					y = Y_KOORD(GetYKoord())-2;
				break;

				case 2:
					x = X_KOORD(GetXKoord())+20;
					y = Y_KOORD(GetYKoord())-12;
				break;

				case 3:
					x = X_KOORD(GetXKoord());
					y = Y_KOORD(GetYKoord())-12;
				break;

				case 4:
					x = X_KOORD(GetXKoord())-20;
					y = Y_KOORD(GetYKoord())+2;
				break;

				}

				LinkRessources.pSurfUpSword[nFightFrame-1]->DrawTrans(Game->Screen->GetBack(),x, y, &rc);
			}
		break;

		case LINK_DOWN:
			if (LinkRessources.pSurfDownSword[nFightFrame-1] != NULL)
			{
				rc.right  = LinkRessources.pSurfDownSword[nFightFrame-1]->GetWidth();
				rc.bottom = LinkRessources.pSurfDownSword[nFightFrame-1]->GetHeight();

				int x, y;
			
				switch(nFightFrame)
				{
				case 1:
					x = X_KOORD(GetXKoord())-22;
					y = Y_KOORD(GetYKoord())+22;
				break;

				case 2:
					x = X_KOORD(GetXKoord())+10;
					y = Y_KOORD(GetYKoord())+35;
				break;

				case 3:
					x = X_KOORD(GetXKoord())+25;
					y = Y_KOORD(GetYKoord())+32;
				break;

				case 4:
					x = X_KOORD(GetXKoord())+20;
					y = Y_KOORD(GetYKoord())+25;
				break;

				}

				LinkRessources.pSurfDownSword[nFightFrame-1]->DrawTrans(Game->Screen->GetBack(),x, y, &rc);
			}
		break;
		}
	}
}

void ZeldaLink::Update(void)
{
	static int nStepSound = 0;

	RECT rc;
	rc.left = 0; rc.top = 0;

	CDXSurface *pSurf = NULL;

	switch (GetDirection())
	{
	case LINK_LEFT:
		if (bSwingSword)
			pSurf = LinkRessources.pSurfLeftFight;
		else
			pSurf = LinkRessources.pSurfLeft[nAnim];
	break;

	case LINK_RIGHT:
		if (bSwingSword)
			pSurf = LinkRessources.pSurfRightFight;
		else
			pSurf = LinkRessources.pSurfRight[nAnim];
	break;

	case LINK_UP:
		if (bSwingSword)
			pSurf = LinkRessources.pSurfUpFight;
		else
			pSurf = LinkRessources.pSurfUp[nAnim];
	break;	

	case LINK_DOWN:
		if (bSwingSword)
			pSurf = LinkRessources.pSurfDownFight;
		else
			pSurf = LinkRessources.pSurfDown[nAnim];
	break;
	}

	if (pSurf == NULL) return;

	rc.right  = pSurf->GetWidth();
	rc.bottom = pSurf->GetHeight();

	if (!Game->bMiniMap)
	{
		pSurf->DrawTrans(Game->Screen->GetBack(), X_KOORD(GetXKoord()), Y_KOORD(GetYKoord()), &rc);

		if (bStandInHighGras)
		{
			// Gras Animation wenn Link durch hohes Gras läuft
			if (LinkRessources.pSurfStandingGras[nHighGrasFrame] != NULL)
				LinkRessources.pSurfStandingGras[nHighGrasFrame]->DrawTrans(Game->Screen->GetBack(),
					X_KOORD(GetXKoord()), Y_KOORD(GetYKoord())+GetHeight()-10, &rc);

			if (LinkRessources.pSoundStepGras != NULL && nStepSound == 15) 
			{ 
				if (IsPlayerMoving())
					LinkRessources.pSoundStepGras->Play();
				nStepSound = 0;
			}
			else nStepSound++;
			
			bStandInHighGras = false;
		}
		
		if (pLiftObject != NULL)
		{
			// Chicken ?
			if (pLiftObject->GetModuleType() == MODULE_CHICKEN)
			{
				if (pLiftObject->GetModule() != NULL)
				{
					((ZeldaChickenModule*)pLiftObject->GetModule())->DoIdle(true);
				}
			}
			
			pLiftObject->SetLocation(GetXKoord(),GetYKoord());
		}
	}
	else
	{                                    
		if (Game->GetActiveWorld()->pMiniMap != NULL)
			pSurf->DrawTrans(Game->GetActiveWorld()->pMiniMap, GetXKoord(), GetYKoord(), &rc);
	}
}

bool ZeldaLink::CheckSolid(int x, int y)
{
	return ::CheckSolid(Game->GetActiveWorld(),x,y);
}

bool ZeldaLink::MoveRight(void)
{
	//if (bSwingSword)
	//	return false;

	// Läuft durch hohes Gras
	if (nHighGrasCount == 3)
	{
		if (++nHighGrasFrame == 2)
			nHighGrasFrame = 0;

		nHighGrasCount = 0;
	}
	else nHighGrasCount++;

	link_dir = LINK_RIGHT;
	int dx = 5;

	// Bewegungs-Anim. Frame
	static int nCount = 0;
	if (nCount == 3)
	{
		if (nAnim == 0)
			nAnim = 1;
		else
			nAnim = 0;

		nCount = 0;
	}
	else nCount++;

	int r = GetSolidRect().right, l = GetSolidRect().left;
	int t = GetSolidRect().top,   b = GetSolidRect().bottom;

	for (; (CheckSolid(r+dx, t) || CheckSolid(r+dx, b) || CheckSolid(r+dx,t+GetHeight()/2)) && dx > 0; ) 
	{
		dx--;
		
		// Ecke ausweichen
		if (dx == 0)
		{
			// obere Ecke
			if (!CheckSolid(r+10, t-GetHeight()/2) && !CheckSolid(l+GetWidth()/2, t-GetHeight()/2))
			{
				dx = 5; y -= dx;

				if (y < Game->GetActiveWorld()->GetWorldViewRect().bottom - ((Game->GetActiveWorld()->GetWorldViewRect().bottom-Game->GetActiveWorld()->GetWorldViewRect().top)/2))
					MoveWorldViewRectUp(dx);

				return true;
			}

			// untere Ecke
			if (!CheckSolid(r+5, b+GetHeight()/2) && !CheckSolid(l+GetWidth()/2,b+GetHeight()/2))
			{
				dx = 5; y += dx;

				if (y > Game->GetActiveWorld()->GetWorldViewRect().bottom - ((Game->GetActiveWorld()->GetWorldViewRect().bottom-Game->GetActiveWorld()->GetWorldViewRect().top)/2))		
					MoveWorldViewRectDown(dx);

				return true;
			}
		}
	}

	x += dx;

	if (x > Game->GetActiveWorld()->GetWorldViewRect().right - ((Game->GetActiveWorld()->GetWorldViewRect().right-Game->GetActiveWorld()->GetWorldViewRect().left)/2))
		MoveWorldViewRectRight(dx);

	return true;
}

bool ZeldaLink::MoveLeft(void)
{
	//if (bSwingSword)
	//	return false;

	// Läuft durch hohes Gras
	if (nHighGrasCount == 3)
	{
		if (++nHighGrasFrame == 2)
			nHighGrasFrame = 0;

		nHighGrasCount = 0;
	}
	else nHighGrasCount++;

	link_dir = LINK_LEFT;
	int dx = 5;

	// Bewegungs-Anim. Frame
	static int nCount = 0;
	if (nCount == 3)
	{
		if (nAnim == 0)
			nAnim = 1;
		else
			nAnim = 0;

		nCount = 0;
	}
	else nCount++;

	int r = GetSolidRect().right, l = GetSolidRect().left;
	int t = GetSolidRect().top,   b = GetSolidRect().bottom;

	for (; (CheckSolid(l-dx, t) || CheckSolid(l-dx, b) || CheckSolid(l-dx,t+GetHeight()/2)) && dx > 0; ) 
	{
		dx--;

		// Ecke ausweichen
		if (dx == 0)
		{
			// obere Ecke
			if (!CheckSolid(l-5, t-GetHeight()/2) && !CheckSolid(l+GetWidth()/2,t-GetHeight()/2))
			{
				dx = 5; y -= 5;

				if (y < Game->GetActiveWorld()->GetWorldViewRect().bottom - ((Game->GetActiveWorld()->GetWorldViewRect().bottom-Game->GetActiveWorld()->GetWorldViewRect().top)/2))
					MoveWorldViewRectUp(dx);

				return true;
			}

			// untere Ecke
			if (!CheckSolid(l-5, t+GetHeight()/2) && !CheckSolid(l+GetWidth()/2, b+GetHeight()/2))
			{
				dx = 5; y += dx;

				if (y > Game->GetActiveWorld()->GetWorldViewRect().bottom - ((Game->GetActiveWorld()->GetWorldViewRect().bottom-Game->GetActiveWorld()->GetWorldViewRect().top)/2))		
					MoveWorldViewRectDown(dx);

				return true;
			}
		}
	}

	x -= dx;

	if (x < Game->GetActiveWorld()->GetWorldViewRect().right - ((Game->GetActiveWorld()->GetWorldViewRect().right-Game->GetActiveWorld()->GetWorldViewRect().left)/2))
		MoveWorldViewRectLeft(dx);

	return true;
}

bool ZeldaLink::MoveUp(void)
{
	//if (bSwingSword)
	//	return false;

	// Läuft durch hohes Gras
	if (nHighGrasCount == 3)
	{
		if (++nHighGrasFrame == 2)
			nHighGrasFrame = 0;

		nHighGrasCount = 0;
	}
	else nHighGrasCount++;

	link_dir = LINK_UP;
	int dy = 5;

	// Bewegungs-Anim. Frame
	static int nCount = 0;
	if (nCount == 3)
	{
		if (nAnim == 0)
			nAnim = 1;
		else
			nAnim = 0;

		nCount = 0;
	}
	else nCount++;

	int r = GetSolidRect().right, l = GetSolidRect().left;
	int t = GetSolidRect().top,   b = GetSolidRect().bottom;

	for (; (CheckSolid(l, t-dy) || CheckSolid(r, t-dy) || CheckSolid(l+GetWidth()/2, t-dy)) && dy > 0; ) 
	{
		dy--;

		// Ecke ausweichen
		if (dy == 0)
		{
			// linke Ecke
			if (!CheckSolid(l-GetWidth()/2, t-5) && !CheckSolid(l-GetWidth()/2, t+GetHeight()/2))
			{
				dy = 5; x -= dy;

				if (x < Game->GetActiveWorld()->GetWorldViewRect().right - ((Game->GetActiveWorld()->GetWorldViewRect().right-Game->GetActiveWorld()->GetWorldViewRect().left)/2))
					MoveWorldViewRectLeft(dy);

				return true;
			}

			// rechte Ecke
			if (!CheckSolid(r+GetWidth(), t-5) && !CheckSolid(r+GetWidth()/2,t+GetHeight()/2))
			{
				dy = 5; x += dy;

				if (x > Game->GetActiveWorld()->GetWorldViewRect().right - ((Game->GetActiveWorld()->GetWorldViewRect().right-Game->GetActiveWorld()->GetWorldViewRect().left)/2))
					MoveWorldViewRectRight(dy);

				return true;
			}
		}
	}

	y -= dy;

	if (y < Game->GetActiveWorld()->GetWorldViewRect().bottom - ((Game->GetActiveWorld()->GetWorldViewRect().bottom-Game->GetActiveWorld()->GetWorldViewRect().top)/2))
		MoveWorldViewRectUp(dy);

	return true;
}

bool ZeldaLink::MoveDown(void)
{
	//if (bSwingSword)
	//	return false;

	// Läuft durch hohes Gras
	if (nHighGrasCount == 3)
	{
		if (++nHighGrasFrame == 2)
			nHighGrasFrame = 0;

		nHighGrasCount = 0;
	}
	else nHighGrasCount++;

	link_dir = LINK_DOWN;
	int dy = 5;

	// Bewegungs-Anim. Frame
	static int nCount = 0;
	if (nCount == 3)
	{
		if (nAnim == 0)
			nAnim = 1;
		else
			nAnim = 0;

		nCount = 0;
	}
	else nCount++;

	int r = GetSolidRect().right, l = GetSolidRect().left;
	int t = GetSolidRect().top,   b = GetSolidRect().bottom;

	for (; (CheckSolid(l, b+dy) || CheckSolid(r, b+dy) || CheckSolid(l+GetWidth()/2, b+dy)) && dy > 0; )
	{
		dy--;

		// Ecke ausweichen
		if (dy == 0)
		{
			// linke Ecke
			if (!CheckSolid(l-GetWidth()/2, b+5) && !CheckSolid(l-GetWidth()/2, t+GetHeight()/2))
			{
				dy = 5; x -= dy;

				if (x < Game->GetActiveWorld()->GetWorldViewRect().right - ((Game->GetActiveWorld()->GetWorldViewRect().right-Game->GetActiveWorld()->GetWorldViewRect().left)/2))
					MoveWorldViewRectLeft(dy);
				
				return true;
			}

			// rechte Ecke
			if (!CheckSolid(r+GetWidth()/2, b+5) && !CheckSolid(r+GetWidth()/2,t-GetHeight()/2))
			{
				dy = 5; x += 5;

				if (x > Game->GetActiveWorld()->GetWorldViewRect().right - ((Game->GetActiveWorld()->GetWorldViewRect().right-Game->GetActiveWorld()->GetWorldViewRect().left)/2))
					MoveWorldViewRectRight(dy);

				return true;
			}
		}
	}

	y += dy;

	if (y > Game->GetActiveWorld()->GetWorldViewRect().bottom - ((Game->GetActiveWorld()->GetWorldViewRect().bottom-Game->GetActiveWorld()->GetWorldViewRect().top)/2))		
		MoveWorldViewRectDown(dy);

	return true;
}

bool ZeldaLink::SwingSword(void)
{
	ZeldaObject* pObj = NULL;
	//int x, y;
	RECT rcSword;

	if (GetLiftObject() == NULL)
	{
		if (LinkRessources.pSoundSword != NULL) LinkRessources.pSoundSword->Play();

		switch(GetDirection())
		{
		case LINK_LEFT:
			rcSword.left   = GetXKoord() - GetWidth() / 2;
			rcSword.top    = GetYKoord();
			rcSword.right  = GetXKoord();
			rcSword.bottom = GetYKoord() + GetHeight(); 
		break;

		case LINK_RIGHT:
			rcSword.left   = GetXKoord() + GetWidth();
			rcSword.top    = GetYKoord();
			rcSword.right  = GetXKoord() + GetWidth() + GetWidth() / 2;
			rcSword.bottom = GetYKoord() + GetHeight();
		break;

		case LINK_UP:
			rcSword.left   = GetXKoord();
			rcSword.top    = GetYKoord() - GetHeight() / 2;
			rcSword.right  = GetXKoord() + GetWidth();
			rcSword.bottom = GetYKoord();
		break;

		case LINK_DOWN:
			rcSword.left   = GetXKoord();
			rcSword.top    = GetYKoord() + GetHeight();
			rcSword.right  = GetXKoord() + GetWidth();
			rcSword.bottom = GetYKoord() + GetHeight() + GetHeight() / 2;
		break;
		}

		for (int i = 0; i != Game->GetActiveWorld()->GetObjectList()->GetElementCount(); i++)
		{
			pObj = Game->GetActiveWorld()->GetObjectList()->GetElementByIndex(i);

			// Kann Schwert mit Objekt interagieren?
			if (pObj == NULL || (!pObj->IsBreakable() && !pObj->IsChicken() && !pObj->IsSoldier()))
				continue;
		
			if (RectInRect(rcSword,pObj->GetRect()))
			//if (PointInRect(x,y,pObj->GetRect()))
			{
				if (pObj->IsBreakable())
					pObj->BreakIt();
				else if (pObj->IsChicken())
					((ZeldaChickenModule*)pObj->GetModule())->Hurt();
				else if (pObj->IsSoldier())
					((ZeldaSoldierModule*)pObj->GetModule())->Hurt();

				break;
			}
		}

		return (bSwingSword = true);
	}
	else
		return (bSwingSword = false);
}

bool ZeldaLink::ThrowObject(void)
{
	int x, y;

	if (pLiftObject == NULL)
		return false;

	switch(GetDirection())
	{
		case LINK_LEFT:
			x = GetXKoord()-pLiftObject->GetWidth()-10;
			y = GetYKoord();

			pLiftObject->SetLocation(x,y);
		break;

		case LINK_RIGHT:
			x = GetXKoord()+GetWidth()+10;
			y = GetYKoord();

			pLiftObject->SetLocation(x,y);
		break;

		case LINK_UP:
			x = GetXKoord();
			y = GetYKoord()-pLiftObject->GetHeight()-10;

			pLiftObject->SetLocation(x,y);
		break;

		case LINK_DOWN:
			x = GetXKoord();
			y = GetYKoord()+GetHeight()+10;
				
			pLiftObject->SetLocation(x,y);
		break;
	}

	pLiftObject->BreakIt();

	if (pLiftObject->GetModuleType() != MODULE_CHICKEN)
		if (LinkRessources.pSoundThrow != NULL) LinkRessources.pSoundThrow->Play();
	else
		((ZeldaChickenModule*)pLiftObject->GetModule())->DoIdle(false);


	pLiftObject = NULL;

	return true;

}

bool ZeldaLink::LiftObject(void)
{
	ZeldaObject* pObj = NULL;
	int x, y, index;

    switch(GetDirection())
	{
	case LINK_LEFT:
		x = GetXKoord()-20;
		y = GetYKoord()+(GetHeight()/2);
	break;

	case LINK_RIGHT:
		x = GetXKoord()+GetWidth()+20;
		y = GetYKoord()+(GetHeight()/2);
	break;

	case LINK_UP:
		x = GetXKoord()+(GetWidth()/2);
		y = GetYKoord()+20;
	break;

	case LINK_DOWN:
		x = GetXKoord()+(GetWidth()/2);
		y = GetYKoord()+GetHeight()+20;
	break;
	}

	for (index = 0; index != Game->GetActiveWorld()->GetObjectList()->GetElementCount(); index++)
	{
		pObj = Game->GetActiveWorld()->GetObjectList()->GetElementByIndex(index);

		if (pObj == NULL || !pObj->IsLiftable())
			continue;
		
		if (PointInRect(x,y,pObj->GetRect()))
		{
			pLiftObject = pObj;
			
			if (pObj->GetModule() == NULL)
				if (LinkRessources.pSoundLift != NULL) LinkRessources.pSoundLift->Play();
			
			return true;
		}
	}
	
	return false;
}

void ZeldaLink::Hurt(void)
{
	if (LinkRessources.pSoundHurt != NULL) LinkRessources.pSoundHurt->Play();

	SetCurrentLifes(GetCurrentLifes()-1);
}

void ZeldaLink::SetCurrentLifes(int lifes)
{
	nCurrentLifes = lifes;

	if (lifes == 1)
	{
		if (LinkRessources.pSoundHealthWarning != NULL)
			LinkRessources.pSoundHealthWarning->Play(DSBPLAY_LOOPING);
	}
	else if (lifes > 1)
	{
		if (LinkRessources.pSoundHealthWarning != NULL)
			LinkRessources.pSoundHealthWarning->Stop();
	}
	else nCurrentLifes = 0;
}
