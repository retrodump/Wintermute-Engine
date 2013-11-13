// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADINVENTORY_H__69F2630D_F816_4D77_97CC_928E2FC20A5F__INCLUDED_)
#define AFX_ADINVENTORY_H__69F2630D_F816_4D77_97CC_928E2FC20A5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CAdItem;

class CAdInventory : public CBObject  
{
public:
	DECLARE_PERSISTENT(CAdInventory, CBObject);
	HRESULT RemoveItem(char* Name);
	HRESULT RemoveItem(CAdItem* Item);
	HRESULT InsertItem(char* Name, char* InsertAfter=NULL);
	CAdInventory(CBGame* inGame);
	virtual ~CAdInventory();	
	CBArray<CAdItem*, CAdItem*> m_TakenItems;
	int m_ScrollOffset;
};

#endif // !defined(AFX_ADINVENTORY_H__69F2630D_F816_4D77_97CC_928E2FC20A5F__INCLUDED_)
