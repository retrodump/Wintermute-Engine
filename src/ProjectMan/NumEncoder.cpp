#include "StdAfx.h"
#include "NumEncoder.h"

//////////////////////////////////////////////////////////////////////////
CNumEncoder::CNumEncoder(void)
{
}

//////////////////////////////////////////////////////////////////////////
CNumEncoder::~CNumEncoder(void)
{	
	m_Numbers.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
bool CNumEncoder::DecodeFromString(CString Str)
{
	Str = Str.MakeLower();

	Clear();
	bool InNumber = false;
	int ToRead = 0;
	CString Temp = "";

	for(int i=0; i<Str.GetLength(); i++)
	{
		char Ch = GetChar(i, Str);
		if(!InNumber)
		{
			ToRead = Ch;
			InNumber = true;
			Temp = "";
		}
		else
		{
			Temp += (char)('0' + Ch);
			ToRead--;

			if(ToRead==0)
			{
				m_Numbers.Add(atoi(Temp));
				InNumber = false;
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
char CNumEncoder::GetChar(int Index, CString Str)
{
	if(Index < 0 || Index >= Str.GetLength()) return 0;
	char ToSubtract = (Index + 1) % 5;

	char Ret = Str.GetAt(Index);
	Ret -= ToSubtract;
	Ret -= 'a';

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
CString CNumEncoder::EncodeToString()
{
	CString Ret = "";

	for(int i=0; i<m_Numbers.GetSize(); i++)
	{
		CString Temp;
		Temp.Format("%d", m_Numbers[i]);
		AddChar(Ret, (char)Temp.GetLength());
		for(int j=0; j<Temp.GetLength(); j++)
		{
			AddChar(Ret, Temp.GetAt(j) - '0');
		}		
	}

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
void CNumEncoder::AddChar(CString& Str, char Ch)
{
	char ToAdd = (Str.GetLength() + 1) % 5;
	Str += (char)('a' + Ch + ToAdd);
}

//////////////////////////////////////////////////////////////////////////
void CNumEncoder::Clear()
{
	m_Numbers.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
bool CNumEncoder::AddNumber(int Num)
{
	m_Numbers.Add(Num);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CNumEncoder::SetNumber(int Index, int Num)
{
	while(m_Numbers.GetSize() <= Index) m_Numbers.Add(0);

	m_Numbers.SetAt(Index, Num);
	return true;
}

//////////////////////////////////////////////////////////////////////////
int CNumEncoder::GetNumber(int Index, int InitVal)
{
	if(Index < 0 || Index >= m_Numbers.GetCount()) return InitVal;
	else return m_Numbers[Index];
}
