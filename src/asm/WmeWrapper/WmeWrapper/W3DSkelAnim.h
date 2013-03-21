// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WGame;

	//////////////////////////////////////////////////////////////////////////
	public ref class W3DAnimEvent
	{
	internal:
		W3DAnimEvent(C3DSkelAnim::C3DAnimEvent* Native)
		{
			m_Native = Native;
		}
	public:
		W3DAnimEvent(String^ Name, int Frame)
		{
			char* S = (char*)WUtils::GetString(Name);
			m_Native = new C3DSkelAnim::C3DAnimEvent(S, Frame);
			WUtils::FreeString(S);
		}

	private:
		C3DSkelAnim::C3DAnimEvent* m_Native;

	internal:
		property C3DSkelAnim::C3DAnimEvent* Native
		{
			C3DSkelAnim::C3DAnimEvent* get() { return m_Native; };
		}
	};


	//////////////////////////////////////////////////////////////////////////
	public ref class W3DSkelAnim : public WNamedObject
	{
	public:
		W3DSkelAnim(){};
		W3DSkelAnim(WGame^ Game);
		W3DSkelAnim(C3DSkelAnim* Native);
		virtual ~W3DSkelAnim(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		WDynArray<C3DSkelAnim::C3DAnimEvent*, W3DAnimEvent>^ get()
		{
			if(!Native) return nullptr;
			else return gcnew WDynArray<C3DSkelAnim::C3DAnimEvent*, W3DAnimEvent>(&Native->m_Events);
		}

		property double TotalTime
		{
			double get()
			{
				return Native->m_TotalTime;
			}
		}

		property double FrameTime
		{
			double get()
			{
				return Native->m_FrameTime;
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
		}

	internal:
		property C3DSkelAnim* Native
		{
			C3DSkelAnim* get() { return (C3DSkelAnim*)m_Native; };
		}

	};

}}};

