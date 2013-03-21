// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdTalkNode.h"


IMPLEMENT_PERSISTENT(CAdTalkNode, false);

//////////////////////////////////////////////////////////////////////////
CAdTalkNode::CAdTalkNode(CBGame* inGame):CBBase(inGame)
{
	m_Sprite = NULL;
	m_SpriteFilename = NULL;
	m_SpriteSet = NULL;
	m_SpriteSetFilename = NULL;
	m_Comment = NULL;

	m_StartTime = m_EndTime = 0;
	m_PlayToEnd = false;
	m_PreCache = false;
}


//////////////////////////////////////////////////////////////////////////
CAdTalkNode::~CAdTalkNode()
{
	SAFE_DELETE_ARRAY(m_SpriteFilename);
	SAFE_DELETE(m_Sprite);
	SAFE_DELETE_ARRAY(m_SpriteSetFilename);
	SAFE_DELETE(m_SpriteSet);
	SAFE_DELETE_ARRAY(m_Comment);
}



TOKEN_DEF_START
	TOKEN_DEF (ACTION)
	TOKEN_DEF (SPRITESET_FILE)
	TOKEN_DEF (SPRITESET)
	TOKEN_DEF (SPRITE)
	TOKEN_DEF (START_TIME)
	TOKEN_DEF (END_TIME)
	TOKEN_DEF (COMMENT)
	TOKEN_DEF (PRECACHE)
	TOKEN_DEF (EDITOR_PROPERTY)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CAdTalkNode::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (ACTION)
		TOKEN_TABLE (SPRITESET_FILE)
		TOKEN_TABLE (SPRITESET)
		TOKEN_TABLE (SPRITE)
		TOKEN_TABLE (START_TIME)
		TOKEN_TABLE (END_TIME)
		TOKEN_TABLE (COMMENT)
		TOKEN_TABLE (PRECACHE)
		TOKEN_TABLE (EDITOR_PROPERTY)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_ACTION){
			Game->LOG(0, "'ACTION' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	m_EndTime = 0;
	m_PlayToEnd = false;
	m_PreCache = false;

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_SPRITE:
				CBUtils::SetString(&m_SpriteFilename, (char*)params);
			break;

			case TOKEN_SPRITESET_FILE:
				CBUtils::SetString(&m_SpriteSetFilename, (char*)params);
			break;

			case TOKEN_SPRITESET:
			{
				SAFE_DELETE(m_SpriteSet);
				m_SpriteSet = new CAdSpriteSet(Game);
				if(!m_SpriteSet || FAILED(m_SpriteSet->LoadBuffer(params, false)))
				{
					SAFE_DELETE(m_SpriteSet);
					cmd = PARSERR_GENERIC;
				}
			}				
			break;

			case TOKEN_START_TIME:
				parser.ScanStr((char*)params, "%d", &m_StartTime);
			break;

			case TOKEN_END_TIME:
				parser.ScanStr((char*)params, "%d", &m_EndTime);
			break;

			case TOKEN_PRECACHE:
				parser.ScanStr((char*)params, "%b", &m_PreCache);
			break;

			case TOKEN_COMMENT:
				if(Game->m_EditorMode) CBUtils::SetString(&m_Comment, (char*)params);
			break;

			case TOKEN_EDITOR_PROPERTY:
				ParseEditorProperty(params, false);
			break;		
		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in ACTION definition");
		return E_FAIL;
	}

	if (cmd == PARSERR_GENERIC){
		Game->LOG(0, "Error loading ACTION definition");
		return E_FAIL;
	}

	if(m_EndTime==0) m_PlayToEnd = true;
	else m_PlayToEnd = false;

	if(m_PreCache && m_SpriteFilename){
		SAFE_DELETE(m_Sprite);
		m_Sprite = new CBSprite(Game);
		if(!m_Sprite || FAILED(m_Sprite->LoadFile(m_SpriteFilename))) return E_FAIL;
	}

	if(m_PreCache && m_SpriteSetFilename){
		SAFE_DELETE(m_SpriteSet);
		m_SpriteSet = new CAdSpriteSet(Game);
		if(!m_SpriteSet || FAILED(m_SpriteSet->LoadFile(m_SpriteSetFilename))) return E_FAIL;
	}


	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CAdTalkNode::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Comment));
	PersistMgr->Transfer(TMEMBER(m_StartTime));
	PersistMgr->Transfer(TMEMBER(m_EndTime));
	PersistMgr->Transfer(TMEMBER(m_PlayToEnd));
	PersistMgr->Transfer(TMEMBER(m_Sprite));
	PersistMgr->Transfer(TMEMBER(m_SpriteFilename));
	PersistMgr->Transfer(TMEMBER(m_SpriteSet));
	PersistMgr->Transfer(TMEMBER(m_SpriteSetFilename));

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdTalkNode::SaveAsText(CBDynBuffer* Buffer, int Indent)
{
	Buffer->PutTextIndent(Indent, "ACTION {\n");
	if(m_Comment) Buffer->PutTextIndent(Indent+2, "COMMENT=\"%s\"\n", m_Comment);
	Buffer->PutTextIndent(Indent+2, "START_TIME=%d\n", m_StartTime);
	if(!m_PlayToEnd) Buffer->PutTextIndent(Indent+2, "END_TIME=%d\n", m_EndTime);
	if(m_SpriteFilename) Buffer->PutTextIndent(Indent+2, "SPRITE=\"%s\"\n", m_SpriteFilename);
	if(m_SpriteSetFilename) Buffer->PutTextIndent(Indent+2, "SPRITESET_FILE=\"%s\"\n", m_SpriteSetFilename);
	else if(m_SpriteSet) m_SpriteSet->SaveAsText(Buffer, Indent+2);
	if(m_PreCache) Buffer->PutTextIndent(Indent+2, "PRECACHE=\"%s\"\n", m_PreCache?"TRUE":"FALSE");

	CBBase::SaveAsText(Buffer, Indent+2);
	
	Buffer->PutTextIndent(Indent, "}\n");

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdTalkNode::LoadSprite()
{
	if(m_SpriteFilename && !m_Sprite){
		m_Sprite = new CBSprite(Game);
		if(!m_Sprite || FAILED(m_Sprite->LoadFile(m_SpriteFilename))){
			SAFE_DELETE(m_Sprite);
			return E_FAIL;
		}
		else return S_OK;
	}

	else if(m_SpriteSetFilename && !m_SpriteSet){
		m_SpriteSet = new CAdSpriteSet(Game);
		if(!m_SpriteSet || FAILED(m_SpriteSet->LoadFile(m_SpriteSetFilename))){
			SAFE_DELETE(m_SpriteSet);
			return E_FAIL;
		}
		else return S_OK;
	}

	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CAdTalkNode::IsInTimeInterval(DWORD Time, TDirection Dir)
{
	if(Time >= m_StartTime){
		if(m_PlayToEnd){
			if((m_SpriteFilename && m_Sprite==NULL) || (m_Sprite && m_Sprite->m_Finished==false)) return true;
			else if((m_SpriteSetFilename && m_SpriteSet==NULL) || (m_SpriteSet && m_SpriteSet->GetSprite(Dir) && m_SpriteSet->GetSprite(Dir)->m_Finished==false)) return true;
			else return false;
		}
		else return m_EndTime >= Time;
	}
	else return false;
}


//////////////////////////////////////////////////////////////////////////
CBSprite* CAdTalkNode::GetSprite(TDirection Dir)
{
	LoadSprite();
	if(m_Sprite) return m_Sprite;
	else if(m_SpriteSet) return m_SpriteSet->GetSprite(Dir);
	else return NULL;
}
