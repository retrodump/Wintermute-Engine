// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SXMATH_H__BA740577_83AE_4D95_87DA_801F4B86888F__INCLUDED_)
#define AFX_SXMATH_H__BA740577_83AE_4D95_87DA_801F4B86888F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BScriptable.h"

class CSXMath : public CBScriptable  
{
public:
	DECLARE_PERSISTENT(CSXMath, CBScriptable);
	CSXMath(CBGame* inGame);
	virtual ~CSXMath();
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);

private:
	double DegreeToRadian(double Value);
	double RadianToDegree(double Value);

};

#endif // !defined(AFX_SXMATH_H__BA740577_83AE_4D95_87DA_801F4B86888F__INCLUDED_)
