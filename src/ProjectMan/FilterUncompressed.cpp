// FilterUncompressed.cpp: implementation of the CFilterUncompressed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProjectMan.h"
#include "FilterUncompressed.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CFilterUncompressed::CFilterUncompressed(CProjectDoc* Doc):CPackagerFilter(Doc)
{
	m_Type = FILTER_UNCOMPRESS;
	DefaultSettings();
}


//////////////////////////////////////////////////////////////////////////
CFilterUncompressed::~CFilterUncompressed()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CFilterUncompressed::DefaultSettings()
{
	HRESULT ret = CPackagerFilter::DefaultSettings();
	if(FAILED(ret)) return ret;

	m_Active = true;

	m_Masks.Add("*.wav");
	m_Masks.Add("*.ogg");
	m_Masks.Add("*.ogv");

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
CString CFilterUncompressed::GetSection()
{
	return "Filter:Uncompressed";
}


//////////////////////////////////////////////////////////////////////////
CPackagerFilter::TProcessedType CFilterUncompressed::ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize)
{
	NewFilename = Filename;
	*NewSize = 0;
	*NewBuffer = NULL;
	return CPackagerFilter::PROC_USE_ORIGINAL_UNCOMPRESSED;
}