// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WDynStringArray.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WDynStringArray::WDynStringArray(CBArray<char*, char*>* Array)
{
	m_Array = Array;
}

//////////////////////////////////////////////////////////////////////////
int WDynStringArray::Count::get()
{
	return m_Array->GetSize();
}


//////////////////////////////////////////////////////////////////////////
String^ WDynStringArray::default::get(int Index)
{
	return gcnew String(m_Array->GetAt(Index));
}


void WDynStringArray::default::set(int Index, String^ Value)
{
	const char* S = WUtils::GetString(Value);
	char* S2 = NULL;
	CBUtils::SetString(&S2, (char*)S);
	WUtils::FreeString(S);

	m_Array->SetAt(Index, S2);
}

//////////////////////////////////////////////////////////////////////////
IEnumerator^ WDynStringArray::GetEnumerator()
{
	return gcnew WDynStringArrayEnumerator(this);
}

//////////////////////////////////////////////////////////////////////////
bool WDynStringArray::IsSynchronized::get()
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
Object^ WDynStringArray::SyncRoot::get()
{
	return this;
}

//////////////////////////////////////////////////////////////////////////
void WDynStringArray::CopyTo(Array^ Array, int StartIndex)
{
	if(Array == nullptr) throw gcnew System::NullReferenceException();

	if(Array->Rank != 1)
		throw gcnew System::ArgumentException( "Array", "Array must be one dimensional");			
	if(StartIndex < 0)
		throw gcnew System::ArgumentOutOfRangeException( "StartIndex", "StartIndex must be greater or equal to 0" );
	if(StartIndex + Count > Array->Length)
		throw gcnew System::ArgumentException( "Array", "The array is not large enough to copy the collection contents into" );

	for (int i = 0; i < Count; i++)
		Array->SetValue(this[i], StartIndex + i);
}

//////////////////////////////////////////////////////////////////////////
int WDynStringArray::Add(String^ Value)
{
	const char* S = WUtils::GetString(Value);
	char* S2 = NULL;
	CBUtils::SetString(&S2, (char*)S);
	WUtils::FreeString(S);

	return m_Array->Add(S2);
}

//////////////////////////////////////////////////////////////////////////
void WDynStringArray::Clear()
{
	for(int i=0; i<m_Array->GetSize(); i++)
	{
		delete [] m_Array->GetAt(i);
	}
	m_Array->RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
bool WDynStringArray::Contains(String^ Value)
{
	if(Value==nullptr) return false;

	const char* S = WUtils::GetString(Value);			
	for(int i=0; i<m_Array->GetSize(); i++)
	{
		if(strcmp(m_Array->GetAt(i), S)==0)
		{
			WUtils::FreeString(S);
			return true;
		}
	}

	WUtils::FreeString(S);
	return false;
}

//////////////////////////////////////////////////////////////////////////
int WDynStringArray::IndexOf(String^ Value)
{
	if(Value==nullptr) return -1;

	const char* S = WUtils::GetString(Value);			

	for(int i=0; i<m_Array->GetSize(); i++)
	{
		if(strcmp(m_Array->GetAt(i), S)==0)
		{
			WUtils::FreeString(S);
			return i;
		}
	}
	WUtils::FreeString(S);
	return -1;
}

//////////////////////////////////////////////////////////////////////////
void WDynStringArray::Insert(int Index, String^ Value)
{
	const char* S = WUtils::GetString(Value);
	char* S2 = NULL;
	CBUtils::SetString(&S2, (char*)S);
	WUtils::FreeString(S);

	m_Array->InsertAt(Index, S2);
}

//////////////////////////////////////////////////////////////////////////
void WDynStringArray::Remove(String^ Value)
{
	if(Value==nullptr) return;

	const char* S = WUtils::GetString(Value);			

	for(int i=0; i<m_Array->GetSize(); i++)
	{
		if(strcmp(m_Array->GetAt(i), S)==0)
		{
			m_Array->RemoveAt(i);
		}
	}
	WUtils::FreeString(S);
}

//////////////////////////////////////////////////////////////////////////
void WDynStringArray::RemoveAt(int Index)
{
	if(Index>=0 && Index<m_Array->GetSize()) delete [] m_Array->GetAt(Index);
	m_Array->RemoveAt(Index);
}


/////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
WDynStringArrayEnumerator::WDynStringArrayEnumerator(WDynStringArray^ Collection)
{
	m_Collection = Collection;
	m_CurrentIndex = -1;
};

Object^ WDynStringArrayEnumerator::Current::get()
{
	try
	{
		return m_Collection[m_CurrentIndex];
	}
	catch (System::IndexOutOfRangeException^)
	{
		throw gcnew System::InvalidOperationException();
	}
}

//////////////////////////////////////////////////////////////////////////
bool WDynStringArrayEnumerator::MoveNext()
{
	m_CurrentIndex++;
	if(m_CurrentIndex>=m_Collection->Count) return false;
	else return true;
}

//////////////////////////////////////////////////////////////////////////
void WDynStringArrayEnumerator::Reset()
{
	m_CurrentIndex = -1;
}
