// settings.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "settings.h"

int GetArgCount(const char* CmdLine);
bool GetArg(int Num, const char* CmdLine, char* Buf);
bool GetExeName(char* Buf);
bool IsWmeExe(char* Filename);


//////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	char Command[32768];

	bool DetectFound = false;
	for(int i=0; i<GetArgCount(lpCmdLine); i++)
	{
		char Param[MAX_PATH];
		GetArg(i, lpCmdLine, Param);
		if(_stricmp(Param, "-detect")==0)
		{
			DetectFound = true;
			break;
		}
	}

	char ExeName[MAX_PATH];
	if(!GetExeName(ExeName)) return 1;

	sprintf(Command, "%s %s%s", ExeName, lpCmdLine, DetectFound?"":" -detect");
	WinExec(Command, nCmdShow);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
bool GetExeName(char* Buf)
{
	// find exe
	struct _finddata_t find_file;
	long hFile;

	if((hFile = _findfirst("*.exe", &find_file)) != -1L){
		if(!(find_file.attrib & _A_SUBDIR))
		{
			if(IsWmeExe(find_file.name))
			{
				strcpy(Buf, find_file.name);
				_findclose(hFile);
				return true;
			}
		}

		while(_findnext(hFile, &find_file) == 0)
		{
			if(!(find_file.attrib & _A_SUBDIR))
			{
				if(IsWmeExe(find_file.name))
				{
					strcpy(Buf, find_file.name);
					_findclose(hFile);
					return true;
				}
			}
		}
		_findclose(hFile);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool IsWmeExe(char* Filename)
{
	DWORD Handle;
	UINT  Size;
	Size = GetFileVersionInfoSize(Filename, &Handle);
	if (Size > 0)
	{
		BYTE* FileVersionBuffer = new BYTE[Size];
		if(FileVersionBuffer == NULL) return false;

		if(GetFileVersionInfo(Filename, 0, Size, FileVersionBuffer))
		{
			char* IntName=NULL;
			VerQueryValue(FileVersionBuffer, "\\StringFileInfo\\000004b0\\InternalName", (void**)&IntName, &Size);
			if(IntName!=NULL && _stricmp(IntName, "wme")==0)
			{
				delete[] FileVersionBuffer;
				return true;
			}
		}
		delete[] FileVersionBuffer;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
int GetArgCount(const char* CmdLine)
{
	int ret = 0;

	bool InQuote = false;
	for(int i=0; i<strlen(CmdLine); i++){
		if(CmdLine[i]=='\"'){
			InQuote = !InQuote;
			if(InQuote) ret++;
		}
		else{
			if(CmdLine[i]!=' ' && (i==0 || (CmdLine[i-1]==' ' && !InQuote))){
				ret++;
			}
		}
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////////
bool GetArg(int Num, const char* CmdLine, char* Buf)
{
	int CurrEntry=-1;
	int Pos = 0;
	bool InQuote = false;
	for(int i=0; i<strlen(CmdLine); i++){
		if(CmdLine[i]=='\"'){
			InQuote = !InQuote;
			if(InQuote) CurrEntry++;
		}
		else{
			if(CmdLine[i]!=' ' && (i==0 || (CmdLine[i-1]==' ' && !InQuote))){
				CurrEntry++;
			}

			if(CurrEntry==Num){
				if(CmdLine[i]!=' ' || InQuote){
					Buf[Pos] = CmdLine[i];
					Pos++;
				}
			}
		}
	}
	Buf[Pos] = '\0';
	return Pos>0;
}
