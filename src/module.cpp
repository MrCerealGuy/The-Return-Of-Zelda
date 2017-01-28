/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: module.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

//////////////////////////////////
// Implementierung von ZeldaModule
//////////////////////////////////

ZeldaModule::ZeldaModule()
{
	pObj = NULL;
}

ZeldaModule::~ZeldaModule()
{

}

void ZeldaModule::Update(void)
{

}

/*
 * CheckSolid
 *
 * Die Welt besteht nun mal aus festen Objekten, so dass das kleine Huhn nicht überall hingehen
 * kann. Man kann es auf diese Weise in ein Gehege einsperren. Pech fürs Huhn *g*
 *
 */

bool ZeldaModule::CheckSolid(int x, int y)
{
	return ::CheckSolid(pObj->GetObjectList()->GetWorld(), x, y);
}

/////////////////////////////////////////
// Implementierung von ZeldaChickenModule
/////////////////////////////////////////

ZeldaChickenModule::ZeldaChickenModule()
{
	SetDirection(DIR_LEFT);
	Move(MOVE_IDLE);
	bIdle = false;

	pSurfMoveLeft[0]  = pSurfMoveLeft[1]  = pSurfMoveLeft[2]  = NULL;
	pSurfMoveRight[0] = pSurfMoveRight[1] = pSurfMoveRight[2] = NULL;

	// Grafiken laden
	pSurfMoveLeft[0]  = LoadSurface("dat\\gfx\\chicken.bmp");
	pSurfMoveLeft[1]  = LoadSurface("dat\\gfx\\chicken_l0.bmp");
	pSurfMoveLeft[2]  = LoadSurface("dat\\gfx\\chicken_l1.bmp");
	pSurfMoveRight[0] = LoadSurface("dat\\gfx\\chicken_r.bmp");
	pSurfMoveRight[1] = LoadSurface("dat\\gfx\\chicken_r0.bmp");
	pSurfMoveRight[2] = LoadSurface("dat\\gfx\\chicken_r1.bmp");

	// Sound laden
	pSoundGluck = LoadSound("dat\\snd\\chicken\\gluck.wav");
}

ZeldaChickenModule::~ZeldaChickenModule()
{
	if (pSoundGluck != NULL) 
		delete pSoundGluck;
	pSoundGluck = NULL;

	Game->Screen->ReleaseSurfaceFromList(pSurfMoveLeft[0]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveLeft[1]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveLeft[2]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveRight[0]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveRight[1]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveRight[2]);
}

/*
 * Gluck
 *
 * Wird aufgerufen, wenn das arme Huhn geschlagen wurde, dann machts gluck gluck :-)
 *
 */

void ZeldaChickenModule::Gluck(void)
{
	if (pSoundGluck != NULL) pSoundGluck->Play();
}

/*
 * Hurt
 *
 * Wird aufgerufen, wenn das Huhn geschlagen wurde
 *
 */

void ZeldaChickenModule::Hurt(void)
{
	Gluck();
}

/*
 * Update
 *
 * Zeichnet das Huhn
 *
 */

void ZeldaChickenModule::Update(void)
{
	static int nMoveFrame = 0;

	if (GetObject() == NULL)
		return;

	if (!Game->bEditor && !bIdle)
	{
		int r = rand();

		if (r < 1000)
			Move(MOVE_LEFT);
		else if (r > 1000 && r < 1500)
			Move(MOVE_RIGHT);
		else if (r > 1500 && r < 2000)
			Move(MOVE_UP);
		else if (r > 2000 && r < 2500)
			Move(MOVE_DOWN);
		else if (r > 2500)
			Move(MOVE_IDLE);
	}
	else Move(MOVE_IDLE);

	RECT rc;
	rc.left   = 0; rc.top = 0;
	rc.right  = GetObject()->GetWidth();
	rc.bottom = GetObject()->GetHeight();

	int x, y;

	CDXSurface* pSurf;

	if (!Game->bMiniMap)
	{

		if (GetMoveCmd() == MOVE_IDLE)
			if (GetDirection() == DIR_LEFT)
				pSurf = pSurfMoveLeft[0];
			else
				pSurf = pSurfMoveRight[0];
		else
		{
			switch (nMoveFrame)
			{
			case 0:
				if (GetDirection() == DIR_LEFT)
				pSurf = pSurfMoveLeft[0];
			else
				pSurf = pSurfMoveRight[0];
			break;

			case 1:
				if (GetDirection() == DIR_LEFT)
				pSurf = pSurfMoveLeft[1];
			else
				pSurf = pSurfMoveRight[1];
			break;

			case 2:
				if (GetDirection() == DIR_LEFT)
				pSurf = pSurfMoveLeft[2];
			else
				pSurf = pSurfMoveRight[2];
			break;
			}

			nMoveFrame++;

			if (nMoveFrame == 3)
			{
				nMoveFrame = 0;
			}

			switch (GetMoveCmd())
			{
			case MOVE_LEFT:
				x = GetObject()->GetXKoord()-10;
				y = GetObject()->GetYKoord();

				if (!CheckSolid(x,y))
					GetObject()->SetLocation(x, y);
			break;

			case MOVE_RIGHT:
				x = GetObject()->GetXKoord()+10;
				y = GetObject()->GetYKoord();

				if (!CheckSolid(x+GetObject()->GetWidth(),y))
					GetObject()->SetLocation(x, y);
			break;

			case MOVE_UP:
				x = GetObject()->GetXKoord();
				y = GetObject()->GetYKoord()-10;

				if (!CheckSolid(x,y))
					GetObject()->SetLocation(x, y);
			break;

			case MOVE_DOWN:
				x = GetObject()->GetXKoord();
				y = GetObject()->GetYKoord()+10;

				if (!CheckSolid(x,y+GetObject()->GetHeight()))
					GetObject()->SetLocation(x, y);
			break;
			}
		}

		// Huhn nur zeichnen, wenn er zur aktuellen Welt gehört
		if (GetObject()->GetObjectList()->GetWorld() == Game->GetActiveWorld())
		{
			LONG lDestX = X_KOORD(GetObject()->GetXKoord());
			LONG lDestY = Y_KOORD(GetObject()->GetYKoord());
		
			// Objekt zeichnen
			pSurf->DrawTrans(Game->Screen->GetBack(), lDestX, lDestY, &rc);	
		}
	}
	else
	{
		// Huhn nur zeichnen, wenn er zur aktuellen Welt gehört
		if (GetObject()->GetObjectList()->GetWorld() == Game->GetActiveWorld())
		{
			if (GetObject()->GetObjectList()->GetWorld()->pMiniMap != NULL)
				GetObject()->GetSurface()->DrawTrans(GetObject()->GetObjectList()->GetWorld()->pMiniMap, 
					GetObject()->GetXKoord(), GetObject()->GetYKoord(), &rc);
		}
	}
}

/*
 * Move
 *
 * Lässt das Huhn in eine bestimmte Richtung laufen
 *
 */

void ZeldaChickenModule::Move(MOVE move)
{
	mov = move;

	switch (mov)
	{
	case MOVE_LEFT:
		SetDirection(DIR_LEFT);
	break;

	case MOVE_RIGHT:
		SetDirection(DIR_RIGHT);
	break;
	}
}

/////////////////////////////////////////
// Implementierung von ZeldaSoldierModule
/////////////////////////////////////////

ZeldaSoldierModule::ZeldaSoldierModule()
{
	SetDirection(DIR_DOWN);
	Move(MOVE_IDLE);
	bIdle = false;

	pSurfMoveLeft[0]  = pSurfMoveLeft[1]  = NULL;
	pSurfMoveRight[0] = pSurfMoveRight[1] = NULL;
	pSurfMoveUp[0]    = pSurfMoveUp[1]    = NULL;
	pSurfMoveDown[0]  = pSurfMoveDown[1]  = NULL;

	nLives       = 3;
	nRadius      = 2500;
	nSightRadius = 100;
	bDying       = false;
	bFollowLink  = false;
	bHurted      = false;

	bPatroullieHorz  = true;
	nPatroullieSteps = 5;

	nDyingCount  = 0;
	nMoveFrame   = 0;
	nMoveCount   = 0;
	nHurtWait    = 0;
	nHurtedCount = 0;

	nPatroullieFrame = 0;
	nPatroullieDir   = 1;  // 1 == rechts, oben, -1 == links, unten

	// Grafiken laden
	pSurfMoveLeft[0]  = LoadSurface("dat\\gfx\\soldier_l0.bmp");
	pSurfMoveLeft[1]  = LoadSurface("dat\\gfx\\soldier_l1.bmp");
	pSurfMoveRight[0] = LoadSurface("dat\\gfx\\soldier_r0.bmp");
	pSurfMoveRight[1] = LoadSurface("dat\\gfx\\soldier_r1.bmp");
	pSurfMoveUp[0]    = LoadSurface("dat\\gfx\\soldier_u0.bmp");
	pSurfMoveUp[1]    = LoadSurface("dat\\gfx\\soldier_u1.bmp");
	pSurfMoveDown[0]  = LoadSurface("dat\\gfx\\soldier_d0.bmp");
	pSurfMoveDown[1]  = LoadSurface("dat\\gfx\\soldier_d1.bmp");

	// Sound laden
	pSoundHurt  = LoadSound("dat\\snd\\enemy\\hurt.wav");
	pSoundDying = LoadSound("dat\\snd\\enemy\\dying.wav");
}

ZeldaSoldierModule::~ZeldaSoldierModule()
{
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveLeft[0]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveLeft[1]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveRight[0]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveRight[1]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveUp[0]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveUp[1]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveDown[0]);
	Game->Screen->ReleaseSurfaceFromList(pSurfMoveDown[1]);

	if (pSoundHurt != NULL) 
		delete pSoundHurt;
	pSoundHurt = NULL;

	if (pSoundDying != NULL) 
		delete pSoundDying;
	pSoundDying = NULL;
}

void ZeldaSoldierModule::Update(void)
{
	if (GetObject() == NULL)
		return;

	static int x_middle = GetObject()->GetXKoord();
	static int y_middle = GetObject()->GetYKoord();

	RECT rc;

	if (bDying)  // Soldat hat es erwischt, R.I.P.
	{
		if (nDyingCount == 20)  // kleine Verzögerung des Sterbe Prozesses
		{
			// Soldat Objekt zerstören
			Game->GetActiveWorld()->GetObjectList()->DestroyElement(GetObject());
			return;
		}
		else nDyingCount++;
	}

	
	Move(MOVE_IDLE);
	
	// Befindet sich der Soldat in der selben Welt wie unser Link?
	if (GetObject()->GetObjectList()->GetWorld() == Game->GetActiveWorld())
		SearchLink();     // Schaue, ob Link sich in der Nähe befindet

	// Nächster Move Frame
	if (nMoveCount == 10)
	{
		nMoveCount = 0;
		
		if (bFollowLink && !Game->bEditor && !bIdle && !bHurted)  // Folge Link
		{
			// Patroullie Parameter zurücksetzen
			nPatroullieDir   = 1;
			nPatroullieFrame = 0;

			// Distanz zum Link, Vorzeichen bestimmt über Richtung
			int x_dist, y_dist;  

			x_dist = GetObject()->GetXKoord() - Game->Link->GetXKoord();
			y_dist = GetObject()->GetYKoord() - Game->Link->GetYKoord();

			if (abs(x_dist) > abs(y_dist))
			{
				if (x_dist > 0)
					Move(MOVE_LEFT);
				else
					Move(MOVE_RIGHT);
			}
			else
			{
				if (y_dist > 0)
					Move(MOVE_UP);
				else
					Move(MOVE_DOWN);
			}

			// Befindet sich der Soldat in der selben Welt wie unser Link?
			if (GetObject()->GetObjectList()->GetWorld() == Game->GetActiveWorld())
				// Befindet sich Link mit dem Soldaten Auge in Auge
				// dann bekommt Link sein Schwert zu spüren
				CheckHurt();
		}
		else if (!Game->bEditor && !bIdle && !bHurted)  // Patroullie, wenn Link nicht in Nähe
		{
			// Soll horizontal patroulliert werden?
			if (bPatroullieHorz)
			{
				if (nPatroullieDir == 1)  // 1 = rechts
					Move(MOVE_RIGHT);
				else
					Move(MOVE_LEFT);

				nPatroullieFrame++;
			}
			else
			{
				if (nPatroullieDir == 1)
					Move(MOVE_UP);
				else
					Move(MOVE_DOWN);
			}

			// Ende der Patroullie ?
			if (nPatroullieFrame == nPatroullieSteps)
			{
				nPatroullieFrame = 0;

				// Kehrtwende
				if (nPatroullieDir == 1)
					nPatroullieDir = -1;
				else
					nPatroullieDir = 1;
			}
		}
		else if (bHurted)  // Hat der Soldat eins aufs Maul bekommen? (he he)
		{
			if (nHurtedCount == 1)
			{
				bHurted = false;
				nHurtedCount = 0;
			}
			else nHurtedCount++;

			Move(MOVE_IDLE);
		}
	}

	nMoveCount++;

	int x, y;

	CDXSurface* pSurf;

	if (!Game->bMiniMap)
	{
		if (GetMoveCmd() == MOVE_IDLE)
			nMoveFrame = 0;
		
		// Animations Frame bestimmen
		switch (nMoveFrame)
		{
		case 0:  // Animations Frame 1
			if (GetDirection() == DIR_LEFT)
				pSurf = pSurfMoveLeft[0];
			
			if (GetDirection() == DIR_RIGHT)
				pSurf = pSurfMoveRight[0];

			if (GetDirection() == DIR_UP)
				pSurf = pSurfMoveUp[0];

			if (GetDirection() == DIR_DOWN)
				pSurf = pSurfMoveDown[0];
		break;

		case 1:  // Animations Frame 2
			if (GetDirection() == DIR_LEFT)
				pSurf = pSurfMoveLeft[1];
			
			if (GetDirection() == DIR_RIGHT)
				pSurf = pSurfMoveRight[1];

			if (GetDirection() == DIR_UP)
				pSurf = pSurfMoveUp[1];

			if (GetDirection() == DIR_DOWN)
				pSurf = pSurfMoveDown[1];
		break;
		}

		nMoveFrame++;

		if (nMoveFrame == 2)
			nMoveFrame = 0;

		switch (GetMoveCmd())
		{
		case MOVE_LEFT:
			x = GetObject()->GetXKoord()-10;
			y = GetObject()->GetYKoord();

			if (!CheckSolid(x+GetObject()->GetWidth()/2,y+GetObject()->GetHeight()/2) && 
				SoldierInRadius(x,y,x_middle,y_middle))
				GetObject()->SetLocation(x, y);
		break;

		case MOVE_RIGHT:
			x = GetObject()->GetXKoord()+10;
			y = GetObject()->GetYKoord();

			if (!CheckSolid(x+GetObject()->GetWidth()/2,y+GetObject()->GetHeight()/2) && 
				SoldierInRadius(x,y,x_middle,y_middle))
				GetObject()->SetLocation(x, y);
		break;

		case MOVE_UP:
			x = GetObject()->GetXKoord();
			y = GetObject()->GetYKoord()-10;

			if (!CheckSolid(x+GetObject()->GetWidth()/2,y+GetObject()->GetHeight()/2) && 
				SoldierInRadius(x,y,x_middle,y_middle))
				GetObject()->SetLocation(x, y);
		break;

		case MOVE_DOWN:
			x = GetObject()->GetXKoord();
			y = GetObject()->GetYKoord()+10;

			if (!CheckSolid(x+GetObject()->GetWidth()/2,y+GetObject()->GetHeight()/2) && 
				SoldierInRadius(x,y,x_middle,y_middle))
				GetObject()->SetLocation(x, y);
		break;

		case MOVE_IDLE:
			x = GetObject()->GetXKoord();
			y = GetObject()->GetYKoord();
		break;
		}
	
		// Soldat nur zeichnen, wenn er zur aktuellen Welt gehört
		if (GetObject()->GetObjectList()->GetWorld() == Game->GetActiveWorld())
		{
			rc.left   = 0; rc.top = 0;
			rc.right  = pSurf->GetWidth();
			rc.bottom = pSurf->GetHeight();

			// Relative Bildschirmkoordinaten berechnen
			LONG lDestX = X_KOORD(GetObject()->GetXKoord());
			LONG lDestY = Y_KOORD(GetObject()->GetYKoord());
		
			// Objekt zeichnen
			pSurf->DrawTrans(Game->Screen->GetBack(), lDestX, lDestY, &rc);	
		}
	}
	else  // bMiniMap == 1
	{
		// Soldat nur zeichnen, wenn er zur aktuellen Welt gehört
		if (GetObject()->GetObjectList()->GetWorld() == Game->GetActiveWorld())
		{
			rc.left   = 0; rc.top = 0;
			rc.right  = GetObject()->GetWidth();
			rc.bottom = GetObject()->GetHeight();

			if (GetObject()->GetObjectList()->GetWorld()->pMiniMap != NULL)
				GetObject()->GetSurface()->DrawTrans(GetObject()->GetObjectList()->GetWorld()->pMiniMap, 
					GetObject()->GetXKoord(), GetObject()->GetYKoord(), &rc);
		}
	}
}

/*
 * Move
 *
 * Lässt den Soldaten in eine bestimmte Richtung laufen
 *
 */

void ZeldaSoldierModule::Move(MOVE move)
{
	mov = move;

	switch (mov)
	{
	case MOVE_LEFT:
		SetDirection(DIR_LEFT);
	break;

	case MOVE_RIGHT:
		SetDirection(DIR_RIGHT);
	break;

	case MOVE_UP:
		SetDirection(DIR_UP);
	break;

	case MOVE_DOWN:
		SetDirection(DIR_DOWN);
	break;
	}
}

void ZeldaSoldierModule::Hurt(void)
{
	if (GetObject() == NULL)
		return;

	// Ein Lebenspunkt abziehen
	nLives--;

	if (nLives == 0)  // An der Schwelle zum Tod?
	{
		bDying = true;
		
		if (pSoundDying != NULL)
			pSoundDying->Play();
	}
	else  // Er hats "noch" überlebt
	{
		if (pSoundHurt != NULL) 
			pSoundHurt->Play();

		switch (GetDirection())  // Soldat wird durch den Schlag etwas zurückgedrängt
		{
		case DIR_LEFT:
			GetObject()->SetLocation(GetObject()->GetXKoord()+20, GetObject()->GetYKoord());
		break;

		case DIR_RIGHT:
			GetObject()->SetLocation(GetObject()->GetXKoord()-20, GetObject()->GetYKoord());
		break;

		case DIR_UP:
			GetObject()->SetLocation(GetObject()->GetXKoord(), GetObject()->GetYKoord()+20);
		break;

		case DIR_DOWN:
			GetObject()->SetLocation(GetObject()->GetXKoord(), GetObject()->GetYKoord()-20);
		break;
		}

		bHurted = true;  // wichtig für Update()
	}
}

bool ZeldaSoldierModule::SoldierInRadius(int& x, int& y, int& x_middle, int& y_middle)
{
	// Befindet sich der Soldat noch in seinem zugewiesenem
	// Bereich?
	if (abs(x - x_middle) > nRadius || abs(y - y_middle) > nRadius)
		return false;

	return true;
}

void ZeldaSoldierModule::SearchLink(void)
{
	if (GetObject() == NULL)
		return;

	bFollowLink = false;

	// Überprüfe Abstand in horizontaler Richtung
	if (abs(Game->Link->GetXKoord() - GetObject()->GetXKoord()) <= nSightRadius)
		bFollowLink = true;

	// Überprüfe Abhstand in vertikaler Richtung
	if (abs(Game->Link->GetYKoord() - GetObject()->GetYKoord()) <= nSightRadius)
		bFollowLink = true;	
}

void ZeldaSoldierModule::CheckHurt(void)
{
	RECT rc;
	
	// "Schwert Aktions Rechteck" berechnen
	switch(GetDirection())
	{
	case DIR_LEFT:
		rc.left   = GetObject()->GetXKoord();
		rc.top    = GetObject()->GetYKoord()+60;
		rc.right  = rc.left+50;
		rc.bottom = rc.top+20;
	break;

	case DIR_RIGHT:
		rc.left   = GetObject()->GetXKoord()+67;
		rc.top    = GetObject()->GetYKoord()+60;
		rc.right  = rc.left+50;
		rc.bottom = rc.top+20;
	break;

	case DIR_UP:
		rc.left   = GetObject()->GetXKoord()+57;
		rc.top    = GetObject()->GetYKoord();
		rc.right  = rc.left+20;
		rc.bottom = rc.top+40;
	break;

	case DIR_DOWN:
		rc.left   = GetObject()->GetXKoord();
		rc.top    = GetObject()->GetYKoord()+70;
		rc.right  = rc.left+20;
		rc.bottom = rc.top+40;
	break;
	}

	// Soldat nah genug an Link dran?
	if (RectInRect(rc, Game->Link->GetRect()))
	{
		// Zwischen den Schlägen etwas warten, sonst ist 
		// es ein bisschen unfair für den Link
		if (nHurtWait != 1)
		{
			nHurtWait++;
			return;
		}
		else
		{
			// Ok, Link konnte nicht mehr dem Schlag des bösen
			// Soldaten ausweichen, Pech gehabt
			Game->Link->Hurt();
			nHurtWait = 0;
		}
	}
}

//////////////////////////////////////////
// Implementierung von ZeldaHighGrasModule
//////////////////////////////////////////

ZeldaHighGrasModule::ZeldaHighGrasModule()
{

}


ZeldaHighGrasModule::~ZeldaHighGrasModule()
{

}

void ZeldaHighGrasModule::Update(void)
{
	if (GetObject() == NULL)
		return;

	RECT rc;
	rc.left   = 0; rc.top = 0;
	rc.right  = GetObject()->GetWidth();
	rc.bottom = GetObject()->GetHeight();

	CDXSurface* pSurf = GetObject()->GetSurface();

	LONG lDestX = X_KOORD(GetObject()->GetXKoord());
	LONG lDestY = Y_KOORD(GetObject()->GetYKoord());
	
	if (!Game->bMiniMap)
	{
		// Objekt zeichnen
		if (pSurf != NULL) pSurf->DrawTrans(Game->Screen->GetBack(), lDestX, lDestY, &rc);
	}
	else  // bMiniMap == true
	{  
		// Objekt wird in die Mini Map gezeichnet
		if (Game->GetActiveWorld()->pMiniMap != NULL)
			GetObject()->GetSurface()->DrawTrans(Game->GetActiveWorld()->pMiniMap,GetObject()->GetXKoord(), 
			GetObject()->GetYKoord(), &rc);

		return;
	}
	
	// Steht Link zufällig im Gras?
	rc.left   = GetObject()->GetXKoord();
	rc.top    = GetObject()->GetYKoord();
	rc.right  = rc.left + GetObject()->GetWidth();
	rc.bottom = rc.top  + GetObject()->GetHeight();

	if (PointInRect(Game->Link->GetXKoord()+Game->Link->GetWidth()/2,
		Game->Link->GetYKoord()+Game->Link->GetHeight()-10,rc))
		Game->Link->bStandInHighGras = true;
}