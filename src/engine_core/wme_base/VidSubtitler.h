// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CVidSubtitler :
	public CBBase
{
public:
	CVidSubtitler(CBGame* inGame);
	virtual ~CVidSubtitler(void);

	bool m_ShowSubtitle;
	int m_CurrentSubtitle;
	HRESULT LoadSubtitles(char* Filename, char* SubtitleFile);
	HRESULT Display();
	HRESULT Update(LONG Frame);
	LONG m_LastSample;
	CBArray<CVidSubtitle*, CVidSubtitle*> m_Subtitles;
};
