#include "stdafx.h"
#include "utils_mfc.h"



//////////////////////////////////////////////////////////////////////////
CString MakePath(CString Path){

	char drive[_MAX_DRIVE+1];
	char path[_MAX_PATH+1];
	int i;
	CString result;

	_splitpath(LPCSTR(Path), drive, path, NULL, NULL);
	Path = CString(drive) + "\\" + CString(path);

	if(Path[Path.GetLength()-1] != '\\') Path += '\\';


	for(i = 0; i<Path.GetLength(); i++)
		if(Path[i] == '/') Path.SetAt(i, '\\');

	for(i = 0; i<Path.GetLength(); i++)
		if(i == 0 || Path[i] != '\\' || (Path[i] == '\\' && Path[i-1] != '\\')){
			result += Path[i];
			if(Path[i] == '\\'){
				CreateDirectory(LPCSTR(result), NULL);
				DWORD attr = GetFileAttributes(LPCSTR(result));
				if(attr == 0xFFFFFFFF || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0) return "";
			}

		}

	return result;
}



//////////////////////////////////////////////////////////////////////////
int NumEntries(CString List, char Delimiter)
{
	int NumEntries=1;

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]==Delimiter) NumEntries++;
	}
			
	return NumEntries;
}


//////////////////////////////////////////////////////////////////////////
int NumEntriesEx(CString List, char Delimiter)
{
	int NumEntries=1;
	bool InQuote = false;

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]=='\"'){
			InQuote = !InQuote; 
			continue;
		}		
		if(List[i]==Delimiter && !InQuote) NumEntries++;
	}
			
	return NumEntries;
}


//////////////////////////////////////////////////////////////////////////
CString Entry(int Entry, CString List, char Delimiter)
{
	int CurrEntry=1;
	CString ret = "";

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]==Delimiter) CurrEntry++;
		else if(CurrEntry==Entry){
			ret += List[i];
		}
		if(CurrEntry>Entry) break;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
CString EntryEx(int Entry, CString List, char Delimiter)
{
	int CurrEntry=1;
	CString ret = "";
	bool InQuote = false;

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]=='\"'){
			InQuote = !InQuote; 
			continue;
		}
		if(List[i]==Delimiter && !InQuote) CurrEntry++;
		else if(CurrEntry==Entry){
			ret += List[i];
		}
		if(CurrEntry>Entry) break;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
CString GetExt(CString Filename)
{
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, NULL, ext);
	return CString(ext+1);
}


//////////////////////////////////////////////////////////////////////////
CString GetPath(CString Filename)
{
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	_splitpath(Filename, drive, path, NULL, NULL);
	
	CString ret;
	ret.Format("%s%s", drive, path);
	return ret;
}


//////////////////////////////////////////////////////////////////////////
CString GetFilename(CString Filename)
{
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, fname, ext);
	
	CString ret;
	ret.Format("%s%s", fname, ext);
	return ret;
}


//////////////////////////////////////////////////////////////////////////
CString GetFName(CString Filename)
{
	char fname[_MAX_FNAME];
	_splitpath(Filename, NULL, NULL, fname, NULL);
	
	return CString(fname);
}


//////////////////////////////////////////////////////////////////////////
CString GetAbsolutePath(CString Base, CString RelPath)
{
	Base.Replace("/", "\\");
	RelPath.Replace("/", "\\");

	Base.TrimRight("\\");
	RelPath.TrimRight("\\");

	int NumUp=0;
	for(int i=1; i<=NumEntries(RelPath, '\\'); i++){
		if(Entry(i, RelPath, '\\')=="..") NumUp++;
		else break;
	}

	if(NumUp>NumEntries(Base, '\\')) return RelPath;

	CString Result = "";
	if(NumUp>0){
		for(int i=1; i<=NumEntries(Base, '\\')-NumUp; i++){
			Result+=Entry(i, Base, '\\') + "\\";
		}
	}

	for(int i=NumUp+1; i<=NumEntries(RelPath, '\\'); i++){
		Result+=Entry(i, RelPath, '\\') + '\\';
	}

	return Result;	
}



//////////////////////////////////////////////////////////////////////////
int ComparePattern(const char* pattern, const char* string)
{
  char stringc,patternc;

  for (;; ++string)
  {
    stringc=toupper(*string);
    patternc=toupper(*pattern++);
    switch (patternc)
    {
      case 0:
        return(stringc==0);
      case '?':
        if (stringc == 0)
          return(FALSE);

        break;
      case '*':
        if (!*pattern)
          return(TRUE);

        if (*pattern=='.')
        {
          char *dot;
          if (pattern[1]=='*' && pattern[2]==0)
            return(TRUE);
          dot=(char*)strchr(string,'.');
          if (pattern[1]==0)
            return (dot==NULL || dot[1]==0);
          if (dot!=NULL)
          {
            string=dot;
            if (strpbrk(pattern,"*?[")==NULL && strchr(string+1,'.')==NULL)
              return(_stricmp(pattern+1,string+1)==0);
          }
        }

        while (*string)
          if (ComparePattern(pattern,string++))
            return(TRUE);
        return(FALSE);
      default:
        if (patternc != stringc)
          if (patternc=='.' && stringc==0)
            return(ComparePattern(pattern,string));
          else
            return(FALSE);
        break;
    }
  }
}


//////////////////////////////////////////////////////////////////////////
CString GetRegString(HKEY hKey, CString Path, CString Key, CString Init)
{
	HKEY key;
	DWORD type, size;
	LONG lr;

	size = MAX_PATH;
	char val[MAX_PATH];
	lr = RegOpenKeyEx(hKey, Path, 0, KEY_READ, &key);
	if(lr == ERROR_SUCCESS){
		lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)val, &size);
		RegCloseKey(key);
	}
	
	if(lr == ERROR_SUCCESS) return CString(val);
	else return Init;
}


//////////////////////////////////////////////////////////////////////////
int GetRegInt(HKEY hKey, CString Path, CString Key, int Init)
{
	HKEY key;
	DWORD type, size;
	LONG lr;

	size = sizeof(int);
	int val;
	lr = RegOpenKeyEx(hKey, Path, 0, KEY_READ, &key);
	if(lr == ERROR_SUCCESS){
		lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)&val, &size);
		RegCloseKey(key);
	}
	
	if(lr == ERROR_SUCCESS) return val;
	else return Init;
}


//////////////////////////////////////////////////////////////////////////
bool SetRegString(HKEY hKey, CString Path, CString Key, CString Value)
{
	HKEY key;

	LONG lr = RegCreateKeyEx(hKey, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegSetValueEx(key, Key, 0, REG_SZ, (BYTE*)LPCSTR(Value), Value.GetLength()+1);
		RegCloseKey(key);
	}

	return (lr==ERROR_SUCCESS);
}


//////////////////////////////////////////////////////////////////////////
bool SetRegInt(HKEY hKey, CString Path, CString Key, int Value)
{
	HKEY key;

	LONG lr = RegCreateKeyEx(hKey, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegSetValueEx(key, Key, 0, REG_DWORD, (BYTE*)&Value, sizeof(int));
		RegCloseKey(key);
	}

	return (lr==ERROR_SUCCESS);
}



//////////////////////////////////////////////////////////////////////////
bool DelRegKey(HKEY hKey, CString Path, CString Key)
{
	HKEY key;

	LONG lr = RegCreateKeyEx(hKey, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegDeleteValue(key, Key);
	}

	return (lr==ERROR_SUCCESS);
}
