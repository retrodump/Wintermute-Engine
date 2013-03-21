// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "UIMarkup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_PERSISTENT(CUIMarkup, false);

//////////////////////////////////////////////////////////////////////////
CUIMarkup::CUIMarkup(CBGame* InGame):CUIObject(InGame)
{

}



//////////////////////////////////////////////////////////////////////////
CUIMarkup::~CUIMarkup()
{

}


//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CUIMarkup::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
/*
	//////////////////////////////////////////////////////////////////////////
	// SetDisabledFont
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SetDisabledFont")==0){
		Stack->CorrectParams(1);

		if(m_FontDisable) Game->m_FontStorage->RemoveFont(m_FontDisable);
		m_FontDisable = Game->m_FontStorage->AddFont(Stack->Pop()->GetString());
		Stack->PushBool(m_FontDisable!=NULL);

		return S_OK;
	}
	else*/ return CUIObject::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIMarkup::ScSetProperty(char *Name, CScValue *Value)
{
/*
	//////////////////////////////////////////////////////////////////////////
	// Focusable
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Focusable")==0){
		m_CanFocus = Value->GetBool();
		return S_OK;
	}
	else*/ return CUIObject::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CUIMarkup::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("html");
		return m_ScValue;
	}
	else return CUIObject::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
char* CUIMarkup::ScToString()
{
	return "[html]";
}



//////////////////////////////////////////////////////////////////////////
HRESULT CUIMarkup::Display(int OffsetX, int OffsetY)
{
	if(!m_Visible) return S_OK;

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CUIMarkup::LoadFile(char* Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CUIMarkup::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing HTML control file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (HTML)
	TOKEN_DEF (TEMPLATE)
	TOKEN_DEF (VISIBLE)
	TOKEN_DEF (X)
	TOKEN_DEF (Y)
	TOKEN_DEF (WIDTH)
	TOKEN_DEF (HEIGHT)
	TOKEN_DEF (NAME)
	TOKEN_DEF (SCRIPT)
	TOKEN_DEF (PARENT_NOTIFY)
	TOKEN_DEF (EDITOR_PROPERTY)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CUIMarkup::LoadBuffer(BYTE* Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (HTML)
		TOKEN_TABLE (TEMPLATE)
		TOKEN_TABLE (VISIBLE)
		TOKEN_TABLE (X)
		TOKEN_TABLE (Y)
		TOKEN_TABLE (WIDTH)
		TOKEN_TABLE (HEIGHT)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (SCRIPT)
		TOKEN_TABLE (PARENT_NOTIFY)
		TOKEN_TABLE (EDITOR_PROPERTY)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd=2;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_HTML){
			Game->LOG(0, "'HTML' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while (cmd>0 && (cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_TEMPLATE:
				if(FAILED(LoadFile((char*)params))) cmd = PARSERR_GENERIC;
			break;

			case TOKEN_NAME:
				SetName((char*)params);
			break;

			case TOKEN_X:
				parser.ScanStr((char*)params, "%d", &m_PosX);
			break;

			case TOKEN_Y:
				parser.ScanStr((char*)params, "%d", &m_PosY);
			break;

			case TOKEN_WIDTH:
				parser.ScanStr((char*)params, "%d", &m_Width);
			break;

			case TOKEN_HEIGHT:
				parser.ScanStr((char*)params, "%d", &m_Height);
			break;

			case TOKEN_SCRIPT:
				AddScript((char*)params);
			break;

			case TOKEN_PARENT_NOTIFY:
				parser.ScanStr((char*)params, "%b", &m_ParentNotify);
			break;

			case TOKEN_VISIBLE:
				parser.ScanStr((char*)params, "%b", &m_Visible);
			break;

			case TOKEN_EDITOR_PROPERTY:
				ParseEditorProperty(params, false);
			break;		
		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in HTML control definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC){
		Game->LOG(0, "Error loading HTML control definition");
		return E_FAIL;
	}

	return S_OK;
}
			

//////////////////////////////////////////////////////////////////////////
HRESULT CUIMarkup::Persist(CBPersistMgr* PersistMgr){

	CUIObject::Persist(PersistMgr);
/*
	PersistMgr->Transfer(TMEMBER_INT(m_Align));
	PersistMgr->Transfer(TMEMBER(m_BackDisable));
*/
	return S_OK;
}