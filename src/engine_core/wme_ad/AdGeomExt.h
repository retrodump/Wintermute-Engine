// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CAdGeomExt : public CBBase
{
public:
	CAdGeomExt(CBGame* inGame);
	virtual ~CAdGeomExt(void);
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer);
	HRESULT AddStandardNodes();
	CAdGeomExtNode* MatchName(char* Name);

private:
	CBArray<CAdGeomExtNode*, CAdGeomExtNode*> m_Nodes;
};
