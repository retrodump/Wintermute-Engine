// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_HWVIDEOMODE_H__1FC51B6C_C330_4C06_AB0F_CEF11F4B6998__INCLUDED_)
#define AFX_HWVIDEOMODE_H__1FC51B6C_C330_4C06_AB0F_CEF11F4B6998__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CHWVideoMode : public CBBase  
{
public:
	int m_BPP;
	bool m_Windowed;
	bool m_Multisample[17];
	int m_Height;
	int m_Width;
	D3DFORMAT m_Format;
	D3DFORMAT m_DepthStencilFormat;
	bool m_SupportsStencil;
	CHWVideoMode(CBGame* inGame);
	virtual ~CHWVideoMode();

};

#endif // !defined(AFX_HWVIDEOMODE_H__1FC51B6C_C330_4C06_AB0F_CEF11F4B6998__INCLUDED_)
