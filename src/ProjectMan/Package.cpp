// Package.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectMan.h"
#include "Package.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CPackage::CPackage()
{
	m_Folder = "";
	m_Description = "";
	m_CD = 0;
	m_Priority = 0;
	m_Primary = false;
}


//////////////////////////////////////////////////////////////////////////
CPackage::~CPackage()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CPackage::LoadSettings(CString Filename, int ID)
{
	CString section;
	section.Format("Package%d", ID);

	char str[MAX_PATH];

	GetPrivateProfileString(section, "Folder", "", str, MAX_PATH, Filename);
	if(strcmp(str, "")==0) return E_FAIL;
	m_Folder = CString(str);

	GetPrivateProfileString(section, "Description", "", str, 100, Filename);
	m_Description = CString(str);

	m_Primary = GetPrivateProfileInt(section, "Primary", 0, Filename)!=0;
	m_CD = GetPrivateProfileInt(section, "CD", 0, Filename);
	m_Priority = GetPrivateProfileInt(section, "Priority", 0, Filename);


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPackage::SaveSettings(FILE *File, int ID)
{
	if(!File) return E_FAIL;

	fprintf(File, "[Package%d]\n", ID);
	fprintf(File, "Folder=%s\n", m_Folder);
	fprintf(File, "Description=%s\n", m_Description);
	fprintf(File, "Primary=%d\n", m_Primary?1:0);
	fprintf(File, "CD=%d\n", m_CD);
	fprintf(File, "Priority=%d\n", m_Priority);

	return S_OK;
}
