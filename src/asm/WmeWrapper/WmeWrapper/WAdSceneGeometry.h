// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "W3DLight.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;
	using namespace System::Runtime::InteropServices;


	public ref class WAdSceneGeometry : public WObject
	{
	public:
		WAdSceneGeometry(){};
		WAdSceneGeometry(WGame^ Game);
		WAdSceneGeometry(CAdSceneGeometry* Native);
		virtual ~WAdSceneGeometry(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WSprite^ WaypointMarker
		{
			WSprite^ get()
			{
				if(Native->m_WptMarker) return gcnew WSprite(Native->m_WptMarker);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_WptMarker);
				if(Value!=nullptr) Native->m_WptMarker = Value->Native;
			}
		}

		property float WaypointHeight
		{
			float get()
			{
				return Native->m_WaypointHeight;
			}
			void set(float Value)
			{
				Native->m_WaypointHeight = Value;
			}
		}

		property W3DCamera^ ActiveCamera
		{
			W3DCamera^ get()
			{
				if(Native->GetActiveCamera()) return gcnew W3DCamera(Native->GetActiveCamera());
				else return nullptr;
			}
		}

		property WDynArray<CAdWalkplane*, WAdWalkplane>^ Walkplanes
		{
			WDynArray<CAdWalkplane*, WAdWalkplane>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdWalkplane*, WAdWalkplane>(&Native->m_Planes);
			}
		}

		property WDynArray<CAdBlock*, WAdBlock>^ Blocks
		{
			WDynArray<CAdBlock*, WAdBlock>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdBlock*, WAdBlock>(&Native->m_Blocks);
			}
		}

		property WDynArray<C3DCamera*, W3DCamera>^ Cameras
		{
			WDynArray<C3DCamera*, W3DCamera>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<C3DCamera*, W3DCamera>(&Native->m_Cameras);
			}
		}

		property WDynArray<C3DLight*, W3DLight>^ Lights
		{
			WDynArray<C3DLight*, W3DLight>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<C3DLight*, W3DLight>(&Native->m_Lights);
			}
		}

		property WDynArray<CAdWaypointGroup3D*, WAdWaypointGroup3D>^ WaypointGroups
		{
			WDynArray<CAdWaypointGroup3D*, WAdWaypointGroup3D>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdWaypointGroup3D*, WAdWaypointGroup3D>(&Native->m_WaypointGroups);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool DropWaypoints()
		{
			return SUCCEEDED(Native->DropWaypoints());
		}

		bool SetLightColor(String^ LightName, System::Drawing::Color Color)
		{
			char* S = (char*)WUtils::GetString(LightName);
			bool Ret = SUCCEEDED(Native->SetLightColor(S, Color.ToArgb()));
			WUtils::FreeString(S);
			return Ret;
		}

		bool EnableNode(String^ NodeName, bool Enable)
		{
			char* S = (char*)WUtils::GetString(NodeName);
			bool Ret = SUCCEEDED(Native->EnableNode(S, Enable));
			WUtils::FreeString(S);
			return Ret;
		}

		bool EnableNode(String^ NodeName)
		{
			return EnableNode(NodeName, true);
		}

		bool EnableLight(String^ LightName, bool Enable)
		{
			char* S = (char*)WUtils::GetString(LightName);
			bool Ret = SUCCEEDED(Native->EnableLight(S, Enable));
			WUtils::FreeString(S);
			return Ret;
		}

		bool EnableLight(String^ LightName)
		{
			return EnableNode(LightName, true);
		}

		bool CorrectTargetPoint(WVector3^ Source, [Out]WVector3^ Target)
		{
			D3DXVECTOR3 vSource(Source->X, Source->Y, Source->Z);
			D3DXVECTOR3 vTarget;

			bool Ret = SUCCEEDED(Native->CorrectTargetPoint(vSource, &vTarget));
			Target = gcnew WVector3(vTarget.x, vTarget.y, vTarget.z);

			return Ret;
		}

		bool CreateLights()
		{
			return SUCCEEDED(Native->CreateLights());
		}

		array<WVector3^>^ GetPath(WVector3^ Start, WVector3^ Target)
		{
			CAdPath3D* Path = new CAdPath3D(Native->Game);

			Native->GetPath(D3DXVECTOR3(Start->X, Start->Y, Start->Z), D3DXVECTOR3(Target->X, Target->Y, Target->Z), Path);

			int Counter = 0;
			while(!Path->m_Ready && Counter < 1000) Native->PathFinderStep();

			array<WVector3^>^ Ret = nullptr;
			if(Path->m_Ready)
			{
				Ret = gcnew array<WVector3^>(Path->m_Points.GetSize());
				for(int i=0; i<Path->m_Points.GetSize(); i++)
				{
					Ret[i] = gcnew WVector3(Path->m_Points[i]->x, Path->m_Points[i]->y, Path->m_Points[i]->z);
				}
			}

			delete Path;
			return Ret;
		}

		WVector3^ Convert2Dto3D(int X, int Y)
		{
			D3DXVECTOR3 v;
			if(Native->Convert2Dto3D(X, Y, &v))
			{
				return gcnew WVector3(v.x, v.y, v.z);
			}
			else return nullptr;
		}

		WVector3^ Convert2Dto3DTolerant(int X, int Y)
		{
			D3DXVECTOR3 v;
			if(Native->Convert2Dto3DTolerant(X, Y, &v))
			{
				return gcnew WVector3(v.x, v.y, v.z);
			}
			else return nullptr;
		}

		bool Render(bool Render)
		{
			return SUCCEEDED(Native->Render(Render));
		}

		bool Render()
		{
			return Render(true);
		}

		bool SetActiveCamera(String^ CameraName, float FOV, float NearClipPlane, float FarClipPlane)
		{
			char* S = (char*)WUtils::GetString(CameraName);
			bool Ret = SUCCEEDED(Native->SetActiveCamera(S, FOV, NearClipPlane, FarClipPlane));
			WUtils::FreeString(S);
			return Ret;
		}

		bool SetActiveCamera(int CameraIndex, float FOV, float NearClipPlane, float FarClipPlane)
		{
			return SUCCEEDED(Native->SetActiveCamera(CameraIndex, FOV, NearClipPlane, FarClipPlane));
		}

		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		bool LoadFromFile(String^ Filename)
		{
			char* S = (char*)WUtils::GetString(Filename);
			bool Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);
			return Ret;
		}


	internal:
		property CAdSceneGeometry* Native
		{
			CAdSceneGeometry* get() { return (CAdSceneGeometry*)m_Native; };
		}

	};

}}};

