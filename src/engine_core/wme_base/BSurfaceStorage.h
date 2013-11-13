// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSURFACESTORAGE_H__DAA97524_137F_11D4_9F37_8F90855A4202__INCLUDED_)
#define AFX_BSURFACESTORAGE_H__DAA97524_137F_11D4_9F37_8F90855A4202__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "coll_templ.h"

class CBSurfaceStorage:CBBase
{
public:
	DWORD m_LastCleanupTime;
	HRESULT InitLoop();
	HRESULT SortSurfaces();
	int static __cdecl SurfaceSortCB(const VOID* arg1, const VOID* arg2);
	HRESULT Cleanup(bool Warn=false);
	//DECLARE_PERSISTENT(CBSurfaceStorage, CBBase);

	HRESULT RestoreAll();
	CBSurface* AddSurface(char* Filename, bool default_ck=true, BYTE ck_red=0, BYTE ck_green=0, BYTE ck_blue=0, int LifeTime=-1, bool KeepLoaded=false);
	HRESULT RemoveSurface(CBSurface* surface);
	CBSurfaceStorage(CBGame* inGame);
	virtual ~CBSurfaceStorage();

	CBArray<CBSurface*, CBSurface*> m_Surfaces;
};

#endif // !defined(AFX_BSURFACESTORAGE_H__DAA97524_137F_11D4_9F37_8F90855A4202__INCLUDED_)
