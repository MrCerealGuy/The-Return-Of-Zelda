/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: game.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/


#ifndef __GAME_H__
#define __GAME_H__

#include "zelda.h"
#include "world.h"
#include "lsttempl.h"
#include "fmod.h"
#include "status.h"

class ZeldaGame
{
public:

	ZeldaGame();
	~ZeldaGame();

	bool InitEngine(void);
	bool InitGame(void);

	inline ZeldaWorld* GetActiveWorld(void);
	ZeldaWorld* GetWorldByName(const char* szName);
	bool SetActiveWorld(int nWorldId);
	bool SetActiveWorld(ZeldaWorld* pWorld);

	inline ZELDA_MENU GetCurrentMenu(void);
	inline void SetCurrentMenu(ZELDA_MENU menu);
	inline CDXSurface* GetSplashScreen(void);

	void InitPreloader(int progress);

private:

	void InitInput(void);
	bool InitSound(void);
	bool InitGfx(void);
	
	void DeInit(void);

public:
	ZeldaList<ZeldaWorld>*   WorldList;	// Unsere Weltliste, in der alle Zeiger der Welten gespeichert sind

	ZeldaLink*     Link;				// Unser Spieler Link
	ZeldaStatus*   Status;				// Zeigt Statusinformationen unseres Spielers an
	CDXScreen*     Screen;				// Ist für die Grafikausgabe zuständig
	CDXInput*      Input;				// Ist für Gamepad Steuerung zuständig
	CDXSound*      Sound;				// Ist für Soundwiedergabe zuständig
	FMUSIC_MODULE* BackgroundMusic;		// Ist für die Hintergrundmusik zuständig

	bool bGfxInit;              // true, wenn die Grafik erfolgreich initialisiert wurde
	bool bSoundInit;            // true, wenn die Sound Ausgabe erfolgreich initialisiert wurde
	bool bGameInit;             // true, wenn Zelda erfolgreich initialisiert wurde
	bool bObjDefsLoaded;        // true, wenn alle Objektdefinitionen erfolgreich geladen wurden
	bool bEngineInit;           // true, wenn Engine erfolgreich initialisiert wurde
	int  nObjDefsLoaded;		// Anzahl geladener Objekt-Definitionen 
	bool bEditor;				// Built-in Editor, wenn bEditor = true
	bool bMiniMap;				// Übersichtskarte von der aktuellen Welt
	bool bHasJoystick; 
	bool bLogWindow;			// true = Log Fenster aktiviert

	ZELDA_MENU currMenu;        // Gibt das aktuelle Menü an, definiert in defs.h

private:

	CDXSurface* pSplashScreen;
	int ActiveWorld;			// Gibt uns den Index der aktuellen Welt an
};

inline ZeldaWorld* ZeldaGame::GetActiveWorld(void)
{
	if (ActiveWorld == -1)
		return NULL;
	else
		return WorldList->GetElementByIndex(ActiveWorld);
}

inline ZELDA_MENU ZeldaGame::GetCurrentMenu(void)
{
	return currMenu;
}

inline void ZeldaGame::SetCurrentMenu(ZELDA_MENU menu)
{
	currMenu = menu;
}

inline CDXSurface* ZeldaGame::GetSplashScreen(void)
{
	return pSplashScreen;
}

#endif