// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdInventoryBox : public WObject, public IEditorResizable
	{
	public:
		WAdInventoryBox(){};
		WAdInventoryBox(WGame^ Game);
		WAdInventoryBox(CAdInventoryBox* Native);
		virtual ~WAdInventoryBox(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool HideSelected
		{
			bool get()
			{
				return Native->m_HideSelected;
			}
			void set(bool Value)
			{
				Native->m_HideSelected = Value;
			}
		}

		property int ScrollBy
		{
			int get()
			{
				return Native->m_ScrollBy;
			}
			void set(int Value)
			{
				Native->m_ScrollBy = Value;
			}
		}

		property int ItemWidth
		{
			int get()
			{
				return Native->m_ItemWidth;
			}
			void set(int Value)
			{
				Native->m_ItemWidth = Value;
			}
		}

		property int ItemHeight
		{
			int get()
			{
				return Native->m_ItemHeight;
			}
			void set(int Value)
			{
				Native->m_ItemHeight = Value;
			}
		}

		property bool Visible
		{
			bool get()
			{
				return Native->m_Visible;
			}
			void set(bool Value)
			{
				Native->m_Visible = Value;
			}
		}

		property int Spacing
		{
			int get()
			{
				return Native->m_Spacing;
			}
			void set(int Value)
			{
				Native->m_Spacing = Value;
			}
		}

		property int ScrollOffset
		{
			int get()
			{
				return Native->m_ScrollOffset;
			}
			void set(int Value)
			{
				Native->m_ScrollOffset = Value;
			}
		}

		property bool Exclusive
		{
			bool get()
			{
				return Native->m_Exclusive;
			}
			void set(bool Value)
			{
				Native->m_Exclusive = Value;
			}
		}

		property System::Drawing::Rectangle Area
		{
			System::Drawing::Rectangle get()
			{
				return System::Drawing::Rectangle::FromLTRB(Native->m_ItemsArea.left, Native->m_ItemsArea.top, Native->m_ItemsArea.right, Native->m_ItemsArea.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				Native->m_ItemsArea.left = Value.Left;
				Native->m_ItemsArea.right = Value.Right;
				Native->m_ItemsArea.top = Value.Top;
				Native->m_ItemsArea.bottom = Value.Bottom;
			}
		}

		property WUIWindow^ Window
		{
			WUIWindow^ get()
			{
				if(Native->m_Window) return gcnew WUIWindow(Native->m_Window);
				else return nullptr;
			}
			void set(WUIWindow^ Value)
			{
				SAFE_DELETE(Native->m_Window);
				if(Value!=nullptr) Native->m_Window = Value->Native;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Display()
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

		bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		bool WAdInventoryBox::SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}


		//////////////////////////////////////////////////////////////////////////
		// IEditorResizable implementation
		//////////////////////////////////////////////////////////////////////////
		property int X
		{
			virtual int get() override
			{
				return this->Area.X;
			}
			virtual void set(int Value) override
			{
				this->Area = System::Drawing::Rectangle(Value, this->Area.Y, this->Area.Width, this->Area.Height);
			}
		}

		property int Y
		{
			virtual int get() override
			{
				return this->Area.Y;
			}
			virtual void set(int Value) override
			{
				this->Area = System::Drawing::Rectangle(this->Area.X, Value, this->Area.Width, this->Area.Height);
			}
		}

		property int Width
		{
			virtual int get()
			{
				return this->Area.Width;
			}
			virtual void set(int Value)
			{
				this->Area = System::Drawing::Rectangle(this->Area.X, this->Area.Y, Value, this->Area.Height);
			}
		}

		property int Height
		{
			virtual int get() override
			{
				return this->Area.Height;
			}
			virtual void set(int Value) override
			{
				this->Area = System::Drawing::Rectangle(this->Area.X, this->Area.Y, this->Area.Width, Value);
			}
		}


	internal:
		property CAdInventoryBox* Native
		{
			CAdInventoryBox* get() { return (CAdInventoryBox*)m_Native; };
		}

	};

}}};

