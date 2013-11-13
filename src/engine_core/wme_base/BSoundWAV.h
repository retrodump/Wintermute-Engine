// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSOUNDWAV_H__E90B7C16_66E1_4206_B97A_DA1B4C871E18__INCLUDED_)
#define AFX_BSOUNDWAV_H__E90B7C16_66E1_4206_B97A_DA1B4C871E18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BSound.h"

class CBSoundWAV : public CBSoundBuffer  
{
public:
	CBSoundWAV(CBGame* inGame);
	virtual ~CBSoundWAV();
	DWORD m_WaveDataOffset;
	virtual HRESULT InitializeBuffer(CBFile *File);
	virtual HRESULT FillBuffer(CBFile *File);
	virtual HRESULT FillRawBuffer(CBFile *File);
	virtual HRESULT ResetStream();
	virtual HRESULT LoadSegment(int Segment);
	virtual DWORD GetPosition();
	virtual HRESULT SetPosition(DWORD Pos);
};

#endif // !defined(AFX_BSOUNDWAV_H__E90B7C16_66E1_4206_B97A_DA1B4C871E18__INCLUDED_)
