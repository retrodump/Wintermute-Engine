// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BQUICKMSG_H__657AE639_B959_42AA_BAEF_C8E1D57C48E4__INCLUDED_)
#define AFX_BQUICKMSG_H__657AE639_B959_42AA_BAEF_C8E1D57C48E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBQuickMsg : public CBBase  
{
public:
	char* GetText();
	DWORD m_StartTime;
	char* m_Text;
	CBQuickMsg(CBGame* inGame, char* Text);
	virtual ~CBQuickMsg();

};

#endif // !defined(AFX_BQUICKMSG_H__657AE639_B959_42AA_BAEF_C8E1D57C48E4__INCLUDED_)
