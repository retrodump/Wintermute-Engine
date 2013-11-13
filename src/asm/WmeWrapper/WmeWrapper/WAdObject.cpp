// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdObject.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdObject::WAdObject(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdObject(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdObject::WAdObject(CAdObject* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdObject::~WAdObject(void)
{
}

//////////////////////////////////////////////////////////////////////////
WAdRegion^ WAdObject::StickToRegion::get()
{
	if(Native->m_StickRegion) return gcnew WAdRegion(Native->m_StickRegion);
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void WAdObject::StickToRegion::set(WAdRegion^ Value)
{
	if(Value==nullptr) Native->m_StickRegion = NULL;
	else Native->m_StickRegion = Value->Native;
}

//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup^ WAdObject::Waypoints::get()
{
	if(Native->m_WptGroup) return gcnew WAdWaypointGroup(Native->m_WptGroup);
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void WAdObject::Waypoints::set(WAdWaypointGroup^ Value)
{
	SAFE_DELETE(Native->m_WptGroup);
	SAFE_DELETE(Native->m_CurrentWptGroup);

	if(Value!=nullptr)
	{
		Native->m_WptGroup = Value->Native;
		Native->m_CurrentWptGroup = new CAdWaypointGroup(Native->m_WptGroup->Game);
		Native->m_CurrentWptGroup->Mimic(Native->m_WptGroup);
	}
}

//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup^ WAdObject::CurrentWaypoints::get()
{
	if(Native->m_CurrentWptGroup) return gcnew WAdWaypointGroup(Native->m_CurrentWptGroup);
	else return nullptr;
}


//////////////////////////////////////////////////////////////////////////
WAdInventory^ WAdObject::Inventory::get()
{
	return gcnew WAdInventory(Native->GetInventory());
}
