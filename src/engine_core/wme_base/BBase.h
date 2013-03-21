// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BBASE_H__FE7D21E1_5646_11D4_B319_89ABE791FA72__INCLUDED_)
#define AFX_BBASE_H__FE7D21E1_5646_11D4_B319_89ABE791FA72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <map>
#include <string>

class CBGame;
class CBDynBuffer;

class CBBase  
{
public:
	bool m_Persistable;
	HRESULT SetEditorProp(const char* PropName, const char* PropValue);
	const char* GetEditorProp(const char* PropName, const char* InitVal=NULL);
	CBBase(TDynamicConstructor, TDynamicConstructor){};
	HRESULT ParseEditorProperty(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent=0);
	CBBase();
	CBGame* Game;
	CBBase(CBGame* GameOwner);
	virtual ~CBBase();

	map<string, string> m_EditorProps;
	map<string, string>::iterator m_EditorPropsIter;
};

#endif // !defined(AFX_BBASE_H__FE7D21E1_5646_11D4_B319_89ABE791FA72__INCLUDED_)
