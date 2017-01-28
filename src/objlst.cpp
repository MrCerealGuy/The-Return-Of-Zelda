/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: objlst.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

ZeldaObjectList::ZeldaObjectList(ZeldaWorld* world)
{
	zelda_world = world;
}

/*
 * GetWorld
 *
 * Liefert einen Zeiger auf die Welt zurück, die mit dieser Objektliste verknüpft ist.
 *
 */

ZeldaWorld* ZeldaObjectList::GetWorld(void)
{
	return zelda_world;
}

/*
 * SortObjects
 *
 * Sortiert die Liste so, dass die Objekte mit aufsteigender Y-Koord. aufgelistet werden.
 * Dies hat den Vorteil, dass die hinteren Objekte zuerst gezeichnet werden.
 *
 */

bool ZeldaObjectList::SortObjects(ZeldaObjectList* obj_list)
{
	if (obj_list == NULL)
		return false;

	for (int i = 0; i != obj_list->GetElementCount(); i++)
	{
		// Alle Objekte mit LEVEL_ONE ignorien, wie Gras, Holzboden oder
		// die Blumen, da dies ohnehin wenig Sinn machen würde
		if (obj_list->GetElementByIndex(i)->GetDrawingType() == LEVEL_ONE)
			continue;

		for (int j = i+1; j != obj_list->GetElementCount(); j++)
			if (obj_list->GetElementByIndex(j)->GetYKoord() < obj_list->GetElementByIndex(i)->GetYKoord())
				obj_list->SwitchIndex(obj_list->GetElementByIndex(j),obj_list->GetElementByIndex(i));
	}

	return true;
}
