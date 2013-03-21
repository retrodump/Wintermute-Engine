// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "StringConvertor.h"

using namespace DeadCode::WME::Debugger;
using namespace StringUtilities;

#ifdef SetProp
	#undef SetProp
#endif

ref class DebugServer : public IDebugServer
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	DebugServer(IWmeDebugServer* NativeServer, IWmeDebugClient* NativeClient)
	{
		m_NativeServer = NativeServer;
		m_NativeClient = NativeClient;
	}

	//////////////////////////////////////////////////////////////////////////
	virtual ~DebugServer(void)
	{

	}

// IDebugServer interface implementation
public:
	//////////////////////////////////////////////////////////////////////////
	virtual bool QueryData()
	{
		return m_NativeServer->QueryData(m_NativeClient);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual int GetPropInt(String^ PropName)
	{
		StringConvertor Sc(PropName);
		return m_NativeServer->GetPropInt(Sc.NativeCharPtr);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual double GetPropFloat(String^ PropName)
	{
		StringConvertor Sc(PropName);
		return m_NativeServer->GetPropFloat(Sc.NativeCharPtr);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool GetPropBool(String^ PropName)
	{
		StringConvertor Sc(PropName);
		return m_NativeServer->GetPropBool(Sc.NativeCharPtr);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual String^ GetPropString(String^ PropName)
	{
		StringConvertor Sc(PropName);
		return gcnew String(m_NativeServer->GetPropString(Sc.NativeCharPtr));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool SetProp(String^ PropName, int PropValue)
	{
		StringConvertor Sc(PropName);
		return m_NativeServer->SetProp(Sc.NativeCharPtr, PropValue);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool SetProp(String^ PropName, double PropValue)
	{
		StringConvertor Sc(PropName);
		return m_NativeServer->SetProp(Sc.NativeCharPtr, PropValue);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool SetProp(String^ PropName, String^ PropValue)
	{
		StringConvertor Sc(PropName);
		StringConvertor ScVal(PropValue);
		return m_NativeServer->SetProp(Sc.NativeCharPtr, ScVal.NativeCharPtr);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool SetProp(String^ PropName, bool PropValue)
	{
		StringConvertor Sc(PropName);
		return m_NativeServer->SetProp(Sc.NativeCharPtr, PropValue);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual String^ ResolveFilename(String^ RelativeFilename)
	{
		StringConvertor Sc(RelativeFilename);
		char AbsFilename[MAX_PATH];
		if(m_NativeServer->ResolveFilename(Sc.NativeCharPtr, AbsFilename, MAX_PATH))
		{
			return gcnew String(AbsFilename);
		}
		else return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool AddBreakpoint(String^ Filename, int Line)
	{
		StringConvertor Sc(Filename);
		return m_NativeServer->AddBreakpoint(Sc.NativeCharPtr, Line);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool RemoveBreakpoint(String^ Filename, int Line)
	{
		StringConvertor Sc(Filename);
		return m_NativeServer->RemoveBreakpoint(Sc.NativeCharPtr, Line);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool ContinueExecution()
	{
		return m_NativeServer->ContinueExecution();
	}

private:
	IWmeDebugServer* m_NativeServer;
	IWmeDebugClient* m_NativeClient;
};
