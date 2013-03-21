// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "bbase.h"

class CBNamedObject : public CBBase
{
public:
	CBNamedObject(CBGame* inGame);
	CBNamedObject();
	virtual ~CBNamedObject(void);
	CBNamedObject(TDynamicConstructor, TDynamicConstructor);

	char* m_Name;
	void SetName(char* Name);
};
