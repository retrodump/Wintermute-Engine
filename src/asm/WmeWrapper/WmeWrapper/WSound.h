// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;
	using namespace System::Runtime::InteropServices;


	public ref class WSound : public WBase
	{
	public:
		WSound() {};
		WSound(WGame^ Game);
		WSound(CBSound* Native);
		virtual ~WSound(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property int Volume
		{
			int get()
			{
				return Native->GetVolume();
			}
			void set(int Value)
			{
				Native->SetVolume(Value);
			}
		}

		property int Position
		{
			int get()
			{
				return Native->GetPositionTime();
			}
			void set(int Value)
			{
				Native->SetPositionTime(Value);
			}
		}

		property bool Playing
		{
			bool get()
			{
				return Native->IsPlaying();
			}
		}

		property bool Paused
		{
			bool get()
			{
				return Native->IsPaused();
			}
		}

		property int Length
		{
			int get()
			{
				return Native->GetLength();
			}
		}

		property String^ Filename
		{
			String^ get()
			{
				if(Native->m_SoundFilename) return gcnew String(Native->m_SoundFilename);
				else return nullptr;
			}
		}

		property bool Streamed
		{
			bool get()
			{
				return Native->m_SoundStreamed;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelanous
		//////////////////////////////////////////////////////////////////////////
		bool SetLoopStart(int Time)
		{
			return SUCCEEDED(Native->SetLoopStart(Time));
		}

		void GetRawSample(int Time, [Out]int% LeftChannel, [Out]int% RightChannel)
		{
			int Left, Right;
			Native->GetRawData(Time, &Left, &Right);

			LeftChannel = Left;
			RightChannel = Right;
		}

		bool LoadRawData()
		{
			return SUCCEEDED(Native->LoadRawData());
		}

		bool SetPan(int Pan)
		{
			return SUCCEEDED(Native->SetPan(Pan));
		}

		bool Play(bool Looping)
		{
			return SUCCEEDED(Native->Play(Looping));
		}

		bool Play()
		{
			return Play(false);
		}

		bool Stop()
		{
			return SUCCEEDED(Native->Stop());
		}

		bool Pause()
		{
			return SUCCEEDED(Native->Pause());
		}

		bool Resume()
		{
			return SUCCEEDED(Native->Resume());
		}

		bool SetSound(String^ Filename, WESoundType Type, bool Streamed)
		{
			char* S = (char*)WUtils::GetString(Filename);
			bool Ret = SUCCEEDED(Native->SetSound(S, (TSoundType)Type, Streamed));
			WUtils::FreeString(S);
			return Ret;
		}

		bool SetSound(String^ Filename, WESoundType Type)
		{
			return SetSound(Filename, WESoundType::Sfx, false);
		}

		bool SetSound(String^ Filename)
		{
			return SetSound(Filename, WESoundType::Sfx);
		}

		bool ApplyFX(WESFXType Type, float Param1, float Param2, float Param3, float Param4)
		{
			return SUCCEEDED(Native->ApplyFX((TSFXType)Type, Param1, Param2, Param3, Param4));
		}

		bool ApplyFX(WESFXType Type, float Param1, float Param2, float Param3)
		{
			return ApplyFX(Type, Param1, Param2, Param3, 0);
		}

		bool ApplyFX(WESFXType Type, float Param1, float Param2)
		{
			return ApplyFX(Type, Param1, Param2, 0);
		}

		bool ApplyFX(WESFXType Type, float Param1)
		{
			return ApplyFX(Type, Param1, 0);
		}

		bool ApplyFX(WESFXType Type)
		{
			return ApplyFX(Type, 0);
		}

		bool ApplyFX()
		{
			return ApplyFX(WESFXType::None);
		}


	internal:
		property CBSound* Native
		{
			CBSound* get() { return (CBSound*)m_Native; };
		}
	};
}}}
