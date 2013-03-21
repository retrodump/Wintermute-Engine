// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BImage.h"
#include "2passScale.h"
#include "3dUtils.h"

#if _DEBUG
	#pragma comment(lib, "libpng_d.lib")
	#pragma comment(lib, "libjpeg_d.lib")
#else
	#pragma comment(lib, "libpng.lib")
	#pragma comment(lib, "libjpeg.lib")
#endif


//////////////////////////////////////////////////////////////////////
CBImage::CBImage(CBGame* inGame):CBBase(inGame)
{
	m_Width = m_Height = 0;
	
	m_Image = NULL;
	m_AlphaMap = NULL;
	m_Palette = NULL;
	m_Type = IMG_PALETTED8;

	m_Filename = NULL;
}


//////////////////////////////////////////////////////////////////////
CBImage::~CBImage()
{
	FreeAll();
}


//////////////////////////////////////////////////////////////////////
void CBImage::FreeAll()
{
	if(m_Filename != NULL) delete [] m_Filename;

	if(m_Image != NULL){
		switch(m_Type){
			case IMG_PALETTED8: delete [] (BYTE*) m_Image; break;
			case IMG_TRUECOLOR: delete [] (RGBPixel*) m_Image; break;
		}
	}

	if(m_Palette != NULL) delete [] m_Palette;
	if(m_AlphaMap != NULL) delete [] m_AlphaMap;

	m_Filename = NULL;
	m_Image = NULL;
	m_Palette = NULL;
	m_AlphaMap = NULL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBImage::LoadFile(char * Filename)
{
	FreeAll();

	DWORD Size=0;
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename, &Size);
	if(Buffer==NULL){
		Game->LOG(0, "CBImage::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	// TODO better type detection

	HRESULT ret;
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, NULL, ext);
	if     (stricmp(ext, ".BMP")==0 || strnicmp(Filename, "savegame:", 9)==0) ret = LoadBMPFile(Filename, Buffer, Size);
	else if(stricmp(ext, ".TGA")==0) ret = LoadTGAFile(Filename, Buffer, Size);
	else if(stricmp(ext, ".PNG")==0) ret = LoadPNGFile(Filename, Buffer, Size);
	else if(stricmp(ext, ".JPG")==0) ret = LoadJPGFile(Filename, Buffer, Size);
	//else if(_stricmp(ext, ".PCX")==0) ret = LoadPCXFile(Filename, Buffer, Size);
	else {
		Game->LOG(0, "CBImage::LoadFile - file '%s' is an unknown image type", Filename);
		ret = E_FAIL;
	}

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(strnicmp(Filename, "savegame:", 9)==0){
		strupr(Filename);
		if(Filename[strlen(Filename)-2]==',' && Filename[strlen(Filename)-1]=='G') Grayscale();
	}

	delete [] Buffer;

	return ret;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBImage::Convert(TImageType Type)
{
	if(m_Type == Type) return S_OK;

	BYTE* new_image = NULL;

	switch(m_Type) {
		case IMG_PALETTED8:
			if(Type==IMG_TRUECOLOR){
				new_image = (BYTE*)new RGBPixel [m_Width*m_Height];
				if(new_image==NULL){
					Game->LOG(0, "CBImage::Convert (pal->RGB) failed for '%s'", m_Filename);
					return E_FAIL;
				}
				for(int line=0; line<m_Height; line++){
					for(int col=0; col<m_Width; col++){
						((RGBPixel*)new_image)[line*m_Width+col] = m_Palette[((BYTE*)m_Image)[line*m_Width+col]];
					}
				}
				delete [] m_Palette;
				m_Palette = NULL;

				delete [] (BYTE*)m_Image;
				m_Image = new_image;
				m_Type = IMG_TRUECOLOR;
			}
		break;

		case IMG_TRUECOLOR:
			if(Type==IMG_PALETTED8){
				RGBPixel* new_palette = NULL;
				//QuantizeRGB((RGBPixel*)m_Image, m_Width*m_Height, new_image, new_palette);
				if(new_image==NULL){
					Game->LOG(0, "CBImage::Convert (RGB->pal) failed for '%s'", m_Filename);
					return E_FAIL;
				}
				delete [] (RGBPixel*)m_Image;
				m_Image = new_image;
				if(m_Palette!=NULL) delete [] m_Palette;
				m_Palette = new_palette;
				m_Type = IMG_PALETTED8;
			}
		break;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
LPDIRECTDRAWSURFACE7 CBImage::CreateSurface(int * Width, int * Height, bool ForceSysMem, BYTE Red, BYTE Green, BYTE Blue)
{
	LPDIRECTDRAWSURFACE7 surface = NULL;
	DDSURFACEDESC2 ddsd;

	CBRenderDD* renderer = (CBRenderDD*)Game->m_Renderer;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = m_Width;
	ddsd.dwHeight = m_Height;

	if(ForceSysMem) ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	HRESULT res = renderer->m_DirectDraw->CreateSurface(&ddsd, &surface, NULL);
	if(res != DD_OK){
		Game->LOG(0, "CBImage::CreateSurface failed for image '%s'", m_Filename);
		return NULL;
	}

	surface->GetSurfaceDesc(&ddsd);

	if(ddsd.ddpfPixelFormat.dwRGBBitCount < 8 || (((ddsd.ddpfPixelFormat.dwFlags&DDPF_PALETTEINDEXED8)==0) && ((ddsd.ddpfPixelFormat.dwFlags&DDPF_RGB)==0))){
		RELEASE(surface);
		Game->LOG(0, "CBImage::CreateSurface - invalid surface format for image '%s'", m_Filename);
		return NULL;
	}

	// set the right format of image
	if(ddsd.ddpfPixelFormat.dwRGBBitCount == 8) Convert(IMG_PALETTED8);
	else Convert(IMG_TRUECOLOR);
	if(m_Image==NULL){
		RELEASE(surface);
		Game->LOG(0, "CBImage::CreateSurface - can't create surface out of NULL image ('%s')", m_Filename);
		return NULL;
	}

	int line, col;

	// fill surface
	res = surface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL);
	if(res != DD_OK){
		Game->LOG(0, "CBImage::CreateSurface - error locking surface for image '%s'", m_Filename);
		return NULL;
	}


	if(ddsd.ddpfPixelFormat.dwRGBBitCount == 8) {
		for(line=0; line<m_Height; line++){
			memcpy((BYTE*)ddsd.lpSurface + line*ddsd.lPitch, (BYTE*)m_Image + line*m_Width, m_Width);
		}

		// create palette
		LPDIRECTDRAWPALETTE pal;
		PALETTEENTRY entries[256];
		for(int i=0; i<256; i++){
			entries[i].peRed   = m_Palette[i].red;
			entries[i].peGreen = m_Palette[i].green;
			entries[i].peBlue  = m_Palette[i].blue;
			entries[i].peFlags = 0;
		}
		res = renderer->m_DirectDraw->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, entries, &pal, NULL);
		if(res != DD_OK){
			Game->LOG(0, "CBImage::CreateSurface - error creating palette ('%s')", m_Filename);
			RELEASE(surface);
			return NULL;
		}

		res = surface->SetPalette(pal);
		if(res != DD_OK){
			RELEASE(surface);
			RELEASE(pal);
			Game->LOG(0, "CBImage::CreateSurface - error setting palette ('%s')", m_Filename);
			RELEASE(surface);
			return NULL;
		}
	}

	// rgb surface
	else{
		DWORD i, j, rshift, gshift, bshift, rbits, gbits, bbits;
		DDPIXELFORMAT pf = ddsd.ddpfPixelFormat;
	
		j = (int) pf.dwRBitMask; rshift = 0;
		i = 1; while (!(i&j)) { rshift++; i<<=1; }
		rbits = 0; while (i&j) { rbits++; i<<=1; }
		j = (int) pf.dwGBitMask; gshift = 0;
		i = 1; while (!(i&j)) { gshift++; i<<=1; }
		gbits = 0; while (i&j) { gbits++; i<<=1; }
		j = (int) pf.dwBBitMask; bshift = 0;
		i = 1; while (!(i&j)) { bshift++; i<<=1; }
		bbits = 0; while (i&j) { bbits++; i<<=1; }
	
		int num_bytes = pf.dwRGBBitCount / 8;
		BYTE R, G, B, A;

		for(line=0; line<m_Height; line++){
			for(col=0; col<m_Width; col++){
				R = ((RGBPixel*)m_Image)[line*m_Width+col].red;
				G = ((RGBPixel*)m_Image)[line*m_Width+col].green;
				B = ((RGBPixel*)m_Image)[line*m_Width+col].blue;
				A = ((RGBPixel*)m_Image)[line*m_Width+col].alpha;

				if(A < 128){
					R = Red;
					G = Green;
					B = Blue;
				}

				DWORD pixel = ((R>>(8-rbits)) << rshift) +
						      ((G>>(8-gbits)) << gshift) +
							  ((B>>(8-bbits)) << bshift);
				
				/*
				DWORD pixel =	(((((RGBPixel*)m_Image)[line*m_Width+col].red  <<rshift) >>(8-rbits)) & pf.dwRBitMask) |
      							(((((RGBPixel*)m_Image)[line*m_Width+col].green<<gshift) >>(8-gbits)) & pf.dwGBitMask) |
								(((((RGBPixel*)m_Image)[line*m_Width+col].blue <<bshift) >>(8-bbits)) & pf.dwBBitMask);
				*/
				memcpy((BYTE*)ddsd.lpSurface + line*ddsd.lPitch + col*num_bytes, &pixel, num_bytes);
			}
		}	
	}
	surface->Unlock(NULL);


	if(Width!=NULL)  *Width = m_Width;
	if(Height!=NULL) *Height = m_Height;
	return surface;
}


//////////////////////////////////////////////////////////////////////
LPDIRECTDRAWPALETTE CBImage::CreatePalette()
{
	Convert(IMG_PALETTED8);
	if(m_Image==NULL){
		Game->LOG(0, "CBImage::CreatePalette - can't create palette out of NULL image ('%s')", m_Filename);
		return NULL;
	}

	LPDIRECTDRAWPALETTE pal;
	PALETTEENTRY entries[256];
	for(int i=0; i<256; i++){
		entries[i].peRed   = m_Palette[i].red;
		entries[i].peGreen = m_Palette[i].green;
		entries[i].peBlue  = m_Palette[i].blue;
		entries[i].peFlags = 0;
	}
	HRESULT res = ((CBRenderDD*)Game->m_Renderer)->m_DirectDraw->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256, entries, &pal, NULL);
	if(res != DD_OK){
		Game->LOG(0, "CBImage::CreatePalette - error creating palette ('%s')", m_Filename);
		return NULL;
	}

	return pal;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBImage::LoadBMPFile(char * Filename, BYTE* Buffer, DWORD Size)
{
	DWORD i;

	BITMAPFILEHEADER* FileHeader = (BITMAPFILEHEADER*)Buffer;
	if(FileHeader->bfType != 0x4d42){ // "BM"
		Game->LOG(0, "File '%s' is not a valid BMP image", Filename);
		return E_FAIL;
	}

	BYTE* Bits = (BYTE*)(Buffer + FileHeader->bfOffBits);
	BITMAPINFOHEADER* InfoHeader = (BITMAPINFOHEADER*)(Buffer + sizeof(BITMAPFILEHEADER));
	if(InfoHeader->biCompression!=BI_RGB || (InfoHeader->biBitCount != 8 && InfoHeader->biBitCount != 24 && InfoHeader->biBitCount != 16 && InfoHeader->biBitCount != 32)){
		Game->LOG(0, "Bitmap image '%s' has an unsupported format", Filename);
		return E_FAIL;
	}

	m_Width = InfoHeader->biWidth;
	m_Height = abs(InfoHeader->biHeight);
	
	int pitch = m_Width * (InfoHeader->biBitCount / 8);
	if(pitch%4 != 0) pitch += 4 - pitch%4;

	// 8 BPP, load palette
	if(InfoHeader->biBitCount==8){
		m_Palette = new RGBPixel[256];		
		if(m_Palette==NULL){
			Game->LOG(0, "CBImage::LoadBMPFile - Error allocating memory for palette (%s)", Filename);
			return E_FAIL;
		}
		RGBQUAD* pal = (RGBQUAD*)(Buffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
		DWORD NumColors = InfoHeader->biClrUsed;
		if(!NumColors) NumColors = 256;
		for(i=0; i<NumColors; i++){
			m_Palette[i].red   = pal[i].rgbRed;
			m_Palette[i].green = pal[i].rgbGreen;
			m_Palette[i].blue  = pal[i].rgbBlue;
		}

		m_Type = IMG_PALETTED8;
		m_Image = new BYTE[m_Width*m_Height];
	}
	// true color
	else {
		m_Type = IMG_TRUECOLOR;
		m_Image = new RGBPixel[m_Width*m_Height];
	}

	if(m_Image==NULL){
		Game->LOG(0, "CBImage::LoadBMPFile - Error allocating memory for image (%s)", Filename);
		if(m_Palette!=NULL) delete [] m_Palette;
		return E_FAIL;
	}
	
	// read bits
	for(int line=0; line<m_Height; line++){
		if(InfoHeader->biBitCount==8)
			memcpy(((BYTE*)m_Image)+line*m_Width, Bits+(m_Height-line-1)*pitch, m_Width);
		else{
			for(int col=0; col<m_Width; col++){
				switch(InfoHeader->biBitCount){
				case 24:
					((RGBPixel*)m_Image)[line*m_Width+col].blue = Bits[(m_Height-line-1)*pitch+col*3];
					((RGBPixel*)m_Image)[line*m_Width+col].green = Bits[(m_Height-line-1)*pitch+col*3+1];
					((RGBPixel*)m_Image)[line*m_Width+col].red = Bits[(m_Height-line-1)*pitch+col*3+2];
				break;

				case 32:
					((RGBPixel*)m_Image)[line*m_Width+col].blue = Bits[(m_Height-line-1)*pitch+col*4];
					((RGBPixel*)m_Image)[line*m_Width+col].green = Bits[(m_Height-line-1)*pitch+col*4+1];
					((RGBPixel*)m_Image)[line*m_Width+col].red = Bits[(m_Height-line-1)*pitch+col*4+2];
					//((RGBPixel*)m_Image)[line*m_Width+col].alpha = Bits[(m_Height-line-1)*pitch+col*4+3];
				break;

				case 16:
				{
					WORD pixel = *(WORD*)&Bits[(m_Height-line-1)*pitch+col*2];
					((RGBPixel*)m_Image)[line*m_Width+col].blue = (pixel&0x001F)<<3;
					((RGBPixel*)m_Image)[line*m_Width+col].green = ((pixel&0x03E0)>>5)<<3;
					((RGBPixel*)m_Image)[line*m_Width+col].red = ((pixel&0x7C00)>>10)<<3;
				}
				break;
				}
			}
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBImage::LoadTGAFile(char * Filename, BYTE* Buffer, DWORD Size)
{
	int i;

	_tgaHeader* Header = (_tgaHeader*)Buffer;
	BYTE* Bits = Buffer + sizeof(_tgaHeader) + Header->cIDLength;
	bool Inverted = true;
	if(Header->cDescriptorBits & 0x20) Inverted = false;

	m_Width = Header->iWidth;
	m_Height = Header->iHeight;

	// palette
	if(Header->cColorMapType ){

        int color_map_length = Header->iColourMapLength;
        int color_map_start  = Header->iFirstColourMapEntry;
		int bpp_pal = Header->cColourMapEntrySize;
		
        if(bpp_pal==15) bpp_pal = 16;

        if (Header->cBPP == 8){

            m_Palette = new RGBPixel[256];
			if(m_Palette==NULL){
				Game->LOG(0, "CBImage::LoadTGAFile - Error allocating memory for palette (%s)", Filename);
				return E_FAIL;
			}

            switch (bpp_pal){

				case 16:
					for (i=color_map_start;i<color_map_length;i++){
                    
						if (i >= 256) break;
						WORD data = TGAGetWord(Bits);

						m_Palette[i].blue  = ((data >> 10) & 0x1F) << 3; // blue
						m_Palette[i].green = ((data >> 5) & 0x1F) << 3; // green
						m_Palette[i].red   = (data & 0x1F) << 3; // red
					}
				break;

				case 24:
					for (i=color_map_start;i<color_map_length;i++){
                    
						if (i >= 256) break;

						m_Palette[i].blue  = *Bits++; // blue
						m_Palette[i].green = *Bits++; // green
						m_Palette[i].red   = *Bits++; // red
					}
				break;
            }		
        }
        else{
		    Bits += (long)color_map_length * ( (long)bpp_pal / 8L );
        }
	}


	// allocate mem and set type
	if(Header->cBPP == 8){
		m_Type = IMG_PALETTED8;
		m_Image = new BYTE[m_Width*m_Height];
	}
	else{
		m_Type = IMG_TRUECOLOR;
		m_Image = new RGBPixel[m_Width*m_Height];
	}
	
	if(m_Image==NULL){
		Game->LOG(0, "CBImage::LoadTGAFile - Error allocating memory for image (%s)", Filename);
		if(m_Palette!=NULL) delete [] m_Palette;
		return E_FAIL;
	}

    Bits = Buffer + sizeof(_tgaHeader) + Header->cIDLength + (Header->iColourMapLength * (Header->cColourMapEntrySize==15?16:Header->cColourMapEntrySize)/8);

	//int srcWidth = m_Width * (Header->cBPP / 8);
	

	//////////////////////////////////////////////////////////////////////////	
	WORD c;
	BYTE r,g,b,a;

	switch(Header->cType){
		///// RLE compressed
		case TGA_RL_CM:
		case TGA_RL_BW:
		case TGA_RL_RGB:
		{
			int x=0, y=0;

			switch(Header->cBPP){
			case 8:
			{
				while(y < m_Height){
				
					int i, cnt = *Bits++;
					if (cnt == -1){
						SAFE_DELETE_ARRAY(m_Palette);
						SAFE_DELETE_ARRAY(m_Image);
						return E_FAIL;
					}

					if(cnt & 0x80){
						BYTE v = *Bits++;
						for(i=0x80; i<=cnt; i++){
							((BYTE*)m_Image)[y*m_Width + x] = v;
							x++;
							
							if(x >= m_Width){ 
								x = 0; 
								y++; 
							}
							if (y > m_Height) break;
						}
					}
					else{
						for(i=0; i<=cnt; i++){
							((BYTE*)m_Image)[y*m_Width + x] = *Bits++;
							x++;

							if(x >= m_Width){ 
								x = 0; 
								y++; 
							}
							if (y > m_Height) break;
						}
					}
				}
			}
			break; // case 8

			case 16:
			{				
				while (y < m_Height){
					int i, cnt = *Bits++;
                
					if(cnt == -1){
						SAFE_DELETE_ARRAY(m_Palette);
						SAFE_DELETE_ARRAY(m_Image);
						return E_FAIL;
					}

					if (cnt & 0x80){
					    c = TGAGetWord(Bits);

					    r = ((c >> 10) & 0x1f) << 3;
					    g = ((c >> 5) & 0x1f) << 3;
					    b = (c & 0x1f) << 3;

						for (i=0x80; i<=cnt; i++){
							((RGBPixel*)m_Image)[y*m_Width + x].red = r;
							((RGBPixel*)m_Image)[y*m_Width + x].green = g;
							((RGBPixel*)m_Image)[y*m_Width + x].blue = b;
							x++;

							if(x >= m_Width){ 
								x = 0; 
								y++; 
							}
							if(y >= m_Height) break;
						}
					}
					else{
						for(i=0; i<=cnt; i++){
							c = TGAGetWord(Bits);
					      
							r = ((c >> 10) & 0x1f) << 3;
							g = ((c >> 5) & 0x1f) << 3;
							b = (c & 0x1f) << 3;

							((RGBPixel*)m_Image)[y*m_Width + x].red = r;
							((RGBPixel*)m_Image)[y*m_Width + x].green = g;
							((RGBPixel*)m_Image)[y*m_Width + x].blue = b;
							x++;

							if (x >= m_Width){ 
								x = 0; 
								y++; 
							}
							if (y >= m_Height) break;
						}
					}
				}
			}
			break; // case 16

			case 24:
			case 32:
			{
				while(y < m_Height){
            
					int i, cnt = *Bits++;

					if (cnt == -1){
						SAFE_DELETE_ARRAY(m_Palette);
						SAFE_DELETE_ARRAY(m_Image);
						return E_FAIL;
					}

					if(cnt & 0x80){
						BYTE v1 = *Bits++;
						BYTE v2 = *Bits++;
						BYTE v3 = *Bits++;
						BYTE v4 = *Bits++;

						for(i=0x80; i<=cnt; i++){
							((RGBPixel*)m_Image)[y*m_Width + x].blue = v1;
							((RGBPixel*)m_Image)[y*m_Width + x].green = v2;
							((RGBPixel*)m_Image)[y*m_Width + x].red = v3;
							((RGBPixel*)m_Image)[y*m_Width + x].alpha = v4;
							x++;
							
							if(x >= m_Width){ 
								x = 0; 
								y++; 
							}

							if (y >= m_Height) break;
						}
					}
					else {
						for (i=0; i<=cnt; i++){
							((RGBPixel*)m_Image)[y*m_Width + x].blue = *Bits++;
							((RGBPixel*)m_Image)[y*m_Width + x].green = *Bits++;
							((RGBPixel*)m_Image)[y*m_Width + x].red = *Bits++;
							if(Header->cBPP==32){								
								((RGBPixel*)m_Image)[y*m_Width + x].alpha = *Bits++;
							}
							x++;

							if (x >= m_Width){
								x = 0; 
								y++; 
							}

							if (y >= m_Height) break;
						}
					}
				}
			}
			break; // case 24, 32
			} // switch(cBPP)
		}
		break;

		///// uncompressed
		case TGA_UNCOMP_BW:
		case TGA_UNCOMP_CM:
		case TGA_UNCOMP_RGB:
		{
			switch (Header->cBPP){
				case 8:
					for(i=0; i<m_Height; i++){
						memcpy(((BYTE*)m_Image) + i*m_Width, Bits, m_Width);
						Bits += m_Width;
					}
				break;

				case 16:
					for(i=0; i<m_Height; i++){
						for(int w=0; w<m_Width; w++){
							c = TGAGetWord(Bits);
							r = ((c >> 10) & 0x1f) << 3;
							g = ((c >> 5) & 0x1f) << 3;
							b = (c & 0x1f) << 3;
							a = (c >> 11);
							if(a) a = 255;

							((RGBPixel*)m_Image)[i*m_Width + w].red = r;
							((RGBPixel*)m_Image)[i*m_Width + w].green = g;
							((RGBPixel*)m_Image)[i*m_Width + w].blue = b;
							((RGBPixel*)m_Image)[i*m_Width + w].alpha = a;
						}
					}
				break;

				case 24:
				case 32:
					for(i=0; i<m_Height; i++){
						for(int w=0; w<m_Width; w++){
							
							b = *Bits++;
							g = *Bits++;
							r = *Bits++;
							if(Header->cBPP==32){
								a = *Bits++;
								//*Bits++; // alpha
							}

							((RGBPixel*)m_Image)[i*m_Width + w].red = r;
							((RGBPixel*)m_Image)[i*m_Width + w].green = g;
							((RGBPixel*)m_Image)[i*m_Width + w].blue = b;
							((RGBPixel*)m_Image)[i*m_Width + w].alpha = a;
						}
					}

				break;
			} // switch cBPP
		}
		break;
	
	} // switch(cType)


	if(Inverted) return Flip();
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
WORD CBImage::TGAGetWord(BYTE* &buf)
{
	WORD temp;
	temp = (*buf & 0xff);
	buf++;
	temp += ((*buf & 0xff) << 8);
	buf++;
	return temp;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::Flip()
{
	if(!m_Image) return E_FAIL;

	if(m_Type == IMG_PALETTED8){
		BYTE* new_image = new BYTE[m_Width*m_Height];
		if(!new_image) return E_FAIL;
		for(int row=0; row<m_Height; row++){
			for(int col=0; col<m_Width; col++){
				new_image[row*m_Width+col] = ((BYTE*)m_Image)[(m_Height-row-1)*m_Width+col];
			}
		}
		delete [] (BYTE*)m_Image;
		m_Image = new_image;
		new_image = NULL;
		return S_OK;
	}
	else{
		RGBPixel* new_image = new RGBPixel[m_Width*m_Height];
		if(!new_image) return E_FAIL;
		for(int row=0; row<m_Height; row++){
			for(int col=0; col<m_Width; col++){
				new_image[row*m_Width+col].red = ((RGBPixel*)m_Image)[(m_Height-row-1)*m_Width+col].red;
				new_image[row*m_Width+col].green = ((RGBPixel*)m_Image)[(m_Height-row-1)*m_Width+col].green;
				new_image[row*m_Width+col].blue = ((RGBPixel*)m_Image)[(m_Height-row-1)*m_Width+col].blue;
				new_image[row*m_Width+col].alpha = ((RGBPixel*)m_Image)[(m_Height-row-1)*m_Width+col].alpha;
			}
		}
		delete [] (RGBPixel*)m_Image;
		m_Image = new_image;
		new_image = NULL;
		return S_OK;
	}

	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBImage::LoadPNGFile(char * Filename, BYTE* Buffer, DWORD Size)
{
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		Game->LOG(0, "Error initializing libpng.");
		return E_FAIL;
    }

	png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr){
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		Game->LOG(0, "Error initializing libpng.");
		return E_FAIL;
	}

	PNGBufferInfo BufferInfo;
	BufferInfo.Pointer = Buffer;
	BufferInfo.Position = 0;

	png_set_read_fn(png_ptr, (void*)&BufferInfo, CBImage::PNG_ReadDataBuffer);
	png_read_info(png_ptr, info_ptr);
	
	//png_get_IHDR(png_ptr, info_ptr, (DWORD*)Width, (DWORD*)Height, BPP, NULL, NULL, NULL, NULL);
	m_Width  = png_get_image_width(png_ptr, info_ptr);
	m_Height = png_get_image_height(png_ptr, info_ptr);
	int BitDepth = png_get_bit_depth(png_ptr, info_ptr);
	int ColorType = png_get_color_type(png_ptr, info_ptr);

	// set transforms
	png_set_strip_16(png_ptr);
	png_set_expand(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

	// update
	png_read_update_info(png_ptr, info_ptr);


	png_bytep* row_pointers = new png_bytep[m_Height];

	m_Type = IMG_TRUECOLOR;
	m_Image = new RGBPixel[m_Width*m_Height];

	for (int row = 0; row < m_Height; row++){
		row_pointers[row] = (BYTE*)m_Image + row * (m_Width * sizeof(RGBPixel));
	}

	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, info_ptr);


	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	delete [] row_pointers;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::GetImageInfo(char *Filename, int *Width, int *Height, int *BPP)
{
	HRESULT ret;
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, NULL, ext);

	if     (_stricmp(ext, ".BMP")==0 || strnicmp(Filename, "savegame:", 9)==0) ret = GetImageInfoBMP(Filename, Width, Height, BPP);
	else if(_stricmp(ext, ".TGA")==0) ret = GetImageInfoTGA(Filename, Width, Height, BPP);
	else if(_stricmp(ext, ".PNG")==0) ret = GetImageInfoPNG(Filename, Width, Height, BPP);
	else if(_stricmp(ext, ".JPG")==0) ret = GetImageInfoJPG(Filename, Width, Height, BPP);
	//else if(_stricmp(ext, ".PCX")==0) ret = GetImageInfoPCX(Filename, Width, Height, BPP);
	else {
		Game->LOG(0, "CBImage::GetImageInfo - file '%s' is an unknown image type", Filename);
		ret = E_FAIL;
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::GetImageInfoBMP(char *Filename, int *Width, int *Height, int *BPP)
{
	CBFile* File = Game->m_FileManager->OpenFile(Filename);
	if(!File) return E_FAIL;
	
	BITMAPFILEHEADER hdr;
	BITMAPINFOHEADER bmiHeader;

	File->Read(&hdr, sizeof(BITMAPFILEHEADER));
	File->Read(&bmiHeader, sizeof(BITMAPINFOHEADER));

	Game->m_FileManager->CloseFile(File);
	File = NULL;

	if(Width)  *Width  = bmiHeader.biWidth;
	if(Height) *Height = bmiHeader.biHeight;
	if(BPP)    *BPP    = bmiHeader.biBitCount;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::GetImageInfoTGA(char *Filename, int *Width, int *Height, int *BPP)
{
	CBFile* File = Game->m_FileManager->OpenFile(Filename);
	if(!File) return E_FAIL;

	_tgaHeader Header;
	File->Read(&Header, sizeof(_tgaHeader));

	Game->m_FileManager->CloseFile(File);
	File = NULL;

	if(Width)  *Width  = Header.iWidth;
	if(Height) *Height = Header.iHeight;
	if(BPP) *BPP = Header.cBPP;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::GetImageInfoPNG(char *Filename, int *Width, int *Height, int *BPP)
{
	CBFile* File = Game->m_FileManager->OpenFile(Filename);
	if(!File) return E_FAIL;

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		Game->LOG(0, "Error initializing libpng.");
		Game->m_FileManager->CloseFile(File);
		return E_FAIL;
    }

	png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr){
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		Game->LOG(0, "Error initializing libpng.");
		Game->m_FileManager->CloseFile(File);
		return E_FAIL;
	}

	png_set_read_fn(png_ptr, (void*)File, CBImage::PNG_ReadData);
	png_read_info(png_ptr, info_ptr);
	
	//png_get_IHDR(png_ptr, info_ptr, (DWORD*)Width, (DWORD*)Height, BPP, NULL, NULL, NULL, NULL);
	if(Width)  *Width  = png_get_image_width(png_ptr, info_ptr);
	if(Height) *Height = png_get_image_height(png_ptr, info_ptr);
	if(BPP)    *BPP    = png_get_bit_depth(png_ptr, info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	Game->m_FileManager->CloseFile(File);
	File = NULL;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::CreateTexture(CBSurfaceD3D *Surface, D3DCOLOR ColorKey)
{
	CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;
	Convert(IMG_TRUECOLOR);
	
	int NumRows = m_Height / rend->m_MaxTexHeight;
	int NumCols = m_Width / rend->m_MaxTexWidth;

	int LastColWidth = m_Width % rend->m_MaxTexWidth;
	int LastRowHeight = m_Height % rend->m_MaxTexHeight;

	if(LastColWidth > 0){
		NumCols++;
		LastColWidth = 32;
		while(LastColWidth < m_Width % rend->m_MaxTexWidth) LastColWidth*=2;
	}
	else LastColWidth = rend->m_MaxTexWidth;

	if(LastRowHeight > 0){
		NumRows++;
		LastRowHeight = 32;
		while(LastRowHeight < m_Height % rend->m_MaxTexHeight) LastRowHeight*=2;
	}
	else LastRowHeight = rend->m_MaxTexHeight;

	// prepare surface
	if(Surface->m_NumTiles != NumCols*NumRows){
		Surface->ReleaseTiles();
		Surface->m_Tiles = new CBSurfaceD3D* [NumCols*NumRows];
		for(int i=0; i<NumCols*NumRows; i++) Surface->m_Tiles[i] = NULL;
	}		
	Surface->m_NumTiles = NumCols*NumRows;
	Surface->m_NumCols = NumCols;
	Surface->m_NumRows = NumRows;
	Surface->m_TileWidth = rend->m_MaxTexWidth;
	Surface->m_TileHeight = rend->m_MaxTexHeight;
	Surface->m_Valid = true;

	// fill textures
	int TileWidth, TileHeight;
	for(int Row = 0; Row<NumRows; Row++){
		TileHeight = (Row==NumRows-1)?LastRowHeight:rend->m_MaxTexHeight;
		for(int Col = 0; Col<NumCols; Col++){
			TileWidth = (Col==NumCols-1)?LastColWidth:rend->m_MaxTexWidth;
			
			if(Surface->m_Tiles[NumCols*Row+Col]==NULL){
				Surface->m_Tiles[NumCols*Row+Col] = new CBSurfaceD3D(Game);
			}
			CBSurfaceD3D* Tile = Surface->m_Tiles[NumCols*Row+Col];
			Tile->m_Child = true;
			Tile->SetFilename(m_Filename);

			if(Tile->m_Texture==NULL || Tile->GetWidth()!= TileWidth || Tile->GetHeight() != TileHeight){
				RELEASE(Tile->m_Texture);
				Tile->CreateEmptyTexture(TileWidth, TileHeight);
			}

			if(Tile->m_Texture){
				SetRect(&Tile->m_Rect, Col*rend->m_MaxTexWidth, Row*rend->m_MaxTexHeight, Col*rend->m_MaxTexWidth+TileWidth, Row*rend->m_MaxTexHeight+TileHeight);
				FillTexture(Tile, Tile->m_Rect, ColorKey);
				Tile->m_Valid = true;
			}
			else Game->LOG(0, "CBImage::CreateTexture - tile at %d,%d, no texture set (%s)", Row, Col, m_Filename);
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::CreateDDSurface(CBSurfaceDD *Surface, BYTE ckred, BYTE ckgreen, BYTE ckblue)
{
	CBRenderDD* rend = (CBRenderDD*)Game->m_Renderer;
	Convert(IMG_TRUECOLOR);
	
	int NumRows = 1;
	int NumCols = m_Width / rend->m_Width;

	int LastColWidth = m_Width % rend->m_Width;
	int LastRowHeight = 0;

	if(LastColWidth > 0){
		NumCols++;
	}
	else LastColWidth = rend->m_Width;

	if(LastRowHeight > 0){
		NumRows++;
	}
	else LastRowHeight = m_Height;

	// prepare surface
	if(Surface->m_NumTiles != NumCols*NumRows){
		Surface->ReleaseTiles();
		Surface->m_Tiles = new CBSurfaceDD* [NumCols*NumRows];
		for(int i=0; i<NumCols*NumRows; i++) Surface->m_Tiles[i] = NULL;
	}		
	Surface->m_NumTiles = NumCols*NumRows;
	Surface->m_NumCols = NumCols;
	Surface->m_NumRows = NumRows;
	Surface->m_TileWidth = rend->m_Width;
	Surface->m_TileHeight = m_Height;
	Surface->m_Valid = true;

	// fill textures
	int TileWidth, TileHeight;
	for(int Row = 0; Row<NumRows; Row++){
		TileHeight = m_Height;
		for(int Col = 0; Col<NumCols; Col++){
			TileWidth = (Col==NumCols-1)?LastColWidth:rend->m_Width;
			
			if(Surface->m_Tiles[NumCols*Row+Col]==NULL){
				Surface->m_Tiles[NumCols*Row+Col] = new CBSurfaceDD(Game);
			}
			CBSurfaceDD* Tile = Surface->m_Tiles[NumCols*Row+Col];
			Tile->m_Child = true;
			Tile->SetFilename(m_Filename);
			

			if(Tile->m_Surface==NULL || Tile->GetWidth()!= TileWidth || Tile->GetHeight() != TileHeight){
				RELEASE(Tile->m_Surface);
				Tile->CreateEmptySurface(TileWidth, TileHeight);
			}

			if(Tile->m_Surface){
				SetRect(&Tile->m_Rect, Col*rend->m_Width, 0, Col*rend->m_Width+TileWidth, m_Height);
				FillTexture(Tile, Tile->m_Rect, DRGBA(ckred, ckgreen, ckblue, 255), true);
				Tile->m_Valid = true;
			}
			else Game->LOG(0, "CBImage::CreateDDSurface - tile at %d,%d, no surface set (%s)", Row, Col, m_Filename);
		}
	}
	Surface->m_RestoreNeeded = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::FillTexture(CBSurface* Texture, RECT Rect, D3DCOLOR ColorKey, bool IgnoreColorKey)
{
	RGBPixel* Pixels = (RGBPixel*)m_Image;

	HRESULT res = Texture->StartPixelOp();
	if(FAILED(res)) return res;

	for(int Row = Rect.top, Y=0; Row<Rect.bottom; Row++, Y++){
		for(int Col = Rect.left, X=0; Col<Rect.right; Col++, X++){
			RGBPixel Pixel;
			if(Row<m_Height && Col<m_Width){
				Pixel = Pixels[Row*m_Width+Col];
				
				if(ColorKey == DRGBA(Pixel.red, Pixel.green, Pixel.blue, 0xFF) && !IgnoreColorKey)
					Pixel.alpha = Pixel.red = Pixel.green = Pixel.blue = 0x00;
			}
			else{
				Pixel.alpha = Pixel.red = Pixel.green = Pixel.blue = 0x00;
			}

			if(!Game->m_UseD3D && Pixel.alpha < 128){
				Pixel.red   = GetRValue(ColorKey);
				Pixel.green = GetGValue(ColorKey);
				Pixel.blue  = GetBValue(ColorKey);
			}

			Texture->PutPixel(X, Y, Pixel.red, Pixel.green, Pixel.blue, Pixel.alpha);
		}
	}

	Texture->EndPixelOp();
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CBImage::PNG_ReadData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	CBFile* File = (CBFile*)png_get_io_ptr(png_ptr);
	if(File) File->Read(data, length);
}


//////////////////////////////////////////////////////////////////////////
void CBImage::PNG_ReadDataBuffer(png_structp png_ptr, png_bytep data, png_size_t length)
{
	PNGBufferInfo* BufferInfo = (PNGBufferInfo*)png_get_io_ptr(png_ptr);
	if(BufferInfo){
		memcpy(data, BufferInfo->Pointer + BufferInfo->Position, length);
		BufferInfo->Position += length;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::LoadFromDDSurface(LPDIRECTDRAWSURFACE7 DDS)
{
	FreeAll();

	HRESULT res = S_OK;
	BYTE* lpvBits = NULL;
	HDC SurfDC=NULL;
	HBITMAP OffscreenBmp = NULL;
	HBITMAP OldBmp = NULL;
	LPBITMAPINFO lpbi = NULL;
	HDC OffscreenDC = NULL;
	int i,j, pitch;

	// get description
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));

    ddsd.dwSize = sizeof(ddsd);
    res = DDS->GetSurfaceDesc(&ddsd);
	if(FAILED(res)) goto finish;

    m_Width = ddsd.dwWidth;
    m_Height = ddsd.dwHeight;

	// get device context	
	res = DDS->GetDC(&SurfDC);
	if(FAILED(res)) goto finish;

	// bitmap
	OffscreenBmp = CreateCompatibleBitmap(SurfDC, m_Width, m_Height);
	if(OffscreenBmp==NULL) goto finish;

	// offscreen DC
	OffscreenDC = CreateCompatibleDC(SurfDC);
	if(OffscreenDC==NULL) goto finish;

	// make a blit and store original bitmap
	OldBmp = (HBITMAP)SelectObject(OffscreenDC, OffscreenBmp);
	BitBlt(OffscreenDC, 0, 0, m_Width, m_Height, SurfDC, 0, 0, SRCCOPY);

	// release DD device context
	DDS->ReleaseDC(SurfDC);
	SurfDC = NULL;

	// prepare bitmap info
	lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)]);
	if(lpbi==NULL) goto finish;

	memset(&lpbi->bmiHeader, 0, sizeof(BITMAPINFOHEADER));
    lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbi->bmiHeader.biWidth = m_Width;
	lpbi->bmiHeader.biHeight = -m_Height;
	lpbi->bmiHeader.biPlanes = 1;
	lpbi->bmiHeader.biBitCount = 24;
	lpbi->bmiHeader.biCompression = BI_RGB;

	// Get info but first de-select OffscrBmp because GetDIBits requires it:
    SelectObject(OffscreenDC, OldBmp);
    
	if(!GetDIBits(OffscreenDC, OffscreenBmp, 0, m_Height, NULL, lpbi, DIB_RGB_COLORS))
        goto finish;
	
    if((lpvBits = new BYTE[lpbi->bmiHeader.biSizeImage]) == NULL)
        goto finish;

    // Have GetDIBits convert OffscrBmp to a DIB (device-independent bitmap):
    if(!GetDIBits(OffscreenDC, OffscreenBmp, 0, m_Height, lpvBits, lpbi, DIB_RGB_COLORS))
		goto finish;

	m_Image = new RGBPixel[m_Width*m_Height];
	if(!m_Image) goto finish;

	m_Type = IMG_TRUECOLOR;

	pitch = m_Width * 3;
	if(pitch%4 != 0) pitch += 4 - pitch%4;

	memset(m_Image, 0, m_Width*m_Height*sizeof(RGBPixel));
	for(j=0; j<m_Height; j++){
		for(i=0; i<m_Width; i++){
			((RGBPixel*)m_Image)[j*m_Width+i].blue = lpvBits[j*pitch + (i*3)];
			((RGBPixel*)m_Image)[j*m_Width+i].green = lpvBits[j*pitch + (i*3) + 1];
			((RGBPixel*)m_Image)[j*m_Width+i].red = lpvBits[j*pitch + (i*3) + 2];
		}
	}

	res = S_OK;

finish:
	if (SurfDC) DDS->ReleaseDC(SurfDC);
    if (OffscreenDC) DeleteDC(OffscreenDC);
    if (OffscreenBmp) DeleteObject(OffscreenBmp);
    if (lpbi) delete[] lpbi;
    if (lpvBits) delete[] lpvBits;

	return res;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::LoadFromD3DDevice(LPDIRECT3DDEVICE Device, int Width, int Height)
{
	FreeAll();

	LPDIRECT3DSURFACE lpSurface = NULL;    // copy of the front buffer


    D3DDEVICE_CREATION_PARAMETERS dcp;
    dcp.AdapterOrdinal = D3DADAPTER_DEFAULT;
    Device->GetCreationParameters(&dcp);

    D3DDISPLAYMODE dm;
    dm.Width = dm.Height = 0;

    // retrieve pointer to IDirect3D8 interface,
    // which provides the GetAdapterDisplayMode method
	LPDIRECT3D lpD3D;
	Device->GetDirect3D(&lpD3D);
    if (lpD3D){
        // query the screen dimensions of the current adapter
        lpD3D->GetAdapterDisplayMode(dcp.AdapterOrdinal, &dm);
		RELEASE(lpD3D);
	}

	if(dm.Width==0 || dm.Height==0) return E_FAIL;

	RECT rc;
	GetClientRect(dcp.hFocusWindow, &rc);
	ClientToScreen(dcp.hFocusWindow, LPPOINT(&rc.left));
	ClientToScreen(dcp.hFocusWindow, LPPOINT(&rc.right));

	// clip to active monitor
	CBPlatform::IntersectRect(&rc, &rc, &Game->m_Renderer->m_MonitorRect);

	/*
	rc.left = max(rc.left, 0);
	rc.top = max(rc.top, 0);

	rc.right = min(rc.right, dm.Width);
	rc.bottom = min(rc.bottom, dm.Height);
	*/


	// initialize image
	m_Image = new RGBPixel[dm.Width*dm.Height];
	if(!m_Image) return E_FAIL;

	m_Width = min(Width, rc.right-rc.left);
	m_Height = min(Height, rc.bottom-rc.top);
	m_Type = IMG_TRUECOLOR;


    // create a 32-bit ARGB system-memory surface that is going to receive
    // a copy of the front buffer's contents
	if (FAILED(C3DUtils::CreateImageSurface(Device, dm.Width, dm.Height, D3DFMT_A8R8G8B8, &lpSurface)))	
		return E_FAIL;

	HRESULT res;
	if(FAILED(res = C3DUtils::GetFrontBuffer(Device, lpSurface)))
	{
		RELEASE(lpSurface);
		return E_FAIL;
    }

	// lock the surface for reading
	D3DLOCKED_RECT LockedRect;
	if (FAILED(lpSurface->LockRect(&LockedRect, NULL, D3DLOCK_READONLY)))
	{
		RELEASE(lpSurface);
		return E_FAIL;
    }


	BYTE* Bits = (BYTE*)LockedRect.pBits;
    // read pixels
    for(int j=0; j<m_Height; j++)
	{
        for(int i=0; i<m_Width; i++)
		{
			int iii = i;
			int jjj = j;

			if(Game->m_Renderer->m_Windowed)
			{
				iii += rc.left - Game->m_Renderer->m_MonitorRect.left;
				jjj += rc.top - Game->m_Renderer->m_MonitorRect.top;
			}

			((RGBPixel*)m_Image)[m_Width*j+i].red   = Bits[LockedRect.Pitch*jjj+iii*4+2];
			((RGBPixel*)m_Image)[m_Width*j+i].green = Bits[LockedRect.Pitch*jjj+iii*4+1];
			((RGBPixel*)m_Image)[m_Width*j+i].blue  = Bits[LockedRect.Pitch*jjj+iii*4+0];
        }
    }

	lpSurface->UnlockRect();
	RELEASE(lpSurface);



	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::SaveBMPFile(char *Filename)
{
	
	DWORD Size;
	BYTE* Buffer = CreateBMPBuffer(&Size);
	if(!Buffer) return E_FAIL;

	// open file
	FILE* f = fopen(Filename, "wb");
	if(!f){
		delete [] Buffer;
		return E_FAIL;
	}
	fwrite(Buffer, Size, 1, f);
	fclose(f);

	delete [] Buffer;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::Resize(int NewWidth, int NewHeight)
{
	if(!m_Image || NewWidth<=0 || NewHeight<=0) return E_FAIL;
	Convert(IMG_TRUECOLOR);

	C2PassScale <CBilinearFilter> ScaleEngine;
	RGBPixel* NewImage = ScaleEngine.AllocAndScale(Game, (RGBPixel*)m_Image, m_Width, m_Height, NewWidth, NewHeight);
	if(!NewImage) return E_FAIL;

	delete [] m_Image;
	m_Image = NewImage;

	m_Width = NewWidth;
	m_Height = NewHeight;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
BYTE* CBImage::CreateBMPBuffer(DWORD* BufferSize)
{
	// convert to true-color
	if(!m_Image || FAILED(Convert(IMG_TRUECOLOR))) return NULL;

	int pitch = m_Width * 3;
	if(pitch%4 != 0) pitch += 4 - pitch%4;

	DWORD Size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pitch*m_Height;

	BYTE* Buffer = new BYTE[Size];
	if(!Buffer) return NULL;

	memset(Buffer, 0, Size);


    BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*)(Buffer + sizeof(BITMAPFILEHEADER));
    bmih->biSize = sizeof(BITMAPINFOHEADER);
    bmih->biWidth = m_Width;
    bmih->biHeight = m_Height;
    bmih->biPlanes = 1;
    bmih->biBitCount = 24;
    bmih->biCompression = BI_RGB;
    bmih->biSizeImage = pitch * m_Height;
    bmih->biXPelsPerMeter = 0;
    bmih->biYPelsPerMeter = 0;
    bmih->biClrUsed = 0;
    bmih->biClrImportant = 0;


	BITMAPFILEHEADER* bmfh = (BITMAPFILEHEADER*)Buffer;
	bmfh->bfType = 0x4d42; // BM
	bmfh->bfSize = Size;
	bmfh->bfReserved1 = bmfh->bfReserved2 = 0;
	bmfh->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BYTE* Bits = Buffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	for(int j=m_Height-1; j>=0; j--){
		for(int i=0; i<m_Width; i++){
			Bits[j*pitch + i*3 + 0] = ((RGBPixel*)m_Image)[(m_Height-j-1)*m_Width+i].blue;
			Bits[j*pitch + i*3 + 1] = ((RGBPixel*)m_Image)[(m_Height-j-1)*m_Width+i].green;
			Bits[j*pitch + i*3 + 2] = ((RGBPixel*)m_Image)[(m_Height-j-1)*m_Width+i].red;
		}
	}

	if(BufferSize) *BufferSize = Size;

	return Buffer;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::GetImageInfoJPG(char *Filename, int *Width, int *Height, int *BPP)
{
	DWORD Size;
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename, &Size);
	if(!Buffer) return E_FAIL;

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
		
	jpeg_dcgf_src(&cinfo, Buffer, Size);

	jpeg_read_header(&cinfo, TRUE);

	*Width = cinfo.image_width;
	*Height = cinfo.image_height;

	
	jpeg_destroy_decompress(&cinfo);
	jpeg_abort_decompress(&cinfo);
	
	delete [] Buffer;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBImage::LoadJPGFile(char * Filename, BYTE* Buffer, DWORD Size)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
		
	jpeg_dcgf_src(&cinfo, Buffer, Size);

	jpeg_read_header(&cinfo, TRUE);

	// force always RGB
	cinfo.out_color_components = 3;
	cinfo.out_color_space = JCS_RGB;

	jpeg_start_decompress(&cinfo);


	m_Width = cinfo.output_width;
	m_Height = cinfo.output_height;
	
	m_Type = IMG_TRUECOLOR;
	m_Image = new RGBPixel[m_Width*m_Height];

	JSAMPARRAY LineBuff = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, 1);
	
	int Line = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, LineBuff, 1);

		BYTE* Base = LineBuff[0];
		for(int i=0; i<m_Width; i++){
			((RGBPixel*)m_Image)[Line*m_Width+i].red   = Base[3*i+0];
			((RGBPixel*)m_Image)[Line*m_Width+i].green = Base[3*i+1];
			((RGBPixel*)m_Image)[Line*m_Width+i].blue  = Base[3*i+2];
			((RGBPixel*)m_Image)[Line*m_Width+i].alpha = 255;
		}
		Line++;
	}


	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	jpeg_abort_decompress(&cinfo);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::Grayscale()
{
	if(!m_Image) return E_FAIL;
	Convert(IMG_TRUECOLOR);

	for(int j=0; j<m_Height; j++){
		for(int i=0; i<m_Width; i++){
			RGBPixel P = ((RGBPixel*)m_Image)[m_Width*j+i];

			BYTE Gray = 0.30*(float)P.red + 0.59*(float)P.green + 0.11*(float)P.blue;
			((RGBPixel*)m_Image)[m_Width*j+i].red = Gray;
			((RGBPixel*)m_Image)[m_Width*j+i].green = Gray;
			((RGBPixel*)m_Image)[m_Width*j+i].blue = Gray;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CBSurface* CBImage::CreateSurface(BYTE ckred, BYTE ckgreen, BYTE ckblue)
{
	// Direct3D
	if(Game->m_UseD3D)
	{
		CBSurfaceD3D* Surf = new CBSurfaceD3D(Game);
		if(Surf && SUCCEEDED(CreateTexture(Surf, DRGBA(ckred, ckgreen, ckblue, 255))))
		{
			Surf->SetSize(m_Width, m_Height);
			return Surf;
		}
		else
		{
			SAFE_DELETE(Surf);
			return NULL;
		}
	}
	// DirectDraw
	else
	{
		CBSurfaceDD* Surf = new CBSurfaceDD(Game);
		if(Surf && SUCCEEDED(CreateDDSurface(Surf, ckred, ckgreen, ckblue)))
		{
			Surf->SetSize(m_Width, m_Height);
			return Surf;
		}
		else
		{
			SAFE_DELETE(Surf);
			return NULL;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::CreateFromRaw(DWORD* Bits, int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;

	m_Type = IMG_TRUECOLOR;
	m_Image = new RGBPixel[m_Width*m_Height];

	for(int y=0; y<Height; y++)
	{
		for(int x=0; x<Width; x++)
		{
			// 0xaarrggbb
			DWORD Pixel = Bits[y*m_Width+x];
			
			/*		
			((RGBPixel*)m_Image)[y*m_Width+x].red   = (BYTE)((Pixel>>16) & 0xFF);
			((RGBPixel*)m_Image)[y*m_Width+x].green = (BYTE)((Pixel>>8)  & 0xFF);
			((RGBPixel*)m_Image)[y*m_Width+x].blue  = (BYTE)((Pixel>>0)  & 0xFF);
			((RGBPixel*)m_Image)[y*m_Width+x].alpha = (BYTE)((Pixel>>24) & 0xFF);
			*/
			
			((RGBPixel*)m_Image)[y*m_Width+x].alpha = (BYTE)((Pixel>>16) & 0xFF);
			((RGBPixel*)m_Image)[y*m_Width+x].red   = 255;
			((RGBPixel*)m_Image)[y*m_Width+x].green = 255;
			((RGBPixel*)m_Image)[y*m_Width+x].blue  = 255;
		}
	}
	//SaveBMPFile("c:\\test.bmp");

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
BYTE CBImage::GetAlphaAt(int X, int Y)
{
	if(m_Type!=IMG_TRUECOLOR || X < 0 || Y < 0 || X >= m_Width || Y >= m_Height) return 0xFF;
	else return ((RGBPixel*)m_Image)[Y*m_Width+X].alpha;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBImage::CopyFrom(CBImage* OrigImage, int NewWidth, int NewHeight)
{
	if(OrigImage->m_Type != IMG_TRUECOLOR) return E_FAIL;

	if(NewWidth <= 0) NewWidth = OrigImage->m_Width;
	if(NewHeight <= 0) NewHeight = OrigImage->m_Height;

	m_Type = OrigImage->m_Type;

	if(NewWidth == OrigImage->m_Width && NewHeight == OrigImage->m_Height)
	{
		m_Image = (BYTE*)new RGBPixel [m_Width*m_Height];
		memcpy(m_Image, OrigImage->m_Image, m_Width*m_Height*sizeof(RGBPixel));
	}
	else
	{
		C2PassScale <CBilinearFilter> ScaleEngine;
		m_Image = ScaleEngine.AllocAndScale(Game, (RGBPixel*)OrigImage->m_Image, OrigImage->m_Width, OrigImage->m_Height, NewWidth, NewHeight);
	}

	m_Width = NewWidth;
	m_Height = NewHeight;

	return m_Image?S_OK:E_FAIL;
}
