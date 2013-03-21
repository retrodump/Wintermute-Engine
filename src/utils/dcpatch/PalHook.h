////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-1999 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#ifndef _PALHOOK_H
#define _PALHOOK_H

#include "Subclass.h"

//////////////////
// Generic palette message handler makes handling palette messages easy.
// To use:
//
// * Instaniate a CPalMsgHandler in your main frame and
//   every CWnd class that needs to realize palettes (e.g., your view).
// * Call Install to install.
// * Call DoRealizePalette(TRUE) from your view's OnInitialUpdate fn.
//
class CPalMsgHandler : protected CSubclassWnd {
public:
	DECLARE_DYNAMIC(CPalMsgHandler);
	CPalMsgHandler();
	~CPalMsgHandler();

	// Get/Set palette obj
	CPalette* GetPalette()				{ return m_pPalette; }
	void SetPalette(CPalette* pPal)	{ m_pPalette = pPal; }

	// Call this to install the palette handler
	BOOL Install(CWnd* pWnd, CPalette* pPal) {
		m_pPalette = pPal;
		return HookWindow(pWnd);
	}
	BOOL IsHooked() { return CSubclassWnd::IsHooked(); }

	// Call this from your OnInitialUpdate functions.
	// Override if you realize your palette some other way
	// (ie, not by having a ptr to a CPalette you pass to SetPalette).
	//
	virtual int  DoRealizePalette(BOOL bForeground);

	static BOOL bTRACE;	// Set TRUE to see extra diagnostics in DEBUG code

protected:
	CPalette* m_pPalette; // ptr to palette

	// These are similar to, but NOT the same as the equivalent CWnd fns.
	// Rarely, if ever need to override.
	//
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);	
	virtual void	 OnPaletteChanged(CWnd* pFocusWnd);
	virtual BOOL	 OnQueryNewPalette();
	virtual void	 OnSetFocus(CWnd* pOldWnd);

};

#endif // _PALHOOK_H
