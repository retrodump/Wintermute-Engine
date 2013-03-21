// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WUtils.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;
	using namespace System::Runtime::InteropServices;



	public ref class WAdScene : public WObject
	{
	public:
		WAdScene(){};
		WAdScene(WGame^ Game);
		WAdScene(CAdScene* Native);
		virtual ~WAdScene(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CAdLayer*, WAdLayer>^ Layers
		{
			WDynArray<CAdLayer*, WAdLayer>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdLayer*, WAdLayer>(&Native->m_Layers);
			}
		}

		property WDynArray<CAdObject*, WAdObject>^ Objects
		{
			WDynArray<CAdObject*, WAdObject>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdObject*, WAdObject>(&Native->m_Objects);
			}
		}

		property WDynArray<CAdWaypointGroup*, WAdWaypointGroup>^ WaypointGroups
		{
			WDynArray<CAdWaypointGroup*, WAdWaypointGroup>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdWaypointGroup*, WAdWaypointGroup>(&Native->m_WaypointGroups);
			}
		}

		property WDynArray<CAdScaleLevel*, WAdScaleLevel>^ ScaleLevels
		{
			WDynArray<CAdScaleLevel*, WAdScaleLevel>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdScaleLevel*, WAdScaleLevel>(&Native->m_ScaleLevels);
			}
		}

		property WDynArray<CAdRotLevel*, WAdRotLevel>^ RotationLevels
		{
			WDynArray<CAdRotLevel*, WAdRotLevel>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdRotLevel*, WAdRotLevel>(&Native->m_RotLevels);
			}
		}

		property bool PersistentState
		{
			bool get()
			{
				return Native->m_PersistentState;
			}
			void set(bool Value)
			{
				Native->m_PersistentState = Value;
			}
		}

		property bool PersistentStateSprites
		{
			bool get()
			{
				return Native->m_PersistentStateSprites;
			}
			void set(bool Value)
			{
				Native->m_PersistentStateSprites = Value;
			}
		}

		property int OffsetLeft
		{
			int get()
			{
				return Native->GetOffsetLeft();
			}
			void set(int Value)
			{
				Native->SetOffset(Value, Native->GetOffsetTop());
			}
		}

		property int OffsetTop
		{
			int get()
			{
				return Native->GetOffsetTop();
			}
			void set(int Value)
			{
				Native->SetOffset(Native->GetOffsetLeft(), Value);
			}
		}

		property System::Drawing::Rectangle Viewport
		{
			System::Drawing::Rectangle get()
			{
				if(!Native->m_Viewport) return Rectangle::Empty;
				else
				{
					RECT* rc = Native->m_Viewport->GetRect();
					return System::Drawing::Rectangle::FromLTRB(rc->left, rc->top, rc->right, rc->bottom);
				}
			}
			void set(System::Drawing::Rectangle Value)
			{
				if(Value==System::Drawing::Rectangle::Empty)
				{
					SAFE_DELETE(Native->m_Viewport);
					return;
				}
				if(!Native->m_Viewport) Native->m_Viewport = new CBViewport(Native->Game);
				Native->m_Viewport->SetRect(Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property bool ParallaxScrolling
		{
			bool get()
			{
				return Native->m_ParalaxScrolling;
			}
			void set(bool Value)
			{
				Native->m_ParalaxScrolling = Value;
			}
		}

		property bool AutoScroll
		{
			bool get()
			{
				return Native->m_AutoScroll;
			}
			void set(bool Value)
			{
				Native->m_AutoScroll = Value;
			}
		}

		property WAdLayer^ MainLayer
		{
			WAdLayer^ get()
			{
				if(Native->m_MainLayer) return gcnew WAdLayer(Native->m_MainLayer);
				else return nullptr;
			}
			void set(WAdLayer^ Value)
			{
				if(Value==nullptr)
				{
					Native->m_MainLayer = NULL;
					Native->m_Width = Native->m_Height = 0;
				}
				else
				{
					Native->m_MainLayer = Value->Native;
					Native->m_Width = Native->m_MainLayer->m_Width;
					Native->m_Height = Native->m_MainLayer->m_Height;
				}
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
			int get() new
			{
				return Native->m_Height;
			}
			void set(int Value) new
			{
				Native->m_Height = Value;
			}
		}

		property int ScrollPixelsVertical
		{
			int get()
			{
				return Native->m_ScrollPixelsV;
			}
			void set(int Value)
			{
				Native->m_ScrollPixelsV = Value;
			}
		}

		property int ScrollPixelsHorizontal
		{
			int get()
			{
				return Native->m_ScrollPixelsH;
			}
			void set(int Value)
			{
				Native->m_ScrollPixelsH = Value;
			}
		}

		property UInt32 ScrollTimeVertical
		{
			UInt32 get()
			{
				return Native->m_ScrollTimeV;
			}
			void set(UInt32 Value)
			{
				Native->m_ScrollTimeV = Value;
			}
		}

		property UInt32 ScrollTimeHorizontal
		{
			UInt32 get()
			{
				return Native->m_ScrollTimeH;
			}
			void set(UInt32 Value)
			{
				Native->m_ScrollTimeH = Value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Querying
		//////////////////////////////////////////////////////////////////////////
		array<WAdRegion^>^ GetRegionsAt(int X, int Y)
		{
			CAdRegion* Regions[MAX_NUM_REGIONS];
			Native->GetRegionsAt(X, Y, Regions, MAX_NUM_REGIONS);
			int NumRegions = 0;
			for(int i=0; i<MAX_NUM_REGIONS; i++) if(Regions[i]) NumRegions++;

			array<WAdRegion^>^ Ret = gcnew array<WAdRegion^>(NumRegions);

			NumRegions = 0;
			for(int i=0; i<MAX_NUM_REGIONS; i++)
			{
				if(Regions[i]) Ret[NumRegions] = gcnew WAdRegion(Regions[i]);
				NumRegions++;
			}

			return Ret;
		}

		float GetRotationAt(int X, int Y)
		{
			return Native->GetRotationAt(X, Y);
		}

		WObject^ GetNodeByName(String^ Name)
		{
			char* S = (char*)WUtils::GetString(Name);
			CBObject* Obj = Native->GetNodeByName(S);
			WUtils::FreeString(S);

			if(!Obj) return nullptr;
			else return WUtils::CastObject(Obj);
		}

		float GetScaleAt(int X, int Y)
		{
			return Native->GetZoomAt(X, Y);
		}

		System::Drawing::Color GetColorAt(int X, int Y, bool IndicateMisplacement)
		{
			return System::Drawing::Color::FromArgb(Native->GetAlphaAt(X, Y, IndicateMisplacement));
		}

		System::Drawing::Color GetColorAt(int X, int Y)
		{
			return GetColorAt(X, Y, false);
		}

		bool IsBlockedAt(int X, int Y, bool CheckFreeObjects, WObject^ Requester)
		{
			CBObject* Req = NULL;
			if(Requester!=nullptr) Req = Requester->Native;

			return Native->IsBlockedAt(X, Y, CheckFreeObjects, Req);
		}

		bool IsBlockedAt(int X, int Y, bool CheckFreeObjects)
		{
			return IsBlockedAt(X, Y, CheckFreeObjects, nullptr);
		}

		bool IsBlockedAt(int X, int Y)
		{
			return IsBlockedAt(X, Y, false, nullptr);
		}

		bool IsWalkableAt(int X, int Y)
		{
			return Native->IsWalkableAt(X, Y);
		}

		//////////////////////////////////////////////////////////////////////////
		// misc
		//////////////////////////////////////////////////////////////////////////
		void CorrectTargetPoint(int X, int Y, [Out]int% TargetX, [Out]int% TargetY)
		{
			int X2, Y2;
			Native->CorrectTargetPoint(X, Y, &X2, &Y2);

			TargetX = X2;
			TargetY = Y2;
		}

		System::Drawing::Point CorrectTargetPoint(System::Drawing::Point Original)
		{
			int X, Y;
			CorrectTargetPoint(Original.X, Original.Y, X, Y);
			return System::Drawing::Point(X, Y);
		}

		bool SortRotationLevels()
		{
			return SUCCEEDED(Native->SortRotLevels());
		}

		bool SortScaleLevels()
		{
			return SUCCEEDED(Native->SortScaleLevels());
		}

		virtual bool Update()
		{
			return SUCCEEDED(Native->Update());
		}

		virtual bool Display()
		{
			return SUCCEEDED(Native->Display());
		}

		bool FrameInit()
		{
			return SUCCEEDED(Native->InitLoop());
		}

		array<System::Drawing::Point>^ GetPath(System::Drawing::Point Start, System::Drawing::Point Target, WObject^ Requester)
		{
			CBObject* Req = NULL;
			if(Requester!=nullptr) Req = Requester->Native;

			CAdPath* Path = new CAdPath(Native->Game);

			Native->GetPath(CBPoint(Start.X, Start.Y), CBPoint(Target.X, Target.Y), Path, Req);

			int Counter = 0;
			while(!Path->m_Ready && Counter < 1000) Native->PathFinderStep();

			array<System::Drawing::Point>^ Ret = nullptr;
			if(Path->m_Ready)
			{
				Ret = gcnew array<System::Drawing::Point>(Path->m_Points.GetSize());
				for(int i=0; i<Path->m_Points.GetSize(); i++)
				{
					Ret[i].X = Path->m_Points[i]->x;
					Ret[i].Y = Path->m_Points[i]->y;
				}
			}

			delete Path;
			return Ret;
		}

		array<System::Drawing::Point>^ GetPath(System::Drawing::Point Start, System::Drawing::Point Target)
		{
			return GetPath(Start, Target, nullptr);
		}

		//////////////////////////////////////////////////////////////////////////
		// saving / loading
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

		virtual bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		virtual bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
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
		// 3D related
		//////////////////////////////////////////////////////////////////////////
		property float NearClippingPlane
		{
			float get()
			{
				return Native->m_NearClipPlane;
			}
			void set(float Value)
			{
				Native->m_NearClipPlane = Value;
			}
		}

		property float FarClippingPlane
		{
			float get()
			{
				return Native->m_FarClipPlane;
			}
			void set(float Value)
			{
				Native->m_FarClipPlane = Value;
			}
		}

		property float CameraFOV
		{
			float get()
			{
				return Native->m_FOV;
			}
			void set(float Value)
			{
				Native->m_FOV = Value;
			}
		}

		property bool ShowGeometry
		{
			bool get()
			{
				return Native->m_ShowGeometry;
			}
			void set(bool Value)
			{
				Native->m_ShowGeometry = Value;
			}
		}

		property WAdSceneGeometry^ Geometry
		{
			WAdSceneGeometry^ get()
			{
				if(Native->m_Geom) return gcnew WAdSceneGeometry(Native->m_Geom);
				else return nullptr;
			}
			void set(WAdSceneGeometry^ Value)
			{
				SAFE_DELETE(Native->m_Geom);
				if(Value!=nullptr) Native->m_Geom = Value->Native;
			}
		}


	internal:
		property CAdScene* Native
		{
			CAdScene* get() { return (CAdScene*)m_Native; };
		}

	};

}}};

