// PackagerFilter.cpp: implementation of the CPackagerFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectMan.h"
#include "ProjectDoc.h"
#include "PackagerFilter.h"
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
CPackagerFilter::CPackagerFilter(CProjectDoc* Doc)
{
	m_Document = Doc;
	DefaultSettings();
	m_Type = FILTER_NONE;
}


//////////////////////////////////////////////////////////////////////////
CPackagerFilter::~CPackagerFilter()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CPackagerFilter::LoadSettings(CString Filename)
{
	CString section = GetSection();
	
	char str[MAX_PATH];
	int i;

	
	GetPrivateProfileString(section, "Mask", "***", str, MAX_PATH, Filename);
	if(stricmp(str, "***")!=0)
	{
		m_Masks.RemoveAll();
		int NumMasks = NumEntries(CString(str), ';');
		for(i=1; i<=NumMasks; i++){
			m_Masks.Add(Entry(i, CString(str), ';'));
		}
	}

	m_Active = GetPrivateProfileInt(section, "Active", 1, Filename)!=0;


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPackagerFilter::DefaultSettings()
{
	m_Active = true;
	m_Masks.RemoveAll();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CString CPackagerFilter::GetSection()
{
	return "Filter";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPackagerFilter::SaveSettings(FILE *File)
{
	if(!File) return E_FAIL;

	fprintf(File, "[%s]\n", GetSection());
	fprintf(File, "Active=%d\n", m_Active?1:0);
	fprintf(File, "Mask=");
	for(int i=0; i<m_Masks.GetSize(); i++){
		fprintf(File, m_Masks[i]);
		if(i<m_Masks.GetSize()-1) fprintf(File, ";");
	}
	fprintf(File, "\n");

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CPackagerFilter::TProcessedType CPackagerFilter::ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize)
{
	NewFilename = Filename;
	NewSize = 0;
	*NewBuffer = NULL;
	return CPackagerFilter::PROC_USE_ORIGINAL;
}


//////////////////////////////////////////////////////////////////////////
bool CPackagerFilter::FilenameMatches(CString Filename)
{
	for(int i=0; i<m_Masks.GetSize(); i++){
		if(ComparePattern(m_Masks[i], Filename)) return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CPackagerFilter::Initialize(CPackageBuilder::TPackage* Package)
{
	return true;
}
