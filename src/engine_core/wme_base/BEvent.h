// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BEVENT_H__EFC9DD8B_C6ED_42E7_BDA3_B46FD62150A5__INCLUDED_)
#define AFX_BEVENT_H__EFC9DD8B_C6ED_42E7_BDA3_B46FD62150A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "dctypes.h"	// Added by ClassView

class CBEvent : public CBBase  
{
public:
	DECLARE_PERSISTENT(CBEvent, CBBase);
	void SetScript(char* Script);
	void SetName(char* Name);
	static const char* GetEventName(TEventType Type);
	char* m_Script;
	char* m_Name;
	TEventType m_Type;
	CBEvent(CBGame* inGame);
	CBEvent(CBGame* inGame, TEventType Type, char* Script);
	virtual ~CBEvent();
	HRESULT LoadFile(char * Filename);
	HRESULT LoadBuffer(BYTE * Buffer, bool Complete=true);
};

#endif // !defined(AFX_BEVENT_H__EFC9DD8B_C6ED_42E7_BDA3_B46FD62150A5__INCLUDED_)
