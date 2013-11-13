// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BVIEWPORT_H__F626DD9B_8411_4B1F_B464_D3D4BC7BAF21__INCLUDED_)
#define AFX_BVIEWPORT_H__F626DD9B_8411_4B1F_B464_D3D4BC7BAF21__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "BBase.h"

class CBViewport : public CBBase  
{
public:
	int GetHeight();
	int GetWidth();
	RECT* GetRect();
	HRESULT SetRect(int left, int top, int right, int bottom, bool NoCheck=false);
	DECLARE_PERSISTENT(CBViewport, CBBase);
	int m_OffsetY;
	int m_OffsetX;
	CBObject* m_MainObject;
	CBViewport(CBGame* inGame=NULL);
	virtual ~CBViewport();
private:
	RECT m_Rect;
};

#endif // !defined(AFX_BVIEWPORT_H__F626DD9B_8411_4B1F_B464_D3D4BC7BAF21__INCLUDED_)
