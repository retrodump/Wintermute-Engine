// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSOUNDOGG_H__634222FB_F508_4DF6_88A8_8B53DAD314E9__INCLUDED_)
#define AFX_BSOUNDOGG_H__634222FB_F508_4DF6_88A8_8B53DAD314E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vorbis/vorbisfile.h>
#include "BSound.h"

typedef ogg_int64_t int64_t;

class CBSoundOGG : public CBSoundBuffer  
{
public:
	HRESULT OGGReadBlock(void* Buffer, DWORD Size);
	bool m_OGGOpen;
	HRESULT OGGClose();		
	HRESULT OGGOpen(CBFile* File);
	OggVorbis_File m_VorbisFile;
	CBSoundOGG(CBGame* inGame);
	virtual ~CBSoundOGG();
	virtual HRESULT InitializeBuffer(CBFile *File);
	virtual HRESULT FillBuffer(CBFile *File);
	virtual HRESULT FillRawBuffer(CBFile *File);
	virtual HRESULT ResetStream();
	virtual HRESULT LoadSegment(int Segment);
	virtual DWORD GetPosition();
	virtual HRESULT SetPosition(DWORD Pos);

	// OGG -> CBFile interface
	static size_t FileRead(void* ptr, size_t size, size_t nmemb, void* datasource);
	static int    FileSeek(void* datasource, int64_t offset, int whence);
	static int    FileClose(void* datasource);
	static long   FileTell(void* datasource);
};

#endif // !defined(AFX_BSOUNDOGG_H__634222FB_F508_4DF6_88A8_8B53DAD314E9__INCLUDED_)
