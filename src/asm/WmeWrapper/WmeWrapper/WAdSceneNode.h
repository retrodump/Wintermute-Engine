// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WAdRegion.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdSceneNode : public WObject
	{
	public:
		WAdSceneNode(){};
		WAdSceneNode(WGame^ Game);
		WAdSceneNode(CAdSceneNode* Native);
		virtual ~WAdSceneNode(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WObject^ Object
		{
			WObject^ get()
			{
				switch(Native->m_Type)
				{
					case OBJECT_ENTITY: return gcnew WAdEntity(Native->m_Entity);
					case OBJECT_REGION: return gcnew WAdRegion(Native->m_Region);
					default: return nullptr;
				}
			}
		}

		property WAdEntity^ Entity
		{
			WAdEntity^ get()
			{
				if(Native->m_Type==OBJECT_ENTITY && Native->m_Entity) return gcnew WAdEntity(Native->m_Entity);
				else return nullptr;
			}
			void set(WAdEntity^ Value)
			{
				if(Value!=nullptr) Native->SetEntity(Value->Native);
			}
		}

		property WAdRegion^ Region
		{
			WAdRegion^ get()
			{
				if(Native->m_Type==OBJECT_REGION && Native->m_Region) return gcnew WAdRegion(Native->m_Region);
				else return nullptr;
			}
			void set(WAdRegion^ Value)
			{
				if(Value!=nullptr) Native->SetRegion(Value->Native);
			}
		}

	internal:
		property CAdSceneNode* Native
		{
			CAdSceneNode* get() { return (CAdSceneNode*)m_Native; };
		}

	};

}}};
