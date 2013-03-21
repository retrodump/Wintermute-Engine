// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BPKGFILE_H__FCB9EEBF_8FBD_4572_8554_B456A5C08D52__INCLUDED_)
#define AFX_BPKGFILE_H__FCB9EEBF_8FBD_4572_8554_B456A5C08D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BFile.h"
#include "zlib.h"	// Added by ClassView

#define COMPRESSED_BUFFER_SIZE 4096

class CBPkgFile : public CBFile  
{
public:	
	CBPkgFile(CBGame* inGame);
	virtual ~CBPkgFile();
	virtual HRESULT Seek(DWORD Pos, TSeek Origin=SEEK_TO_BEGIN);
	virtual HRESULT Read(void* Buffer, DWORD Size);
	virtual HRESULT Close();
    virtual HRESULT Open(char* Filename);
private:
	bool m_InflateInit;
	HRESULT SeekToPos(DWORD NewPos);
	bool m_Compressed;
	CBFileEntry* m_FileEntry;
	z_stream m_Stream;
	BYTE m_CompBuffer[COMPRESSED_BUFFER_SIZE];
};

#endif // !defined(AFX_BPKGFILE_H__FCB9EEBF_8FBD_4572_8554_B456A5C08D52__INCLUDED_)
