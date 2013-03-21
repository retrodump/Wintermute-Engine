// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WObject.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	//////////////////////////////////////////////////////////////////////////
	public ref class WJointsArray
	{
	internal:
		WJointsArray(C3DModel* Native)
		{
			m_Native = Native;
		}
	public:
		property virtual int Count
		{
			int get()
			{
				return m_Native->m_NumJoints;
			}
		}

		property W3DJoint^ default[int]
		{
			W3DJoint^ get(int Index)
			{
				if(Index < 0 || Index >= m_Native->m_NumJoints)
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				return gcnew W3DJoint(&m_Native->m_Joints[Index]);

			}
		}
	private:
		C3DModel* m_Native;
	};


	//////////////////////////////////////////////////////////////////////////
	public ref class W3DModel : public WObject
	{
	public:
		W3DModel(){};
		W3DModel(WGame^ Game, WObject^ Owner);
		W3DModel(C3DModel* Native);
		virtual ~W3DModel(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WObject^ Owner
		{
			WObject^ get()
			{
				if(Native->m_Owner) return WUtils::CastObject(Native->m_Owner);
				else return nullptr;
			}
			void set(WObject^ Value)
			{
				if(Value==nullptr) Native->m_Owner = NULL;
				else Native->m_Owner = Value->Native;
			}
		}

		property System::Drawing::Rectangle BoundingRectangle
		{
			System::Drawing::Rectangle get()
			{
				Native->UpdateBoundingRect();
				return System::Drawing::Rectangle::FromLTRB(Native->m_BoundingRect.left, Native->m_BoundingRect.top, Native->m_BoundingRect.right, Native->m_BoundingRect.bottom);
			}
		}

		property WDynArray<C3DMesh*, W3DMesh>^ Meshes
		{
			WDynArray<C3DMesh*, W3DMesh>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<C3DMesh*, W3DMesh>(&Native->m_Meshes);
			}
		}

		property WDynArray<C3DSkelAnim*, W3DSkelAnim>^ Animations
		{
			WDynArray<C3DSkelAnim*, W3DSkelAnim>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<C3DSkelAnim*, W3DSkelAnim>(&Native->m_Animations);
			}
		}

		property WJointsArray^ Joints
		{
			WJointsArray^ get()
			{
				return gcnew WJointsArray(Native);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Animation control
		//////////////////////////////////////////////////////////////////////////
		bool SetAnimation(int Channel, String^ Name, bool ForceReset)
		{
			char* S = (char*)WUtils::GetString(Name);
			bool Ret = SUCCEEDED(Native->SetAnimation(Channel, S, ForceReset));
			WUtils::FreeString(S);
			return Ret;
		}

		bool SetAnimation(int Channel, String^ Name)
		{
			return SetAnimation(Channel, Name, false);
		}

		bool IsAnimPending(String^ AnimName)
		{
			char* S = (char*)WUtils::GetString(AnimName);
			bool Ret = Native->IsAnimPending(S);
			WUtils::FreeString(S);
			return Ret;
		}

		bool IsAnimPending()
		{
			return IsAnimPending(nullptr);
		}

		bool IsAnimPending(int Channel, String^ AnimName)
		{
			char* S = (char*)WUtils::GetString(AnimName);
			bool Ret = Native->IsAnimPending(Channel, S);
			WUtils::FreeString(S);
			return Ret;
		}

		bool IsAnimPending(int Channel)
		{
			return IsAnimPending(Channel, nullptr);
		}

		bool AnimStop()
		{
			return SUCCEEDED(Native->AnimStop());
		}

		bool AnimStop(int Channel)
		{
			return SUCCEEDED(Native->AnimStop(Channel));
		}

		//////////////////////////////////////////////////////////////////////////
		// Mesh manipulation
		//////////////////////////////////////////////////////////////////////////
		bool AddMesh(String^ Filename, String^ MeshName, String^ JointName)
		{
			char* SFilename  = (char*)WUtils::GetString(Filename);
			char* SMeshName  = (char*)WUtils::GetString(MeshName);
			char* SJointName = (char*)WUtils::GetString(JointName);

			bool Ret = SUCCEEDED(Native->AddMesh(SFilename, SMeshName, SJointName));

			WUtils::FreeString(SFilename);
			WUtils::FreeString(SMeshName);
			WUtils::FreeString(SJointName);

			return Ret;
		}

		bool ShowMesh(String^ MeshName, bool Show)
		{
			char* S = (char*)WUtils::GetString(MeshName);
			bool Ret = SUCCEEDED(Native->ShowMesh(S, Show));
			WUtils::FreeString(S);
			return Ret;
		}

		bool ShowMesh(String^ MeshName)
		{
			return ShowMesh(MeshName, true);
		}

		bool IsMeshVisible(String^ MeshName)
		{
			char* S = (char*)WUtils::GetString(MeshName);
			bool Ret = Native->IsMeshVisible(S);
			WUtils::FreeString(S);
			return Ret;
		}

		bool DeleteMesh(String^ MeshName)
		{
			char* S = (char*)WUtils::GetString(MeshName);
			bool Ret = SUCCEEDED(Native->DeleteMesh(S));
			WUtils::FreeString(S);
			return Ret;
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool IsTransparentAt(int X, int Y)
		{
			return Native->IsTransparentAt(X, Y);
		}

		bool Render()
		{
			return SUCCEEDED(Native->Render());
		}

		bool Update()
		{
			return SUCCEEDED(Native->Update());
		}

		bool LoadFromFile(String^ Filename)
		{
			char* S = (char*)WUtils::GetString(Filename);
			bool Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);
			return Ret;
		}


	internal:
		property C3DModel* Native
		{
			C3DModel* get() { return (C3DModel*)m_Native; };
		}

	};

}}};

