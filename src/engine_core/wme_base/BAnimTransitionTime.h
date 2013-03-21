// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

class CBAnimTransitionTime
{
public:
	CBAnimTransitionTime(char* From, char* To, DWORD Time);
	CBAnimTransitionTime();
	virtual ~CBAnimTransitionTime(void);
	HRESULT Persist(CBPersistMgr* PersistMgr);

	char* m_AnimFrom;
	char* m_AnimTo;
	DWORD m_Time;
};
