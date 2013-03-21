// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BViewport.h"



IMPLEMENT_PERSISTENT(CBViewport, false);

//////////////////////////////////////////////////////////////////////////
CBViewport::CBViewport(CBGame* inGame):CBBase(inGame)
{
	SetRectEmpty(&m_Rect);
	m_MainObject = NULL;
	m_OffsetX = m_OffsetY = 0;
}


//////////////////////////////////////////////////////////////////////////
CBViewport::~CBViewport()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CBViewport::Persist(CBPersistMgr* PersistMgr){

	PersistMgr->Transfer(TMEMBER(Game));

	PersistMgr->Transfer(TMEMBER(m_MainObject));
	PersistMgr->Transfer(TMEMBER(m_OffsetX));
	PersistMgr->Transfer(TMEMBER(m_OffsetY));
	PersistMgr->Transfer(TMEMBER(m_Rect));

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBViewport::SetRect(int left, int top, int right, int bottom, bool NoCheck)
{
	if(!NoCheck){
		left = max(left, 0);
		top = max(top, 0);
		right = min(right, Game->m_Renderer->m_Width);
		bottom = min(bottom, Game->m_Renderer->m_Height);
	}
	
	::SetRect(&m_Rect, left, top, right, bottom);
	m_OffsetX = left;
	m_OffsetY = top;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
RECT* CBViewport::GetRect()
{
	return &m_Rect;
}


//////////////////////////////////////////////////////////////////////////
int CBViewport::GetWidth()
{
	return m_Rect.right - m_Rect.left;
}


//////////////////////////////////////////////////////////////////////////
int CBViewport::GetHeight()
{
	return m_Rect.bottom - m_Rect.top;
}
