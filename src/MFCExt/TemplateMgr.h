// TemplateMgr.h: interface for the CTemplateMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPLATEMGR_H__BA0687C8_4431_4831_A40C_42887BCDBCA8__INCLUDED_)
#define AFX_TEMPLATEMGR_H__BA0687C8_4431_4831_A40C_42887BCDBCA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_IF_NEST 5

#include "AfxTempl.h"
#include "TemplateProp.h"

class CTemplateMgr  
{
public:
	void ResetParser();
	void Cleanup();
	CString GetErrorString();
	bool LoadDefinitions(CString Filename);
	bool m_NoDir;
	CString m_Description;
	bool ReadExtendedInfo(CStringArray& InArray);
	bool FileToFile(CString Source, CString Target);
	bool EvaluateMacroList(CString Macros);
	void AddError(CString Error, int Line=-1);
	bool ExpandMacros(CString Line, CString& Result, bool Uppercase=false);
	int m_CurrLine;
	bool m_IfIsTrue[MAX_IF_NEST];
	bool m_IfProcessed[MAX_IF_NEST];
	bool m_IfHasElse[MAX_IF_NEST];
	int m_IfNest;
	int m_LoopStart;
	bool ParseTemplate(CStringArray& InArray, CStringArray& OutArray);
	CTemplateMgr();
	virtual ~CTemplateMgr();
	CStringArray m_Errors;

	CMapStringToString m_Dict;
	CArray<CTemplateProp*, CTemplateProp*> m_Props;
private:
	void ClearProps();
	bool GetMacroList(CString Line, CString& Result, int Starting=0);
};

#endif // !defined(AFX_TEMPLATEMGR_H__BA0687C8_4431_4831_A40C_42887BCDBCA8__INCLUDED_)
