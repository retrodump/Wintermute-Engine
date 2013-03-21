// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BRENDERER_H__DAA97523_137F_11D4_9F37_8F90855A4202__INCLUDED_)
#define AFX_BRENDERER_H__DAA97523_137F_11D4_9F37_8F90855A4202__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "coll_templ.h"
class C3DCamera;
class CBImage;

class CBRenderer:public CBBase
{
public:
	int m_RealWidth;
	int m_RealHeight;
	int m_DrawOffsetX;
	int m_DrawOffsetY;

	virtual void DumpData(char* Filename){};
	virtual CBImage* TakeScreenshot();
	virtual HRESULT SetViewport(int left, int top, int right, int bottom);
	virtual HRESULT SetViewport(RECT* Rect);
	virtual HRESULT SetScreenViewport();
	virtual HRESULT Fade(WORD Alpha);
	virtual HRESULT FadeToColor(DWORD Color, RECT* rect=NULL);
	virtual HRESULT DrawLine(int X1, int Y1, int X2, int Y2, DWORD Color);
	virtual HRESULT DrawRect(int X1, int Y1, int X2, int Y2, DWORD Color, int Width=1);
	CBRenderer(CBGame* inGame=NULL);
	virtual ~CBRenderer();
	virtual HRESULT SetProjection() { return S_OK; };

	virtual HRESULT WindowedBlt();	
	virtual HRESULT Fill(BYTE r, BYTE g, BYTE b, RECT* rect=NULL);
	virtual void OnWindowChange();
	virtual HRESULT InitRenderer(CHWManager* hwManager);
	virtual HRESULT Flip();
	virtual void InitLoop();
	virtual HRESULT SwitchFullscreen();
	virtual HRESULT Setup3D(C3DCamera* Camera=NULL, bool Force=false);
	virtual HRESULT Setup3DCustom(D3DXMATRIX* ViewMat, D3DXMATRIX* ProjMat);
	virtual HRESULT Setup2D(bool Force=false);
	virtual HRESULT SetupLines();
	virtual bool UsingStencilBuffer(){ return false; };

	virtual HRESULT EnableShadows() { return E_FAIL; };
	virtual HRESULT DisableShadows() { return E_FAIL; };
	virtual bool StencilSupported() { return false; };

	virtual const char* GetName(){ return ""; };
	virtual HRESULT DisplayDebugInfo() { return E_FAIL; };
	virtual HRESULT DrawShaderQuad() { return E_FAIL; }

	HRESULT ClipCursor();
	HRESULT UnclipCursor();

	CBObject* GetObjectAt(int X, int Y);
	void DeleteRectList();

	virtual HRESULT StartSpriteBatch() { return S_OK; };
	virtual HRESULT EndSpriteBatch() { return S_OK; };
	bool PointInViewport(POINT* P);
	DWORD m_ForceAlphaColor;
	HINSTANCE m_Instance;
	HWND m_Window;
	HWND m_ClipperWindow;
	bool m_Active;
	bool m_Ready;
	bool m_Windowed;
	RECT m_WindowRect;
	RECT m_ViewportRect;
	RECT m_ScreenRect;
	RECT m_MonitorRect;
	int m_BPP;	
	int m_Height;
	int m_Width;

	CBArray<CBActiveRect*, CBActiveRect*> m_RectList;
};

#endif // !defined(AFX_BRENDERER_H__DAA97523_137F_11D4_9F37_8F90855A4202__INCLUDED_)
