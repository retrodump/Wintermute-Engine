// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSTRINGTABLE_H__F033F3FC_2012_41BB_A5E1_5D6AED953563__INCLUDED_)
#define AFX_BSTRINGTABLE_H__F033F3FC_2012_41BB_A5E1_5D6AED953563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "BBase.h"

class CBStringTable : public CBBase  
{
public:
	static BOOL CALLBACK LocWinCallback(HWND hwnd, LPARAM lParam);
	HRESULT LocalizeWindow(HWND Window);
	const char* ExpandStatic(const char* String, bool ForceExpand=false);
	HRESULT LoadFile(char* Filename, bool DeleteAll=true);
	void Expand(char** Str, bool ForceExpand=false);
	HRESULT AddString(const char* Key, const char* Val, bool ReportDuplicities=true);
	CBStringTable(CBGame* inGame);
	virtual ~CBStringTable();
	map<string, string> m_Strings;
	char* GetKey(const char* Str);
private:
	map<string, string>::iterator m_StringsIter;

};

#endif // !defined(AFX_BSTRINGTABLE_H__F033F3FC_2012_41BB_A5E1_5D6AED953563__INCLUDED_)
