// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WPropCollection.h"
#include "WScriptable.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WScValue : public WBase
	{
	public:
		WScValue(){};
		WScValue(WGame^ Game);
		WScValue(CScValue* Native);
		virtual ~WScValue(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property String^ PropertyName;

		property WEValueType Type
		{
			WEValueType get()
			{
				return (WEValueType)Native->GetType();
			}
		}

		property WEValueType SafeType
		{
			WEValueType get()
			{
				return (WEValueType)Native->GetTypeTolerant();
			}
		}

		property WPropCollection^ Properties
		{
			WPropCollection^ get()
			{
				return gcnew WPropCollection(this);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		static int Compare(WScValue^ Val1, WScValue^ Val2)
		{
			if(Val1==nullptr && Val2==nullptr) return 0;
			if(Val1==nullptr) return -1;
			if(Val2==nullptr) return 1;

			return CScValue::Compare(Val1->Native, Val2->Native);
		}

		int CompareStrict(WScValue^ Val1, WScValue^ Val2)
		{
			if(Val1==nullptr && Val2==nullptr) return 0;
			if(Val1==nullptr) return -1;
			if(Val2==nullptr) return 1;

			return CScValue::CompareStrict(Val1->Native, Val2->Native);
		}

		bool SetValue(WScValue^ Val)
		{
			if(Val==nullptr) return false;
			Native->SetValue(Val->Native);
			return true;
		}

		bool Copy(WScValue^ Val, bool CopyEverything)
		{
			if(Val==nullptr) return false;
			Native->Copy(Val->Native, CopyEverything);
			return true;
		}

		bool Copy(WScValue^ Val)
		{
			return Copy(Val, false);
		}

		bool GetBool()
		{
			return Native->GetBool();
		}

		int GetInt()
		{
			return Native->GetInt();
		}

		double GetFloat()
		{
			return Native->GetFloat();
		}

		String^ WScValue::GetString()
		{
			return gcnew String(Native->GetString());
		}

		WScriptable^ GetNative()
		{
			CBScriptable* Obj = Native->GetNative();
			if(Obj) return gcnew WScriptable(Obj);
			else return nullptr;
		}

		bool SetBool(bool Value)
		{
			Native->SetBool(Value);
			return true;
		}

		bool SetInt(int Value)
		{
			Native->SetInt(Value);
			return true;
		}

		bool SetFloat(double Value)
		{
			Native->SetFloat(Value);
			return true;
		}

		bool SetString(String^ Value)
		{
			char* S = (char*)WUtils::GetString(Value);
			Native->SetString(S);
			WUtils::FreeString(S);
			return true;
		}

		bool SetNative(WScriptable^ Value)
		{
			if(Value==nullptr) Native->SetNULL();
			else Native->SetNative(Value->Native);
			return true;
		}

		bool SetNULL()
		{
			Native->SetNULL();
			return true;
		}

		bool SetObject()
		{
			Native->SetObject();
			return true;
		}

		bool SetReference(WScValue^ Value)
		{
			if(Value==nullptr) return false;
			Native->SetReference(Value->Native);
			return true;
		}

		bool IsNULL()
		{
			return Native->IsNULL();
		}

		bool IsNative()
		{
			return Native->IsNative();
		}

		bool IsString()
		{
			return Native->IsString();
		}

		bool IsBool()
		{
			return Native->IsBool();
		}

		bool IsFloat()
		{
			return Native->IsFloat();
		}

		bool IsInt()
		{
			return Native->IsInt();
		}

		bool IsObject()
		{
			return Native->IsObject();
		}


	internal:
		property CScValue* Native
		{
			CScValue* get() { return (CScValue*)m_Native; };
		}
	};
}}}
