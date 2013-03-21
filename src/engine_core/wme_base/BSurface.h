// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSURFACE_H__A939B640_13BF_11D4_9F37_D411023FBD14__INCLUDED_)
#define AFX_BSURFACE_H__A939B640_13BF_11D4_9F37_D411023FBD14__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBSurface:public CBBase
{
public:
	virtual HRESULT Invalidate();
	virtual HRESULT PrepareToDraw();
	bool m_CKDefault;
	BYTE m_CKRed;
	BYTE m_CKGreen;
	BYTE m_CKBlue;

	DWORD m_LastUsedTime;
	bool m_Valid;
	int m_LifeTime;
	bool m_KeepLoaded;

	bool m_PixelOpReady;
	CBSurface(CBGame* inGame);	
	virtual ~CBSurface();	

	virtual HRESULT DisplayHalfTrans(int X, int Y, RECT rect);
	virtual bool IsTransparentAt(int X, int Y);
	virtual HRESULT DisplayTransZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha=0xFFFFFFFF, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayTrans(int X, int Y, RECT rect, DWORD Alpha=0xFFFFFFFF, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT Display(int X, int Y, RECT rect, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha=0xFFFFFFFF, bool Transparent=false, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayTransform(int X, int Y, int HotX, int HotY, RECT Rect, float ZoomX, float ZoomY, DWORD Alpha, float Rotate, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT Restore();
	virtual HRESULT Create(char* Filename, bool default_ck, BYTE ck_red, BYTE ck_green, BYTE ck_blue, int LifeTime=-1, bool KeepLoaded=false);
	virtual HRESULT Create(int Width, int Height);
	virtual HRESULT PutPixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A=-1);
	virtual HRESULT GetPixel(int X, int Y, BYTE* R, BYTE* G, BYTE* B, BYTE* A=NULL);
	virtual bool ComparePixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A=-1);
	virtual HRESULT StartPixelOp();
	virtual HRESULT EndPixelOp();
	virtual bool IsTransparentAtLite(int X, int Y);
	void SetFilename(char* Filename);
	void SetSize(int Width, int Height);
	
	int m_ReferenceCount;
	char* m_Filename;

	int GetWidth(){ return m_Width;	}
	int GetHeight(){ return m_Height; }
	//void SetWidth(int Width){ m_Width = Width;	}
	//void SetHeight(int Height){ m_Height = Height; }
protected:
	int m_Height;
	int m_Width;

};

#endif // !defined(AFX_BSURFACE_H__A939B640_13BF_11D4_9F37_D411023FBD14__INCLUDED_)
