// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WAdRegion;
	ref class WAdWaypointGroup;
	ref class WAdInventory;

	public ref class WAdObject : public WObject
	{
	public:
		WAdObject(){};
		WAdObject(WGame^ Game);
		WAdObject(CAdObject* Native);
		virtual ~WAdObject(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool SubtitlesPosRelative
		{
			bool get()
			{
				return Native->m_SubtitlesModRelative;
			}
			void set(bool Value)
			{
				Native->m_SubtitlesModRelative = Value;
			}
		}

		property int SubtitlesPosX
		{
			int get()
			{
				return Native->m_SubtitlesModX;
			}
			void set(int Value)
			{
				Native->m_SubtitlesModX = Value;
			}
		}

		property int SubtitlesPosY
		{
			int get()
			{
				return Native->m_SubtitlesModY;
			}
			void set(int Value)
			{
				Native->m_SubtitlesModY = Value;
			}
		}

		property int SubtitlesWidth
		{
			int get()
			{
				return Native->m_SubtitlesWidth;
			}
			void set(int Value)
			{
				Native->m_SubtitlesWidth = Value;
			}
		}

		property WAdRegion^ StickToRegion
		{
			WAdRegion^ get();
			void set(WAdRegion^ Value);
		}

		property bool SceneIndependent
		{
			bool get()
			{
				return Native->m_SceneIndependent;
			}
			void set(bool Value)
			{
				Native->m_SceneIndependent = Value;
			}
		}

		property bool IgnoreItems
		{
			bool get()
			{
				return Native->m_IgnoreItems;
			}
			void set(bool Value)
			{
				Native->m_IgnoreItems = Value;
			}
		}

		property WFont^ Font
		{
			WFont^ get()
			{
				if(Native->m_Font) return gcnew WFont(Native->m_Font);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(Native->m_Font) Native->Game->m_FontStorage->RemoveFont(Native->m_Font);

				if(Value!=nullptr) Native->m_Font = Value->Native;
				else Native->m_Font = NULL;
			}
		}

		property bool Active
		{
			bool get()
			{
				return Native->m_Active;
			}
			void set(bool Value)
			{
				Native->m_Active = Value;
			}
		}

		property WRegion^ BlockedRegion
		{
			WRegion^ get()
			{
				if(Native->m_BlockRegion) return gcnew WRegion(Native->m_BlockRegion);
				else return nullptr;
			}
			void set(WRegion^ Value)
			{
				SAFE_DELETE(Native->m_BlockRegion);
				SAFE_DELETE(Native->m_CurrentBlockRegion);

				if(Value!=nullptr)
				{
					Native->m_BlockRegion = Value->Native;
					Native->m_CurrentBlockRegion = new CBRegion(Native->m_BlockRegion->Game);
					Native->m_CurrentBlockRegion->Mimic(Native->m_BlockRegion);
				}
			}
		}

		property WRegion^ CurrentBlockedRegion
		{
			WRegion^ get()
			{
				if(Native->m_CurrentBlockRegion) return gcnew WRegion(Native->m_CurrentBlockRegion);
				else return nullptr;
			}
		}

		property WAdWaypointGroup^ Waypoints
		{
			WAdWaypointGroup^ get();
			void set(WAdWaypointGroup^ Value);
		}

		property WAdWaypointGroup^ CurrentWaypoints
		{
			WAdWaypointGroup^ get();
		}

		property WAdInventory^ Inventory
		{
			WAdInventory^ get();
		}

		property WEObjectState State
		{
			WEObjectState get()
			{
				return (WEObjectState)Native->m_State;
			}
		}

		property WEObjectState NextState
		{
			WEObjectState get()
			{
				return (WEObjectState)Native->m_NextState;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool UpdateBlockedRegion()
		{
			return SUCCEEDED(Native->UpdateBlockRegion());
		}

		bool UpdateSounds()
		{
			return SUCCEEDED(Native->UpdateSounds());
		}

		bool WAdObject::Reset()
		{
			return SUCCEEDED(Native->Reset());
		}

		bool Talk(String^ Text, String^ SoundFilename, int Duration, String^ Stances, WETextAlign Align)
		{
			char* SText = NULL;
			if(Text) SText = (char*)WUtils::GetString(Text);

			char* SSoundFilename = NULL;
			if(SoundFilename) SSoundFilename = (char*)WUtils::GetString(SoundFilename);

			char* SStances = NULL;
			if(Stances) SStances = (char*)WUtils::GetString(Stances);

			Native->Talk(SText, SSoundFilename, Duration, SStances, (TTextAlign)Align);

			if(SText) WUtils::FreeString(SText);
			if(SSoundFilename) WUtils::FreeString(SSoundFilename);
			if(SStances) WUtils::FreeString(SStances);

			return true;
		}

		bool Talk(String^ Text, String^ SoundFilename, int Duration, String^ Stances)
		{
			return Talk(Text, SoundFilename, Duration, Stances, WETextAlign::Center);
		}

		bool Talk(String^ Text, String^ SoundFilename, int Duration)
		{
			return Talk(Text, SoundFilename, Duration, nullptr);
		}

		bool Talk(String^ Text, String^ SoundFilename)
		{
			return Talk(Text, SoundFilename, 0);
		}

		virtual bool Talk(String^ Text)
		{
			return Talk(Text, nullptr);
		}

		virtual bool Update()
		{
			return SUCCEEDED(Native->Update());
		}

		virtual bool Display()
		{
			return SUCCEEDED(Native->Display());
		}

		bool PlayAnim(String^ Filename)
		{
			char* S = (char*)WUtils::GetString(Filename);
			bool Ret = SUCCEEDED(Native->PlayAnim(S));
			WUtils::FreeString(S);
			return Ret;
		}

		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		virtual bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		virtual bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}


	internal:
		property CAdObject* Native
		{
			CAdObject* get() { return (CAdObject*)m_Native; };
		}

	};

}}};

