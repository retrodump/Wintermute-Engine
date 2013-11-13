// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\xfile.h"
#include <rmxftmpl.h>


//////////////////////////////////////////////////////////////////////////
CXFile::CXFile(CBGame* inGame) : CBBase(inGame)
{
	m_Size = 0;
	m_Buffer = NULL;
	m_XFile = NULL;
	m_XEnum = NULL;
}


//////////////////////////////////////////////////////////////////////////
CXFile::~CXFile(void)
{
	CloseFile();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXFile::CloseFile()
{
	RELEASE(m_XEnum);
	RELEASE(m_XFile);

	SAFE_DELETE_ARRAY(m_Buffer);
	m_Size = 0;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFile::OpenFile(char* Filename)
{
	CloseFile();

	HRESULT res = S_OK;

	// load file
	m_Buffer = Game->m_FileManager->ReadWholeFile(Filename, &m_Size);
	if(!m_Buffer)
	{
		CloseFile();
		return E_FAIL;
	}

	// create XFile interface
#ifdef WME_D3D9
	res = D3DXFileCreate(&m_XFile);
#else
	res = Game->m_DirectX.DirectXFileCreate(&m_XFile);
#endif
	if(FAILED(res))
	{
		Game->LOG(res, "Error creating XFile for '%s'", Filename);
		CloseFile();
		return res;
	}

	// register templates for d3drm.
	res = m_XFile->RegisterTemplates((void*)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES);
	if(FAILED(res))
	{
		Game->LOG(res, "Error registering templates for '%s'", Filename);
		CloseFile();
		return res;
	}

	// create enum object
	DXFILELOADMEMORY MemFile;
	MemFile.lpMemory = m_Buffer;
	MemFile.dSize = m_Size;

	res = m_XFile->CreateEnumObject(&MemFile, DXFILELOAD_FROMMEMORY, &m_XEnum);
	if(FAILED(res))
	{
		Game->LOG(res, "Error creating XFile enum object for '%s'", Filename);
		CloseFile();
		return res;
	}

	return S_OK;
}