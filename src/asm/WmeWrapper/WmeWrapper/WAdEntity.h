// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdEntity : public WAdTalkHolder
	{
	public:
		WAdEntity(){};
		WAdEntity(WGame^ Game);
		WAdEntity(CAdEntity* Native);
		virtual ~WAdEntity(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property int WalkToX
		{
			int get()
			{
				return Native->m_WalkToX;
			}
			void set(int Value)
			{
				Native->m_WalkToX = Value;
			}
		}

		property int WalkToY
		{
			int get()
			{
				return Native->m_WalkToY;
			}
			void set(int Value)
			{
				Native->m_WalkToY = Value;
			}
		}

		property WEDirection WalkToDirection
		{
			WEDirection get()
			{
				return (WEDirection)Native->m_WalkToDir;
			}
			void set(WEDirection Value)
			{
				Native->m_WalkToDir = (TDirection)Value;
			}
		}

		property String^ AttachedItem
		{
			String^ get()
			{
				if(Native->m_Item) return gcnew String(Native->m_Item);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				Native->SetItem(S);
				WUtils::FreeString(S);
			}
		}

		property WRegion^ Region
		{
			WRegion^ get()
			{
				if(Native->m_Region) return gcnew WRegion(Native->m_Region);
				else return nullptr;
			}
			void set(WRegion^ Value)
			{
				SAFE_DELETE(Native->m_Region);
				if(Value!=nullptr) Native->m_Region = Value->Native;
			}
		}

		property WEEntityType Subtype
		{
			WEEntityType get()
			{
				return (WEEntityType)Native->m_Subtype;
			}
			void set(WEEntityType Value)
			{
				Native->m_Subtype = (TEntityType)Value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		int GetHeight()
		{
			return Native->GetHeight();
		}

		bool UpdatePosition()
		{
			Native->UpdatePosition();
			return true;
		}

		virtual bool Update() override
		{
			return SUCCEEDED(Native->Update());
		}

		virtual bool Display() override
		{
			return SUCCEEDED(Native->Display());
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

		virtual bool SaveAsText(WDynBuffer^ Buffer, int Indent) override
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		virtual bool SaveAsText(WDynBuffer^ Buffer) override
		{
			return SaveAsText(Buffer, 0);
		}

	internal:
		property CAdEntity* Native
		{
			CAdEntity* get() { return (CAdEntity*)m_Native; };
		}

	};

}}};

