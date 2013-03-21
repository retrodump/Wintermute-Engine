// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;
using namespace DeadCode::WME::Debugger;

#include "WmeObject.h"

namespace wme_console
{
	ref class WmeValue : public IWmeValue
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		WmeValue(IWmeDebugProp* Native, const char* Name)
		{
			m_Native = Native;
			m_Name = gcnew String(Name);
			m_Properties = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		WmeValue(IWmeDebugProp* Native)
		{
			m_Native = Native;
			m_Name = nullptr;
			m_Properties = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property IntPtr NativeID
		{
			IntPtr get()
			{
				return IntPtr(m_Native);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property String^ Name
		{
			String^ get()
			{
				return m_Name;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property EWmeValue Type
		{
			EWmeValue get()
			{
				try
				{
					switch(m_Native->DbgGetType())
					{
						case WME_DBGPROP_INT:
							return EWmeValue::Int;
						case WME_DBGPROP_FLOAT:
							return EWmeValue::Float;
						case WME_DBGPROP_BOOL:
							return EWmeValue::Bool;
						case WME_DBGPROP_STRING:
							return EWmeValue::String;
						case WME_DBGPROP_OBJECT:
							return EWmeValue::Object;
						case WME_DBGPROP_NATIVE:
							return EWmeValue::Native;
						default:
							return EWmeValue::Unknown;
					}
				}
				catch(Exception^)
				{
					return EWmeValue::Unknown;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual int GetInt()
		{
			try
			{
				return m_Native->DbgGetValInt();
			}
			catch (Exception^)
			{
				return 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual double GetFloat()
		{
			try
			{
				return m_Native->DbgGetValFloat();
			}
			catch (Exception^)
			{
				return 0.0;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool GetBool()
		{
			try
			{
				return m_Native->DbgGetValBool();
			}
			catch (Exception^)
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual String^ GetString()
		{
			try
			{
				return gcnew String(m_Native->DbgGetValString());
			}
			catch(Exception^)
			{
				return gcnew String("");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual IWmeObject^ GetNative()
		{
			try
			{
				if(m_Native->DbgGetValNative()==NULL) return nullptr;
				else return gcnew WmeObject(m_Native->DbgGetValNative());
			}
			catch(Exception^)
			{
				return nullptr;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool SetValue(int Value)
		{
			try
			{
				return m_Native->DbgSetVal(Value);
			}
			catch(Exception^)
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool SetValue(double Value)
		{
			try
			{
				return m_Native->DbgSetVal(Value);
			}
			catch(Exception^)
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool SetValue(bool Value)
		{
			try
			{
				return m_Native->DbgSetVal(Value);
			}
			catch(Exception^)
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool SetValue(String^ Value)
		{
			try
			{
				IntPtr ip = Marshal::StringToHGlobalAnsi(Value);
				const char* str = static_cast<const char*>(ip.ToPointer());

				bool Ret = m_Native->DbgSetVal(str);

				Marshal::FreeHGlobal(ip);
				return Ret;
			}
			catch(Exception^)
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool SetValue()
		{
			try
			{
				return m_Native->DbgSetVal();
			}
			catch(Exception^)
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual property ICollection<IWmeValue^>^ Properties
		{
			ICollection<IWmeValue^>^ get()
			{
				if(m_Properties==nullptr) m_Properties = gcnew List<IWmeValue^>();
				m_Properties->Clear();

				try
				{
					int NumProps = m_Native->DbgGetNumProperties();
					for(int i=0; i<NumProps; i++)
					{
						const char* PropName;
						IWmeDebugProp* PropValue;

						if(m_Native->DbgGetProperty(i, &PropName, &PropValue))
						{
							m_Properties->Add(gcnew WmeValue(PropValue, PropName));
						}
					}
				}
				catch(Exception^)
				{

				}
				
				return m_Properties;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual String^ GetDebuggerDesc()
		{
			try
			{
				char Temp[32768];
				m_Native->DbgGetDescription(Temp, 32768);
				return gcnew String(Temp);
			}
			catch(Exception^)
			{
				return gcnew String("");
			}
		}

	private:
		IWmeDebugProp* m_Native;
		String^ m_Name;
		List<IWmeValue^>^ m_Properties;
	};

}