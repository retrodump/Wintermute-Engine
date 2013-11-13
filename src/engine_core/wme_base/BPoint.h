// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BPOINT_H__B0B4C789_2076_44C5_909B_3FEEE4822FE8__INCLUDED_)
#define AFX_BPOINT_H__B0B4C789_2076_44C5_909B_3FEEE4822FE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBPoint: public CBBase
{
public:
	DECLARE_PERSISTENT(CBPoint, CBBase);
	CBPoint();
	CBPoint(int initX, int initY);
	int y;
	int x;
	virtual ~CBPoint();

};

#endif // !defined(AFX_BPOINT_H__B0B4C789_2076_44C5_909B_3FEEE4822FE8__INCLUDED_)
