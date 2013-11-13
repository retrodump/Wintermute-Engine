// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "SXDirectory.h"
#include "SXArray.h"
#include <io.h>
#include <direct.h>


IMPLEMENT_PERSISTENT(CSXDirectory, true);

//////////////////////////////////////////////////////////////////////////
CSXDirectory::CSXDirectory(CBGame* inGame):CBScriptable(inGame)
{
}


//////////////////////////////////////////////////////////////////////////
CSXDirectory::~CSXDirectory()
{
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXDirectory::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// Create
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Create")==0){
		Stack->CorrectParams(1);
		char* Path = Stack->Pop()->GetString();
		CBUtils::CreatePath(Path, true);
		Stack->PushBool(mkdir(Path)==0);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Delete
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Delete")==0){
		Stack->CorrectParams(1);
		char* Path = Stack->Pop()->GetString();
		Stack->PushBool(rmdir(Path)==0);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetFiles / GetDirectories
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetFiles")==0 || strcmp(Name, "GetDirectories")==0){
		Stack->CorrectParams(2);
		char* Path = Stack->Pop()->GetString();
		CScValue* Val = Stack->Pop();

		char DefaultMask[] = "*.*";
		char* Mask;
		if(!Val->IsNULL()) Mask = Val->GetString();
		else Mask = &DefaultMask[0];

		char FinalPath[MAX_PATH];
		strcpy(FinalPath, Path);
		if(FinalPath[strlen(FinalPath)]!='\\') strcat(FinalPath, "\\");
		strcat(FinalPath, Mask);

		CSXArray* Array = new CSXArray(Game);
		if(!Array)
		{
			Stack->PushNULL();
			return S_OK;
		}

		struct _finddata_t find_file;
		long hFile;

		if((hFile = _findfirst(FinalPath, &find_file )) != -1L){
			if(!(find_file.attrib & _A_SUBDIR) && strcmp(Name, "GetFiles")==0)
			{
				// file
				Val = new CScValue(Game, find_file.name);
				Array->Push(Val);
			}
			else if(strcmp(Name, "GetDirectories")==0 && find_file.name[0]!='.')
			{
				// dir
				Val = new CScValue(Game, find_file.name);
				Array->Push(Val);
			}

			while(_findnext(hFile, &find_file) == 0)
			{
				if(!(find_file.attrib & _A_SUBDIR) && strcmp(Name, "GetFiles")==0)
				{
					// file
					Val = new CScValue(Game, find_file.name);
					Array->Push(Val);
				}
				else if(strcmp(Name, "GetDirectories")==0 && find_file.name[0]!='.')
				{
					// dir
					Val = new CScValue(Game, find_file.name);
					Array->Push(Val);
				}
			}
			_findclose( hFile );
		}

		Stack->PushNative(Array, false);
		return S_OK;
	}

#ifndef _WIN32_WINCE
	//////////////////////////////////////////////////////////////////////////
	// GetDrives
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetDrives")==0){
		Stack->CorrectParams(0);

		CSXArray* Array = new CSXArray(Game);
		if(!Array)
		{
			Stack->PushNULL();
			return S_OK;
		}

		char Drive[4];
		for (char ch = 'A'; ch <= 'Z'; ch++){
			sprintf(Drive, "%c:\\", ch);
			if(GetDriveType(Drive) != DRIVE_NO_ROOT_DIR){				
				CScValue* Val = new CScValue(Game, Drive);
				Array->Push(Val);
			}
		}

		Stack->PushNative(Array, false);
		return S_OK;
	}
#endif

	else return CBScriptable::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CSXDirectory::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type (RO)
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("directory");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// PathSeparator (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PathSeparator")==0){
		m_ScValue->SetString("\\");
		return m_ScValue;
	}

#ifndef _WIN32_WINCE
	//////////////////////////////////////////////////////////////////////////
	// CurrentDirectory
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CurrentDirectory")==0){
		char Dir[MAX_PATH];
		if(GetCurrentDirectory(MAX_PATH, Dir)) m_ScValue->SetString(Dir);
		else m_ScValue->SetNULL();
		return m_ScValue;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// TempDirectory (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TempDirectory")==0){
		char Dir[MAX_PATH];

#ifdef _UNICODE
		wchar_t DirW[MAX_PATH];
		BOOL b = GetTempPath(MAX_PATH, DirW);
		WideCharToMultiByte(CP_ACP, 0, DirW, -1, Dir, MAX_PATH, NULL, NULL);
#else
		BOOL b = GetTempPath(MAX_PATH, Dir);
#endif

		if(b) m_ScValue->SetString(Dir);
		else m_ScValue->SetNULL();
		return m_ScValue;
	}

	else return CBScriptable::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXDirectory::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// CurrentDirectory
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "CurrentDirectory")==0){
		SetCurrentDirectory(Value->GetString());
		return S_OK;
	}
	else return CBScriptable::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXDirectory::Persist(CBPersistMgr* PersistMgr){

	CBScriptable::Persist(PersistMgr);
	return S_OK;
}
