/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: lsttempl.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __LSTTEMPL_H__
#define __LSTTEMPL_H__

#include "defs.h"

template<class T> class ZeldaList
{
public:
	ZeldaList();
	~ZeldaList();

	int AddElement(T* elem);
	bool DestroyAllElements(void);
	bool DestroyElement(T* elem);
	bool DestroyElement(const int& index);
	T* GetElementByIndex(const int& index);
	int GetElementCount(void);
	int GetIndex(T* elem);
	bool SwitchIndex(T* elem1, T* elem2);
	T** GetList(void);
	bool SetElement(int& index, T* elem);
	bool RemovePointerFromList(int index);

protected:
	int elem_count;
	T* elem_list[MAX_ELEMENTS];
};

template<class T> ZeldaList<T>::ZeldaList()
{
	elem_count = 0;
}

template<class T> ZeldaList<T>::~ZeldaList()
{
}

/*
 * SwitchIndex
 *
 * Vertauscht die Indizes der Elemente elem1 und elem2.
 *
 */

template<class T> bool ZeldaList<T>::SwitchIndex(T* elem1, T* elem2)
{
	if (elem1 == NULL || elem2 == NULL || elem1 == elem2)
		return false;

	elem_list[GetIndex(elem1)] = elem2;
	elem_list[GetIndex(elem2)] = elem1;

	return true;
}

/*
 * AddElement
 *
 * Fügt ein Element in die Elemente Liste. Rückgabewert ist der Index des Elements, bei Fehler
 * -1.
 *
 */

template<class T> int ZeldaList<T>::AddElement(T* elem)
{
	if (elem_count >= MAX_ELEMENTS || elem == NULL)
		return -1;

	elem_list[elem_count] = elem;
	elem_count++;

	return (elem_count-1);	
}

/*
 * DestroyAllElements
 *
 * Zerstört alle Elemente, die sich in der Liste befinden und gibt so den belegten Speicher 
 * wieder frei.
 *
 */

template<class T> bool ZeldaList<T>::DestroyAllElements(void)
{
	bool ret = true;

	if (elem_count == 0)
		return false;

	for (int i = 0; i != elem_count; i++)
		if (!DestroyElement(i))
			ret = false;

	return ret;	
}

/*
 * DestroyElement
 *
 * Zerstört ein Element aus der Liste mit dem Index index.
 *
 */

template<class T> bool ZeldaList<T>::DestroyElement(const int& index)
{
	if (index >= MAX_ELEMENTS || index >= elem_count || elem_list[index] == NULL)
		return false;

	delete elem_list[index];
	return RemovePointerFromList(index);
}

/*
 * DestroyElement
 *
 * Zerstört ein Element aus der Liste mit dem Zeiger elem.
 *
 */

template<class T> bool ZeldaList<T>::DestroyElement(T* elem)
{
	int index;

	if (elem == NULL || (index = GetIndex(elem)) == -1)
		return false;

	delete elem_list[index]; 
	elem_list[index] = NULL;

	return RemovePointerFromList(index);
}

/*
 * GetIndex
 *
 * Liefert den Index des Eintrags mit dem Zeiger elem.
 *
 */

template<class T> int ZeldaList<T>::GetIndex(T* elem)
{
	for (int i = 0; i != elem_count; i++)
		if (elem_list[i] == elem)
			return i;

	return -1;
}

/*
 * GetElementByIndex
 *
 * Liefert einen Zeiger auf ein Element mit dem Index index der Liste zurück. Bei Fehler wird
 * NULL zurückgegeben.
 *
 */

template<class T> inline T* ZeldaList<T>::GetElementByIndex(const int& index)
{
	if (index >= MAX_ELEMENTS || index >= elem_count)
		return NULL;

	return elem_list[index];
}

/*
 * GetElementCount
 *
 * Liefert die Anzahl der Elemente, die in der Liste gespeichert sind, zurück.
 *
 */

template<class T> inline int ZeldaList<T>::GetElementCount(void)
{
	return elem_count;
}

/*
 * RemovePointerFromList
 *
 * Entfernt aus elem_list einen Eintrag und schiebt alle Einträge rechts vom Index index eine
 * Stelle weiter nach links. Rückgabewert ist true bei Erfolg, ansonsten false.
 *
 */

template<class T> bool ZeldaList<T>::RemovePointerFromList(int index)
{
	if (index >= MAX_ELEMENTS || index >= elem_count)
		return false;

	for (int i = index; i != elem_count-1; i++)
		elem_list[i] = elem_list[i+1];

	elem_count--;
	
	return true;	
}

template<class T> T** ZeldaList<T>::GetList(void)
{
	return elem_list;
}

template<class T> bool ZeldaList<T>::SetElement(int& index, T* elem)
{
	if (index >= MAX_ELEMENTS || index >= elem_count || elem == NULL)
		return false;

	elem_list[index] = elem;

	return true;
}

#endif
