// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "VidRendererD3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CVidRendererD3D::CVidRendererD3D(CBGame* inGame):CVidRenderer(inGame)
{

}


//////////////////////////////////////////////////////////////////////////
CVidRendererD3D::~CVidRendererD3D()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRendererD3D::ProcessFrame(LPBITMAPINFOHEADER Frame)
{
	CBSurfaceD3D* surf = (CBSurfaceD3D*)m_Surface;

	LONG BmpPitch = Frame->biWidth * (Frame->biBitCount >> 3);	
	BYTE* BmpBuffer = (BYTE*)Frame+Frame->biSize+Frame->biClrUsed * sizeof(RGBQUAD);
	// bitmap is bottom up...
	BmpBuffer += BmpPitch * (Frame->biHeight - 1);

	// Lock the Texture
	D3DLOCKED_RECT d3dlr;
	if(FAILED(surf->m_Texture->LockRect(0, &d3dlr, 0, 0))) return E_FAIL;
	
	// Get the texture buffer & pitch
	BYTE* TxtBuffer =(BYTE*)(d3dlr.pBits);

	RenderFrame(BmpBuffer, TxtBuffer, BmpPitch, d3dlr.Pitch, Frame->biWidth, Frame->biHeight);

	// Unlock the Texture
	if(FAILED(surf->m_Texture->UnlockRect(0)))
		return E_FAIL;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRendererD3D::ProcessFrameSlow(LPBITMAPINFOHEADER Frame)
{	
	CBSurfaceD3D* surf = (CBSurfaceD3D*)m_Surface;

	// for divided textures go the put-pixel way
	if(surf->m_NumCols > 1 || surf->m_NumRows > 1) return ProcessFrameDivTex(Frame);

	int Pitch = (surf->GetWidth() * 3 + 3) & ~(3); // We are forcing RGB24
	
	BYTE  *pBmpBuffer, *pTxtBuffer; 	// Bitmap buffer, texture buffer
	LONG  lTxtPitch;
	
	pBmpBuffer = (BYTE*) Frame+Frame->biSize+Frame->biClrUsed * sizeof(RGBQUAD);
		

	// Lock the Texture
	D3DLOCKED_RECT d3dlr;
	if(FAILED(surf->m_Texture->LockRect(0, &d3dlr, 0, 0)))
		return E_FAIL;
	
	// Get the texture buffer & pitch
	pTxtBuffer =(BYTE*)(d3dlr.pBits);
	lTxtPitch = d3dlr.Pitch;
	
	pTxtBuffer += (surf->GetHeight()-1) * lTxtPitch;

	// Copy the bits	
	if(surf->m_TextureFormat == D3DFMT_A8R8G8B8){
		for(int y = 0; y < surf->GetHeight(); y++){		
			BYTE *pBmpBufferOld = pBmpBuffer;
			BYTE *pTxtBufferOld = pTxtBuffer;
			for(int x = 0; x < surf->GetWidth(); x++){
				pTxtBuffer[0] = pBmpBuffer[0];
				pTxtBuffer[1] = pBmpBuffer[1];
				pTxtBuffer[2] = pBmpBuffer[2];
				pTxtBuffer[3] = 0xff;
				pTxtBuffer += 4;
				pBmpBuffer += 3;
			}
			pBmpBuffer = pBmpBufferOld + Pitch;
			pTxtBuffer = pTxtBufferOld - lTxtPitch;
		}
	}
	
	if(surf->m_TextureFormat == D3DFMT_A1R5G5B5){
		for(int y = 0; y < surf->GetHeight(); y++){
			BYTE *pBmpBufferOld = pBmpBuffer;
			BYTE *pTxtBufferOld = pTxtBuffer;	
			for (int x = 0; x < surf->GetWidth(); x++){
				*(WORD *)pTxtBuffer =
					0x8000 +
					((pBmpBuffer[2] & 0xF8) << 7) +
					((pBmpBuffer[1] & 0xF8) << 2) +
					(pBmpBuffer[0] >> 3);
				pTxtBuffer += 2;
				pBmpBuffer += 3;
			}
			pBmpBuffer = pBmpBufferOld + Pitch;
			pTxtBuffer = pTxtBufferOld - lTxtPitch;
		}
	}
		
	// Unlock the Texture
	if(FAILED(surf->m_Texture->UnlockRect(0)))
		return E_FAIL;
	

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRendererD3D::ProcessFrameDivTex(LPBITMAPINFOHEADER Frame)
{
	int Pitch = (m_Surface->GetWidth() * 3 + 3) & ~(3); // We are forcing RGB24
	BYTE  *pBmpBuffer = (BYTE*) Frame+Frame->biSize+Frame->biClrUsed * sizeof(RGBQUAD);

	int Width = m_Surface->GetWidth();
	int Height = m_Surface->GetHeight();

	m_Surface->StartPixelOp();
	for(int row = 0; row < Height; row++)
	{
		BYTE* buf = pBmpBuffer + (Height - row - 1) * Pitch;
		for(int col = 0; col < Width; col++)
		{			
			m_Surface->PutPixel(col, row, buf[2], buf[1], buf[0], 0xff);
			buf += 3;
		}
	}
	m_Surface->EndPixelOp();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CVidRendererD3D::GetTargetFormat(LPBITMAPV4HEADER Format)
{
	CBSurfaceD3D* surf = (CBSurfaceD3D*)m_Surface;
	
	Format->bV4V4Compression = BI_BITFIELDS;
	Format->bV4ClrUsed = 0;	

	D3DSURFACE_DESC sd;
	if(FAILED(surf->m_Texture->GetLevelDesc(0, &sd))) return E_FAIL;
	
	switch(sd.Format){
	case D3DFMT_A8R8G8B8:
		Format->bV4BitCount  = 32;
		Format->bV4AlphaMask = 0xFF000000;
		Format->bV4RedMask   = 0x00FF0000;
		Format->bV4GreenMask = 0x0000FF00;
		Format->bV4BlueMask  = 0x000000FF;
		break;

	case D3DFMT_A1R5G5B5:
		Format->bV4BitCount  = 16;
		Format->bV4AlphaMask = 0x8000;
		Format->bV4RedMask   = 0x7C00;
		Format->bV4GreenMask = 0x03E0;
		Format->bV4BlueMask  = 0x001F;
		break;

	case D3DFMT_A4R4G4B4:
		Format->bV4BitCount  = 16;
		Format->bV4AlphaMask = 0xF000;
		Format->bV4RedMask   = 0x0F00;
		Format->bV4GreenMask = 0x00F0;
		Format->bV4BlueMask  = 0x000F;
		break;

	case D3DFMT_A8R3G3B2:
		Format->bV4BitCount  = 16;
		Format->bV4AlphaMask = 0xFF00;
		Format->bV4RedMask   = 0x00E0;
		Format->bV4GreenMask = 0x001C;
		Format->bV4BlueMask  = 0x000C;
		break;
	}

	Format->bV4SizeImage = ((Format->bV4Width+3)&0xFFFFFFFC)*Format->bV4Height * (Format->bV4BitCount>>3);

	return S_OK;
}
