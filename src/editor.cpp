/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: editor.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\editor.h"

///////////////////////////////
// Editor spezifische Variablen
///////////////////////////////
ZeldaObject*      SelectedObject     = NULL;
ZeldaObject*      PraeSelectedObject = NULL;
ZeldaObjWnd*      EditorObjWnd       = NULL; 
ZeldaBoundaryBox* BoundaryBox        = NULL;
DRAWING_TYPE      EditorDrawingType  = LEVEL_ONE;

/* Liegen mehrere Objekte mit gleichem DRAWING_TYPE übereinander, kann mit der objekt_stufe
   eine Ebene höher oder tiefer gelangen (im Editor Modus).
*/
int objekt_stufe = 0;

ZeldaObjWnd::ZeldaObjWnd()
{
	width  = SCREEN_X;
	height = SCREEN_Y / 6;

	title_height = height / 5;
	
	x = 0;
	y = SCREEN_Y - height;

	scroll_btn_width = 20;
	title_width  = (width - (2*scroll_btn_width)) / 4;

	active_obj_type = OBJECT_NATURE;
	obj_index = 0;
}

ZeldaObjWnd::~ZeldaObjWnd()
{

}

void ZeldaObjWnd::Update(void)
{
	DrawWindow();
	DrawObjects();
}

void ZeldaObjWnd::DrawWindow(void)
{
	// Ecken zeichnen
	Game->GetActiveWorld()->GetSurface()->FillRect(x, y, x+width-(4*title_width)-scroll_btn_width, y+title_height, 0xc0c0c0);
	Game->GetActiveWorld()->GetSurface()->FillRect(x+width-scroll_btn_width, y, x+width, y+title_height, 0xc0c0c0);

	// Hauptrahmen zeichnen
	Game->GetActiveWorld()->GetSurface()->Rect(x, y, x+width, y+height, 0x000000);
	Game->GetActiveWorld()->GetSurface()->FillRect(x+scroll_btn_width, y+title_height, x+width-scroll_btn_width, y+height, 0xc0c0c0);

	// Scroll Button Rahmen zeichnen
	Game->GetActiveWorld()->GetSurface()->FillRect(x, y+title_height, x+scroll_btn_width, y+height, 0x000000);
	Game->GetActiveWorld()->GetSurface()->FillRect(x+width-scroll_btn_width, y+title_height, x+width, y+height, 0x000000);

	// Zeichne alle vier Titelrahmen
	Game->GetActiveWorld()->GetSurface()->FillRect(x+scroll_btn_width, y, x+scroll_btn_width+title_width, y+title_height, 0x000000);
	Game->GetActiveWorld()->GetSurface()->FillRect(x+scroll_btn_width+title_width, y, x+scroll_btn_width+2*title_width, y+title_height, 0x000000);
	Game->GetActiveWorld()->GetSurface()->FillRect(x+scroll_btn_width+2*title_width, y, x+scroll_btn_width+3*title_width, y+title_height, 0x000000);
	Game->GetActiveWorld()->GetSurface()->FillRect(x+scroll_btn_width+3*title_width, y, x+scroll_btn_width+4*title_width, y+title_height, 0x000000);

	// Schreibe alle vier Titelüberschriften
	char szText[256];
	
	Game->Screen->GetBack()->GetDC();

	strcpy(szText, "NATURE");
	Game->Screen->GetBack()->TextXY(x+scroll_btn_width, y, 0x0000FFF0, szText);

	strcpy(szText, "OBJECT");
	Game->Screen->GetBack()->TextXY(x+scroll_btn_width+title_width, y, 0x0000FFF0, szText);

	strcpy(szText, "BUILDING");
	Game->Screen->GetBack()->TextXY(x+scroll_btn_width+2*title_width, y, 0x0000FFF0, szText);

	strcpy(szText, "LIVING");
	Game->Screen->GetBack()->TextXY(x+scroll_btn_width+3*title_width, y, 0x0000FFF0, szText);

	Game->Screen->GetBack()->ReleaseDC();
}

_Object_Def* ZeldaObjWnd::GetObjectDef(int rel_index, OBJECT_TYPE obj_type)
{
	int count = 0;

	for (int i = 0; i != GetObjectDefCount(); i++)
	{
		if (Object_Def[i].object_type == obj_type && Object_Def[i].nSelectable == 1)
		{
			if (count == rel_index)
				return &Object_Def[i];
			
			count++;
		}
	}

	return NULL;
}

void ZeldaObjWnd::DrawObjects(void)
{
	int objs  = 0;
	int count = 0;

	for (int i = 0; strcmp((const char*)ObjDefNames[i],"ENDLIST") != 0; i++)
	{
		if (Object_Def[i].pSurface != NULL)
		{
			if (Object_Def[i].object_type == active_obj_type && Object_Def[i].nSelectable)
			{
				if (count >= obj_index)
				{
					Object_Def[i].pSurface->DrawBlkStretched(Game->Screen->GetBack(),x+scroll_btn_width+objs*title_width,y+title_height,NULL,title_width,height-title_height);

					// Draw object name
					Game->Screen->GetBack()->GetDC();
					Game->Screen->GetBack()->TextXY(x+scroll_btn_width+objs*title_width,y+height-title_height, 0x0000FFF0, Object_Def[i].name);
					Game->Screen->GetBack()->ReleaseDC();
					
					objs++;
				}

				count++;
			}
		}

		if (objs == 4) return;
	}
}

RECT ZeldaObjWnd::GetRect(void)
{
	RECT rc;

	rc.left   = x;
	rc.top    = y;
	rc.right  = x+width;
	rc.bottom = y+height;

	return rc;
}

int ZeldaObjWnd::CheckClickedTitle(int x_pos, int y_pos)
{
	int k = 0, l = 1;

	for (; k != 4; k++, l++)
	{	
		if (x_pos >= x+scroll_btn_width+k*title_width && x_pos <= x+scroll_btn_width+l*title_width &&
			y_pos >= y && y_pos <= y+title_height)
			return k;
	}

	return -1;
}

bool ZeldaObjWnd::CheckClickedObject(int x_pos, int y_pos)
{
	int k = 0, l = 1;
	
	for (; k != 4; k++, l++)
	{
		if (x_pos >= x+scroll_btn_width+k*title_width && x_pos <= x+scroll_btn_width+l*title_width &&
			y_pos >= y+title_height && y_pos <= y+height)
		{
			if (GetObjectDef(obj_index+k, active_obj_type)->name != NULL)
			{
				strcpy(obj_name, GetObjectDef(obj_index+k, active_obj_type)->name);
				return true;
			}
		}
	}

	return false;
}

void ZeldaObjWnd::DoClick(int x_pos, int y_pos)
{
	// Überprüfe linken Button
	if (x_pos >= x && x_pos <= x+scroll_btn_width && 
		y_pos >= y+title_height && y_pos <= y+height)
	{
		if (obj_index > 0) obj_index--;

		return;
	}

	// Überprüfe rechten Button
	if (x_pos >= x+width-scroll_btn_width && x_pos <= x+width && 
		y_pos >= y+title_height && y_pos <= y+height)
	{
		obj_index++;

		return;
	}

	switch (CheckClickedTitle(x_pos, y_pos))
	{
	case 0: active_obj_type = OBJECT_NATURE;   obj_index = 0; return;
	case 1: active_obj_type = OBJECT_NORMAL;   obj_index = 0; return;
	case 2: active_obj_type = OBJECT_BUILDING; obj_index = 0; return;
	case 3: active_obj_type = OBJECT_LIVING;   obj_index = 0; return;
	}

	if (CheckClickedObject(x_pos, y_pos))
	{
		ZeldaObject* newObj = ZeldaObject::CreateObject(
			obj_name, x_pos+Game->GetActiveWorld()->GetWorldViewRect().left, 
			y_pos+Game->GetActiveWorld()->GetWorldViewRect().top, 
			Game->GetActiveWorld());

		EditorDrawingType = newObj->GetDrawingType();
		SelectedObject    = newObj;
	}
}

ZeldaBoundaryBox::ZeldaBoundaryBox()
{
	for (int i = 0; i != 4; i++)
		PullPoints[i] = new ZeldaPullPoint(this);

	DeletePositionsArray();
}

ZeldaBoundaryBox::~ZeldaBoundaryBox()
{
	for (int i = 0; i != 4; i++)
	{
		if (PullPoints[i] != NULL) 
			delete PullPoints[i];
		PullPoints[i] = NULL;
	}
}

void ZeldaBoundaryBox::Update(void)
{
	if (SelectedObject == NULL)
		return;

	// Zeichne BoundaryRect
	pos_x  = X_KOORD(SelectedObject->GetXKoord());
	pos_y  = Y_KOORD(SelectedObject->GetYKoord());
	width  = SelectedObject->GetWidth();
	height = SelectedObject->GetHeight();

	ZeldaPullPoint* pSelectPullPoint = NULL;

	// Wurde ein Ziehpunkt ausgewählt?
	if ((pSelectPullPoint = GetSelectedPullPoint()) != NULL)
	{
		// Zieh Abstand berechnen
		int offset_x = pSelectPullPoint->click_point_x - GetMouseKoordX();
		int offset_y = pSelectPullPoint->click_point_y - GetMouseKoordY();

		LONG lDestX, lDestY;
		RECT rc;

		// Ziehpunkt links
		if (pSelectPullPoint == PullPoints[0])
		{
			if (offset_x > 0)
			{
				pos_x -= offset_x;
				width += offset_x;

				int anz_obj = offset_x/SelectedObject->GetWidth();

				// Liste löschen
				DeletePositionsArray();

				for (int i = 0; i != anz_obj; i++)
				{
					rc.left   = 0; rc.top = 0;
					rc.right  = SelectedObject->GetWidth();
					rc.bottom = SelectedObject->GetHeight();
	
					lDestX = SelectedObject->GetXKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().left-SelectedObject->GetWidth()-i*SelectedObject->GetWidth();
					lDestY = SelectedObject->GetYKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().top;
		
					SelectedObject->GetSurface()->DrawTrans(Game->Screen->GetBack(), lDestX, lDestY, &rc);

					pos_x_array[i] = SelectedObject->GetXKoord()-SelectedObject->GetWidth()-i*SelectedObject->GetWidth();
					pos_y_array[i] = SelectedObject->GetYKoord();
				}
			}
		}

		// Ziehpunkt oben
		if (pSelectPullPoint == PullPoints[1])
		{
			if (offset_y > 0)
			{
				pos_y -= offset_y;
				height += offset_y;

				int anz_obj = offset_y/SelectedObject->GetHeight();

				// Liste löschen
				DeletePositionsArray();

				for (int i = 0; i != anz_obj; i++)
				{
					rc.left   = 0; rc.top = 0;
					rc.right  = SelectedObject->GetWidth();
					rc.bottom = SelectedObject->GetHeight();
	
					lDestX = SelectedObject->GetXKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().left;
					lDestY = SelectedObject->GetYKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().top-SelectedObject->GetHeight()-i*SelectedObject->GetHeight();
		
					SelectedObject->GetSurface()->DrawTrans(Game->Screen->GetBack(), lDestX, lDestY, &rc);

					pos_x_array[i] = SelectedObject->GetXKoord();
					pos_y_array[i] = SelectedObject->GetYKoord()-SelectedObject->GetHeight()-i*SelectedObject->GetHeight();
				}
			}
		}

		// Ziehpunkt rechts
		if (pSelectPullPoint == PullPoints[2])
		{
			if (offset_x < 0)
			{
				width += abs(offset_x);

				int anz_obj = abs(offset_x)/SelectedObject->GetWidth();

				// Liste löschen
				DeletePositionsArray();

				for (int i = 0; i != anz_obj; i++)
				{
					rc.left   = 0; rc.top = 0;
					rc.right  = SelectedObject->GetWidth();
					rc.bottom = SelectedObject->GetHeight();
	
					lDestX = SelectedObject->GetXKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().left+SelectedObject->GetWidth()+i*SelectedObject->GetWidth();
					lDestY = SelectedObject->GetYKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().top;
		
					SelectedObject->GetSurface()->DrawTrans(Game->Screen->GetBack(), lDestX, lDestY, &rc);

					pos_x_array[i] = SelectedObject->GetXKoord()+SelectedObject->GetWidth()+i*SelectedObject->GetWidth();
					pos_y_array[i] = SelectedObject->GetYKoord();
				}
			}
		}
		

		// Ziehpunkt unten
		if (pSelectPullPoint == PullPoints[3])
		{
			if (offset_y < 0)
			{
				height += abs(offset_y);

				int anz_obj = abs(offset_y)/SelectedObject->GetHeight();

				// Liste löschen
				DeletePositionsArray();

				for (int i = 0; i != anz_obj; i++)
				{
					rc.left   = 0; rc.top = 0;
					rc.right  = SelectedObject->GetWidth();
					rc.bottom = SelectedObject->GetHeight();
	
					lDestX = SelectedObject->GetXKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().left;
					lDestY = SelectedObject->GetYKoord() - SelectedObject->GetObjectList()->GetWorld()->GetWorldViewRect().top+SelectedObject->GetHeight()+i*SelectedObject->GetHeight();
		
					SelectedObject->GetSurface()->DrawTrans(Game->Screen->GetBack(), lDestX, lDestY, &rc);

					pos_x_array[i] = SelectedObject->GetXKoord();
					pos_y_array[i] = SelectedObject->GetYKoord()+SelectedObject->GetHeight()+i*SelectedObject->GetHeight();
				}
			}
		}
	}
	else
	{
		// Objekte erschaffen
		if (pos_x_array[0] != -1)  // Liste leer?
		{
			for (int i = 0; pos_x_array[i] != -1 && i != MAX_OBJ_ARRAY; i++)
				ZeldaObject::CreateObject(SelectedObject->GetObjectName(), 
					pos_x_array[i],pos_y_array[i], Game->GetActiveWorld());
			
			// Liste löschen
			DeletePositionsArray();

			// Objekte sortieren
			ZeldaObjectList::SortObjects(Game->GetActiveWorld()->GetObjectList());
		}
	}
	
	// BoundaryBox zeichnen
	Game->GetActiveWorld()->GetSurface()->RoundedRect(pos_x, pos_y, pos_x+width, pos_y+height, 20,0xff0000);

	// Positionen der PullPoints berechnen

	// links
	if (PullPoints[0] != NULL) 
		PullPoints[0]->SetLocation(pos_x-PullPoints[0]->GetWidth()/2,pos_y+height/2);

	// oben
	if (PullPoints[1] != NULL) 
		PullPoints[1]->SetLocation(pos_x+width/2,pos_y-PullPoints[1]->GetHeight()/2);

	// rechts
	if (PullPoints[2] != NULL) 
		PullPoints[2]->SetLocation(pos_x+width-PullPoints[2]->GetWidth()/2,pos_y+height/2);

	// unten
	if (PullPoints[3] != NULL) 
		PullPoints[3]->SetLocation(pos_x+width/2,pos_y+height-PullPoints[3]->GetHeight()/2);


	// PullPoints zeichnen
	for (int i = 0; i != 4; i++)
		if (PullPoints[i] != NULL)
			PullPoints[i]->Update();
}

bool ZeldaBoundaryBox::DoClick(int x, int y)
{
	// Alle PullPoints deselektieren
	for (int i = 0; i != 4; i++)
		if (GetPullPoint(i) != NULL)
			GetPullPoint(i)->Select(false);

	// Überprüfen ob ein PullPoint angeklickt wurde
	for (int i = 0; i != 4; i++)
	{
		if (PointInRect(x, y, GetPullPoint(i)->GetRect()))
		{
			GetPullPoint(i)->Select(true);	
			return true;
		}
	}

	// Überprüfen ob BoundaryBox angeklickt wurde
	if (PointInRect(x, y, GetRect()))
		return true;

	return false;
}

ZeldaPullPoint* ZeldaBoundaryBox::GetSelectedPullPoint(void)
{
	for (int i = 0; i != 4; i++)
		if (GetPullPoint(i) != NULL && GetPullPoint(i)->Selected())
			return GetPullPoint(i);
			
	return NULL;
}

ZeldaPullPoint::ZeldaPullPoint(ZeldaBoundaryBox* pBox)
{
	this->pBox = pBox;
	bPull = false;

	pos_x = 0;
	pos_y = 0;

	width  = 10;
	height = 10;

	col = 0x00ff00;  // Grün
}

ZeldaPullPoint::~ZeldaPullPoint()
{

}

void ZeldaPullPoint::Update(void)
{
	if (SelectedObject == NULL)
		return;

	if (Selected() && !IsLeftMouseButtonDown())
		Select(false);

	Game->GetActiveWorld()->GetSurface()->Rect(pos_x, pos_y, pos_x+width, pos_y+height,col);
}

void DrawPortalBox(void)
{
#ifndef ZELDA_RELEASE

	if (SelectedObject == NULL || SelectedObject->GetPortal() == NULL)
		return;

	Game->GetActiveWorld()->GetSurface()->RoundedRect(
		X_KOORD(SelectedObject->GetPortal()->GetXKoord()), 
		Y_KOORD(SelectedObject->GetPortal()->GetYKoord()), 
		X_KOORD(SelectedObject->GetPortal()->GetXKoord())+SelectedObject->GetPortal()->GetWidth(), 
		Y_KOORD(SelectedObject->GetPortal()->GetYKoord())+SelectedObject->GetPortal()->GetHeight(), 
		20,0x0000ff);

#endif
}

void PrintEditorStatus(void)
{
#ifndef ZELDA_RELEASE

	char szText[256], draw_type[256];

	Game->Screen->GetBack()->GetDC();
		
	switch (EditorDrawingType)
	{
		case LEVEL_ONE:
			strcpy(draw_type, "LEVEL_ONE");
		break;
	
		case LEVEL_TWO:
			strcpy(draw_type, "LEVEL_TWO");
		break;

		case LEVEL_THREE:
			strcpy(draw_type, "LEVEL_THREE");
		break;

		default:
			strcpy(draw_type, "LEVEL_INVALID");
		break;
	}

	if (SelectedObject != NULL)
	{
		sprintf(szText, "EDITOR MODE LINK X=%d Y=%d WORLD=%s DRAWING_TYPE=%s  OBJECT=%s LISTID=%d STUFE=%d  X=%d  Y=%d", 
			Game->Link->GetXKoord(), Game->Link->GetYKoord(),
			Game->GetActiveWorld()->GetWorldName(), 
			draw_type, SelectedObject->GetObjectName(), 
			SelectedObject->GetObjectList()->GetIndex(SelectedObject),
			objekt_stufe, SelectedObject->GetXKoord(), SelectedObject->GetYKoord());
	}
	else
	{
		sprintf(szText, "EDITOR MODE LINK X=%d Y=%d WORLD=%s DRAWING_TYPE=%s",		
			Game->Link->GetXKoord(), Game->Link->GetYKoord(),
			Game->GetActiveWorld()->GetWorldName(), draw_type);
	}

	Game->Screen->GetBack()->TextXY(0, 0, 0x0000FFF0, szText);
	Game->Screen->GetBack()->ReleaseDC();

#endif
}