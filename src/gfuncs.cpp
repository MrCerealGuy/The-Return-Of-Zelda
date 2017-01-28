/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: gfuncs.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

#include <time.h>     // Benötigen wir für time()
#include <limits.h>   // Benötigen wir für INT_MAX

char LogText[MAX_LOG_COUNT][MAX_LOG_STRING];
int nLogCount         = 0;
int log_scroll_offset = 0;
int log_start_i       = 0;
int log_end_i         = MAX_LOGS_PRO_PAGE;

const char* DEBUG(const char* szText, ...)
{
#ifdef ZELDA_LOG

	char buff[MAX_LOG_STRING];

	va_list	va;
    va_start(va, szText);

	wvsprintf(buff, szText, va);

#ifdef ZELDA_LOG_TO_FILE
	
	char file_name[20];

	// Zufallszahl generieren und Debug Dateinamen
	// dranhängen
	srand((unsigned int)time(NULL));
	static int id = rand() % INT_MAX;

	sprintf(file_name, "debug_%d.txt", id);
	FILE* flog = fopen(file_name, "a");

	if (flog == NULL)
		return szText;

	fprintf(flog, buff);
	fclose(flog);

#endif

	if (nLogCount != MAX_LOG_COUNT)
	{
		strcpy(LogText[nLogCount], buff);
		nLogCount++;
	}

	// Zum Ende scrollen
	if (!(nLogCount < MAX_LOGS_PRO_PAGE))
	{
		log_end_i   = nLogCount;
		log_start_i = nLogCount - MAX_LOGS_PRO_PAGE;
	}

	return LogText[nLogCount-1];

#endif

	return NULL;
}

bool ReadInteger(FILE* file, const char* szProperty, int* i)
{
	char temp[256];

	if (file == NULL || i == NULL)
		return false;

	fscanf(file, "%s", temp);

	if (strcmp(temp, szProperty) != 0)
	{
		DEBUG("LoadObjectDefs : Error at ");
		DEBUG((char*)szProperty); DEBUG("\n");
		
		fclose(file);
		return false;
	}

	fscanf(file, "%d", i);

	sprintf(temp, "%s=%d\n",szProperty,*i);
	DEBUG(temp);

	return true;
}

bool ReadString(FILE* file, const char* szProperty, char* s)
{
	char buf[256];

	if (file == NULL || s == NULL)
		return false;

	fscanf(file,"%s",buf);

	if (strcmp(buf, szProperty) != 0)
	{
		DEBUG("LoadObjectDefs : Error at ");
		DEBUG((char*)szProperty); DEBUG("\n");

		fclose(file);
		return false;
	}

	fscanf(file,"%s",s);

	sprintf(buf, "%s=%s\n",szProperty,s);
	DEBUG(buf);

	return true;
}

bool LoadObjectDefs(void)
{
	FILE* file;
	char  temp[256], temp2[256];

	// Speicher für Object_Def reservieren
	Object_Def = (_Object_Def*)malloc(GetObjectDefCount()*sizeof(_Object_Def));

	if (Object_Def == NULL)
		return false;
	
	for (int i = 0; strcmp((const char*)ObjDefNames[i],"ENDLIST") != 0; i++)
	{
		strcpy(Object_Def[i].name, (const char*)ObjDefNames[i]);
		sprintf(temp, "dat\\obj\\%s.txt", ObjDefNames[i]);
		strcpy(Object_Def[i].file_name, (const char*)temp);

		DEBUG("\nLoadObjectDefs : %s\n[\n", Object_Def[i].name);

		file = fopen(respack_interface(temp), "r");

		if (file == NULL)
		{
			DEBUG("Error at fopen(\"%s\")\n", temp);
			return false;
		}

		fscanf(file, "%s", temp);  // Header

		sprintf(temp2, "dat\\gfx\\%s.bmp",Object_Def[i].name);

		if ((Object_Def[i].pSurface = LoadSurface(temp2)) == NULL)
			return false;

		// OBJECT_TYPE
		fscanf(file, "%s", temp);

		Object_Def[i].object_type = OBJECT_INVALID;

		if (strcmp((const char*)temp, "OBJECT_NATURE") == 0)
			Object_Def[i].object_type = OBJECT_NATURE;

		if (strcmp((const char*)temp, "OBJECT_LIVING") == 0)
			Object_Def[i].object_type = OBJECT_LIVING;

		if (strcmp((const char*)temp, "OBJECT_BUILDING") == 0)
			Object_Def[i].object_type = OBJECT_BUILDING;

		if (strcmp((const char*)temp, "OBJECT_NORMAL") == 0)
			Object_Def[i].object_type = OBJECT_NORMAL;

		if (strcmp((const char*)temp, "OBJECT_PLAYER") == 0)
			Object_Def[i].object_type = OBJECT_PLAYER;

		// DRAWING_TYPE

		fscanf(file, "%s", temp);

		Object_Def[i].drawing_type = LEVEL_INVALID;

		if (strcmp((const char*)temp, "LEVEL_ONE") == 0)
			Object_Def[i].drawing_type = LEVEL_ONE;

		if (strcmp((const char*)temp, "LEVEL_TWO") == 0)
			Object_Def[i].drawing_type = LEVEL_TWO;

		if (strcmp((const char*)temp, "LEVEL_THREE") == 0)
			Object_Def[i].drawing_type = LEVEL_THREE;

		ReadInteger(file, "SELECTABLE", &Object_Def[i].nSelectable);
		ReadInteger(file, "ANIMATIONS", &Object_Def[i].nAnimations);

		if (Object_Def[i].nAnimations >= 1 && Object_Def[i].nAnimations <= MAX_ANIM)
		{
			// Animations
			for (int j = 0; j != Object_Def[i].nAnimations; j++)
			{			
				Object_Def[i].pAnims[j] = new ZeldaAnimation();

				if (Object_Def[i].pAnims[j] == NULL)
				{
					DEBUG("LoadObjectDefs: new ZeldaAnimation()");
					fclose(file);
					return false;
				}

				int nFrames = 0, nFrameDelay = 0;
				fscanf(file, "%d", &nFrames);     // Frames
				fscanf(file, "%d", &nFrameDelay); // Delay

				// Frames laden
				for (int k = 0; k != nFrames; k++)
				{					
					sprintf(temp, "dat\\gfx\\%s_%d%d.bmp", Object_Def[i].name, j, k);

					if (!Object_Def[i].pAnims[j]->AddFrame(LoadSurface(temp)))
					{
						DEBUG("LoadObjectDefs: AddFrame()");
						fclose(file);
						return false;
					}

					// Frame Delay setzen
					Object_Def[i].pAnims[j]->SetFrameDelay(nFrameDelay);
				}
			}  // Animationen

			if (!ReadInteger(file, "DEFAULT_ANIMATION", &Object_Def[i].nDefaultAnim))
				return false;
			
			// Überprüfe nDefaultAnim auf Plausibilität
			if (Object_Def[i].nDefaultAnim >= MAX_ANIM || Object_Def[i].nDefaultAnim < -2)
				Object_Def[i].nDefaultAnim = -1;  // keine Animation
		}

		if (!ReadInteger(file, "SOLID_MASK", &Object_Def[i].nSolidMask))
			return false;

		// Solid Mask Bild laden
		if (Object_Def[i].nSolidMask == 1)
		{
			sprintf(temp, "dat\\gfx\\sm_%s.bmp", Object_Def[i].name);

			if ((Object_Def[i].pSolidMask = LoadSurface(temp)) == NULL)
				return false;
		}

		if (!ReadInteger(file, "PORTAL", &Object_Def[i].nPortal))
			return false;

		if (Object_Def[i].nPortal != 0)
		{
			if (!ReadInteger(file, "OFFSET_X", &Object_Def[i].x_offset_port) ||
			    !ReadInteger(file, "OFFSET_Y", &Object_Def[i].y_offset_port) ||
				!ReadInteger(file, "PORT_WIDTH", &Object_Def[i].port_width)  ||
				!ReadInteger(file, "PORT_HEIGHT", &Object_Def[i].port_height))
				return false;
		}

		if (!ReadInteger(file, "LIFTABLE", &Object_Def[i].nLiftable))
			return false;

		if (!ReadInteger(file, "BREAKABLE", &Object_Def[i].nBreakable))
			return false;

		if (Object_Def[i].nBreakable == 1)
		{
			if (!ReadString(file, "BREAK_SOUND", Object_Def[i].break_sound))
				return false;

			// Lade Break Sound
			Object_Def[i].pSoundBreak = new CDXSoundBuffer();

			if (Object_Def[i].pSoundBreak != NULL)
			{
				char file_name[515];
				sprintf(file_name, "dat\\snd\\%s", Object_Def[i].break_sound);
				if (FAILED(Object_Def[i].pSoundBreak->Load(Game->Sound, respack_interface((const char*)file_name))))
					DEBUG("LoadObjectDefs : pSoundBreak->Load()");		
			}
		}

		if (!ReadInteger(file, "BREAK_ANIMATION", &Object_Def[i].nBreakAnim))
			return false;

		if (!ReadInteger(file, "SCRIPT", &Object_Def[i].nScript))
			return false;

		if (!ReadInteger(file, "MODULE", &Object_Def[i].nModule))
			return false;

		// Module Type
		if (Object_Def[i].nModule == 1)
		{
			if (!ReadString(file, "MODULE_TYPE", temp))
				return false;

			if (strcmp("MODULE_CHICKEN", temp) == 0)
				Object_Def[i].module_type = MODULE_CHICKEN;
			else if (strcmp("MODULE_SOLDIER", temp) == 0)
				Object_Def[i].module_type = MODULE_SOLDIER;
			else if (strcmp("MODULE_HIGHGRAS", temp) == 0)
				Object_Def[i].module_type = MODULE_HIGHGRAS;
			else
				Object_Def[i].module_type = MODULE_INVALID;
		}

		if (!ReadInteger(file, "LAYER", &Object_Def[i].nLayer))
			return false;

		if (Object_Def[i].nLayer == 1)
		{
			if (!ReadInteger(file, "USE_LAYER_GFX", &Object_Def[i].nUseLayerGfx))
				return false;

			if (Object_Def[i].nUseLayerGfx == 1)
			{
				if (!ReadString(file, "LAYER_GFX", temp))
					return false;

				// $BASE_GFX$
				if (strcmp(temp, "$BASE_GFX$") == 0)
					Object_Def[i].pLayer = Object_Def[i].pSurface;
				else
				{
					sprintf(temp2, "dat\\gfx\\%s", temp);

					if ((Object_Def[i].pLayer = LoadSurface(temp2)) == NULL)
						return false;
				}
			}
			// Wenn keine separate Layer Grafik benutzt wird, dann Koordinaten für die Hauptgrafik ermitteln
			else
			{
				// LAYER_COORDS
				fscanf(file, "%s", temp);

				if (strcmp(temp, "LAYER_COORDS") != 0)
				{
					DEBUG("LoadObjectDefs : Error at LAYER_COORDS\n");

					fclose(file);
					return false;
				}

				fscanf(file, "%d", &Object_Def[i].nLayerCoords[0]);
				fscanf(file, "%d", &Object_Def[i].nLayerCoords[1]);
				fscanf(file, "%d", &Object_Def[i].nLayerCoords[2]);
				fscanf(file, "%d", &Object_Def[i].nLayerCoords[3]);

				DEBUG("LAYER_COORDS=%d %d %d %d\n",Object_Def[i].nLayerCoords[0],
					Object_Def[i].nLayerCoords[1],Object_Def[i].nLayerCoords[2],Object_Def[i].nLayerCoords[3]);
			}
		}

		fclose(file);
		Game->nObjDefsLoaded++;

		DEBUG("]\n");
	}

	return true;
}

int GetObjectDefCount(void)
{
	int i;
	for (i = 0; strcmp((const char*)ObjDefNames[i],"ENDLIST") != 0;)
	{
		i++;
	}

	return i;
}

int GetWorldDefCount(void)
{
	int i;
	for (i = 0; strcmp((const char*)WorldDefNames[i],"ENDLIST") != 0;)
	{
		i++;
	}

	return i;
}

_Object_Def* GetObjectDef(const char* name)
{
	for (int i = 0; i != Game->nObjDefsLoaded; i++)
	{
		if (strcmp((const char*)Object_Def[i].name,name) == 0)
			return &Object_Def[i];
	}

	return NULL;
}

bool ObjectInWorldViewRect(ZeldaObject* object)
{
	if (object != NULL && object->GetXKoord() <= Game->GetActiveWorld()->GetWorldViewRect().right &&
		object->GetXKoord()+object->GetWidth()  >= Game->GetActiveWorld()->GetWorldViewRect().left &&
		object->GetYKoord()+object->GetHeight() >= Game->GetActiveWorld()->GetWorldViewRect().top &&
		object->GetYKoord() <= Game->GetActiveWorld()->GetWorldViewRect().bottom)
		return true;

	return false;
}

void MoveWorldViewRectRight(int abs)
{
	if (Game->GetActiveWorld() != NULL && Game->GetActiveWorld()->GetWorldViewRect().right <= Game->GetActiveWorld()->GetWidth()-abs)
		Game->GetActiveWorld()->SetWorldViewRect(Game->GetActiveWorld()->GetWorldViewRect().left+abs,
		Game->GetActiveWorld()->GetWorldViewRect().top);
}

void MoveWorldViewRectLeft(int abs)
{
	if (Game->GetActiveWorld() != NULL && Game->GetActiveWorld()->GetWorldViewRect().left >= abs)
		Game->GetActiveWorld()->SetWorldViewRect(Game->GetActiveWorld()->GetWorldViewRect().left-abs,
		Game->GetActiveWorld()->GetWorldViewRect().top);
}

void MoveWorldViewRectUp(int abs)
{
	if (Game->GetActiveWorld() != NULL && Game->GetActiveWorld()->GetWorldViewRect().top >= abs)
		Game->GetActiveWorld()->SetWorldViewRect(Game->GetActiveWorld()->GetWorldViewRect().left,
		Game->GetActiveWorld()->GetWorldViewRect().top-abs);
}

void MoveWorldViewRectDown(int abs)
{
	if (Game->GetActiveWorld() != NULL && Game->GetActiveWorld()->GetWorldViewRect().bottom <= Game->GetActiveWorld()->GetHeight()-abs)
		Game->GetActiveWorld()->SetWorldViewRect(Game->GetActiveWorld()->GetWorldViewRect().left,
		Game->GetActiveWorld()->GetWorldViewRect().top+abs);
}

const char* respack_interface(const char* file_name)
{
#ifdef ZELDA_RELEASE

	if (extract_file(ZELDA_ARCHIVE, file_name, ZELDA_ARCHIVE_FILE) == -1)
	{
		DEBUG("respack_interface: extract_file() == -1\n");
		return NULL;
	}

	return ZELDA_ARCHIVE_FILE;

#else

	return file_name;

#endif
}

bool unpack_data_archive(void)
{
	HANDLE hArcData;
	int RHCode,PFCode;
	int Mode = 0;
	char CmtBuf[16384];
	struct RARHeaderData HeaderData;
	struct RAROpenArchiveDataEx OpenArchiveData;

	memset(&OpenArchiveData,0,sizeof(OpenArchiveData));
	OpenArchiveData.ArcName="data.rar";
	OpenArchiveData.CmtBuf=CmtBuf;
	OpenArchiveData.CmtBufSize=sizeof(CmtBuf);
	OpenArchiveData.OpenMode=RAR_OM_EXTRACT;
	hArcData=RAROpenArchiveEx(&OpenArchiveData);

	if (OpenArchiveData.OpenResult!=0)
		return false;

	HeaderData.CmtBuf=NULL;

	while ((RHCode=RARReadHeader(hArcData,&HeaderData))==0)
	{	
		PFCode=RARProcessFile(hArcData, RAR_EXTRACT,NULL,NULL);
		if (PFCode!=0)
			break;
	}

	if (RHCode==ERAR_BAD_DATA)
		return false;

	RARCloseArchive(hArcData);

	return true;
}

CDXSurface* LoadSurface(const char* file_name)
{
	CDXSurface* pSurf = new CDXSurface();

	if (pSurf != NULL)
	{
		if (FAILED(pSurf->Create(Game->Screen, respack_interface(file_name))))
		{
			DEBUG("LoadSurface : pSurf->Create()\n");
			DEBUG("FILE        : ");
			DEBUG((char*)file_name);
			delete pSurf; return NULL;
		}

		pSurf->SetColorKey(0);
	}

	return pSurf;
}

CDXSoundBuffer* LoadSound(const char* file_name)
{
	if (Game->Sound == NULL)
		return NULL;

	CDXSoundBuffer* pSound = new CDXSoundBuffer();

	if (pSound != NULL)
	{
		if (FAILED(pSound->Load(Game->Sound, respack_interface(file_name))))
		{
			DEBUG("LoadSound : pSound->Load()\n");
			delete pSound; pSound = NULL;
		}
	}

	return pSound;
}

bool RectInRect(RECT rc1, RECT rc2)
{
	if (PointInRect(rc1.left,rc1.top,rc2) || PointInRect(rc1.right,rc1.top,rc2) ||
		PointInRect(rc1.left,rc1.bottom,rc2) || PointInRect(rc1.right,rc1.bottom,rc2))
		return true;

	// Für den Fall dass das Rechteck rc1 vollkommen bedechkt
	//
	//    rc1
	//     -----------
	//    |           |
	//    |  -----    |
	//    | |     |   |
	//    |  ----- rc2|
	//    |           |
	//     -----------
	if (PointInRect(rc2.left,rc2.top,rc1) || PointInRect(rc2.right,rc2.top,rc1) ||
		PointInRect(rc2.left,rc2.bottom,rc1) || PointInRect(rc2.right,rc2.bottom,rc1))
		return true;

	return false;
}

bool PointInRect(int x, int y, RECT rc)
{
	if (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom)
		return true;

	return false;
}

bool IsPlayerMoving(void)
{
	switch (wKeyStatus)
	{
		case VK_LEFT:  return true;
		case VK_RIGHT: return true;
		case VK_UP:    return true;
		case VK_DOWN:  return true;;
	}

	LONG JoyX , JoyY;

	if (Game->Input != NULL && Game->bHasJoystick && !Game->bEditor) 
	{
		Game->Input->GetJoystickDeltas(&JoyX, &JoyY);

		if (JoyX > 0 || JoyX < 0 || JoyY > 0 || JoyY < 0) return true;
	}

	return false;
}

bool IsLeftMouseButtonDown(void)
{
	return bLButtonDown;
}

int GetMouseKoordX(void)
{
	return mouse_pos_x;
}

int GetMouseKoordY(void)
{
	return mouse_pos_y;
}

RECT TransformRectToScreen(RECT rc)
{
	if (Game->GetActiveWorld() != NULL)
	{
		rc.left   = X_KOORD(rc.left);
		rc.top    = Y_KOORD(rc.top);
		rc.right  = X_KOORD(rc.right);
		rc.bottom = Y_KOORD(rc.bottom);
	}

	return rc;
}

/*
 * CheckSolid
 *
 * Die Welt besteht nun mal aus festen Objekten, so dass das kleine Huhn nicht überall hingehen
 * kann. Man kann es auf diese Weise in ein Gehege einsperren. Pech fürs Huhn ;-)
 *
 */

bool CheckSolid(ZeldaWorld* pWorld, int x, int y)
{
	ZeldaObject*     pObj  = NULL;
	CDXSurface*      pSurf = NULL;
	ZeldaObjectList* pList = NULL;

	int x_koord, y_koord;

	if (pWorld == NULL)
		return false;

	pList = pWorld->GetObjectList();

	for (int i = 0; i != pList->GetElementCount(); i++)
	{
		if ((pObj = pList->GetElementByIndex(i)) == NULL)
			continue;

		if (CheckPortal(pObj)) return false;

		// Objekte, die gerade zerbrechen oder überhaupt nicht fest sind
		// werden übersprungen
		if (pObj->IsBreaking() || pObj->GetObjectDef()->nSolidMask == 0 || pObj == Game->Link->GetLiftObject())
			continue;
		else
		{
			if (PointInRect(x,y,pObj->GetRect()))
			{
				x_koord = x - pObj->GetXKoord();
				y_koord = y - pObj->GetYKoord();			

				if (pObj->GetObjectDef()->nSolidMask == 1)
					pSurf = pObj->GetObjectDef()->pSolidMask;  // Solid Mask Bild
				else
					pSurf = pObj->GetObjectDef()->pSurface;    // Original Bild
				
				pSurf->Lock();

				if (pSurf != NULL && pSurf->GetPixel(x_koord,y_koord) != 0)
				{
					pSurf->UnLock();
					return true;
				}

				pSurf->UnLock();
			}
		}
	}

	return false;
}

bool CheckPortal(ZeldaObject *pObj)
{                                                                   // ???
	if (pObj->GetPortal() == NULL || pObj->GetObjectList()->GetWorld() != Game->GetActiveWorld())
		return false;

	if (RectInRect(Game->Link->GetRect(), pObj->GetPortal()->GetRect()))
	{
		pPortal = pObj->GetPortal();
		bChangeWorld = true;

		if (Game->Link->GetLiftObject() != NULL)
		{
			Game->Link->GetLiftObject()->GetObjectList()->DestroyElement(Game->Link->GetLiftObject());
			Game->Link->pLiftObject = NULL;
		}


/*
		// Objekt in die andere Welt mitnehmen
		if (GetLiftObject() != NULL)
		{
			GetLiftObject()->GetObjectList()->RemovePointerFromList(GetLiftObject()->GetObjectList()->GetIndex(GetLiftObject()));
			
			//Game->GetWorldByName(pPortal->GetWorld())->GetObjectList()->AddElement(GetLiftObject());
			
			// Unser Objekt zwischenspeichern
			pTransObj   = pLiftObject;
			pLiftObject = NULL;
		}
*/
		return true;
	}
	
	return false;
}
