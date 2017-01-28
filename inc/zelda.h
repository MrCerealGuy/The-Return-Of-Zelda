/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: zelda.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __ZELDA_H__
#define __ZELDA_H__

#pragma warning(disable : 4996)

//#define ZELDA_RELEASE			// Das Spiel wird ohne Zusatzfunktionen kompiliert und benutzt die Archivdatei

#ifndef ZELDA_RELEASE
	//#define ZELDA_WINDOW		// Spiel wird im Windows Modus gestartet
	#define ZELDA_LOG			// Es wird geloggt, aufrufbar über F5

#ifdef ZELDA_LOG
	#define ZELDA_LOG_TO_FILE	// Es wird in eine Log Datei geschrieben (nur in Kombination mit ZELDA_LOG)
#endif

	#define ZELDA_SHOW_FPS		// Die Framerate wird oben links angezeigt
#endif

#define ZELDA_ARCHIVE      "data.pak"	// Unser Zelda Archiv mit Zelda Resourcen (nicht komprimiert)
#define ZELDA_ARCHIVE_ZIP  "data.zip"	// Enthält unser Zelda Archiv in komprimierter Form
#define ZELDA_ARCHIVE_FILE "data.tmp"	// Aktuelle entpackte Zelda Resource

#include <stdlib.h>
#include "game.h"
#include "gfuncs.h"
#include "world.h"
#include "portal.h"
#include "object.h"
#include "anim.h"
#include "link.h"
#include "updenv.h"
#include "input.h"
#include "lsttempl.h"
#include "respack.h"
#include "editor.h"
#include "defs.h"
#include "objlst.h"
#include "unrar.h"
#include "module.h"
#include "script.h"

extern ZeldaGame* Game;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern BOOL bActive;

extern struct _Object_Def* Object_Def;
extern struct _LinkRessources LinkRessources;
extern char* ObjDefNames[];
extern char* WorldDefNames[];

#endif