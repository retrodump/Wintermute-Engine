// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BResourceFile.h"


//////////////////////////////////////////////////////////////////////////
CBResourceFile::CBResourceFile(CBGame* inGame):CBFile(inGame)
{
	m_Data = NULL;
}


//////////////////////////////////////////////////////////////////////////
CBResourceFile::~CBResourceFile()
{
	Close();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBResourceFile::Open(char* Filename)
{
	Close();

	// try game first
	if(SUCCEEDED(OpenModule(Filename, Game->m_ResourceModule))) return S_OK;

	// now search plugins
	for(int i=0; i<Game->m_PluginMgr->m_Plugins.GetSize(); i++)
	{
		CBPlugin* Plugin = Game->m_PluginMgr->m_Plugins[i];
		if(SUCCEEDED(OpenModule(Filename, Plugin->m_DllHandle))) return S_OK;
	}

	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBResourceFile::OpenModule(char* Filename, HMODULE Module)
{
	HRSRC res = FindResource(Module, Filename, "GF_RES");
	if(res!=NULL)
	{
		m_Size = SizeofResource(Module, res);

		m_Data = LoadResource(Module, res);
		if(m_Data != NULL)
		{
			m_Pos = 0;
			return S_OK;
		}
	}
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBResourceFile::Close()
{
	m_Data = NULL;
	m_Pos = 0;
	m_Size = 0;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBResourceFile::Read(void *Buffer, DWORD Size)
{
	if(!m_Data || m_Pos + Size > m_Size) return E_FAIL;

	memcpy(Buffer, (BYTE*)m_Data+m_Pos, Size);
	m_Pos+=Size;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBResourceFile::Seek(DWORD Pos, TSeek Origin)
{
	if(!m_Data) return E_FAIL;

	DWORD NewPos=0;

	switch(Origin){
		case SEEK_TO_BEGIN:   NewPos = Pos;          break;
		case SEEK_TO_END:     NewPos = m_Size + Pos; break;
		case SEEK_TO_CURRENT: NewPos = m_Pos + Pos;  break;
	}

	if(NewPos<0 || NewPos > m_Size) return E_FAIL;
	else m_Pos = NewPos;

	return S_OK;
}
