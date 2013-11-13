// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSOUNDAVI_H__64290D45_76A4_4833_961D_FF0E3F99FF1F__INCLUDED_)
#define AFX_BSOUNDAVI_H__64290D45_76A4_4833_961D_FF0E3F99FF1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BSoundBuffer.h"

class CBSoundAVI : public CBSoundBuffer  
{
public:
	CBSoundAVI(CBGame* inGame);
	virtual ~CBSoundAVI();
	PAVISTREAM m_AudioStream;
	HRESULT InitializeBuffer(PAVISTREAM Stream);
	virtual HRESULT FillBuffer(CBFile *File);
	virtual DWORD GetPosition();
	virtual HRESULT LoadFromFile(char* Filename, bool ForceReload=false){ return S_OK; }
	virtual HRESULT ResetStream();
	virtual HRESULT LoadSegment(int Segment);
};

#endif // !defined(AFX_BSOUNDAVI_H__64290D45_76A4_4833_961D_FF0E3F99FF1F__INCLUDED_)
