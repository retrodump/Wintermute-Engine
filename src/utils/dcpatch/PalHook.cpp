////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-1999 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#include "StdAfx.h"
#include "PalHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// if you want to see extra TRACE diagnostics, set below to TRUE
BOOL CPalMsgHandler::bTRACE = FALSE;

#ifdef _DEBUG
#define PALTRACEFN			\
	CTraceFn __fooble;		\
	if (bTRACE)					\
		TRACE
#define PALTRACE				\
	if (bTRACE)					\
		TRACE
#else
#define PALTRACEFN TRACE
#define PALTRACE   TRACE
#endif

IMPLEMENT_DYNAMIC(CPalMsgHandler, CSubclassWnd);

CPalMsgHandler::CPalMsgHandler()
{
	m_pPalette = NULL;
}

CPalMsgHandler::~CPalMsgHandler()
{
}

//////////////////
// Message handler handles palette-related messages
//
LRESULT CPalMsgHandler::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
	ASSERT(m_hWnd);

	switch (msg) {
	case WM_PALETTECHANGED:
		OnPaletteChanged(CWnd::FromHandle((HWND)wp));
		return 0;
	case WM_QUERYNEWPALETTE:
		return OnQueryNewPalette();
	case WM_SETFOCUS:
		OnSetFocus(CWnd::FromHandle((HWND)wp));
		return 0;
	}
	return CSubclassWnd::WindowProc(msg, wp, lp);
}

//////////////////
// Handle WM_PALETTECHANGED
//
void CPalMsgHandler::OnPaletteChanged(CWnd* pFocusWnd)
{
	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);

	if (pFocusWnd->GetSafeHwnd() != wnd.m_hWnd) {
		if (DoRealizePalette(FALSE)==0) {
			if (wnd.GetParent()==NULL) {
				// I'm the top-level frame: Broadcast to children
				// (only MFC permanent CWnd's!)
				//
				const MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
				wnd.SendMessageToDescendants(WM_PALETTECHANGED,
					curMsg.wParam, curMsg.lParam);
			}
		}
	} else {
		// I'm the window that triggered the WM_PALETTECHANGED
		// in the first place: ignore it
		//
		PALTRACE(_T("[It's me, don't realize palette.]\n"));
	}
}

//////////////////
// Handle WM_QUERYNEWPALETTE
//
BOOL CPalMsgHandler::OnQueryNewPalette()
{
	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);

	if (DoRealizePalette(TRUE)==0) {	// realize in foreground
		// No colors changed: if this is the top-level frame,
		// give active view a chance to realize itself
		//
		if (wnd.GetParent()==NULL && wnd.IsFrameWnd()) {
			CWnd* pView = ((CFrameWnd&)wnd).GetActiveFrame()->GetActiveView();
			if (pView) 
				pView->SendMessage(WM_QUERYNEWPALETTE);
		}
	}
	return TRUE;
}

//////////////////
// Handle WM_SETFOCUS
//
void CPalMsgHandler::OnSetFocus(CWnd* pOldWnd) 
{
	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);

	wnd.SetForegroundWindow();		// Windows likes this
	DoRealizePalette(TRUE);			// realize in foreground
	Default();							// let app handle focus message too
}

/////////////////
// Function to actually realize the palette.
// Override this to do different kind of palette realization; e.g.,
// DrawDib instead of setting the CPalette.
//
int CPalMsgHandler::DoRealizePalette(BOOL bForeground)
{
	if (!m_pPalette || !m_pPalette->m_hObject)
		return 0;

	ASSERT(m_hWnd);
	CWnd& wnd = *CWnd::FromHandle(m_hWnd);

	CClientDC dc(&wnd);
	CPalette* pOldPal = dc.SelectPalette(m_pPalette, !bForeground);
	int nColorsChanged = dc.RealizePalette();
	if (pOldPal)
		dc.SelectPalette(pOldPal, TRUE);
	if (nColorsChanged > 0)
		wnd.Invalidate(FALSE); // repaint

	PALTRACE(_T("[%d colors changed]\n"), nColorsChanged);

	return nColorsChanged;
}
