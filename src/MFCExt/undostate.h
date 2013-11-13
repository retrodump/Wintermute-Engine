// UndoState.h: interface for the CUndoState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDOSTATE_H__96A98532_AAFC_4C5A_89F9_97D5901390D2__INCLUDED_)
#define AFX_UNDOSTATE_H__96A98532_AAFC_4C5A_89F9_97D5901390D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../engine_core/wme_ad/dcgf_ad.h"

class CUndoState  
{
public:
	CString m_Name;
	BYTE* GetData(bool AddZero=false);
	HRESULT SetData(BYTE* Buffer, DWORD Size, CString Name);
	void Cleanup();
	DWORD m_CompSize;
	DWORD m_Size;
	BYTE* m_Buffer;
	CUndoState();
	virtual ~CUndoState();

};

#endif // !defined(AFX_UNDOSTATE_H__96A98532_AAFC_4C5A_89F9_97D5901390D2__INCLUDED_)
