//////////////////////////////////////////////////////////////////////////
// Wintermute Engine plugin interface
// Convenience functions
// (c) Dead:Code 2010
//
//////////////////////////////////////////////////////////////////////////


#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "WmeUtils.h"

bool CWmeUtils::m_RandInit = false;

//////////////////////////////////////////////////////////////////////////
CWmeUtils::CWmeUtils(void)
{	
}

//////////////////////////////////////////////////////////////////////////
CWmeUtils::~CWmeUtils(void)
{
}

//////////////////////////////////////////////////////////////////////////
// returns a random number from a given range
int CWmeUtils::Random(int From, int To)
{	
	if(!m_RandInit)
	{
		srand((unsigned)time(NULL));
		m_RandInit = true;
	}

	if(From>To)
	{
		int Temp = To;
		To = From;
		From = Temp;
	}
	return (rand() % (To - From + 1)) + From;
}

//////////////////////////////////////////////////////////////////////////
// packs RGBA values into a single DWORD number
unsigned long CWmeUtils::MakeRGBA(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
{
	return (unsigned long)((((A)&0xff)<<24)|(((R)&0xff)<<16)|(((G)&0xff)<<8)|((B)&0xff));
}

//////////////////////////////////////////////////////////////////////////
// extracts Red component from a DWORD color definition
unsigned char CWmeUtils::GetR(unsigned long Color)
{
	return (unsigned char)Color;
}

//////////////////////////////////////////////////////////////////////////
// extracts Green component from a DWORD color definition
unsigned char CWmeUtils::GetG(unsigned long Color)
{
	return (unsigned char)((unsigned short)Color>>8);
}

//////////////////////////////////////////////////////////////////////////
// extracts Blue component from a DWORD color definition
unsigned char CWmeUtils::GetB(unsigned long Color)
{
	return (unsigned char)(Color>>16);
}

//////////////////////////////////////////////////////////////////////////
// extracts Alpha component from a DWORD color definition
unsigned char CWmeUtils::GetA(unsigned long Color)
{
	return (unsigned char)(Color>>24);
}

//////////////////////////////////////////////////////////////////////////
// convenience function for serialization and deserialization
// of string variables
void CWmeUtils::TransferString(IWmeSerialMgr* SerialMgr, char** Str)
{
	int StrLen;
	if(SerialMgr->IsSaving())
	{
		if(*Str==NULL)
		{
			StrLen = -1;
			SerialMgr->TransferValue(&StrLen);
		}
		else
		{
			StrLen = strlen(*Str)+1;
			SerialMgr->TransferValue(&StrLen);
			SerialMgr->PutBuffer((unsigned char*)*Str, StrLen);
		}
	}
	else
	{
		SerialMgr->TransferValue(&StrLen);
		if(StrLen<0) *Str = NULL;
		else
		{
			*Str = new char[StrLen];
			SerialMgr->GetBuffer((unsigned char*)*Str, StrLen);
		}
	}
}
