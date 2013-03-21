// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "Interfaces.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace System::Drawing;
	using namespace DeadCode::WME::Core;

	ref class WGame;
	ref class WSprite;
	ref class W3DModel;


	public ref class WCaptionArray
	{
	private:
		CBObject* m_Native;
	internal:
		WCaptionArray(CBObject* Native)
		{
			m_Native = Native;
		}
	public:
		property String^ default[int]
		{
			String^ get(int Index)
			{
				if(Index < 0 || Index >= 7)
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				if(m_Native->m_Caption[Index]) return gcnew String(m_Native->m_Caption[Index]);
				else return nullptr;
			}

			void set(int Index, String^ Value)
			{
				if(Index < 0 || Index >= 7)
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&m_Native->m_Caption[Index], S);
				WUtils::FreeString(S);
			}
		} 
	};



	public ref class WObject : public WScriptHolder, public IEditorMovable
	{
	public:
		WObject(){};
		WObject(WGame^ Game);
		WObject(CBObject* Native);
		virtual ~WObject(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property String^ NativeClass
		{
			String^ get()
			{
				return gcnew String(Native->GetClassName());
			}
		}

		property bool CustomRotation
		{
			bool get()
			{
				return Native->m_RotateValid;
			}
			void set(bool Value)
			{
				Native->m_RotateValid = Value;
			}
		}

		property float Rotation
		{
			float get()
			{
				return Native->m_Rotate;
			}
			void set(float Value)
			{
				Native->m_Rotate = Value;
			}
		}

		property float RelativeRotation
		{
			float get()
			{
				return Native->m_RelativeRotate;
			}
			void set(float Value)
			{
				Native->m_RelativeRotate = Value;
			}
		}

		property bool Rotatable
		{
			bool get()
			{
				return Native->m_Rotatable;
			}
			void set(bool Value)
			{
				Native->m_Rotatable = Value;
			}
		}

		property System::Drawing::Color Color
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_AlphaColor);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_AlphaColor = Value.ToArgb();
			}
		}

		property float Scale
		{
			float get()
			{
				return Native->m_Scale;
			}
			void set(float Value)
			{
				Native->m_Scale = Value;
			}
		}

		property float ScaleX
		{
			float get()
			{
				return Native->m_ScaleX;
			}
			void set(float Value)
			{
				Native->m_ScaleX = Value;
			}
		}

		property float ScaleY
		{
			float get()
			{
				return Native->m_ScaleY;
			}
			void set(float Value)
			{
				Native->m_ScaleY = Value;
			}
		}

		property float RelativeScale
		{
			float get()
			{
				return Native->m_RelativeScale;
			}
			void set(float Value)
			{
				Native->m_RelativeScale = Value;
			}
		}

		property bool Ready
		{
			bool get()
			{
				return Native->m_Ready;
			}
		}

		property String^ SoundEvent
		{
			String^ get()
			{
				if(Native->m_SoundEvent) return gcnew String(Native->m_SoundEvent);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				Native->SetSoundEvent(S);
				WUtils::FreeString(S);
			}
		}

		property bool AutoSoundPanning
		{
			bool get()
			{
				return Native->m_AutoSoundPanning;
			}
			void set(bool Value)
			{
				Native->m_AutoSoundPanning = Value;
			}
		}

		property int SoundStartPos
		{
			int get()
			{
				return Native->m_SFXStart;
			}
			void set(int Value)
			{
				Native->SetSFXTime(Value);
			}
		}

		property int SoundVolume
		{
			int get()
			{
				return Native->m_SFXVolume;
			}
			void set(int Value)
			{
				Native->SetSFXVolume(Value);
			}
		}

		property int Height
		{
			virtual int get()
			{
				return Native->GetHeight();
			}
			virtual void set(int Value)
			{
			}
		}

		property bool SharedCursors
		{
			bool get()
			{
				return Native->m_SharedCursors;
			}
			void set(bool Value)
			{
				Native->m_SharedCursors = Value;
			}
		}

		property WSprite^ Cursor
		{
			WSprite^ get();
			void set(WSprite^ Value);
		}

		property WSprite^ ActiveCursor
		{
			WSprite^ get();
			void set(WSprite^ Value);
		}

		property String^ Caption
		{
			String^ get()
			{
				return gcnew String(Native->GetCaption());
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				Native->SetCaption(S);
				WUtils::FreeString(S);
			}
		}

		property WCaptionArray^ Captions
		{
			WCaptionArray^ get()
			{
				return gcnew WCaptionArray(Native);
			}
		}

		property bool EditorAlwaysInteractive
		{
			bool get()
			{
				return Native->m_EditorAlwaysRegister;
			}
			void set(bool Value)
			{
				Native->m_EditorAlwaysRegister = Value;
			}
		}

		property bool DisplayInEditorOnly
		{
			bool get()
			{
				return Native->m_EditorOnly;
			}
			void set(bool Value)
			{
				Native->m_EditorOnly = Value;
			}
		}

		property bool Is3D
		{
			bool get()
			{
				return Native->m_Is3D;
			}
			void set(bool Value)
			{
				Native->m_Is3D = Value;
			}
		}

		property bool Interactive
		{
			bool get()
			{
				return Native->m_Registrable;
			}
			void set(bool Value)
			{
				Native->m_Registrable = Value;
			}
		}

		property bool Scalable
		{
			bool get()
			{
				return Native->m_Zoomable;
			}
			void set(bool Value)
			{
				Native->m_Zoomable = Value;
			}
		}

		property bool Colorable
		{
			bool get()
			{
				return Native->m_Shadowable;
			}
			void set(bool Value)
			{
				Native->m_Shadowable = Value;
			}
		}

		property bool Movable
		{
			bool get()
			{
				return Native->m_Movable;
			}
			void set(bool Value)
			{
				Native->m_Movable = Value;
			}
		}

		property bool SaveState
		{
			bool get()
			{
				return Native->m_SaveState;
			}
			void set(bool Value)
			{
				Native->m_SaveState = Value;
			}
		}

		property int X
		{
			virtual int get()
			{
				return Native->m_PosX;
			}
			virtual void set(int Value)
			{
				Native->m_PosX = Value;
			}
		}

		property int Y
		{
			virtual int get()
			{
				return Native->m_PosY;
			}
			virtual void set(int Value)
			{
				Native->m_PosY = Value;
			}
		}

		property bool NonIntMouseEvents
		{
			bool get()
			{
				return Native->m_NonIntMouseEvents;
			}
			void set(bool Value)
			{
				Native->m_NonIntMouseEvents = Value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 3D related
		//////////////////////////////////////////////////////////////////////////
		property float Angle
		{
			float get()
			{
				return Native->m_Angle;
			}
			void set(float Value)
			{
				Native->m_Angle = Value;
			}
		}

		property WVector3^ PostitionVector
		{
			WVector3^ get()
			{
				return gcnew WVector3(Native->m_PosVector.x, Native->m_PosVector.y, Native->m_PosVector.z);
			}
			void set(WVector3^ Value)
			{
				Native->m_PosVector.x = Value->X;
				Native->m_PosVector.y = Value->Y;
				Native->m_PosVector.z = Value->Z;
			}
		}

		property WVector3^ LightPostitionVector
		{
			WVector3^ get()
			{
				return gcnew WVector3(Native->m_ShadowLightPos.x, Native->m_ShadowLightPos.y, Native->m_ShadowLightPos.z);
			}
			void set(WVector3^ Value)
			{
				Native->m_ShadowLightPos.x = Value->X;
				Native->m_ShadowLightPos.y = Value->Y;
				Native->m_ShadowLightPos.z = Value->Z;
			}
		}

		property WEShadowType ShadowType
		{
			WEShadowType get()
			{
				return (WEShadowType)Native->m_ShadowType;
			}
			void set(WEShadowType Value)
			{
				Native->m_ShadowType = (TShadowType)Value;
			}
		}

		property System::Drawing::Color ShadowColor
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_ShadowColor);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_ShadowColor = Value.ToArgb();
			}
		}

		property String^ ShadowImage
		{
			String^ get()
			{
				if(Native->m_ShadowImage && Native->m_ShadowImage->m_Filename)
				{
					return gcnew String(Native->m_ShadowImage->m_Filename);
				}
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);

				if(Native->m_ShadowImage) Native->Game->m_SurfaceStorage->RemoveSurface(Native->m_ShadowImage);
				Native->m_ShadowImage = NULL;

				if(S) Native->m_ShadowImage = Native->Game->m_SurfaceStorage->AddSurface(S);

				WUtils::FreeString(S);
			}
		}

		property float ShadowSize
		{
			float get()
			{
				return Native->m_ShadowSize;
			}
			void set(float Value)
			{
				Native->m_ShadowSize = Value;
			}
		}

		property float Scale3D
		{
			float get()
			{
				return Native->m_Scale3D;
			}
			void set(float Value)
			{
				Native->m_Scale3D = Value;
			}
		}

		property W3DModel^ Model
		{
			W3DModel^ get();
			void set(W3DModel^ Value);
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool UpdateSounds()
		{
			return SUCCEEDED(Native->UpdateSounds());
		}

		bool PlaySound(String^ Filename, bool Looping, bool PlayNow, String^ EventName, UInt32 LoopStart)
		{
			char* SFilename  = (char*)WUtils::GetString(Filename);
			char* SEventName = (char*)WUtils::GetString(EventName);

			bool Ret = SUCCEEDED(Native->PlaySFX(SFilename, Looping, PlayNow, SEventName, LoopStart));

			WUtils::FreeString(SFilename);
			WUtils::FreeString(SEventName);

			return Ret;
		}

		bool PlaySound(String^ Filename, bool Looping, bool PlayNow, String^ EventName)
		{
			return PlaySound(Filename, Looping, PlayNow, EventName, 0);
		}

		bool PlaySound(String^ Filename, bool Looping, bool PlayNow)
		{
			return PlaySound(Filename, Looping, PlayNow, nullptr);
		}

		bool PlaySound(String^ Filename, bool Looping)
		{
			return PlaySound(Filename, Looping, true);
		}

		bool PlaySound(String^ Filename)
		{
			return PlaySound(Filename, false);
		}

		bool PlaySound()
		{
			return PlaySound(nullptr);
		}

		bool StopSound()
		{
			return SUCCEEDED(Native->StopSFX());
		}

		bool PauseSound()
		{
			return SUCCEEDED(Native->PauseSFX());
		}

		bool ResumeSound()
		{
			return SUCCEEDED(Native->ResumeSFX());
		}

		//////////////////////////////////////////////////////////////////////////
		// editor props
		//////////////////////////////////////////////////////////////////////////
		property System::Drawing::Point OrigPos
		{
			virtual System::Drawing::Point get()
			{
				int OrigX;
				int::TryParse(GetEditorProp("OrigX", "0"), OrigX);

				int OrigY;
				int::TryParse(GetEditorProp("OrigY", "0"), OrigY);

				return System::Drawing::Point(OrigX, OrigY);
			}
			
			virtual void set(System::Drawing::Point Value)
			{
				SetEditorProp("OrigX", Value.X.ToString());
				SetEditorProp("OrigY", Value.Y.ToString());
			}
		}

		property System::Drawing::Rectangle OrigRect
		{
			virtual System::Drawing::Rectangle get()
			{
				int OrigX;
				int::TryParse(GetEditorProp("OrigX", "0"), OrigX);

				int OrigY;
				int::TryParse(GetEditorProp("OrigY", "0"), OrigY);

				int OrigWidth;
				int::TryParse(GetEditorProp("OrigWidth", "0"), OrigWidth);

				int OrigHeight;
				int::TryParse(GetEditorProp("OrigHeight", "0"), OrigHeight);

				return System::Drawing::Rectangle(OrigX, OrigY, OrigWidth, OrigHeight);
			}
			virtual void set(System::Drawing::Rectangle Value)
			{
				SetEditorProp("OrigX", Value.X.ToString());
				SetEditorProp("OrigY", Value.Y.ToString());
				SetEditorProp("OrigWidth", Value.Width.ToString());
				SetEditorProp("OrigHeight", Value.Height.ToString());
			}
		}

		property bool EditorSelected
		{
			virtual bool get()
			{
				bool Ret;
				bool::TryParse(GetEditorProp("Selected", "no"), Ret);
				return Ret;
			}
			virtual void set(bool Value)
			{
				SetEditorProp("Selected", Value.ToString());
			}
		}

	internal:
		property CBObject* Native
		{
			CBObject* get() { return (CBObject*)m_Native; };
		}

	};

}}};

