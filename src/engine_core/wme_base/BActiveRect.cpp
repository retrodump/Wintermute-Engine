// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BActiveRect.h"
#include "XModel.h"


//////////////////////////////////////////////////////////////////////
CBActiveRect::CBActiveRect(CBGame* inGame):CBBase(inGame)
{
	SetRectEmpty(&m_Rect);
	m_Owner = NULL;
	m_Frame = NULL;
	m_Model = NULL;
	m_ModelX = NULL;
	m_Region = NULL;
	m_ZoomX = 100;
	m_ZoomY = 100;
	m_OffsetX = m_OffsetY = 0;
	ClipRect();
}


//////////////////////////////////////////////////////////////////////
CBActiveRect::CBActiveRect(CBGame* inGame, CBObject* Owner, CBSubFrame* Frame, int X, int Y, int Width, int Height, float ZoomX, float ZoomY, bool Precise):CBBase(inGame)
{
	m_Owner = Owner;
	m_Frame = Frame;
	SetRect(&m_Rect, X, Y, X+Width, Y+Height);
	m_ZoomX = ZoomX;
	m_ZoomY = ZoomY;
	m_Precise = Precise;
	m_Model = NULL;
	m_ModelX = NULL;
	m_Region = NULL;
	m_OffsetX = m_OffsetY = 0;
	ClipRect();
}


//////////////////////////////////////////////////////////////////////
CBActiveRect::CBActiveRect(CBGame* inGame, CBObject* Owner, C3DModel* Model, int X, int Y, int Width, int Height, bool Precise):CBBase(inGame)
{
	m_Owner = Owner;
	m_Model = Model;
	m_ModelX = NULL;
	SetRect(&m_Rect, X, Y, X+Width, Y+Height);
	m_ZoomX = 100;
	m_ZoomY = 100;
	m_Precise = Precise;
	m_Frame = NULL;
	m_Region = NULL;
	m_OffsetX = m_OffsetY = 0;
	ClipRect();
}

//////////////////////////////////////////////////////////////////////
CBActiveRect::CBActiveRect(CBGame* inGame, CBObject* Owner, CXModel* ModelX, int X, int Y, int Width, int Height, bool Precise):CBBase(inGame)
{
	m_Owner = Owner;
	m_Model = NULL;
	m_ModelX = ModelX;
	SetRect(&m_Rect, X, Y, X+Width, Y+Height);
	m_ZoomX = 100;
	m_ZoomY = 100;
	m_Precise = Precise;
	m_Frame = NULL;
	m_Region = NULL;
	m_OffsetX = m_OffsetY = 0;
	ClipRect();
}

//////////////////////////////////////////////////////////////////////
CBActiveRect::CBActiveRect(CBGame* inGame, CBObject* Owner, CBRegion* Region, int OffsetX, int OffsetY):CBBase(inGame)
{
	m_Owner = Owner;	
	m_Region = Region;
	CopyRect(&m_Rect, &Region->m_Rect);
	OffsetRect(&m_Rect, -OffsetX, -OffsetY);
	m_ZoomX = 100;
	m_ZoomY = 100;
	m_Precise = true;
	m_Frame = NULL;	
	m_Model = NULL;
	m_ModelX = NULL;
	ClipRect();
	m_OffsetX = OffsetX;
	m_OffsetY = OffsetY;
}


//////////////////////////////////////////////////////////////////////
CBActiveRect::~CBActiveRect()
{
	m_Owner = NULL;
	m_Frame = NULL;
	m_Model = NULL;
	m_ModelX = NULL;
	m_Region = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CBActiveRect::ClipRect()
{
	RECT rc;
	bool CustomViewport;
	Game->GetCurrentViewportRect(&rc, &CustomViewport);
	CBRenderer* Rend = Game->m_Renderer;

	if(!CustomViewport)
	{
		rc.left -= Rend->m_DrawOffsetX;
		rc.right -= Rend->m_DrawOffsetX;
		rc.top -= Rend->m_DrawOffsetY;
		rc.bottom -= Rend->m_DrawOffsetY;
	}

	if(rc.left > m_Rect.left) m_OffsetX = rc.left - m_Rect.left;
	if(rc.top  > m_Rect.top)  m_OffsetY = rc.top  - m_Rect.top;

	IntersectRect(&m_Rect, &m_Rect, &rc);
}
