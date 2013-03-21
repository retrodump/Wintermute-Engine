// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WUtils.h"
#include "WAdGame.h"

using namespace System;
using System::Runtime::InteropServices::Marshal;
using namespace DeadCode::WME::Core;

//////////////////////////////////////////////////////////////////////////	
const char* WUtils::GetString(String^ s)
{
	if(s==nullptr) return 0;

	IntPtr ip = Marshal::StringToHGlobalAnsi(s);
	return static_cast<const char*>(ip.ToPointer());
}

//////////////////////////////////////////////////////////////////////////
void WUtils::FreeString(const char* s)
{
	if(!s) return;

	IntPtr ip = static_cast<IntPtr>(const_cast<void*>(static_cast<const void*>(s)));
	Marshal::FreeHGlobal(ip);
}


//////////////////////////////////////////////////////////////////////////
WObject^ WUtils::CastObject(CBObject* Obj)
{
	if(!Obj) return nullptr;

	if     (strcmp(Obj->GetClassName(), "CBGame")==0)				return gcnew WGame((CBGame*)Obj);
	else if(strcmp(Obj->GetClassName(), "CBRegion")==0)				return gcnew WRegion((CBRegion*)Obj);
	else if(strcmp(Obj->GetClassName(), "CBFont")==0)				return gcnew WFont((CBFont*)Obj);
	else if(strcmp(Obj->GetClassName(), "CBFontBitmap")==0)			return gcnew WFontBitmap((CBFontBitmap*)Obj);
	else if(strcmp(Obj->GetClassName(), "CBFontTT")==0)				return gcnew WFontTT((CBFontTT*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIButton")==0)			return gcnew WUIButton((CUIButton*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIEdit")==0)				return gcnew WUIEdit((CUIEdit*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIText")==0)				return gcnew WUIStatic((CUIText*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIWindow")==0)			return gcnew WUIWindow((CUIWindow*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIEntity")==0)			return gcnew WUIEntity((CUIEntity*)Obj);

	else if(strcmp(Obj->GetClassName(), "CAdGame")==0)				return gcnew WAdGame((CAdGame*)Obj);	
	else if(strcmp(Obj->GetClassName(), "CAdActor")==0)				return gcnew WAdActor((CAdActor*)Obj);	
	else if(strcmp(Obj->GetClassName(), "CAdActor3D")==0)			return gcnew WAdActor3D((CAdActor3D*)Obj);	
	else if(strcmp(Obj->GetClassName(), "CAdEntity")==0)			return gcnew WAdEntity((CAdEntity*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdInventory")==0)			return gcnew WAdInventory((CAdInventory*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdInventoryBox")==0)		return gcnew WAdInventoryBox((CAdInventoryBox*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdItem")==0)				return gcnew WAdItem((CAdItem*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdLayer")==0)				return gcnew WAdLayer((CAdLayer*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdRegion")==0)			return gcnew WAdRegion((CAdRegion*)Obj);	
	else if(strcmp(Obj->GetClassName(), "CAdResponse")==0)			return gcnew WAdResponse((CAdResponse*)Obj);	
	else if(strcmp(Obj->GetClassName(), "CAdResponseBox")==0)		return gcnew WAdResponseBox((CAdResponseBox*)Obj);	
	else if(strcmp(Obj->GetClassName(), "CAdRotLevel")==0)			return gcnew WAdRotLevel((CAdRotLevel*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdScaleLevel")==0)		return gcnew WAdScaleLevel((CAdScaleLevel*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdScene")==0)				return gcnew WAdScene((CAdScene*)Obj);
	else if(strcmp(Obj->GetClassName(), "CAdWaypointGroup")==0)		return gcnew WAdWaypointGroup((CAdWaypointGroup*)Obj);

	else return gcnew WObject(Obj);
}

//////////////////////////////////////////////////////////////////////////
WUIObject^ WUtils::CastUIObject(CUIObject* Obj)
{
	if(!Obj) return nullptr;

	if     (strcmp(Obj->GetClassName(), "CUIButton")==0)			return gcnew WUIButton((CUIButton*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIEdit")==0)				return gcnew WUIEdit((CUIEdit*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIText")==0)				return gcnew WUIStatic((CUIText*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIWindow")==0)			return gcnew WUIWindow((CUIWindow*)Obj);
	else if(strcmp(Obj->GetClassName(), "CUIEntity")==0)			return gcnew WUIEntity((CUIEntity*)Obj);

	else return gcnew WUIObject(Obj);
}

//////////////////////////////////////////////////////////////////////////
array<System::Byte>^ WUtils::CompressBuffer(array<System::Byte>^ Buffer)
{
	DWORD UncompSize = Buffer->Length;
	DWORD CompSize = UncompSize * 2;

	BYTE* UncompBuffer = new BYTE[UncompSize];
	if(!UncompBuffer) return nullptr;

	BYTE* CompBuffer = new BYTE[CompSize];
	if(!CompBuffer)
	{
		delete [] UncompBuffer;
		return nullptr;
	}

	Marshal::Copy(Buffer, 0, IntPtr(UncompBuffer), UncompSize);

	array<System::Byte>^ Ret;
	if(Z_OK!=compress(CompBuffer, &CompSize, UncompBuffer, UncompSize))
	{
		Ret = nullptr;
	}
	else
	{
		Ret = gcnew array<System::Byte>(CompSize);
		Marshal::Copy(IntPtr(CompBuffer), Ret, 0, CompSize);
	}

	delete [] UncompBuffer;
	delete [] CompBuffer;

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
array<System::Byte>^ WUtils::DecompressBuffer(array<System::Byte>^ Buffer, int UncompressedSize)
{
	DWORD CompSize = Buffer->Length;
	DWORD UncompSize = UncompressedSize;

	BYTE* UncompBuffer = new BYTE[UncompSize];
	if(!UncompBuffer) return nullptr;

	BYTE* CompBuffer = new BYTE[CompSize];
	if(!CompBuffer)
	{
		delete [] UncompBuffer;
		return nullptr;
	}

	Marshal::Copy(Buffer, 0, IntPtr(CompBuffer), CompSize);

	array<System::Byte>^ Ret;
	if(Z_OK!=uncompress(UncompBuffer, &UncompSize, CompBuffer, CompSize))
	{
		Ret = nullptr;
	}
	else
	{
		Ret = gcnew array<System::Byte>(UncompSize);
		Marshal::Copy(IntPtr(UncompBuffer), Ret, 0, UncompSize);
	}

	delete [] UncompBuffer;
	delete [] CompBuffer;

	return Ret;
}
