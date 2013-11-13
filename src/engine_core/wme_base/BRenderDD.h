// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BRENDERDD_H__53478464_B66E_429C_B549_DE9BDA2CFFD8__INCLUDED_)
#define AFX_BRENDERDD_H__53478464_B66E_429C_B549_DE9BDA2CFFD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BRenderer.h"

class C3DCamera;

class CBRenderDD : public CBRenderer  
{
public:
	virtual const char* GetName(){ return "DirectDraw7"; };
	virtual HRESULT DisplayDebugInfo();
	virtual CBImage* TakeScreenshot();
	HRESULT SetViewport(int left, int top, int right, int bottom);
	bool m_SupportsWideSurfaces;
	virtual HRESULT Fade(WORD Alpha);
	virtual HRESULT FadeToColor(DWORD Color, RECT* rect=NULL);
	virtual HRESULT DrawLine(int X1, int Y1, int X2, int Y2, DWORD Color);
	CBRenderDD(CBGame* inGame);
	virtual ~CBRenderDD();
	HRESULT InitSurfaces();
	HRESULT ReleaseAll();
	DWORD GetFreeVideoMem();
	DWORD GetUsedVideoMem();
	LPDIRECTDRAW7 m_DirectDraw;
	LPDIRECTDRAWSURFACE7 m_FrontBuffer;
	LPDIRECTDRAWSURFACE7 m_BackBuffer;
	bool m_WinBackBufferSysMem;
	void SetPaletteFile(char* Filename);
	char* m_PaletteFile;
	LPDIRECTDRAWPALETTE m_Palette;	

	virtual HRESULT WindowedBlt();	
	virtual HRESULT Fill(BYTE r, BYTE g, BYTE b, RECT* rect=NULL);
	virtual void OnWindowChange();
	virtual HRESULT InitRenderer(CHWManager* hwManager);
	virtual HRESULT Flip();
	virtual HRESULT SwitchFullscreen();
	virtual HRESULT Setup3D(C3DCamera* Camera=NULL, bool Force=false);
	virtual HRESULT Setup2D(bool Force=false);
	virtual HRESULT SetupLines();

private:
	BYTE m_LastFillR;
	BYTE m_LastFillG;
	BYTE m_LastFillB;
	DWORD m_LastFillNative;
};

#endif // !defined(AFX_BRENDERDD_H__53478464_B66E_429C_B549_DE9BDA2CFFD8__INCLUDED_)
