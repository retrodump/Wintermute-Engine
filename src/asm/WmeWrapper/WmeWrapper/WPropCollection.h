// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WUtils.h"
#include "WScValue.h"

using namespace System::Collections;

namespace DeadCode { namespace WME { namespace Core
{

	ref class WPropCollectionEnumerator;
	ref class WScValue;


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public ref class WPropCollection : public IEnumerable
	{
	public:		
		WPropCollection(WScValue^ Value);

		property virtual int Count
		{
			int get();
		}

		property WScValue^ default[String^]
		{
			WScValue^ get(String^ PropName);
			void set(String^ PropName, WScValue^ Value);
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

		bool ContainsProperty(String^ PropName);
		bool DeleteProperty(String^ PropName);
		bool SetProperty(String^ PropName, WScValue^ Value, bool CopyEverything, bool SetAsConst);
		bool SetProperty(String^ PropName, WScValue^ Value, bool CopyEverything);
		bool SetProperty(String^ PropName, WScValue^ Value);
		bool ClearProperties(bool IncludingNatives);
		bool ClearProperties();


	private:
		WScValue^ m_Value;
	};



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public ref class WPropCollectionEnumerator : public IEnumerator
	{
	public:
		WPropCollectionEnumerator(WScValue^ Value);

		property virtual Object^ Current
		{
			Object^ get();
		}

		virtual bool MoveNext();
		virtual void Reset();

	private:
		WScValue^ m_Value;
		int m_CurrentIndex;
	};

}}};
