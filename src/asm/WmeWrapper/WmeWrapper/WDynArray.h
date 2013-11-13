// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

using namespace System::Collections;

namespace DeadCode { namespace WME { namespace Core
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	template <class TYPE, class CLI_TYPE> public ref class WDynArray : public ICollection
	{
	public:
		typedef CLI_TYPE^ (*NativeToManagedConvertor) (TYPE NativeVal);

		//////////////////////////////////////////////////////////////////////////
		WDynArray(CBArray<TYPE, TYPE>* Array, NativeToManagedConvertor Convertor)
		{
			m_Array = Array;
			m_Convertor = Convertor;
		};

		//////////////////////////////////////////////////////////////////////////
		WDynArray(CBArray<TYPE, TYPE>* Array)
		{
			m_Array = Array;
			m_Convertor = NULL;
		};

		//////////////////////////////////////////////////////////////////////////
		property virtual int Count
		{
			int get()
			{
				return m_Array->GetSize();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		property CLI_TYPE^ default[int]
		{
			CLI_TYPE^ get(int Index)
			{
				if(m_Convertor) return m_Convertor(m_Array->GetAt(Index));
				else return gcnew CLI_TYPE(m_Array->GetAt(Index));
			}
			
			
			void set(int Index, CLI_TYPE^ Value)
			{
				m_Array->SetAt(Index, Value->Native);
			}
			
		}

		//////////////////////////////////////////////////////////////////////////
		virtual IEnumerator^ GetEnumerator()
		{
			return gcnew WDynArrayEnumerator<TYPE, CLI_TYPE>(this);
		}

		//////////////////////////////////////////////////////////////////////////
		property virtual bool IsSynchronized
		{
			bool get()
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		property virtual Object^ SyncRoot
		{
			Object^ get()
			{
				return this;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual void CopyTo(Array^ Array, int StartIndex)
		{
			if(Array == nullptr) throw gcnew System::NullReferenceException();
			
			if(Array->Rank != 1)
				throw gcnew System::ArgumentException( "Array", "Array must be one dimensional");			
			if(StartIndex < 0)
				throw gcnew System::ArgumentOutOfRangeException( "StartIndex", "StartIndex must be greater or equal to 0" );
			if(StartIndex + Count > Array->Length)
				throw gcnew System::ArgumentException( "Array", "The array is not large enough to copy the collection contents into" );
				
			for (int i = 0; i < Count; i++)
				Array->SetValue(this[i], StartIndex + i);
		}

		//////////////////////////////////////////////////////////////////////////
		virtual int Add(CLI_TYPE^ Value)
		{
			return m_Array->Add(Value->Native);
		}

		//////////////////////////////////////////////////////////////////////////
		virtual void Clear()
		{
			m_Array->RemoveAll();
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool Contains(CLI_TYPE^ Value)
		{
			for(int i=0; i<m_Array->GetSize(); i++)
			{
				if(m_Array->GetAt(i)==Value->Native) return true;
			}
			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		virtual int IndexOf(CLI_TYPE^ Value)
		{
			for(int i=0; i<m_Array->GetSize(); i++)
			{
				if(m_Array->GetAt(i)==Value->Native) return i;
			}
			return -1;
		}

		//////////////////////////////////////////////////////////////////////////
		virtual void Insert(int Index, CLI_TYPE^ Value)
		{
			m_Array->InsertAt(Index, Value->Native);
		}

		//////////////////////////////////////////////////////////////////////////
		virtual void Remove(CLI_TYPE^ Value)
		{
			for(int i=0; i<m_Array->GetSize(); i++)
			{
				if(m_Array->GetAt(i)==Value->Native)
				{
					m_Array->RemoveAt(i);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual void RemoveAt(int Index)
		{
			m_Array->RemoveAt(Index);
		}


	private:
		CBArray<TYPE, TYPE>* m_Array;
		NativeToManagedConvertor m_Convertor;
	};


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	template <class TYPE, class CLI_TYPE> public ref class WDynArrayEnumerator : public IEnumerator
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		WDynArrayEnumerator(WDynArray<TYPE, CLI_TYPE>^ Collection)
		{
			m_Collection = Collection;
			m_CurrentIndex = -1;
		};

		//////////////////////////////////////////////////////////////////////////
		property virtual Object^ Current
		{
			Object^ get()
			{
				try
				{
					return m_Collection[m_CurrentIndex];
				}
				catch (System::IndexOutOfRangeException^)
				{
					throw gcnew System::InvalidOperationException();
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		virtual bool MoveNext()
		{
			m_CurrentIndex++;
			if(m_CurrentIndex>=m_Collection->Count) return false;
			else return true;
		}

		//////////////////////////////////////////////////////////////////////////
		virtual void Reset()
		{
			m_CurrentIndex = -1;
		}

	private:
		WDynArray<TYPE, CLI_TYPE>^ m_Collection;
		int m_CurrentIndex;
	};

}}};
