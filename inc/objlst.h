/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: objlst.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __OBJLST_H__
#define __OBJLST_H__

#include "zelda.h"
#include "lsttempl.h"

/*
 * class ZeldaObjectList
 *
 * ZeldaObjectList ist ein Datenmodul welche Objekte organisiert und verwaltet. Man kann Objekte
 * hinzufügen, löschen oder suchen. ZeldaObjectList muss ein Zeiger von ZeldaWorld übergeben
 * werden. Jede Welt verfügt über so ein Datenmodul.
 *
 */

class ZeldaObjectList : public ZeldaList<ZeldaObject>
{
public:
	ZeldaObjectList(ZeldaWorld* world);

	ZeldaWorld* GetWorld(void);
	static bool SortObjects(ZeldaObjectList* obj_list);
	
private:
	ZeldaWorld* zelda_world;
};

#endif