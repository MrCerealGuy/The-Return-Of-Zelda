/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: editor.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/


#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "zelda.h"

#define MAX_OBJ_ARRAY 100

class ZeldaBoundaryBox;
class ZeldaObjWnd;
class ZeldaObject;

extern ZeldaObject*      SelectedObject;
extern ZeldaObject*      PraeSelectedObject;
extern ZeldaObjWnd*      EditorObjWnd;
extern ZeldaBoundaryBox* BoundaryBox;
extern DRAWING_TYPE      EditorDrawingType;
extern int               objekt_stufe;

void PrintEditorStatus(void);
void DrawPortalBox(void);

/*
 * class ZeldaObjWnd
 *
 * ZeldaObjWnd ist unser Objektfenster, aus dem ein Objekt einer bestimmten Kategorie
 * per Drag & Drop auf die Spielewelt gezogen werden kann.
 *
 */

class ZeldaObjWnd
{
public:
	ZeldaObjWnd();
	~ZeldaObjWnd();

	void Update(void);
	RECT GetRect(void);
	void DoClick(int x_pos, int y_pos);

private:
	void DrawWindow(void);
	void DrawObjects(void);
	int CheckClickedTitle(int x_pos, int y_pos);
	bool CheckClickedObject(int x_pos, int y_pos);
	_Object_Def* GetObjectDef(int rel_index, OBJECT_TYPE obj_type);

private:
	int x, y;
	int width, height;
	int title_height;
	int title_width;
	int scroll_btn_width;

	OBJECT_TYPE active_obj_type;
	int obj_index;
	char obj_name[32];
};

/*
 * class ZeldaPullPoint
 *
 * Ist ein Objekt in der Spielewelt ausgewählt, so ist es möglich über die Ziehpunkte
 * das Objekt in eine Richtung zu kopieren. Objekte werden erst erstellt wenn der
 * Ziehpunkt losgelassen wurde.
 *
 */

class ZeldaPullPoint
{
public:
	ZeldaPullPoint(ZeldaBoundaryBox* pBox);
	~ZeldaPullPoint();

	inline int GetKoordX(void);
	inline int GetKoordY(void);
	inline int GetWidth(void);
	inline int GetHeight(void);
	inline void SetLocation(int x, int y);
	inline RECT GetRect(void);
	inline void Select(bool bSel);
	inline bool Selected(void);

	void Update(void);

	int click_point_x;
	int click_point_y;

private:
	ZeldaBoundaryBox* pBox;
	bool bPull;

	int pos_x;
	int pos_y;
	int width;
	int height;

	DWORD col;
};

inline int ZeldaPullPoint::GetKoordX(void)
{
	return pos_x;
}

inline int ZeldaPullPoint::GetKoordY(void)
{
	return pos_y;
}

inline int ZeldaPullPoint::GetWidth(void)
{
	return width;
}

inline int ZeldaPullPoint::GetHeight(void)
{
	return height;
}

inline void ZeldaPullPoint::SetLocation(int x, int y)
{
	pos_x = x;
	pos_y = y;
}

inline RECT ZeldaPullPoint::GetRect(void)
{
	RECT rc;
	rc.left   = pos_x;
	rc.top    = pos_y;
	rc.right  = pos_x+width;
	rc.bottom = pos_y+height;

	return rc;
}

inline void ZeldaPullPoint::Select(bool bSel)
{
	if (bSel)
	{
		bPull = true;
		col   = 0x0000ff;

		click_point_x = GetMouseKoordX();
		click_point_y = GetMouseKoordY();
	}
	else
	{
		bPull = false;
		col   = 0x00ff00;
	}
}

inline bool ZeldaPullPoint::Selected(void)
{
	return bPull;
}

/*
 * class ZeldaBoundaryBox
 *
 * Ist ein Objekt auf der Spielewelt ausgewählt, so wird es von einm abgerundeten 
 * Rechteck umgeben. Auf diesem Rechteck befinden sich auch die Ziehpunkte. Das
 * Rechteck soll signalisieren dass das Objekt im Augenblick ausgewählt ist.
 *
 */

class ZeldaBoundaryBox
{
public:
	ZeldaBoundaryBox();
	~ZeldaBoundaryBox();

	inline int GetKoordX(void);
	inline int GetKoordY(void);
	inline int GetWidth(void);
	inline int GetHeight(void);
	inline ZeldaPullPoint* GetPullPoint(int index);
	inline void DeletePositionsArray(void);
	bool DoClick(int x, int y);
	inline RECT GetRect(void);
	ZeldaPullPoint* GetSelectedPullPoint(void);

	void Update(void);

private:
	ZeldaPullPoint* PullPoints[4];
	int pos_x_array[MAX_OBJ_ARRAY];
	int pos_y_array[MAX_OBJ_ARRAY];

	int pos_x;
	int pos_y;
	int width;
	int height;
};

inline int ZeldaBoundaryBox::GetKoordX(void)
{
	return pos_x;
}

inline int ZeldaBoundaryBox::GetKoordY(void)
{
	return pos_y;
}

inline int ZeldaBoundaryBox::GetWidth(void)
{
	return width;
}

inline int ZeldaBoundaryBox::GetHeight(void)
{
	return height;
}

inline ZeldaPullPoint* ZeldaBoundaryBox::GetPullPoint(int index)
{
	if (index >=0 && index <= 3)
		return PullPoints[index];

	return NULL;
}

inline RECT ZeldaBoundaryBox::GetRect(void)
{
	RECT rc;
	rc.left   = pos_x;
	rc.top    = pos_y;
	rc.right  = pos_x+width;
	rc.bottom = pos_y+height;

	return rc;
}

inline void ZeldaBoundaryBox::DeletePositionsArray(void)
{
	for (int i = 0; i != MAX_OBJ_ARRAY; i++)
		pos_x_array[i] = -1;
}

#endif