// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BFILEENTRY_H__33934FA5_7E27_4AAB_A7F0_DCD41A6D91C2__INCLUDED_)
#define AFX_BFILEENTRY_H__33934FA5_7E27_4AAB_A7F0_DCD41A6D91C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBPackage;

class CBFileEntry : public CBBase  
{
public:
	DWORD m_TimeDate2;
	DWORD m_TimeDate1;
	DWORD m_Flags;
	DWORD m_JournalTime;
	string m_Filename;
	DWORD m_CompressedLength;
	DWORD m_Length;
	DWORD m_Offset;
	CBPackage* m_Package;
	CBFileEntry(CBGame* inGame);
	virtual ~CBFileEntry();

};

#endif // !defined(AFX_BFILEENTRY_H__33934FA5_7E27_4AAB_A7F0_DCD41A6D91C2__INCLUDED_)
