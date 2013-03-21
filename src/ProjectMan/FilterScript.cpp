// FilterScript.cpp: implementation of the CFilterScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectMan.h"
#include "ProjectDoc.h"
#include "FilterScript.h"
#include "../MFCExt/utils_mfc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CFilterScript::CFilterScript(CProjectDoc* Doc):CPackagerFilter(Doc)
{
	m_Type = FILTER_SCRIPT;
	DefaultSettings();
	
	m_Game = ::new CAdGame;
	//m_Game->DEBUG_DebugEnable();
	m_Game->Initialize1();
	m_Game->m_ScEngine->SetCompileErrorCallback(ErrorCallback, this);

	m_CurrentFile = "";
}


//////////////////////////////////////////////////////////////////////////
CFilterScript::~CFilterScript()
{
	delete m_Game;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CFilterScript::DefaultSettings()
{
	HRESULT ret = CPackagerFilter::DefaultSettings();
	if(FAILED(ret)) return ret;

	m_Active = true;

	m_Masks.Add("*.dcs");
	m_Masks.Add("*.script");

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
CString CFilterScript::GetSection()
{
	return "Filter:Script";
}


//////////////////////////////////////////////////////////////////////////
bool CFilterScript::Initialize(CPackageBuilder::TPackage* Package)
{
	m_Document->PrioritizePackage(Package->Name);
	m_Game->m_Registry->SetIniName((char*)LPCSTR(m_Document->GetPathName()));
	m_Game->m_FileManager->SetBasePath((char*)LPCSTR(m_Document->m_ProjectRoot));
	m_Game->m_ScEngine->EmptyScriptCache();

	return true;
}


//////////////////////////////////////////////////////////////////////////
CPackagerFilter::TProcessedType CFilterScript::ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize)
{
	NewFilename = Filename;
	*NewSize = 0;
	*NewBuffer = NULL;

	m_CurrentFile = Filename;
	DWORD CompSize = 0;
	BYTE* CompBuffer = m_Game->m_ScEngine->GetCompiledScript((char*)LPCSTR(Filename), &CompSize, true);
	if(CompBuffer==NULL){
		return CPackagerFilter::PROC_ERROR;
	}
	else{
		/*
		*NewBuffer = new BYTE[CompSize+Size];
		memcpy(*NewBuffer, CompBuffer, CompSize);
		memcpy(*NewBuffer+CompSize, Buffer, Size);

		*NewSize = CompSize + Size;
		*/

		*NewBuffer = new BYTE[CompSize];
		memcpy(*NewBuffer, CompBuffer, CompSize);
		*NewSize = CompSize;

		return CPackagerFilter::PROC_USE_BUFFER;
	}
}


//////////////////////////////////////////////////////////////////////////
void CFilterScript::ErrorCallback(int Line, char *Text, void *Data)
{
	CFilterScript* _this = (CFilterScript*)Data;
	if(_this){
		if(_this->m_CurrentFile!="") _this->m_Document->AddInfo(CString(LOC("/str0100/Compiling file")) + " '" + _this->m_CurrentFile + "'...", _this->m_CurrentFile);
		CString Error;
		Error.Format(CString(LOC("/str0101/Line")) + " %d: %s", Line, Text);
		_this->m_Document->AddError(Error, _this->m_CurrentFile);
	}
}
