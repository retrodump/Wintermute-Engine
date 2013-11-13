// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"
#include "WDynBuffer.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdActor : public WAdTalkHolder
	{
	public:
		WAdActor(){};
		WAdActor(WGame^ Game);
		WAdActor(CAdActor* Native);
		virtual ~WAdActor(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CAdSpriteSet*, WAdSpriteSet>^ TalkSprites
		{
			WDynArray<CAdSpriteSet*, WAdSpriteSet>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdSpriteSet*, WAdSpriteSet>(&Native->m_TalkSprites);
			}
		}

		property WDynArray<CAdSpriteSet*, WAdSpriteSet>^ TalkSpritesEx
		{
			WDynArray<CAdSpriteSet*, WAdSpriteSet>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdSpriteSet*, WAdSpriteSet>(&Native->m_TalkSpritesEx);
			}
		}

		property System::Drawing::Point TargetPoint
		{
			System::Drawing::Point get()
			{
				return System::Drawing::Point(Native->m_TargetPoint->x, Native->m_TargetPoint->y);
			}
			void set(System::Drawing::Point Value)
			{
				Native->m_TargetPoint->x = Value.X;
				Native->m_TargetPoint->y = Value.Y;
			}
		}

		property WEDirection TargetDirection
		{
			WEDirection get()
			{
				return (WEDirection)Native->m_TargetDir;
			}
			void set(WEDirection Value)
			{
				Native->m_TargetDir = (TDirection)Value;
			}
		}

		property WEDirection AfterWalkDirection
		{
			WEDirection get()
			{
				return (WEDirection)Native->m_AfterWalkDir;
			}
			void set(WEDirection Value)
			{
				Native->m_AfterWalkDir = (TDirection)Value;
			}
		}

		property WEDirection Direction
		{
			WEDirection get()
			{
				return (WEDirection)Native->m_Dir;
			}
			void set(WEDirection Value)
			{
				Native->m_Dir = (TDirection)Value;
			}
		}

		property WAdSpriteSet^ WalkSprite
		{
			WAdSpriteSet^ get()
			{
				if(Native->m_WalkSprite) return gcnew WAdSpriteSet(Native->m_WalkSprite);
				else return nullptr;
			}
			void set(WAdSpriteSet^ Value)
			{
				SAFE_DELETE(Native->m_WalkSprite);
				if(Value!=nullptr) Native->m_WalkSprite = Value->Native;
			}
		}

		property WAdSpriteSet^ StandSprite
		{
			WAdSpriteSet^ get()
			{
				if(Native->m_StandSprite) return gcnew WAdSpriteSet(Native->m_StandSprite);
				else return nullptr;
			}
			void set(WAdSpriteSet^ Value)
			{
				SAFE_DELETE(Native->m_StandSprite);
				if(Value!=nullptr) Native->m_StandSprite = Value->Native;
			}
		}

		property WAdSpriteSet^ TurnLeftSprite
		{
			WAdSpriteSet^ get()
			{
				if(Native->m_TurnLeftSprite) return gcnew WAdSpriteSet(Native->m_TurnLeftSprite);
				else return nullptr;
			}
			void set(WAdSpriteSet^ Value)
			{
				SAFE_DELETE(Native->m_TurnLeftSprite);
				if(Value!=nullptr) Native->m_TurnLeftSprite = Value->Native;
			}
		}

		property WAdSpriteSet^ TurnRightSprite
		{
			WAdSpriteSet^ get()
			{
				if(Native->m_TurnRightSprite) return gcnew WAdSpriteSet(Native->m_TurnRightSprite);
				else return nullptr;
			}
			void set(WAdSpriteSet^ Value)
			{
				SAFE_DELETE(Native->m_TurnRightSprite);
				if(Value!=nullptr) Native->m_TurnRightSprite = Value->Native;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		WEDirection AngleToDirection(int Angle)
		{
			return (WEDirection)Native->AngleToDirection(Angle);
		}

		bool GoTo(int X, int Y, WEDirection DirAfterWalk)
		{
			Native->GoTo(X, Y, (TDirection)DirAfterWalk);
			return true;
		}

		bool GoTo(int X, int Y)
		{
			return GoTo(X, Y, WEDirection::None);
		}

		virtual bool Update() override
		{
			return SUCCEEDED(Native->Update());
		}

		virtual bool Display() override
		{
			return SUCCEEDED(Native->Display());
		}

		bool TurnTo(WEDirection Direction)
		{
			Native->TurnTo((TDirection)Direction);
			return true;
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
		property CAdActor* Native
		{
			CAdActor* get() { return (CAdActor*)m_Native; };
		}

	};

}}};

