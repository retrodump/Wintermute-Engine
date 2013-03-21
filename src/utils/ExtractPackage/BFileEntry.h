// BFileEntry.h: interface for the CBFileEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BFILEENTRY_H__33934FA5_7E27_4AAB_A7F0_DCD41A6D91C2__INCLUDED_)
#define AFX_BFILEENTRY_H__33934FA5_7E27_4AAB_A7F0_DCD41A6D91C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBPackage;

class CBFileEntry
{
public:
	DWORD m_Flags;
	DWORD m_JournalTime;
	CString m_Filename;
	DWORD m_CompressedLength;
	DWORD m_Length;
	DWORD m_Offset;
	CBFileEntry();
	virtual ~CBFileEntry();

};

#endif // !defined(AFX_BFILEENTRY_H__33934FA5_7E27_4AAB_A7F0_DCD41A6D91C2__INCLUDED_)
