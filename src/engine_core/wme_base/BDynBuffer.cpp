// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BDynBuffer.h"


//////////////////////////////////////////////////////////////////////////
CBDynBuffer::CBDynBuffer(CBGame* inGame, DWORD InitSize, DWORD GrowBy):CBBase(inGame)
{
	m_Buffer = NULL;
	m_Size = 0;
	m_RealSize = 0;

	m_Offset = 0;
	m_InitSize = InitSize;
	m_GrowBy = GrowBy;

	m_Initialized = false;
}


//////////////////////////////////////////////////////////////////////////
CBDynBuffer::~CBDynBuffer()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void CBDynBuffer::Cleanup()
{
	if(m_Buffer) free(m_Buffer);
	m_Buffer = NULL;
	m_Size = 0;
	m_RealSize = 0;
	m_Offset = 0;
	m_Initialized = false;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBDynBuffer::GetSize()
{
	return m_Size;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDynBuffer::Init(DWORD InitSize)
{
	Cleanup();

	if(InitSize==0) InitSize = m_InitSize;

	m_Buffer = (BYTE*)malloc(InitSize);
	if(!m_Buffer){
		Game->LOG(0, "CBDynBuffer::Init - Error allocating %d bytes", InitSize);
			return E_FAIL;
	}

	m_RealSize = InitSize;
	m_Initialized = true;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDynBuffer::PutBytes(BYTE *Buffer, DWORD Size)
{
	if(!m_Initialized) Init();

	while(m_Offset + Size > m_RealSize){
		m_RealSize += m_GrowBy;
		m_Buffer = (BYTE*)realloc(m_Buffer, m_RealSize);
		if(!m_Buffer){
			Game->LOG(0, "CBDynBuffer::PutBytes - Error reallocating buffer to %d bytes", m_RealSize);
			return E_FAIL;
		}
	}

	memcpy(m_Buffer+m_Offset, Buffer, Size);
	m_Offset+=Size;
	m_Size+=Size;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDynBuffer::GetBytes(BYTE *Buffer, DWORD Size)
{
	if(!m_Initialized) Init();

	if(m_Offset+Size>m_Size){
		Game->LOG(0, "CBDynBuffer::GetBytes - Buffer underflow");
		return E_FAIL;
	}

	memcpy(Buffer, m_Buffer+m_Offset, Size);
	m_Offset+=Size;
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CBDynBuffer::PutDWORD(DWORD Val)
{
	PutBytes((BYTE*)&Val, sizeof(DWORD));
}


//////////////////////////////////////////////////////////////////////////
DWORD CBDynBuffer::GetDWORD()
{
	DWORD ret;
	GetBytes((BYTE*)&ret, sizeof(DWORD));
	return ret;
}


//////////////////////////////////////////////////////////////////////////
void CBDynBuffer::PutString(char* Val)
{
	if(!Val) PutString("(null)");
	else{
		PutDWORD(strlen(Val)+1);
		PutBytes((BYTE*)Val, strlen(Val)+1);
	}
}


//////////////////////////////////////////////////////////////////////////
char* CBDynBuffer::GetString()
{
	DWORD len = GetDWORD();
	char* ret = (char*)(m_Buffer+m_Offset);
	m_Offset+=len;

	if(!strcmp(ret, "(null)")) return NULL;
	else return ret;
}


//////////////////////////////////////////////////////////////////////////
void CBDynBuffer::PutText(LPSTR fmt, ...)
{	
	va_list va;

	va_start(va, fmt);
	PutTextForm(fmt, va);
	va_end(va);
	
}


//////////////////////////////////////////////////////////////////////////
void CBDynBuffer::PutTextIndent(int Indent, LPSTR fmt, ...)
{	
	va_list va;
	
	PutText("%*s", Indent, "");

	va_start(va, fmt);
	PutTextForm(fmt, va);
	va_end(va);
}


//////////////////////////////////////////////////////////////////////////
void CBDynBuffer::PutTextForm(const char *format, va_list argptr)
{
	char buff[32768];
	vsprintf(buff, format, argptr);
	PutBytes((BYTE*)buff, strlen(buff));
}