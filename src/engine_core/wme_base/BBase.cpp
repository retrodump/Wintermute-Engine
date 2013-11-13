// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BBase.h"
#include "BParser.h"


//////////////////////////////////////////////////////////////////////
CBBase::CBBase(CBGame* GameOwner)
{
	Game = GameOwner;
	m_Persistable = true;
}


//////////////////////////////////////////////////////////////////////////
CBBase::CBBase()
{
	Game = NULL;
	m_Persistable = true;
}


//////////////////////////////////////////////////////////////////////
CBBase::~CBBase()
{	
	m_EditorProps.clear();
}


//////////////////////////////////////////////////////////////////////////
const char* CBBase::GetEditorProp(const char *PropName, const char *InitVal)
{
	m_EditorPropsIter = m_EditorProps.find(PropName);
	if (m_EditorPropsIter != m_EditorProps.end()) return m_EditorPropsIter->second.c_str();
	else return InitVal;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBBase::SetEditorProp(const char *PropName, const char *PropValue)
{
	if(PropName==NULL) return E_FAIL;

	if(PropValue==NULL){
		m_EditorProps.erase(PropName);
	}
	else{
		m_EditorProps[PropName] = PropValue;
	}
	return S_OK;
}



TOKEN_DEF_START
	TOKEN_DEF (EDITOR_PROPERTY)
	TOKEN_DEF (NAME)
	TOKEN_DEF (VALUE)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CBBase::ParseEditorProperty(BYTE* Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (EDITOR_PROPERTY)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (VALUE)
	TOKEN_TABLE_END


	if(!Game->m_EditorMode) return S_OK;
	

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_EDITOR_PROPERTY){
			Game->LOG(0, "'EDITOR_PROPERTY' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	char* PropName=NULL;
	char* PropValue=NULL;

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_NAME:
				SAFE_DELETE_ARRAY(PropName);
				PropName = new char[strlen((char*)params)+1];
				if(PropName) strcpy(PropName, (char*)params);
				else cmd = PARSERR_GENERIC;
			break;

			case TOKEN_VALUE:
				SAFE_DELETE_ARRAY(PropValue);
				PropValue = new char[strlen((char*)params)+1];
				if(PropValue) strcpy(PropValue, (char*)params);
				else cmd = PARSERR_GENERIC;
			break;
		}

	}
	if (cmd == PARSERR_TOKENNOTFOUND){
		SAFE_DELETE_ARRAY(PropName);
		SAFE_DELETE_ARRAY(PropValue);
		Game->LOG(0, "Syntax error in EDITOR_PROPERTY definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC || PropName==NULL || PropValue==NULL){
		SAFE_DELETE_ARRAY(PropName);
		SAFE_DELETE_ARRAY(PropValue);
		Game->LOG(0, "Error loading EDITOR_PROPERTY definition");
		return E_FAIL;
	}


	SetEditorProp(PropName, PropValue);
	
	SAFE_DELETE_ARRAY(PropName);
	SAFE_DELETE_ARRAY(PropValue);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBBase::SaveAsText(CBDynBuffer* Buffer, int Indent)
{
	m_EditorPropsIter = m_EditorProps.begin();
	while(m_EditorPropsIter!=m_EditorProps.end()){
		Buffer->PutTextIndent(Indent, "EDITOR_PROPERTY\n");
		Buffer->PutTextIndent(Indent, "{\n");
		Buffer->PutTextIndent(Indent+2, "NAME=\"%s\"\n", (char*)m_EditorPropsIter->first.c_str());
		Buffer->PutTextIndent(Indent+2, "VALUE=\"%s\"\n", m_EditorPropsIter->second.c_str());
		Buffer->PutTextIndent(Indent, "}\n\n");
		
		m_EditorPropsIter++;
	}
	return S_OK;
}
