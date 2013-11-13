// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADSCALELEVEL_H__250F4095_0B33_4B86_9D62_2E0EDFEE130A__INCLUDED_)
#define AFX_ADSCALELEVEL_H__250F4095_0B33_4B86_9D62_2E0EDFEE130A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CAdScaleLevel : public CBObject  
{
public:
	DECLARE_PERSISTENT(CAdScaleLevel, CBObject);
	float m_Scale;
	CAdScaleLevel(CBGame* inGame);
	virtual ~CAdScaleLevel();
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
};

#endif // !defined(AFX_ADSCALELEVEL_H__250F4095_0B33_4B86_9D62_2E0EDFEE130A__INCLUDED_)
