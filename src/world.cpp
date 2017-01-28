/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: world.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

ZeldaWorld::ZeldaWorld(const char* name)
{
	world_name       = NULL;
	link_init_x      = 0;
	link_init_y      = 0;
	worldview_init_x = 0;
	worldview_init_y = 0;

	if (name != NULL)
	{
		world_name = (char*)malloc(strlen(name)*sizeof(char));
		strcpy(world_name,name);
	}

	file_name = NULL;
	width     = 0;
	height    = 0;

	pMiniMap  = NULL;

	/* *** Init Object List *** */

	ObjectList = new ZeldaObjectList(this);
}

ZeldaWorld::~ZeldaWorld()
{
	// Lösche MiniMap
	if (Game->Screen != NULL && pMiniMap != NULL) 
		Game->Screen->ReleaseSurfaceFromList(pMiniMap);
}

/*
 * SetWorldViewRect
 *
 * Definiert den Bereich der Welt, den man dann auf dem Monitor zu Gesicht bekommt.
 *
 */

void ZeldaWorld::SetWorldViewRect(int x, int y)
{
	world_view_rect.left   = x;
	world_view_rect.top    = y;
	world_view_rect.right  = world_view_rect.left + SCREEN_X;
	world_view_rect.bottom = world_view_rect.top  + SCREEN_Y;
}

/*
 * Save
 *
 * Speichert die aktuelle Welt.
 *
 */

bool ZeldaWorld::Save(void)
{
	FILE* file = NULL;

	//file = fopen(GetFileName(), "w");
	file = fopen("save.txt", "w");

	if (file == NULL)
	{
		DEBUG("ZeldaWorld::Save : fopen() == NULL\n");
		return false;
	}

	fprintf(file, "%d %d\n", GetWidth(), GetHeight());
	fprintf(file, "%d %d\n", GetWorldViewRect().left, GetWorldViewRect().top);
	fprintf(file, "%d %d\n", Game->Link->GetXKoord(), Game->Link->GetYKoord());
	fprintf(file, "%d", GetObjectList()->GetElementCount());

	if (!ZeldaObjectList::SortObjects(GetObjectList()))
	{
		fclose(file);
		DeleteFile(GetFileName());
		return false;
	}
						
	for (int i = 0; i != GetObjectList()->GetElementCount(); i++)
	{
		ZeldaObject* obj = GetObjectList()->GetElementByIndex(i);

		if (obj == NULL)
		{
			DEBUG("ZeldaWorld::Save : GetElementByIndex() == NULL\n");
			fclose(file);
			return false;
		}

		fprintf(file, "\n%s %d %d", obj->GetObjectName(), obj->GetXKoord(), obj->GetYKoord());

		// Portal
		if (obj->GetPortal() != NULL)
			fprintf(file, " %s %d %d", obj->GetPortal()->GetWorld(), obj->GetPortal()->GetLinkInitXPos(), obj->GetPortal()->GetLinkInitYPos());
	}
	
	fclose(file);			

	return true;
}

/*
 * LoadWorld
 *
 * Lädt eine Welt von einer Datei
 *
 */

bool ZeldaWorld::LoadWorld(const char* file_name)
{
	this->file_name = (char*)malloc(strlen(file_name)*sizeof(char));
	strcpy(this->file_name,file_name);

	FILE* file = fopen(respack_interface(file_name), "r");

	if (file == NULL)
	{
		DEBUG("ZeldaWorld::LoadWorld : fopen(...)\n");
		return false;
	}

	// Breite und Höhe der Welt lesen
	fscanf(file, "%d", &width);
	fscanf(file, "%d", &height);

	// Position des WorldViewRects
	fscanf(file, "%d", &worldview_init_x);
	fscanf(file, "%d", &worldview_init_y);

	SetWorldViewRect(worldview_init_x, worldview_init_y);

	// Position von Link
	fscanf(file, "%d", &link_init_x);
	fscanf(file, "%d", &link_init_y);

	// Objekte erzeugen und platzieren
	int anzObj; 
	char temp1[256];
	ZeldaObject* obj;
	int x, y;

	fscanf(file, "%d", &anzObj);

	for (int i = 0; i != anzObj; i++)
	{
		fscanf(file, "%s", temp1);
		fscanf(file, "%d", &x);
		fscanf(file, "%d", &y);

		DEBUG("Create OBJECT=%s...\n", temp1);

		obj = new ZeldaObject(GetObjectList());
		
		if (obj == NULL)
		{
			DEBUG("ZeldaWorld::LoadWorld : new ZeldaObject()\n");
			fclose(file);
			return false;
		}
		
		if (!obj->LoadObject((const char*)temp1))
		{
			DEBUG("ZeldaWorld::LoadWorld : obj->LoadObject(%s)\n", temp1);
			fclose(file);
			return false;
		}

		obj->SetLocation(x,y);

		// Portal Welt
		if (obj->GetObjectDef()->nPortal != 0)
		{
			fscanf(file, "%s", temp1); // Welt
			obj->InitPortal((const char*)temp1);

			int x, y;

			fscanf(file, "%d", &x);    // Link Init X Pos 
			fscanf(file, "%d", &y);    // Link Init Y Pos
			obj->GetPortal()->SetLinkInitPosition(x,y);
		}

		if (GetObjectList()->AddElement(obj) == -1)
		{
			DEBUG("ZeldaWorld::LoadWorld : AddElement()\n");
			delete obj;
			fclose(file);
			return false;
		}

		DEBUG("OBJECT=%s successfully created.\n", obj->GetObjectName());
	}

	fclose(file);

	return true;
}

/*
 * GetSurface
 *
 * Liefert einen Zeiger auf eine DirectSurface Oberfläche zurück. Bei Fehler ist der Rückgabewert
 * NULL.
 *
 */

CDXSurface* ZeldaWorld::GetSurface(void)
{
	return Game->Screen->GetBack();
}
