// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BEventSet.h"


IMPLEMENT_PERSISTENT(CBEventSet, false);

//////////////////////////////////////////////////////////////////////////
CBEventSet::CBEventSet(CBGame* inGame):CBObject(inGame)
{

}


//////////////////////////////////////////////////////////////////////////
CBEventSet::~CBEventSet()
{
	for(int i=0; i<m_Events.GetSize(); i++)
		delete m_Events[i];
	m_Events.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBEventSet::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CBEventSet::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing EVENTS file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (EVENTS)
	TOKEN_DEF (EVENT)
	TOKEN_DEF (TEMPLATE)
	TOKEN_DEF (INIT)
	TOKEN_DEF (SHUTDOWN)
	TOKEN_DEF (LEFT_CLICK)
	TOKEN_DEF (RIGHT_CLICK)
	TOKEN_DEF (MIDDLE_CLICK)
	TOKEN_DEF (LEFT_RELEASE)
	TOKEN_DEF (RIGHT_RELEASE)
	TOKEN_DEF (MIDDLE_RELEASE)
	TOKEN_DEF (LEFT_DBLCLICK)
	TOKEN_DEF (PRESS)
	TOKEN_DEF (MOUSE_OVER)
	TOKEN_DEF (IDLE)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CBEventSet::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (EVENTS)
		TOKEN_TABLE (EVENT)
		TOKEN_TABLE (TEMPLATE)
		TOKEN_TABLE (INIT)
		TOKEN_TABLE (SHUTDOWN)
		TOKEN_TABLE (LEFT_CLICK)
		TOKEN_TABLE (RIGHT_CLICK)
		TOKEN_TABLE (MIDDLE_CLICK)
		TOKEN_TABLE (LEFT_RELEASE)
		TOKEN_TABLE (RIGHT_RELEASE)
		TOKEN_TABLE (MIDDLE_RELEASE)
		TOKEN_TABLE (LEFT_DBLCLICK)
		TOKEN_TABLE (PRESS)
		TOKEN_TABLE (MOUSE_OVER)
		TOKEN_TABLE (IDLE)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_EVENTS){
			Game->LOG(0, "'EVENTS' keyword expected.");
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

			case TOKEN_INIT:
				m_Events.Add(new CBEvent(Game, EVENT_INIT, (char*)params));
			break;

			case TOKEN_SHUTDOWN:
				m_Events.Add(new CBEvent(Game, EVENT_SHUTDOWN, (char*)params));
			break;

			case TOKEN_LEFT_CLICK:
				m_Events.Add(new CBEvent(Game, EVENT_LEFT_CLICK, (char*)params));
			break;

			case TOKEN_RIGHT_CLICK:
				m_Events.Add(new CBEvent(Game, EVENT_RIGHT_CLICK, (char*)params));
			break;

			case TOKEN_MIDDLE_CLICK:
				m_Events.Add(new CBEvent(Game, EVENT_MIDDLE_CLICK, (char*)params));
			break;

			case TOKEN_LEFT_RELEASE:
				m_Events.Add(new CBEvent(Game, EVENT_LEFT_RELEASE, (char*)params));
			break;

			case TOKEN_RIGHT_RELEASE:
				m_Events.Add(new CBEvent(Game, EVENT_RIGHT_RELEASE, (char*)params));
			break;

			case TOKEN_MIDDLE_RELEASE:
				m_Events.Add(new CBEvent(Game, EVENT_MIDDLE_RELEASE, (char*)params));
			break;

			case TOKEN_LEFT_DBLCLICK:
				m_Events.Add(new CBEvent(Game, EVENT_LEFT_DBLCLICK, (char*)params));
			break;

			case TOKEN_PRESS:
				m_Events.Add(new CBEvent(Game, EVENT_PRESS, (char*)params));
			break;

			case TOKEN_MOUSE_OVER:
				m_Events.Add(new CBEvent(Game, EVENT_MOUSE_OVER, (char*)params));
			break;
			
			case TOKEN_IDLE:
				m_Events.Add(new CBEvent(Game, EVENT_IDLE, (char*)params));
			break;

			case TOKEN_EVENT:
				{
					CBEvent* event = new CBEvent(Game);
					if(event && SUCCEEDED(event->LoadBuffer(params, false)))
						m_Events.Add(event);
					else{
						SAFE_DELETE(event);
					}
				}				
			break;
		}
	
	}
	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in EVENTS definition");
		return E_FAIL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBEventSet::ApplyEvent(TEventType Event, CBObject *Owner)
{
	HRESULT ret = E_FAIL;
	for(int i=0; i<m_Events.GetSize(); i++){
		if(m_Events[i]->m_Type==Event){
			Owner->AddScript(m_Events[i]->m_Script);
			ret = S_OK;
		}
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBEventSet::SaveAsText(CBDynBuffer *Buffer, int Indent)
{
	Buffer->PutTextIndent(Indent, "EVENTS {\n");
	
	for(int i=0; i<m_Events.GetSize(); i++){
		if(m_Events[i]->m_Type==EVENT_NONE && m_Events[i]->m_Name){
			Buffer->PutTextIndent(Indent+2, "EVENT{\n");
			Buffer->PutTextIndent(Indent+4, "NAME=\"%s\"\n", m_Events[i]->m_Name);
			Buffer->PutTextIndent(Indent+4, "SCRIPT=\"%s\"\n", m_Events[i]->m_Script);
			Buffer->PutTextIndent(Indent+2, "}\n");
		}
		else Buffer->PutTextIndent(Indent+2, "%s=\"%s\"\n",CBEvent::GetEventName(m_Events[i]->m_Type) , m_Events[i]->m_Script);
	}

	Buffer->PutTextIndent(Indent, "}\n\n");

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBEventSet::Persist(CBPersistMgr* PersistMgr){

	CBObject::Persist(PersistMgr);
	m_Events.Persist(PersistMgr);

	return S_OK;
}
