/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: anim.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __ANIM_H__
#define __ANIM_H__

#define CDXINCLUDEALL
#include "cdx\cdx.h"

#define MAX_ANIM_FRAMES 20

/*
 * class ZeldaAnimation
 *
 * 
 * 
 *
 */

class ZeldaAnimation
{
public:
	ZeldaAnimation();
	~ZeldaAnimation();

	CDXSurface* GetNextFrame(void);
	CDXSurface* GetPrevFrame(void);
	CDXSurface* GetFrameByIndex(const int& nIndex);
	inline int GetCurrentFrameIndex(void);
	CDXSurface* GetCurrentFrame(void);
	inline int GetFramesCount(void);
	bool AddFrame(CDXSurface* pFrame);
	inline void SetFrameDelay(const int& delay);
	inline CDXSurface* GetLastFrame(void);

private:
	CDXSurface* pAnimSurf[MAX_ANIM_FRAMES];
	int nAnimFrames;
	int nCurrentFrame;
	int nFrameDelay;
	DWORD dwTickCount;
	DWORD dwLastTickCount;
};

inline void ZeldaAnimation::SetFrameDelay(const int& delay)
{
	nFrameDelay = delay;
}

inline CDXSurface* ZeldaAnimation::GetLastFrame(void)
{
	return GetFrameByIndex(nAnimFrames-1);
}

inline int ZeldaAnimation::GetCurrentFrameIndex(void)
{
	return nCurrentFrame;
}

inline int ZeldaAnimation::GetFramesCount(void)
{
	return nAnimFrames;
}

#endif