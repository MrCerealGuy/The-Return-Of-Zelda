/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: updenv.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"
#include "..\inc\portal.h"

extern char LogText[MAX_LOG_COUNT][100];
extern int nLogCount;
extern int log_scroll_offset;
extern int log_start_i;
extern int log_end_i;

// Wird in UpdateArea und ZeldaObject::Update benötigt
bool bObjectDeleted = false;

void UpdateInput(void)
{
	static DWORD dwLiftCount  = 0;
	static DWORD dwFightCount = 0;
	static DWORD dwMoveCount  = 0;

	LONG JoyX , JoyY;

	if (Game->Input != NULL && Game->bHasJoystick && !Game->bEditor && !Game->bMiniMap) 
	{
		Game->Input->Update();  // Update GamePad

		// Aufheben, Werfen
		if(Game->Input->GetKeyState(CDXKEY_JOYBUTN0))
		{
			if (Game->Link != NULL && Game->GetActiveWorld() != NULL)
			{	
				if (dwLiftCount == 10)
				{
					if (Game->Link->GetLiftObject() == NULL)
						Game->Link->LiftObject();
					else
						Game->Link->ThrowObject();

					dwLiftCount = 0;
				}
				else dwLiftCount++;
			}
		}

		// Kämpfen
		if(Game->Input->GetKeyState(CDXKEY_JOYBUTN1))
		{
			if (Game->Link != NULL && Game->GetActiveWorld() != NULL)
			{	
				if (dwFightCount == 10)
				{
					Game->Link->SwingSword();

					dwFightCount = 0;
				}
				else dwFightCount++;
			}
		}

		Game->Input->GetJoystickDeltas(&JoyX, &JoyY);

		if (Game->GetActiveWorld() != NULL && Game->Link != NULL && dwMoveCount == 1)
		{	
			bool bX = false;
			if (JoyX > 0) { Game->Link->MoveRight(); bX = true; }
			if (JoyX < 0) { Game->Link->MoveLeft(); bX = true; }			
			if (JoyY > 0 && !bX) Game->Link->MoveDown();
			if (JoyY < 0 && !bX) Game->Link->MoveUp();
			dwMoveCount = 0;
		}
		else dwMoveCount++;
	}
}

void UpdateArea(DRAWING_TYPE area)
{
	ZeldaObjectList* pList = Game->GetActiveWorld()->GetObjectList();
	
	ZeldaObject* obj = pList->GetElementByIndex(0);

	for (int i = 1; obj != NULL; i++) 
	{
		if (obj->GetDrawingType() == area && obj->GetObjectType() != OBJECT_LIVING)
		{
			if (Game->bMiniMap || ObjectInWorldViewRect(obj))
			{
				obj->Update();

				// Wurde gerade ein Objekt zerstört?
				if (bObjectDeleted)
				{
					i--;  // Dann ID zurücksetzen, damit das Nachfolgeobjekt, das
				          // in der Liste um eine Stelle zurückgefallen ist, 
					      // auch gezeichnet wird ansonsten würde es übersprungen
					      // werden und würde für einen kurzen Augenblick blinken.
					bObjectDeleted = false;
				}
			}
		}

		obj = pList->GetElementByIndex(i);
	}
}

void UpdateLivings(void)
{
	ZeldaWorld*  pWorld = NULL;
	ZeldaObject* obj    = NULL;

	for (int j = 0; j != Game->WorldList->GetElementCount(); j++)
	{
		pWorld = Game->WorldList->GetElementByIndex(j);

		if (pWorld == NULL) continue;

		obj = pWorld->GetObjectList()->GetElementByIndex(0);

		for (int i = 1; obj != NULL; i++) 
		{
			if (obj->GetObjectType() == OBJECT_LIVING)
				obj->Update();

			obj = pWorld->GetObjectList()->GetElementByIndex(i);
		}
	}
}

//--- UpdateFrame ----------------------------------------------------
//
// Function: Performs drawing of the current frame 
//
//--------------------------------------------------------------------

void UpdateFrame(void) 
{ 					
	if (!bActive)
		return;

#ifdef ZELDA_LOG

	// Log Fenster zeichnen
	if (Game->bLogWindow)
	{
		Game->Screen->GetBack()->Fill(0);
		Game->Screen->GetBack()->GetDC();

		int i, j;

		// Ist automatisches Scrollen aktiviert? (über F6)
		//if (Game->bLogWindowAutoScroll)
		//	InputLogEnd();
		
		for (j = 0, i = log_start_i; i != log_end_i; i++, j++)
			Game->Screen->GetBack()->TextXY(0, j * PIXEL_PRO_LOG_ENTRY, 0x0000FFF0, LogText[i]);

		log_scroll_offset = 0;
		
		Game->Screen->GetBack()->ReleaseDC();
		Game->Screen->Flip();

		return;
	}

#endif

	UpdateInput();

	if (Game->bMiniMap)
		Game->GetActiveWorld()->pMiniMap->Fill(0);
	else
		Game->Screen->GetBack()->Fill(0);
	
	UpdateArea(LEVEL_ONE);                    /* ERSTE EBENE */

	UpdateArea(LEVEL_TWO);                    /* ZWEITE EBENE */

	UpdateLivings();                          /* LEBEWESEN */

	if (Game->Link != NULL) 
	{
		Game->Link->Update();                 /* LINK */
		Game->Link->UpdateSword();
	}

	UpdateArea(LEVEL_THREE);                  /* DRITTE EBENE */

	UpdateLayers();  						  /* LAYER */

	Game->Status->DrawLinkStatus();

#ifndef ZELDA_RELEASE

	if (Game->bEditor)
	{
		if (SelectedObject != NULL)
		{
			if (BoundaryBox != NULL)
				BoundaryBox->Update();

			DrawPortalBox();
		}

		PrintEditorStatus();
		
		if (EditorObjWnd != NULL) EditorObjWnd->Update();
	}

#endif

	if (Game->bMiniMap)
	{
		// MiniMap generieren		                
		if (Game->GetActiveWorld()->pMiniMap != NULL)
		{
			Game->Screen->GetBack()->Fill(0);			
			Game->GetActiveWorld()->pMiniMap->DrawBlkStretched(Game->Screen->GetBack(),0,0,NULL,SCREEN_X,SCREEN_Y);
		}
	}

#ifdef ZELDA_SHOW_FPS
	Game->Screen->Flip(TRUE, TRUE, TRUE);
#else
	Game->Screen->Flip();
#endif

	// PORTAL AKTIVIERT ???
	if (bChangeWorld == true && pPortal != NULL)
	{
		ZeldaWorld* pWorld = NULL;

		pWorld = Game->GetWorldByName(pPortal->GetWorld());

		if (pWorld != NULL)
		{
			Game->SetActiveWorld(pWorld);

		    if (Game->Link != NULL) Game->Link->SetLocation(pPortal->GetLinkInitXPos(), pPortal->GetLinkInitYPos());
		}
		
		bChangeWorld   = false;
		pPortal        = NULL;
		pTransObj      = NULL;
		SelectedObject = NULL;
	}
}

void UpdateLayers(void)
{
	ZeldaObject* obj = Game->GetActiveWorld()->GetObjectList()->GetElementByIndex(0);

	for (int i = 1; obj != NULL; i++) 
	{
		if (obj->GetObjectDef()->nLayer == 1)
		{
			if (Game->bMiniMap || ObjectInWorldViewRect(obj))
				obj->UpdateLayer();
		}

		obj = Game->GetActiveWorld()->GetObjectList()->GetElementByIndex(i);
	}
}