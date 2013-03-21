// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WObject.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WObject::WObject(WGame^ Game):WScriptHolder()
{
	m_Game = Game;
	m_Native = new CBObject(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WObject::WObject(CBObject* Native):WScriptHolder(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WObject::~WObject(void)
{
}


//////////////////////////////////////////////////////////////////////////
W3DModel^ WObject::Model::get()
{
	if(Native->m_Model) return gcnew W3DModel(Native->m_Model);
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void WObject::Model::set(W3DModel^ Value)
{
	SAFE_DELETE(Native->m_Model);
	if(Value!=nullptr) Native->m_Model = Value->Native;
}

//////////////////////////////////////////////////////////////////////////
WSprite^ WObject::Cursor::get()
{
	if(Native->m_Cursor) return gcnew WSprite(Native->m_Cursor);
	else return nullptr;
}


//////////////////////////////////////////////////////////////////////////
void WObject::Cursor::set(WSprite^ Value)
{
	if(!Native->m_SharedCursors) SAFE_DELETE(Native->m_Cursor);
	else Native->m_Cursor = NULL;

	if(Value!=nullptr) Native->m_Cursor = Value->Native;
}

//////////////////////////////////////////////////////////////////////////
WSprite^ WObject::ActiveCursor::get()
{
	if(Native->m_ActiveCursor) return gcnew WSprite(Native->m_ActiveCursor);
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void WObject::ActiveCursor::set(WSprite^ Value)
{
	if(!Native->m_SharedCursors) SAFE_DELETE(Native->m_ActiveCursor);
	else Native->m_ActiveCursor = NULL;

	if(Value!=nullptr) Native->m_ActiveCursor = Value->Native;
}
