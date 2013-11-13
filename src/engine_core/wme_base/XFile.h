// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "BBase.h"

class CXFile : public CBBase
{
public:
	CXFile(CBGame* inGame);
	virtual ~CXFile(void);

	HRESULT OpenFile(char* Filename);
	HRESULT CloseFile();

	LPDIRECTXFILEENUMOBJECT GetEnum()
	{
		return m_XEnum;
	}

private:
	DWORD m_Size;
	BYTE* m_Buffer;
	LPDIRECTXFILE m_XFile;
	LPDIRECTXFILEENUMOBJECT m_XEnum;
};
