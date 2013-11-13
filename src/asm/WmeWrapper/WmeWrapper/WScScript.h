// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WScStack.h"
#include "WScValue.h"
#include "WScriptHolder.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WScEngine;
	ref class WScriptHolder;

	public ref class WScScript : public WBase
	{
	public:
		WScScript(){};
		WScScript(CScScript* Native);
		virtual ~WScScript(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool Unbreakable
		{
			bool get()
			{
				return Native->m_Unbreakable;
			}
		}

		property bool Freezable
		{
			bool get()
			{
				return Native->m_Freezable;
			}
			void set(bool Value)
			{
				Native->m_Freezable = Value;
			}
		}

		property int TimeSlice
		{
			int get()
			{
				return Native->m_TimeSlice;
			}
			void set(int Value)
			{
				Native->m_TimeSlice = Value;
			}
		}

		property int WaitForTime
		{
			int get()
			{
				return Native->m_WaitTime;
			}
		}

		property WObject^ WaitForObject
		{
			WObject^ get()
			{
				if(Native->m_WaitObject) return WUtils::CastObject(Native->m_WaitObject);
				else return nullptr;
			}
		}

		property WScScript^ WaitForScript
		{
			WScScript^ get()
			{
				if(Native->m_WaitScript) return gcnew WScScript(Native->m_WaitScript);
				else return nullptr;
			}
		}

		property WEScriptState State
		{
			WEScriptState get()
			{
				return (WEScriptState)Native->m_State;
			}
		}

		property WScStack^ Stack
		{
			WScStack^ get()
			{
				return gcnew WScStack(Native->m_Stack);
			}
		}

		property WScStack^ CallStack
		{
			WScStack^ get()
			{
				return gcnew WScStack(Native->m_CallStack);
			}
		}

		property WScValue^ Globals
		{
			WScValue^ get()
			{
				return gcnew WScValue(Native->m_Globals);
			}
		}

		property WScEngine^ ScriptEngine
		{
			WScEngine^ get();
		}

		property int CurrentLine
		{
			int get()
			{
				return Native->m_CurrentLine;
			}
		}

		property int IP
		{
			int get()
			{
				return Native->m_IP;
			}
		}

		property String^ Filename
		{
			String^ get()
			{
				if(Native->m_Filename) return gcnew String(Native->m_Filename);
				else return nullptr;
			}
		}

		property bool IsEventThread
		{
			bool get()
			{
				return Native->m_Thread;
			}
		}

		property bool IsMethodThread
		{
			bool get()
			{
				return Native->m_MethodThread;
			}
		}

		property String^ EventMethodName
		{
			String^ get()
			{
				if(Native->m_ThreadEvent) return gcnew String(Native->m_ThreadEvent);
				else return nullptr;
			}
		}

		property WScriptHolder^ Owner
		{
			WScriptHolder^ get();
		}


		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Pause()
		{
			return SUCCEEDED(Native->Pause());
		}

		bool Resume()
		{
			return SUCCEEDED(Native->Resume());
		}

		bool Run()
		{
			return SUCCEEDED(Native->Run());
		}

		bool Finish(bool IncludingThreads)
		{
			return SUCCEEDED(Native->Finish(IncludingThreads));
		}

		bool Finish()
		{
			return Finish(false);
		}

		bool Sleep(int Duration)
		{
			return SUCCEEDED(Native->Sleep(Duration));
		}

		bool WaitFor(WObject^ Object);

		bool WaitForExclusive(WObject^ Object);

		bool CanHandleEvent(String^ EventName)
		{
			char* S = (char*)WUtils::GetString(EventName);
			bool Ret = Native->CanHandleEvent(S);
			WUtils::FreeString(S);
			return Ret;
		}

		bool CanHandleMethod(String^ MethodName)
		{
			char* S = (char*)WUtils::GetString(MethodName);
			bool Ret = Native->CanHandleMethod(S);
			WUtils::FreeString(S);
			return Ret;
		}

		WScValue^ GetVariable(String^ Name)
		{
			char* S = (char*)WUtils::GetString(Name);
			CScValue* Val = Native->GetVar(S);
			WUtils::FreeString(S);
			if(Val) return gcnew WScValue(Val);
			else return nullptr;
		}

		bool ExecuteInstruction()
		{
			return SUCCEEDED(Native->ExecuteInstruction());
		}

	internal:
		property CScScript* Native
		{
			CScScript* get() { return (CScScript*)m_Native; };
		}

	};

}}};
