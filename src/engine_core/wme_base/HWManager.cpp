// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "HWManager.h"
#include "resource.h"
#include <windowsx.h>
#include <tchar.h>
#include "DlgSettings.h"
#include "3dUtils.h"



//////////////////////////////////////////////////////////////////////////
CHWManager::CHWManager(CBGame* inGame):CBBase(inGame)
{
	m_ReqWidth = m_ReqHeight = 0;
	m_OrigReqWidth = m_OrigReqHeight = 0;
	m_ReqWindowedOnly = false;
	m_ReqFullscreenOnly = false;
	m_ReqAcclerated = false;
	m_ReqSound = false;
	m_TLMode = 0;

	m_SelectedVideoDevice = NULL;
	m_SelectedAudioDevice = NULL;
	m_SelectedWindowed = false;
	m_SelectedTL = false;
	m_SelectedMaintainAspect = false;
	m_SelectedDesktopRes = false;
	m_SelectedMultisample = 0;
	m_Selected16bitColors = true;

	m_AllowAdvanced3DSettings = false;
	m_AllowDesktopRes = false;

	m_Force16bitTextures = false;
	m_ForceTextureSubdivision = false;

	if(Game->m_MaxShadowType >= SHADOW_STENCIL)
	{
		m_DepthBits = 16;
		m_StencilBits = 4;
	}
	else
	{
		m_DepthBits = 16;
		m_StencilBits = 0;
	}

	m_ShowSettings = true;
}


//////////////////////////////////////////////////////////////////////////
CHWManager::~CHWManager()
{
	ClearDevices();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::ClearDevices()
{
	int i;

	for(i=0; i<m_VideoDevices.GetSize(); i++) delete m_VideoDevices[i];
	m_VideoDevices.RemoveAll();

	for(i=0; i<m_AudioDevices.GetSize(); i++) delete m_AudioDevices[i];
	m_AudioDevices.RemoveAll();

	for(i=0; i<m_Monitors.GetSize(); i++) delete m_Monitors[i];
	m_Monitors.RemoveAll();

	m_SelectedVideoDevice = NULL;
	m_SelectedAudioDevice = NULL;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::EnumAllDevices()
{
	m_OrigReqWidth = m_ReqWidth;
	m_OrigReqHeight = m_ReqHeight;


	HRESULT res;

	CBUtils::DebugMessage(NULL, "About to enumerate monitors...");
	res = EnumMonitors();
	if(FAILED(res)) Game->LOG(res, "Error enumerating monitors!");

	CBUtils::DebugMessage(NULL, "About to enumerate Direct3D devices...");
	res = EnumD3DDevices();
	if(FAILED(res)) Game->LOG(res, "Error enumerating Direct3D devices!");

	if(!m_ReqAcclerated)
	{
		CBUtils::DebugMessage(NULL, "About to enumerate DirectDraw devices...");
		res = EnumDDDevices();
		if(FAILED(res)) Game->LOG(res, "Error enumerating DirectDraw devices!");
	}

	CBUtils::DebugMessage(NULL, "About to enumerate DirectSound devices...");
	res = EnumDSDevices();
	if(FAILED(res)) Game->LOG(res, "Error enumerating DirectSound devices!");


/*	
	CHWDeviceVideo* dev = new CHWDeviceVideo(Game);
	dev->m_Accelerated = true;
	dev->SetDescript("virtual device");

	CHWVideoMode* mode = new CHWVideoMode(Game);
	mode->m_BPP = 16;
	mode->m_Width = 800;
	mode->m_Height = 600;
	mode->m_Windowed = true;
	
	dev->m_Modes.Add(mode);
	m_VideoDevices.Add(dev);
*/

	int i;

	float GameAspect = (float)m_OrigReqWidth / (float)m_OrigReqHeight;

	// weed devices
	for(i=0; i<m_VideoDevices.GetSize(); i++)
	{
		if(m_ReqAcclerated && !m_VideoDevices[i]->m_Accelerated)
		{
			delete m_VideoDevices[i];
			m_VideoDevices.RemoveAt(i);
			i--;
			continue;
		}


		// assign monitor rectangle
		for(int j=0; j<m_Monitors.GetSize(); j++)
		{
			if(m_Monitors[j]->m_Handle==m_VideoDevices[i]->m_Monitor)
			{
				m_VideoDevices[i]->m_MonitorRect = m_Monitors[j]->m_DesktopRect;
				m_VideoDevices[i]->m_MonitorIndex = j;
				break;
			}
		}

		int MonitorWidth = m_VideoDevices[i]->m_MonitorRect.right - m_VideoDevices[i]->m_MonitorRect.left;
		int MonitorHeight = m_VideoDevices[i]->m_MonitorRect.bottom - m_VideoDevices[i]->m_MonitorRect.top;
		float MonitorAspect;

		if(MonitorWidth == 0 || MonitorHeight == 0)
		{
			MonitorAspect = GameAspect;
		}
		else
		{
			MonitorAspect = (float)MonitorWidth / (float)MonitorHeight;
		}

		// search alternate resolution with monitor aspect ratio
		// game aspect ratio is different from monitor's
		bool SearchAltRes = (GameAspect > 1.4 && MonitorAspect < 1.4) || (GameAspect < 1.4 && MonitorAspect > 1.4);

		Game->LOG(0, "Game aspect ratio:    %f", GameAspect);
		Game->LOG(0, "Monitor aspect ratio: %f", MonitorAspect);

		// look for optimal resolution
		if(SearchAltRes)
		{	
			Game->LOG(0, "Searching for optimal resolution (device: %s)", m_VideoDevices[i]->m_Descript);

			int MinRes = 0;
			for(int j=0; j<m_VideoDevices[i]->m_Modes.GetSize(); j++)
			{				
				CHWVideoMode* mode = m_VideoDevices[i]->m_Modes[j];
				float ResAspect = (float)mode->m_Width / (float)mode->m_Height;

				if(mode->m_Width >= m_OrigReqWidth && mode->m_Height >= m_OrigReqHeight && IsOptimalAspect(ResAspect, MonitorAspect))
				{
					Game->LOG(0, "  -> Considering: %d x %d", mode->m_Width, mode->m_Height);
					if(MinRes == 0 || mode->m_Width * mode->m_Height < MinRes)
					{
						MinRes = mode->m_Width * mode->m_Height;
						m_VideoDevices[i]->m_HasAltRes = true;
						m_VideoDevices[i]->m_AltWidth = mode->m_Width;
						m_VideoDevices[i]->m_AltHeight = mode->m_Height;
					}
				}
			}
			if(!m_VideoDevices[i]->m_HasAltRes) Game->LOG(0, "Optimal resolution not found");
			else Game->LOG(0, "Optimal resolution: %d x %d", m_VideoDevices[i]->m_AltWidth, m_VideoDevices[i]->m_AltHeight);
		}
		else Game->LOG(0, "Game aspect ratio is the same as monitor aspect ratio.");
		
		// search for backup resolution
		// i.e. nearest higher res, used for full-screen if the requested res is not available
		int MinRes = 0;
		bool MinResSameAspect = false;
		for(int j=0; j<m_VideoDevices[i]->m_Modes.GetSize(); j++)
		{				
			CHWVideoMode* mode = m_VideoDevices[i]->m_Modes[j];
			float ResAspect = (float)mode->m_Width / (float)mode->m_Height;
			float ReqAspect = (float)m_OrigReqWidth / (float)m_OrigReqHeight;

			bool SameAspect = IsSameAspect(ResAspect, MonitorAspect);

			if(mode->m_Width >= m_OrigReqWidth && mode->m_Height >= m_OrigReqHeight)
			{
				// we already found same-aspect resolution, skip this one
				if(MinResSameAspect && !SameAspect) continue;

				// not found any yet or the found one is higher
				if(MinRes == 0 || mode->m_Width * mode->m_Height < MinRes || (SameAspect && !MinResSameAspect))
				{
					MinRes = mode->m_Width * mode->m_Height;
					MinResSameAspect = SameAspect;
					m_VideoDevices[i]->m_HasBackupRes = true;
					m_VideoDevices[i]->m_BackupWidth = mode->m_Width;
					m_VideoDevices[i]->m_BackupHeight = mode->m_Height;
				}
			}
		}

		if(m_VideoDevices[i]->m_HasBackupRes)
			Game->LOG(0, "Backup resolution:  %d x %d", m_VideoDevices[i]->m_BackupWidth, m_VideoDevices[i]->m_BackupHeight);
		Game->LOG(0, "");

		m_VideoDevices[i]->m_SupportsWindowed = false;
		m_VideoDevices[i]->m_SupportsFullscreen = false;
		m_VideoDevices[i]->m_Supports16bit = false;
		m_VideoDevices[i]->m_Supports32bit = false;

		// weed modes
		for(int j=0; j<m_VideoDevices[i]->m_Modes.GetSize(); j++)
		{
			CHWDeviceVideo* Dev = m_VideoDevices[i];
			CHWVideoMode* mode = Dev->m_Modes[j];

			bool SuitableRes;
			if(!mode->m_Windowed)
			{
				if(m_AllowDesktopRes && mode->m_Width == MonitorWidth && mode->m_Height == MonitorHeight)
				{
					m_VideoDevices[i]->m_HasDesktopRes = true;
					m_VideoDevices[i]->m_DesktopWidth = mode->m_Width;
					m_VideoDevices[i]->m_DesktopHeight = mode->m_Height;
					SuitableRes = true;
				}
				else
					SuitableRes = (mode->m_Width == m_ReqWidth && mode->m_Height == m_ReqHeight)
								|| (Dev->m_HasAltRes && mode->m_Width == Dev->m_AltWidth && mode->m_Height == Dev->m_AltHeight)
								|| (Dev->m_HasBackupRes && mode->m_Width == Dev->m_BackupWidth && mode->m_Height == Dev->m_BackupHeight);


				if(mode->m_Width == m_ReqWidth && mode->m_Height == m_ReqHeight)
					m_VideoDevices[i]->m_HasRequestedRes = true;
			}
			else SuitableRes = true;


			if(	(m_ReqWindowedOnly && !mode->m_Windowed) ||
				(m_ReqFullscreenOnly && mode->m_Windowed) ||
				(!SuitableRes))
			{

				delete mode;
				m_VideoDevices[i]->m_Modes.RemoveAt(j);
				j--;
				continue;
			}

			if(mode->m_Windowed) m_VideoDevices[i]->m_SupportsWindowed = true;
			else
			{
				m_VideoDevices[i]->m_SupportsFullscreen = true;
			}
			if(mode->m_BPP==16) m_VideoDevices[i]->m_Supports16bit = true;
			if(mode->m_BPP==32) m_VideoDevices[i]->m_Supports32bit = true;
		}

		// no valid modes -> remove device
		if(m_VideoDevices[i]->m_Modes.GetSize()==0)
		{
			delete m_VideoDevices[i];
			m_VideoDevices.RemoveAt(i);
			i--;
			continue;
		}
	}

	// LOG it out
	Game->LOG(0, "Available video devices:");
	for(i=0; i<m_VideoDevices.GetSize(); i++)
	{
		Game->LOG(0, "  %s (%s)", m_VideoDevices[i]->m_Descript, m_VideoDevices[i]->m_Accelerated?"accelerated":"non accelerated");
		if(m_VideoDevices[i]->m_Driver)
			Game->LOG(0, "    Driver: %s", m_VideoDevices[i]->m_Driver);
		Game->LOG(0, "    Monitor: %d", m_VideoDevices[i]->m_MonitorIndex);
	}
	Game->LOG(0, "Available audio devices:");
	for(i=0; i<m_AudioDevices.GetSize(); i++)
	{
		Game->LOG(0, "  %s", m_AudioDevices[i]->m_Descript);
		if(m_AudioDevices[i]->m_Driver)
			Game->LOG(0, "    Driver: %s", m_AudioDevices[i]->m_Driver);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CHWManager::IsOptimalAspect(float Ratio1, float Ratio2)
{
	if(Ratio1 == Ratio2) return true;
	if(IsRes16to9(Ratio1) && IsRes16to9(Ratio2)) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CHWManager::IsSameAspect(float Ratio1, float Ratio2)
{
	if(Ratio1 == Ratio2) return true;
	if(IsRes16to9(Ratio1) == IsRes16to9(Ratio2)) return true;
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CHWManager::IsRes16to9(float Aspect)
{
	if(Aspect >= (float)(16.0 / 10.0) && Aspect <= (float)(16.0 / 9.0)) return true;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CHWManager::IsRes4to3(float Aspect)
{
	if(Aspect >= 1.25 && Aspect <= 1.4) return true;
	else return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::EnumDDDevices()
{
	Game->LOG(0, "Enumerating DirectDraw devices...");
	return Game->m_DirectX.DirectDrawEnumerateEx(DDEnumCallbackEx, this, 0);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::EnumD3DDevices()
{
	Game->LOG(0, "Enumerating Direct3D devices...");

	LPDIRECT3D d3d;
	if((d3d = Game->m_DirectX.Direct3DCreate(D3D_SDK_VERSION)) == NULL) return E_FAIL;


	// for each adapter
	for(int adapter=0; adapter<d3d->GetAdapterCount(); adapter++)
	{
		CHWDeviceVideo* dev = new CHWDeviceVideo(Game);
		if(!dev) continue;

		dev->m_AdapterID = adapter;
		dev->m_Accelerated = true;

		D3DADAPTER_IDENTIFIER id;

		d3d->GetAdapterIdentifier(adapter, D3DENUM_NO_WHQL_LEVEL, &id);
		dev->m_Guid = id.DeviceIdentifier;
		dev->SetDescript(id.Description);
		dev->SetDriver(id.Driver, id.DriverVersion);
		dev->m_Monitor = d3d->GetAdapterMonitor(adapter);

		m_VideoDevices.Add(dev);
			
		D3DCAPS caps;
		d3d->GetDeviceCaps(adapter, D3DDEVTYPE_HAL, &caps);

		// HW T&L?
		if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			dev->m_SupportsTL = true;
		}

		// add desktop mode
#ifndef WME_D3D9 // D3D9 devices always support windowed
		if(caps.Caps2 & D3DCAPS2_CANRENDERWINDOWED)
#endif
		{
			D3DDISPLAYMODE desktop;
			d3d->GetAdapterDisplayMode(adapter, &desktop);
			CHWVideoMode* DesktopMode = new CHWVideoMode(Game);
			DesktopMode->m_Format = desktop.Format;
			DesktopMode->m_Width = DesktopMode->m_Height = 0;
			DesktopMode->m_BPP = 0;
			DesktopMode->m_Windowed = true;
			dev->m_Modes.Add(DesktopMode);
		}

		if(!m_ReqWindowedOnly)
		{
#ifdef WME_D3D9
			D3DFORMAT AllowedAdapterFormatArray[] = { D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5 };
#else
			D3DFORMAT AllowedAdapterFormatArray[] = { D3DFMT_UNKNOWN }; // not used in D3D8
#endif
			DWORD AllowedAdapterFormatArraySize = sizeof(AllowedAdapterFormatArray) / sizeof(AllowedAdapterFormatArray[0]);

			for(UINT iaaf = 0; iaaf < AllowedAdapterFormatArraySize; iaaf++)
			{
				D3DFORMAT AdapterFormat = AllowedAdapterFormatArray[iaaf];

				// for each adapter mode
				DWORD NumAdapterModes = C3DUtils::GetAdapterModeCount(d3d, adapter, AdapterFormat);
				for(int mode = 0; mode < NumAdapterModes; mode++)
				{
					D3DDISPLAYMODE DisplayMode;
					C3DUtils::EnumAdapterModes(d3d, adapter, AdapterFormat, mode, &DisplayMode);

					// Check if the mode already exists (to filter out refresh rates)
					DWORD m;
					for(m=0; m<dev->m_Modes.GetSize(); m++)
					{
						if((dev->m_Modes[m]->m_Width == DisplayMode.Width) && (dev->m_Modes[m]->m_Height == DisplayMode.Height) && (dev->m_Modes[m]->m_Format == DisplayMode.Format) && !dev->m_Modes[m]->m_Windowed) break;
					}

					// If we found a new mode, add it to the list of modes
					if(m == dev->m_Modes.GetSize())
					{
						CHWVideoMode* mode = new CHWVideoMode(Game);
						if(mode)
						{
							mode->m_Width		= DisplayMode.Width;
							mode->m_Height		= DisplayMode.Height;
							mode->m_Format		= DisplayMode.Format;
							mode->m_Windowed    = false;

							dev->m_Modes.Add(mode);
						}
					}
				}
			}

			dev->m_Supports16bit=false;
			dev->m_Supports32bit=false;
			dev->m_SupportsWindowed=false;

			for(int i=0; i<dev->m_Modes.GetSize(); i++)
			{
				// is it a valid mode?
				if(FAILED(d3d->CheckDeviceType(adapter, D3DDEVTYPE_HAL, dev->m_Modes[i]->m_Format, dev->m_Modes[i]->m_Format, dev->m_Modes[i]->m_Windowed)))
				{
					delete dev->m_Modes[i];
					dev->m_Modes.RemoveAt(i);
					i--;
					continue;
				}

				// check supported bit depth
				if(	dev->m_Modes[i]->m_Format == D3DFMT_X8R8G8B8 ||
					dev->m_Modes[i]->m_Format == D3DFMT_A8R8G8B8 ||
					dev->m_Modes[i]->m_Format == D3DFMT_R8G8B8 )
				{

					dev->m_Supports32bit = true;
					dev->m_Modes[i]->m_BPP = 32;
				}
				else if(
					dev->m_Modes[i]->m_Format == D3DFMT_R5G6B5 ||
					dev->m_Modes[i]->m_Format == D3DFMT_X1R5G5B5 ||
					dev->m_Modes[i]->m_Format == D3DFMT_A1R5G5B5)
				{

					dev->m_Supports16bit = true;
					dev->m_Modes[i]->m_BPP = 16;
				}

				if(dev->m_Modes[i]->m_Windowed) dev->m_SupportsWindowed = true;

				// check for stencil support
				FindStencilFormatsForDevice(d3d, adapter, dev);

				// check supported multisample types
				for(int j=0; j<=16; j++)
				{
					if(j==1) continue;
#ifdef WME_D3D9
					dev->m_Modes[i]->m_Multisample[j] = SUCCEEDED(d3d->CheckDeviceMultiSampleType(adapter, D3DDEVTYPE_HAL, dev->m_Modes[i]->m_Format, dev->m_Modes[i]->m_Windowed, (D3DMULTISAMPLE_TYPE)j, NULL));
#else
					dev->m_Modes[i]->m_Multisample[j] = SUCCEEDED(d3d->CheckDeviceMultiSampleType(adapter, D3DDEVTYPE_HAL, dev->m_Modes[i]->m_Format, dev->m_Modes[i]->m_Windowed, (D3DMULTISAMPLE_TYPE)j));
#endif
				}
			}
		}
		else
		{
			// find stencil format for desktop device			
			FindStencilFormatsForDevice(d3d, adapter, dev);
		}
	}

	RELEASE(d3d);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::FindStencilFormatsForDevice(LPDIRECT3D d3d, int adapter, CHWDeviceVideo* dev)
{
	for(int i=0; i<dev->m_Modes.GetSize(); i++)
	{
		// check for stencil support
		D3DFORMAT DepthStencilFormat;
		if(FindDepthStencilFormat(d3d, adapter, D3DDEVTYPE_HAL, dev->m_Modes[i]->m_Format, &DepthStencilFormat))
		{
			dev->m_Modes[i]->m_DepthStencilFormat = DepthStencilFormat;
			dev->m_Modes[i]->m_SupportsStencil = true;
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CHWManager::FindDepthStencilFormat(LPDIRECT3D D3D, UINT iAdapter, D3DDEVTYPE DeviceType,
											 D3DFORMAT TargetFormat,
											 D3DFORMAT* pDepthStencilFormat)
{
	if(m_DepthBits <= 16 && m_StencilBits == 0)
	{
		if( SUCCEEDED(D3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
		{
			if( SUCCEEDED(D3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D16;
				return true;
			}
		}
	}

	if(m_DepthBits <= 15 && m_StencilBits <= 1)
	{
		if( SUCCEEDED( D3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
		{
			if( SUCCEEDED( D3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D15S1;
				return true;
			}
		}
	}

	if(m_DepthBits <= 24 && m_StencilBits == 0)
	{
		if( SUCCEEDED(D3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
		{
			if( SUCCEEDED(D3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D24X8;
				return true;
			}
		}
	}

	if(m_DepthBits <= 24 && m_StencilBits <= 8)
	{
		if( SUCCEEDED( D3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
		{
			if( SUCCEEDED( D3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D24S8;
				return true;
			}
		}
	}

	if(m_DepthBits <= 24 && m_StencilBits <= 4)
	{
		if( SUCCEEDED( D3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
		{
			if( SUCCEEDED( D3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D24X4S4;
				return true;
			}
		}
	}

	if(m_DepthBits <= 32 && m_StencilBits == 0)
	{
		if( SUCCEEDED( D3D->CheckDeviceFormat( iAdapter, DeviceType,
			TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
		{
			if( SUCCEEDED( D3D->CheckDepthStencilMatch( iAdapter, DeviceType,
				TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
			{
				*pDepthStencilFormat = D3DFMT_D32;
				return true;
			}
		}
	}

	return false;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::EnumDSDevices()
{
	Game->LOG(0, "Enumerating DirectSound devices...");
	HRESULT res =  Game->m_DirectX.DirectSoundEnumerate(DSEnumCallback, this);
	if(FAILED(res)) return res;

	if(!m_ReqSound)
	{
		CHWDeviceAudio* dev = new CHWDeviceAudio(Game);
		if(dev)
		{
			dev->SetDescript((char*)Game->m_StringTable->ExpandStatic("/syseng0019/[no sound]"));
			dev->m_NoSound = true;
			m_AudioDevices.Add(dev);
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
BOOL WINAPI CHWManager::DDEnumCallbackEx(GUID *lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm)
{
	CHWManager* manager = (CHWManager*)lpContext;

    LPDIRECTDRAW7 pDD = NULL;	
    HRESULT hr;

    if(FAILED(hr = manager->Game->m_DirectX.DirectDrawCreateEx(lpGUID, (VOID**)&pDD, IID_IDirectDraw7, NULL)))
        return DDENUMRET_CANCEL;

	CHWDeviceVideo* dev = new CHWDeviceVideo(manager->Game);
	dev->m_Width = manager->m_ReqWidth;
	dev->m_Height = manager->m_ReqHeight;
	dev->m_AdapterID = -1;
	dev->m_SupportsWindowed = true;
	dev->m_Accelerated = false;
	dev->m_Monitor = hm;

	// always add windowed mode
	CHWVideoMode* mode = new CHWVideoMode(dev->Game);
	if(mode)
	{
		mode->m_Windowed = true;
		mode->m_BPP = 0;
		dev->m_Modes.Add(mode);
	}

	pDD->EnumDisplayModes(0, NULL, dev, EnumModesCallback2);

	DDDEVICEIDENTIFIER2 id;
	pDD->GetDeviceIdentifier(&id, 0);

	if(lpGUID) dev->m_Guid = *lpGUID;
	else memset(&dev->m_Guid, 0, sizeof(GUID));
	dev->SetDescript(id.szDescription);
	//dev->SetDriver(id.szDriver, id.liDriverVersion);
	manager->m_VideoDevices.Add(dev);

    RELEASE(pDD); 

    return DDENUMRET_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT WINAPI CHWManager::EnumModesCallback2(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext)
{
	CHWDeviceVideo* dev = (CHWDeviceVideo*)lpContext;

	CHWVideoMode* mode = new CHWVideoMode(dev->Game);
	if(mode)
	{
		mode->m_Width  = lpDDSurfaceDesc->dwWidth;
		mode->m_Height = lpDDSurfaceDesc->dwHeight;
		mode->m_Windowed = false;
		mode->m_BPP = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;

		if(mode->m_BPP == 16) dev->m_Supports16bit = true;
		if(mode->m_BPP == 32) dev->m_Supports32bit = true;

		dev->m_Modes.Add(mode);
	}

	return DDENUMRET_OK;
}


//////////////////////////////////////////////////////////////////////////
BOOL WINAPI CHWManager::DSEnumCallback(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
	CHWManager* manager = (CHWManager*)lpContext;

	CHWDeviceAudio* dev = new CHWDeviceAudio(manager->Game);

	if(lpGuid) dev->m_Guid = *lpGuid;
	else memset(&dev->m_Guid, 0, sizeof(GUID));
	dev->SetDescript((char*)lpcstrDescription);
	dev->m_NoSound = false;

	manager->m_AudioDevices.Add(dev);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::ShowDialog()
{
	CDlgSettings* Dlg = new CDlgSettings(Game, this);
	Dlg->CreateIndirect(IDD_WMESETTINGS, NULL, NULL, Game->m_TextRTL);
	int Res = Dlg->DoModal();
	delete Dlg;
	
	return Res==IDOK?S_OK:E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::SaveSettings()
{
	if(!m_SelectedVideoDevice || !m_SelectedAudioDevice) return E_FAIL;

	LPOLESTR  olestr;
	char str[50];

	if(m_SelectedVideoDevice->m_SupportsTL && m_TLMode==1) m_SelectedTL = true;

	// video
	StringFromIID(m_SelectedVideoDevice->m_Guid, &olestr);
	wcstombs(str, olestr, 50);
	CoTaskMemFree(olestr);
	Game->m_Registry->WriteString("Video", "DeviceGuid",  str);
	Game->m_Registry->WriteInt   ("Video", "DeviceId",    m_SelectedVideoDevice->m_AdapterID);
	Game->m_Registry->WriteString("Video", "DeviceName",  m_SelectedVideoDevice->m_Descript);
	Game->m_Registry->WriteBool  ("Video", "16bit",       m_Selected16bitColors);
	Game->m_Registry->WriteBool  ("Video", "Windowed",    m_SelectedWindowed);
	Game->m_Registry->WriteBool  ("Video", "Accelerated", m_SelectedVideoDevice->m_Accelerated);
	Game->m_Registry->WriteBool  ("Video", "HardwareT&L", m_SelectedTL);
	Game->m_Registry->WriteInt   ("Video", "Multisample", m_SelectedMultisample);
	Game->m_Registry->WriteInt   ("Video", "MaintainAspect", m_SelectedMaintainAspect);
	Game->m_Registry->WriteInt   ("Video", "DesktopRes",  m_SelectedDesktopRes);

	Game->m_Registry->WriteBool  ("Video", "Force16bitTextures",      m_Force16bitTextures);
	Game->m_Registry->WriteBool  ("Video", "ForceTextureSubdivision", m_ForceTextureSubdivision);


	// audio
	StringFromIID(m_SelectedAudioDevice->m_Guid, &olestr);
	wcstombs(str, olestr, 50);
	CoTaskMemFree(olestr);
	Game->m_Registry->WriteString("Audio", "DeviceGuid",  str);
	Game->m_Registry->WriteString("Audio", "DeviceName",  m_SelectedAudioDevice->m_Descript);
	Game->m_Registry->WriteBool  ("Audio", "NoSound",     m_SelectedAudioDevice->m_NoSound);

	// system
	Game->m_Registry->WriteBool  ("System", "ShowSettings", m_ShowSettings);

	// accessibility
	Game->m_Registry->WriteBool  ("Accessibility", "TTSEnabled", Game->m_AccessTTSEnabled);
	Game->m_Registry->WriteBool  ("Accessibility", "TTSCaptions", Game->m_AccessTTSCaptions);
	Game->m_Registry->WriteBool  ("Accessibility", "TTSTalk", Game->m_AccessTTSTalk);
	Game->m_Registry->WriteBool  ("Accessibility", "TTSKeypress", Game->m_AccessTTSKeypress);

	Game->m_Registry->WriteBool  ("Accessibility", "KeyboardEnabled", Game->m_AccessKeyboardEnabled);
	Game->m_Registry->WriteBool  ("Accessibility", "KeyboardCursorSkip", Game->m_AccessKeyboardCursorSkip);
	Game->m_Registry->WriteBool  ("Accessibility", "KeyboardPause", Game->m_AccessKeyboardPause);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::LoadSettings()
{
	OLECHAR  olestr[50];
	char str[50];
	DWORD length = 50;
	GUID guid;
	int DeviceID = 0;
	int i;

	// video
	bool accelerated;
	Game->m_Registry->ReadString("Video", "DeviceGuid", str, &length, "");
	mbstowcs(olestr, str, 50);
	IIDFromString(olestr, &guid);
	
	DeviceID              = Game->m_Registry->ReadInt ("Video", "DeviceId", 0);
	m_Selected16bitColors = Game->m_Registry->ReadBool("Video", "16bit",       false); // default to 32 bit
	m_SelectedWindowed    = Game->m_Registry->ReadBool("Video", "Windowed",    false);
	accelerated           = Game->m_Registry->ReadBool("Video", "Accelerated", true);
	m_SelectedTL          = Game->m_Registry->ReadBool("Video", "HardwareT&L", false);
	m_SelectedMaintainAspect = Game->m_Registry->ReadBool("Video", "MaintainAspect", true);
	m_SelectedDesktopRes  = Game->m_Registry->ReadBool("Video", "DesktopRes", false);
	m_SelectedMultisample = Game->m_Registry->ReadInt ("Video", "Multisample", 0);

	//m_Force16bitTextures      = Game->m_Registry->ReadBool("Video", "Force16bitTextures",      false);
	//m_ForceTextureSubdivision = Game->m_Registry->ReadBool("Video", "ForceTextureSubdivision", false);
	m_Force16bitTextures = false;
	m_ForceTextureSubdivision = false;

	if(!m_AllowAdvanced3DSettings)
	{
		m_SelectedMultisample = 0;
	}

	if(m_TLMode==0) m_SelectedTL = false;


	// try to find the device
	m_SelectedVideoDevice = NULL;
	for(i=0; i<m_VideoDevices.GetSize(); i++)
	{
		if(m_VideoDevices[i]->m_Accelerated != accelerated) continue;
		if(m_VideoDevices[i]->m_Guid!=guid) continue;
		if(m_VideoDevices[i]->m_Guid==guid && m_VideoDevices[i]->m_AdapterID != DeviceID) continue;
		if(!m_SelectedWindowed && m_Selected16bitColors && !m_VideoDevices[i]->m_Supports16bit) continue;
		if(!m_SelectedWindowed && !m_Selected16bitColors && !m_VideoDevices[i]->m_Supports32bit) continue;
		if(m_SelectedWindowed && !m_VideoDevices[i]->m_SupportsWindowed) continue;
		if(m_SelectedTL && !m_VideoDevices[i]->m_SupportsTL) continue;

		m_SelectedVideoDevice = m_VideoDevices[i];
		break;
	}


	// check required capabilities
	if(m_SelectedVideoDevice)
	{
		if(m_ReqAcclerated && !m_SelectedVideoDevice->m_Accelerated) m_SelectedVideoDevice = NULL;
		else if(m_ReqWindowedOnly && !m_SelectedVideoDevice->m_SupportsWindowed) m_SelectedVideoDevice = NULL;
		else if(m_ReqFullscreenOnly && !m_SelectedVideoDevice->m_SupportsFullscreen) m_SelectedVideoDevice = NULL;
		else if(!m_SelectedWindowed)
		{
			bool ModeFound = false;
			for(i=0; i<m_SelectedVideoDevice->m_Modes.GetSize(); i++)
			{
				CHWVideoMode* mode = m_SelectedVideoDevice->m_Modes[i];
				if(m_Selected16bitColors  && mode->m_BPP!=16) continue;
				if(!m_Selected16bitColors && mode->m_BPP!=32) continue;

				bool SuitableRes = (mode->m_Width == m_ReqWidth && mode->m_Height == m_ReqHeight)
					|| (m_SelectedVideoDevice->m_HasAltRes && mode->m_Width == m_SelectedVideoDevice->m_AltWidth && mode->m_Height == m_SelectedVideoDevice->m_AltHeight)
					|| (m_SelectedVideoDevice->m_HasBackupRes && mode->m_Width == m_SelectedVideoDevice->m_BackupWidth && mode->m_Height == m_SelectedVideoDevice->m_BackupHeight);

				if(SuitableRes)
				{
					ModeFound = true;
					break;
				}
			}
			if(!ModeFound) m_SelectedVideoDevice = NULL;
		}
	}

	// audio
	length = 50;
	Game->m_Registry->ReadString("Audio", "DeviceGuid", str, &length, "");
	mbstowcs(olestr, str, 50);
	IIDFromString(olestr, &guid);
	bool NoSound = Game->m_Registry->ReadBool("Audio", "NoSound", false);

	// try to find the device
	m_SelectedAudioDevice = NULL;
	for(i=0; i<m_AudioDevices.GetSize(); i++)
	{
		if(m_AudioDevices[i]->m_NoSound != NoSound) continue;
		if(m_AudioDevices[i]->m_Guid!=guid) continue;

		m_SelectedAudioDevice = m_AudioDevices[i];
		break;
	}


	// accessibility
	Game->m_AccessTTSEnabled         = Game->m_Registry->ReadBool("Accessibility", "TTSEnabled", Game->m_AccessTTSEnabled);
	Game->m_AccessTTSCaptions        = Game->m_Registry->ReadBool("Accessibility", "TTSCaptions", Game->m_AccessTTSCaptions);
	Game->m_AccessTTSTalk            = Game->m_Registry->ReadBool("Accessibility", "TTSTalk", Game->m_AccessTTSTalk);
	Game->m_AccessTTSKeypress        = Game->m_Registry->ReadBool("Accessibility", "TTSKeypress", Game->m_AccessTTSKeypress);

	Game->m_AccessKeyboardEnabled    = Game->m_Registry->ReadBool("Accessibility", "KeyboardEnabled", Game->m_AccessKeyboardEnabled);
	Game->m_AccessKeyboardCursorSkip = Game->m_Registry->ReadBool("Accessibility", "KeyboardCursorSkip", Game->m_AccessKeyboardCursorSkip);
	Game->m_AccessKeyboardPause      = Game->m_Registry->ReadBool("Accessibility", "KeyboardPause", Game->m_AccessKeyboardPause);


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CHWManager::QueryDevices(TVideoMode ReqMode, bool ReqAccelerated, int Width, int Height, bool ForceUserQuery, bool AllowAdvanced, bool ReqSound, int TLMode, bool AllowDesktopRes)
{
	m_ShowSettings = Game->m_Registry->ReadBool("System", "ShowSettings", true);

	// did we crash last time?
	if(!Game->m_Registry->ReadBool("System", "LastRun", false))
	{
		Game->LOG(0, "First run / crash recovery");
		ForceUserQuery = true;
	}
	Game->m_Registry->WriteBool("System", "LastRun", false);

	switch(ReqMode)
	{
		case VIDEO_WINDOW:
			m_ReqWindowedOnly = true;
			m_ReqFullscreenOnly = false;
		break;
		case VIDEO_FULLSCREEN:
			m_ReqWindowedOnly = false;
			m_ReqFullscreenOnly = true;
		break;
		default:
			m_ReqWindowedOnly = false;
			m_ReqFullscreenOnly = false;
		break;
	}
	
	m_ReqWidth = Width;
	m_ReqHeight = Height;

	m_ReqAcclerated = ReqAccelerated;
	m_ReqSound = ReqSound;
	m_AllowAdvanced3DSettings = AllowAdvanced;
	m_TLMode = TLMode;
	m_AllowDesktopRes = AllowDesktopRes;

	if(FAILED(EnumAllDevices())) return false;
	if(FAILED(LoadSettings())) return false;

	Game->LOG(0, "");
	bool ret;
	if(!m_SelectedVideoDevice || !m_SelectedAudioDevice || ForceUserQuery || m_ShowSettings)
	{		
		if(FAILED(ShowDialog()))
		{
			Game->LOG(0, "Device selection cancelled by user");
			return false;
		}

		SaveSettings();
		Game->LOG(0, "User selected:");
		ret = true;

		// disable accessibility if the tab is diabled
		if(!Game->m_SettingsAllowAccessTab)
		{
			Game->m_AccessTTSEnabled = false;
			Game->m_AccessKeyboardEnabled = false;
			Game->m_AccessKeyboardPause = false;
		}
	}
	else
	{
		Game->LOG(0, "Auto selecting devices:");
		ret = true;
	}


	// override resolution
	if(m_SelectedVideoDevice && !m_SelectedWindowed)
	{
		if(m_SelectedDesktopRes && m_SelectedVideoDevice->m_HasDesktopRes)
		{
			m_ReqWidth = m_SelectedVideoDevice->m_DesktopWidth;
			m_ReqHeight = m_SelectedVideoDevice->m_DesktopHeight;
			Game->LOG(0, "  Using desktop resolution (%d x %d)", m_ReqWidth, m_ReqHeight);
		}
		else if(m_SelectedMaintainAspect && m_SelectedVideoDevice->m_HasAltRes)
		{
			m_ReqWidth = m_SelectedVideoDevice->m_AltWidth;
			m_ReqHeight = m_SelectedVideoDevice->m_AltHeight;
			Game->LOG(0, "  Using optimal resolution (%d x %d)", m_ReqWidth, m_ReqHeight);
		}
		else if(!m_SelectedVideoDevice->m_HasRequestedRes && m_SelectedVideoDevice->m_HasBackupRes)
		{
			m_ReqWidth = m_SelectedVideoDevice->m_BackupWidth;
			m_ReqHeight = m_SelectedVideoDevice->m_BackupHeight;
			Game->LOG(0, "  Using backup resolution (%d x %d)", m_ReqWidth, m_ReqHeight);
		}
		else Game->LOG(0, "  Using requested resolution (%d x %d)", m_ReqWidth, m_ReqHeight);
	}


	// write selected devices to LOG
	if(ret) ReportSelectedDevices();

	return ret;
}


//////////////////////////////////////////////////////////////////////////
void CHWManager::ReportSelectedDevices()
{
	if(!m_SelectedVideoDevice || !m_SelectedAudioDevice) return;

	Game->LOG(0, "  Video: %s (%s)", m_SelectedVideoDevice->m_Descript, m_SelectedVideoDevice->m_Accelerated?"accelerated":"non accelerated");
	if(m_SelectedVideoDevice->m_Accelerated)
	{
		Game->LOG (0, "         Windowed:%s  Colors:%s  T&L:%s  Multisample:%d", m_SelectedWindowed?"yes":"no", m_Selected16bitColors?"16bit":"32bit", m_SelectedTL?"yes":"no", m_SelectedMultisample);
		if(m_Force16bitTextures)
			Game->LOG (0, "         *Force 16bit textures");
		if(m_ForceTextureSubdivision)
			Game->LOG (0, "         *Force texture subdivision");
	}
	else Game->LOG(0, "         Windowed:%s  Colors:%s", m_SelectedWindowed?"yes":"no", m_Selected16bitColors?"16bit":"32bit");
	
	Game->LOG(0, "  Audio: %s", m_SelectedAudioDevice->m_Descript);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::AutoselectWindowedDevice(bool ForceAccelerated, bool ForceSoftware, HWND Window)
{
	HMONITOR Mon = NULL;
	if(Window) Mon = MonitorFromWindow(Window, MONITOR_DEFAULTTONEAREST);
	else Mon = MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY);


	m_ReqWindowedOnly = true;
	m_ReqFullscreenOnly = false;
	m_ReqWidth = m_ReqHeight = 0;

	m_SelectedVideoDevice = NULL;
	m_SelectedAudioDevice = NULL;

	m_ReqAcclerated = ForceAccelerated;
	m_ReqSound = false;

	if(FAILED(EnumAllDevices())) return E_FAIL;

	int i;

	// if we don't want accelerated devices, weed them out
	if(ForceSoftware)
	{
		for(i=0; i<m_VideoDevices.GetSize(); i++)
		{
			if(m_VideoDevices[i]->m_Accelerated)
			{
				delete m_VideoDevices[i];
				m_VideoDevices.RemoveAt(i);
				i--;
				continue;
			}
		}
	}

	if(m_VideoDevices.GetSize()>0)
	{
		m_SelectedVideoDevice = NULL;
		// try to find the device for a given monitor
		for(i=0; i<m_VideoDevices.GetSize(); i++)
		{
			if(m_VideoDevices[i]->m_Monitor == Mon)
			{
				m_SelectedVideoDevice = m_VideoDevices[i];
				break;
			}
		}
		
		if(!m_SelectedVideoDevice) m_SelectedVideoDevice = m_VideoDevices[0];
		m_SelectedAudioDevice = m_AudioDevices[0]; // always available
		m_Selected16bitColors = true; // does't matter
		m_SelectedMultisample = 0;
		m_SelectedTL = false;
		m_SelectedMaintainAspect = false;
		m_SelectedWindowed = true;

		Game->LOG(0, "Auto selecting devices for windowed mode:");
		ReportSelectedDevices();
		
		return S_OK;
	}
	else return E_FAIL;

}


//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::EnumMonitors()
{
	EnumDisplayMonitors(NULL, NULL, CHWManager::MonitorEnumProc, (LPARAM)this);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CHWManager::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CHWManager* _this = (CHWManager*)dwData;

	CHWMonitorInfo* Info = new CHWMonitorInfo(hMonitor, *lprcMonitor);
	_this->m_Monitors.Add(Info);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CHWManager::GetResolution(int* LogWidth, int* LogHeight, int* RealWidth, int* RealHeight)
{
	*LogWidth = m_OrigReqWidth;
	*LogHeight = m_OrigReqHeight;

	*RealWidth = m_ReqWidth;
	*RealHeight = m_ReqHeight;

	return S_OK;
}