// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	//////////////////////////////////////////////////////////////////////////
	public ref class WAdSpriteSetArray
	{
	internal:
		WAdSpriteSetArray(CAdSpriteSet* Native)
		{
			m_Native = Native;
		}
	public:
		property virtual int Count
		{
			int get()
			{
				return NUM_DIRECTIONS;
			}
		}

		property WSprite^ default[int]
		{
			WSprite^ get(int Index)
			{
				if(Index < 0 || Index >= NUM_DIRECTIONS)
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				return gcnew WSprite(m_Native->m_Sprites[Index]);

			}
			void set(int Index, WSprite^ Value)
			{
				if(Index < 0 || Index >= NUM_DIRECTIONS)
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				SAFE_DELETE(m_Native->m_Sprites[Index]);
				if(Value!=nullptr) m_Native->m_Sprites[Index] = Value->Native;
			}
		}
	private:
		CAdSpriteSet* m_Native;
	};



	//////////////////////////////////////////////////////////////////////////
	public ref class WAdSpriteSet : public WObject
	{
	public:
		WAdSpriteSet(){};
		WAdSpriteSet(WGame^ Game);
		WAdSpriteSet(CAdSpriteSet* Native);
		virtual ~WAdSpriteSet(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WAdSpriteSetArray^ Sprites
		{
			WAdSpriteSetArray^ get()
			{
				return gcnew WAdSpriteSetArray(Native);
			}
		}

		property WObject^ Owner
		{
			WObject^ get()
			{
				if(!Native->m_Owner) return nullptr;
				else return WUtils::CastObject(Native->m_Owner);
			}
			void set(WObject^ Value)
			{
				if(Value==nullptr) Native->m_Owner = NULL;
				else Native->m_Owner = Value->Native;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		WSprite^ GetNearestSprite(WEDirection Direction)
		{
			CBSprite* Spr = Native->GetSprite((TDirection)Direction);
			if(Spr) return gcnew WSprite(Spr);
			else return nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		bool LoadFromFile(String^ Filename)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Filename);
			Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadFromBuffer(String^ Buffer, bool Complete)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadBuffer((BYTE*)S, Complete));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadFromBuffer(String^ Buffer)
		{
			return LoadFromBuffer(Buffer, true);
		}

		bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}


	internal:
		property CAdSpriteSet* Native
		{
			CAdSpriteSet* get() { return (CAdSpriteSet*)m_Native; };
		}

	};

}}};

