// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADSCENENODE_H__7CA97876_3E0A_451B_A0EA_E8CE62AE29BE__INCLUDED_)
#define AFX_ADSCENENODE_H__7CA97876_3E0A_451B_A0EA_E8CE62AE29BE__INCLUDED_

#include "AdTypes.h"	// Added by ClassView
#include "AdRegion.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdEntity.h"

class CAdSceneNode : public CBObject
{
public:
	DECLARE_PERSISTENT(CAdSceneNode, CBObject);
	HRESULT SetRegion(CAdRegion* Region);
	HRESULT SetEntity(CAdEntity* Entity);
	CAdEntity* m_Entity;
	CAdRegion* m_Region;
	TObjectType m_Type;
	CAdSceneNode(CBGame* inGame);
	virtual ~CAdSceneNode();

};

#endif // !defined(AFX_ADSCENENODE_H__7CA97876_3E0A_451B_A0EA_E8CE62AE29BE__INCLUDED_)
