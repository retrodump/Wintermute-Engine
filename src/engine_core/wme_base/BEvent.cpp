// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_PERSISTENT(CBEvent, false);

//////////////////////////////////////////////////////////////////////////
CBEvent::CBEvent(CBGame* inGame):CBBase(inGame)
{
	m_Type = EVENT_NONE;
	m_Script = NULL;
	m_Name = NULL;
}


//////////////////////////////////////////////////////////////////////////
CBEvent::CBEvent(CBGame* inGame, TEventType Type, char* Script):CBBase(inGame)
{
	m_Type = Type;
	m_Script = new char [strlen(Script)+1];
	if(m_Script) strcpy(m_Script, Script);
	m_Name = NULL;
}


//////////////////////////////////////////////////////////////////////////
CBEvent::~CBEvent()
{
	SAFE_DELETE_ARRAY(m_Script);
	SAFE_DELETE_ARRAY(m_Name);
}


//////////////////////////////////////////////////////////////////////////
const char* CBEvent::GetEventName(TEventType Type)
{	
	switch(Type){
		case EVENT_INIT:			return "INIT";
		case EVENT_SHUTDOWN:		return "SHUTDOWN";
		case EVENT_LEFT_CLICK:		return "LEFT_CLICK";
		case EVENT_RIGHT_CLICK:		return "RIGHT_CLICK";
		case EVENT_MIDDLE_CLICK:	return "MIDDLE_CLICK";
		case EVENT_LEFT_DBLCLICK:	return "LEFT_DBLCLICK";
		case EVENT_PRESS:			return "PRESS";
		case EVENT_IDLE:			return "IDLE";
		case EVENT_MOUSE_OVER:		return "MOUSE_OVER";
		case EVENT_LEFT_RELEASE:	return "LEFT_RELEASE";
		case EVENT_RIGHT_RELEASE:	return "RIGHT_RELEASE";
		case EVENT_MIDDLE_RELEASE:	return "MIDDLE_RELEASE";

		default:					return "NONE";
	}
}


//////////////////////////////////////////////////////////////////////////
void CBEvent::SetScript(char *Script)
{
	if(m_Script) delete [] m_Script;

	m_Script = new char [strlen(Script)+1];
	if(m_Script) strcpy(m_Script, Script);
}


//////////////////////////////////////////////////////////////////////////
void CBEvent::SetName(char *Name)
{
	if(m_Name) delete [] m_Name;

	m_Name = new char [strlen(Name)+1];
	if(m_Name) strcpy(m_Name, Name);
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBEvent::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CBEvent::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing EVENT file '%s'", Filename);

	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (EVENT)
	TOKEN_DEF (NAME)
	TOKEN_DEF (SCRIPT)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CBEvent::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (EVENT)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (SCRIPT)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_EVENT){
			Game->LOG(0, "'EVENT' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_NAME:
				SetName((char*)params);
			break;

			case TOKEN_SCRIPT:
				SetScript((char*)params);
			break;
		}
	
	}
	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in EVENT definition");
		return E_FAIL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBEvent::Persist(CBPersistMgr* PersistMgr){

	PersistMgr->Transfer(TMEMBER(Game));

	PersistMgr->Transfer(TMEMBER(m_Script));
	PersistMgr->Transfer(TMEMBER(m_Name));
	PersistMgr->Transfer(TMEMBER_INT(m_Type));

	return S_OK;
}