// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace DeadCode::WME::Core;	
	

	public ref class WRenderer : public WBase
	{
	public:
		WRenderer() {};
		WRenderer(WGame^ Game);
		WRenderer(CBRenderer* Native);
		virtual ~WRenderer(void);


		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property int Width
		{
			int get()
			{
				return Native->m_Width;
			}
		}

		property int Height
		{
			int get()
			{
				return Native->m_Height;
			}
		}

		property IntPtr CanvasHandle
		{
			IntPtr get()
			{
				return IntPtr(Native->m_Window);
			}
		}

		property Control^ Canvas
		{
			void set(Control^ Value)
			{
				Native->m_Window = SAFE_CONTROL_HWND(Value);
				Native->m_ClipperWindow = SAFE_CONTROL_HWND(Value);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Resize(int Width, int Height)
		{
			if(Native->Game->m_UseD3D)
			{
				CBRenderD3D* rend = (CBRenderD3D*)Native;
				if(rend->m_Ready){
					rend->m_Ready = false;
					rend->m_Width = rend->m_RealWidth = Width;
					rend->m_Height = rend->m_RealHeight = Height;
					rend->m_Ready = SUCCEEDED(rend->ResetDevice());
				}
			}
			else
			{
				CBRenderDD* rend = (CBRenderDD*)Native;

				if(rend->m_DirectDraw!=NULL)
				{
					rend->ReleaseAll();
					rend->m_Ready = (rend->InitSurfaces()==DD_OK);
				}
			}
			return Native->m_Ready;
		}

		WObject^ GetObjectAt(Point P)
		{
			return GetObjectAt(P.X, P.Y);
		}

		WObject^ GetObjectAt(int X, int Y)
		{
			return WUtils::CastObject(Native->GetObjectAt(X, Y));
		}

		bool DrawLine(int X1, int Y1, int X2, int Y2, Color Color)
		{
			Native->SetupLines();
			return SUCCEEDED(Native->DrawLine(X1, Y1, X2, Y2, Color.ToArgb()));
		}

		bool DrawLine(Point P1, Point P2, Color Color)
		{
			return DrawLine(P1.X, P1.Y, P2.X, P2.Y, Color);
		}

		bool DrawRect(int X1, int Y1, int X2, int Y2, Color Color, int Width)
		{
			Native->SetupLines();
			return SUCCEEDED(Native->DrawRect(X1, Y1, X2, Y2, Color.ToArgb(), Width));
		}

		bool DrawRect(int X1, int Y1, int X2, int Y2, Color Color)
		{
			return DrawRect(X1, Y1, X2, Y2, Color, 1);
		}

		bool DrawRect(System::Drawing::Rectangle Rect, Color Color, int Width)
		{
			return DrawRect(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom, Color, Width);
		}

		bool DrawRect(System::Drawing::Rectangle Rect, Color Color)
		{
			return DrawRect(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom, Color);
		}

		bool Fill(Color Color)
		{
			return SUCCEEDED(Native->Fill(Color.R, Color.G, Color.B));
		}

		bool Fill()
		{
			return Fill(Color::Black);
		}

		bool FadeToColor(Color Color)
		{
			return SUCCEEDED(Native->FadeToColor(Color.ToArgb()));
		}


	internal:
		property CBRenderer* Native
		{
			CBRenderer* get() { return (CBRenderer*)m_Native; };
		}
	};
}}}
