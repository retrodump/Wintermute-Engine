// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WAdItem.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdInventory : public WObject
	{
	public:
		WAdInventory(){};
		WAdInventory(WGame^ Game);
		WAdInventory(CAdInventory* Native);
		virtual ~WAdInventory(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CAdItem*, WAdItem>^ TakenItems
		{
			WDynArray<CAdItem*, WAdItem>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdItem*, WAdItem>(&Native->m_TakenItems);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool RemoveItem(String^ ItemName)
		{
			char* S = (char*)WUtils::GetString(ItemName);
			bool Ret = SUCCEEDED(Native->RemoveItem(S));
			WUtils::FreeString(S);
			return Ret;
		}

		bool RemoveItem(WAdItem^ Item)
		{
			if(Item!=nullptr) return SUCCEEDED(Native->RemoveItem(Item->Native));
			else return false;
		}

		bool WAdInventory::InsertItem(String^ ItemName, String^ InsertAfter)
		{
			char* SItemName = (char*)WUtils::GetString(ItemName);
			char* SInsertAfter = (char*)WUtils::GetString(InsertAfter);

			bool Ret = SUCCEEDED(Native->InsertItem(SItemName, SInsertAfter));

			WUtils::FreeString(SItemName);
			WUtils::FreeString(SInsertAfter);
			return Ret;
		}

		bool InsertItem(String^ ItemName)
		{
			return InsertItem(ItemName, nullptr);
		}


	internal:
		property CAdInventory* Native
		{
			CAdInventory* get() { return (CAdInventory*)m_Native; };
		}

	};

}}};

