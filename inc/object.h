/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: object.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "zelda.h"
#include "module.h"

/*
 * class ZeldaObject
 *
 * ZeldaObject ist ein Datenmodul, das ein reales Objekt in einer Umgebung repräsentiert. Dies
 * kann zB ein Haus, Baum, Gegner, NPC und weiteres sein. Kann durch Module erweitert werden
 * um komplexere Objekte zu realisieren.
 *
 */

union _UnionModule
{
	ZeldaSoldierModule*  pSoldier;
	ZeldaChickenModule*  pChicken;
	ZeldaHighGrasModule* pHighGras;
};

class ZeldaObject
{
public:
	ZeldaObject(ZeldaObjectList* list);
	~ZeldaObject();

	bool LoadObject(const char* object_name);
	static ZeldaObject* CreateObject(const char* object_name, int x, int y, ZeldaWorld* pWorld);
	inline int GetWidth(void);
	inline int GetHeight(void);
	inline int GetXKoord(void);
	inline int GetYKoord(void);
	inline char* GetFileName(void);
	inline char* GetObjectName(void);
	inline OBJECT_TYPE GetObjectType(void);
	DRAWING_TYPE GetDrawingType(void);
	void SetLocation(const int& x, const int& y);
	void Update(void);
	void UpdateLayer(void);
	RECT GetRect(void);
	inline bool IsSelected(void);
	inline bool Selectable(void);
	inline void Select(bool sel);
	inline CDXSurface* GetSurface(void);
	_Object_Def* GetObjectDef(void);
	inline ZeldaPortal* GetPortal(void);
	bool InitPortal(const char* szToWorld);
	
	bool IsLiftable(void);
	bool IsBreakable(void);
	inline bool IsBreaking(void);
	bool BreakIt(void);                      // wird aufgerufen, wenn das Objekt fallgelassen wurde

	// Spez. Funktionen für Module
	inline ZeldaModule* GetModule(void);
	inline MODULE GetModuleType(void);
	inline bool IsChicken(void);
	inline bool IsSoldier(void);
	inline bool IsHighGras(void);
	void UpdateModule(void);
	
	inline ZeldaObjectList* GetObjectList(void);

private:
	int x, y;
	bool bSelected;
	_Object_Def* ObjDef;           // Zeiger auf die entspr. Objekt Definition
	int nAnimation;                // Akt. Anim, -1 keine Anim.
	ZeldaObjectList* object_list;  // Objekt Liste, in der das Objekt gespeichert ist
	ZeldaPortal* portal;           // Zeiger auf ein Portal, wenn es über eins verfügt
	bool bBreakIt;                 // true, dann geht das Objekt zu Bruch
	ZeldaModule* pModule;          // Zeiger auf unser Modul
	bool bHasModule;               // true, wenn es ein Modul besitzt
    _UnionModule module;           // z.B. ZeldaSoldierModule, ZeldaChickenModule...
};

inline ZeldaPortal* ZeldaObject::GetPortal(void)
{
	return portal;
}

/*
 * GetSurface
 *
 * Liefert einen Zeiger auf eine DirectSurface Oberfläche zurück. Bei Fehler ist der Rückgabewert
 * NULL.
 *
 */

inline CDXSurface* ZeldaObject::GetSurface(void)
{
	return ObjDef->pSurface;
}

/*
 * GetFileName
 *
 * Liefert den Dateinamen des Objektes zurück
 *
 */

inline char* ZeldaObject::GetFileName(void)
{
	return ObjDef->file_name;
}

/*
 * GetObjectType
 *
 * Liefert den Objekt-Typ zurück.
 *
 */

inline OBJECT_TYPE ZeldaObject::GetObjectType(void)
{
	return (ObjDef != NULL) ? ObjDef->object_type : OBJECT_INVALID;
}

/*
 * GetXKoord
 *
 * Liefert die x-Koordinate zurück.
 *
 */

inline int ZeldaObject::GetXKoord(void)
{
	return x;
}

/*
 * GetYKoord
 *
 * Liefert die y-Koordinate zurück.
 *
 */

inline int ZeldaObject::GetYKoord(void)
{
	return y;
}

/*
 * GetWidth
 *
 * Liefert die Breite des Objektes in Pixeln zurück.
 *
 */

inline int ZeldaObject::GetWidth(void)
{
	return (GetSurface() != NULL) ? GetSurface()->GetWidth() : 0;
}

/*
 * GetHeight
 *
 * Liefert die Höhe des Objektes in Pixeln zurück.
 *
 */

inline int ZeldaObject::GetHeight(void)
{
	return (GetSurface() != NULL) ? GetSurface()->GetHeight() : 0;
}

/*
 * GetObjectList
 *
 * Liefert einen Zeiger auf die Objekt Liste zurück, in der das Objekt enthalten ist.
 *
 */

inline ZeldaObjectList* ZeldaObject::GetObjectList(void)
{
	return object_list;
}

/*
 * IsSelected
 *
 * Liefert true zurück, wenn das Objekt selektiert ist, ansonsten false.
 *
 */

inline bool ZeldaObject::IsSelected(void)
{
	return bSelected;
}

/*
 * GetObjectName
 *
 * Liefert den Namen (ID) des Objektes zurück.
 *
 */

inline char* ZeldaObject::GetObjectName(void)
{
	return (ObjDef != NULL) ? ObjDef->name : NULL;
}

inline _Object_Def* ZeldaObject::GetObjectDef(void)
{
	return ObjDef;
}

/*
 * Select
 *
 * Selektiert das Objekt, falls das Programm sich im Editor Modus befindet.
 *
 */

inline void ZeldaObject::Select(bool sel)
{
	bSelected = sel;
}

inline bool ZeldaObject::IsBreaking(void)
{
	return bBreakIt;
}

inline ZeldaModule* ZeldaObject::GetModule(void)
{
	return pModule;
}

inline MODULE ZeldaObject::GetModuleType(void)
{
	return ObjDef->module_type;
}

inline bool ZeldaObject::IsChicken(void)
{
	return (ObjDef->module_type == MODULE_CHICKEN);
}

inline bool ZeldaObject::IsSoldier(void)
{
	return (ObjDef->module_type == MODULE_SOLDIER);
}

inline bool ZeldaObject::IsHighGras(void)
{
	return (ObjDef->module_type == MODULE_HIGHGRAS);
}

inline bool ZeldaObject::Selectable(void)
{
	if (ObjDef->nSelectable == 1)
		return true;

	return false;
}

#endif