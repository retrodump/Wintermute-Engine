// DCGFStringTable.cpp: implementation of the CDCGFStringTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DCGFStringTable.h"
#include "BCMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapStringToString CDCGFStringTable::m_Dict;

//////////////////////////////////////////////////////////////////////////
CDCGFStringTable::CDCGFStringTable()
{

}


//////////////////////////////////////////////////////////////////////////
CDCGFStringTable::~CDCGFStringTable()
{

}


//////////////////////////////////////////////////////////////////////////
bool CDCGFStringTable::Init()
{
	CString AppPath;

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char full_path[MAX_PATH];

	GetModuleFileName(AfxGetInstanceHandle(), full_path, MAX_PATH);
	_splitpath(full_path, drive, dir, NULL, NULL);
	AppPath.Format("%s%s", drive, dir);
	if(AppPath=="" || AppPath[AppPath.GetLength()-1]!='\\')
		AppPath += "\\";

	return LoadStringTable(AppPath + "loc\\" + CString(AfxGetAppName()) + "\\default.lng");
}


//////////////////////////////////////////////////////////////////////////
bool CDCGFStringTable::LoadStringTable(CString Filename)
{
	m_Dict.RemoveAll();

	CStdioFile f;
	CString line;

	if(f.Open(Filename, CFile::modeRead | CFile::typeText)){
		while(f.ReadString(line)){
			if(line.GetLength() < 1 || line[0]==';') continue;

			CString Key, String;
			bool InKey=true;
			for(int i=0; i<line.GetLength(); i++){
				char c = line[i];

				if(c=='\t'){
					if(InKey){
						InKey = false;
						continue;
					}
					else break;
				}

				if(c=='\\' && i<line.GetLength()-1){
					char c2 = line[i+1];
					switch(c2){
						case 'n':  c = '\n'; i++; break;
						case 't':  c = '\t'; i++; break;
						case '\\': c = '\\'; i++; break;
					}
				}

				if(InKey) Key+=c;
				else String+=c;
			}
			Key.MakeLower();
			
			if(Key.GetLength() > 0 && Key!="comment" && Key[0]!='$')
#if _DEBUG
				m_Dict[Key] = CString("l:") + String;
#else
				m_Dict[Key] = String;
#endif
		}
		f.Close();
		return true;
	}
	else return false;
}


//////////////////////////////////////////////////////////////////////////
const char* CDCGFStringTable::GetString(const char *Original)
{
	CString ID;
	CString String;

	if(Original==NULL || strlen(Original)<3 || Original[0]!='/') return Original;

	char* value = (char*)strchr(Original+1, '/');
	if(value==NULL) return Original;

	// extract key
	char* key = new char[value-Original];
	strncpy(key, Original+1, value-Original-1);
	key[value-Original-1] = '\0';
	_strlwr(key);
	
	// extract string
	char* new_str = value+1;

	CString Trans;
	bool IsPresent = (m_Dict.Lookup(key, Trans)!=FALSE);
	delete [] key;

	if(IsPresent) return Trans;
	else return new_str;
}


//////////////////////////////////////////////////////////////////////////
bool CDCGFStringTable::LocWindow(CWnd *Window)
{
	if(!Window || !Window->GetSafeHwnd()) return false;

	CString WinText;
	Window->GetWindowText(WinText);
	Window->SetWindowText(LOC(WinText));

	return EnumChildWindows(Window->GetSafeHwnd(), LocWinCallback, (LPARAM)0)!=0;
}


//////////////////////////////////////////////////////////////////////////
BOOL CDCGFStringTable::LocWinCallback(HWND hwnd, LPARAM lParam)
{
	CWnd* Window = CWnd::FromHandle(hwnd);
	if(!Window) return FALSE;

	CString WinText;
	Window->GetWindowText(WinText);
	Window->SetWindowText(LOC(WinText));
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
bool CDCGFStringTable::LocMenu(CMenu* Menu)
{
	for(int i=0; i<(int)Menu->GetMenuItemCount(); i++){
		UINT cmd = Menu->GetMenuItemID(i);
		if(cmd==0) continue;
		
		CString Str;

		Menu->GetMenuString(i, Str, MF_BYPOSITION);

		if(Str=="" && Menu->IsKindOf(RUNTIME_CLASS(BCMenu))){
			((BCMenu*)Menu)->GetMenuText(i, Str);
			((BCMenu*)Menu)->SetMenuText(i, LOC(Str));
		}
		else{
			Menu->ModifyMenu(i, MF_BYPOSITION|MF_STRING, cmd, LOC(Str));
		}

		CMenu* SubMenu = Menu->GetSubMenu(i);
		if(SubMenu) LocMenu(SubMenu);
	}

	return true;
}
