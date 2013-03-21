// DCGFStringTable.h: interface for the CDCGFStringTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCGFSTRINGTABLE_H__D500CFB9_C080_4667_BAD6_A7A439588128__INCLUDED_)
#define AFX_DCGFSTRINGTABLE_H__D500CFB9_C080_4667_BAD6_A7A439588128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LOC(String) (CDCGFStringTable::GetString(String))
#define LOCWIN(Window) (CDCGFStringTable::LocWindow(Window))

class CDCGFStringTable  
{
public:
	static bool LocMenu(CMenu* Menu);
	static bool LocWindow(CWnd* Window);
	static BOOL CALLBACK LocWinCallback(HWND hwnd, LPARAM lParam);
	static bool Init();
	static const char* GetString(const char* Original);
	static bool LoadStringTable(CString Filename);
	CDCGFStringTable();
	virtual ~CDCGFStringTable();
	static CMapStringToString m_Dict;
//	static CObject m_Dict2;
};

#endif // !defined(AFX_DCGFSTRINGTABLE_H__D500CFB9_C080_4667_BAD6_A7A439588128__INCLUDED_)
