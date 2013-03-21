// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WPropCollection.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WPropCollection::WPropCollection(WScValue^ Value)
{
	m_Value = Value;
}

//////////////////////////////////////////////////////////////////////////
int WPropCollection::Count::get()
{
	return m_Value->Native->m_ValObject.size();
}


//////////////////////////////////////////////////////////////////////////
WScValue^ WPropCollection::default::get(String^ PropName)
{
	if(PropName==nullptr) return nullptr;

	char* S = (char*)WUtils::GetString(PropName);
	CScValue* Val = m_Value->Native->GetProp(S);
	WUtils::FreeString(S);

	if(Val) return gcnew WScValue(Val);
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void WPropCollection::default::set(String^ PropName, WScValue^ Value)
{
	if(PropName==nullptr || Value==nullptr) return;

	char* S = (char*)WUtils::GetString(PropName);
	m_Value->Native->SetProp(S, Value->Native);
	WUtils::FreeString(S);
}

//////////////////////////////////////////////////////////////////////////
IEnumerator^ WPropCollection::GetEnumerator()
{
	return gcnew WPropCollectionEnumerator(m_Value);
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollection::IsSynchronized::get()
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
Object^ WPropCollection::SyncRoot::get()
{
	return this;
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollection::ContainsProperty(String^ PropName)
{
	char* S = (char*)WUtils::GetString(PropName);
	bool Ret = m_Value->Native->PropExists(S);
	WUtils::FreeString(S);

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollection::DeleteProperty(String^ PropName)
{
	char* S = (char*)WUtils::GetString(PropName);
	bool Ret = SUCCEEDED(m_Value->Native->DeleteProp(S));
	WUtils::FreeString(S);

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollection::SetProperty(String^ PropName, WScValue^ Value, bool CopyEverything, bool SetAsConst)
{
	if(Value==nullptr) return false;

	char* S = (char*)WUtils::GetString(PropName);
	bool Ret = SUCCEEDED(m_Value->Native->SetProp(S, Value->Native, CopyEverything, SetAsConst));
	WUtils::FreeString(S);

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollection::SetProperty(String^ PropName, WScValue^ Value, bool CopyEverything)
{
	return SetProperty(PropName, Value, CopyEverything, false);
}


//////////////////////////////////////////////////////////////////////////
bool WPropCollection::SetProperty(String^ PropName, WScValue^ Value)
{
	return SetProperty(PropName, Value, false);
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollection::ClearProperties(bool IncludingNatives)
{
	m_Value->Native->CleanProps(IncludingNatives);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollection::ClearProperties()
{
	return ClearProperties(false);
}


/////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
WPropCollectionEnumerator::WPropCollectionEnumerator(WScValue^ Value)
{
	m_Value = Value;
	Reset();
};

//////////////////////////////////////////////////////////////////////////
Object^ WPropCollectionEnumerator::Current::get()
{
	m_Value->Native->m_ValIter = m_Value->Native->m_ValObject.begin();
	for(int i=0; i<m_CurrentIndex; i++)
	{
		m_Value->Native->m_ValIter++;
	}
	if(m_Value->Native->m_ValIter != m_Value->Native->m_ValObject.end())
	{
		WScValue^ Val = gcnew WScValue(m_Value->Native->m_ValIter->second);
		Val->PropertyName = gcnew System::String(m_Value->Native->m_ValIter->first.c_str());
		return Val;
	}
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
bool WPropCollectionEnumerator::MoveNext()
{
	m_CurrentIndex++;
	return m_CurrentIndex < m_Value->Native->m_ValObject.size();
}

//////////////////////////////////////////////////////////////////////////
void WPropCollectionEnumerator::Reset()
{
	m_CurrentIndex = -1;
}
