// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WScScript.h"
#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace DeadCode::WME::Core;

	ref class WScriptHolderTypeConverter;

	[TypeConverter(WScriptHolderTypeConverter::typeid)]
	public ref class WScriptHolder : public WScriptable
	{
	public:
		WScriptHolder(){};
		WScriptHolder(WGame^ Game);
		WScriptHolder(CBScriptHolder* Native);
		virtual ~WScriptHolder(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CScScript*, WScScript>^ Scripts
		{
			WDynArray<CScScript*, WScScript>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CScScript*, WScScript>(&Native->m_Scripts);
			}
		}

		property String^ Name
		{
			String^ get()
			{
				if(Native->m_Name) return gcnew String(Native->m_Name);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				Native->SetName(S);
				WUtils::FreeString(S);
			}
		}

		property bool Ready
		{
			bool get()
			{
				return Native->m_Ready;
			}
		}

		property String^ Filename
		{
			String^ get();
			void set(String^ value);
		}

		property bool Freezable
		{
			bool get()
			{
				return Native->m_Freezable;
			}
			void set(bool Value)
			{
				Native->MakeFreezable(Value);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Micelaneous
		//////////////////////////////////////////////////////////////////////////
		bool AddScript(String^ Filename)
		{
			char* S = (char*)WUtils::GetString(Filename);
			bool Ret = SUCCEEDED(Native->AddScript(S));
			WUtils::FreeString(S);
			return Ret;
		}

		bool RemoveScript(WScScript^ Script)
		{
			if(Script==nullptr) return false;
			else return SUCCEEDED(Native->RemoveScript(Script->Native));
		}

		bool EditorRemoveAllScripts()
		{
			for(int i=0; i<Native->m_Scripts.GetSize(); i++)
			{
				Native->m_Scripts[i]->m_Owner = NULL;
				Native->m_Scripts[i]->Finish();
			}
			Native->m_Scripts.RemoveAll();
			Native->Game->m_ScEngine->EditorCleanup();

			return true;
		}

	internal:
		property CBScriptHolder* Native
		{
			CBScriptHolder* get() { return (CBScriptHolder*)m_Native; };
		}
	};


	//////////////////////////////////////////////////////////////////////////
	public ref class WScriptHolderTypeConverter : public TypeConverter
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		virtual bool CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType) override
		{
			if (destinationType == String::typeid)
			{
				return true;
			}
			else return TypeConverter::CanConvertFrom(context, destinationType);
		}

		//////////////////////////////////////////////////////////////////////////
		virtual Object^ ConvertTo(ITypeDescriptorContext^ context, System::Globalization::CultureInfo^ culture, Object^ value, Type^ destinationType) override
		{
			WScriptHolder^ Holder = dynamic_cast<WScriptHolder^>(value);
			if(Holder != nullptr && destinationType == String::typeid && Holder->Filename != nullptr)
				return Holder->Filename;
			else
				return TypeConverter::ConvertTo(context, culture, value, destinationType);
		}

	};


}}};

