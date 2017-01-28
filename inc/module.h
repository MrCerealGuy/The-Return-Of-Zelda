/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: module.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/


#ifndef __MODULE_H__
#define __MODULE_H__

#include "zelda.h"

/*
 * class ZeldaModule
 *
 * ZeldaModule ist unsere Basisklasse für alle weitere Module. Wird ein spezielles
 * Modul definiert, so ist es von dieser Klasse abzuleiten. Mit ZeldaModule wird
 * ein vorhandenes ZeldaObject erweitert.
 *
 */

class ZeldaModule
{
public:
	ZeldaModule();
	~ZeldaModule();

	virtual void Update(void);
	inline ZeldaObject* GetObject(void);
	inline void SetObject(ZeldaObject* pObj);
	bool CheckSolid(int x, int y);

	ZeldaObject* pObj;
};

inline ZeldaObject* ZeldaModule::GetObject(void)
{
	return pObj;
}

inline void ZeldaModule::SetObject(ZeldaObject* pObj)
{
	this->pObj = pObj;
}

/*
 * class ZeldaChickenModule
 *
 * Das Modul ZeldaChickenModule erweitert das "Huhn" Objekt um ein Eigenleben. Ohne dieses
 * Modul würde das Huhn über keine Intelligenz verfügen, sondern wäre nur dann ein ganz 
 * normales Objekt.
 *
 */

class ZeldaChickenModule : public ZeldaModule
{
public:
	ZeldaChickenModule();
	~ZeldaChickenModule();

	void Update(void);
	void Gluck(void);
	void Hurt(void);
	void Move(MOVE move);
	inline MOVE GetMoveCmd(void);
	inline void SetDirection(DIR dir);
	inline DIR GetDirection(void);
	inline void ChangeDirection(void);
	inline void DoIdle(bool idle);

private:
	CDXSurface* pSurfMoveLeft[3];
	CDXSurface* pSurfMoveRight[3];

	CDXSoundBuffer *pSoundGluck;

	DIR dir;  // Aktuelle Richtung, links o. rechts
	MOVE mov;
	bool bIdle;
};

inline void ZeldaChickenModule::SetDirection(DIR dir)
{
	this->dir = dir;
}

inline DIR ZeldaChickenModule::GetDirection(void)
{
	return dir;
}

inline void ZeldaChickenModule::ChangeDirection(void)
{
	dir = (dir == DIR_LEFT) ? DIR_RIGHT : DIR_LEFT;
}

inline MOVE ZeldaChickenModule::GetMoveCmd(void)
{
	return mov;
}

inline void ZeldaChickenModule::DoIdle(bool idle)
{
	bIdle = idle;
}

/*
 * class ZeldaSoldierModule
 *
 * Das Modul ZeldaSoldierModule erweitert das "Soldat" Objekt um ein Eigenleben. Ohne dieses
 * Modul würde der Soldat über keine Intelligenz verfügen, sondern wäre nur dann ein ganz 
 * normales Objekt. Wer will denn schon gegen eine Statue kämpfen?
 *
 */

class ZeldaSoldierModule : public ZeldaModule
{
public:
	ZeldaSoldierModule();
	~ZeldaSoldierModule();

	void Update(void);

	void Move(MOVE move);
	inline MOVE GetMoveCmd(void);
	inline void SetDirection(DIR dir);
	inline DIR GetDirection(void);
	inline void DoIdle(bool idle);
	void Hurt(void);
	bool SoldierInRadius(int& x, int& y, int& x_middle, int& y_middle);
	void SearchLink(void);
	void CheckHurt(void);

private:
	CDXSurface* pSurfMoveLeft[2];
	CDXSurface* pSurfMoveRight[2];
	CDXSurface* pSurfMoveUp[2];
	CDXSurface* pSurfMoveDown[2];

	CDXSoundBuffer *pSoundHurt;
	CDXSoundBuffer *pSoundDying;

	DIR dir;
	MOVE mov;
	bool bIdle;
	bool bDying;
	bool bFollowLink;
	bool bPatroullieHorz;
	bool bHurted;
	int nRadius;
	int nSightRadius;
	int nLives;
	int nPatroullieSteps;
	int nDyingCount;
	int nMoveFrame;
	int nMoveCount;
	int nPatroullieFrame;
	int nPatroullieDir;  // 1 == rechts, oben, -1 == links, unten
	int nHurtWait;       // Etwas warten bis zum nächsten Schlag
	int nHurtedCount;    // Etwas warten wenn Soldat geschlagen wurde
};

inline void ZeldaSoldierModule::SetDirection(DIR dir)
{
	this->dir = dir;
}

inline DIR ZeldaSoldierModule::GetDirection(void)
{
	return dir;
}

inline MOVE ZeldaSoldierModule::GetMoveCmd(void)
{
	return mov;
}

inline void ZeldaSoldierModule::DoIdle(bool idle)
{
	bIdle = idle;
}

/*
 * class ZeldaHighGrasModule
 *
 * Das Modul ZeldaHighGrasModule erweitert das "hohe Gras" Objekt. Wenn Link durch
 * hohes Gras läuft, erscheint eine kleine Gras Animation und macht dabei Geräusche.
 *
 */

class ZeldaHighGrasModule : public ZeldaModule
{
public:
	ZeldaHighGrasModule();
	~ZeldaHighGrasModule();

	void Update(void);
};

#endif