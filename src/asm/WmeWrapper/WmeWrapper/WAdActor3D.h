// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdActor3D : public WAdObject3D
	{
	public:
		WAdActor3D(){};
		WAdActor3D(WGame^ Game);
		WAdActor3D(CAdActor3D* Native);
		virtual ~WAdActor3D(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property float AfterWalkAngle
		{
			float get()
			{
				return Native->m_AfterWalkAngle;
			}
			void set(float Value)
			{
				Native->m_AfterWalkAngle = Value;
			}
		}

		property String^ TalkAnimName
		{
			String^ get()
			{
				if(Native->m_TalkAnimName) return gcnew String(Native->m_TalkAnimName);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_TalkAnimName, S);
				WUtils::FreeString(S);
			}
		}

		property String^ IdleAnimName
		{
			String^ get()
			{
				if(Native->m_IdleAnimName) return gcnew String(Native->m_IdleAnimName);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_IdleAnimName, S);
				WUtils::FreeString(S);
			}
		}

		property String^ WalkAnimName
		{
			String^ get()
			{
				if(Native->m_WalkAnimName) return gcnew String(Native->m_WalkAnimName);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_WalkAnimName, S);
				WUtils::FreeString(S);
			}
		}

		property WEDirectWalkMode DirectWalkMode
		{
			WEDirectWalkMode get()
			{
				return (WEDirectWalkMode)Native->m_DirectWalkMode;
			}
			void set(WEDirectWalkMode Value)
			{
				Native->m_DirectWalkMode = (TDirectWalkMode)Value;
			}
		}

		property WEDirectTurnMode DirectTurnMode
		{
			WEDirectTurnMode get()
			{
				return (WEDirectTurnMode)Native->m_DirectTurnMode;
			}
			void set(WEDirectTurnMode Value)
			{
				Native->m_DirectTurnMode = (TDirectTurnMode)Value;
			}
		}

		property String^ DirectWalkAnim
		{
			String^ get()
			{
				if(Native->m_DirectWalkAnim) return gcnew String(Native->m_DirectWalkAnim);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_DirectWalkAnim, S);
				WUtils::FreeString(S);
			}
		}

		property String^ DirectTurnAnim
		{
			String^ get()
			{
				if(Native->m_DirectTurnAnim) return gcnew String(Native->m_DirectTurnAnim);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_DirectTurnAnim, S);
				WUtils::FreeString(S);
			}
		}

		property float DirectWalkVelocity
		{
			float get()
			{
				return Native->m_DirectWalkVelocity;
			}
			void set(float Value)
			{
				Native->m_DirectWalkVelocity = Value;
			}
		}

		property float DirectTurnVelocity
		{
			float get()
			{
				return Native->m_DirectTurnVelocity;
			}
			void set(float Value)
			{
				Native->m_DirectTurnVelocity = Value;
			}
		}

		property WVector3^ TargetPoint
		{
			WVector3^ get()
			{
				return gcnew WVector3(Native->m_TargetPoint.x, Native->m_TargetPoint.y, Native->m_TargetPoint.z);
			}
			void set(WVector3^ Value)
			{
				Native->m_TargetPoint = D3DXVECTOR3(Value->X, Value->Y, Value->Z);
			}
		}

		property float TargetAngle
		{
			float get()
			{
				return Native->m_TargetAngle;
			}
			void set(float Value)
			{
				Native->m_TargetAngle = Value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool GoTo(WVector3^ TargetPos, float TargetAngle)
		{
			Native->GoTo(D3DXVECTOR3(TargetPos->X, TargetPos->Y, TargetPos->Z), TargetAngle);
			return true;
		}

		bool GoTo(WVector3^ TargetPos)
		{
			return GoTo(TargetPos, -1.0f);
		}

		bool TurnTo(float Angle)
		{
			return SUCCEEDED(Native->TurnTo(Angle));
		}

		float DirToAngle(WEDirection Direction)
		{
			return Native->DirToAngle((TDirection)Direction);
		}

		WEDirection AngleToDir(float Angle)
		{
			return (WEDirection)Native->AngleToDir(Angle);
		}

		virtual bool Update() override
		{
			return SUCCEEDED(Native->Update());
		}

		virtual bool Display() override
		{
			return SUCCEEDED(Native->Display());
		}

		bool PlayAnim(String^ Name)
		{
			char* S = (char*)WUtils::GetString(Name);
			bool Ret = SUCCEEDED(Native->PlayAnim(S));
			WUtils::FreeString(S);
			return Ret;
		}

		bool PlayAnim(int Channel, String^ Name)
		{
			char* S = (char*)WUtils::GetString(Name);
			bool Ret = SUCCEEDED(Native->PlayAnim(Channel, S));
			WUtils::FreeString(S);
			return Ret;
		}


		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		bool LoadFromFile(String^ Filename)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Filename);
			Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);

			return Ret;
		}

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

		virtual bool SaveAsText(WDynBuffer^ Buffer, int Indent) override
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		virtual bool SaveAsText(WDynBuffer^ Buffer) override
		{
			return SaveAsText(Buffer, 0);
		}


	internal:
		property CAdActor3D* Native
		{
			CAdActor3D* get() { return (CAdActor3D*)m_Native; };
		}

	};

}}};

