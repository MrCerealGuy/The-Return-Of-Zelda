/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: anim.cpp

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#include "..\inc\zelda.h"

ZeldaAnimation::ZeldaAnimation()
{
	nAnimFrames   = 0;
	nCurrentFrame = -1;
	nFrameDelay   = 0;
}

ZeldaAnimation::~ZeldaAnimation()
{

}

bool ZeldaAnimation::AddFrame(CDXSurface* pFrame)
{
	if (pFrame == NULL || GetFramesCount() == MAX_ANIM_FRAMES) 
		return false;

	pAnimSurf[nAnimFrames] = pFrame;
	nAnimFrames++;

	DWORD dwTickCount     = 0;
	DWORD dwLastTickCount = 0;

	return true;
}

CDXSurface* ZeldaAnimation::GetNextFrame(void)
{
	dwTickCount = GetTickCount();

	if ((dwTickCount - dwLastTickCount) <= (DWORD)nFrameDelay)
		return GetCurrentFrame();

	dwLastTickCount = dwTickCount;
	
	nCurrentFrame++;

	if (nCurrentFrame == nAnimFrames)
		nCurrentFrame = -1;

	return GetCurrentFrame();
}

CDXSurface* ZeldaAnimation::GetPrevFrame(void)
{
	dwTickCount = GetTickCount();

	if ((dwTickCount - dwLastTickCount) <= (DWORD)nFrameDelay)
		return GetCurrentFrame();

	dwLastTickCount = dwTickCount;

	nCurrentFrame--;

	if (nCurrentFrame == -2)
		nCurrentFrame = nAnimFrames-1;

	return GetCurrentFrame();
}

CDXSurface* ZeldaAnimation::GetFrameByIndex(const int& nIndex)
{
	if (nIndex >= 0 && nIndex < nAnimFrames)
		return pAnimSurf[nIndex];
	else
		return NULL;
}

CDXSurface* ZeldaAnimation::GetCurrentFrame(void)
{
	if (nCurrentFrame == -1)
		return NULL;
	else
		return GetFrameByIndex(nCurrentFrame);
}
