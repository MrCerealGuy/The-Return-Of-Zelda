/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: link.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/

#ifndef __LINK_H__
#define __LINK_H__

#define CDXINCLUDEALL
#include "cdx\cdx.h"

#include "defs.h"

extern ZeldaGame* Game;

/*
 * class ZeldaLink
 *
 * ZeldaLink repräsentiert die Hauptfigur in diesem Spiel dar.
 *
 */

class ZeldaLink
{
public:
	ZeldaLink();
	~ZeldaLink();

	bool MoveRight(void);
	bool MoveLeft(void);
	bool MoveUp(void);
	bool MoveDown(void);
	void Update(void);
	void UpdateSword(void);
	inline int GetXKoord(void);
	inline int GetYKoord(void);
	RECT GetSolidRect(void);
	RECT GetRect(void);
	int GetWidth(void);
	int GetHeight(void);
	inline LINK_DIR GetDirection(void);
	inline void SetDirection(const LINK_DIR& dir);
	bool CheckSolid(int x, int y);
	bool SetLocation(const int x, const int y);
	bool LiftObject(void);
	bool ThrowObject(void);
	bool SwingSword(void);
	inline ZeldaObject* GetLiftObject(void);
	inline int GetMaxLifes(void);
	inline int GetCurrentLifes(void);
//	inline void SetName(const char* szName);
//	inline const char* GetName(void);
	void Hurt(void);
	void SetCurrentLifes(int lifes);

public:

	bool bStandInHighGras;  // true, wenn Link durch hohes Gras läuft
	int nHighGrasFrame;
	int nHighGrasCount;     

	ZeldaObject* pLiftObject;  // Objekt, das vom Link getragen wird
	
private:

	int x, y, nAnim;
	LINK_DIR link_dir;
	
	bool bSwingSword;  // true, wenn Link sein Schwert schwingt
	int nCurrentLifes;
	int nMaxLifes;

//	char szName[256];
};
/*
inline void ZeldaLink::SetName(const char* szName)
{
	strcpy(this->szName, szName);
}

inline const char* ZeldaLink::GetName(void)
{
	return szName;
}
*/
inline ZeldaObject* ZeldaLink::GetLiftObject(void)
{
	return pLiftObject;
}

inline int ZeldaLink::GetXKoord(void)
{
	return x;
}

inline int ZeldaLink::GetYKoord(void)
{
	return y;
}

inline void ZeldaLink::SetDirection(const LINK_DIR& dir)
{
	link_dir = dir;
}
	
inline LINK_DIR ZeldaLink::GetDirection(void)
{
	return link_dir;
}

inline int ZeldaLink::GetMaxLifes(void)
{
	return nMaxLifes;
}

inline int ZeldaLink::GetCurrentLifes(void)
{
	return nCurrentLifes;
}

#endif