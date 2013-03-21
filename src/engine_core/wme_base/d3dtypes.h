// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#ifndef __D3DTYPES_H_
#define __D3DTYPES_H_

#ifdef WME_D3D9
	#ifdef _DEBUG
		#define D3D_DEBUG_INFO
	#endif

	#include <d3d9.h>
	#include <d3dx9.h>
	#include <D3DX9Xof.h>
#else
	#include <d3d8.h>
	#include <d3dx8.h>
#endif


#ifdef WME_D3D9
	
	typedef struct IDirect3D9* LPDIRECT3D;
	typedef struct IDirect3DTexture9* LPDIRECT3DTEXTURE;
	typedef struct IDirect3DVertexBuffer9* LPDIRECT3DVERTEXBUFFER;
	typedef struct IDirect3DIndexBuffer9* LPDIRECT3DINDEXBUFFER;
	typedef struct IDirect3DDevice9* LPDIRECT3DDEVICE;
	typedef struct IDirect3DSurface9* LPDIRECT3DSURFACE;
	
	typedef struct _D3DMATERIAL9 D3DMATERIAL;
	typedef struct _D3DVIEWPORT9 D3DVIEWPORT;
	typedef struct _D3DLIGHT9 D3DLIGHT;
	typedef struct _D3DCAPS9 D3DCAPS;	
	typedef struct _D3DADAPTER_IDENTIFIER9 D3DADAPTER_IDENTIFIER;

	typedef struct ID3DXFile* LPDIRECTXFILE;
	typedef struct ID3DXFile* LPDIRECTXFILEOBJECT;
	typedef struct ID3DXFileData* LPDIRECTXFILEDATA;
	typedef struct ID3DXFileEnumObject* LPDIRECTXFILEENUMOBJECT;
	typedef struct _D3DXF_FILELOADMEMORY DXFILELOADMEMORY;
	#define DXFILELOAD_FROMMEMORY D3DXF_FILELOAD_FROMMEMORY

	#define LockVB(OffsetToLock, SizeToLock, ppbData, Flags) Lock(OffsetToLock, SizeToLock, (void**)ppbData, Flags)
	#define LockIB(OffsetToLock, SizeToLock, ppbData, Flags) Lock(OffsetToLock, SizeToLock, (void**)ppbData, Flags)
	#define CreateVertexBufferUni(Length, Usage, FVF, Pool, ppVertexBuffer) CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, NULL)
	#define CreateIndexBufferUni(Length, Usage, Format, Pool, ppIndexBuffer) CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, NULL)
	#define LockVertexBufferUni(Flags, ppData) LockVertexBuffer(Flags, (void**)ppData)
	#define LockIndexBufferUni(Flags, ppData) LockIndexBuffer(Flags, (void**)ppData)


	// the following used to be TEXTURESTAGESTATES but are now SAMPLERSTATES
	enum
	{
		D3DTSS_ADDRESSU       = 13,
		D3DTSS_ADDRESSV       = 14,
		D3DTSS_BORDERCOLOR    = 15,
		D3DTSS_MAGFILTER      = 16,
		D3DTSS_MINFILTER      = 17,
		D3DTSS_MIPFILTER      = 18,
		D3DTSS_MIPMAPLODBIAS  = 19,
		D3DTSS_MAXMIPLEVEL    = 20,
		D3DTSS_MAXANISOTROPY  = 21
	};

	#define D3DENUM_NO_WHQL_LEVEL 0
#else
	typedef struct IDirect3D8* LPDIRECT3D;
	typedef struct IDirect3DTexture8* LPDIRECT3DTEXTURE;
	typedef struct IDirect3DVertexBuffer8* LPDIRECT3DVERTEXBUFFER;
	typedef struct IDirect3DIndexBuffer8* LPDIRECT3DINDEXBUFFER;
	typedef struct IDirect3DDevice8* LPDIRECT3DDEVICE;
	typedef struct IDirect3DSurface8* LPDIRECT3DSURFACE;

	typedef struct _D3DMATERIAL8 D3DMATERIAL;
	typedef struct _D3DVIEWPORT8 D3DVIEWPORT;
	typedef struct _D3DLIGHT8 D3DLIGHT;
	typedef struct _D3DCAPS8 D3DCAPS;
	typedef struct _D3DADAPTER_IDENTIFIER8 D3DADAPTER_IDENTIFIER;

	#define D3DXMatrixDeterminant D3DXMatrixfDeterminant
	#define LockVB(OffsetToLock, SizeToLock, ppbData, Flags) Lock(OffsetToLock, SizeToLock, (BYTE**)ppbData, Flags)
	#define LockIB(OffsetToLock, SizeToLock, ppbData, Flags) Lock(OffsetToLock, SizeToLock, (BYTE**)ppbData, Flags)
	#define CreateVertexBufferUni(Length, Usage, FVF, Pool, ppVertexBuffer) CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer)
	#define CreateIndexBufferUni(Length, Usage, Format, Pool, ppIndexBuffer) CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer)
	#define LockVertexBufferUni(Flags, ppData) LockVertexBuffer(Flags, (BYTE**)ppData)
	#define LockIndexBufferUni(Flags, ppData) LockIndexBuffer(Flags, (BYTE**)ppData)

#endif



#endif // __D3DTYPES_H_