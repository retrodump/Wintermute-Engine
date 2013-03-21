// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BRESOURCEFILE_H__F4522977_2533_43E9_9450_DC1FEC47D0E2__INCLUDED_)
#define AFX_BRESOURCEFILE_H__F4522977_2533_43E9_9450_DC1FEC47D0E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BFile.h"

class CBResourceFile : public CBFile  
{
public:
	CBResourceFile(CBGame* inGame);
	virtual ~CBResourceFile();
	virtual HRESULT Seek(DWORD Pos, TSeek Origin=SEEK_TO_BEGIN);
	virtual HRESULT Read(void* Buffer, DWORD Size);
	virtual HRESULT Close();
    virtual HRESULT Open(char* Filename);
private:
	HRESULT OpenModule(char* Filename, HMODULE Module);
	void* m_Data;
};

#endif // !defined(AFX_BRESOURCEFILE_H__F4522977_2533_43E9_9450_DC1FEC47D0E2__INCLUDED_)
