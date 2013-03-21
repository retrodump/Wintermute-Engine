// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSurfaceDD.h"


//////////////////////////////////////////////////////////////////////////
CBSurfaceDD::CBSurfaceDD(CBGame* inGame):CBSurface(inGame)
{
	m_Locked = false;
	m_Surface = NULL;

	ZeroMemory(&m_SurfaceDesc, sizeof(DDSURFACEDESC2));
	m_SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

	m_TransIndex = 0;
	m_TransNative = 0;
	m_TransDefault = false;

	m_RedMask  = m_GreenMask  = m_BlueMask  = 0;
	m_RedBits  = m_GreenBits  = m_BlueBits  = 0;
	m_RedShift = m_GreenShift = m_BlueShift = 0;

	m_BytesPerPixel = 0;


	m_Tiles = NULL;
	m_NumTiles = 0;
	m_NumRows = m_NumCols = 0;
	m_TileWidth = m_TileHeight = 0;
	SetRectEmpty(&m_Rect);
	m_Child = false;

	m_RestoreNeeded = false;
}


//////////////////////////////////////////////////////////////////////////
CBSurfaceDD::~CBSurfaceDD()
{
	RELEASE(m_Surface);
	ReleaseTiles();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::Create(char * Filename, bool default_ck, BYTE ck_red, BYTE ck_green, BYTE ck_blue, int LifeTime, bool KeepLoaded)
{
	HRESULT ret;


	CBImage* Image = new CBImage(Game);
	ret = Image->GetImageInfo(Filename, &m_Width, &m_Height);


	if(LifeTime!=-1 && m_LifeTime==0){
		m_Valid = false;
		delete Image;
	}
	else{
		CBRenderDD* rend = (CBRenderDD*)Game->m_Renderer;

		if(!rend->m_SupportsWideSurfaces && m_Width > rend->m_Width){			
			
			// use CBImage to load tiled image
			RELEASE(m_Surface);
			ReleaseTiles();

			/*
			if(m_RestoreNeeded){
				if(m_NumTiles>0){
					for(int i=0; i<m_NumTiles; i++){
						if(m_Tiles[i]->m_Surface) m_Tiles[i]->m_Surface->Restore();
					}
				}
				m_RestoreNeeded = false;
			}
			*/
			m_RestoreNeeded = false;
			
			if(FAILED(Image->LoadFile(Filename))){
				delete Image;
				Game->LOG(0, "CBSurfaceDD::Create failed for file '%s' - error loading file", Filename);
				return E_FAIL;
			}

			BYTE r, g, b;
			r = 255;
			g = 0;
			b = 255;
			
			if(FAILED(ret = Image->CreateDDSurface(this, r, g, b))){
				Game->LOG(ret, "CBSurfaceDD::Create failed for file '%s' - error creating tiled surface", Filename);
			}
			delete Image;			
		}
		else{
			// normal load
			delete Image;			
			ret = LoadFile(Filename);
		}

		// OK so far, get texture format
		if(SUCCEEDED(ret)){
			GetSurfaceDesc();

			if(default_ck) SetTransDefault();
			else SetTrans(ck_red, ck_green, ck_blue);

			m_Valid = true;
		}
	}

	m_CKDefault = default_ck;
	m_CKRed = ck_red;
	m_CKGreen = ck_green;
	m_CKBlue = ck_blue;


	if(!m_Filename || stricmp(m_Filename, Filename)!=0){
		SetFilename(Filename);
	}

	
	if(m_LifeTime==0 || LifeTime==-1 || LifeTime>m_LifeTime)
		m_LifeTime = LifeTime;

	m_KeepLoaded = KeepLoaded;
	if(m_KeepLoaded) m_LifeTime = -1;


	return ret;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::LoadFile(char * Filename)
{
	ReleaseTiles(); // we don't need them anymore

	CBImage* image = new CBImage(Game);
	HRESULT ret = image->LoadFile(Filename);
	if(ret != S_OK){
		Game->LOG(ret, "CBSurfaceDD::LoadFile failed ('%s')", Filename);
		delete image;
		return ret;
	}

	RELEASE(m_Surface);
	m_RestoreNeeded = false;

	BYTE r, g, b;
	r = 255;
	g = 0;
	b = 255;

	m_Surface = image->CreateSurface(&m_Width, &m_Height, false, r, g, b);
	if(m_Surface==NULL){
		Game->LOG(0, "CBSurfaceDD::LoadFile failed");
		return E_FAIL;
	}
	
	GetSurfaceDesc();

	if(m_TransDefault) SetTransDefault();
	else SetTrans(m_TransRGB.red, m_TransRGB.green, m_TransRGB.blue);

	delete image;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::Lock()
{
	if(m_Locked) return S_OK;

	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			HRESULT hr = m_Tiles[i]->Lock();
			if(FAILED(hr)) return hr;
		}
	}

	HRESULT ret;
	ZeroMemory(&m_SurfaceDesc, sizeof(DDSURFACEDESC2));
	m_SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);
	ret = m_Surface->Lock(NULL, &m_SurfaceDesc, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL);
	if(ret != DD_OK){
		Game->LOG(ret, "CBSurfaceDD::Lock failed");
		return ret;
	}	
	m_Locked = true;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::Unlock()
{
	if(!m_Locked) return S_OK;

	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			HRESULT hr = m_Tiles[i]->Unlock();
			if(FAILED(hr)) return hr;
		}
	}


	HRESULT ret;
	ret = m_Surface->Unlock(NULL);
	if(ret != DD_OK){
		Game->LOG(ret, "CBSurfaceDD::Unlock failed");
		return ret;
	}		
	m_Locked = false;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::GetSurfaceDesc()
{
	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			HRESULT hr = m_Tiles[i]->GetSurfaceDesc();
			if(FAILED(hr)) return hr;
		}
	}
	
	if(!m_Surface) return E_FAIL;
	ZeroMemory(&m_SurfaceDesc, sizeof(DDSURFACEDESC2));
	m_SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);
	return m_Surface->GetSurfaceDesc(&m_SurfaceDesc);
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::SetTrans(BYTE r, BYTE g, BYTE b)
{
	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			HRESULT hr = m_Tiles[i]->SetTrans(r, g, b);
			if(FAILED(hr)) return hr;
		}
	}
	
	m_TransRGB.red   = r;
	m_TransRGB.green = g;
	m_TransRGB.blue  = b;

	m_TransDefault = false;
	if(!m_Surface) return E_FAIL;

	GetSurfaceDesc();

	if(m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount > 8){
		m_TransNative = CBUtils::RGBtoNative(m_SurfaceDesc.ddpfPixelFormat, r, g, b);
	}


	// RGB surface
	DDCOLORKEY ddck;
	HRESULT res;
	if(m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount > 8){
		ddck.dwColorSpaceLowValue = m_TransNative;
		ddck.dwColorSpaceHighValue = m_TransNative;
		res = m_Surface->SetColorKey(DDCKEY_SRCBLT, &ddck);
		if(res!=DD_OK){
			Game->LOG(res, "Error setting color key for '%s' (%d %d %d)", m_Filename, r, g, b);
			return res;
		}
	}
	// paletted surface, get nearest index in palette
	else{
		LPDIRECTDRAWPALETTE pal;
		res = m_Surface->GetPalette(&pal);
		if(res != DD_OK){
			Game->LOG(res, "GetPalette failed for surface '%s'", m_Filename);
			return res;
		}
		PALETTEENTRY entries[256];
		pal->GetEntries(0, 0, 256, entries);
		m_TransIndex = CBUtils::ClosestColor(r, g, b, entries, 256);
		if(m_TransIndex==-1) m_TransIndex = 255;
		pal->Release();

		ddck.dwColorSpaceLowValue = m_TransIndex;
		ddck.dwColorSpaceHighValue = m_TransIndex;
		res = m_Surface->SetColorKey(DDCKEY_SRCBLT, &ddck);
		if(res!=DD_OK){
			Game->LOG(res, "Error setting color key for '%s' (index %d)", m_Filename, m_TransIndex);
			return res;
		}
	}
	return S_OK;
}


// sets transparent color same as lower right pixel
//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::SetTransDefault()
{
	return SetTrans(255, 0, 255);
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::Restore()
{
	//m_Surface->Restore();
	//return LoadFile(m_Filename);
	m_RestoreNeeded = true;
	return Create(m_Filename, m_CKDefault, m_CKRed, m_CKGreen, m_CKBlue, m_LifeTime);
}




//////////////////////////////////////////////////////////////////////
bool CBSurfaceDD::IsTransparentAt(int X, int Y)
{
	if(X < 0 || Y < 0 || X >= m_Width || Y >= m_Height) return false;

	PrepareToDraw();

	if(m_NumTiles>0){
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->IsTransparentAt(X, Y);
	}	


	if(FAILED(Lock())) return false;

	bool ret = false;
	BYTE *Bitmap = (BYTE*)m_SurfaceDesc.lpSurface;
	Bitmap += Y * m_SurfaceDesc.lPitch;

	// RGB
	if(m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount > 8){
		int num_bytes = m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount / 8;
		ret = (memcmp(Bitmap + X * num_bytes, &m_TransNative, num_bytes) == 0);
	}
	// palettized
	else ret = (Bitmap[X] == m_TransIndex);

	Unlock();
	return ret;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::Display(int X, int Y, RECT rect, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	HRESULT ret = S_OK;
	
	PrepareToDraw();

	if(m_NumTiles>0){
		HRESULT ret = S_OK;
		for(int Row=0; Row<m_NumRows; Row++){
			for(int Col=0; Col<m_NumCols; Col++){
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect)){
					ret = m_Tiles[Row*m_NumCols+Col]->Display(X+Col*m_TileWidth, Y+Row*m_TileHeight, SubRect, BlendMode, MirrorX, MirrorY);
					if(FAILED(ret)) return ret;
				}
			}
		}
		return ret;
	}
	else{
		RECT ModRect = rect;

		CBUtils::Clip(&X, &Y, &ModRect, &Game->m_Renderer->m_ViewportRect);

		if(!IsRectEmpty(&ModRect)){
			ret = ((CBRenderDD*)Game->m_Renderer)->m_BackBuffer->BltFast(X, Y, m_Surface, &ModRect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
			if(ret == DDERR_SURFACELOST) Restore();
			else if(ret != DD_OK) Game->LOG(ret, "CBSurfaceDD::Display failed for surface '%s'", m_Filename);
		}
		return ret;
	}
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::DisplayTrans(int X, int Y, RECT rect, DWORD Alpha, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	HRESULT ret = S_OK;
	
	PrepareToDraw();
	
	if(m_NumTiles>0){
		HRESULT ret = S_OK;

		int NewY = Y;
		for(int Row=0; Row<m_NumRows; Row++){
			int NewX = X;
			int YtoSet = 0;
			for(int Col=0; Col<m_NumCols; Col++){
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect)){
					OffsetRect(&SubRect, -(Col*m_TileWidth), -(Row*m_TileHeight));
					ret = m_Tiles[Row*m_NumCols+Col]->DisplayTrans(NewX, NewY, SubRect, Alpha, BlendMode, MirrorX, MirrorY);
					if(FAILED(ret)) return ret;
					NewX += (SubRect.right - SubRect.left);
					YtoSet = SubRect.bottom - SubRect.top;
				}
			}
			NewY += YtoSet;
		}
		return ret;
	}
	else{
		RECT ModRect = rect;
		CBUtils::Clip(&X, &Y, &ModRect, &Game->m_Renderer->m_ViewportRect);

		if(!IsRectEmpty(&ModRect)){
			((CBRenderDD*)Game->m_Renderer)->m_BackBuffer->BltFast(X, Y, m_Surface, &ModRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			if(ret == DDERR_SURFACELOST) Restore();
			else if(ret != DD_OK) Game->LOG(ret, "CBSurfaceDD::DisplayTrans failed for surface '%s'", m_Filename);
		}
		return ret;
	}
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::DisplayTransZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	return DisplayZoom(X, Y, rect, ZoomX, ZoomY, Alpha, true, BlendMode, MirrorX, MirrorY);
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::DisplayZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha, bool Transparent, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	if(ZoomX <= 0 || ZoomY <= 0) return S_OK;
	if(ZoomX == 100 && ZoomY == 100) return DisplayTrans(X, Y, rect, Alpha, BlendMode, MirrorX, MirrorY);

	PrepareToDraw();


	
	if(m_NumTiles>0){
		HRESULT ret = S_OK;
		float NewY = (float)Y;
		for(int Row=0; Row<m_NumRows; Row++){
			float NewX = (float)X;
			for(int Col=0; Col<m_NumCols; Col++){
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect)){
					ret = m_Tiles[Row*m_NumCols+Col]->DisplayZoom(NewX, NewY, SubRect, ZoomX, ZoomY, Alpha, Transparent, BlendMode, MirrorX, MirrorY);
					if(FAILED(ret)) return ret;
				}
				NewX += (float)m_TileWidth * ZoomX / 100.0f;
			}
			NewY += (float)m_TileHeight * ZoomY / 100.0f;
		}
		return ret;
	}
	else{
		HRESULT ret;
		RECT ModRect = rect;

		ModRect.left   = int(ModRect.left   * (ZoomX/100));
		ModRect.top    = int(ModRect.top    * (ZoomY/100));
		ModRect.right  = int(ModRect.right  * (ZoomX/100));
		ModRect.bottom = int(ModRect.bottom * (ZoomY/100));

		CBUtils::Clip(&X, &Y, &ModRect, &Game->m_Renderer->m_ViewportRect);
		if(IsRectEmpty(&ModRect) || ModRect.left==ModRect.right || ModRect.bottom==ModRect.top) return S_OK;

		RECT DestRect;
		DestRect.left = X;
		DestRect.top = Y;
		DestRect.right = X + (ModRect.right - ModRect.left);
		DestRect.bottom = Y + (ModRect.bottom - ModRect.top);

		ModRect.left   = max(rect.left,   ModRect.left   * (100/ZoomX));
		ModRect.right  = min(rect.right,  ModRect.right  * (100/ZoomX));
		ModRect.bottom = min(rect.bottom, ModRect.bottom * (100/ZoomY));
		ModRect.top    = max(rect.top,    ModRect.top    * (100/ZoomY));
	
		InflateRect(&ModRect, -1, -1);

		DWORD flags = DDBLT_WAIT;
		if(Transparent) flags |= DDBLT_KEYSRC;

		ret = ((CBRenderDD*)Game->m_Renderer)->m_BackBuffer->Blt(&DestRect, m_Surface, &ModRect, flags, NULL);
		if(ret == DDERR_SURFACELOST) Restore();
		else if(ret != DD_OK){
			Game->LOG(ret, "CBSurfaceDD::DisplayZoom failed for surface '%s'", m_Filename);
		}

		return ret;
	}
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::DisplayHalfTrans(int X, int Y, RECT rect)
{
	//TODO: optimize somehow (cache..?)
	PrepareToDraw();

	HRESULT ret = S_OK;


	if(m_NumTiles>0){
		HRESULT ret = S_OK;

		int NewY = Y;
		for(int Row=0; Row<m_NumRows; Row++){
			int NewX = X;
			int YtoSet = 0;
			for(int Col=0; Col<m_NumCols; Col++){
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect)){
					OffsetRect(&SubRect, -(Col*m_TileWidth), -(Row*m_TileHeight));
					ret = m_Tiles[Row*m_NumCols+Col]->DisplayTrans(NewX, NewY, SubRect, 0xFF);
					if(FAILED(ret)) return ret;
					NewX += (SubRect.right - SubRect.left);
					YtoSet = SubRect.bottom - SubRect.top;
				}
			}
			NewY += YtoSet;
		}
		return ret;
	}
	else{
		DDSURFACEDESC2 ddsd;
	
		RECT ModRect = rect;
	

		// clip rectangle
		CBUtils::Clip(&X, &Y, &ModRect, &Game->m_Renderer->m_ViewportRect);
		if(IsRectEmpty(&ModRect)) return S_OK;

		// lock both surfaces
		if(FAILED(ret = Lock())) return ret;

		ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		ret = ((CBRenderDD*)Game->m_Renderer)->m_BackBuffer->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL);
		if(FAILED(ret)){
			Unlock();
			return ret;
		}

		BYTE* src;
		BYTE* dest;

		int i,j,x,y;

		// draw

		// RGB
		if(m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount > 8){
			int num_bytes = m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount / 8;

			for(j=ModRect.top, y=Y; j<ModRect.bottom; j++, y++){
				src = (BYTE*)m_SurfaceDesc.lpSurface + j * m_SurfaceDesc.lPitch;
				dest = (BYTE*)ddsd.lpSurface + y * ddsd.lPitch;
				for(i=ModRect.left+j%2,x=X+j%2; i<ModRect.right; i+=2,x+=2){
					if(memcmp(src + i * num_bytes, &m_TransNative, num_bytes)==0) continue;
					memcpy(dest + x * num_bytes, src + i * num_bytes, num_bytes);
				}
			}
		}
		// palettized
		else{
			for(j=ModRect.top, y=Y; j<ModRect.bottom; j++, y++){
				src = (BYTE*)m_SurfaceDesc.lpSurface + j * m_SurfaceDesc.lPitch;
				dest = (BYTE*)ddsd.lpSurface + y * ddsd.lPitch;
				for(i=ModRect.left+j%2,x=X+j%2; i<ModRect.right; i+=2,x+=2){
					if(*(src + i) == m_TransIndex) continue;
					*(dest + x) = *(src + i);
				}
			}
		}

		// unlock surfaces and return
		((CBRenderDD*)Game->m_Renderer)->m_BackBuffer->Unlock(NULL);
		Unlock();

		return S_OK;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::Create(int Width, int Height){

	LPDIRECTDRAWSURFACE7 surface = NULL;
	DDSURFACEDESC2 ddsd;

	CBRenderDD* renderer = (CBRenderDD*)Game->m_Renderer;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = Width;
	ddsd.dwHeight = Height;
	
	HRESULT res = renderer->m_DirectDraw->CreateSurface(&ddsd, &surface, NULL);
	if(res != DD_OK){
		Game->LOG(res, "CBSurfaceDD::Create failed");
		return E_FAIL;
	}
	m_Surface = surface;

	GetSurfaceDesc();
	SetTransDefault();

	m_Width = Width;
	m_Height = Height;
	SetRectEmpty(&m_Rect);

	m_Valid = true;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::CreateEmptySurface(int Width, int Height)
{
	LPDIRECTDRAWSURFACE7 surface = NULL;
	DDSURFACEDESC2 ddsd;

	CBRenderDD* renderer = (CBRenderDD*)Game->m_Renderer;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = Width;
	ddsd.dwHeight = Height;
	
	HRESULT res = renderer->m_DirectDraw->CreateSurface(&ddsd, &surface, NULL);
	if(res != DD_OK){
		Game->LOG(res, "CBSurfaceDD::CreateEmptySurface failed");
		return E_FAIL;
	}
	m_Surface = surface;

	GetSurfaceDesc();

	m_Width = Width;
	m_Height = Height;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::StartPixelOp()
{
	if(m_PixelOpReady) return S_OK;

	PrepareToDraw();

	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			HRESULT hr = m_Tiles[i]->StartPixelOp();
			if(FAILED(hr)) return hr;
		}
	}
	else{
		HRESULT res = Lock();
		if(FAILED(res)) return res;

		m_RedMask = m_SurfaceDesc.ddpfPixelFormat.dwRBitMask;
		GetBitMaskInfo(m_RedMask, &m_RedShift, &m_RedBits);

		m_GreenMask = m_SurfaceDesc.ddpfPixelFormat.dwGBitMask;
		GetBitMaskInfo(m_GreenMask, &m_GreenShift, &m_GreenBits);

		m_BlueMask = m_SurfaceDesc.ddpfPixelFormat.dwBBitMask;
		GetBitMaskInfo(m_BlueMask, &m_BlueShift, &m_BlueBits);

		m_BytesPerPixel = m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount / 8;
	}

	m_PixelOpReady = true;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::EndPixelOp()
{
	if(!m_PixelOpReady) return S_OK;

	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			HRESULT hr = m_Tiles[i]->EndPixelOp();
			if(FAILED(hr)) return hr;
		}
	}
	else{
		Unlock();
	}

	m_PixelOpReady = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::GetPixel(int X, int Y, BYTE *R, BYTE *G, BYTE *B, BYTE *A)
{
	if(!m_PixelOpReady) return E_FAIL;

	if(m_NumTiles>0){
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->GetPixel(X, Y, R, G, B, A);
	}

	DWORD NativePixel = 0;
	memcpy(&NativePixel, (BYTE*)m_SurfaceDesc.lpSurface + Y*m_SurfaceDesc.lPitch + X*m_BytesPerPixel, m_BytesPerPixel);

	if(R) *R = (BYTE)((NativePixel&m_RedMask)   >> m_RedShift);
	if(G) *G = (BYTE)((NativePixel&m_GreenMask) >> m_GreenShift);
	if(B) *B = (BYTE)((NativePixel&m_BlueMask)  >> m_BlueShift);
	if(A) *A = 0xFF;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::PutPixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A)
{
	if(!m_PixelOpReady) return E_FAIL;

	if(m_NumTiles>0){
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->PutPixel(X, Y, R, G, B, A);
	}

	if(A < 128)
	{
		R = 255;
		G = 0;
		B = 255;
	}

	DWORD NativePixel;

	NativePixel = ((R>>(8-m_RedBits))   << m_RedShift)   +
		          ((G>>(8-m_GreenBits)) << m_GreenShift) +
				  ((B>>(8-m_BlueBits))  << m_BlueShift);
	
	memcpy((BYTE*)m_SurfaceDesc.lpSurface + Y*m_SurfaceDesc.lPitch + X*m_BytesPerPixel, &NativePixel, m_BytesPerPixel);

	return S_OK;;
}


//////////////////////////////////////////////////////////////////////////
bool CBSurfaceDD::ComparePixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A)
{
	if(!m_PixelOpReady) return false;

	if(m_NumTiles>0){
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->ComparePixel(X, Y, R, G, B, A);
	}


	DWORD NativePixel;

	NativePixel = ((R>>(8-m_RedBits))   << m_RedShift)   +
		          ((G>>(8-m_GreenBits)) << m_GreenShift) +
				  ((B>>(8-m_BlueBits))  << m_BlueShift);

	return (memcmp((BYTE*)m_SurfaceDesc.lpSurface + Y*m_SurfaceDesc.lPitch + X*m_BytesPerPixel, &NativePixel, m_BytesPerPixel)==0);
}


//////////////////////////////////////////////////////////////////////////
bool CBSurfaceDD::IsTransparentAtLite(int X, int Y)
{
	if(!m_PixelOpReady) return false;
	if(X < 0 || Y < 0 || X >= m_Width || Y >= m_Height) return false;
	
	PrepareToDraw();

	if(m_NumTiles>0){
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->IsTransparentAtLite(X, Y);
	}	

	BYTE *Bitmap = (BYTE*)m_SurfaceDesc.lpSurface;
	Bitmap += Y * m_SurfaceDesc.lPitch;

	// RGB
	if(m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount > 8){
		int num_bytes = m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount / 8;
		return (memcmp(Bitmap + X * num_bytes, &m_TransNative, num_bytes) == 0);
	}
	// palettized
	else return (Bitmap[X] == m_TransIndex);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::GetBitMaskInfo(DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits)
{
    DWORD dwShift = 0;
    DWORD dwBits  = 0; 

    if(pdwShift == NULL || pdwBits == NULL) return E_INVALIDARG;

    if(dwBitMask){
        while((dwBitMask & 1) == 0){
            dwShift++;
            dwBitMask >>= 1;
        }
    }

    while((dwBitMask & 1) != 0){
        dwBits++;
        dwBitMask >>= 1;
    }

    *pdwShift = dwShift;
    *pdwBits  = dwBits;

    return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::Invalidate()
{
	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			HRESULT hr = m_Tiles[i]->Invalidate();
			if(FAILED(hr)) return hr;
		}
	}
	else{
		RELEASE(m_Surface);
	}

	m_Valid = false;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::ReleaseTiles()
{
	if(m_NumTiles>0){
		for(int i=0; i<m_NumTiles; i++){
			if(m_Tiles[i]) delete m_Tiles[i];
		}
		delete [] m_Tiles;
	}
	m_Tiles = NULL;
	m_NumTiles = 0;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceDD::PrepareToDraw()
{
	if(!m_Child) return CBSurface::PrepareToDraw();
	else return S_OK;
}
