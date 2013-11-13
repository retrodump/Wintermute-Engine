// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BDISKFILE_H__19B36B9B_1D93_4E7F_AD98_2C257C2F0C1C__INCLUDED_)
#define AFX_BDISKFILE_H__19B36B9B_1D93_4E7F_AD98_2C257C2F0C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BFile.h"

class CBDiskFile : public CBFile  
{
public:
	CBDiskFile(CBGame* inGame);
	virtual ~CBDiskFile();
	virtual HRESULT Seek(DWORD Pos, TSeek Origin=SEEK_TO_BEGIN);
	virtual HRESULT Read(void* Buffer, DWORD Size);
	virtual HRESULT Close();
    virtual HRESULT Open(char* Filename);
private:
	FILE* m_File;
	BYTE* m_Data;
	bool m_Compressed;
	DWORD m_PrefixSize;
};

#endif // !defined(AFX_BDISKFILE_H__19B36B9B_1D93_4E7F_AD98_2C257C2F0C1C__INCLUDED_)
