/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: game.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"
#include <stdlib.h>  // _sleep()

// InitPreloader
int InitLoader=0;
int MaxInitLoader=13;
int InitProgressBar_x=SCREEN_X/4;
int InitProgressBar_y=SCREEN_Y-30;
int InitProgressBar_x2=0;

void ZeldaGame::InitPreloader(int progress)
{
	// Splash Screen zeichnen
	Screen->GetBack()->Fill(0);		

	GetSplashScreen()->DrawBlkStretched(Game->Screen->GetBack(),0,0,NULL,SCREEN_X,SCREEN_Y);

	Screen->Flip();
}		

ZeldaGame::ZeldaGame()
{
	bGfxInit             = false;
	bSoundInit           = false;
	bGameInit            = false;
	bObjDefsLoaded       = false;
	bEngineInit          = false;

	nObjDefsLoaded       = 0;      // Anzahl geladener Objekt-Definitionen 
	bEditor              = false;  // Built-in Editor, wenn bEditor = true
	bMiniMap             = false;  // Übersichtskarte von der aktuellen Welt
	bHasJoystick         = false;
	bLogWindow			 = false;

	currMenu             = MENU_SPLASH;
	pSplashScreen        = NULL;

	Link = NULL;

	ActiveWorld = -1;
}

ZeldaGame::~ZeldaGame()
{
	DeInit();
}

bool ZeldaGame::InitEngine(void)
{
	// Grafik initialisieren
	bGfxInit = InitGfx();

	// Lade Splash Screen
	if ((pSplashScreen = LoadSurface("dat\\gfx\\splash.bmp")) == NULL)
		return false;

	++InitLoader;
	InitPreloader(InitLoader);

	// Sound initialisieren
	if (bGfxInit) bSoundInit = InitSound();

	++InitLoader;
	InitPreloader(InitLoader);

	// Gamepad Steuerung initialisieren
	InitInput(); // optional

	++InitLoader;
	InitPreloader(InitLoader);

	// Objektdefinitionen laden
	if (bSoundInit) bObjDefsLoaded = LoadObjectDefs();

	++InitLoader;
	InitPreloader(InitLoader);

	// Weltliste erstellen
	if (bObjDefsLoaded)
	{
		if ((WorldList = new ZeldaList<ZeldaWorld>()) == NULL)
			DEBUG("ZeldaGame::ZeldaGame() : new ZeldaList<ZeldaWorld>()\n");
	}

	// Link erstellen
	if ((Link = new ZeldaLink()) == NULL)
		DEBUG("ZeldaGame::ZeldaGame() : new ZeldaLink\n");

	++InitLoader;
	InitPreloader(InitLoader);

	// Tools für Editor erstellen
	if ((EditorObjWnd = new ZeldaObjWnd()) == NULL)
		DEBUG("ZeldaGame::ZeldaGame() : new ZeldaObjWnd\n");

	++InitLoader;
	InitPreloader(InitLoader);
	
	if ((Status = new ZeldaStatus()) == NULL)
		DEBUG("ZeldaGame::ZeldaGame() : new ZeldaStatus\n");

	++InitLoader;
	InitPreloader(InitLoader);

	if ((BoundaryBox = new ZeldaBoundaryBox()) == NULL)
		DEBUG("ZeldaGame::ZeldaGame() : new ZeldaBoundaryBox\n");

	++InitLoader;
	InitPreloader(InitLoader);


	++InitLoader;
	InitPreloader(InitLoader);

	bEngineInit = (bGfxInit && bSoundInit && bObjDefsLoaded && WorldList != NULL &&
	               Link != NULL && EditorObjWnd != NULL && Status != NULL &&
				   BoundaryBox != NULL);

	return bEngineInit;

}

ZeldaWorld* ZeldaGame::GetWorldByName(const char* szName)
{
	ZeldaWorld* pWorld = NULL;

	for (int i = 0; i != WorldList->GetElementCount(); i++)
	{
		pWorld = WorldList->GetElementByIndex(i);

		if (pWorld != NULL)
			if (strcmp(pWorld->GetWorldName(), szName) == 0)
				return pWorld;
	}

	return NULL;
}

bool ZeldaGame::SetActiveWorld(int nWorldId)
{
	// Ist die ID gültig?
	if (WorldList->GetElementByIndex(nWorldId) != NULL)
	{
		ActiveWorld = nWorldId;
		return true;
	}

	return false;
}
	
bool ZeldaGame::SetActiveWorld(ZeldaWorld* pWorld)
{
	int nWorldId;

	if ((nWorldId = WorldList->GetIndex(pWorld)) != -1)
	{
		ActiveWorld = nWorldId;
		return true;
	}

	return false;
}

//--- InitGfx --------------------------------------------------------
//
// Function: Handles initialization of the CDX Gfx Objects
// 
//--------------------------------------------------------------------

bool ZeldaGame::InitGfx(void) 
{	
	Screen = new CDXScreen();
	
	if (Screen == NULL) 
		return false;   	
	
#ifdef ZELDA_WINDOW	
	if (FAILED(Screen->CreateWindowed(g_hWnd, SCREEN_X, SCREEN_Y)))
#else
	if (FAILED(Screen->CreateFullScreen(g_hWnd, SCREEN_X, SCREEN_Y, 32))) 
#endif
		return false;

	return true; 
}

bool ZeldaGame::InitSound(void)
{
	Sound = new CDXSound();

	if (Sound == NULL)
		return false;

	if (FAILED(Sound->Create((void*)g_hWnd, FALSE)))
		return false;

	return true;
}

void ZeldaGame::InitInput(void)
{
	Input = new CDXInput();

	bHasJoystick = true;

	if (Input == NULL)
		bHasJoystick = false;

	if (!bHasJoystick || FAILED(Input->Create(g_hInst, g_hWnd)))
		bHasJoystick = false;

	if (!bHasJoystick || FAILED(Input->SetActiveDevices(FALSE,FALSE,TRUE)))
		bHasJoystick = false;

	if (!bHasJoystick || FAILED(Input->SetJoystickExculsive(TRUE)))
		bHasJoystick = false;

	if(bHasJoystick && Input->HasJoystick())
	{
		bHasJoystick = true;
	}
	else 
	{
		delete Input;
		Input = NULL;
	}
}

//--- InitGame -------------------------------------------------------
//
// Function: Handles initialization of Zelda
// 
//--------------------------------------------------------------------

bool ZeldaGame::InitGame(void)
{
	ZeldaWorld* pWorld = NULL;
	char szFileName[256];

	// Welten laden
	for (int i = 0; strcmp((const char*)WorldDefNames[i],"ENDLIST") != 0; i++)
	{
		pWorld = NULL;

		sprintf(szFileName, "dat\\wrl\\%s.txt", WorldDefNames[i]);

		if ((pWorld = new ZeldaWorld(WorldDefNames[i])) == NULL)
		{
			DEBUG("ZeldaGame::InitGame() : new ZeldaWorld(...)\n");
			return false;
		}

		WorldList->AddElement(pWorld);

		if (!pWorld->LoadWorld(szFileName))  // kein respack_interface
		{
			DEBUG("ZeldaGame::InitGame() : pWorld->LoadWorld()");
			DeInit();
			return false;
		}

		DEBUG("\n########################################\n");
		DEBUG("Zelda Welt: %s erfolgreich geladen\n", pWorld->GetWorldName()); 
		DEBUG("########################################\n\n");

		++InitLoader;
		InitPreloader(InitLoader);
	}

	SetActiveWorld(GetWorldByName("main"));

	// Position von Link setzen
	if (!Game->Link->SetLocation(GetActiveWorld()->GetLinkInitX(), GetActiveWorld()->GetLinkInitY()))
	{
		DEBUG("ZeldaGame::InitGame: Game->Link->SetLocation(...)\n");
		return false;
	}

	// Hintergrund Musik laden
	FMUSIC_MODULE *BackgroundMusic = 0;

	if (FSOUND_Init(32000, 64, 0))
		BackgroundMusic = FMUSIC_LoadSong(respack_interface("dat\\snd\\triforce2.mid"));

	if (BackgroundMusic != NULL)
		FMUSIC_PlaySong(BackgroundMusic);

#ifndef ZELDA_WINDOW
	ShowCursor(FALSE);
#endif

	bActive = true;

	++InitLoader;
	InitPreloader(InitLoader);

	return (bGameInit = true);
}

//--- DeInit -----------------------------------------------------------
//
// Function: Handles de-initialization of the CDX objects
// 
//--------------------------------------------------------------------

void ZeldaGame::DeInit(void)
{	
	bActive = false;

	FMUSIC_FreeSong(BackgroundMusic);
    FSOUND_Close();

	// Objektdefinitionen löschen
	free(Object_Def);

	if (Input != NULL)
	{
		delete Input;
		Input = NULL;
	}

	if (WorldList != NULL)
	{
		WorldList->DestroyAllElements();
		delete WorldList; WorldList = NULL;
		ActiveWorld = -1;
	}

	if (EditorObjWnd != NULL)
	{
		delete EditorObjWnd;
		EditorObjWnd = NULL;
	}

	if (Status != NULL)
	{
		delete Status;
		Status = NULL;
	}

	if (BoundaryBox != NULL)
	{
		delete BoundaryBox;
		BoundaryBox = NULL;
	}

	if (Link != NULL)
	{
		delete Link;
		Link = NULL;
	}

	// Lösche Objekt Definitionen
	for (int i = 0; i != nObjDefsLoaded; i++)
	{
		// Layer Grafik gleich der Standard/Basis Grafik
		bool bBASE_GFX = (Object_Def[i].pSurface == Object_Def[i].pLayer) ? true : false;
		
		Screen->ReleaseSurfaceFromList(Object_Def[i].pSurface);
		Object_Def[i].pSurface = NULL;
		
		if (bBASE_GFX) 
			Object_Def[i].pLayer   = NULL;
		else
		{
			Screen->ReleaseSurfaceFromList(Object_Def[i].pLayer);
			Object_Def[i].pLayer = NULL;
		}

		for (int j = 0; j != Object_Def[i].nAnimations; j++)
		{
			for (int k = 0; k != Object_Def[i].pAnims[j]->GetFramesCount(); k++)
				Screen->ReleaseSurfaceFromList(Object_Def[i].pAnims[j]->GetFrameByIndex(k));

			delete Object_Def[i].pAnims[j];
			Object_Def[i].pAnims[j] = NULL;
		}
	}

	nObjDefsLoaded = 0;
}



