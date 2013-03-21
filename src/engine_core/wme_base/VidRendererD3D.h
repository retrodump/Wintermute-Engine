// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_VIDRENDERERD3D_H__086CCBC8_092E_43DE_B458_1D90DA2425A4__INCLUDED_)
#define AFX_VIDRENDERERD3D_H__086CCBC8_092E_43DE_B458_1D90DA2425A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VidRenderer.h"

class CVidRendererD3D : public CVidRenderer  
{
public:
	virtual HRESULT GetTargetFormat(LPBITMAPV4HEADER Format);
	virtual HRESULT ProcessFrame(LPBITMAPINFOHEADER Frame);
	virtual HRESULT ProcessFrameSlow(LPBITMAPINFOHEADER Frame);
	HRESULT ProcessFrameDivTex(LPBITMAPINFOHEADER Frame);
	CVidRendererD3D(CBGame* inGame);
	virtual ~CVidRendererD3D();
};

#endif // !defined(AFX_VIDRENDERERD3D_H__086CCBC8_092E_43DE_B458_1D90DA2425A4__INCLUDED_)
