// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	//////////////////////////////////////////////////////////////////////////
	public ref class WFontWidthArray
	{
	internal:
		WFontWidthArray(CBFontBitmap* Native)
		{
			m_Native = Native;
		}
	public:
		property virtual int Count
		{
			int get()
			{
				return NUM_CHARACTERS;
			}
		}

		property System::Byte default[int]
		{
			System::Byte get(int Index)
			{
				if(Index < 0 || Index >= NUM_CHARACTERS)
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				return m_Native->m_Widths[Index];
				
			}
			void set(int Index, System::Byte Value)
			{
				if(Index < 0 || Index >= NUM_CHARACTERS)
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				if(Value) m_Native->m_Widths[Index] = Value;
			}
		}
	private:
		CBFontBitmap* m_Native;
	};


	//////////////////////////////////////////////////////////////////////////	
	public ref class WFontBitmap : public WFont
	{
	public:
		WFontBitmap() {};
		WFontBitmap(WGame^ Game);
		WFontBitmap(CBFontBitmap* Native);
		virtual ~WFontBitmap(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WFontWidthArray^ Widths
		{
			WFontWidthArray^ get()
			{
				return gcnew WFontWidthArray(Native);
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

		property int MainFrame
		{
			int get()
			{
				return Native->m_WidthsFrame;
			}
			void set(int Value)
			{
				Native->m_WidthsFrame = Value;
			}
		}

		property bool FontextFix
		{
			bool get()
			{
				return Native->m_FontextFix;
			}
			void set(bool Value)
			{
				Native->m_FontextFix = Value;
			}
		}

		property int Columns
		{
			int get()
			{
				return Native->m_NumColumns;
			}
			void set(int Value)
			{
				Native->m_NumColumns = Value;
			}
		}

		property int TileWidth
		{
			int get()
			{
				return Native->m_TileWidth;
			}
			void set(int Value)
			{
				Native->m_TileWidth = Value;
			}
		}

		property int TileHeight
		{
			int get()
			{
				return Native->m_TileHeight;
			}
			void set(int Value)
			{
				Native->m_TileHeight = Value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool GetWidths()
		{
			return SUCCEEDED(Native->GetWidths());
		}

	internal:
		property CBFontBitmap* Native
		{
			CBFontBitmap* get() { return (CBFontBitmap*)m_Native; };
		}
	};
}}}
