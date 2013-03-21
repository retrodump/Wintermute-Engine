// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdScaleLevel.h"


IMPLEMENT_PERSISTENT(CAdScaleLevel, false);

//////////////////////////////////////////////////////////////////////////
CAdScaleLevel::CAdScaleLevel(CBGame* inGame):CBObject(inGame)
{
	m_PosY = 0;
	m_Scale = 100;
}


//////////////////////////////////////////////////////////////////////////
CAdScaleLevel::~CAdScaleLevel()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdScaleLevel::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CAdScaleLevel::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing SCALE_LEVEL file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (SCALE_LEVEL)
	TOKEN_DEF (TEMPLATE)
	TOKEN_DEF (Y)
	TOKEN_DEF (SCALE)
	TOKEN_DEF (EDITOR_PROPERTY)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CAdScaleLevel::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (SCALE_LEVEL)
		TOKEN_TABLE (TEMPLATE)
		TOKEN_TABLE (Y)
		TOKEN_TABLE (SCALE)
		TOKEN_TABLE (EDITOR_PROPERTY)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_SCALE_LEVEL){
			Game->LOG(0, "'SCALE_LEVEL' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_TEMPLATE:
				if(FAILED(LoadFile((char*)params))) cmd = PARSERR_GENERIC;
			break;

			case TOKEN_Y:
				parser.ScanStr((char*)params, "%d", &m_PosY);
			break;

			case TOKEN_SCALE:
			{
				int i;
				parser.ScanStr((char*)params, "%d", &i);
				m_Scale = (float)i;
			}
			break;

			case TOKEN_EDITOR_PROPERTY:
				ParseEditorProperty(params, false);
			break;		
		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in SCALE_LEVEL definition");
		return E_FAIL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdScaleLevel::SaveAsText(CBDynBuffer *Buffer, int Indent)
{
	Buffer->PutTextIndent(Indent, "SCALE_LEVEL {\n");
	Buffer->PutTextIndent(Indent+2, "Y=%d\n", m_PosY);
	Buffer->PutTextIndent(Indent+2, "SCALE=%d\n", (int)m_Scale);
	CBBase::SaveAsText(Buffer, Indent+2);
	Buffer->PutTextIndent(Indent, "}\n");

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdScaleLevel::Persist(CBPersistMgr* PersistMgr){

	CBObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Scale));

	return S_OK;
}