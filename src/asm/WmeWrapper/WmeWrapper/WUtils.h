// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WObject;
	ref class WUIObject;

	public ref class WUtils
	{
	internal:
		static const char* GetString(String^ s);
		static void FreeString(const char* s);

		static WObject^ CastObject(CBObject* Obj);
		static WUIObject^ CastUIObject(CUIObject* Obj);
	public:
		static array<System::Byte>^ CompressBuffer(array<System::Byte>^ Buffer);
		static array<System::Byte>^ DecompressBuffer(array<System::Byte>^ Buffer, int UncompressedSize);

	};
}}}

