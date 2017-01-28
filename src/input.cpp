/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: input.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

WPARAM wKeyStatus;
bool bLButtonDown = false;
int mouse_pos_x = 0;
int mouse_pos_y = 0;

extern int log_scroll_offset;
extern int log_start_i;
extern int log_end_i;
extern int nLogCount;

void InputLiftThrowObject(void)
{
	if (Game->Link != NULL && Game->GetActiveWorld() != NULL)
	{
		if (Game->Link->GetLiftObject() == NULL)
			Game->Link->LiftObject();
		else
			Game->Link->ThrowObject();
	}
}

void InputSwingSword(void)
{
	if (Game->Link != NULL && Game->GetActiveWorld() != NULL)
		Game->Link->SwingSword();
}

void InputLeft(void)
{
	if (Game->GetActiveWorld() != NULL && !Game->bMiniMap)
	{
#ifndef ZELDA_RELASE
		if (Game->bEditor)													
			MoveWorldViewRectLeft(10);						
		else 
#endif
			if (Game->Link != NULL) Game->Link->MoveLeft();
	}
}

void InputRight(void)
{
	if (Game->GetActiveWorld() != NULL && !Game->bMiniMap)
	{
#ifndef ZELDA_RELEASE
		if (Game->bEditor)												
			MoveWorldViewRectRight(10);					
		else 
#endif
			if (Game->Link != NULL) Game->Link->MoveRight();
	}
}

void InputUp(void)
{
	if (Game->GetActiveWorld() != NULL && !Game->bMiniMap)
	{
#ifndef ZELDA_RELEASE
		if (Game->bEditor)
			MoveWorldViewRectUp(10);						
		else 
#endif
			if (Game->Link != NULL) Game->Link->MoveUp();
	}
}

void InputDown(void)
{
	if (Game->GetActiveWorld() != NULL && !Game->bMiniMap)
	{
#ifndef ZELDA_RELEASE
		if (Game->bEditor)
			MoveWorldViewRectDown(10);						
		else 
#endif
			if (Game->Link != NULL) Game->Link->MoveDown();
	}
}

void InputIncObjectLevel(void)
{
#ifndef ZELDA_RELEASE

	if (Game->bEditor)
	{
		if (objekt_stufe == 10)
			objekt_stufe = 0;
		else
			objekt_stufe++;
	}

#endif
}

void InputSwitchObject(void)
{
#ifndef ZELDA_RELEASE

	if (Game->bEditor && SelectedObject != NULL && PraeSelectedObject != NULL && !Game->bMiniMap)						
		Game->GetActiveWorld()->GetObjectList()->SwitchIndex(SelectedObject,PraeSelectedObject);

#endif
}

void InputDeleteObject(void)
{
#ifndef ZELDA_RELEASE

	if (Game->bEditor && !Game->bMiniMap && SelectedObject != NULL)
	{
		if (Game->GetActiveWorld()->GetObjectList()->DestroyElement(SelectedObject))
			SelectedObject = NULL;
		
		ZeldaObjectList::SortObjects(Game->GetActiveWorld()->GetObjectList());
	}

#endif
}

void InputMiniMap(void)
{
	if (Game->GetActiveWorld() == NULL) return;

	Game->bMiniMap = !Game->bMiniMap;

	if (Game->bMiniMap)
	{						
		if (Game->GetActiveWorld()->pMiniMap == NULL)
		{
			Game->GetActiveWorld()->pMiniMap = new CDXSurface();
					                       						
			if (Game->GetActiveWorld()->pMiniMap != NULL)						                       
				if (FAILED(Game->GetActiveWorld()->pMiniMap->Create(Game->Screen, Game->GetActiveWorld()->GetWidth(), Game->GetActiveWorld()->GetHeight())))						
					return;							
		}
	}
}

void InputDrawingType(void)
{
#ifndef ZELDA_RELEASE

	if (Game->bEditor && !Game->bMiniMap)
	{
		switch (EditorDrawingType)
		{
			case LEVEL_ONE:
				EditorDrawingType = LEVEL_TWO;
			break;

			case LEVEL_TWO:
				EditorDrawingType = LEVEL_THREE;
			break;

			case LEVEL_THREE:
				EditorDrawingType = LEVEL_ONE;
			break;
		}
	}

#endif
}

void InputWorldSave(void)
{
#ifndef ZELDA_RELEASE

#ifndef ZELDA_WINDOW

	if (Game->bEditor && !Game->bMiniMap && Game->GetActiveWorld() != NULL)
	{
		if (!Game->GetActiveWorld()->Save())
			DEBUG("SPEICHERN FEHLGESCHLAGEN!!!");	
	}

#endif

#endif

#ifdef ZELDA_WINDOW
	MessageBox(g_hWnd, "Speichern im Windows Modus nicht verfügbar!", "Fehler", MB_ICONEXCLAMATION);
#endif
}

void InputEditor(void)
{
#ifndef ZELDA_RELEASE

#ifndef ZELDA_WINDOW

	if (Game->bEditor && !Game->bMiniMap)
	{
		Game->bEditor = FALSE;
		ShowCursor(FALSE);
	}
	else if (!Game->bMiniMap)
	{
		SelectedObject = NULL;
		Game->bEditor = TRUE;
		ShowCursor(TRUE);
	}

#endif

#endif

#ifdef ZELDA_WINDOW
	MessageBox(g_hWnd, "Editor im Windows Modus nicht verfügbar!", "Fehler", MB_ICONEXCLAMATION);
#endif
}

void InputPrgExit(void)
{
	bActive = false;
	
#ifdef ZELDA_RELEASE
	DeleteFile(ZELDA_ARCHIVE_FILE);
	DeleteFile(ZELDA_ARCHIVE);
#endif

	// Wichtig!!!
	//if (Game != NULL) delete Game;

	SendMessage(g_hWnd, WM_CLOSE, 0, 0);
	PostQuitMessage(0);
}

void InputMouseMove(WPARAM wParam, LPARAM lParam, 
					int *x1, int *y1, int *x2, int *y2)
{
#ifndef ZELDA_RELEASE

#ifndef ZELDA_WINDOW

	if (Game->GetActiveWorld() == NULL) return;

	*x2 = *x1;
	*y2 = *y1;
	*x1 = mouse_pos_x = LOWORD(lParam);
	*y1 = mouse_pos_y = HIWORD(lParam);

	if (Game->bEditor && bLButtonDown && !Game->bMiniMap  && SelectedObject != NULL)
	{
		if (BoundaryBox != NULL && BoundaryBox->GetSelectedPullPoint() == NULL)
			SelectedObject->SetLocation(SelectedObject->GetXKoord()+(*x1-*x2),
				SelectedObject->GetYKoord()+(*y1-*y2));	
	}

#endif

#endif
}

void InputMouseLButtonDown(WPARAM wParam, LPARAM lParam)
{
#ifndef ZELDA_RELEASE

#ifndef ZELDA_WINDOW

	if (Game->bMiniMap || Game->GetActiveWorld() == NULL || EditorObjWnd == NULL) return;

	bLButtonDown = true;

	int x_pos = LOWORD(lParam);
	int y_pos = HIWORD(lParam);

	if (Game->bEditor)
	{
		if (SelectedObject != NULL)
		{
			// Überprüfe PullPoints
			if (BoundaryBox != NULL)
			{
				if (BoundaryBox->DoClick(x_pos, y_pos))
					return;
			}

			SelectedObject->Select(false);
			PraeSelectedObject = SelectedObject;
			SelectedObject     = NULL;
		}
		
		if (PointInRect(x_pos,y_pos,EditorObjWnd->GetRect()))
		{
			EditorObjWnd->DoClick(x_pos, y_pos);
			return;
		}

		ZeldaObject* obj = Game->GetActiveWorld()->GetObjectList()->GetElementByIndex(0);

		for (int i = 1; obj != NULL; i++) 
		{
			if (PointInRect(x_pos,y_pos,
				TransformRectToScreen(obj->GetRect())))
			{
				if (obj->Selectable() && obj->GetDrawingType() == EditorDrawingType)
				{
					if (objekt_stufe == 0)
					{
						obj->Select(true);
						SelectedObject = obj;
						return;
					}
					else objekt_stufe--;
				}
			}
					
			obj = Game->GetActiveWorld()->GetObjectList()->GetElementByIndex(i);
		}
	}

#endif

#endif
}

void InputMouseLButtonUp(void)
{
#ifndef ZELDA_RELEASE

#ifndef ZELDA_WINDOW

	if (Game->bMiniMap) 
		return;

	// Sortiere Objekte, damit sie in der richtigen
	// Reihenfolge gespeichert werden
	ZeldaObjectList::SortObjects(Game->GetActiveWorld()->GetObjectList());

	bLButtonDown = false;

#endif

#endif
}

void InputMouseRButtonDown(void)
{
}

void InputLogWindow(void)
{
	if (Game->bLogWindow)
		Game->bLogWindow = false;
	else
		Game->bLogWindow = true;
}

void InputLogPageUp(void)
{
	if (Game != NULL && Game->bLogWindow)
	{
		if (log_start_i >= (MAX_LOGS_PRO_PAGE))
		{
			log_start_i -= MAX_LOGS_PRO_PAGE;
			log_end_i   -= MAX_LOGS_PRO_PAGE;
		}
		else
		{
			log_start_i = 0;
			log_end_i   = MAX_LOGS_PRO_PAGE;
		}

	}
}

void InputLogPageDown(void)
{
	if (Game != NULL && Game->bLogWindow)
	{
		if ((log_end_i + MAX_LOGS_PRO_PAGE) < nLogCount)
		{
			log_start_i += MAX_LOGS_PRO_PAGE;
			log_end_i   += MAX_LOGS_PRO_PAGE;
		}
		else
		{
			log_start_i += nLogCount - log_end_i;
			log_end_i   += nLogCount - log_end_i;

		}
	}
}

void InputLogHome(void)
{
	if (Game != NULL && Game->bLogWindow)
	{
		log_start_i = 0;
		log_end_i   = MAX_LOGS_PRO_PAGE;
	}
}

void InputLogEnd(void)
{
	if (Game != NULL && Game->bLogWindow)
	{
		if (nLogCount < MAX_LOGS_PRO_PAGE)
			return;

		log_end_i   = nLogCount;
		log_start_i = nLogCount - MAX_LOGS_PRO_PAGE;
	}
}

void InputLogUp(void)
{
	if (Game != NULL && Game->bLogWindow)
	{
		if (log_start_i != 0)
		{
			log_start_i--;
			log_end_i--;
		}	
	}
}

void InputLogDown(void)
{
	if (Game != NULL && Game->bLogWindow)
	{
		// Ende erreicht?
		if (log_end_i == nLogCount)
			return;

		log_start_i++;
		log_end_i++;
	}
}