// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "HWVideoMode.h"


//////////////////////////////////////////////////////////////////////////
CHWVideoMode::CHWVideoMode(CBGame* inGame):CBBase(inGame)
{
	m_Width = m_Height = 0;
	m_BPP = 0;
	memset(&m_Multisample, 0, sizeof(m_Multisample));	
	m_Windowed = false;
	m_DepthStencilFormat = D3DFMT_D16;
	m_SupportsStencil = false;
}


//////////////////////////////////////////////////////////////////////////
CHWVideoMode::~CHWVideoMode()
{

}
