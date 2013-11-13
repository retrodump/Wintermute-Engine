// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADPATH_H__BC1EB555_D0D7_4ADE_A985_FC075B9886D5__INCLUDED_)
#define AFX_ADPATH_H__BC1EB555_D0D7_4ADE_A985_FC075B9886D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CAdPath : public CBBase  
{
public:
	DECLARE_PERSISTENT(CAdPath, CBBase);
	CBPoint* GetCurrent();
	bool SetReady(bool ready=true);
	void AddPoint(CBPoint* point);
	CBPoint* GetNext();
	CBPoint* GetFirst();
	void Reset();
	CAdPath(CBGame* inGame);
	virtual ~CAdPath();
	CBArray <CBPoint*, CBPoint*> m_Points;
	int m_CurrIndex;
	bool m_Ready;
};

#endif // !defined(AFX_ADPATH_H__BC1EB555_D0D7_4ADE_A985_FC075B9886D5__INCLUDED_)
