/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: defs.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __DEFS_H__
#define __DEFS_H__

#define MAX_CLIENTS 100							// Maximal MAX_CLIENTS Netzwerkspieler
#define FD_SETSIZE  MAX_CLIENTS

#define CDXINCLUDEALL
#include "cdx\cdx.h"
#include "cdx\cdxdebug.h"
#include <winsock.h>

#define MAX_WORLDS   100						// Gibt die max. Anzahl an Welten an
#define MAX_OBJECTS  5000						// Gibt die max. Anzahl an Objekten einer Welt an
#define MAX_ELEMENTS MAX_OBJECTS				// Gibt die max. Anzahl an Elementen einer Liste an
#define MAX_ANIM     20							// Gibt die max. Anzahl an Animat. eines Objektes an
#define SCREEN_X     1024						// Bildschirmbreite in Pixeln
#define SCREEN_Y     768						// Bildschirmhöhe in Pixeln

#define PIXEL_PRO_LOG_ENTRY 20                  // Gibt die Anzahl der Pixeln pro Log Eintrag an

#define MAX_LOGS_PRO_PAGE (SCREEN_Y / PIXEL_PRO_LOG_ENTRY - 1)   // Gibt die max. Anzahl von Log Einträgen pro Bildschirmseite an

#define MAX_LOG_STRING 100						// Gibt die max. Länge eines Log Einträgen an
#define MAX_LOG_COUNT 10000						// Gibt die max. Anzahl von Log Einträgen an


// --- Prae-Deklaration -----------------------------------------------

class ZeldaWorld;
class ZeldaObject;
class ZeldaAnimation;
class ZeldaPortal;
class ZeldaLink;
class ZeldaNetLink;

// OBJECT_TYPE

typedef enum 
{
	OBJECT_INVALID=-1, OBJECT_LIVING, 
	OBJECT_BUILDING, OBJECT_NATURE, 
	OBJECT_NORMAL, OBJECT_PLAYER

} OBJECT_TYPE;


// DRAWING_TYPE

typedef enum
{
	LEVEL_INVALID=-1, LEVEL_ONE, 
	LEVEL_TWO, LEVEL_THREE

} DRAWING_TYPE;


// LINK_DIR

typedef enum                             // Gibt an, in welche Richtung Link zeigt
{
	LINK_LEFT, LINK_RIGHT,
	LINK_UP,   LINK_DOWN

} LINK_DIR;


// MODULE

typedef enum
{
	MODULE_INVALID=-1,
	MODULE_SOLDIER,
	MODULE_CHICKEN,
	MODULE_HIGHGRAS

} MODULE;


// DIR

typedef enum
{
	DIR_LEFT, DIR_RIGHT,
	DIR_UP,   DIR_DOWN

} DIR;


// MOVE

typedef enum
{
	MOVE_IDLE,
	MOVE_LEFT, MOVE_RIGHT,
	MOVE_UP,   MOVE_DOWN

} MOVE;


// ZELDA_MENU

typedef enum
{
	MENU_NOMENU=-1,
	MENU_SPLASH

} ZELDA_MENU;


// _Object_Def

struct _Object_Def
{
	char name[256];                      // Name des Objekts = Name der Hauptgrafik
	char file_name[256];                 // Pfad + Dateiname
	CDXSurface*  pSurface;               // Hauptgrafik
	CDXSurface*  pSolidMask;             // Solid Mask Bild, wenn nSolidMask = 1
	CDXSurface*  pLayer;                 // Layer Grafik (zB Krone beim Baum)
	int nLayer;                          // 1 = besitzt Layer, 0 = bes. kein Layer
	int nLayerCoords[4];                 // Layer Koordinaten der Hauptgrafik
	int nUseLayerGfx;                    // 1 = eigene Layer Grafik verwenden, 0 = Teilgrafik aus der Hauptgrafik
	ZeldaAnimation* pAnims[MAX_ANIM];    // Animationen
	int nDefaultAnim;                    // Standard Animation, -1 keine Std. Anim.
	int nAnimations;                     // Anzahl der Animationen
	OBJECT_TYPE object_type;             // Objekt Typ
	DRAWING_TYPE drawing_type;           // Objekt Ebene
	int nLiftable;                       // 0 = nicht nehmbar, 1 = nehmbar
	int nBreakable;                      // 0 = "zerbricht" nicht fallenlassen, 1 = "zerbricht"...
	int nSelectable;                     // 1 = kann im Editor selektiert werden
	char break_sound[256];               // Dateiname des Break Geräusches
	CDXSoundBuffer* pSoundBreak;         // Break Geräusch
	int nBreakAnim;                      // Nummer der Anim., -1 = keine Anim.
	int nPortal;                         // 0 = kein Portal, 1 = Portal
	int x_offset_port;                   // x-Koord. des Portals rel. zum Objekt
	int y_offset_port;                   // y-Koord. des Portals rel. zum Objekt
	int port_width;                      // Breite des Portals
	int port_height;                     // Höhe des Portals
	int nScript;                         // 0 = kein Script, 1 = besitzt Script
	int nModule;                         // 1 = Objekt wird durch ein Modul erweitert
	MODULE module_type;                  // wenn nModule = 1, e.g. MODULE_CHICKEN
	int nSolidMask;                      // Solid Mask, -1 = Default Bild, 0 = keine Sol.M.
	                                     // 1 = Solid Mask, "sm_ + name"
};

struct _LinkRessources
{
	CDXSurface* pSurfRight[2], *pSurfLeft[2];
	CDXSurface* pSurfUp[2],    *pSurfDown[2];

	CDXSurface* pSurfRightFight, *pSurfLeftFight;
	CDXSurface* pSurfUpFight,    *pSurfDownFight;

	CDXSurface* pSurfLeftSword[4];
	CDXSurface* pSurfRightSword[4];
	CDXSurface* pSurfUpSword[4];
	CDXSurface* pSurfDownSword[4];

	CDXSoundBuffer *pSoundLift,  *pSoundThrow;
	CDXSoundBuffer *pSoundSword, *pSoundHurt;
	CDXSoundBuffer *pSoundHealthWarning;
	CDXSoundBuffer *pSoundStepGras;

	CDXSurface* pSurfStandingGras[2];
};

#endif