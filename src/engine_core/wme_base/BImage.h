// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BIMAGE_H__46BF5840_1837_11D4_9F37_9067C7F29A3D__INCLUDED_)
#define AFX_BIMAGE_H__46BF5840_1837_11D4_9F37_9067C7F29A3D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <png.h>
extern "C"{
#include <jpeglib.h>
}
#include "rgbpixel.h"


#define	TGA_NO_IMAGE	0		// No image data included in file
#define	TGA_UNCOMP_CM	1		// Uncompressed, Color-Mapped (VDA/D 
								// and Targa M-8 images)             
#define	TGA_UNCOMP_RGB	2		// Uncompressed, RGB images (eg: ICB 
								// Targa 16, 24 and 32)              
#define	TGA_UNCOMP_BW	3		// Uncompressed, B/W images (eg:     
								// Targa 8 and Targa M-8 images)     
#define	TGA_RL_CM		9		// Run-length, Color-Mapped (VDA/D   
								// and Targa M-8 images)             
#define	TGA_RL_RGB		10		// Run-length, RGB images (eg: ICB   
								// Targa 16, 24 and 32)              
#define	TGA_RL_BW		11		// Run-length, B/W images (eg: Targa 
								// 8 and Targa M-8)                  
#define	TGA_COMP_CM		32		// Compressed Color-Mapped (VDA/D)   
								// data using Huffman, Delta, and    
								// run length encoding               
#define	TGA_COMP_CM_4	33		// Compressed Color-Mapped (VDA/D)   
								// data using Huffman, Delta, and    
								// run length encoding in 4 passes   

#pragma pack(1)

struct _tgaHeader 
{
	unsigned char     cIDLength;
	unsigned char     cColorMapType;
	unsigned char     cType;
	unsigned short    iFirstColourMapEntry;
	unsigned short    iColourMapLength;
	unsigned char     cColourMapEntrySize;
	unsigned short    iImageXOrigin;
	unsigned short    iImageYOrigin;
	unsigned short    iWidth;
	unsigned short    iHeight;
	unsigned char     cBPP;
	unsigned char     cDescriptorBits;
};

struct _tgaFooter 
{
    unsigned long   extarea_offset;
    unsigned long   devdir_offset;
    unsigned char   signature[16];
    unsigned char   period;         // always be "."
    unsigned char   null;           // (0x00)
};

struct _tgaStruct 
{
	_tgaHeader    header;
	char          *ID;
	_tgaFooter    footer;
	unsigned char *color_map;
};


struct PNGBufferInfo
{
	BYTE* Pointer;
	int Position;
};

#pragma pack()

void jpeg_dcgf_src (j_decompress_ptr cinfo, BYTE* Buffer, DWORD Size);

class CBImage:CBBase
{

public:
	HRESULT Grayscale();
	BYTE* CreateBMPBuffer(DWORD* BufferSize = NULL);
	HRESULT Resize(int NewWidth, int NewHeight);
	HRESULT SaveBMPFile(char* Filename);
	HRESULT LoadFromDDSurface(LPDIRECTDRAWSURFACE7 DDS);
	HRESULT LoadFromD3DDevice(LPDIRECT3DDEVICE Device, int Width, int Height);
	HRESULT CreateDDSurface(CBSurfaceDD *Surface, BYTE ckred, BYTE ckgreen, BYTE ckblue);
	HRESULT CreateTexture(CBSurfaceD3D* Surface, D3DCOLOR ColorKey);
	HRESULT GetImageInfo(char* Filename, int* Width=NULL, int* Height=NULL, int* BPP=NULL);
	CBSurface* CreateSurface(BYTE ckred=255, BYTE ckgreen=0, BYTE ckblue=255);
	HRESULT Flip();
	LPDIRECTDRAWPALETTE CreatePalette();
	LPDIRECTDRAWSURFACE7 CreateSurface(int* Width, int* Height, bool ForceSysMem=false, BYTE Red=255, BYTE Green=0, BYTE Blue=255);
	HRESULT Convert(TImageType Type);
	void FreeAll();
	HRESULT LoadFile(char* Filename);
	char* m_Filename;
	RGBPixel* m_Palette;
	BYTE* m_AlphaMap;
	void* m_Image;
	int m_Height;
	int m_Width;
	CBImage(CBGame* inGame=NULL);
	virtual ~CBImage();
	TImageType m_Type;
	static void _cdecl PNG_ReadData(png_structp png_ptr, png_bytep data, png_size_t length);
	static void _cdecl PNG_ReadDataBuffer(png_structp png_ptr, png_bytep data, png_size_t length);
	HRESULT CreateFromRaw(DWORD* Bits, int Width, int Height);
	BYTE GetAlphaAt(int X, int Y);
	HRESULT CopyFrom(CBImage* OrigImage, int NewWidth = 0, int NewHeight = 0);
private:
	WORD TGAGetWord(BYTE* &buf);

	HRESULT GetImageInfoBMP(char* Filename, int* Width=NULL, int* Height=NULL, int* BPP=NULL);
	HRESULT GetImageInfoTGA(char* Filename, int* Width=NULL, int* Height=NULL, int* BPP=NULL);
	HRESULT GetImageInfoPNG(char* Filename, int* Width=NULL, int* Height=NULL, int* BPP=NULL);
	HRESULT GetImageInfoJPG(char* Filename, int* Width=NULL, int* Height=NULL, int* BPP=NULL);
	HRESULT LoadBMPFile(char* Filename, BYTE* Buffer, DWORD Size);
	HRESULT LoadTGAFile(char* Filename, BYTE* Buffer, DWORD Size);
	HRESULT LoadPNGFile(char* Filename, BYTE* Buffer, DWORD Size);
	HRESULT LoadJPGFile(char* Filename, BYTE* Buffer, DWORD Size);
	HRESULT FillTexture(CBSurface* Texture, RECT Rect, D3DCOLOR ColorKey, bool IgnoreColorKey=false);
};

#endif // !defined(AFX_BIMAGE_H__46BF5840_1837_11D4_9F37_9067C7F29A3D__INCLUDED_)
