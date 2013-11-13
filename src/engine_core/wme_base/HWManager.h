// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_HWMANAGER_H__ADE09135_4482_4FFB_9F31_2DC31255BEDE__INCLUDED_)
#define AFX_HWMANAGER_H__ADE09135_4482_4FFB_9F31_2DC31255BEDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CHWManager : public CBBase  
{
public:
	class CHWMonitorInfo
	{
	public:
		HMONITOR m_Handle;
		RECT m_DesktopRect;

		CHWMonitorInfo(HMONITOR Handle, RECT DesktopRect)
		{
			m_Handle = Handle;
			m_DesktopRect = DesktopRect;
		}
	};

	HRESULT EnumMonitors();
	static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

	HRESULT GetResolution(int* LogWidth, int* LogHeight, int* RealWidth, int* RealHeight);

	bool m_ShowSettings;
	HRESULT FindStencilFormatsForDevice(LPDIRECT3D d3d, int adapter, CHWDeviceVideo* dev);
	bool FindDepthStencilFormat(LPDIRECT3D D3D, UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat);
	bool m_ForceTextureSubdivision;
	bool m_Force16bitTextures;
	bool m_AllowAdvanced3DSettings;
	bool m_AllowDesktopRes;
	HRESULT AutoselectWindowedDevice(bool ForceAccelerated, bool ForceSoftware, HWND Window=NULL);
	void ReportSelectedDevices();
	bool QueryDevices(TVideoMode ReqMode, bool ReqAccelerated, int Width, int Height, bool ForceUserQuery, bool AllowAdvanced=false, bool ReqSound=false, int TLMode=0, bool AllowDesktopRes=false);
	HRESULT LoadSettings();
	HRESULT SaveSettings();
	HRESULT ShowDialog();
	HRESULT EnumAllDevices();
	HRESULT EnumDSDevices();
	HRESULT EnumD3DDevices();
	HRESULT EnumDDDevices();
	HRESULT ClearDevices();
	CHWManager(CBGame* inGame);
	virtual ~CHWManager();
	CBArray<CHWDeviceVideo*, CHWDeviceVideo*> m_VideoDevices;
	CBArray<CHWDeviceAudio*, CHWDeviceAudio*> m_AudioDevices;
	CBArray<CHWMonitorInfo*, CHWMonitorInfo*> m_Monitors;
	bool IsOptimalAspect(float Ratio1, float Ratio2);
	bool IsSameAspect(float Ratio1, float Ratio2);
	bool IsRes16to9(float Aspect);
	bool IsRes4to3(float Aspect);

	int m_ReqWidth;
	int m_ReqHeight;
	int m_OrigReqWidth;
	int m_OrigReqHeight;

	bool m_ReqWindowedOnly;
	bool m_ReqFullscreenOnly;
	bool m_ReqAcclerated;
	bool m_ReqSound;
	int m_DepthBits;
	int m_StencilBits;

	CHWDeviceVideo* m_SelectedVideoDevice;
	CHWDeviceAudio* m_SelectedAudioDevice;
	bool m_SelectedWindowed;
	bool m_SelectedTL;
	bool m_SelectedMaintainAspect;
	bool m_SelectedDesktopRes;
	int m_TLMode;
	int m_SelectedMultisample;
	bool m_Selected16bitColors;

	// callbacks
	static BOOL WINAPI DDEnumCallbackEx(GUID *lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm);
	static BOOL WINAPI DSEnumCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext);
	static HRESULT WINAPI EnumModesCallback2(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext);	
};

#endif // !defined(AFX_HWMANAGER_H__ADE09135_4482_4FFB_9F31_2DC31255BEDE__INCLUDED_)
