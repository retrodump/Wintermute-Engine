// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WUtils.h"


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WGame;

	/// <summary>
	/// Summary for WBase
	/// </summary>
	public ref class WBase
	{
	public:
		WBase(void);
		WBase(CBBase* Native);
		WBase(WGame^ Game);
		virtual ~WBase();

		property IntPtr NativeObject
		{
			IntPtr get();
		}

		property WGame^ Game
		{
			WGame^ get();
		}

		virtual bool Equals(Object^ Obj) override
		{
			if(Obj==nullptr) return false;
			WBase^ B = dynamic_cast<WBase^>(Obj);			
			return (B->NativeObject == NativeObject);
		}

		static bool operator ==(WBase^ Left, WBase^ Right)
		{
			if((Object^)Left==nullptr && (Object^)Right==nullptr) return true;
			else if((Object^)Left==nullptr && (Object^)Right!=nullptr) return false;
			else return (Left->Equals(Right));
		}

		static bool operator !=(WBase^ Left, WBase^ Right)
		{
			return !(Left == Right);
		}
		

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		String^ GetEditorProp(String^ Name, String^ InitVal)
		{
			LPCTSTR UmName = WUtils::GetString(Name);
			LPCTSTR UmInitVal = WUtils::GetString(InitVal);

			String^ Ret = gcnew String(Native->GetEditorProp(UmName, UmInitVal));

			WUtils::FreeString(UmName);
			WUtils::FreeString(UmInitVal);

			return Ret;
		}

		String^ GetEditorProp(String^ Name)
		{
			return GetEditorProp(Name, nullptr);
		}

		bool SetEditorProp(String^ Name, String^ Value)
		{
			LPCTSTR UmName = WUtils::GetString(Name);
			LPCTSTR UmValue = WUtils::GetString(Value);

			bool Ret = SUCCEEDED(Native->SetEditorProp(UmName, UmValue));

			WUtils::FreeString(UmName);
			WUtils::FreeString(UmValue);

			return Ret;
		}

		bool DeleteEditorProp(String^ Name)
		{
			return SetEditorProp(Name, nullptr);
		}

		void InvalidateNative()
		{
			m_Native = NULL;
			m_SelfDestroy = false;
		}

		void ClaimOwnership()
		{
			m_SelfDestroy = true;
		}

	protected:
		CBBase* m_Native;
		bool m_SelfDestroy;
		WGame^ m_Game;

	internal:
		property CBBase* Native
		{
			CBBase* get() { return (CBBase*)m_Native; };
		}

	};
}}}
