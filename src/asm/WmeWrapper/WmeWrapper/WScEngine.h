// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"
#include "WBase.h"
#include "WScScript.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;
	using namespace System::Runtime::InteropServices;

	public ref class WScEngine : public WBase
	{
	public:
		WScEngine(){};
		WScEngine(WGame^ Game);
		WScEngine(CScEngine* Native);
		virtual ~WScEngine(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CScScript*, WScScript>^ Scripts
		{
			WDynArray<CScScript*, WScScript>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CScScript*, WScScript>(&Native->m_Scripts);
			}
		}

		property bool CompilerAvailable
		{
			bool get()
			{
				return Native->m_CompilerAvailable;
			}
		}

		property WScValue^ Globals
		{
			WScValue^ get()
			{
				if(Native->m_Globals) return gcnew WScValue(Native->m_Globals);
				else return nullptr;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		array<System::Byte>^ CompileScript(System::String^ Filename)
		{
			return CompileScript(Filename, false);
		}

		array<System::Byte>^ WScEngine::CompileScript(String^ Filename, bool IgnoreCache)
		{
			char* S = (char*)WUtils::GetString(Filename);
			DWORD Size;
			BYTE* Buffer = Native->GetCompiledScript(S, &Size, IgnoreCache);
			WUtils::FreeString(S);
			if(Size==0) return nullptr;

			array<System::Byte>^ Ret = gcnew array<System::Byte>(Size);
			Marshal::Copy(IntPtr(Buffer), Ret, 0, Size);
			return Ret;
		}

		bool EmptyScriptCache()
		{
			return SUCCEEDED(Native->EmptyScriptCache());
		}

		bool Tick()
		{
			return SUCCEEDED(Native->Tick());
		}

		bool TickUnbreakable()
		{
			return SUCCEEDED(Native->TickUnbreakable());
		}

		bool ClearGlobals(bool IncludingNatives)
		{
			return SUCCEEDED(Native->ClearGlobals(IncludingNatives));
		}

		bool ClearGlobals()
		{
			return ClearGlobals(false);
		}

		bool RemoveFinishedScripts()
		{
			return SUCCEEDED(Native->RemoveFinishedScripts());
		}

		bool PauseAllScripts()
		{
			return SUCCEEDED(Native->PauseAll());
		}

		bool ResumeAllScripts()
		{
			return SUCCEEDED(Native->ResumeAll());
		}

		bool EditorCleanup()
		{
			Native->EditorCleanup();
			return true;
		}

		bool ResetObject(WObject^ Object)
		{
			if(Object==nullptr) return false;
			else return SUCCEEDED(Native->ResetObject(Object->Native));
		}

		int GetNumScripts([Out]int% Running, [Out]int% Waiting, [Out]int% Persistent)
		{
			int NumRunning, NumWaiting, NumPersistent;
			int Ret = Native->GetNumScripts(&NumRunning, &NumWaiting, &NumPersistent);
			Running = NumRunning;
			Waiting = NumWaiting;
			Persistent = NumPersistent;

			return Ret;
		}

		WScScript^ WScEngine::RunScript(String^ Filename, WObject^ Owner)
		{
			char* S = (char*)WUtils::GetString(Filename);
			CBObject* Obj = NULL;
			if(Owner!=nullptr) Obj = Owner->Native;

			CScScript* Scr = Native->RunScript(S, Obj);
			WUtils::FreeString(S);

			if(Scr) return gcnew WScScript(Scr);
			else return nullptr;
		}

		WScScript^ RunScript(String^ Filename)
		{
			return RunScript(Filename, nullptr);
		}


	internal:
		property CScEngine* Native
		{
			CScEngine* get() { return (CScEngine*)m_Native; };
		}

	};

}}};

