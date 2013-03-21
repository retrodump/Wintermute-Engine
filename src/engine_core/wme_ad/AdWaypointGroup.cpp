// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdWaypointGroup.h"


IMPLEMENT_PERSISTENT(CAdWaypointGroup, false);

//////////////////////////////////////////////////////////////////////////
CAdWaypointGroup::CAdWaypointGroup(CBGame* inGame):CBObject(inGame)
{
	m_Active = true;
	m_EditorSelectedPoint = -1;
	m_LastMimicScale = -1;
	m_LastMimicX = m_LastMimicY = INT_MIN;
}


//////////////////////////////////////////////////////////////////////////
CAdWaypointGroup::~CAdWaypointGroup()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void CAdWaypointGroup::Cleanup()
{
	for(int i=0; i<m_Points.GetSize(); i++)
		delete m_Points[i];
	m_Points.RemoveAll();
	m_EditorSelectedPoint = -1;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CAdWaypointGroup::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing WAYPOINTS file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (WAYPOINTS)
	TOKEN_DEF (TEMPLATE)
	TOKEN_DEF (NAME)
	TOKEN_DEF (POINT)
	TOKEN_DEF (EDITOR_SELECTED_POINT)
	TOKEN_DEF (EDITOR_SELECTED)
	TOKEN_DEF (PROPERTY)
	TOKEN_DEF (EDITOR_PROPERTY)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (WAYPOINTS)
		TOKEN_TABLE (TEMPLATE)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (POINT)
		TOKEN_TABLE (EDITOR_SELECTED_POINT)
		TOKEN_TABLE (EDITOR_SELECTED)
		TOKEN_TABLE (PROPERTY)
		TOKEN_TABLE (EDITOR_PROPERTY)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_WAYPOINTS){
			Game->LOG(0, "'WAYPOINTS' keyword expected.");
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

			case TOKEN_NAME:
				SetName((char*)params);
			break;

			case TOKEN_POINT:
			{
				int x,y;
				parser.ScanStr((char*)params, "%d,%d", &x, &y);
				m_Points.Add(new CBPoint(x, y));
			}
			break;

			case TOKEN_EDITOR_SELECTED:
				parser.ScanStr((char*)params, "%b", &m_EditorSelected);
			break;

			case TOKEN_EDITOR_SELECTED_POINT:
				parser.ScanStr((char*)params, "%d", &m_EditorSelectedPoint);
			break;

			case TOKEN_PROPERTY:
				ParseProperty(params, false);
			break;

			case TOKEN_EDITOR_PROPERTY:
				ParseEditorProperty(params, false);
			break;
		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in WAYPOINTS definition");
		return E_FAIL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup::SaveAsText(CBDynBuffer *Buffer, int Indent)
{
	Buffer->PutTextIndent(Indent, "WAYPOINTS {\n");
	Buffer->PutTextIndent(Indent+2, "NAME=\"%s\"\n", m_Name);
	Buffer->PutTextIndent(Indent+2, "EDITOR_SELECTED=%s\n", m_EditorSelected?"TRUE":"FALSE");
	Buffer->PutTextIndent(Indent+2, "EDITOR_SELECTED_POINT=%d\n", m_EditorSelectedPoint);

	if(m_ScProp) m_ScProp->SaveAsText(Buffer, Indent+2);
	CBBase::SaveAsText(Buffer, Indent+2);

	for(int i=0; i<m_Points.GetSize(); i++){
		Buffer->PutTextIndent(Indent+2, "POINT {%d,%d}\n", m_Points[i]->x, m_Points[i]->y);
	}

	Buffer->PutTextIndent(Indent, "}\n");

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup::Persist(CBPersistMgr* PersistMgr){

	CBObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Active));
	PersistMgr->Transfer(TMEMBER(m_EditorSelectedPoint));
	PersistMgr->Transfer(TMEMBER(m_LastMimicScale));
	PersistMgr->Transfer(TMEMBER(m_LastMimicX));
	PersistMgr->Transfer(TMEMBER(m_LastMimicY));
	m_Points.Persist(PersistMgr);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CScValue* CAdWaypointGroup::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("waypoint-group");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Active
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Active")==0){
		m_ScValue->SetBool(m_Active);
		return m_ScValue;
	}

	else return CBObject::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// Active
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Active")==0){
		m_Active = Value->GetBool();
		return S_OK;
	}

	else return CBObject::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup::Mimic(CAdWaypointGroup* Wpt, float Scale, int X, int Y)
{
	if(Scale==m_LastMimicScale && X==m_LastMimicX && Y==m_LastMimicY) return S_OK;

	Cleanup();

	for(int i=0; i<Wpt->m_Points.GetSize(); i++){
		int x, y;

		x = (int)((float)Wpt->m_Points[i]->x * Scale / 100.0f);
		y = (int)((float)Wpt->m_Points[i]->y * Scale / 100.0f);

		m_Points.Add(new CBPoint(x+X, y+Y));
	}

	m_LastMimicScale = Scale;
	m_LastMimicX = X;
	m_LastMimicY = Y;

	return S_OK;
}
