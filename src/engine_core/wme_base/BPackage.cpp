// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BPackage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CBPackage::CBPackage(CBGame* inGame):CBBase(inGame)
{
	m_File = NULL;
	m_Name = NULL;
	m_CD = 0;
	m_Priority = 0;
	m_BoundToExe = false;
}


//////////////////////////////////////////////////////////////////////////
CBPackage::~CBPackage()
{
	if(m_Name) delete [] m_Name;
	if(m_File) fclose(m_File);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPackage::Open()
{
	if(m_File) return S_OK;
	else{
		if(m_BoundToExe){
			char Filename[MAX_PATH];
			GetModuleFileName(NULL, Filename, MAX_PATH);
			m_File = Game->m_FileManager->OpenSingleFile(Filename);
		}
		else{
			m_File = Game->m_FileManager->OpenPackage(m_Name);
			if(!m_File){
				Game->m_FileManager->RequestCD(m_CD, m_Name, "");
				m_File = Game->m_FileManager->OpenPackage(m_Name);
			}
		}
		return m_File?S_OK:E_FAIL;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPackage::Close()
{
	if(m_File) fclose(m_File);
	m_File = NULL;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPackage::Read(DWORD Offset, BYTE *Buffer, DWORD Size)
{
	HRESULT ret;
	if(FAILED(ret=Open())) return ret;
	else{
		if(fseek(m_File, Offset, SEEK_SET)) return E_FAIL;
		if(fread(Buffer, Size, 1, m_File)!=1) return E_FAIL;
		else return S_OK;
	}
}
