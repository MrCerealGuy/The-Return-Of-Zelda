/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: portal.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

///////////////////////////
// Portalbezogene Variablen
///////////////////////////
bool bChangeWorld      = false;
ZeldaPortal* pPortal   = NULL;
ZeldaObject* pTransObj = NULL;  // Objekt, das mit in eine andere Welt mitgetragen wird

ZeldaPortal::ZeldaPortal()
{
	x = y = 0;
	width = height = 0;
	link_to_x = 0;
	link_to_y = 0;
}

ZeldaPortal::~ZeldaPortal()
{

}

/*
 * SetWorld
 *
 * Definiert die Welt, in die man teleportiert wird, wenn man sich im Portal Bereich findet
 *
 */

void ZeldaPortal::SetWorld(const char* szToWorld)
{
	if (szToWorld != NULL)
		strcpy(this->szToWorld, szToWorld);
}

/*
 * SetSize
 *
 * Definiert die Größe des Portalbereichs (wird in der Objekt Definitionsdatei angegeben)
 *
 */

void ZeldaPortal::SetSize(int width, int height)
{
	this->width  = width;
	this->height = height;
}

/*
 * SetLocation
 *
 * Platziert das Portal an die angegebenen Koordinaten
 *
 */

void ZeldaPortal::SetLocation(int x, int y)
{
	this->x = x;
	this->y = y;
}

/*
 * GetRect
 *
 * Gibt eine RECT Struktur zurück, die den Portal Bereich definiert
 *
 */

RECT ZeldaPortal::GetRect(void)
{
	RECT rc;

	rc.left = x;
	rc.top  = y;
	rc.right  = x + width;
	rc.bottom = y + height;

	return rc;
}