// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BFontStorage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_PERSISTENT(CBFontStorage, true);

//////////////////////////////////////////////////////////////////////////
CBFontStorage::CBFontStorage(CBGame* inGame):CBBase(inGame)
{

}


//////////////////////////////////////////////////////////////////////////
CBFontStorage::~CBFontStorage()
{
	Cleanup(true);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBFontStorage::Cleanup(bool Warn)
{
	int i;

	for(i=0; i<m_Fonts.GetSize(); i++)
	{
		if(Warn) Game->LOG(0, "Removing orphan font '%s'", m_Fonts[i]->m_Filename);
		delete m_Fonts[i];
	}
	m_Fonts.RemoveAll();


	// unregister all TTF fonts
	for(i=0; i<m_RegTTFonts.GetSize(); i++)
	{
		if(m_RegTTFonts[i])
		{
			CBPlatform::RemoveFontResource(m_RegTTFonts[i]);
			delete [] m_RegTTFonts[i];
		}
	}
	m_RegTTFonts.RemoveAll();


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBFontStorage::RegisterTTFont(char* FontPath)
{
	for(int i=0; i<m_RegTTFonts.GetSize(); i++)
	{
		if(stricmp(m_RegTTFonts[i], FontPath)==0) return S_OK;
	}
	
	char FullPath[MAX_PATH];
	Game->m_FileManager->GetFullPath(FontPath, FullPath);

	if(CBPlatform::AddFontResource(FullPath))
	{
		RegTTFontInternal(FontPath);
		return S_OK;
	}
	else
	{
		Game->LOG(0, "Error registering TrueType font '%s'", FullPath);
		// add it to the list anyway so that we're not getting millions of log errors
		RegTTFontInternal(FontPath);
		return E_FAIL;
	}
}

//////////////////////////////////////////////////////////////////////////
void CBFontStorage::RegTTFontInternal(char* fileName)
{
	char* regName = new char[strlen(fileName) + 1];
	strcpy(regName, fileName);
	m_RegTTFonts.Add(regName);
}

//////////////////////////////////////////////////////////////////////////
CBFont* CBFontStorage::AddFont(char *Filename)
{
	if(!Filename) return NULL;

	for(int i=0; i<m_Fonts.GetSize(); i++){
		if(_stricmp(m_Fonts[i]->m_Filename, Filename)==0){
			m_Fonts[i]->m_RefCount++;
			return m_Fonts[i];
		}
	}

	/*
	CBFont* font = new CBFont(Game);
	if(!font) return NULL;

	if(FAILED(font->LoadFile(Filename))){
		delete font;
		return NULL;
	}
	else {
		font->m_RefCount = 1;
		m_Fonts.Add(font);
		return font;
	}
	*/
	CBFont* font = CBFont::CreateFromFile(Game, Filename);
	if(font)
	{
		font->m_RefCount = 1;
		m_Fonts.Add(font);
	}
	return font;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBFontStorage::RemoveFont(CBFont *Font)
{
	if(!Font) return E_POINTER;

	for(int i=0; i<m_Fonts.GetSize(); i++){
		if(m_Fonts[i]==Font){
			m_Fonts[i]->m_RefCount--;
			if(m_Fonts[i]->m_RefCount <= 0){
				delete m_Fonts[i];
				m_Fonts.RemoveAt(i);
			}
			break;
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBFontStorage::Persist(CBPersistMgr* PersistMgr){

	if(!PersistMgr->m_Saving) Cleanup(false);

	PersistMgr->Transfer(TMEMBER(Game));
	m_Fonts.Persist(PersistMgr);

	return S_OK;
}
