// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADINVENTORYBOX_H__1D72F90E_1116_478B_9A8E_C01B2B8A2FC1__INCLUDED_)
#define AFX_ADINVENTORYBOX_H__1D72F90E_1116_478B_9A8E_C01B2B8A2FC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CAdInventoryBox : public CBObject  
{
public:
	bool m_HideSelected;
	DECLARE_PERSISTENT(CAdInventoryBox, CBObject);
	bool m_Exclusive;
	int m_ScrollBy;
	int m_ItemHeight;
	int m_ItemWidth;
	bool m_Visible;
	virtual HRESULT Display();
	CUIButton* m_CloseButton;
	int m_Spacing;
	int m_ScrollOffset;
	RECT m_ItemsArea;
	HRESULT Listen(CBScriptHolder* param1, DWORD param2);
	CUIWindow* m_Window;
	CAdInventoryBox(CBGame* inGame);
	virtual ~CAdInventoryBox();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);
};

#endif // !defined(AFX_ADINVENTORYBOX_H__1D72F90E_1116_478B_9A8E_C01B2B8A2FC1__INCLUDED_)
