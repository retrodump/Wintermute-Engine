// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CAdGeomExtNode :	public CBBase
{
public:
	CAdGeomExtNode(CBGame* inGame);
	virtual ~CAdGeomExtNode(void);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete);

	HRESULT SetupNode(char* NamePattern, TGeomNodeType Type=GEOM_GENERIC, bool ReceiveShadows=false);
	bool MatchesName(char* Name);

	bool m_ReceiveShadows;
	TGeomNodeType m_Type;	

private:
	char* m_NamePattern;
};
