// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "HWDeviceVideo.h"


//////////////////////////////////////////////////////////////////////////
CHWDeviceVideo::CHWDeviceVideo(CBGame* inGame):CHWDevice(inGame)
{
	m_SupportsTL = false;
	m_SupportsFullscreen = false;
	m_Monitor = NULL;
	m_MonitorIndex = -1;
	SetRectEmpty(&m_MonitorRect);

	m_HasAltRes = false;
	m_AltWidth = m_AltHeight = 0;

	m_HasDesktopRes = false;
	m_DesktopWidth = m_DesktopHeight = 0;

	m_HasBackupRes = false;
	m_BackupWidth = m_BackupHeight = 0;

	m_HasRequestedRes = false;
}


//////////////////////////////////////////////////////////////////////////
CHWDeviceVideo::~CHWDeviceVideo()
{
	for(int i=0; i<m_Modes.GetSize(); i++) delete m_Modes[i];
	m_Modes.RemoveAll();
}
