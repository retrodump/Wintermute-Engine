// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSaveThumbFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CBSaveThumbFile::CBSaveThumbFile(CBGame* inGame):CBFile(inGame)
{
	m_Data = NULL;
}


//////////////////////////////////////////////////////////////////////////
CBSaveThumbFile::~CBSaveThumbFile()
{
	Close();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSaveThumbFile::Open(char* Filename)
{
	Close();

	if(strnicmp(Filename, "savegame:", 9)!=0) return E_FAIL;

	char* TempFilename = new char[strlen(Filename)-8];
	strcpy(TempFilename, Filename+9);
	for(int i=0; i<strlen(TempFilename); i++){
		if(TempFilename[i] < '0' || TempFilename[i]>'9'){
			TempFilename[i] = '\0';
			break;
		}
	}

	// get slot number from name
	int Slot = atoi(TempFilename);
	delete [] TempFilename;
	
	char SlotFilename[MAX_PATH+1];
	Game->GetSaveSlotFilename(Slot, SlotFilename);
	CBPersistMgr* pm = new CBPersistMgr(Game);
	if(!pm) return E_FAIL;

	Game->m_DEBUG_AbsolutePathWarning = false;
	if(FAILED(pm->InitLoad(SlotFilename)))
	{
		Game->m_DEBUG_AbsolutePathWarning = true;
		delete pm;
		return E_FAIL;
	}
	Game->m_DEBUG_AbsolutePathWarning = true;

	HRESULT res;

	if(pm->m_ThumbnailDataSize!=0){
		m_Data = new BYTE[pm->m_ThumbnailDataSize];
		memcpy(m_Data, pm->m_ThumbnailData, pm->m_ThumbnailDataSize);
		m_Size = pm->m_ThumbnailDataSize;
		res = S_OK;
	}
	else res = E_FAIL;	
	delete pm;	

	return res;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSaveThumbFile::Close()
{
	SAFE_DELETE_ARRAY(m_Data);
	m_Pos = 0;
	m_Size = 0;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSaveThumbFile::Read(void *Buffer, DWORD Size)
{
	if(!m_Data || m_Pos + Size > m_Size) return E_FAIL;

	memcpy(Buffer, (BYTE*)m_Data+m_Pos, Size);
	m_Pos+=Size;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSaveThumbFile::Seek(DWORD Pos, TSeek Origin)
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
