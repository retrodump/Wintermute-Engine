// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynBuffer.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace System::Drawing;
	using namespace DeadCode::WME::Core;


	public ref class WSubFrame : public WScriptable
	{
	public:
		WSubFrame(){};
		WSubFrame(WGame^ Game);
		WSubFrame(CBSubFrame* Native);
		virtual ~WSubFrame(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool Decoration
		{
			bool get()
			{
				return Native->m_Decoration;
			}
			void set(bool Value)
			{
				Native->m_Decoration = Value;
			}
		}

		property System::Drawing::Color TransparentColor
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_Transparent);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_Transparent = Value.ToArgb();
			}
		}

		property int HotspotX
		{
			int get()
			{
				return Native->m_HotspotX;
			}
			void set(int Value)
			{
				Native->m_HotspotX = Value;
			}
		}

		property int HotspotY
		{
			int get()
			{
				return Native->m_HotspotY;
			}
			void set(int Value)
			{
				Native->m_HotspotY = Value;
			}
		}

		property System::Drawing::Color Color
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_Alpha);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_Alpha = Value.ToArgb();
			}
		}

		property System::Drawing::Rectangle Rectangle
		{
			System::Drawing::Rectangle get()
			{
				return System::Drawing::Rectangle::FromLTRB(Native->m_Rect.left, Native->m_Rect.top, Native->m_Rect.right, Native->m_Rect.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				if(Value==System::Drawing::Rectangle::Empty) Native->SetDefaultRect();
				else
				{
					Native->m_Rect.left = Value.Left;
					Native->m_Rect.right = Value.Right;
					Native->m_Rect.top = Value.Top;
					Native->m_Rect.bottom = Value.Bottom;
				}
			}
		}

		property String^ ImageFilename
		{
			String^ get()
			{
				return gcnew String(Native->m_SurfaceFilename);
			}
		}

		property bool Is2DOnly
		{
			bool get()
			{
				return Native->m_2DOnly;
			}
			void set(bool Value)
			{
				Native->m_2DOnly = Value;
			}
		}

		property bool Is3DOnly
		{
			bool get()
			{
				return Native->m_3DOnly;
			}
			void set(bool Value)
			{
				Native->m_3DOnly = Value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool SetImage(String^ Filename)
		{
			char* S = (char*)WUtils::GetString(Filename);
			bool Ret = SUCCEEDED(Native->SetSurface(S));
			WUtils::FreeString(S);
			return Ret;
		}

		bool SetDefaultRect()
		{
			Native->SetDefaultRect();
			return true;
		}

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect, System::Drawing::Color Color, float Rotate);

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect, System::Drawing::Color Color)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, PixelPerfect, Color, 0.0f);
		}

		bool Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect)
		{
			return Display(X, Y, Register, ScaleX, ScaleY, PixelPerfect, System::Drawing::Color::FromArgb(0xFFFFFFFF));
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

		System::Drawing::Rectangle WSubFrame::GetBoundingRect(int X, int Y)
		{
			RECT rc;
			Native->GetBoundingRect(&rc, X, Y);
			return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
		}

		System::Drawing::Rectangle GetBoundingRect()
		{
			return GetBoundingRect(0, 0);
		}

		Bitmap^ GetBitmap()
		{
			if(!Native->m_Surface) return nullptr;
			
			Bitmap^ Ret = gcnew Bitmap(Native->m_Surface->GetWidth(), Native->m_Surface->GetHeight());
			
			Native->m_Surface->StartPixelOp();
			for(int y = 0; y < Native->m_Surface->GetHeight(); y++)
			{
				for(int x = 0; x < Native->m_Surface->GetWidth(); x++)
				{
					BYTE r, g, b, a;
					Native->m_Surface->GetPixel(x, y, &r, &g, &b, &a);
					Ret->SetPixel(x, y, System::Drawing::Color::FromArgb(a, r, g, b));
				}
			}
			Native->m_Surface->EndPixelOp();

			return Ret;
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

		bool SaveAsText(WDynBuffer^ Buffer, int Indent, bool Complete)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent, Complete));
		}

		bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SaveAsText(Buffer, Indent, true);
		}

		bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}

	internal:
		property CBSubFrame* Native
		{
			CBSubFrame* get() { return (CBSubFrame*)m_Native; };
		}

	};

}}};

