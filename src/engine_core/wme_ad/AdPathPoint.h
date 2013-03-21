// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADPATHPOINT_H__B5EDED63_5A19_4DA9_9B4F_CB3709F9A506__INCLUDED_)
#define AFX_ADPATHPOINT_H__B5EDED63_5A19_4DA9_9B4F_CB3709F9A506__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPoint.h"

class CAdPathPoint : public CBPoint  
{
public:
	DECLARE_PERSISTENT(CAdPathPoint, CBPoint);
	CAdPathPoint(int initX, int initY, int initDistance);
	CAdPathPoint();
	virtual ~CAdPathPoint();
	CAdPathPoint* m_Origin;
	bool m_Marked;
	int m_Distance;
};

#endif // !defined(AFX_ADPATHPOINT_H__B5EDED63_5A19_4DA9_9B4F_CB3709F9A506__INCLUDED_)
