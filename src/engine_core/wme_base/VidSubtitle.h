// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_VIDSUBTITLE_H__E670B863_2E61_46C7_8C91_23303AB4B622__INCLUDED_)
#define AFX_VIDSUBTITLE_H__E670B863_2E61_46C7_8C91_23303AB4B622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CVidSubtitle : public CBBase  
{
public:
	LONG m_EndFrame;
	LONG m_StartFrame;
	char* m_Text;
	CVidSubtitle(CBGame* inGame);
	CVidSubtitle(CBGame* inGame, char* Text, LONG StartFrame, LONG EndFrame);
	virtual ~CVidSubtitle();

};

#endif // !defined(AFX_VIDSUBTITLE_H__E670B863_2E61_46C7_8C91_23303AB4B622__INCLUDED_)
