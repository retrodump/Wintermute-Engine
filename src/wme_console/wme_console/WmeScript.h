// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

using namespace System;
using namespace DeadCode::WME::Debugger;

namespace wme_console
{
	ref class WmeScript : public IWmeScript
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		WmeScript(IWmeDebugScript* Native)
		{
			m_Native = Native;
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property IntPtr NativeID
		{
			IntPtr get()
			{
				return IntPtr(m_Native);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property int Line
		{
			int get()
			{
				return m_Native->DbgGetLine();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property String^ Filename
		{
			String^ get()
			{
				return gcnew String(m_Native->DbgGetFilename());
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property EWmeScriptState State
		{
			EWmeScriptState get()
			{
				switch(m_Native->DbgGetState())
				{
					case SCRIPT_RUNNING:
						return EWmeScriptState::Running;
					case SCRIPT_WAITING:
						return EWmeScriptState::WaitingForObject;
					case SCRIPT_SLEEPING:
						return EWmeScriptState::Sleeping;
					case SCRIPT_PERSISTENT:
						return EWmeScriptState::Persistent;
					case SCRIPT_ERROR:
						return EWmeScriptState::Error;
					case SCRIPT_PAUSED:
						return EWmeScriptState::Paused;
					case SCRIPT_WAITING_SCRIPT:
						return EWmeScriptState::WaitingForScript;
					case SCRIPT_THREAD_FINISHED:
						return EWmeScriptState::ThreadFinished;
					default:
						return EWmeScriptState::Finished;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property array<int>^ BreakpointLines
		{
			array<int>^ get()
			{
				int NumBreakpoints = m_Native->DbgGetNumBreakpoints();

				array<int>^ Ret = gcnew array<int>(NumBreakpoints);
				for(int i=0; i<NumBreakpoints; i++)
				{
					Ret[i] = m_Native->DbgGetBreakpoint(i);
				}
				return Ret;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property bool TracingMode
		{
			bool get()
			{
				return m_Native->DbgGetTracingMode();
			}
			void set(bool Value)
			{
				m_Native->DbgSetTracingMode(Value);
			}
		}


	private:
		IWmeDebugScript* m_Native;
	};
}