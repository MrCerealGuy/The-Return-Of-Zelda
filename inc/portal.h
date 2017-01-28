/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: portal.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __PORTAL_H__
#define __PORTAL_H__

#include <windows.h>

extern ZeldaPortal* pPortal;
extern ZeldaObject* pTransObj;
extern bool         bChangeWorld;

/*
 * class ZeldaPortal
 *
 * ZeldaPortal repräsentiert ein Tor zu einer anderen Zelda Welt zB. beim Betreten einer
 * Höhle oder eines Gebäudes.
 *
 */

class ZeldaPortal
{
public:
	ZeldaPortal();
	~ZeldaPortal();

	inline int GetXKoord(void);
	inline int GetYKoord(void);
	void SetSize(int width, int height);
	void SetLocation(int x, int y);
	RECT GetRect(void);
	void SetWorld(const char* szToWorld);
	inline int GetWidth(void);
	inline int GetHeight(void);
	inline char* GetWorld(void);
	inline void SetLinkInitPosition(int x, int y);
	inline int GetLinkInitXPos(void);
	inline int GetLinkInitYPos(void);

private:
	int x, y;
	int link_to_x, link_to_y;
	int width, height;
	char szToWorld[256];
};

inline int ZeldaPortal::GetLinkInitXPos(void)
{
	return link_to_x;
}
	
inline int ZeldaPortal::GetLinkInitYPos(void)
{
	return link_to_y;
}

inline void ZeldaPortal::SetLinkInitPosition(int x, int y)
{
	link_to_x = x;
	link_to_y = y;
}

inline char* ZeldaPortal::GetWorld(void)
{
	return szToWorld;
}

inline int ZeldaPortal::GetWidth(void)
{
	return width;
}

inline int ZeldaPortal::GetHeight(void)
{
	return height;
}

inline int ZeldaPortal::GetXKoord(void)
{
	return x;
}

inline int ZeldaPortal::GetYKoord(void)
{
	return y;
}

#endif