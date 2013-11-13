// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\btextutils.h"


//////////////////////////////////////////////////////////////////////////
CBTextUtils::CBTextUtils(void)
{
}


//////////////////////////////////////////////////////////////////////////
CBTextUtils::~CBTextUtils(void)
{
}


//////////////////////////////////////////////////////////////////////////
BYTE* CBTextUtils::ConvertToNative(TTextEncoding TextEncoding, BYTE* OrigText, int MaxLength)
{
	if(!OrigText) return NULL;

	if(TextEncoding==TEXT_ANSI)
	{
		int Length = strlen((char*)OrigText);
		if(MaxLength!=-1 && MaxLength < Length) Length = MaxLength;

		BYTE* Res = new BYTE[Length+1];
		strncpy((char*)Res, (char*)OrigText, Length);
		Res[Length] = '\0';

		return Res;
	}
	else if(TextEncoding==TEXT_UTF8)
	{
		int TempBufChars = (strlen((char*)OrigText)+1);
		wchar_t* Temp = new wchar_t[TempBufChars];

		if(!MultiByteToWideChar(CP_UTF8, 0, (char*)OrigText, -1, Temp, TempBufChars))
		{
			//Game->LOG(0, "Error converting string '%s' to unicode, error=%d", OrigText, ::GetLastError());
			delete [] Temp;
			return OrigText;
		}

		int Length = wcslen(Temp);
		if(MaxLength!=-1 && MaxLength < Length) Length = MaxLength;

		BYTE* Res = (BYTE*)new wchar_t[Length+1];
		memset(Res, 0, (Length+1)*sizeof(wchar_t));
		wcsncpy((wchar_t*)Res, Temp, Length);
		delete [] Temp;

		return Res;
	}
	else return OrigText;
}


//////////////////////////////////////////////////////////////////////////
void CBTextUtils::SetWindowText(TTextEncoding TextEncoding, HWND hWnd, BYTE* Text)
{
	if(!Text) return;

	if(TextEncoding==TEXT_ANSI)
	{
		::SetWindowText(hWnd, (char*)Text);
	}
	else if(TextEncoding==TEXT_UTF8)
	{
		int TempBufChars = (strlen((char*)Text)+1);
		wchar_t* Temp = new wchar_t[TempBufChars];

		if(MultiByteToWideChar(CP_UTF8, 0, (char*)Text, -1, Temp, TempBufChars))
		{
			if(CBUtils::IsWinNT())
			{
				// on NT use unicode functions
				::SetWindowTextW(hWnd, Temp);
			}
			else
			{
				// on Win9x convert to multibyte string using default codepage
				char* TempMB = new char[wcslen(Temp)+1];
				WideCharToMultiByte(CP_ACP, 0, Temp, wcslen(Temp), TempMB, wcslen(Temp)+1, NULL, NULL);
				TempMB[wcslen(Temp)] = '\0';
				::SetWindowText(hWnd, TempMB);
				delete [] TempMB;
			}
		}
		delete [] Temp;
	}
}

//////////////////////////////////////////////////////////////////////////
int CBTextUtils::GetTextLength(TTextEncoding TextEncoding, BYTE* Text)
{
	if(!Text) return 0;

	if(TextEncoding==TEXT_ANSI)
	{
		return strlen((char*)Text);
	}
	else if(TextEncoding==TEXT_UTF8)
	{
		int Ret = 0;

		int TempBufChars = (strlen((char*)Text)+1);
		wchar_t* Temp = new wchar_t[TempBufChars];
		if(MultiByteToWideChar(CP_UTF8, 0, (char*)Text, -1, Temp, TempBufChars))
		{
			Ret = wcslen(Temp);
		}
		delete [] Temp;

		return Ret;
	}
	else return 0;
}


//////////////////////////////////////////////////////////////////////////
wchar_t* CBTextUtils::Utf8ToUnicode(char* Text)
{
	int TempBufChars = (strlen((char*)Text)+1);
	wchar_t* Temp = new wchar_t[TempBufChars];
	if(MultiByteToWideChar(CP_UTF8, 0, (char*)Text, -1, Temp, TempBufChars)) return Temp;
	else
	{
		delete [] Temp;
		return NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
wchar_t* CBTextUtils::AnsiToUnicode(char* Text)
{
	int TempBufChars = (strlen((char*)Text)+1);
	wchar_t* Temp = new wchar_t[TempBufChars];
	if(MultiByteToWideChar(CP_ACP, 0, (char*)Text, -1, Temp, TempBufChars)) return Temp;
	else
	{
		delete [] Temp;
		return NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
char* CBTextUtils::AnsiToUtf8(char* Text)
{
	wchar_t* UnicodeString = AnsiToUnicode(Text);
	char* Ret = UnicodeToUtf8(UnicodeString);

	delete [] UnicodeString;
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
char* CBTextUtils::UnicodeToUtf8(wchar_t* Text)
{
	// utf8 encoded character can take up to 6 bytes
	int TempBufChars = (wcslen(Text)*6+1);
	char* Temp = new char[TempBufChars];

	if(WideCharToMultiByte(CP_UTF8, 0, Text, -1, Temp, TempBufChars, NULL, NULL)) return Temp;
	else
	{
		delete [] Temp;
		return NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
char* CBTextUtils::Utf8ToAnsi(char* Text)
{
	char* Ret = NULL;

	int TempBufChars = (strlen((char*)Text)+1);
	wchar_t* Temp = new wchar_t[TempBufChars];

	if(MultiByteToWideChar(CP_UTF8, 0, (char*)Text, -1, Temp, TempBufChars))
	{
		int AnsiBufChars = (wcslen(Temp)*6+1);
		Ret = new char[AnsiBufChars];

		WideCharToMultiByte(CP_ACP, 0, Temp, wcslen(Temp), Ret, AnsiBufChars, NULL, NULL);
		Ret[wcslen(Temp)] = '\0';
	}
	delete [] Temp;
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
wchar_t* CBTextUtils::NativeToUnicode(TTextEncoding TextEncoding, const char* OrigText)
{
	if(TextEncoding==TEXT_ANSI)
	{
		return AnsiToUnicode((char*)OrigText);
	}
	else if(TextEncoding==TEXT_UTF8)
	{
		return Utf8ToUnicode((char*)OrigText);
	}
	else return NULL;
}
