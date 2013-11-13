// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\wincombo.h"


//////////////////////////////////////////////////////////////////////////
CWinCombo::CWinCombo(void) : CWinWindow()
{
}


//////////////////////////////////////////////////////////////////////////
CWinCombo::~CWinCombo(void)
{
}

//////////////////////////////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////////////////////////////
int CWinCombo::GetCount()
{
	if(m_Hwnd) return ComboBox_GetCount(m_Hwnd);
	else return 0;
}

//////////////////////////////////////////////////////////////////////////
bool CWinCombo::ResetContent()
{
	if(m_Hwnd)
	{
		ComboBox_ResetContent(m_Hwnd);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::AddString(char* Str, TTextEncoding TextEncoding)
{
	return ComboSendMessage(CB_ADDSTRING, Str, TextEncoding, 0L);
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::InsertString(int Index, char* Str, TTextEncoding TextEncoding)
{
	return ComboSendMessage(CB_INSERTSTRING, Str, TextEncoding, 0L);
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::DeleteString(int Index)
{
	if(m_Hwnd) return ComboBox_DeleteString(m_Hwnd, Index);
	else return CB_ERR;
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::SetItemData(int Index, void* Data)
{
	if(m_Hwnd) return ComboBox_SetItemData(m_Hwnd, Index, (LPARAM)Data);
	else return CB_ERR;
}

//////////////////////////////////////////////////////////////////////////
void* CWinCombo::GetItemData(int Index)
{
	if(m_Hwnd) return (void*)ComboBox_GetItemData(m_Hwnd, Index);
	else return (void*)CB_ERR;
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::FindString(char* Str, TTextEncoding TextEncoding, int StartIndex)
{
	return ComboSendMessage(CB_FINDSTRING, Str, TextEncoding, (WPARAM)StartIndex);
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::FindItemData(void* Data, int StartIndex)
{
	if(m_Hwnd) return ComboBox_FindItemData(m_Hwnd, StartIndex, (LPARAM)Data);
	else return CB_ERR;
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::GetCurSel()
{
	if(m_Hwnd) return ComboBox_GetCurSel(m_Hwnd);
	else return CB_ERR;
}

//////////////////////////////////////////////////////////////////////////
int CWinCombo::SetCurSel(int Index)
{
	if(m_Hwnd) return ComboBox_SetCurSel(m_Hwnd, Index);
	else return CB_ERR;
}


//////////////////////////////////////////////////////////////////////////
int CWinCombo::ComboSendMessage(int Msg, char* Str, TTextEncoding TextEncoding, WPARAM wParam)
{
	if(!m_Hwnd) return CB_ERR;

	int Ret = CB_ERR;

	if(TextEncoding==TEXT_ANSI)
	{
		Ret = ::SendMessageA(m_Hwnd, Msg, wParam, (LPARAM)Str);
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
				Ret = ::SendMessageW(m_Hwnd, Msg, wParam, (LPARAM)Temp);
			}
			else
			{
				// on Win9x convert to multibyte string using default codepage
				setlocale(LC_ALL, "");
				char* TempMB = new char[wcslen(Temp)+1];
				wcstombs(TempMB, Temp, wcslen(Temp));
				TempMB[wcslen(Temp)] = '\0';

				Ret = ::SendMessageA(m_Hwnd, Msg, wParam, (LPARAM)TempMB);
				delete [] TempMB;
				setlocale(LC_ALL, "English");
			}
		}
		delete [] Temp;
	}
	return Ret;
}