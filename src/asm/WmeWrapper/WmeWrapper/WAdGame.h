// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WGame.h"
#include "WAdItem.h"
#include "WAdResponseBox.h"
#include "WAdInventoryBox.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdGame : public WGame
	{
	public:
		WAdGame();
		WAdGame(CAdGame* Native);
		virtual ~WAdGame(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property String^ DebugStartupScene
		{
			String^ get()
			{
				if(Native->m_DebugStartupScene) return gcnew String(Native->m_DebugStartupScene);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_DebugStartupScene, S);
				WUtils::FreeString(S);
			}
		}

		property System::Drawing::Rectangle Viewport
		{
			System::Drawing::Rectangle get()
			{
				if(!Native->m_SceneViewport) return Rectangle::Empty;
				else
				{
					RECT* rc = Native->m_SceneViewport->GetRect();
					return System::Drawing::Rectangle::FromLTRB(rc->left, rc->top, rc->right, rc->bottom);
				}
			}
			void set(System::Drawing::Rectangle Value)
			{
				if(Value==System::Drawing::Rectangle::Empty)
				{
					SAFE_DELETE(Native->m_SceneViewport);
					return;
				}
				if(!Native->m_SceneViewport) Native->m_SceneViewport = new CBViewport(Native);
				Native->m_SceneViewport->SetRect(Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property WETalkSkipButton TalkSkipButton
		{
			WETalkSkipButton get()
			{
				return (WETalkSkipButton)Native->m_TalkSkipButton;
			}
			void set(WETalkSkipButton Value)
			{
				Native->m_TalkSkipButton = (TTalkSkipButton)Value;
			}
		}

		property String^ PrevSceneName
		{
			String^ get()
			{
				if(Native->m_PrevSceneName) return gcnew String(Native->m_PrevSceneName);
				else return nullptr;
			}
		}

		property String^ PrevSceneFilename
		{
			String^ get()
			{
				if(Native->m_PrevSceneFilename) return gcnew String(Native->m_PrevSceneFilename);
				else return nullptr;
			}
		}

		property WAdItem^ SelectedItem
		{
			WAdItem^ get()
			{
				if(Native->m_SelectedItem) return gcnew WAdItem(Native->m_SelectedItem);
				else return nullptr;
			}
			void set(WAdItem^ Value)
			{
				if(Value!=nullptr) Native->m_SelectedItem = Value->Native;
				else Native->m_SelectedItem = NULL;
			}
		}

		property WAdResponseBox^ ResponseBox
		{
			WAdResponseBox^ get()
			{
				if(Native->m_ResponseBox) return gcnew WAdResponseBox(Native->m_ResponseBox);
				else return nullptr;
			}
			void set(WAdResponseBox^ Value)
			{
				SAFE_DELETE(Native->m_ResponseBox);
				if(Value!=nullptr) Native->m_ResponseBox = Value->Native;
			}
		}

		property WAdInventoryBox^ InventoryBox
		{
			WAdInventoryBox^ get()
			{
				if(Native->m_InventoryBox) return gcnew WAdInventoryBox(Native->m_InventoryBox);
				else return nullptr;
			}
			void set(WAdInventoryBox^ Value)
			{
				SAFE_DELETE(Native->m_InventoryBox);
				if(Value!=nullptr) Native->m_InventoryBox = Value->Native;
			}
		}

		property WAdScene^ Scene
		{
			WAdScene^ get()
			{
				if(Native->m_Scene) return gcnew WAdScene(Native->m_Scene);
				else return nullptr;
			}
		}

		property WDynArray<CAdItem*, WAdItem>^ Items
		{
			WDynArray<CAdItem*, WAdItem>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdItem*, WAdItem>(&Native->m_Items);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		/// Display
		///////////////////////////////////////////////////////////////////////
		bool FrameInit()
		{
			return SUCCEEDED(Native->InitLoop());
		}

		bool DisplaySentences()
		{
			return SUCCEEDED(Native->DisplaySentences(false));
		}

		virtual bool DisplayContent(bool UpdateFirst) override
		{
			return SUCCEEDED(Native->DisplayContent(UpdateFirst));
		}

		virtual bool DisplayContent() override
		{
			return DisplayContent(true);
		}

		virtual bool DisplayCursor() override
		{
			return SUCCEEDED(Native->ShowCursor());
		}


		//////////////////////////////////////////////////////////////////////////
		// Object manipulation
		//////////////////////////////////////////////////////////////////////////
		bool AddObject(WAdObject^ Object)
		{
			if(Object==nullptr) return false;
			else return SUCCEEDED(Native->AddObject(Object->Native));
		}

		bool RemoveObject(WAdObject^% Object)
		{
			if(Object==nullptr) return false;
			else
			{
				bool Ret = SUCCEEDED(Native->RemoveObject(Object->Native));
				if(Ret) Object = nullptr;
				return Ret;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Loading / saving
		//////////////////////////////////////////////////////////////////////////
		virtual bool LoadFromFile(String^ Filename)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Filename);
			Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);

			return Ret;
		}

		virtual bool LoadFromBuffer(String^ Buffer, bool Complete)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadBuffer((BYTE*)S, Complete));
			WUtils::FreeString(S);

			return Ret;
		}

		virtual bool LoadFromBuffer(String^ Buffer)
		{
			return LoadFromBuffer(Buffer, true);
		}

		bool LoadItemsFromFile(String^ Filename, bool Merge)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Filename);
			Ret = SUCCEEDED(Native->LoadItemsFile(S, Merge));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadItemsFromFile(String^ Filename)
		{
			return LoadItemsFromFile(Filename, false);
		}

		bool LoadItemsFromBuffer(String^ Buffer, bool Merge)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadItemsBuffer((BYTE*)S, Merge));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadItemsFromBuffer(String^ Buffer)
		{
			return LoadItemsFromBuffer(Buffer, false);
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool ChangeScene(String^ Filename, bool FadeIn)
		{
			char* S = (char*)WUtils::GetString(Filename);
			bool Ret = SUCCEEDED(Native->ChangeScene(S, FadeIn));
			WUtils::FreeString(S);
			return Ret;
		}

		bool ChangeScene(String^ Filename)
		{
			return ChangeScene(Filename, false);
		}


	internal:
		property CAdGame* Native
		{
			CAdGame* get() { return (CAdGame*)m_Native; };
		}

	};

}}};

