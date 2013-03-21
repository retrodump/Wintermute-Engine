// HotObject.cpp: implementation of the CHotObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SceneEdit.h"
#include "HotObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CHotObject::CHotObject()
{
	m_Type = HOT_OBJECT_NONE;
	m_Node = NULL;
	m_Point = NULL;
	m_Scale = NULL;
	m_Rot = NULL;
	m_Rect.SetRectEmpty();
}


//////////////////////////////////////////////////////////////////////////
CHotObject::CHotObject(CBPoint* Point, CRect* Rect)
{
	m_Type = HOT_OBJECT_POINT;
	m_Node = NULL;
	m_Scale = NULL;
	m_Rot = NULL;
	m_Point = Point;
	m_Entity = NULL;

	if(Rect) m_Rect = *Rect;
	else m_Rect.SetRect(Point->x-2, Point->y-2, Point->x+2, Point->y+2);
}


//////////////////////////////////////////////////////////////////////////
CHotObject::CHotObject(CAdEntity* Entity, CRect* Rect)
{
	m_Type = HOT_OBJECT_ENTITY;
	m_Node = NULL;
	m_Scale = NULL;
	m_Rot = NULL;
	m_Point = NULL;
	m_Entity = Entity;

	if(Rect) m_Rect = *Rect;
	else{
		if(Entity->m_CurrentSprite){
			RECT rc;				
			Entity->m_CurrentSprite->GetBoundingRect(&rc, Entity->m_PosX, Entity->m_PosY);
			m_Rect = CRect(rc);
		}
		else m_Rect.SetRect(Entity->m_PosX, Entity->m_PosY, Entity->m_PosX + 100, Entity->m_PosY + 100);
	}
}


//////////////////////////////////////////////////////////////////////////
CHotObject::CHotObject(CAdSceneNode* Node, CRect* Rect)
{
	m_Type = HOT_OBJECT_NODE;
	m_Node = Node;
	m_Scale = NULL;
	m_Rot = NULL;
	m_Point = NULL;
	m_Entity = NULL;

	if(Rect) m_Rect = *Rect;
	else{
		switch(m_Node->m_Type){
			case OBJECT_ENTITY:
			{
				CAdEntity* Entity = m_Node->m_Entity;
				if(m_Node->m_Entity->m_CurrentSprite){
					RECT rc;				
					Entity->m_CurrentSprite->GetBoundingRect(&rc, Entity->m_PosX, Entity->m_PosY);
					m_Rect = CRect(rc);
				}
				else m_Rect.SetRect(Entity->m_PosX, Entity->m_PosY, Entity->m_PosX + 100, Entity->m_PosY + 100);
			}
			break;
			
			case OBJECT_REGION:
				m_Rect = CRect(m_Node->m_Region->m_Rect);
			break;
			
			default: m_Rect.SetRectEmpty();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
CHotObject::CHotObject(CAdScaleLevel* Scale, CRect* Rect)
{
	m_Type = HOT_OBJECT_SCALE;
	m_Node = NULL;
	m_Scale = Scale;
	m_Rot = NULL;
	m_Point = NULL;
	m_Entity = NULL;

	if(Rect) m_Rect = *Rect;
	else m_Rect.SetRect(0, Scale->m_PosY, 10, Scale->m_PosY+4);
}


//////////////////////////////////////////////////////////////////////////
CHotObject::CHotObject(CAdRotLevel* Rot, CRect* Rect)
{
	m_Type = HOT_OBJECT_ROT;
	m_Node = NULL;
	m_Scale = NULL;
	m_Rot = Rot;
	m_Point = NULL;
	m_Entity = NULL;

	if(Rect) m_Rect = *Rect;
	else m_Rect.SetRect(Rot->m_PosX, 0, Rot->m_PosX+4, 10);
}


//////////////////////////////////////////////////////////////////////////
CHotObject::~CHotObject()
{

}
