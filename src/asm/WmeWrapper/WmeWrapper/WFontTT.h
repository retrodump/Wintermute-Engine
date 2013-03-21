// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	//////////////////////////////////////////////////////////////////////////
	public ref class WFontTTLayer
	{
	public:
		WFontTTLayer()
		{
			m_Native = new CBFontTT::CBTTFontLayer();
		}
		WFontTTLayer(CBFontTT::CBTTFontLayer* Native)
		{
			m_Native = Native;
		}

		property int OffsetX
		{
			int get()
			{
				return m_Native->m_OffsetX;
			}
			void set(int Value)
			{
				m_Native->m_OffsetX = Value;
			}
		}

		property int OffsetY
		{
			int get()
			{
				return m_Native->m_OffsetY;
			}
			void set(int Value)
			{
				m_Native->m_OffsetY = Value;
			}
		}

		property System::Drawing::Color Color
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(m_Native->m_Color);
			}
			void set(System::Drawing::Color Value)
			{
				m_Native->m_Color = Value.ToArgb();
			}
		}
	internal:
		property CBFontTT::CBTTFontLayer* Native
		{
			CBFontTT::CBTTFontLayer* get()
			{
				return m_Native;
			}
		}
	private:
		CBFontTT::CBTTFontLayer* m_Native;
	};


	//////////////////////////////////////////////////////////////////////////	
	public ref class WFontTT : public WFont
	{
	public:
		WFontTT() {};
		WFontTT(WGame^ Game);
		WFontTT(CBFontTT* Native);
		virtual ~WFontTT(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CBFontTT::CBTTFontLayer*, WFontTTLayer>^ Layers
		{
			WDynArray<CBFontTT::CBTTFontLayer*, WFontTTLayer>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBFontTT::CBTTFontLayer*, WFontTTLayer>(&Native->m_Layers);
			}
		}

		property bool Bold
		{
			bool get()
			{
				return Native->m_IsBold;
			}
			void set(bool Value)
			{
				Native->m_IsBold = Value;
				Native->ClearCache();
			}
		}

		property bool Italic
		{
			bool get()
			{
				return Native->m_IsItalic;
			}
			void set(bool Value)
			{
				Native->m_IsItalic = Value;
				Native->ClearCache();
			}
		}

		property bool Underline
		{
			bool get()
			{
				return Native->m_IsUnderline;
			}
			void set(bool Value)
			{
				Native->m_IsUnderline = Value;
				Native->ClearCache();
			}
		}

		property bool Striked
		{
			bool get()
			{
				return Native->m_IsStriked;
			}
			void set(bool Value)
			{
				Native->m_IsStriked = Value;
				Native->ClearCache();
			}
		}

		property int FontHeight
		{
			int get()
			{
				return Native->m_FontHeight;
			}
			void set(int Value)
			{
				Native->m_FontHeight = Value;
				Native->ClearCache();
			}
		}

		property String^ Typeface
		{
			String^ get()
			{
				if(Native->m_Typeface) return gcnew String(Native->m_Typeface);
				else return nullptr;
			}
			void set(String^ Value)
			{
				SAFE_DELETE_ARRAY(Native->m_Typeface);
				if(Value!=nullptr)
				{
					char* S = (char*)WUtils::GetString(Value);
					CBUtils::SetString(&Native->m_Typeface, S);
					WUtils::FreeString(S);
				}
				Native->ClearCache();
			}
		}

		property String^ FontFile
		{
			String^ get()
			{
				if(Native->m_FontFile) return gcnew String(Native->m_FontFile);
				else return nullptr;
			}
			void set(String^ Value)
			{
				SAFE_DELETE_ARRAY(Native->m_FontFile);
				if(Value!=nullptr)
				{
					char* S = (char*)WUtils::GetString(Value);
					CBUtils::SetString(&Native->m_FontFile, S);
					WUtils::FreeString(S);
				}
				Native->ClearCache();
			}
		}

		property int Charset
		{
			int get()
			{
				return Native->m_Charset;
			}
			void set(int Value)
			{
				Native->m_Charset = Value;
				Native->ClearCache();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool ClearCache()
		{
			Native->ClearCache();
			return true;
		}

	internal:
		property CBFontTT* Native
		{
			CBFontTT* get() { return (CBFontTT*)m_Native; };
		}
	};
}}}
