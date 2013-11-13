// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\WinTabCtrl.h"


//////////////////////////////////////////////////////////////////////////
CWinTabCtrl::CWinTabCtrl(void) : CWinWindow()
{
}


//////////////////////////////////////////////////////////////////////////
CWinTabCtrl::~CWinTabCtrl(void)
{
}

//////////////////////////////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::GetItemCount()
{
	if(m_Hwnd) return (int)::SendMessage(m_Hwnd, TCM_GETITEMCOUNT, 0, 0L);
	else return 0;
}

//////////////////////////////////////////////////////////////////////////
bool CWinTabCtrl::GetItem(int Index, TCITEM* Item)
{
	if(m_Hwnd) return ::SendMessage(m_Hwnd, TCM_GETITEM, Index, (LPARAM)Item) != FALSE;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinTabCtrl::SetItemA(int Index, TCITEMA* Item)
{
	if(m_Hwnd) return ::SendMessage(m_Hwnd, TCM_SETITEMA, Index, (LPARAM)Item) != FALSE;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinTabCtrl::SetItemW(int Index, TCITEMW* Item)
{
	if(m_Hwnd) return ::SendMessage(m_Hwnd, TCM_SETITEMW, Index, (LPARAM)Item) != FALSE;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::InsertItemA(int Index, TCITEMA* Item)
{
	if(m_Hwnd) return (int)::SendMessage(m_Hwnd, TCM_INSERTITEMA, Index, (LPARAM)Item);
	else return -1;
}

//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::InsertItemW(int Index, TCITEMW* Item)
{
	if(m_Hwnd) return (int)::SendMessage(m_Hwnd, TCM_INSERTITEMW, Index, (LPARAM)Item);
	else return -1;
}

//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::InsertItem(UINT Mask, int Index, char* Str, int Image, LPARAM lParam, TTextEncoding TextEncoding)
{
	if(!m_Hwnd) return -1;

	int Ret = -1;

	if(TextEncoding==TEXT_ANSI)
	{
		TCITEMA item;
		item.mask = Mask;
		item.iImage = Image;
		item.lParam = lParam;
		item.pszText = (LPTSTR)Str;

		Ret = InsertItemA(Index, &item);
	}
	else if(TextEncoding==TEXT_UTF8)
	{
		int TempBufChars = (strlen(Str)+1);
		wchar_t* Temp = new wchar_t[TempBufChars];

		if(MultiByteToWideChar(CP_UTF8, 0, Str, -1, Temp, TempBufChars))
		{
			if(CBUtils::IsWinNT())
			{
				// on NT use unicode functions
				TCITEMW item;
				item.mask = Mask;
				item.iImage = Image;
				item.lParam = lParam;
				item.pszText = (LPWSTR)Temp;

				Ret = InsertItemW(Index, &item);
			}
			else
			{
				// on Win9x convert to multibyte string using default codepage
				setlocale(LC_ALL, "");
				char* TempMB = new char[wcslen(Temp)+1];
				wcstombs(TempMB, Temp, wcslen(Temp));
				TempMB[wcslen(Temp)] = '\0';
				
				TCITEMA item;
				item.mask = Mask;
				item.iImage = Image;
				item.lParam = lParam;
				item.pszText = (LPTSTR)TempMB;

				Ret = InsertItemA(Index, &item);				
				
				delete [] TempMB;
				setlocale(LC_ALL, "English");
			}
		}
		delete [] Temp;
	}
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::InsertItem(int Index, char* Str, TTextEncoding TextEncoding)
{
	return InsertItem(TCIF_TEXT, Index, Str, 0, 0, TextEncoding);
}

//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::InsertItem(int Index, char* Str, int Image, TTextEncoding TextEncoding)
{
	return InsertItem(TCIF_TEXT|TCIF_IMAGE, Index, Str, Image, 0, TextEncoding);
}

//////////////////////////////////////////////////////////////////////////
bool CWinTabCtrl::DeleteItem(int Index)
{
	if(m_Hwnd) return ::SendMessage(m_Hwnd, TCM_DELETEITEM, Index, 0L) != FALSE;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinTabCtrl::DeleteAllItems()
{
	if(m_Hwnd) return ::SendMessage(m_Hwnd, TCM_DELETEALLITEMS, 0, 0L) != FALSE;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::GetCurSel()
{
	if(m_Hwnd) return (int)::SendMessage(m_Hwnd, TCM_GETCURSEL, 0, 0L);
	else return -1;
}

//////////////////////////////////////////////////////////////////////////
int CWinTabCtrl::SetCurSel(int Index)
{
	if(m_Hwnd) return (int)::SendMessage(m_Hwnd, TCM_SETCURSEL, Index, 0L);
	else return -1;
}

//////////////////////////////////////////////////////////////////////////
bool CWinTabCtrl::AdjustRect(bool Larger, LPRECT Rect)
{
	if(m_Hwnd)
	{
		::SendMessage(m_Hwnd, TCM_ADJUSTRECT, Larger, (LPARAM)Rect);
		return true;
	}
	else return false;
}
