// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BRegistry.h"


//////////////////////////////////////////////////////////////////////////
CBRegistry::CBRegistry(CBGame* inGame):CBBase(inGame)
{
	m_BasePath = NULL;
	m_IniName = NULL;
#ifdef _DEBUG
	SetBasePath("Software\\DEAD:CODE\\Games Framework\\Debug");
#else
	SetBasePath("Software\\DEAD:CODE\\Games Framework\\Retail");
#endif

	SetIniName(".\\wme.ini");
}


//////////////////////////////////////////////////////////////////////////
CBRegistry::~CBRegistry()
{
	SAFE_DELETE_ARRAY(m_BasePath);
	SAFE_DELETE_ARRAY(m_IniName);
}


//////////////////////////////////////////////////////////////////////////
void CBRegistry::SetBasePath(char *BasePath)
{
	if(m_BasePath) delete [] m_BasePath;
	m_BasePath = new char [strlen(BasePath)+1];
	strcpy(m_BasePath, BasePath);

	int i = strlen(m_BasePath) - 1;

	while(i>=0 && m_BasePath[i]=='\\'){
		m_BasePath[i--]='\0';
	}
}

//////////////////////////////////////////////////////////////////////////
const char* CBRegistry::GetBasePath()
{
	return m_BasePath;
}

//////////////////////////////////////////////////////////////////////////
bool CBRegistry::ReadString(char *Subkey, char *Key, char *Buffer, DWORD *BufferSize, char* Init)
{
	char* Path = new char[strlen(m_BasePath) + strlen(Subkey) + 2];
	strcpy(Path, m_BasePath);
	if(Subkey && strcmp(Subkey, "")){
		strcat(Path, "\\");
		strcat(Path, Subkey);
	}

	HKEY key;
	DWORD type;

	// check ini file first
	GetPrivateProfileString(Subkey, Key, Init?Init:"", Buffer, *BufferSize, m_IniName);
	if((Init&&!strcmp(Buffer, Init)) || (!Init&&!strcmp(Buffer, ""))){
		// current user
		LONG lr = RegOpenKeyEx(HKEY_CURRENT_USER, Path, 0, KEY_READ, &key);
		if(lr == ERROR_SUCCESS){
			lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)Buffer, BufferSize);
			RegCloseKey(key);
		}
		// local machine
		if(lr != ERROR_SUCCESS){
			lr = RegOpenKeyEx(HKEY_LOCAL_MACHINE, Path, 0, KEY_READ, &key);
			if(lr == ERROR_SUCCESS){
				lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)Buffer, BufferSize);
				RegCloseKey(key);
			}
		}

		if(lr != ERROR_SUCCESS){
			strncpy(Buffer, Init?Init:"", *BufferSize);
		}
	}
	delete [] Path;
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CBRegistry::WriteString(char *Subkey, char *Key, char *String)
{
	char* Path = new char[strlen(m_BasePath) + strlen(Subkey) + 2];
	strcpy(Path, m_BasePath);
	if(Subkey && strcmp(Subkey, "")){
		strcat(Path, "\\");
		strcat(Path, Subkey);
	}

	HKEY key;

	LONG lr = RegCreateKeyEx(HKEY_CURRENT_USER, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegSetValueEx(key, Key, 0, REG_SZ, (BYTE*)String, strlen(String)+1);
		RegCloseKey(key);
	}

	delete [] Path;

	return (lr==ERROR_SUCCESS);
}


//////////////////////////////////////////////////////////////////////////
int CBRegistry::ReadInt(char *Subkey, char *Key, int Init)
{
	char* Path = new char[strlen(m_BasePath) + strlen(Subkey) + 2];
	strcpy(Path, m_BasePath);
	if(Subkey && strcmp(Subkey, "")){
		strcat(Path, "\\");
		strcat(Path, Subkey);
	}

	HKEY key;
	DWORD type;
	DWORD size = sizeof(int);
	int ret;

	ret = GetPrivateProfileInt(Subkey, Key, Init, m_IniName);
	if(ret==Init){
		LONG lr = RegOpenKeyEx(HKEY_CURRENT_USER, Path, 0, KEY_READ, &key);
		if(lr == ERROR_SUCCESS){
			lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)&ret, &size);
			RegCloseKey(key);
		}
		if(lr != ERROR_SUCCESS){
			lr = RegOpenKeyEx(HKEY_LOCAL_MACHINE, Path, 0, KEY_READ, &key);
			if(lr == ERROR_SUCCESS){
				lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)&ret, &size);
				RegCloseKey(key);
			}
		}
	
		if(lr != ERROR_SUCCESS) ret = Init;
	}
	
	delete [] Path;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
bool CBRegistry::WriteInt(char *Subkey, char *Key, int Value)
{
	char* Path = new char[strlen(m_BasePath) + strlen(Subkey) + 2];
	strcpy(Path, m_BasePath);
	if(Subkey && strcmp(Subkey, "")){
		strcat(Path, "\\");
		strcat(Path, Subkey);
	}

	HKEY key;

	LONG lr = RegCreateKeyEx(HKEY_CURRENT_USER, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegSetValueEx(key, Key, 0, REG_DWORD, (BYTE*)&Value, sizeof(int));
		RegCloseKey(key);
	}

	delete [] Path;

	return (lr==ERROR_SUCCESS);
}


//////////////////////////////////////////////////////////////////////////
bool CBRegistry::ReadBool(char *Subkey, char *Key, bool Init)
{
	return (ReadInt(Subkey, Key, (int)Init) != 0);
}


//////////////////////////////////////////////////////////////////////////
bool CBRegistry::WriteBool(char *Subkey, char *Key, bool Value)
{
	return WriteInt(Subkey, Key, (int)Value);
}


//////////////////////////////////////////////////////////////////////////
void CBRegistry::SetIniName(char *Name)
{
	SAFE_DELETE_ARRAY(m_IniName);

	if(strchr(Name, '\\')==NULL && strchr(Name, '/')==NULL){
		m_IniName = new char [strlen(Name)+3];
		sprintf(m_IniName, ".\\%s", Name);
	}
	else{
		m_IniName = new char [strlen(Name)+1];
		strcpy(m_IniName, Name);
	}
}


//////////////////////////////////////////////////////////////////////////
char* CBRegistry::GetIniName()
{
	return m_IniName;
}