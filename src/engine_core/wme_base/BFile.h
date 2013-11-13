// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BFILE_H__A456CB82_6C61_4CF8_93CE_4A3BC0E059E3__INCLUDED_)
#define AFX_BFILE_H__A456CB82_6C61_4CF8_93CE_4A3BC0E059E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "wme_plugin.h"

class CBFile : public CBBase, public IWmeFile
{
public:
	DWORD m_Pos;
	DWORD m_Size;
	virtual DWORD GetSize(){ return m_Size; };
	virtual DWORD GetPos() { return m_Pos; };
	virtual HRESULT Seek(DWORD Pos, TSeek Origin=SEEK_TO_BEGIN)=0;
	virtual HRESULT Read(void* Buffer, DWORD Size)=0;
	virtual HRESULT Close()=0;
	virtual HRESULT Open(char* Filename)=0;
	virtual bool IsEOF();
	CBFile(CBGame* inGame);
	virtual ~CBFile();

	// IWmeFile
	unsigned long ReadData(unsigned char* Buffer, unsigned long NumBytes);
	unsigned long GetFileSize();
	unsigned long GetFilePosition();
	bool SeekToPosition(unsigned long Position);
};

#endif // !defined(AFX_BFILE_H__A456CB82_6C61_4CF8_93CE_4A3BC0E059E3__INCLUDED_)
