// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSurfaceD3D.h"


//////////////////////////////////////////////////////////////////////////
CBSurfaceD3D::CBSurfaceD3D(CBGame* inGame):CBSurface(inGame)
{
	m_Texture = NULL;
	m_TextureFormat = D3DFMT_UNKNOWN;

	m_Tiles = NULL;
	m_NumTiles = 0;
	m_NumRows = m_NumCols = 0;
	m_TileWidth = m_TileHeight = 0;
	SetRectEmpty(&m_Rect);
	m_Child = false;
}


//////////////////////////////////////////////////////////////////////////
CBSurfaceD3D::~CBSurfaceD3D()
{
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	if(Rend) Rend->InvalidateTexture(m_Texture);

	RELEASE(m_Texture);
	ReleaseTiles();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::Create(char *Filename, bool default_ck, BYTE ck_red, BYTE ck_green, BYTE ck_blue, int LifeTime, bool KeepLoaded)
{
	HRESULT ret;

	if(default_ck)
	{
		ck_red   = 255;
		ck_green = 0;
		ck_blue  = 255;
	}


	CBImage* Image = new CBImage(Game);
	ret = Image->GetImageInfo(Filename, &m_Width, &m_Height);


	if(LifeTime!=-1 && m_LifeTime==0)
	{
		m_Valid = false;
		delete Image;
	}
	else
	{
		CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;

		bool isSaveGameGrayscale = strnicmp(Filename, "savegame:", 9)==0 && (Filename[strlen(Filename)-1]=='g' || Filename[strlen(Filename)-1]=='G');

		if(m_Width > rend->m_MaxTexWidth || m_Height > rend->m_MaxTexHeight || isSaveGameGrayscale)
		{
			// use CBImage to load the texture
			if(FAILED(Image->LoadFile(Filename)))
			{
				delete Image;
				Game->LOG(0, "CBSurfaceD3D::Create failed for file '%s' - error loading file", Filename);
				return E_FAIL;
			}
			
			if(FAILED(ret = Image->CreateTexture(this, DRGBA(ck_red,ck_green,ck_blue,255))))
			{
				Game->LOG(0, "CBSurfaceD3D::Create failed for file '%s' - error creating tiled texture", Filename);
			}

			delete Image;
		}
		else
		{
			// use D3DX to load the texture
			delete Image;

			// load file
			DWORD Size=0;
			BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename, &Size);
			if(Buffer==NULL)
			{
				Game->LOG(0, "CBSurfaceD3D::Create failed for file '%s' - error loading file", Filename);				
				return E_FAIL;
			}

			D3DXIMAGE_INFO SrcInfo;
			ret = D3DXCreateTextureFromFileInMemoryEx(
				rend->m_Device,
				Buffer, Size,
				D3DX_DEFAULT, D3DX_DEFAULT,
				1,
				0,
				//(((CBRenderD3D*)Game->m_Renderer)->m_BPP==16)?D3DFMT_A1R5G5B5:D3DFMT_A8R8G8B8,		
				rend->m_Force16bitTextures?D3DFMT_A1R5G5B5:D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				DRGBA(ck_red,ck_green,ck_blue,255),
				&SrcInfo,
				NULL,
				&m_Texture);

			if(FAILED(ret)) Game->LOG(ret, "Error loading texture '%s'.", Filename);
			else
			{
				m_Width  = SrcInfo.Width;
				m_Height = SrcInfo.Height;
			}
			// close file
			delete [] Buffer;
		}

		// OK so far, get texture format
		if(SUCCEEDED(ret))
		{
			if(FAILED(ret = GetTextureFormat())) Game->LOG(ret, "Error getting texture format");
			else m_Valid = true;
		}
	}

	m_CKDefault = default_ck;
	m_CKRed = ck_red;
	m_CKGreen = ck_green;
	m_CKBlue = ck_blue;


	if(!m_Filename || stricmp(m_Filename, Filename)!=0)
	{
		SetFilename(Filename);
	}

	if(m_LifeTime==0 || LifeTime==-1 || LifeTime>m_LifeTime)
		m_LifeTime = LifeTime;

	m_KeepLoaded = KeepLoaded;
	if(m_KeepLoaded) m_LifeTime = -1;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::Display(int X, int Y, RECT rect, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	PrepareToDraw();

	if(m_NumTiles>0)
	{
		HRESULT ret = S_OK;
		for(int Row=0; Row<m_NumRows; Row++)
		{
			for(int Col=0; Col<m_NumCols; Col++)
			{
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect))
				{
					ret = m_Tiles[Row*m_NumCols+Col]->Display(X+Col*m_TileWidth, Y+Row*m_TileHeight, SubRect, BlendMode, MirrorX, MirrorY);
					if(FAILED(ret)) return ret;
				}
			}
		}
		return ret;
	}
	else
	{
		D3DXVECTOR2 pos(X, Y);
		return ((CBRenderD3D*)Game->m_Renderer)->DrawSprite(m_Texture, &rect, 100, 100, &pos, 0xFFFFFFFF, true, BlendMode, MirrorX, MirrorY);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::DisplayTrans(int X, int Y, RECT rect, DWORD Alpha, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	PrepareToDraw();

	if(m_NumTiles>0)
	{
		HRESULT ret = S_OK;

		int NewY = Y;
		for(int Row=0; Row<m_NumRows; Row++)
		{
			int NewX = X;
			int YtoSet = 0;
			for(int Col=0; Col<m_NumCols; Col++)
			{
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect))
				{
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
	else
	{
		D3DXVECTOR2 pos(X,Y);
		return ((CBRenderD3D*)Game->m_Renderer)->DrawSprite(m_Texture, &rect, 100, 100, &pos, Alpha, false, BlendMode, MirrorX, MirrorY);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::DisplayTransZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	PrepareToDraw();

	if(m_NumTiles>0)
	{
		HRESULT ret = S_OK;
		float NewY = (float)Y;
		for(int Row=0; Row<m_NumRows; Row++)
		{
			float NewX = (float)X;
			for(int Col=0; Col<m_NumCols; Col++)
			{
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect))
				{
					ret = m_Tiles[Row*m_NumCols+Col]->DisplayTransZoom(NewX, NewY, SubRect, ZoomX, ZoomY, Alpha, BlendMode, MirrorX, MirrorY);
					if(FAILED(ret)) return ret;
				}
				NewX += (float)m_TileWidth * ZoomX / 100.0f;
			}
			NewY += (float)m_TileHeight * ZoomY / 100.0f;
		}
		return ret;
	}
	else
	{
		D3DXVECTOR2 pos(X,Y);
		return ((CBRenderD3D*)Game->m_Renderer)->DrawSprite(m_Texture, &rect, ZoomX, ZoomY, &pos, Alpha, false, BlendMode, MirrorX, MirrorY);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::DisplayZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha, bool Transparent, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	PrepareToDraw();

	if(m_NumTiles>0)
	{
		HRESULT ret = S_OK;
		float NewY = (float)Y;
		for(int Row=0; Row<m_NumRows; Row++)
		{
			float NewX = (float)X;
			for(int Col=0; Col<m_NumCols; Col++)
			{
				RECT SubRect;
				if(IntersectRect(&SubRect, &rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect))
				{
					ret = m_Tiles[Row*m_NumCols+Col]->DisplayZoom(NewX, NewY, SubRect, ZoomX, ZoomY, Alpha, Transparent, BlendMode, MirrorX, MirrorY);
					if(FAILED(ret)) return ret;
				}
				NewX += (float)m_TileWidth * ZoomX / 100.0f;
			}
			NewY += (float)m_TileHeight * ZoomY / 100.0f;
		}
		return ret;
	}
	else
	{
		D3DXVECTOR2 pos(X,Y);
		return ((CBRenderD3D*)Game->m_Renderer)->DrawSprite(m_Texture, &rect, ZoomX, ZoomY, &pos, Alpha, !Transparent, BlendMode, MirrorX, MirrorY);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::DisplayTransform(int X, int Y, int HotX, int HotY, RECT Rect, float ZoomX, float ZoomY, DWORD Alpha, float Rotate, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	PrepareToDraw();
	
	if(m_NumTiles>0)
	{
		HRESULT ret = S_OK;
		float NewY = (float)Y;
		for(int Row=0; Row<m_NumRows; Row++)
		{
			float NewX = (float)X;
			for(int Col=0; Col<m_NumCols; Col++)
			{
				RECT SubRect;
				if(IntersectRect(&SubRect, &Rect, &m_Tiles[Row*m_NumCols+Col]->m_Rect))
				{
					ret = m_Tiles[Row*m_NumCols+Col]->DisplayTransform(NewX, NewY, HotX-(NewX-X)/(ZoomX/100), HotY-(NewY-Y)/(ZoomY/100), SubRect, ZoomX, ZoomY, Alpha, Rotate, BlendMode, MirrorX, MirrorY);
					if(FAILED(ret)) return ret;
				}
				NewX += (float)m_TileWidth * ZoomX / 100.0f;
			}
			NewY += (float)m_TileHeight * ZoomY / 100.0f;
		}
		return ret;
	}
	else
	{
		D3DXVECTOR2 pos(X,Y);
		D3DXVECTOR2 rot(X+HotX*(ZoomX/100), Y+HotY*(ZoomY/100));
		D3DXVECTOR2 scale(ZoomX/100.0f, ZoomY/100.0f);
		float Angle = DegToRad(Rotate);
		return ((CBRenderD3D*)Game->m_Renderer)->DrawSpriteEx(m_Texture, &Rect, &pos, &rot, &scale, Angle, Alpha, false, BlendMode, MirrorX, MirrorY);
	}
}


//////////////////////////////////////////////////////////////////////////
bool CBSurfaceD3D::IsTransparentAt(int X, int Y)
{
	PrepareToDraw();

	if(m_NumTiles>0)
	{
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->IsTransparentAt(X, Y);
	}

	bool ret = false;
	D3DSURFACE_DESC sd;
	D3DLOCKED_RECT lr;

	if(FAILED(m_Texture->GetLevelDesc(0, &sd))) return false;
	if(FAILED(m_Texture->LockRect(0, &lr, NULL, D3DLOCK_READONLY))) return false;
	
	DWORD pixel=0;
	switch(sd.Format)
	{
	case D3DFMT_A8R8G8B8:
		memcpy(&pixel, (BYTE*)lr.pBits + Y*lr.Pitch + X*4, sizeof(DWORD));
		pixel &= 0xFF000000;
		pixel >>= 24;
		//ret = (pixel!=0xFF);
		ret = (pixel<128);
		break;

	case D3DFMT_A1R5G5B5:
		memcpy(&pixel, (BYTE*)lr.pBits + Y*lr.Pitch + X*2, sizeof(DWORD));
		pixel &= 0x8000;
		pixel >>= 15;
		ret = (pixel!=0x01);
		break;

	case D3DFMT_A4R4G4B4:
		memcpy(&pixel, (BYTE*)lr.pBits + Y*lr.Pitch + X*2, sizeof(DWORD));
		pixel &= 0xF000;
		pixel >>= 12;
		//ret = (pixel!=0x0F);
		ret = (pixel<8);
		break;

	case D3DFMT_A8R3G3B2:
		memcpy(&pixel, (BYTE*)lr.pBits + Y*lr.Pitch + X*2, sizeof(DWORD));
		pixel &= 0xFF00;
		pixel >>= 8;
		//ret = (pixel!=0xFF);
		ret = (pixel<128);
		break;

	}

	m_Texture->UnlockRect(0);
	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::Create(int Width, int Height)
{
	HRESULT hr;
	
	m_Width  = Width;
	m_Height = Height;
	SetRectEmpty(&m_Rect);
	CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;

	// create the texture
	hr = D3DXCreateTexture( rend->m_Device,
							m_Width,
							m_Height,
							1, 0,
							rend->m_Force16bitTextures?D3DFMT_A1R5G5B5:D3DFMT_A8R8G8B8,
							D3DPOOL_MANAGED,
							&m_Texture);

	if(FAILED(hr))
	{
		Game->LOG(hr, "CBSurfaceD3D::Create: Error creating an empty texture");
		return hr;
	}

	if(FAILED(hr = GetTextureFormat()))
	{
		Game->LOG(hr, "CBSurfaceD3D::Create: Error getting texture format");
		return hr;
	}
	
	if(m_TextureFormat != D3DFMT_A8R8G8B8 && m_TextureFormat != D3DFMT_A1R5G5B5)
	{
		Game->LOG(0, "CBSurfaceD3D::Create: Unsupported texture format");
		return E_FAIL;
	}

	m_Valid = true;
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::CreateEmptyTexture(int Width, int Height)
{
	HRESULT hr;

	CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;

	// find best format
	D3DFORMAT Format = D3DFMT_A8R8G8B8;
	if(rend->m_Force16bitTextures) Format = D3DFMT_A1R5G5B5;

	D3DXCheckTextureRequirements(rend->m_Device, NULL, NULL, NULL, 0, &Format, D3DPOOL_MANAGED);
	if(Format!=D3DFMT_A8R8G8B8 && Format!=D3DFMT_A1R5G5B5)
	{
		Game->LOG(0, "Warning: non-standard recommended format: %d", Format);
		
		Format = D3DFMT_A1R5G5B5;
		D3DXCheckTextureRequirements(rend->m_Device, NULL, NULL, NULL, 0, &Format, D3DPOOL_MANAGED);
	}

	if(!Game->m_ReportTextureFormat)
	{
		Game->LOG(0, "Using texture format: %d", Format);
		Game->m_ReportTextureFormat = true;
	}
	
	// create the texture
	hr = D3DXCreateTexture( rend->m_Device,
							Width,
							Height,
							1, 0,
							Format,
							D3DPOOL_MANAGED,
							&m_Texture);

	if(FAILED(hr))
	{
		Game->LOG(hr, "CBSurfaceD3D::CreateEmptyTexture: Error creating an empty texture");
		return hr;
	}

	m_Width = Width;
	m_Height = Height;
	return GetTextureFormat();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::GetTextureFormat()
{
	HRESULT hr = S_OK;

	if(m_NumTiles>0)
	{
		for(int i=0; i<m_NumTiles; i++)
		{
			hr = m_Tiles[i]->GetTextureFormat();
			if(FAILED(hr)) return hr;
		}
	}
	else
	{
		if(!m_Texture) return E_FAIL;

		D3DSURFACE_DESC ddsd;
		if(SUCCEEDED(hr = m_Texture->GetLevelDesc(0, &ddsd))) m_TextureFormat = ddsd.Format;
	}

	return hr;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::StartPixelOp()
{
	if(m_PixelOpReady) return S_OK;

	PrepareToDraw();
	
	if(m_NumTiles>0)
	{
		for(int i=0; i<m_NumTiles; i++)
		{
			HRESULT hr = m_Tiles[i]->StartPixelOp();
			if(FAILED(hr)) return hr;
		}
	}
	else
	{
		if(FAILED(m_Texture->LockRect(0, &m_LockedRect, 0, 0))) return E_FAIL;

	
		if(FAILED(m_Texture->GetLevelDesc(0, &m_SurfaceDesc)))
		{
			m_Texture->UnlockRect(0);
			return E_FAIL;
		}
	}

	m_PixelOpReady = true;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::EndPixelOp()
{
	if(!m_PixelOpReady) return S_OK;

	if(m_NumTiles>0)
	{
		for(int i=0; i<m_NumTiles; i++)
		{
			HRESULT hr = m_Tiles[i]->EndPixelOp();
			if(FAILED(hr)) return hr;
		}
	}
	else m_Texture->UnlockRect(0);

	m_PixelOpReady = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::GetPixel(int X, int Y, BYTE *R, BYTE *G, BYTE *B, BYTE *A)
{
	if(!m_PixelOpReady) return E_FAIL;

	if(m_NumTiles>0)
	{
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->GetPixel(X, Y, R, G, B, A);
	}
	
		
	DWORD NativePixel;

	switch(m_TextureFormat)
	{
		case D3DFMT_A8R8G8B8:
			NativePixel = ((DWORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X];
			if(A) *A = (BYTE)((NativePixel&0xFF000000)>>24);
			if(R) *R = (BYTE)((NativePixel&0x00FF0000)>>16);
			if(G) *G = (BYTE)((NativePixel&0x0000FF00)>>8);
			if(B) *B = (BYTE)((NativePixel&0x000000FF));
			break;

		case D3DFMT_A4R4G4B4:
			NativePixel = ((WORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X];
			if(A) *A = (BYTE)((NativePixel&0xF000)>>12);
			if(R) *R = (BYTE)((NativePixel&0x0F00)>>8);
			if(G) *G = (BYTE)((NativePixel&0x00F0)>>4);
			if(B) *B = (BYTE)((NativePixel&0x000F));
			break;

		case D3DFMT_A1R5G5B5:
			NativePixel = ((WORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X];
			if(A) *A = (BYTE)((NativePixel&0x8000)>>15);
			if(R) *R = (BYTE)((NativePixel&0x7C00)>>10);
			if(G) *G = (BYTE)((NativePixel&0x03E0)>>5);
			if(B) *B = (BYTE)((NativePixel&0x001F));
			break;

		default:
			return E_FAIL;
	}

	/*
	// 32bit
	if(m_TextureFormat==D3DFMT_A8R8G8B8)
		NativePixel = ((DWORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X];
	// 16 bit
	else
		NativePixel = ((WORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X];

	if(m_TextureFormat == D3DFMT_A8R8G8B8)
	{
		if(A) *A = (BYTE)((NativePixel&0xFF000000)>>24);
		if(R) *R = (BYTE)((NativePixel&0x00FF0000)>>16);
		if(G) *G = (BYTE)((NativePixel&0x0000FF00)>>8);
		if(B) *B = (BYTE)((NativePixel&0x000000FF));
	}
	else if(m_TextureFormat == D3DFMT_A4R4G4B4)
	{
		if(A) *A = (BYTE)((NativePixel&0xF000)>>12);
		if(R) *R = (BYTE)((NativePixel&0x0F00)>>8);
		if(G) *G = (BYTE)((NativePixel&0x00F0)>>4);
		if(B) *B = (BYTE)((NativePixel&0x000F));
	}
    else if(m_TextureFormat == D3DFMT_A1R5G5B5)
	{
		if(A) *A = (BYTE)((NativePixel&0x8000)>>15);
		if(R) *R = (BYTE)((NativePixel&0x7C00)>>10);
		if(G) *G = (BYTE)((NativePixel&0x03E0)>>5);
		if(B) *B = (BYTE)((NativePixel&0x001F));
	}
	else return E_FAIL;
	*/

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::PutPixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A)
{
	if(!m_PixelOpReady) return E_FAIL;

	if(m_NumTiles>0)
	{
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->PutPixel(X, Y, R, G, B, A);
	}


	DWORD NativePixel;
	BYTE AA;
	
	if(A>0) AA = (BYTE)A;
	else GetPixel(X, Y, NULL, NULL, NULL, &AA);

	switch(m_TextureFormat)
	{
		case D3DFMT_A8R8G8B8:
			NativePixel = (AA<<24) + (R<<16) + (G<<8) + (B);
			((DWORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X] = NativePixel;
			break;
		case D3DFMT_A4R4G4B4:
			NativePixel = ((AA>>4)<<16) + ((R>>4)<<8) + ((G>>4)<<4) + (B>>4);
			((WORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X] = NativePixel;
			break;
		case D3DFMT_A1R5G5B5:
			NativePixel = ((AA?1:0)<<15) + ((R>>3)<<10) + ((G>>3)<<5) + (B>>3);	
			((WORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X] = NativePixel;
			break;
		default:
			return E_FAIL;
	}

/*
	if(m_TextureFormat == D3DFMT_A8R8G8B8)
		NativePixel = (AA<<24) + (R<<16) + (G<<8) + (B);
	else if(m_TextureFormat == D3DFMT_A4R4G4B4)
		NativePixel = ((AA>>4)<<16) + ((R>>4)<<8) + ((G>>4)<<4) + (B>>4);
    else if(m_TextureFormat == D3DFMT_A1R5G5B5)
		NativePixel = ((AA?1:0)<<15) + ((R>>3)<<10) + ((G>>3)<<5) + (B>>3);	
	else return E_FAIL;

	// 32bit
	if(m_TextureFormat==D3DFMT_A8R8G8B8)
		((DWORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X] = NativePixel;
	// 16 bit
	else
		((WORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X] = NativePixel;
*/

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBSurfaceD3D::ComparePixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A)
{
	if(!m_PixelOpReady) return false;

	if(m_NumTiles>0)
	{
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->ComparePixel(X, Y, R, G, B, A);
	}


	DWORD NativePixel;
	BYTE AA;
	
	if(A>0) AA = (BYTE)A;
	else GetPixel(X, Y, NULL, NULL, NULL, &AA);


	if(m_TextureFormat == D3DFMT_A8R8G8B8)
		NativePixel = (AA<<24) + (R<<16) + (G<<8) + (B);
	else if(m_TextureFormat == D3DFMT_A4R4G4B4)
		NativePixel = ((AA>>4)<<16) + ((R>>4)<<8) + ((G>>4)<<4) + (B>>4);
    else if(m_TextureFormat == D3DFMT_A1R5G5B5)
		NativePixel = ((AA?1:0)<<15) + ((R>>3)<<10) + ((G>>3)<<5) + (B>>3);	
	else return false;

	bool ret;

	// 32bit
	if(m_TextureFormat==D3DFMT_A8R8G8B8)
		ret = (((DWORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X] == NativePixel);
	// 16 bit
	else
		ret = (((WORD*)m_LockedRect.pBits)[m_SurfaceDesc.Width*Y+X] == NativePixel);

	return ret;
}


//////////////////////////////////////////////////////////////////////
bool CBSurfaceD3D::IsTransparentAtLite(int X, int Y)
{
	if(X < 0 || Y < 0 || X >= m_Width || Y >= m_Height) return false;
	
	if(!m_PixelOpReady) return false;
	PrepareToDraw();

	if(m_NumTiles>0)
	{
		int Col = X / m_TileWidth;
		int Row = Y / m_TileHeight;
		X %= m_TileWidth;
		Y %= m_TileHeight;
		return m_Tiles[Row*m_NumCols+Col]->IsTransparentAtLite(X, Y);
	}

	bool ret = false;
	
	DWORD pixel=0;
	switch(m_TextureFormat)
	{
	case D3DFMT_A8R8G8B8:
		memcpy(&pixel, (BYTE*)m_LockedRect.pBits + Y*m_LockedRect.Pitch + X*4, sizeof(DWORD));
		pixel &= 0xFF000000;
		pixel >>= 24;
		//ret = (pixel!=0xFF);
		ret = (pixel==0x00);
		break;

	case D3DFMT_A1R5G5B5:
		memcpy(&pixel, (BYTE*)m_LockedRect.pBits + Y*m_LockedRect.Pitch + X*2, sizeof(DWORD));
		pixel &= 0x8000;
		pixel >>= 15;
		//ret = (pixel!=0x01);
		ret = (pixel==0x00);
		break;

	case D3DFMT_A4R4G4B4:
		memcpy(&pixel, (BYTE*)m_LockedRect.pBits + Y*m_LockedRect.Pitch + X*2, sizeof(DWORD));
		pixel &= 0xF000;
		pixel >>= 12;
		//ret = (pixel!=0x0F);
		ret = (pixel==0x00);
		break;

	case D3DFMT_A8R3G3B2:
		memcpy(&pixel, (BYTE*)m_LockedRect.pBits + Y*m_LockedRect.Pitch + X*2, sizeof(DWORD));
		pixel &= 0xFF00;
		pixel >>= 8;
		//ret = (pixel!=0xFF);
		ret = (pixel==0xFF);
		break;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::Invalidate()
{
	if(m_NumTiles>0)
	{
		for(int i=0; i<m_NumTiles; i++)
		{
			HRESULT hr = m_Tiles[i]->Invalidate();
			if(FAILED(hr)) return hr;
		}
	}	
	else
	{
		CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
		if(Rend) Rend->InvalidateTexture(m_Texture);

		RELEASE(m_Texture);
	}

	m_Valid = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::ReleaseTiles()
{
	if(m_NumTiles>0)
	{
		for(int i=0; i<m_NumTiles; i++)
		{
			if(m_Tiles[i]) delete m_Tiles[i];
		}
		delete [] m_Tiles;
	}
	m_Tiles = NULL;
	m_NumTiles = 0;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurfaceD3D::PrepareToDraw()
{
	if(!m_Child) return CBSurface::PrepareToDraw();
	else return S_OK;
}
