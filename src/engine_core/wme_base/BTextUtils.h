// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

class CBTextUtils
{
public:
	CBTextUtils(void);
	virtual ~CBTextUtils(void);

	static BYTE* ConvertToNative(TTextEncoding TextEncoding, BYTE* OrigText, int MaxLength=-1);
	static void SetWindowText(TTextEncoding TextEncoding, HWND hWnd, BYTE* Text);
	static int GetTextLength(TTextEncoding TextEncoding, BYTE* Text);
	static wchar_t* Utf8ToUnicode(char* Text);
	static wchar_t* AnsiToUnicode(char* Text);
	static char* AnsiToUtf8(char* Text);
	static char* UnicodeToUtf8(wchar_t* Text);
	static char* Utf8ToAnsi(char* Text);
	static wchar_t* NativeToUnicode(TTextEncoding TextEncoding, const char* OrigText);
};
