// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BPACKAGE_H__D95E9844_EE72_4E5C_99ED_183022560A44__INCLUDED_)
#define AFX_BPACKAGE_H__D95E9844_EE72_4E5C_99ED_183022560A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBPackage : public CBBase  
{
public:
	bool m_BoundToExe;
	BYTE m_Priority;
	HRESULT Read(DWORD Offset, BYTE* Buffer, DWORD Size);
	HRESULT Close();
	HRESULT Open();
	char* m_Name;
	int m_CD;
	FILE* m_File;
	CBPackage(CBGame* inGame);
	virtual ~CBPackage();

};

#endif // !defined(AFX_BPACKAGE_H__D95E9844_EE72_4E5C_99ED_183022560A44__INCLUDED_)
