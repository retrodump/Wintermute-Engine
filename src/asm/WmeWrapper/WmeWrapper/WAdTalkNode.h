// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdTalkNode : public WBase
	{
	public:
		WAdTalkNode() {};
		WAdTalkNode(WGame^ Game);
		WAdTalkNode(CAdTalkNode* Native);
		virtual ~WAdTalkNode(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property String^ SpriteSetFilename
		{
			String^ get()
			{
				if(Native->m_SpriteSetFilename) return gcnew String(Native->m_SpriteSetFilename);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_SpriteSetFilename, S);
				WUtils::FreeString(S);
			}
		}

		property WAdSpriteSet^ SpriteSet
		{
			WAdSpriteSet^ get()
			{
				if(Native->m_SpriteSet) return gcnew WAdSpriteSet(Native->m_SpriteSet);
				else return nullptr;
			}
			void set(WAdSpriteSet^ Value)
			{
				SAFE_DELETE(Native->m_SpriteSet);
				if(Value!=nullptr) Native->m_SpriteSet = Value->Native;
			}
		}

		property String^ SpriteFilename
		{
			String^ get()
			{
				if(Native->m_SpriteFilename) return gcnew String(Native->m_SpriteFilename);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_SpriteFilename, S);
				WUtils::FreeString(S);
			}
		}

		property WSprite^ Sprite
		{
			WSprite^ get()
			{
				if(Native->m_Sprite) return gcnew WSprite(Native->m_Sprite);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_Sprite);
				if(Value!=nullptr) Native->m_Sprite = Value->Native;
			}
		}

		property int StartTime
		{
			int get()
			{
				return Native->m_StartTime;
			}
			void set(int Value)
			{
				Native->m_StartTime = Value;
			}
		}

		property int EndTime
		{
			int get()
			{
				return Native->m_EndTime;
			}
			void set(int Value)
			{
				Native->m_EndTime = Value;
			}
		}

		property bool PlayToEnd
		{
			bool get()
			{
				return Native->m_PlayToEnd;
			}
			void set(bool Value)
			{
				Native->m_PlayToEnd = Value;
			}				
		}

		property String^ Comment
		{
			String^ get()
			{
				if(Native->m_Comment) return gcnew String(Native->m_Comment);
				else return nullptr;
			}
			void set(String^ Value)
			{
				SAFE_DELETE_ARRAY(Native->m_Comment);
				if(Value!=nullptr)
				{
					char* S = (char*)WUtils::GetString(Value);
					CBUtils::SetString(&Native->m_Comment, S);
					WUtils::FreeString(S);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		WSprite^ GetSprite(WEDirection Direction)
		{
			CBSprite* Spr = Native->GetSprite((TDirection)Direction);
			if(Spr) return gcnew WSprite(Spr);
			else return nullptr;
		}

		WSprite^ GetSprite()
		{
			return GetSprite(WEDirection::Down);
		}

		bool IsInTimeInterval(int Time, WEDirection Direction)
		{
			return Native->IsInTimeInterval(Time, (TDirection)Direction);
		}

		bool IsInTimeInterval(int Time)
		{
			return IsInTimeInterval(Time, WEDirection::Down);
		}

		bool LoadSprite()
		{
			return SUCCEEDED(Native->LoadSprite());
		}

	internal:
		property CAdTalkNode* Native
		{
			CAdTalkNode* get() { return (CAdTalkNode*)m_Native; };
		}
	};
}}}
