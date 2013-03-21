// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdResponse : public WObject
	{
	public:
		WAdResponse(){};
		WAdResponse(WGame^ Game);
		WAdResponse(CAdResponse* Native);
		virtual ~WAdResponse(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WSprite^ Icon
		{
			WSprite^ get()
			{
				if(Native->m_Icon) return gcnew WSprite(Native->m_Icon);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_Icon);
				if(Value!=nullptr) Native->m_Icon = Value->Native;
			}
		}

		property WSprite^ IconHover
		{
			WSprite^ get()
			{
				if(Native->m_IconHover) return gcnew WSprite(Native->m_IconHover);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_IconHover);
				if(Value!=nullptr) Native->m_IconHover = Value->Native;
			}
		}

		property WSprite^ IconPressed
		{
			WSprite^ get()
			{
				if(Native->m_IconPressed) return gcnew WSprite(Native->m_IconPressed);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_IconPressed);
				if(Value!=nullptr) Native->m_IconPressed = Value->Native;
			}
		}

		property String^ Text
		{
			String^ get()
			{
				if(Native->m_Text) return gcnew String(Native->m_Text);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				Native->SetText(S);
				WUtils::FreeString(S);
			}
		}

		property int Id
		{
			int get()
			{
				return Native->m_ID;
			}
			void set(int Value)
			{
				Native->m_ID = Value;
			}
		}



	internal:
		property CAdResponse* Native
		{
			CAdResponse* get() { return (CAdResponse*)m_Native; };
		}

	};

}}};

