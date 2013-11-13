// HotObject.h: interface for the CHotObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOTOBJECT_H__2E160703_AECA_44B3_A8F4_F96861B5A5C3__INCLUDED_)
#define AFX_HOTOBJECT_H__2E160703_AECA_44B3_A8F4_F96861B5A5C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "dcgf_ad.h"

class CHotObject  
{
public:
	enum{
		HOT_OBJECT_NONE,
		HOT_OBJECT_NODE,
		HOT_OBJECT_POINT,
		HOT_OBJECT_SCALE,
		HOT_OBJECT_ROT,
		HOT_OBJECT_ENTITY
	} m_Type;
	bool IsPoint(){ return m_Type == HOT_OBJECT_POINT; }
	bool IsNode() { return m_Type == HOT_OBJECT_NODE;  }
	bool IsScale() { return m_Type == HOT_OBJECT_SCALE;  }
	bool IsEntity() { return m_Type == HOT_OBJECT_ENTITY;  }
	bool IsRot() { return m_Type == HOT_OBJECT_ROT;  }
	CAdSceneNode* m_Node;
	CAdScaleLevel* m_Scale;
	CAdRotLevel* m_Rot;
	CAdEntity* m_Entity;
	CBPoint* m_Point;
	CRect m_Rect;
	CHotObject();
	CHotObject(CBPoint* Point, CRect* Rect=NULL);
	CHotObject(CAdSceneNode* Node, CRect* Rect=NULL);
	CHotObject(CAdScaleLevel* Scale, CRect* Rect=NULL);
	CHotObject(CAdRotLevel* Rot, CRect* Rect=NULL);
	CHotObject(CAdEntity* Entity, CRect* Rect=NULL);
	virtual ~CHotObject();

};

#endif // !defined(AFX_HOTOBJECT_H__2E160703_AECA_44B3_A8F4_F96861B5A5C3__INCLUDED_)
