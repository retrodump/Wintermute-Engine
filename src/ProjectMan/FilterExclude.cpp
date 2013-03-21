// FilterExclude.cpp: implementation of the CFilterExclude class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectMan.h"
#include "FilterExclude.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CFilterExclude::CFilterExclude(CProjectDoc* Doc):CPackagerFilter(Doc)
{
	m_Type = FILTER_EXCLUDE;
	DefaultSettings();
}


//////////////////////////////////////////////////////////////////////////
CFilterExclude::~CFilterExclude()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CFilterExclude::DefaultSettings()
{
	HRESULT ret = CPackagerFilter::DefaultSettings();
	if(FAILED(ret)) return ret;

	m_Active = true;

	m_Masks.Add("*.tmp");
	m_Masks.Add("*.bak");
	m_Masks.Add("*.inc");

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
CString CFilterExclude::GetSection()
{
	return "Filter:Exclude";
}


//////////////////////////////////////////////////////////////////////////
CPackagerFilter::TProcessedType CFilterExclude::ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize)
{
	NewFilename = Filename;
	*NewSize = 0;
	*NewBuffer = NULL;
	return CPackagerFilter::PROC_IGNORE;
}