// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BRENDERD3D_H__B2BE93C5_5064_4AD3_8052_425EEB8729B3__INCLUDED_)
#define AFX_BRENDERD3D_H__B2BE93C5_5064_4AD3_8052_425EEB8729B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C3DCamera;
class C3DShadowGen;

#include "BRenderer.h"

class CBRenderD3D : public CBRenderer  
{
public:
	HRESULT GetProjectionParams(float* ResWidth, float* ResHeight, float* LayerWidth, float* LayerHeight, float* ModWidth, float* ModHeight, bool* CustomViewport);
	int GetMaxActiveLights();

	HRESULT SetAmbientLightColor(DWORD Color);
	HRESULT SetDefaultAmbientLightColor();

	DWORD m_AmbientLightColor;
	bool m_AmbientLightOverride;

	virtual void DumpData(char* Filename);
	virtual HRESULT EnableShadows();
	virtual HRESULT DisableShadows();
	virtual bool StencilSupported();
	HRESULT DisplayShadow(CBObject* Object, D3DXVECTOR3* LightPos, D3DXVECTOR3* LightTarget, bool LightPosRelative);
	C3DShadowGen* m_ShadowGen;
	HRESULT InvalidateTexture(LPDIRECT3DTEXTURE Texture);

	TSpriteBlendMode m_BlendMode;
	HRESULT SetSpriteBlendMode(TSpriteBlendMode Mode, bool ForceChange=false);
	virtual const char* GetName();
	virtual HRESULT DisplayDebugInfo();

	virtual CBImage* TakeScreenshot();
	virtual HRESULT SetViewport(int left, int top, int right, int bottom);
	bool m_Force16bitTextures;
	int m_MaxTexWidth;
	int m_MaxTexHeight;
	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();
	LPDIRECT3DTEXTURE m_LastTexture;
	virtual HRESULT Fade(WORD Alpha);
	virtual HRESULT FadeToColor(DWORD Color, RECT* rect=NULL);
	int m_MultisampleType;
	bool m_HardwareTL;
	virtual HRESULT DrawLine(int X1, int Y1, int X2, int Y2, DWORD Color);
	int m_NumPolygons;
	float m_FOV;
	float m_NearClipPlane;
	float m_FarClipPlane;
	virtual HRESULT SetProjection();
	HRESULT DrawSprite(LPDIRECT3DTEXTURE Texture, CONST RECT* Rect, float ZoomX, float ZoomY, CONST D3DXVECTOR2* Pos, DWORD Color, bool AlphaDisable=false, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	HRESULT DrawSpriteEx(LPDIRECT3DTEXTURE Texture, CONST RECT *Rect, CONST D3DXVECTOR2 *Pos, CONST D3DXVECTOR2 *Rot, CONST D3DXVECTOR2 *Scale, float Angle, DWORD Color, bool AlphaDisable=false, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	LPDIRECT3DVERTEXBUFFER m_SpriteVB;
	LPDIRECT3DVERTEXBUFFER m_SpriteBatchVB;
	LPDIRECT3DVERTEXBUFFER m_LineVB;
	LPDIRECT3DVERTEXBUFFER m_ShaderVB;
	TRendererState m_State;
	virtual HRESULT Setup3D(C3DCamera* Camera=NULL, bool Force=false);
	virtual HRESULT Setup3DCustom(D3DXMATRIX* ViewMat, D3DXMATRIX* ProjMat);
	virtual HRESULT Setup2D(bool Force=false);
	virtual HRESULT SetupLines();
	C3DCamera* m_Camera;
	HRESULT ResetDevice();
	virtual void InitLoop();
	virtual HRESULT Fill(BYTE r, BYTE g, BYTE b, RECT* rect=NULL);
	virtual HRESULT Flip();
	virtual HRESULT InitRenderer(CHWManager* hwManager);
	virtual HRESULT SwitchFullscreen();
	virtual HRESULT WindowedBlt();	
	//LPD3DXSPRITE m_Sprite;
	LPDIRECT3DDEVICE m_Device;
	LPDIRECT3D m_Direct3D;
	CBRenderD3D(CBGame* inGame);
	virtual ~CBRenderD3D();

	D3DFORMAT m_TextureFormat;
	D3DFORMAT m_FullScreenFormat;
	D3DFORMAT m_WindowedFormat;
	D3DFORMAT m_Format;
	D3DFORMAT m_DepthStencilFormat;	
	virtual bool UsingStencilBuffer();

    DWORD m_VBBase;
    DWORD m_VBFlush;
	DWORD m_VBDiscard;

	DWORD m_BatchVBBase;
	DWORD m_BatchVBFlush;
	DWORD m_BatchVBDiscard;

    DWORD m_LineVBBase;
    DWORD m_LineVBFlush;
	DWORD m_LineVBDiscard;

	virtual HRESULT StartSpriteBatch();
	virtual HRESULT EndSpriteBatch();
	HRESULT CommitSpriteBatch();
	bool m_SpriteBatchMode;
	TSpriteBlendMode m_BatchBlendMode;
	bool m_BatchAlphaDisable;
	int m_BatchVertices;
	LPDIRECT3DTEXTURE m_BatchTexture;

	virtual HRESULT DrawShaderQuad();

private:
	HRESULT CreateShaderQuad();
	HRESULT SetAmbientLightRenderState();
	D3DMATRIX* BuildMatrix(D3DMATRIX* pOut, const D3DXVECTOR2* centre, const D3DXVECTOR2* scaling, float angle);
	void TransformVertices(struct SPRITEVERTEX* vertices, const D3DXVECTOR2* pCentre, const D3DXVECTOR2* pScaling, float angle);
	bool m_UsingStencil;
};

#endif // !defined(AFX_BRENDERD3D_H__B2BE93C5_5064_4AD3_8052_425EEB8729B3__INCLUDED_)
