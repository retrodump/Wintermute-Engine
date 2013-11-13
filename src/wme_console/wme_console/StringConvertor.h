/***

Author - Nishant Sivakumar
Copyright(C) - Nishant Sivakumar 2005

***/

#pragma once

#include <wtypes.h>

#include <vector>
#include <string>

#include <vcclr.h>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace StringUtilities
{
	//Class StringConvertorBase
	//Serves as the abstract base class for StringConvertor and implements
	//default initialization and clean-up code.
	ref class StringConvertorBase abstract
	{
	protected:
		StringConvertorBase()
		{
			InternalInit();	
		}	

		~StringConvertorBase()
		{
			InternalCleanUp();
		}

		//While the finalizer is included to take care of cases
		//where the destructor does not get called, it's highly inefficient
		//and ruins the purpose of the class if the class is not used
		//with deterministic destruction. It's recommended that you either use
		//the auto-variable non-handle declaration format or explicitly
		//call delete on the StringConvertor object after use.
		!StringConvertorBase()
		{
			InternalCleanUp();

#ifdef _DEBUG
			throw gcnew Exception("Finalizer should not have got called");
#else
			Diagnostics::Trace::WriteLine("Finalizer should not have got called");
#endif
		}

		std::vector< gcroot<IntPtr> >* m_vec_hglobal;
		std::vector< gcroot<IntPtr> >* m_vec_bstr;

		//Frees the allocated unmanaged memory
		void InternalCleanUp()
		{
			for each(gcroot<IntPtr> ptr in *m_vec_hglobal)
			{
				Marshal::FreeHGlobal(ptr);
			}
			m_vec_hglobal->clear();
			delete m_vec_hglobal;

			for each(gcroot<IntPtr> ptr in *m_vec_bstr)
			{
				Marshal::FreeBSTR(ptr);
			}
			m_vec_bstr->clear();
			delete m_vec_bstr;
		}

		void InternalInit()
		{
			m_vec_hglobal = new std::vector< gcroot<IntPtr> >(); 
			m_vec_bstr = new std::vector< gcroot<IntPtr> >(); 
		}
	};

	//Class StringConvertor
	//General purpose convertor class for System::String (both from and to)
	ref class StringConvertor : StringConvertorBase
	{
	protected:
		String^ m_String; //The internal System::String object

	public: 
		//Various constructors each taking a different type as argument
		StringConvertor(String^ s) : m_String(s)
		{
			if(m_String == nullptr)
				throw gcnew Exception("You need to pass a non-null String");			
		}

		StringConvertor(const char* s)
		{
			if(s == nullptr)
				throw gcnew Exception("You need to pass a non-null char*");
			m_String = gcnew String(s);
		}

		StringConvertor(const __wchar_t* s)
		{
			if(s == nullptr)
				throw gcnew Exception("You need to pass a non-null __wchar_t*");
			m_String = gcnew String(s);
		}

		StringConvertor(array<Char>^ s)
		{
			if(s == nullptr)
				throw gcnew Exception("You need to pass a non-null Char array");
			m_String = gcnew String(s);
		}

		StringConvertor(BSTR s)
		{
			if(s == nullptr)
				throw gcnew Exception("You need to pass a non-null BSTR");
			m_String = gcnew String(s);
		}

		StringConvertor(std::string s)
		{
			m_String = gcnew String(s.c_str());
		}

		StringConvertor(std::wstring s)
		{
			m_String = gcnew String(s.c_str());
		}

		//Methods

		virtual String^ ToString() override
		{
			return m_String;
		}

		//Operators

		operator String^()
		{
			return m_String;
		}

		//Properties	

		//Returns an interior pointer to the underlying string. The
		//pointer is of type const Char so you can only read through it.
		property interior_ptr<const Char> InteriorConstCharPtr
		{
			interior_ptr<const Char> get()
			{
				return PtrToStringChars(m_String);			
			}
		}

		//Returns an interior pointer to the underlying string that can be
		//written to. Use with extreme care, as you are directly editing the 
		//System::String's internal character buffer.
		property interior_ptr<Char> InteriorCharPtr
		{
			interior_ptr<Char> get()
			{
				return const_cast< interior_ptr<Char> >(InteriorConstCharPtr);			 
			}
		}

		//Generic character mapping to use with LPTSTR.
		//Since it's a #define, intellisense won't show it.

#ifdef _UNICODE 
	#define NativeTCharPtr NativeWideCharPtr
#else
	#define NativeTCharPtr NativeCharPtr
#endif

		//Returns a char* (allocated on the native heap)
		property char* NativeCharPtr
		{
			char* get()
			{
				IntPtr ptr = Marshal::StringToHGlobalAnsi(m_String);
				if(ptr != IntPtr::Zero)
				{
					m_vec_hglobal->push_back(ptr);
					return reinterpret_cast<char*>(static_cast<void*>(ptr));
				}
				else 
					return nullptr;			
			}
		}

		//Returns a __wchar_t* (allocated on the native heap)
		property __wchar_t* NativeWideCharPtr
		{
			__wchar_t* get()
			{
				IntPtr ptr = Marshal::StringToHGlobalUni(m_String);
				if(ptr != IntPtr::Zero)
				{
					m_vec_hglobal->push_back(ptr);
					return reinterpret_cast<__wchar_t*>(static_cast<void*>(ptr));
				}
				else 
					return nullptr;
			}
		}

		//Returns a CLI array of Chars
		property array<Char>^ CharArray
		{
			array<Char>^ get()
			{
				return m_String->ToCharArray();
			}
		}

		//Returns a BSTR allocated natively (unmanaged heap)
		property BSTR BSTRCopy
		{
			BSTR get()
			{
				IntPtr ptr = Marshal::StringToBSTR(m_String);
				if(ptr != IntPtr::Zero)
				{
					m_vec_bstr->push_back(ptr);
					return reinterpret_cast<BSTR>(static_cast<void*>(ptr));
				}
				else 
					return nullptr;	
			}
		}

		//Returns an std::string object
		property std::string STLAnsiString
		{
			std::string get()
			{
				IntPtr ptr = Marshal::StringToHGlobalAnsi(m_String);
				if(ptr != IntPtr::Zero)
				{
					return std::string(reinterpret_cast<char*>(static_cast<void*>(ptr)));
					Marshal::FreeHGlobal(ptr);
				}
				return std::string();
			}
		}

		//Returns an std::wstring object
		property std::wstring STLWideString
		{
			std::wstring get()
			{
				IntPtr ptr = Marshal::StringToHGlobalUni(m_String);
				if(ptr != IntPtr::Zero)
				{
					return std::wstring(reinterpret_cast<__wchar_t*>(static_cast<void*>(ptr)));
					Marshal::FreeHGlobal(ptr);
				}
				return std::wstring();
			}
		}
	};
}