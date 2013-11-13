// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"
#include "WFrame.h"
#include "WDynBuffer.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::ComponentModel;
	using namespace DeadCode::WME::Core;	

	ref class WGame;
	ref class WObject;	
	
	public ref class WSprite : public WScriptHolder
	{
	public:
		WSprite(){};
		WSprite(WGame^ Game);
		WSprite(CBSprite* Native);
		virtual ~WSprite(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CBFrame*, WFrame>^ Frames
		{
			WDynArray<CBFrame*, WFrame>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBFrame*, WFrame>(&Native->m_Frames);
			}
		}

		property bool Streamed
		{
			bool get()
			{
				return Native->m_Streamed;
			}
			void set(bool Value)
			{
				Native->m_Streamed = Value;
			}
		}

		property bool PixelPerfect
		{
			bool get()
			{
				return Native->m_Precise;
			}
			void set(bool Value)
			{
				Native->m_Precise = Value;
			}
		}

		property bool EditorAllFrames
		{
			bool get()
			{
				return Native->m_EditorAllFrames;
			}
			void set(bool Value)
			{
				Native->m_EditorAllFrames;
			}
		}

		property bool EditorMuted
		{
			bool get()
			{
				return Native->m_EditorMuted;
			}
			void set(bool Value)
			{
				Native->m_EditorMuted = Value;
			}
		}

		property int CurrentMoveX
		{
			int get()
			{
				return Native->m_MoveX;
			}
		}

		property int CurrentMoveY
		{
			int get()
			{
				return Native->m_MoveY;
			}
		}

		property bool CurrentCanBreak
		{
			bool get()
			{
				return Native->m_CanBreak;
			}
		}

		property bool Continuous
		{
			bool get()
			{
				return Native->m_Continuous;
			}
			void set(bool Value)
			{
				Native->m_Continuous = Value;
			}
		}

		property WObject^ Owner
		{
			WObject^ get();
			void set(WObject^ Value);
		}

		property bool FrameChanged
		{
			bool get()
			{
				return Native->m_Changed;
			}
		}

		property bool Finished
		{
			bool get()
			{
				return Native->m_Finished;
			}
		}

		property int LastFrameTime
		{
			int get()
			{
				return Native->m_LastFrameTime;
			}
		}

		property bool Looping
		{
			bool get()
			{
				return Native->m_Looping;
			}
			void set(bool Value)
			{
				Native->m_Looping = Value;
			}
		}

		property int CurrentFrame
		{
			int get()
			{
				return Native->m_CurrentFrame;
			}
			void set(int Value)
			{
				if(Value>=0 && Value<Native->m_Frames.GetSize()) Native->m_CurrentFrame = Value;
				else Native->m_CurrentFrame = -1;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
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

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, System::Drawing::Color Color, float Rotate);

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, System::Drawing::Color Color)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, Color, 0.0f);
		}

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, Color::FromArgb(0xFFFFFFFF));
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

		bool GetCurrentFrame(float ScaleX, float ScaleY)
		{
			return SUCCEEDED(Native->GetCurrentFrame(ScaleX, ScaleY));
		}

		bool GetCurrentFrame(float ScaleX)
		{
			return GetCurrentFrame(ScaleX, 100.0f);
		}

		bool GetCurrentFrame()
		{
			return GetCurrentFrame(100.0f);
		}

		bool WSprite::Reset()
		{
			Native->Reset();
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

		bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}


	internal:
		property CBSprite* Native
		{
			CBSprite* get() { return (CBSprite*)m_Native; };
		}

	};

	//////////////////////////////////////////////////////////////////////////
	public ref class WSpriteTypeConverter : public TypeConverter
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
			WSprite^ Sprite = dynamic_cast<WSprite^>(value);
			if(Sprite != nullptr && destinationType == String::typeid && Sprite->Filename != nullptr)
				return Sprite->Filename;
			else
				return TypeConverter::ConvertTo(context, culture, value, destinationType);
		}

	};

}}};

