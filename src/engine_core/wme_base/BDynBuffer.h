// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BDYNBUFFER_H__FA4068D2_B704_437C_B190_926E10C35C45__INCLUDED_)
#define AFX_BDYNBUFFER_H__FA4068D2_B704_437C_B190_926E10C35C45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBDynBuffer : public CBBase  
{
public:
	bool m_Initialized;
	void PutText(LPSTR fmt, ...);
	void PutTextIndent(int Indent, LPSTR fmt, ...);	
	DWORD GetDWORD();
	void PutDWORD(DWORD Val);
	char* GetString();
	void PutString(char* Val);
	HRESULT GetBytes(BYTE* Buffer, DWORD Size);
	HRESULT PutBytes(BYTE* Buffer, DWORD Size);
	DWORD GetSize();
	HRESULT Init(DWORD InitSize=0);
	void Cleanup();
	DWORD m_Size;
	BYTE* m_Buffer;
	CBDynBuffer(CBGame* inGame, DWORD InitSize=1000, DWORD GrowBy=1000);
	virtual ~CBDynBuffer();

private:
	DWORD m_RealSize;
	DWORD m_GrowBy;
	DWORD m_InitSize;
	DWORD m_Offset;
	void PutTextForm(const char *format, va_list argptr);
};

#endif // !defined(AFX_BDYNBUFFER_H__FA4068D2_B704_437C_B190_926E10C35C45__INCLUDED_)
