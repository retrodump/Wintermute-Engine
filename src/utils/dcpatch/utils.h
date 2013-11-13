#ifndef _UTILS_H_
#define _UTILS_H_

CString Entry(int Entry, CString List, char Delimiter);
CString GetRegString(HKEY hKey, CString Path, CString Key, CString Init="");
HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor=0, COLORREF cTolerance=0x101010);
CString MakePath(CString Path);


#endif