/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: input.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/


#ifndef __INPUT_H__
#define __INPUT_H__

extern WPARAM wKeyStatus;
extern bool bLButtonDown;
extern int mouse_pos_x;
extern int mouse_pos_y;

// Key inputs
void InputLiftThrowObject(void);
void InputSwingSword(void);
void InputLeft(void);
void InputRight(void);
void InputUp(void);
void InputDown(void);
void InputIncObjectLevel(void);
void InputSwitchObject(void);
void InputDeleteObject(void);
void InputMiniMap(void);
void InputDrawingType(void);
void InputWorldSave(void);
void InputEditor(void);
void InputPrgExit(void);

void InputLogWindow(void);
void InputLogPageUp(void);
void InputLogPageDown(void);
void InputLogHome(void);
void InputLogEnd(void);
void InputLogUp(void);
void InputLogDown(void);

// Mouse inputs
void InputMouseMove(WPARAM wParam, LPARAM lParam, int *x1, int *y1, int *x2, int *y2);
void InputMouseLButtonDown(WPARAM wParam, LPARAM lParam);
void InputMouseLButtonUp(void);
void InputMouseRButtonDown(void);

#endif
