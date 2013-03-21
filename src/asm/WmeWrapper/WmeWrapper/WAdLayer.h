// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WAdSceneNode.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdLayer : public WObject
	{
	public:
		WAdLayer(){};
		WAdLayer(WGame^ Game);
		WAdLayer(CAdLayer* Native);
		virtual ~WAdLayer(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CAdSceneNode*, WAdSceneNode>^ Nodes
		{
			WDynArray<CAdSceneNode*, WAdSceneNode>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdSceneNode*, WAdSceneNode>(&Native->m_Nodes);
			}
		}

		property bool CloseUp
		{
			bool get()
			{
				return Native->m_CloseUp;
			}
			void set(bool Value)
			{
				Native->m_CloseUp = Value;
			}
		}

		property bool Active
		{
			bool get()
			{
				return Native->m_Active;
			}
			void set(bool Value)
			{
				Native->m_Active = Value;
			}
		}

		property int Width
		{
			int get()
			{
				return Native->m_Width;
			}
			void set(int Value)
			{
				Native->m_Width = Value;
			}
		}

		property int Height
		{
			virtual int get() new
			{
				return Native->m_Height;
			}
			virtual void set(int Value) new
			{
				Native->m_Height = Value;
			}
		}

		property bool Main
		{
			bool get()
			{
				return Native->m_Main;
			}
			void set(bool Value)
			{
				Native->m_Main = Value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		bool LoadFromBuffer(String^ Buffer, bool Complete)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadBuffer((BYTE*)S, Complete));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadFromBuffer(String^ Buffer)
		{
			return LoadFromBuffer(Buffer, true);
		}

		bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}


	internal:
		property CAdLayer* Native
		{
			CAdLayer* get() { return (CAdLayer*)m_Native; };
		}

	};

}}};

