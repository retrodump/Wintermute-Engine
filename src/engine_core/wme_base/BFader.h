// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BFADER_H__D118E985_8DD9_4196_94B1_2F2CF283DC51__INCLUDED_)
#define AFX_BFADER_H__D118E985_8DD9_4196_94B1_2F2CF283DC51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CBFader : public CBObject  
{
public:
	bool m_System;
	DWORD GetCurrentColor();
	HRESULT FadeOut(DWORD TargetColor, DWORD Duration, bool System = false);
	HRESULT FadeIn(DWORD SourceColor, DWORD Duration, bool System = false);
	HRESULT Deactivate();
	HRESULT Display();
	HRESULT Update();
	DECLARE_PERSISTENT(CBFader, CBObject);
	CBFader(CBGame* inGame);
	virtual ~CBFader();
	bool m_Active;
	BYTE m_Red;
	BYTE m_Green;
	BYTE m_Blue;
	BYTE m_CurrentAlpha;
	BYTE m_TargetAlpha;
	BYTE m_SourceAlpha;
	DWORD m_Duration;
	DWORD m_StartTime;
};

#endif // !defined(AFX_BFADER_H__D118E985_8DD9_4196_94B1_2F2CF283DC51__INCLUDED_)
