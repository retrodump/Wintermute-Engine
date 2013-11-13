// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BFileEntry.h"


//////////////////////////////////////////////////////////////////////////
CBFileEntry::CBFileEntry(CBGame* inGame):CBBase(inGame)
{
	m_Package = NULL;
	m_Length = m_CompressedLength = m_Offset = m_Flags = 0;
	m_Filename = "";

	m_TimeDate1 = m_TimeDate2 = 0;

	m_JournalTime = 0;
}


//////////////////////////////////////////////////////////////////////////
CBFileEntry::~CBFileEntry()
{
	m_Package = NULL; // ref only
}
