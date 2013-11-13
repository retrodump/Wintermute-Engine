// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CAdObject3D;

class C3DShadowGen :
	public CBBase
{
public:
	DWORD m_CurrentColor;
	bool m_SupportsRealTime;
	bool m_Initialized;
	C3DShadowGen(CBGame* Game);
	virtual ~C3DShadowGen(void);
	HRESULT Render(CBObject* Object, D3DXVECTOR3* LightPos, D3DXVECTOR3* LightTarget, bool LightPosRelative);
	bool m_Ready;

	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();


	static HRESULT SetAdjustedProjectionMatrix(D3DXMATRIX& mat, FLOAT fFOV, FLOAT fAspect,
		FLOAT fNearPlane, FLOAT fFarPlane,
		FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
		FLOAT fVPHeight );
	static void SetFrustumMatrix(D3DXMATRIX& mat, 
		FLOAT fLeft, FLOAT fRight, FLOAT fTop,
		FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane );

	static void SetColorFactorShader(LPDIRECT3DDEVICE Device, D3DCOLOR Color);

	HRESULT SetTextureMatrixStage0Stage1(D3DXMATRIX* MatLightProj, D3DXMATRIX* MatLightView);

private:
	HRESULT RenderSimple(CBObject* Object);
	HRESULT CreateFloor();
	HRESULT CreateSimpleShadow();
	HRESULT CreateSurfaces();
	LPDIRECT3DVERTEXBUFFER m_FloorVB;
	LPDIRECT3DVERTEXBUFFER m_ShadowVB;
	D3DXMATRIX m_FloorMat;
	LPDIRECT3DTEXTURE m_RenderTargetTexture;	
	LPDIRECT3DSURFACE m_RenderTargetZBuffer;
	LPDIRECT3DSURFACE m_RenderTargetSurface;
	LPD3DXRENDERTOSURFACE m_RenderToSurface;
	int m_ShadowTexWidth;
	int m_ShadowTexHeight;

};
