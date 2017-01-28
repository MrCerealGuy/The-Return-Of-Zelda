/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: object.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

ZeldaObject::ZeldaObject(ZeldaObjectList* list)
{
	ObjDef      = NULL;
	x = y       = 0;
	bSelected   = false;
	object_list = list;
	nAnimation  = -1;
	portal      = NULL;
	bBreakIt    = false;
	bHasModule  = false;
	pModule     = NULL;

	module.pSoldier  = NULL;
	module.pChicken  = NULL;
	module.pHighGras = NULL;
}

ZeldaObject::~ZeldaObject()
{
	// Portal löschen
	if (portal != NULL)
		delete portal;

	// Modul löschen
	if (IsChicken())
	{
		if (module.pChicken != NULL)
			delete module.pChicken;
	}
	else if (IsSoldier())
	{
		if (module.pSoldier != NULL)
			delete module.pSoldier;
	}
	else if (IsHighGras())
	{
		if (module.pHighGras != NULL)
			delete module.pHighGras;
	}

	// Wichtig!!! Verweis auf (Prae)SelectedObject
	// muss unbedingt gelöscht werden
	if (SelectedObject == this)
		SelectedObject = NULL;
	else if (PraeSelectedObject == this)
		PraeSelectedObject = NULL;
}

/*
 * GetRect
 *
 * Liefert die Reckteckkoordinaten zurück.
 *
 */

RECT ZeldaObject::GetRect(void)
{
	RECT rc;
	rc.left   = GetXKoord();
	rc.top    = GetYKoord();
	rc.right  = GetXKoord() + GetWidth();
	rc.bottom = GetYKoord() + GetHeight();

	return rc;
}

/*
 * InitPortal
 *
 * Initialisiert das Portal, vorrausgesetzt das Objekt verfügt über ein Portal
 *
 */

bool ZeldaObject::InitPortal(const char* szToWorld)
{
	if (GetObjectDef() == NULL ||portal == NULL)
		return false;

	portal->SetLocation(GetXKoord()+GetObjectDef()->x_offset_port,
			GetYKoord()+GetObjectDef()->y_offset_port);

	portal->SetSize(GetObjectDef()->port_width, GetObjectDef()->port_height);

	// Welt, in die man teleportiert wird
	portal->SetWorld(szToWorld);

	return true;
}

void ZeldaObject::UpdateModule(void)
{
	if (pModule == NULL) return;

	switch (ObjDef->module_type)
	{
	case MODULE_CHICKEN:
		((ZeldaChickenModule*)pModule)->Update();
	break;
	
	case MODULE_SOLDIER:
		((ZeldaSoldierModule*)pModule)->Update();
	break;
	

	case MODULE_HIGHGRAS:
		((ZeldaHighGrasModule*)pModule)->Update();
	break;
	}
}

/*
 * Update
 *
 * Zeichnet das Objekt neu.
 *
 */

void ZeldaObject::Update(void)
{
	if (object_list == NULL)
		return;

	// Update Module
	if (bHasModule)
	{
		// Rufe modul interne Update Funktion auf
		UpdateModule();  
		return;
	}

	RECT rc;
	rc.left   = 0; rc.top = 0;
	rc.right  = GetWidth();
	rc.bottom = GetHeight();

	if (!Game->bMiniMap)
	{
		CDXSurface *pSurf = NULL;

		if (bBreakIt)
			nAnimation = ObjDef->nBreakAnim;

		if (ObjDef->nAnimations >= 1 && nAnimation != -1)
			pSurf = ObjDef->pAnims[nAnimation]->GetNextFrame();

		if (pSurf == NULL)
			pSurf = GetSurface();
		
		// Objekt zeichnen
		if (pSurf != NULL) 
			pSurf->DrawTrans(Game->Screen->GetBack(), X_KOORD(GetXKoord()),Y_KOORD(GetYKoord()), &rc);

		// Ende der Break-Anim, Objekt zerstören
		if (bBreakIt && !Game->bMiniMap)
		{
			if (ObjDef->pAnims[nAnimation]->GetLastFrame() == pSurf)
			{
				if (ObjDef->pSoundBreak != NULL)
					ObjDef->pSoundBreak->Play();

				object_list->DestroyElement(this);

				ZeldaObjectList::SortObjects(Game->GetActiveWorld()->GetObjectList());

				bObjectDeleted = true;  // Für nähere Informationen bitte in der Funktion
				                        // UpdateArea nachschauen
			}
		}
	}
	else  // bMiniMap == true
	{  
		// Objekt wird in die Mini Map gezeichnet
		if (GetObjectList()->GetWorld()->pMiniMap != NULL)
			GetSurface()->DrawTrans(GetObjectList()->GetWorld()->pMiniMap, 
				GetXKoord(), GetYKoord(), &rc);
	}
}

/*
 * UpdateLayer
 *
 * Zeichnet den Layer des Objektes. Geeignet für Objekte, die LEVEL_TWO und LEVEL_THREE
 * Zeichen Modi gleichzeitig sind. Man gibt aber dann nur LEVEL_TWO an und defniert einen
 * Layer der dann LEVEL_THREE Eigenschaften besitzt. Z.b. Baum: Baumstamm ist dann LEVEL_TWO
 * und die Baumkrone LEVEL_THREE.
 *
 */

void ZeldaObject::UpdateLayer(void)
{
	if (GetObjectList() == NULL || GetObjectDef() == NULL)
		return;

	RECT rc;

	if (GetObjectDef()->nUseLayerGfx == 1)  // wird separate Layer Grafik verwendet?
	{
		rc.left   = 0; rc.top = 0;
		rc.right  = GetWidth();
		rc.bottom = GetHeight();

		if (GetObjectDef()->pLayer != NULL)
			GetObjectDef()->pLayer->DrawTrans(Game->Screen->GetBack(),
			X_KOORD(GetXKoord()), Y_KOORD(GetYKoord()), &rc);
	}
	else  // Nein, dann besorgen wir uns die Grafik über die Hauptgrafik
	{
		rc.left   = GetObjectDef()->nLayerCoords[0]; 
		rc.top    = GetObjectDef()->nLayerCoords[1];
		rc.right  = GetObjectDef()->nLayerCoords[2];
		rc.bottom = GetObjectDef()->nLayerCoords[3];

		if (GetObjectDef()->pSurface != NULL)
			GetObjectDef()->pSurface->DrawTrans(Game->Screen->GetBack(),
			X_KOORD(GetXKoord()), Y_KOORD(GetYKoord()), &rc);
	}
}

/*
 * SetLocation
 *
 * Positioniert das Objekt an die Koordinaten (x,y).
 *
 */

void ZeldaObject::SetLocation(const int& x, const int& y)
{
	this->x = x;
	this->y = y;

	// Portal wenn vorhanden, neu platzieren
	if (portal != NULL)
		portal->SetLocation(x+GetObjectDef()->x_offset_port,y+GetObjectDef()->y_offset_port);
}

/*
 * LoadObject
 *
 * Lädt ein Objekt aus dem Objekt Definitions Modul.
 *
 */

bool ZeldaObject::LoadObject(const char* object_name)
{
	if ((ObjDef = ::GetObjectDef(object_name)) == NULL)
		return false;

	nAnimation = ObjDef->nDefaultAnim;

	// Erschaffe Portal
	if (ObjDef->nPortal != 0)
	{
		portal = new ZeldaPortal();

		if (portal == NULL)
		{
			DEBUG("ZeldaObject::LoadObject : new ZeldaPortal()\n");
			return false;
		}
	}

	// Module
	if (ObjDef->nModule == 1)
	{
		bHasModule = true;

		switch (ObjDef->module_type)
		{
		case MODULE_SOLDIER:
			DEBUG("ZeldaObject::LoadObject : Create MODULE_SOLDIER\n");
			module.pSoldier = new ZeldaSoldierModule();
			if (module.pSoldier != NULL) 
			{ module.pSoldier->SetObject(this); pModule = module.pSoldier; } 
		break;

		case MODULE_CHICKEN:
			DEBUG("ZeldaObject::LoadObject : Create MODULE_CHICKEN\n");
			module.pChicken = new ZeldaChickenModule();
			if (module.pChicken != NULL)
			{ module.pChicken->SetObject(this); pModule = module.pChicken; }
		break;

		case MODULE_HIGHGRAS:
			DEBUG("ZeldaObject::LoadObject : Create MODULE_HIGHGRAS\n");
			module.pHighGras = new ZeldaHighGrasModule();
			if (module.pHighGras != NULL)
			{ module.pHighGras->SetObject(this); pModule = module.pHighGras; }
		break;
		}
	}

	return true;
}

/*
 * IsLiftable
 *
 * Gibt true zurück, wenn das Objekt vom Link getragen werden kann (kann man in der Objekt-
 * definitionsdatei definieren)
 *
 */

bool ZeldaObject::IsLiftable(void)
{
	if (ObjDef->nLiftable == 1 && !bBreakIt)
		return true;
	
	return false;
}

/*
 * BreakIt
 *
 * Wird aufgerufen, nachdem das Objekt geworfen wurde, Objekt muss zerstörbar sein, wird in
 * der Objekt Definitionsdatei angegeben
 *
 */

bool ZeldaObject::BreakIt(void)
{
	if (!IsBreakable() || ObjDef->nBreakAnim == -1)
		return false;

	bBreakIt = true;

	return true;
}

/*
 * IsBreakable
 *
 * Gibt true zurück, wenn das Objekt zerstört wird, nachdem es von Link geworfen wurde
 * (wird in der Objekt Definitionsdatei angegeben)
 *
 */

bool ZeldaObject::IsBreakable(void)
{
	if (ObjDef->nBreakable == 1)
		return true;

	return false;
}

/*
 * GetDrawingType
 *
 * Liefert den Zeichenebe-Typ zurück.
 *
 */

DRAWING_TYPE ZeldaObject::GetDrawingType(void)
{
	if (this == Game->Link->GetLiftObject())
		return LEVEL_THREE;
	else
		return (ObjDef != NULL) ? ObjDef->drawing_type : LEVEL_INVALID;
}

ZeldaObject* ZeldaObject::CreateObject(const char* object_name, int x, int y, ZeldaWorld* pWorld)
{
	if (pWorld == NULL && pWorld->GetObjectList() != NULL)
		return NULL;

	ZeldaObject* pObj = new ZeldaObject(pWorld->GetObjectList());

	if (pObj != NULL)
	{
		if (pObj->LoadObject(object_name))
		{
			pObj->SetLocation(x,y);
	
			if (pWorld->GetObjectList()->AddElement(pObj) == -1)
				delete pObj;
			else
				return pObj;
		}
	}

	return NULL;
}
