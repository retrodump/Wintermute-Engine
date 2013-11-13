// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_VIDRENDERER_H__6F577475_4E5E_491A_BE10_67980553F9A0__INCLUDED_)
#define AFX_VIDRENDERER_H__6F577475_4E5E_491A_BE10_67980553F9A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CVidRenderer : public CBBase
{
public:		
	HRESULT RenderFrame(BYTE* SrcBuf, BYTE* TargetBuf, LONG SrcPitch, LONG TargetPitch, LONG Width, LONG Height);
	virtual HRESULT GetTargetFormat(LPBITMAPV4HEADER Format);
	virtual HRESULT ProcessFrame(LPBITMAPINFOHEADER);
	virtual HRESULT ProcessFrameSlow(LPBITMAPINFOHEADER);
	virtual HRESULT Initialize(LPBITMAPINFO Format, LPBITMAPV4HEADER TargetFormat);
	HRESULT Display(int X, int Y, float Zoom);
	CBSurface* m_Surface;
	CVidRenderer(CBGame* inGame);
	virtual ~CVidRenderer();
};

#endif // !defined(AFX_VIDRENDERER_H__6F577475_4E5E_491A_BE10_67980553F9A0__INCLUDED_)
