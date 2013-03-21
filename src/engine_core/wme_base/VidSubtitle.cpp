// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "VidSubtitle.h"


//////////////////////////////////////////////////////////////////////////
CVidSubtitle::CVidSubtitle(CBGame* inGame):CBBase(inGame)
{
	m_Text = NULL;
	m_StartFrame = m_EndFrame = 0;
}


//////////////////////////////////////////////////////////////////////////
CVidSubtitle::CVidSubtitle(CBGame* inGame, char* Text, LONG StartFrame, LONG EndFrame):CBBase(inGame)
{
	m_Text = new char[strlen(Text)+1];
	strcpy(m_Text, Text);
	Game->m_StringTable->Expand(&m_Text);

	m_StartFrame = StartFrame;
	m_EndFrame = EndFrame;
}


//////////////////////////////////////////////////////////////////////////
CVidSubtitle::~CVidSubtitle()
{
	SAFE_DELETE_ARRAY(m_Text);
}
