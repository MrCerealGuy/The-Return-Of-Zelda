/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: updenv.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __UPDENV_H__
#define __UPDENV_H__

extern bool bObjectDeleted;

void UpdateInput(void);
void UpdateArea(DRAWING_TYPE area);
void UpdateFrame(void);
void UpdateLayers(void);
void UpdateLivings(void);

#endif