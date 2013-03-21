// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"
#include "WDynStringArray.h"
#include "WSubFrame.h"
#include "WDynBuffer.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WSound;
	ref class WObject;

	public ref class WFrame : public WScriptable
	{
	public:
		WFrame(){};
		WFrame(WGame^ Game);
		WFrame(CBFrame* Native);
		virtual ~WFrame(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CBSubFrame*, WSubFrame>^ Subframes
		{
			WDynArray<CBSubFrame*, WSubFrame>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBSubFrame*, WSubFrame>(&Native->m_Subframes);
			}
		}

		property WDynStringArray^ Events
		{
			WDynStringArray^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynStringArray(&Native->m_ApplyEvent);
			}
		}

		property bool KillSounds
		{
			bool get()
			{
				return Native->m_KillSound;
			}
			void set(bool Value)
			{
				Native->m_KillSound = Value;
			}
		}

		property bool Keyframe
		{
			bool get()
			{
				return Native->m_Keyframe;
			}
			void set(bool Value)
			{
				Native->m_Keyframe = Value;
			}
		}

		property WSound^ Sound
		{
			WSound^ get();
			void set(WSound^ Value);
		}

		property int MoveX
		{
			int get()
			{
				return Native->m_MoveX;
			}
			void set(int Value)
			{
				Native->m_MoveX = Value;
			}
		}

		property int MoveY
		{
			int get()
			{
				return Native->m_MoveY;
			}
			void set(int Value)
			{
				Native->m_MoveY = Value;
			}
		}

		property int Delay
		{
			int get()
			{
				return Native->m_Delay;
			}
			void set(int Value)
			{
				Native->m_Delay = Value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect, System::Drawing::Color Color, bool AllFrames, float Rotate);

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect, System::Drawing::Color Color, bool AllFrames)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, PixelPerfect, Color, AllFrames, 0.0f);
		}

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect, System::Drawing::Color Color)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, PixelPerfect, Color, false);
		}

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, PixelPerfect, Color::FromArgb(0xFFFFFFFF));
		}

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, true);
		}

		bool Display(int X, int Y, WObject^ Register, float ScaleX)
		{
			return Display(X, Y, Register, ScaleX, 100.0f);
		}

		bool Display(int X, int Y, WObject^ Register)
		{
			return Display(X, Y, Register, 100.0f);
		}

		bool Display(int X, int Y)
		{
			return Display(X, Y, nullptr);
		}

		System::Drawing::Rectangle GetBoundingRect(int X, int Y)
		{
			RECT rc;
			Native->GetBoundingRect(&rc, X, Y);
			return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
		}

		System::Drawing::Rectangle GetBoundingRect()
		{
			return GetBoundingRect(0, 0);
		}


		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		bool LoadFromBuffer(String^ Buffer)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadBuffer((BYTE*)S, -1, true));
			WUtils::FreeString(S);

			return Ret;
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
		property CBFrame* Native
		{
			CBFrame* get() { return (CBFrame*)m_Native; };
		}

	};

}}};

