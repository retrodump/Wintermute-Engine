// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_HWDEVICEVIDEO_H__18D46BD9_E927_432E_8944_6CF741A1AC77__INCLUDED_)
#define AFX_HWDEVICEVIDEO_H__18D46BD9_E927_432E_8944_6CF741A1AC77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HWDevice.h"

class CHWDeviceVideo : public CHWDevice  
{
public:
	bool m_SupportsTL;
	CHWDeviceVideo(CBGame* inGame);
	virtual ~CHWDeviceVideo();
	CBArray<CHWVideoMode*, CHWVideoMode*> m_Modes;
	bool m_SupportsFullscreen;
	HMONITOR m_Monitor;
	RECT m_MonitorRect;
	int m_MonitorIndex;
	bool m_HasAltRes;
	int m_AltWidth;
	int m_AltHeight;

	bool m_HasDesktopRes;
	int m_DesktopWidth;
	int m_DesktopHeight;

	bool m_HasBackupRes;
	int m_BackupWidth;
	int m_BackupHeight;

	bool m_HasRequestedRes;

};

#endif // !defined(AFX_HWDEVICEVIDEO_H__18D46BD9_E927_432E_8944_6CF741A1AC77__INCLUDED_)
