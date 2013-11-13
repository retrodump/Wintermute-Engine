// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BSoundBuffer.h"

class CVidTheoraPlayer;

class CBSoundTheora : public CBSoundBuffer
{
public:
	CBSoundTheora(CBGame* inGame);
	virtual ~CBSoundTheora(void);

	virtual HRESULT FillBuffer(CBFile *File);
	virtual DWORD GetPosition();
	virtual HRESULT LoadFromFile(char* Filename, bool ForceReload=false){ return S_OK; }
	virtual HRESULT ResetStream();
	virtual HRESULT LoadSegment(int Segment);

	HRESULT InitializeBuffer(CVidTheoraPlayer* Player);
	bool WriteBlock(BYTE* Buffer, DWORD Size);

	CVidTheoraPlayer* m_Player;
	int m_NumBuffers;
	int m_LastPlayBlock;
	int m_CurrentTime;
};
