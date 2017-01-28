/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: status.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

ZeldaStatus::ZeldaStatus()
{
	pSurfHeartFull  = NULL;
	pSurfHeartEmpty = NULL;
	pSurfLifeTitle  = NULL;

	pSurfHeartFull  = LoadSurface("dat\\gfx\\heart_full.bmp");
	pSurfHeartEmpty = LoadSurface("dat\\gfx\\heart_empty.bmp");
	pSurfLifeTitle  = LoadSurface("dat\\gfx\\life_title.bmp");
}

ZeldaStatus::~ZeldaStatus()
{
	Game->Screen->ReleaseSurfaceFromList(pSurfHeartFull);
	Game->Screen->ReleaseSurfaceFromList(pSurfHeartEmpty);
	Game->Screen->ReleaseSurfaceFromList(pSurfLifeTitle);
}

void ZeldaStatus::DrawLinkStatus(void)
{
	if (pSurfLifeTitle == NULL || pSurfHeartFull == NULL || pSurfHeartEmpty == NULL)
		return;

	CDXSurface* pSurf = NULL;

	RECT rc;
	rc.left   = 0;
	rc.top    = 0;
	rc.right  = pSurfLifeTitle->GetWidth();
	rc.bottom = pSurfLifeTitle->GetHeight();

	pSurfLifeTitle->DrawTrans(Game->Screen->GetBack(), SCREEN_X-SCREEN_X/4, 10, &rc);

	// Herzen zeichnen
	int x = SCREEN_X-SCREEN_X/4;
	int y = 40;

	rc.right  = pSurfHeartFull->GetWidth();
	rc.bottom = pSurfHeartFull->GetHeight();

	int j;

	// Leere Herzen zeichnen
	for (j = 0; j != Game->Link->GetMaxLifes() - Game->Link->GetCurrentLifes(); j++)
	{
		pSurfHeartEmpty->DrawTrans(Game->Screen->GetBack(), x, y, &rc);
		x += pSurfHeartFull->GetWidth()+5;
	}

	// Volle Herzen zeichnen
	for (; j != Game->Link->GetMaxLifes(); j++)
	{
		pSurfHeartFull->DrawTrans(Game->Screen->GetBack(), x, y, &rc);
		x += pSurfHeartFull->GetWidth()+5;
	}
}
