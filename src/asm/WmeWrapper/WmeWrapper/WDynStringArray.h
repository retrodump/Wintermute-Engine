// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WUtils.h"

using namespace System::Collections;

namespace DeadCode { namespace WME { namespace Core
{

	ref class WDynStringArrayEnumerator;


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public ref class WDynStringArray : public ICollection
	{
	public:		
		WDynStringArray(CBArray<char*, char*>* Array);

		property virtual int Count
		{
			int get();
		}

		property String^ default[int]
		{
			String^ get(int Index);
			void set(int Index, String^ Value);
		}

		virtual IEnumerator^ GetEnumerator();

		property virtual bool IsSynchronized
		{
			bool get();
		}

		property virtual Object^ SyncRoot
		{
			Object^ get();
		}

		virtual void CopyTo(Array^ Array, int StartIndex);
		virtual int Add(String^ Value);
		virtual void Clear();
		virtual bool Contains(String^ Value);
		virtual int IndexOf(String^ Value);
		virtual void Insert(int Index, String^ Value);
		virtual void Remove(String^ Value);
		virtual void RemoveAt(int Index);

	private:
		CBArray<char*, char*>* m_Array;
	};



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public ref class WDynStringArrayEnumerator : public IEnumerator
	{
	public:
		WDynStringArrayEnumerator(WDynStringArray^ Collection);

		property virtual Object^ Current
		{
			Object^ get();
		}

		virtual bool MoveNext();
		virtual void Reset();

	private:
		WDynStringArray^ m_Collection;
		int m_CurrentIndex;
	};

}}};
