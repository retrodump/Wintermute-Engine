// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_VIDRENDERERDD_H__8857C074_B13F_412C_98BE_CE2E22CC58BE__INCLUDED_)
#define AFX_VIDRENDERERDD_H__8857C074_B13F_412C_98BE_CE2E22CC58BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VidRenderer.h"

class CVidRendererDD : public CVidRenderer  
{
public:
	virtual HRESULT GetTargetFormat(LPBITMAPV4HEADER Format);
	virtual HRESULT ProcessFrame(LPBITMAPINFOHEADER Frame);
	virtual HRESULT ProcessFrameSlow(LPBITMAPINFOHEADER Frame);
	CVidRendererDD(CBGame* inGame);
	virtual ~CVidRendererDD();

};

#endif // !defined(AFX_VIDRENDERERDD_H__8857C074_B13F_412C_98BE_CE2E22CC58BE__INCLUDED_)
