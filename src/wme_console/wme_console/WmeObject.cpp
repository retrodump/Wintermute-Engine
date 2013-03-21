// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WmeObject.h"
#include "WmeValue.h"
#include "StringConvertor.h"

using namespace System;
using namespace DeadCode::WME::Debugger;
using namespace wme_console;
using namespace StringUtilities;

//////////////////////////////////////////////////////////////////////////
WmeObject::WmeObject(IWmeDebugObject* Native)
{
	m_Native = Native;
}

//////////////////////////////////////////////////////////////////////////
String^ WmeObject::NativeClass::get()
{
	try
	{
		return gcnew String(m_Native->DbgGetNativeClass());
	}
	catch(Exception^)
	{
		return nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////
IWmeValue^ WmeObject::GetProperty(String^ Name)
{
	try
	{
		StringConvertor Sc(Name);
		return gcnew WmeValue(m_Native->DbgGetProperty(Sc.NativeCharPtr));
	}
	catch(Exception^)
	{
		return nullptr;
	}
}
