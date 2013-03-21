// TreeItem.h: interface for the CTreeItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREEITEM_H__026C2F7C_885B_4415_9D69_CC9F5888B6B5__INCLUDED_)
#define AFX_TREEITEM_H__026C2F7C_885B_4415_9D69_CC9F5888B6B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../MFCExt/objects.h"

class CTreeItem  
{
public:	
	bool m_IsTopLevel;
	CString m_KeyFile;
	CString m_FullPath;
	CString m_ShortName;
	int m_IconIndex;
	HTREEITEM m_Item;	
	CTreeItem();
	virtual ~CTreeItem();

	bool m_IsDir;
	bool m_IsPackage;

	// complex objects (dirs)
	DWORD m_IsObject;

	// containers
	DWORD m_Contains;

};

#endif // !defined(AFX_TREEITEM_H__026C2F7C_885B_4415_9D69_CC9F5888B6B5__INCLUDED_)
