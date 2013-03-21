// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSurface.h"


//////////////////////////////////////////////////////////////////////
CBSurface::CBSurface(CBGame* inGame):CBBase(inGame)
{	
	m_ReferenceCount = 0;	

	m_Width = m_Height = 0;

	m_Filename = NULL;

	m_PixelOpReady = false;

	m_CKDefault = true;
	m_CKRed = m_CKGreen = m_CKBlue = 0;
	m_LifeTime = 0;
	m_KeepLoaded = false;

	m_LastUsedTime = 0;
	m_Valid = false;
}


//////////////////////////////////////////////////////////////////////
CBSurface::~CBSurface()
{
	if(m_PixelOpReady) EndPixelOp();
	if(m_Filename) delete [] m_Filename;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurface::Create(char * Filename, bool default_ck, BYTE ck_red, BYTE ck_green, BYTE ck_blue, int LifeTime, bool KeepLoaded)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurface::Restore()
{
	return E_FAIL;
}




//////////////////////////////////////////////////////////////////////
bool CBSurface::IsTransparentAt(int X, int Y)
{
	return false;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurface::Display(int X, int Y, RECT rect, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurface::DisplayTrans(int X, int Y, RECT rect, DWORD Alpha, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurface::DisplayTransZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurface::DisplayZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha, bool Transparent, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBSurface::DisplayHalfTrans(int X, int Y, RECT rect)
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::DisplayTransform(int X, int Y, int HotX, int HotY, RECT Rect, float ZoomX, float ZoomY, DWORD Alpha, float Rotate, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	return DisplayTransZoom(X, Y, Rect, ZoomX, ZoomY, Alpha, BlendMode, MirrorX, MirrorY);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::Create(int Width, int Height){
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::StartPixelOp()
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::EndPixelOp()
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::GetPixel(int X, int Y, BYTE *R, BYTE *G, BYTE *B, BYTE *A)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::PutPixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
bool CBSurface::ComparePixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A)
{
	return false;
}


//////////////////////////////////////////////////////////////////////
bool CBSurface::IsTransparentAtLite(int X, int Y)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::Invalidate()
{
	return E_FAIL;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBSurface::PrepareToDraw()
{
	m_LastUsedTime = Game->m_LiveTimer;
	
	if(!m_Valid){
		//Game->LOG(0, "Reviving: %s", m_Filename);
		return Create(m_Filename, m_CKDefault, m_CKRed, m_CKGreen, m_CKBlue, m_LifeTime, m_KeepLoaded);
	}
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CBSurface::SetFilename(char* Filename)
{
	SAFE_DELETE_ARRAY(m_Filename);
	if(!Filename) return;

	m_Filename = new char[strlen(Filename)+1];
	if(m_Filename) strcpy(m_Filename, Filename);
}

//////////////////////////////////////////////////////////////////////////
void CBSurface::SetSize(int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;
}
