/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: gfuncs.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/


#ifndef __GFUNCS_H__
#define __GFUNCS_H__

#include "objlst.h"
#include "defs.h"

// Makro Funktionen um absolute Koord. in rel. Koord. umzuwandeln
#define X_KOORD(x) ((x) - Game->GetActiveWorld()->GetWorldViewRect().left)
#define Y_KOORD(x) ((x) - Game->GetActiveWorld()->GetWorldViewRect().top)

const char* DEBUG(const char* szText, ...);
bool LoadObjectDefs(void);
_Object_Def* GetObjectDef(const char* name);
bool ObjectInWorldViewRect(ZeldaObject* object);
void MoveWorldViewRectRight(int abs);
void MoveWorldViewRectLeft(int abs);
void MoveWorldViewRectUp(int abs);
void MoveWorldViewRectDown(int abs);
int GetObjectDefCount(void);
int GetWorldDefCount(void);
CDXSurface* LoadSurface(const char* file_name);
CDXSoundBuffer* LoadSound(const char* file_name);
bool RectInRect(RECT rc1, RECT rc2);
bool PointInRect(int x, int y, RECT rc);
bool IsPlayerMoving(void);
bool IsLeftMouseButtonDown(void);
int GetMouseKoordX(void);
int GetMouseKoordY(void);
RECT TransformRectToScreen(RECT rc);
bool CheckSolid(ZeldaWorld* pWorld, int x, int y);
bool CheckPortal(ZeldaObject *pObj);

// ResPack Modul Schnittstelle
const char* respack_interface(const char* file_name);
bool unpack_data_archive(void);

#endif