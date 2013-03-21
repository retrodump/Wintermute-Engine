// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BQuickMsg.h"


//////////////////////////////////////////////////////////////////////////
CBQuickMsg::CBQuickMsg(CBGame* inGame, char* Text):CBBase(inGame)
{
	m_Text = new char [strlen(Text)+1];
	if(m_Text) strcpy(m_Text, Text);
	m_StartTime = Game->m_CurrentTime;
}


//////////////////////////////////////////////////////////////////////////
CBQuickMsg::~CBQuickMsg()
{
	if(m_Text) delete [] m_Text;
}


//////////////////////////////////////////////////////////////////////////
char* CBQuickMsg::GetText()
{
	return m_Text;
}
