// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#ifndef _DCGF_H_
#define _DCGF_H_


//////////////////////////////////////////////////////////////////////////
#define DCGF_VER_MAJOR 1
#define DCGF_VER_MINOR 10
#define DCGF_VER_BUILD 2
#define DCGF_VER_SUFFIX "beta"
#define DCGF_VER_BETA true

#define DCGF_NAME "Wintermute Engine"
#define DCGF_MAGIC 0xDEC0ADDE

// minimal saved game version we support
#define SAVEGAME_VER_MAJOR 1
#define SAVEGAME_VER_MINOR 10
#define SAVEGAME_VER_BUILD 1
//////////////////////////////////////////////////////////////////////////

#define COMPRESSED_FILE_MAGIC 0x504D435A // ZCMP

#pragma warning (disable: 4786) // symbols longer than 255 chars (STL)
#pragma warning (disable: 4355) // this in base member initialization
#pragma warning (disable: 4018) // signed/unsigned match
#pragma warning (disable: 4244) // conversion from float to int
#pragma warning (disable: 4996) // CRT deprecate
#pragma warning (disable: 4799) // No EMMS at end of function ...
#pragma warning (disable: 4701) // No EMMS at end of function ...




#ifdef _WIN32_WINCE
	#pragma warning (disable: 4267)
	#pragma warning (disable: 4311)
	#pragma warning (disable: 4312)
#endif



#define NUM_SKEL_ANIM_CHANNELS 10 // number of channels for skeletal animations


#pragma comment(lib, "dxguid.lib")

#ifdef WME_D3D9
	#pragma comment(lib, "d3dx9.lib")
#else
	#pragma comment(lib, "d3dx8.lib")
#endif

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "ws2_32.lib")



#include "PlatformWin.h"

#include <stdio.h>

#include "d3dtypes.h"
#include <ddraw.h>
#include <rmxfguid.h>
#include <dsound.h>
#include <Dxerr.h>
#include <assert.h>
//#include <tinyxml.h>


#ifndef WM_MOUSEWHEEL
	#include <zmouse.h>
#endif

#ifdef GetClassName
	#undef GetClassName
#endif

#define WM_WINSOCK WM_USER+10

#include <string>

using namespace std;


#include "resource.h"
#include "dctypes.h"
#include "dcpackage.h"
#include "utils.h"
#include "BTextUtils.h"

#include "coll_templ.h"

class CBPersistMgr;

// persistence support
typedef void* (WINAPI *PERSISTBUILD)(void);
typedef HRESULT (WINAPI *PERSISTLOAD)(void*, CBPersistMgr*);

#define DECLARE_PERSISTENT(class_name, parent_class)\
	static char m_ClassName[];\
	static void* WINAPI PersistBuild(void);\
	virtual const char* GetClassName();\
	static HRESULT WINAPI PersistLoad(void* Instance, CBPersistMgr* PersistMgr);\
	class_name(TDynamicConstructor p1, TDynamicConstructor p2):parent_class(p1, p2){ /*memset(this, 0, sizeof(class_name));*/ };\
	virtual HRESULT Persist(CBPersistMgr* PersistMgr);\
	void* operator new (size_t size);\
	void operator delete(void* p);\


#define IMPLEMENT_PERSISTENT(class_name, persistent_class)\
	char class_name::m_ClassName[] = #class_name;\
	void* class_name::PersistBuild(){\
		return ::new class_name(DYNAMIC_CONSTRUCTOR, DYNAMIC_CONSTRUCTOR);\
	}\
	\
	HRESULT class_name::PersistLoad(void* Instance, CBPersistMgr* PersistMgr){\
		return ((class_name*)Instance)->Persist(PersistMgr);\
	}\
	\
	const char* class_name::GetClassName(){\
		return #class_name;\
	}\
	\
	CSysClass Register##class_name(class_name::m_ClassName, class_name::PersistBuild, class_name::PersistLoad, persistent_class);\
	\
	void* class_name::operator new (size_t size){\
		void* ret = ::operator new(size);\
		CSysClassRegistry::GetInstance()->RegisterInstance(#class_name, ret);\
		return ret;\
	}\
	\
	void class_name::operator delete (void* p){\
		CSysClassRegistry::GetInstance()->UnregisterInstance(#class_name, p);\
		::operator delete(p);\
	}\

#define TMEMBER(member_name) #member_name, &member_name
#define TMEMBER_INT(member_name) #member_name, (int*)&member_name

typedef void (*SYS_INSTANCE_CALLBACK)(void* Instance, void* Data);


// classes/instances registry
#include "SysInstance.h"
#include "SysClass.h"
#include "SysClassRegistry.h"

// hardware enumerator
#include "HWVideoMode.h"
#include "HWDevice.h"
#include "HWDeviceAudio.h"
#include "HWDeviceVideo.h"
#include "HWManager.h"

// base classes
#include "BBase.h"
#include "BNamedObject.h"
#include "BParser.h"
#include "BDynBuffer.h"
#include "BStringTable.h"
#include "BRegistry.h"
#include "BPersistMgr.h"
#include "BScriptable.h"
#include "BScriptHolder.h"
#include "BFileEntry.h"
#include "BPackage.h"
#include "BFile.h"
#include "BResourceFile.h"
#include "BSaveThumbFile.h"
#include "BDiskFile.h"
#include "BPkgFile.h"
#include "BFileManager.h"
#include "BSocket.h"
#include "BNetworkMgr.h"
#include "BAccessMgr.h"
#include "BPlugin.h"
#include "BPluginMgr.h"
#include "BEvent.h"
#include "BPoint.h"
#include "BObject.h"
#include "BViewport.h"
#include "BRegion.h"
#include "BSurface.h"
#include "BSurfaceDD.h"
#include "BSurfaceD3D.h"
#include "BSurfaceStorage.h"
#include "BSoundBuffer.h"
#include "BSoundWAV.h"
#include "BSoundOGG.h"
#include "BSoundAVI.h"
#include "BSoundTheora.h"
#include "BSoundMgr.h"
#include "BSound.h"
#include "BSubFrame.h"
#include "BFrame.h"
#include "BActiveRect.h"
#include "BSprite.h"
#include "BFontStorage.h"
#include "BFont.h"
#include "BFontBitmap.h"
#include "BFontTT.h"
#include "BRenderer.h"
#include "BRenderDD.h"
#include "BRenderD3D.h"
#include "BImage.h"
#include "BEventSet.h"
#include "BQuickMsg.h"
#include "BTransitionMgr.h"
#include "BKeyboardState.h"
#include "BFader.h"
#include "SXMath.h"
#include "SXMemBuffer.h"
#include "SXFile.h"
#include "SXDirectory.h"
#include "SXTcpClient.h"
#include "BParamSet.h"
#include "PartEmitter.h"

// UI classes
#include "UITiledImage.h"
#include "UIObject.h"
#include "UIButton.h"
#include "UIEdit.h"
#include "UIText.h"
#include "UIWindow.h"
#include "UIMarkup.h"

// scripting classes
#include "dcscript.h"
#include "ScValue.h"
#include "ScStack.h"
#include "ScScript.h"
#include "ScEngine.h"

// video classes
#include "VidSubtitle.h"
#include "VidSubtitler.h"
#include "VidRenderer.h"
#include "VidRendererDD.h"
#include "VidRendererD3D.h"
#include "VidPlayer.h"
#include "VidTheoraPlayer.h"

// 3d classes
#include "3DCamera.h"
#include "3DVertex.h"
#include "3DFace.h"
#include "3DMesh.h"
#include "3DKeyframe.h"
#include "3DSkelAnim.h"
#include "3DJoint.h"
#include "3DLight.h"
#include "3DMaterial.h"
#include "3DLoader3DS.h"
#include "3DLoaderMS3D.h"
#include "3DModel.h"
#include "3DShadowGen.h"
#include "3DFrustum.h"
#include "3DShadowVolume.h"


#include "wme_plugin.h"


// flexible vertex formats
struct MODELVERTEX {
	float x,y,z;	// Coordinates
	float nx,ny,nz;	// Normal components
	float u,v;		// Texture coords
};
#define D3DFVF_MODELVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXTUREFORMAT2)


struct MODELVERTEXCOLOR {
	float x,y,z;	// Coordinates	
	float nx,ny,nz;	// Normal components
	DWORD color1, color2; // diffuse, specular
	float u,v;		// Texture coords
};
#define D3DFVF_MODELVERTEXCOLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXTUREFORMAT2)

#define D3DFVF_MODELVERTEX2 (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXTUREFORMAT2)


struct SPRITEVERTEX {
    D3DXVECTOR4 p;
    DWORD       color;
	FLOAT		tu;
	FLOAT		tv;
};
#define D3DFVF_SPRITEVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


struct LINEVERTEX {
    D3DXVECTOR4 p;
    DWORD       color;
};
#define D3DFVF_LINEVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


// shadow volume vertex format
struct SHADOWVOLVERTEX
{
	D3DXVECTOR4 p;
	D3DCOLOR    color;
};
#define D3DFVF_SHADOWVOLVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)



// big boss
#include "BGame.h"

// framework extensions
//#include "extension.h"


// macros
#define RELEASE(obj) if(obj) { obj->Release(); obj = NULL; } else 0
#define SAFE_DELETE(obj)  if(obj) { delete obj; obj = NULL; } else 0
#define SAFE_DELETE_ARRAY(obj)  if(obj) { delete [] obj; obj = NULL; } else 0
#define DRGBA(r,g,b,a) ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define DegToRad(_val) (_val*D3DX_PI*(1.0f/180.0f))
#define RadToDeg(_val) (_val*(180/D3DX_PI))

#define D3DCOLGetB(rgb)	 ((BYTE)(rgb))
#define D3DCOLGetG(rgb)  ((BYTE)(((WORD)(rgb)) >> 8))
#define D3DCOLGetR(rgb)  ((BYTE)((rgb)>>16))
#define D3DCOLGetA(rgb)  ((BYTE)((rgb)>>24))


#endif // _DCGF_H_
