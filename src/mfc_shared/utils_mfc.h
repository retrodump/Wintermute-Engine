#ifndef _UTILS_MFC_H_
#define _UTILS_MFC_H_

#include "dcgf.h"

bool UpdateIntEdit(CWnd* Owner, CEdit* Edit, int* Var);
bool UpdateFloatEdit(CWnd* Owner, CEdit* Edit, float* Var);
void SetupButton(CBCGButton* Button, LPCTSTR Tooltip, int ID);
CString GetRelativeFilename(CBGame *Game, CString InitFile, CString Filter, CString& Dir, CWnd* Parent=NULL, bool AllowCreate=false);
CString MakePath(CString Path);
CString MakeAbsolutePath(CString RelativePath, CBGame* Game);
CString GetPath(CString Filename);

int NumEntries(CString List, char Delimiter=',');
int NumEntriesEx(CString List, char Delimiter=',');
CString Entry(int Entry, CString List, char Delimiter=',');
CString EntryEx(int Entry, CString List, char Delimiter=',');


CString GetExt(CString Filename);
CString GetPath(CString Filename);
CString GetRelativePath(CBGame* Game, CString Filename);
CString GetAbsolutePath(CString Base, CString RelPath);


#endif // _UTILS_MFC_H_