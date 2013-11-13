// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace System::Runtime::InteropServices;
	using namespace DeadCode::WME::Core;


	public ref class WDynBuffer : public WBase
	{
	public:
		WDynBuffer() {};
		WDynBuffer(WGame^ Game);
		WDynBuffer(CBDynBuffer* Native);
		virtual ~WDynBuffer(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property String^ Text
		{
			String^ get()
			{
				if(Native->m_Size==0) return nullptr;
				else return Marshal::PtrToStringAnsi(IntPtr(Native->m_Buffer), Native->m_Size);
			}
		}

		property int Size
		{
			int get()
			{
				return Native->m_Size;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool PutText(String^ Text, int Indent)
		{
			char* S = (char*)WUtils::GetString(Text);
			Native->PutTextIndent(Indent, S);
			WUtils::FreeString(S);
			return true;
		}

		bool PutText(String^ Text)
		{
			return PutText(Text, 0);
		}

		bool Clear()
		{
			Native->Cleanup();
			return true;
		}


	internal:
		property CBDynBuffer* Native
		{
			CBDynBuffer* get() { return (CBDynBuffer*)m_Native; };
		}
	};
}}}
