/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: zelda.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

/* $Id: zelda.cpp,v 1.0 2003/06/07 19:17:00 mafuba Exp $ */

// --- Includes ------------------------------------------------------

#define WIN32_LEAN_AND_MEAN                // Required for windows programs
#include <windows.h>                       // Required for windows programs
#define CDXINCLUDEALL                      // This define includes all headers required by CDX
#include "..\inc\cdx\cdx.h"                // Header for CDX RunLib

#include "..\inc\zelda.h"                  // Hauptinclude-Datei von Zelda

// --- Glob. Variablen -----------------------------------------------

char szAppName[]   = "ZELDA";   
char szClassName[] = "ZELDA";
HINSTANCE g_hInst;                     
HWND g_hWnd; 
BOOL bActive;
						
ZeldaGame* Game   = NULL;

struct _Object_Def* Object_Def = NULL;
struct _LinkRessources LinkRessources;  // Ressourcen für ZeldaLink

/////////////////////////////
// Objekte, die zu laden sind
/////////////////////////////
char* ObjDefNames[] = 
{
	"l-wand1", "l-wand2", "holzbank", "baum1", "gras1", "gras2", "haus1", "blume1", 
	"blume2", "busch1", "info", "plateau1", "stumpf1", "stumpf2", "treppe1", "zaun1", 
	"plateau2", "plateau3", "plateau4", "teleport1", "teppich1", "fass1", "holztisch1", 
	"holzsitz1", "holzboden1", "krug1", "chicken", "erde", "soldier", "torbogen", 
	"busch_stumpf", "gras3", "pflock", "pilze", "wassergrippe", "gras4", "stone", 
	"statue1", "gras7", "ENDLIST"
};

////////////////////////////
// Welten, die zu laden sind
////////////////////////////
char* WorldDefNames[] =
{
	"main", "main2", "ENDLIST"
};

//--- WinProc --------------------------------------------------------
//
// Function: Handles application messages
//
//--------------------------------------------------------------------

static long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	static int x_koord_neu = 0, y_koord_neu = 0;
	static int x_koord_alt = 0, y_koord_alt = 0;

	wKeyStatus = wParam;
	
	switch(message)
	{
		case WM_COMMAND:			
		break;

		case WM_KEYDOWN:          
		{
			switch(wParam) 
			{          
				case VK_ESCAPE: InputPrgExit(); break;

				case VK_F1: if (!Game->bLogWindow) InputEditor(); break;

				case VK_F2: InputWorldSave(); break;

				case VK_F3: InputDrawingType(); break;

				case VK_F4: if (!Game->bLogWindow) InputMiniMap(); break;

				case VK_F5: InputLogWindow(); break;

				case VK_DELETE: InputDeleteObject(); break;

				case VK_HOME:
				{
					if (!Game->bLogWindow) 
						InputSwitchObject(); 
					else
						InputLogHome(); 
					
					break;
				}

				case VK_END: InputLogEnd(); break;

				case VK_PRIOR: 
				{
					if (!Game->bLogWindow) 
						InputIncObjectLevel(); 
					else
						InputLogPageUp(); 
					
					break;
				}

				case VK_NEXT: InputLogPageDown(); break;

				case VK_LEFT: InputLeft(); break;

				case VK_RIGHT: InputRight(); break;

				case VK_UP: 
				{
					if (!Game->bLogWindow) 
						InputUp(); 
					else
						InputLogUp(); 
					
					break;
				}

				case VK_DOWN:
				{
					if (!Game->bLogWindow) 
						InputDown(); 
					else
						InputLogDown(); 
					
					break;
				}

				case VK_SHIFT: InputLiftThrowObject(); break;

				case VK_CONTROL: InputSwingSword(); break;				
			}
		}
		return 0;

		case WM_ACTIVATEAPP:
//			bActive = wParam;
		break;

		case WM_CLOSE: 
		case WM_DESTROY:
			InputPrgExit();
		break;

		case WM_MOUSEMOVE: 
			InputMouseMove(wParam,lParam,&x_koord_neu,
				&y_koord_neu,&x_koord_alt,&y_koord_alt); 
		break;

		case WM_LBUTTONDOWN: InputMouseLButtonDown(wParam,lParam); break;

		case WM_LBUTTONUP: InputMouseLButtonUp(); break;

		case WM_RBUTTONDOWN: InputMouseRButtonDown(); break;

		case WM_SIZE:
			if (wParam == SIZE_MAXIMIZED)						
				bActive = true;
			
			if (wParam == SIZE_MINIMIZED)	
				bActive = false;	
		break;
				
		default: 
			return DefWindowProc(hWnd, message, wParam, lParam); 	
    }

	return 0;
}

//--- InitApp --------------------------------------------------------
//
// Function: Creates the window
// 
//--------------------------------------------------------------------

static BOOL InitApp(int nCmdShow) 
{ 
	WNDCLASS WndClass;                                 
	WndClass.style = CS_HREDRAW | CS_VREDRAW;          
		                                               
	WndClass.lpfnWndProc = WinProc;                    
	WndClass.cbClsExtra = 0; 
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = g_hInst;                      
	WndClass.hIcon = NULL;						       
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);       
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = 0;                         
	WndClass.lpszClassName = szClassName;              
	
	RegisterClass(&WndClass);
	
#ifdef ZELDA_WINDOW
	g_hWnd = CreateWindow(
		szClassName, 
		szAppName, 
		WS_TILEDWINDOW & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX,
		0,
		0,
		SCREEN_X/2,
		SCREEN_Y/2,
		NULL, 
		NULL, 
		g_hInst,
		NULL);

#else
	g_hWnd = CreateWindowEx(
		WS_EX_TOPMOST,
		szClassName, 
		szAppName, 
		WS_POPUP,  
		0, 
		0, 
		GetSystemMetrics(SM_CXFULLSCREEN),  
		GetSystemMetrics(SM_CYFULLSCREEN),  
		NULL, 
		NULL, 
		g_hInst,
		NULL);
#endif
	
	if (!g_hWnd) return FALSE;
	
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);  
	
	return TRUE;
}

extern int InitLoader;

//--- WinMain --------------------------------------------------------
//
// Function: Inital function called by windows
//
//--------------------------------------------------------------------

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) 
{	 
	g_hInst = hInstance;
	MSG msg;

	DEBUG("THE RETURN OF ZELDA -- LOG DATEI\n");
	DEBUG("%s   %s\n\n", __DATE__, __TIME__);

#ifdef ZELDA_RELEASE

	if (!unpack_data_archive())
		return -1;

#endif 

	// Window Fenster erstellen
	if (!InitApp(nCmdShow)) 
		return -1;
	
	// Game Objekt erstellen
	if ((Game = new ZeldaGame()) == NULL)
		return -1;

#ifdef ZELDA_WINDOW
	SetWindowText(g_hWnd, "The Return Of Zelda);
#endif

	// Zelda Engine
	if (!Game->InitEngine())
	{
		delete Game;
		return -1;
	}

	++InitLoader;
	Game->InitPreloader(InitLoader);

	// Zelda Game initialisieren
	if (!Game->InitGame())
	{
		delete Game;
		return -1;
	}

	Game->SetCurrentMenu(MENU_NOMENU);

	while (1) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
		{
			if (!GetMessage(&msg, NULL, 0, 0 )) 
				return msg.wParam;
			
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		} 
		else 
		{
			UpdateFrame();     
		}
	}

	return 0;
}