// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BPERSISTMGR_H__829E4A70_10FA_4E11_8D70_6B12E57A87BE__INCLUDED_)
#define AFX_BPERSISTMGR_H__829E4A70_10FA_4E11_8D70_6B12E57A87BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "wme_plugin.h"

class CBPersistMgr : public CBBase, public IWmeSerialMgr
{
public:
	char* m_SavedDescription;
	time_t m_SavedTimestamp;
	BYTE m_SavedVerMajor;
	BYTE m_SavedVerMinor;
	BYTE m_SavedVerBuild;
	BYTE m_SavedExtMajor;
	BYTE m_SavedExtMinor;
	HRESULT SaveFile(char* Filename);
	DWORD GetDWORD();
	void PutDWORD(DWORD Val);
	char* GetString();
	void PutString(const char* Val);
	void Cleanup();
	HRESULT InitLoad(char* Filename);
	HRESULT InitSave(char* Desc);
	HRESULT GetBytes(BYTE* Buffer, DWORD Size);
	HRESULT PutBytes(BYTE* Buffer, DWORD Size);
	DWORD m_Offset;
	DWORD m_BufferSize;
	BYTE* m_Buffer;
	bool m_Saving;

	DWORD m_RichBufferSize;
	BYTE* m_RichBuffer;

	HRESULT Transfer(char* Name, void* Val);
	HRESULT Transfer(char* Name, int* Val);
	HRESULT Transfer(char* Name, DWORD* Val);
	HRESULT Transfer(char* Name, float* Val);
	HRESULT Transfer(char* Name, double* Val);
	HRESULT Transfer(char* Name, bool* Val);
	HRESULT Transfer(char* Name, BYTE* Val);
	HRESULT Transfer(char* Name, RECT* Val);
	HRESULT Transfer(char* Name, POINT* Val);
	HRESULT Transfer(char* Name, char** Val);
	HRESULT Transfer(char* Name, D3DXVECTOR2* Val);
	HRESULT Transfer(char* Name, D3DXVECTOR3* Val);
	HRESULT Transfer(char* Name, D3DXVECTOR4* Val);
	HRESULT Transfer(char* Name, D3DXVECTOR3** Val);
	HRESULT Transfer(char* Name, D3DXMATRIX* Val);
	CBPersistMgr(CBGame* inGame=NULL);
	virtual ~CBPersistMgr();
	bool CheckVersion(BYTE VerMajor, BYTE VerMinor, BYTE VerBuild);

	DWORD m_ThumbnailDataSize;
	BYTE* m_ThumbnailData;

// IWmeSerialMgr
public:
	bool IsSaving();
	bool GetBuffer(unsigned char* Buffer, int Size);
	bool PutBuffer(unsigned char* Buffer, int Size);
	bool TransferValue(IWmeGame** Value);	
	bool TransferValue(IWmeObject** Value);
	bool TransferValue(IWmeSubFrame** Value);
	bool TransferValue(int* Value);
	bool TransferValue(unsigned char* Value);
	bool TransferValue(float* Value);
	bool TransferValue(double* Value);
	bool TransferValue(bool* Value);
	HRESULT FillRichBuffer(char* Desc);
};

#endif // !defined(AFX_BPERSISTMGR_H__829E4A70_10FA_4E11_8D70_6B12E57A87BE__INCLUDED_)
