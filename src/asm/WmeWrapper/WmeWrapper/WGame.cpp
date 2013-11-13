// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "Stdafx.h"
#include "WGame.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WGame::WGame(void)
{
	m_Native = new CBGame();
	m_SelfDestroy = true;
	m_Game = this;

	InitializeInternal();
}

//////////////////////////////////////////////////////////////////////////
WGame::WGame(CBGame* Native):WObject()
{
	m_Native = Native;
	m_SelfDestroy = false;	
	m_Game = this;

	//ThisGCHandle = GCHandle::Alloc(this);
}

//////////////////////////////////////////////////////////////////////////
WGame::~WGame()
{
	if(ThisGCHandle.IsAllocated) ThisGCHandle.Free();
	if(LogCallbackHandle.IsAllocated) LogCallbackHandle.Free();
	if(CompilerCallbackHandle.IsAllocated) CompilerCallbackHandle.Free();
	if(ElementCallbackHandle.IsAllocated) ElementCallbackHandle.Free();
}

//////////////////////////////////////////////////////////////////////////
void WGame::SetLogCallback(WEngineLogCallback^ Callback)
{
	EngineLogCallback = Callback;
	EngineLogDelegate^ Del = gcnew EngineLogDelegate(EngineLogStub);
	LogCallbackHandle = GCHandle::Alloc(Del);

	IntPtr ip = Marshal::GetFunctionPointerForDelegate(Del);

	if(!ThisGCHandle.IsAllocated) ThisGCHandle = GCHandle::Alloc(this);
	Native->SetEngineLogCallback((ENGINE_LOG_CALLBACK)ip.ToPointer(), ((IntPtr)ThisGCHandle).ToPointer());
	Native->DEBUG_DebugEnable();
}

//////////////////////////////////////////////////////////////////////////
void WGame::SetScriptCompilerCallback(WScriptCompilerCallback^ Callback)
{
	if(!Native->m_ScEngine) throw gcnew System::InvalidOperationException("Scripting engine not initialized");

	ScriptCompilerCallback = Callback;
	ScriptCompilerDelegate^ Del = gcnew ScriptCompilerDelegate(ScriptCompilerStub);
	CompilerCallbackHandle = GCHandle::Alloc(Del);

	IntPtr ip = Marshal::GetFunctionPointerForDelegate(Del);

	if(!ThisGCHandle.IsAllocated) ThisGCHandle = GCHandle::Alloc(this);
	Native->m_ScEngine->SetCompileErrorCallback((COMPILE_ERROR_CALLBACK)ip.ToPointer(), ((IntPtr)ThisGCHandle).ToPointer());
}

//////////////////////////////////////////////////////////////////////////
void WGame::SetScriptElementCallback(WScriptElementCallback^ Callback)
{
	if(!Native->m_ScEngine) throw gcnew System::InvalidOperationException("Scripting engine not initialized");

	ScriptElementCallback = Callback;
	ScriptElementDelegate^ Del = gcnew ScriptElementDelegate(ScriptElementStub);
	ElementCallbackHandle = GCHandle::Alloc(Del);

	IntPtr ip = Marshal::GetFunctionPointerForDelegate(Del);

	if(!ThisGCHandle.IsAllocated) ThisGCHandle = GCHandle::Alloc(this);
	Native->m_ScEngine->SetParseElementCallback((PARSE_ELEMENT_CALLBACK)ip.ToPointer(), ((IntPtr)ThisGCHandle).ToPointer());
}


//////////////////////////////////////////////////////////////////////////
void WGame::EngineLogStub(char* Text, HRESULT Result, void* Data)
{
	GCHandle gch = (GCHandle)IntPtr(Data);
	WGame^ _this = (WGame^)gch.Target;
	if(_this!=nullptr && _this->EngineLogCallback!=nullptr)
	{
		_this->EngineLogCallback(gcnew String(Text), (int)Result);
	}
}

//////////////////////////////////////////////////////////////////////////
void WGame::ScriptCompilerStub(int Line, char* Text , void* Data)
{
	GCHandle gch = (GCHandle)IntPtr(Data);
	WGame^ _this = (WGame^)gch.Target;
	if(_this!=nullptr && _this->ScriptCompilerCallback!=nullptr)
	{
		_this->ScriptCompilerCallback(Line, gcnew String(Text));
	}
}

//////////////////////////////////////////////////////////////////////////
void WGame::ScriptElementStub(int Line, int Type, void* ElementData , void* Data)
{
	GCHandle gch = (GCHandle)IntPtr(Data);
	WGame^ _this = (WGame^)gch.Target;
	if(_this!=nullptr && _this->ScriptElementCallback!=nullptr)
	{
		_this->ScriptElementCallback(Line, Type, gcnew String((char*)ElementData));
	}
}
