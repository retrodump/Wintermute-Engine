// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdSceneNode.h"


IMPLEMENT_PERSISTENT(CAdSceneNode, false);

//////////////////////////////////////////////////////////////////////////
CAdSceneNode::CAdSceneNode(CBGame* inGame):CBObject(inGame)
{
	m_Type = OBJECT_NONE;
	m_Region = NULL;
	m_Entity = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdSceneNode::~CAdSceneNode()
{
	Game->UnregisterObject(m_Region);
	m_Region = NULL;

	Game->UnregisterObject(m_Entity);
	m_Entity = NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneNode::SetEntity(CAdEntity *Entity)
{
	m_Type = OBJECT_ENTITY;
	m_Entity = Entity;
	return Game->RegisterObject(Entity);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneNode::SetRegion(CAdRegion *Region)
{
	m_Type = OBJECT_REGION;
	m_Region = Region;
	return Game->RegisterObject(Region);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneNode::Persist(CBPersistMgr* PersistMgr){

	CBObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Entity));
	PersistMgr->Transfer(TMEMBER(m_Region));
	PersistMgr->Transfer(TMEMBER_INT(m_Type));

	return S_OK;
}
