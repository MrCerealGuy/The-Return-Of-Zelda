/*====================================================================

  PROJECT		: The Return Of Zelda

  --------------------------------------------------------------------

  FILE			: status.h

  AUTHOR		: Andreas Zahnleiter

 =====================================================================*/


#ifndef __STATUS_H__
#define __STATUS_H__

class ZeldaStatus
{
public:
	ZeldaStatus();
	~ZeldaStatus();

	void DrawLinkStatus(void);

private:
	CDXSurface* pSurfHeartFull;
	CDXSurface* pSurfHeartEmpty;
	CDXSurface* pSurfLifeTitle;
};

#endif