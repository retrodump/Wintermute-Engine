// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSAVETHUMBFILE_H__DFEAF34F_2707_436E_8335_D4A9194DA68D__INCLUDED_)
#define AFX_BSAVETHUMBFILE_H__DFEAF34F_2707_436E_8335_D4A9194DA68D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BFile.h"

class CBSaveThumbFile : public CBFile  
{
public:
	CBSaveThumbFile(CBGame* Game);
	virtual ~CBSaveThumbFile();
	virtual HRESULT Seek(DWORD Pos, TSeek Origin=SEEK_TO_BEGIN);
	virtual HRESULT Read(void* Buffer, DWORD Size);
	virtual HRESULT Close();
    virtual HRESULT Open(char* Filename);
private:
	BYTE* m_Data;
};

#endif // !defined(AFX_BSAVETHUMBFILE_H__DFEAF34F_2707_436E_8335_D4A9194DA68D__INCLUDED_)
