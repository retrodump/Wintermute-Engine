// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "wme_plugin.h"

class CBParamSet : public CBBase, public IWmeParamSet
{
public:
	CBParamSet(CBGame* inGame);
	virtual ~CBParamSet(void);
	HRESULT PushToStack(CScStack* Stack);

	// IWmeParamSet
	bool Clean();
	bool AddParam(int Value);
	bool AddParam(const char* Value);
	bool AddParam(double Value);
	bool AddParam(bool Value);
	bool AddParam(IWmeObject* Value);
	bool AddParam(IWmeValue* Value);
	bool AddParam();
private:
	CBArray<CScValue*, CScValue*> m_Params;
};
