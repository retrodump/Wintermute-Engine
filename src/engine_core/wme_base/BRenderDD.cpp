// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BRenderDD.h"


//////////////////////////////////////////////////////////////////////////
CBRenderDD::CBRenderDD(CBGame* inGame):CBRenderer(inGame)
{
	m_DirectDraw = NULL;
	m_FrontBuffer = NULL;
	m_BackBuffer = NULL;
	m_WinBackBufferSysMem = false;
	m_Palette = NULL;
	m_PaletteFile = NULL;
	m_SupportsWideSurfaces = false;

	m_LastFillR = m_LastFillG = m_LastFillB = 0;
	m_LastFillNative = 0;
}


//////////////////////////////////////////////////////////////////////////
CBRenderDD::~CBRenderDD()
{
	ReleaseAll();
	RELEASE(m_DirectDraw);
	if(m_PaletteFile!=NULL) delete [] m_PaletteFile;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::InitSurfaces()
{
    HRESULT	ret;
    DDSURFACEDESC2 ddsd;
    DDSCAPS2 ddscaps;
    LPDIRECTDRAWCLIPPER Clipper;

    if (m_Windowed){
        // Get normal windowed mode
        ret = m_DirectDraw->SetCooperativeLevel(m_Window, DDSCL_NORMAL);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - SetCooperativeLevel failed");
			return ret;
		}

    	// Get the dimensions of the viewport and screen bounds
    	GetClientRect(m_ClipperWindow, &m_ViewportRect);
    	GetClientRect(m_ClipperWindow, &m_ScreenRect);
    	ClientToScreen(m_Window, (POINT*)&m_ScreenRect.left);
    	ClientToScreen(m_Window, (POINT*)&m_ScreenRect.right);

        // Create the primary surface
        ZeroMemory(&ddsd,sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        ret = m_DirectDraw->CreateSurface(&ddsd, &m_FrontBuffer, NULL);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - CreateSurface for front buffer failed");
			return ret;
		}

		m_FrontBuffer->GetSurfaceDesc(&ddsd);

        // Create a clipper object since this is for a Windowed render
        ret = m_DirectDraw->CreateClipper(0, &Clipper, NULL);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - CreateClipper failed");
			return ret;
		}

        // Associate the clipper with the window
        Clipper->SetHWnd(0, m_ClipperWindow);
        m_FrontBuffer->SetClipper(Clipper);
        Clipper->Release();
        Clipper = NULL;

        // Get the backbuffer. For fullscreen mode, the backbuffer was created
        // along with the primary, but windowed mode still needs to create one.
		m_Width = m_ViewportRect.right - m_ViewportRect.left;
		m_Height = m_ViewportRect.bottom - m_ViewportRect.top;

        ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
        ddsd.dwWidth        = m_Width;
        ddsd.dwHeight       = m_Height;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

		//TODO: experimental
		if(m_WinBackBufferSysMem) ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

        ret = m_DirectDraw->CreateSurface(&ddsd, &m_BackBuffer, NULL);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - CreateSurface for back buffer failed");
			return ret;
		}
    }
    else // full screen
    {
        // Get exclusive mode
        ret = m_DirectDraw->SetCooperativeLevel(m_Window, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - SetCooperativeLevel failed");
			return ret;
		}

        // Set the video mode
        ret = m_DirectDraw->SetDisplayMode(m_Width, m_Height, m_BPP, 0, 0);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - SetDisplayMode failed (%dx%dx%d)", m_Width, m_Height, m_BPP);
			return ret;
		}

		// Get the dimensions of the viewport and screen bounds
		// Store the rectangle which contains the renderer
    	SetRect(&m_ViewportRect, 0, 0, m_Width, m_Height);
    	memcpy(&m_ScreenRect, &m_ViewportRect, sizeof(RECT));

        // Create the primary surface with 1 back buffer
        ZeroMemory(&ddsd,sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
        ddsd.dwBackBufferCount = 1;
        ret = m_DirectDraw->CreateSurface(&ddsd, &m_FrontBuffer, NULL);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - CreateSurface for full screen failed");
			return ret;
		}


        ZeroMemory(&ddscaps, sizeof(ddscaps));
        ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
        ret = m_FrontBuffer->GetAttachedSurface(&ddscaps, &m_BackBuffer);
        if (ret != DD_OK){
            Game->LOG(ret, "CBRenderDD::InitSurfaces - GetAttachedSurface failed");
			return ret;
		}
    }


	// set palette, if 8bit surface
	m_FrontBuffer->GetSurfaceDesc(&ddsd);
	if(ddsd.ddpfPixelFormat.dwRGBBitCount == 8){
		if(m_PaletteFile==NULL){
			Game->LOG(0, "CBRenderDD warning: primary surface is palettized and m_PaletteFile==NULL");
		}
		else{
			CBImage* pal_img = new CBImage(Game);
			ret = pal_img->LoadFile(m_PaletteFile);
	        if (ret != S_OK){
			    Game->LOG(ret, "CBRenderDD::InitSurfaces - can't load palette '%s'", m_PaletteFile);
				delete pal_img;
				return ret;
			}
			m_Palette = pal_img->CreatePalette();
			delete pal_img;
			if(m_Palette == NULL || m_FrontBuffer->SetPalette(m_Palette) != DD_OK){
			    Game->LOG(0, "CBRenderDD::InitSurfaces - can't set palette '%s'", m_PaletteFile);
				return E_FAIL;	
			}				
		}
	}



    return Game->m_SurfaceStorage->RestoreAll();
}


//////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::ReleaseAll()
{
    if(m_DirectDraw != NULL){
        m_DirectDraw->SetCooperativeLevel(m_Window, DDSCL_NORMAL);
		RELEASE(m_Palette);
        RELEASE(m_BackBuffer);
		RELEASE(m_FrontBuffer);
    }
    return S_OK;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBRenderDD::GetFreeVideoMem()
{
	DWORD ret_val=0;

	DDSCAPS2 ddsCaps;
	ZeroMemory(&ddsCaps, sizeof(ddsCaps));
	ddsCaps.dwCaps = DDSCAPS_TEXTURE;

	m_DirectDraw->GetAvailableVidMem(&ddsCaps, NULL, &ret_val);

	return ret_val;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBRenderDD::GetUsedVideoMem()
{
	DWORD ret_val=0, dummy=0;

	DDSCAPS2 ddsCaps;
	ZeroMemory(&ddsCaps, sizeof(ddsCaps));
	ddsCaps.dwCaps = DDSCAPS_TEXTURE;

	m_DirectDraw->GetAvailableVidMem(&ddsCaps, &dummy, &ret_val);

	return dummy - ret_val;
}


//////////////////////////////////////////////////////////////////////
void CBRenderDD::SetPaletteFile(char* Filename){
	if(m_PaletteFile != NULL) delete [] m_PaletteFile;

	m_PaletteFile = new char[strlen(Filename)+1];
	strcpy(m_PaletteFile, Filename);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::SwitchFullscreen()
{	
    HRESULT ret;

    // Release all objects that need to be re-created for the new device
    if (FAILED(ret = ReleaseAll())){
		Game->LOG(ret, "CBRenderDD::ReleaseAll failed");
		return ret;
	}

	CBRenderer::SwitchFullscreen();

    // Re-create the surfaces
    ret = InitSurfaces();

    return ret;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::Flip()
{
	HRESULT ret=S_OK;

	while (TRUE){
		// If we are in windowed mode, perform a blt.
		if (Game->m_Renderer->m_Windowed){			
			ret = m_FrontBuffer->Blt(&m_ScreenRect,
									m_BackBuffer,
									&m_ViewportRect,
									DDBLT_WAIT , NULL);

			// hack: limit framerate
			#define MAX_FRAMERATE 100
			DWORD i = timeGetTime()-Game->m_CurrentTime;
			if(i<1000/MAX_FRAMERATE) Sleep(1000/MAX_FRAMERATE - i);

		}
		else {
			// Else we are in fullscreen mode, so perform a flip.
			ret = m_FrontBuffer->Flip(NULL, 0L);
		}

		if (ret == DD_OK) break;

		if (ret == DDERR_SURFACELOST){
			Game->LOG(0, "Restoring surfaces");
			ret = m_FrontBuffer->Restore();
			Game->m_SurfaceStorage->RestoreAll();
			ClipCursor();
			if (ret != DD_OK) break;
		}
		if (ret != DDERR_WASSTILLDRAWING) break;
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::InitRenderer(CHWManager* hwManager)
{
	if(!hwManager->m_SelectedVideoDevice) return E_FAIL;

	m_Windowed = hwManager->m_SelectedWindowed;
	m_Width = hwManager->m_ReqWidth;
	m_Height = hwManager->m_ReqHeight;
	m_BPP = hwManager->m_Selected16bitColors?16:32;


	HRESULT ret;
	if(FAILED(ret=Game->m_DirectX.DirectDrawCreateEx(&hwManager->m_SelectedVideoDevice->m_Guid, (VOID**)&m_DirectDraw, IID_IDirectDraw7, NULL))){
		Game->LOG(ret, "Error creating DirectDraw object.");
		return ret;
	}
	else{
		// get capabilities
		DDCAPS Caps;
		memset(&Caps, 0, sizeof(DDCAPS));
		Caps.dwSize = sizeof(DDCAPS);
		m_DirectDraw->GetCaps(&Caps, NULL);
		m_SupportsWideSurfaces = (Caps.dwCaps2 & DDCAPS2_WIDESURFACES) != 0;

		if(Game->m_DEBUG_DebugMode && Game->m_Registry->ReadBool("Debug", "DumbVideoCard"))
			m_SupportsWideSurfaces = false;
		
		Game->LOG(0, "Supports wide surfaces: %s", m_SupportsWideSurfaces?"yes":"no");

		HRESULT ret = InitSurfaces();
		ClipCursor();

		return ret;
	}
}

//////////////////////////////////////////////////////////////////////
void CBRenderDD::OnWindowChange()
{
	if(m_ClipperWindow != NULL){
		GetClientRect(m_ClipperWindow, &m_ViewportRect);
		GetClientRect(m_ClipperWindow, &m_ScreenRect);
		ClientToScreen(m_ClipperWindow, (POINT*)&m_ScreenRect.left);
		ClientToScreen(m_ClipperWindow, (POINT*)&m_ScreenRect.right);

		m_Width = m_ViewportRect.right - m_ViewportRect.left;
		m_Height = m_ViewportRect.bottom - m_ViewportRect.top;
	}
}


//////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::Fill(BYTE r, BYTE g, BYTE b, RECT* rect)
{
	if(m_LastFillR!=r || m_LastFillG!=g || m_LastFillB!=b){
		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);

		m_FrontBuffer->GetSurfaceDesc(&ddsd);
		m_LastFillNative = CBUtils::RGBtoNative(ddsd.ddpfPixelFormat, r, g, b);

		m_LastFillR = r;
		m_LastFillG = g;
		m_LastFillB = b;
	}

	DDBLTFX     ddbltfx;
	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = m_LastFillNative;
	return m_BackBuffer->Blt(rect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::WindowedBlt()
{
	HRESULT ret;
	
	while (TRUE){
		ret = m_FrontBuffer->Blt(&m_ScreenRect, m_BackBuffer, &m_ViewportRect, DDBLT_WAIT, NULL);
		
		if (ret == DD_OK) break;
		
		if (ret == DDERR_SURFACELOST){
			ret = m_FrontBuffer->Restore();
			if (ret != DD_OK ) break;
		}
		
		if (ret != DDERR_WASSTILLDRAWING) break;
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::Setup2D(bool Force)
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::Setup3D(C3DCamera* Camera, bool Force)
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::SetupLines()
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::DrawLine(int X1, int Y1, int X2, int Y2, DWORD Color){
	HDC hDC;
	m_BackBuffer->GetDC(&hDC);

	BYTE b = GetRValue(Color);
	BYTE g = GetGValue(Color);
	BYTE r = GetBValue(Color);

	HPEN pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	HGDIOBJ oldPen = SelectObject(hDC, pen);

	MoveToEx(hDC, X1, Y1, NULL);
	LineTo(hDC, X2, Y2);

	SelectObject(hDC, oldPen);
	DeleteObject(pen);

	m_BackBuffer->ReleaseDC(hDC);	
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::Fade(WORD Alpha)
{
	// sloooow
	return S_OK;


	int register i,j;
	unsigned long Pitch;
	HRESULT res;
	BYTE* Data;
	DWORD Temp;
	DWORD dr,dg,db;
	WORD  buf;
	bool  oddWidth = false;
	DDSURFACEDESC2 SurfaceDesc;
	int width, height;

	width = m_Width;
	height = m_Height;

	ZeroMemory(&SurfaceDesc, sizeof(DDSURFACEDESC2));
	SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);
	res = m_BackBuffer->Lock(NULL, &SurfaceDesc, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL);
	if(FAILED(res)) return res;
	

	// Set the pitch for both surfaces
    Pitch = SurfaceDesc.lPitch;

    // Initialize the pointers to the upper left hand corner of surface
    Data  = (BYTE*)SurfaceDesc.lpSurface;

	switch(SurfaceDesc.ddpfPixelFormat.dwRGBBitCount){
	case 15:

		// Set the horizontal padding
		buf = (WORD)(Pitch - (2 * width));

		// Is the width odd or even?
		if (width % 2 == 1){
			oddWidth = true;
			width = (width - 1) / 2; //div by 2, processing 2 pixels at a time.
		}
		else{
			width = width / 2;  //div by 2, processing 2 pixels at a time.
		}

		i = height;
		do{
			if (oddWidth){
				Temp = *((WORD*)Data);

				db = Temp & 0x1f;
				dg = (Temp >> 5)  & 0x1f;
				dr = (Temp >> 10) & 0x1f;

				*((WORD*)Data) = (WORD)((Alpha * db) >> 8 |
					                   ((Alpha * dg) >> 8) << 5 |
					                   ((Alpha * dr) >> 8) << 10);

				Data += 2;
			}

			//
			// 5-5-5 Fading: Read from src and write to dest
			//               Uses optimised MMX style SIMD instructions.
			//
			j = width;
			do{
				Temp = *((DWORD*)Data);

				db =  ((Alpha * (Temp & 0x001F001F)) >> 8) & 0x001F001F;
				dg = (((Alpha * ((Temp >> 5)  & 0x001F001F)) >> 8) & 0x001F001F) << 5;
				dr = (((Alpha * ((Temp >> 10) & 0x001F001F)) >> 8) & 0x001F001F) << 10;

				*((DWORD*)Data) = db | dg | dr;

				Data += 4;
			} while (--j > 0);

			Data += buf;
		} while (--i > 0);		
	break;

	case 16:
		// Set the horizontal padding
		buf = (WORD)(Pitch - (2 * width));

		// Is the width odd or even?
		if (width % 2 == 1){
			oddWidth = true;
			width = (width - 1) / 2; //div by 2, processing 2 pixels at a time.
		}
		else{
			width = width / 2;  //div by 2, processing 2 pixels at a time.
		}


		i = height;
		do{
			if(oddWidth){
				Temp = *((WORD*)Data);

				db = Temp & 0x1f;
				dg = (Temp >> 5)  & 0x3f;
				dr = (Temp >> 11) & 0x1f;

				*((WORD*)Data) = (WORD)((Alpha * db) >> 8 |
					                   ((Alpha * dg) >> 8) << 5 |
					                   ((Alpha * dr) >> 8) << 11);

				Data += 2;
			}

			//
			// 5-6-5 Fading: Read from src and write to dest
			//               Uses optimised MMX style SIMD instructions.
			//
			j = width;
			do{
				Temp = *((DWORD*)Data);

				db =  ((Alpha * (Temp & 0x001F001F)) >> 8) & 0x001F001F;
				dg = (((Alpha * ((Temp >> 5)  & 0x003F003F)) >> 8) & 0x003F003F) << 5;
				dr = (((Alpha * ((Temp >> 11) & 0x001F001F)) >> 8) & 0x001F001F) << 11;

				*((DWORD*)Data) = db | dg | dr;

				Data += 4;
			} while (--j > 0);

			Data += buf;
		} while (--i > 0);
	break;

	case 24:
		// Set the horizontal padding
		buf = (WORD)(Pitch - (3 * width));

		i = height;
		do{
			j = width;
			do{
				Temp = *((DWORD*)Data);

				db =  (Alpha * (Temp & 0xFF)) >> 8;
				dg = ((Alpha * ((Temp >> 8)  & 0xFF)) >> 8) << 8;
				dr =  (Alpha * ((Temp >> 16) & 0xFF)) >> 8;

				*((WORD*)Data) = (WORD)(db | dg);
				Data += 2;
				*Data = (BYTE)dr;

				Data++;
			} while (--j > 0);

			Data += buf;
		} while (--i > 0);
	break;

	case 32:
		// Set the horizontal padding
		buf = (WORD)(Pitch - (4 * width));

		i = height;
		do{
			j = width;
			do{
				Temp = *((DWORD*)Data);

				db =  (Alpha * (Temp & 0xFF)) >> 8;
				dg = ((Alpha * ((Temp >> 8)  & 0xFF)) >> 8) << 8;
				dr =  (Alpha * ((Temp >> 16) & 0xFF)) >> 8;

				*((WORD*)Data) = (WORD)(db | dg);
				Data += 2;
				*Data = (BYTE)dr;

				Data += 2;

			} while (--j > 0);

			Data += buf;
		} while (--i > 0);
	break;
	} // End RGB Format switch statement


	return m_BackBuffer->Unlock(NULL);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::FadeToColor(DWORD Color, RECT* rect)
{
	BYTE r, g, b, a;
	a = D3DCOLGetA(Color);
	if(a==0xFF){
		r = D3DCOLGetR(Color);
		g = D3DCOLGetG(Color);
		b = D3DCOLGetB(Color);
		if(!rect){
			RECT rc;
			Game->GetCurrentViewportRect(&rc);
			return Fill(r, g, b, &rc);
		}
		else return Fill(r, g, b, rect);
	}
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::SetViewport(int left, int top, int right, int bottom)
{
	SetRect(&m_ViewportRect, left, top, right, bottom);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CBImage* CBRenderDD::TakeScreenshot()
{
	CBImage* Image = new CBImage(Game);
	if(!Image || FAILED(Image->LoadFromDDSurface(m_BackBuffer))){
		delete Image;
		return NULL;
	}
	else return Image;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderDD::DisplayDebugInfo()
{
	char str[100];
	sprintf(str, "FreeMem: %d KB", GetFreeVideoMem() / 1024);
	Game->m_SystemFont->DrawText((BYTE*)str, 0, 20, m_Width, TAL_RIGHT);
	sprintf(str, "UsedMem: %d KB", GetUsedVideoMem() / 1024);
	Game->m_SystemFont->DrawText((BYTE*)str, 0, 40, m_Width, TAL_RIGHT);

	return S_OK;
}