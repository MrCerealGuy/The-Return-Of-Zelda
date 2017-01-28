/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: world.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __WORLD_H__
#define __WORLD_H__

#include "objlst.h"


/*
 * class ZeldaWorld
 *
 * ZeldaWorld stellt eine Umgebung dar, in der Link interagieren kann. Eine Umgebung kann mit
 * weiteren Umgebungen verknüpft sein, zb wenn man ein Haus oder ein Dungeon o.ä. betritt. Eine
 * Umgebung kann Objekte beinhalten wie Gegner, Häuser oder einfache Gegenstände sowie Natur-
 * objekte.
 *
 */

class ZeldaWorld
{
public:
	ZeldaWorld(const char* name);
	~ZeldaWorld();

	inline const char* GetWorldName(void);
	inline const char* GetFileName(void);
	inline int GetLinkInitX(void);
	inline int GetLinkInitY(void);
	inline int GetWorldViewInitX(void);
	inline int GetWorldViewInitY(void);
	bool LoadWorld(const char* file_name);
	inline int GetWidth(void);
	inline int GetHeight(void);
	inline ZeldaObjectList* GetObjectList(void);
	bool Save(void);
	CDXSurface* GetSurface(void);
	inline RECT GetWorldViewRect(void);
	void SetWorldViewRect(int x, int y);

public:
	CDXSurface* pMiniMap;
	
//private:
	char* world_name;
	char* file_name;

	int width;			// Breite der Welt in Pixel
	int height;         // Höhe der Welt in Pixel

	int link_init_x;
	int link_init_y;

	int worldview_init_x;
	int worldview_init_y;

	ZeldaObjectList* ObjectList;

	RECT world_view_rect;
};

inline int ZeldaWorld::GetWorldViewInitX(void)
{
	return worldview_init_x;
}

inline int ZeldaWorld::GetWorldViewInitY(void)
{
	return worldview_init_y;
}

inline int ZeldaWorld::GetLinkInitX(void)
{
	return link_init_x;
}
	
inline int ZeldaWorld::GetLinkInitY(void)
{
	return link_init_y;
}

inline RECT ZeldaWorld::GetWorldViewRect(void)
{
	return world_view_rect;
}

/*
 * GetObjectList
 *
 * Liefert einen Zeiger auf eine ZeldaObjectList Objekt zurück
 *
 */

inline ZeldaObjectList* ZeldaWorld::GetObjectList(void)
{
	return ObjectList;
}

/*
 * GetWorldName
 *
 * Liefert den Namen der Welt zurück
 *
 */

inline const char* ZeldaWorld::GetWorldName(void)
{
	return world_name;
}

/*
 * GetFileName
 *
 * Liefert den Dateinamen der Welt zurück
 *
 */

inline const char* ZeldaWorld::GetFileName(void)
{
	return file_name;
}

/*
 * GetWidth
 *
 * Liefert die Breite der Welt in Pixeln zurück.
 *
 */

inline int ZeldaWorld::GetWidth(void)
{
	return width;
}

/*
 * GetHeight
 *
 * Liefert die Höhe der Welt in Pixeln zurück.
 *
 */

inline int ZeldaWorld::GetHeight(void)
{
	return height;
}

#endif