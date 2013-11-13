#ifndef _UTILS_MFC_H_
#define _UTILS_MFC_H_

#define DCGF_TOOLS_REG_PATH "SOFTWARE\\DEAD:CODE\\Wintermute Tools\\Settings"


#include "../engine_core/wme_base/dcgf.h"

bool UpdateIntEdit(CWnd* Owner, CEdit* Edit, int* Var);
CString GetRelativeFilename(CBGame *Game, CString InitFile, CString Filter, CString& Dir, CWnd* Parent=NULL, bool AllowCreate=false);
CString MakePath(CString Path);
CString MakeAbsolutePath(CString RelativePath, CBGame* Game);
bool EditFile(CString Filename, CBGame* Game, CWnd* Parent=NULL, bool OpenOnly=false);

int NumEntries(CString List, char Delimiter=',');
int NumEntriesEx(CString List, char Delimiter=',');
CString Entry(int Entry, CString List, char Delimiter=',');
CString EntryEx(int Entry, CString List, char Delimiter=',');

CString GetExt(CString Filename);
CString GetPath(CString Filename);
CString GetFName(CString Filename);
CString GetRelativePath(CBGame* Game, CString Filename);
CString GetAbsolutePath(CString Base, CString RelPath);
int ComparePattern(const char* pattern, const char* string);
CString GetRegString(HKEY hKey, CString Path, CString Key, CString Init="");
bool RegKeyExists(HKEY hKey, CString Path);
int GetRegInt(HKEY hKey, CString Path, CString Key, int Init=-1);
bool SetRegString(HKEY hKey, CString Path, CString Key, CString Value);
bool SetRegInt(HKEY hKey, CString Path, CString Key, int Value);
bool DelRegKey(HKEY hKey, CString Path, CString Key);

#endif // _UTILS_MFC_H_
