// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
//#include <D3dx8tex.h>
#include "3DShadowGen.h"
#include "XModel.h"
#include "3DUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
C3DShadowGen::C3DShadowGen(CBGame* inGame):CBBase(inGame)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	m_ShadowTexWidth = min(512, m_Renderer->m_MaxTexWidth);
	m_ShadowTexHeight = min(512, m_Renderer->m_MaxTexHeight);

	m_RenderTargetTexture = NULL;
	m_RenderTargetZBuffer = NULL;
	m_RenderTargetSurface = NULL;
	m_RenderToSurface     = NULL;	
	
	m_FloorVB = NULL;
	m_ShadowVB = NULL;

	m_Ready = false;

	m_CurrentColor = 0x80000000;


	// check device caps
	D3DCAPS Caps;
	memset(&Caps, 0, sizeof(D3DCAPS));
	m_Renderer->m_Device->GetDeviceCaps(&Caps);
	m_SupportsRealTime = (Caps.TextureCaps & D3DPTEXTURECAPS_PROJECTED) && (Caps.TextureAddressCaps & D3DPTADDRESSCAPS_CLAMP);
	Game->m_SupportsRealTimeShadows = m_SupportsRealTime;

	m_Initialized = false;
}


//////////////////////////////////////////////////////////////////////////
C3DShadowGen::~C3DShadowGen(void)
{
	InvalidateDeviceObjects();
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::InvalidateDeviceObjects()
{
	if(!m_Initialized) return S_OK;


	RELEASE(m_FloorVB);
	RELEASE(m_ShadowVB);

	RELEASE(m_RenderTargetTexture);
	RELEASE(m_RenderTargetZBuffer);
	RELEASE(m_RenderTargetSurface);
	RELEASE(m_RenderToSurface);

	m_Ready = false;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::RestoreDeviceObjects()
{
	if(!m_Initialized) return S_OK;

	m_Ready = SUCCEEDED(CreateSimpleShadow());
	if(m_Ready && m_SupportsRealTime)
	{
		m_Ready = SUCCEEDED(CreateSurfaces());
		if(m_Ready)	m_Ready = SUCCEEDED(CreateFloor());
	}

	return m_Ready?S_OK:E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::CreateSimpleShadow()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	RELEASE(m_ShadowVB);

	HRESULT Ret = m_Renderer->m_Device->CreateVertexBufferUni(4*sizeof(MODELVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_MODELVERTEX, D3DPOOL_MANAGED, &m_ShadowVB);
	if(SUCCEEDED(Ret))
	{
		MODELVERTEX* v;
		m_ShadowVB->LockVB(0, 0, &v, 0);

		v[0].x  = -1.0f;	v[0].y  =  0.0f;	v[0].z  =  1.0f;
		v[0].nx =  0.0f;	v[0].ny =  1.0f;	v[0].nz =  0.0f;
		v[0].u = 0.0f;
		v[0].v = 1.0f;

		v[1].x  =  1.0f;	v[1].y  =  0.0f;	v[1].z  =  1.0f;
		v[1].nx =  0.0f;	v[1].ny =  1.0f;	v[1].nz =  0.0f;
		v[1].u  =  1.0f;
		v[1].v  = 1.0f;

		v[2].x  = -1.0f;	v[2].y  =  0.0f;	v[2].z  = -1.0f;
		v[2].nx =  0.0f;	v[2].ny =  1.0f;	v[2].nz =  0.0f;
		v[2].u  = 0.0f;
		v[2].v  = 0.0f;

		v[3].x  =  1.0f;	v[3].y  =  0.0f;	v[3].z  = -1.0f;
		v[3].nx =  0.0f;	v[3].ny =  1.0f;	v[3].nz =  0.0f;
		v[3].u  = 1.0f;
		v[3].v  = 0.0f;

		m_ShadowVB->Unlock();
	}
	else Game->LOG(Ret, "Error creating vertex buffer for simple shadow");

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::CreateFloor()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	RELEASE(m_FloorVB);

	// create vertex buffer for the floor plane
	HRESULT Ret = m_Renderer->m_Device->CreateVertexBufferUni(4*sizeof(MODELVERTEXCOLOR), D3DUSAGE_WRITEONLY, D3DFVF_MODELVERTEXCOLOR, D3DPOOL_MANAGED, &m_FloorVB);
	if(SUCCEEDED(Ret))
	{
		MODELVERTEXCOLOR* v;
		m_FloorVB->LockVB(0, 0, &v, 0);

		// colors
		for(int i=0; i<4; i++)
		{
			v[i].color1 = m_CurrentColor;
			v[i].color2 = 0x00;
		}

		v[0].x  = -1.0f;	v[0].y  =  0.0f;	v[0].z  =  1.0f;
		v[0].nx =  0.0f;	v[0].ny =  1.0f;	v[0].nz =  0.0f;
		v[0].u = 0.0f;
		v[0].v = 1.0f;

		v[1].x  =  1.0f;	v[1].y  =  0.0f;	v[1].z  =  1.0f;
		v[1].nx =  0.0f;	v[1].ny =  1.0f;	v[1].nz =  0.0f;
		v[1].u  =  1.0f;
		v[1].v  = 1.0f;

		v[2].x  = -1.0f;	v[2].y  =  0.0f;	v[2].z  = -1.0f;
		v[2].nx =  0.0f;	v[2].ny =  1.0f;	v[2].nz =  0.0f;
		v[2].u  = 0.0f;
		v[2].v  = 0.0f;

		v[3].x  =  1.0f;	v[3].y  =  0.0f;	v[3].z  = -1.0f;
		v[3].nx =  0.0f;	v[3].ny =  1.0f;	v[3].nz =  0.0f;
		v[3].u  = 1.0f;
		v[3].v  = 0.0f;

		D3DXMATRIX scale, trans;
		D3DXMatrixScaling(&scale, 250.0f, 1.0f, 250.0f );
		//D3DXMatrixTranslation(&trans, 50.0f, 0.0f, 50.0f );
		D3DXMatrixTranslation(&trans, 0.0f, 0.0f, 0.0f );
		D3DXMatrixMultiply(&m_FloorMat, &scale, &trans );

		m_FloorVB->Unlock();
	}
	else Game->LOG(Ret, "Error creating vertex buffer for floor");

	return Ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::CreateSurfaces()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	RELEASE(m_RenderTargetTexture);
	RELEASE(m_RenderTargetZBuffer);
	RELEASE(m_RenderTargetSurface);
	RELEASE(m_RenderToSurface);

	HRESULT Ret;
	// create render target texture
	if(	FAILED(Ret = D3DXCreateTexture(m_Renderer->m_Device, m_ShadowTexWidth, m_ShadowTexHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture)) &&
		FAILED(Ret = D3DXCreateTexture(m_Renderer->m_Device, m_ShadowTexWidth, m_ShadowTexHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_RenderTargetTexture)))
		//FAILED(Ret = D3DXCreateTexture(m_Renderer->m_Device, m_ShadowTexWidth, m_ShadowTexHeight, 1, D3DUSAGE_DEPTHSTENCIL, D3DFMT_D24S8, D3DPOOL_DEFAULT, &m_RenderTargetTexture)))
	{
		return Ret;
	}

	D3DSURFACE_DESC desc;

	m_RenderTargetTexture->GetSurfaceLevel(0, &m_RenderTargetSurface);
	m_RenderTargetSurface->GetDesc(&desc);

	if(FAILED(Ret = D3DXCreateRenderToSurface(m_Renderer->m_Device, desc.Width, desc.Height, 
		desc.Format, FALSE, D3DFMT_UNKNOWN, &m_RenderToSurface)))
	{
		return Ret;
	}

	// create render target z-buffer
	Ret = C3DUtils::CreateDepthStencilSurface(m_Renderer->m_Device, m_ShadowTexWidth, m_ShadowTexHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, &m_RenderTargetZBuffer);
	
	return Ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::Render(CBObject* Object, D3DXVECTOR3* LightPos, D3DXVECTOR3* LightTarget, bool LightPosRelative)
{
	if(!m_Initialized)
	{
		m_Initialized = true;
		RestoreDeviceObjects();
	}

	
	if(!m_Ready || !Object || !LightPos) return E_FAIL;

	// redirect simple shadow if needed
	//bool SimpleShadow = Object->m_ShadowType==SHADOW_SIMPLE || Game->m_MaxShadowType <= SHADOW_SIMPLE;
	bool SimpleShadow = Game->GetMaxShadowType(Object) <= SHADOW_SIMPLE;
	if(!m_SupportsRealTime) SimpleShadow = true;
	if(SimpleShadow) return RenderSimple(Object);

	// change shadow color if necessary
	if(Object->m_ShadowColor != m_CurrentColor)
	{
		m_CurrentColor = Object->m_ShadowColor;

		MODELVERTEXCOLOR* Verts;
		if(SUCCEEDED(m_FloorVB->LockVB(0, 0, &Verts, 0)))
		{
			for(int i=0; i<4; i++)
			{
				Verts[i].color1 = m_CurrentColor;
			}
			m_FloorVB->Unlock();
		}		
	}

	// setup light view matrix
	D3DXMATRIX MatLightView;
	
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 pos;
	D3DXVECTOR3 target;
	
	if(LightPosRelative) pos = Object->m_PosVector + *LightPos;
	else pos = *LightPos;

	if(LightTarget) target = *LightTarget;
	else target	 = Object->m_PosVector;
	

	D3DXMatrixLookAtLH(&MatLightView, &pos, &target, &up);
	
	// light projection matrix
	D3DXMATRIX LightProjMat;
	SetAdjustedProjectionMatrix(LightProjMat, D3DX_PI/4.0f, 1.0f, 1.0f, 10000.0f, 0.5f, -0.5f, (float)m_ShadowTexWidth, (float)m_ShadowTexHeight);


	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
	LPDIRECT3DDEVICE m_Device = m_Renderer->m_Device;


	//SetTextureMatrixStage0Stage1(&LightProjMat, &MatLightView);

	// old matrices
	D3DXMATRIX OrigViewMat;
	m_Device->GetTransform(D3DTS_VIEW, &OrigViewMat);
	D3DXMATRIX OrigProjMat;
	m_Device->GetTransform(D3DTS_PROJECTION, &OrigProjMat);


	// get old render target and z-buffer
	LPDIRECT3DSURFACE OldRenderTarget, OldZBuffer;
	C3DUtils::GetRenderTarget(m_Device, &OldRenderTarget, &OldZBuffer);
	//m_Device->GetRenderTarget(&OldRenderTarget);
	//m_Device->GetDepthStencilSurface(&OldZBuffer);

	// get old viewport
	D3DVIEWPORT OldViewport;
	m_Device->GetViewport(&OldViewport);

	// setup new viewport
	D3DVIEWPORT NewViewport;
	NewViewport.X = 1;
	NewViewport.Y = 1;
	NewViewport.Width = m_ShadowTexWidth - 2;
	NewViewport.Height = m_ShadowTexHeight - 2;
	NewViewport.MinZ = 0.0f;
	NewViewport.MaxZ = 1.0f;
	
	// set new render target
	HRESULT Ret;
	do 
	{
		Ret = C3DUtils::SetRenderTarget(m_Device, m_RenderTargetSurface, m_RenderTargetZBuffer);
	}   
	while(Ret!=D3D_OK);
	

	// clear the render target background to all white
	m_Device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00ffffffL, 1.0f, 0L);

	m_Device->SetViewport(&NewViewport);

	// set light point-of-view view transformation.
	m_Device->SetTransform(D3DTS_VIEW, &MatLightView);

	// set texel center aligning projection transformation.
	m_Device->SetTransform(D3DTS_PROJECTION, &LightProjMat);

//	m_RenderToSurface->BeginScene(m_RenderTargetSurface, &NewViewport);

	//SetColorFactorShader(m_Device, 0x00808080);
	SetColorFactorShader(m_Device, m_CurrentColor);

	Object->RenderModel();


//	m_RenderToSurface->EndScene();

	m_Device->SetTransform(D3DTS_VIEW, &OrigViewMat);
	m_Device->SetTransform(D3DTS_PROJECTION, &OrigProjMat);
	
	do 
	{
		Ret = C3DUtils::SetRenderTarget(m_Device, OldRenderTarget, OldZBuffer);
	}   
	while(Ret!=D3D_OK);

	OldRenderTarget->Release();
	OldZBuffer->Release();

	m_Device->SetViewport(&OldViewport);



	m_Renderer->Setup3D(NULL, true);


	D3DXMATRIX trans, final;
	D3DXMatrixTranslation(&trans, Object->m_PosVector.x, Object->m_PosVector.y, Object->m_PosVector.z);
	D3DXMatrixMultiply(&final, &m_FloorMat, &trans);
	m_Renderer->m_Device->SetTransform( D3DTS_WORLD, &final);


	SetTextureMatrixStage0Stage1(&LightProjMat, &MatLightView);
	
	//m_Renderer->m_Device->SetTexture(0, NULL);
	m_Renderer->m_Device->SetTexture(0, m_RenderTargetTexture);

	
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );	
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );	

	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	
	m_Device->SetRenderState(D3DRS_COLORVERTEX, TRUE);

	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED | D3DTTFF_COUNT4 );

	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );

	// render
	C3DUtils::SetFixedVertexShader(m_Renderer->m_Device, D3DFVF_MODELVERTEXCOLOR);
	C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_FloorVB, sizeof(MODELVERTEXCOLOR));
	m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	
	//Game->RenderShadowGeometry();

	m_Renderer->m_Device->SetTexture(0, NULL);

	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_TEXCOORDINDEX, 0 );
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_TEXTURETRANSFORMFLAGS, 0);

	m_Renderer->SetSpriteBlendMode(BLEND_NORMAL);

	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

	m_Device->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	m_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::RenderSimple(CBObject* Object)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	CBSurfaceD3D* ShadowImage;
	if(Object->m_ShadowImage) ShadowImage = (CBSurfaceD3D*)Object->m_ShadowImage;
	else ShadowImage = (CBSurfaceD3D*)Game->m_ShadowImage;

	if(!ShadowImage) return S_OK;


	D3DXMATRIX scale, trans, rot, final;
	D3DXMatrixScaling(&scale, Object->m_ShadowSize * Object->m_Scale3D, 1.0f, Object->m_ShadowSize * Object->m_Scale3D);
	D3DXMatrixRotationY(&rot, DegToRad(Object->m_Angle));
	D3DXMatrixTranslation(&trans, Object->m_PosVector.x, Object->m_PosVector.y, Object->m_PosVector.z);
	D3DXMatrixMultiply(&final, &scale, &rot);
	D3DXMatrixMultiply(&final, &final, &trans);
	m_Renderer->m_Device->SetTransform( D3DTS_WORLD, &final);

	m_Renderer->m_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_Renderer->m_Device->SetTexture(0, ((CBSurfaceD3D*)ShadowImage)->m_Texture);

	C3DUtils::SetFixedVertexShader(m_Renderer->m_Device, D3DFVF_MODELVERTEX);
	C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_ShadowVB, sizeof(MODELVERTEX));
	m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_Renderer->m_Device->SetTexture(0, NULL);
	m_Renderer->m_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}


// Desc: Creates a projection matrix that adjusts pixel centers by fPixDx, fPixDy.
//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::SetAdjustedProjectionMatrix(	D3DXMATRIX& mat, FLOAT fFOV, FLOAT fAspect,
														FLOAT fNearPlane, FLOAT fFarPlane,
														FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
														FLOAT fVPHeight )
{
	if( fabs(fFarPlane-fNearPlane) < 0.01f )
		return E_INVALIDARG;
	if( fabs(sin(fFOV/2)) < 0.01f )
		return E_INVALIDARG;

	float h =   1.0f  * ( cosf(fFOV/2)/sinf(fFOV/2) );

	float fTop = fNearPlane/h;
	float fBottom = -fTop;
	float fRight = fTop * fAspect;
	float fLeft = -fRight;

	float fXWSize = fRight - fLeft;
	float fYWSize = fTop - fBottom;

	float fDx = -( fPixDx*fXWSize/fVPWidth );
	float fDy = -( fPixDy*fYWSize/fVPHeight );

	SetFrustumMatrix(mat, fLeft+fDx, fRight+fDx, fTop+fDy, fBottom+fDy, fNearPlane, fFarPlane);

	return S_OK;
}




// Desc: Creates a projection matrix base on frustum dimension paramters.
//////////////////////////////////////////////////////////////////////////
void C3DShadowGen::SetFrustumMatrix(	D3DXMATRIX& mat, 
										FLOAT fLeft, FLOAT fRight, FLOAT fTop,
										FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane)
{
	float Q = fFarPlane / ( fFarPlane - fNearPlane );

	ZeroMemory(&mat, sizeof(D3DXMATRIX));
	mat._11 = ( 2.0f*fNearPlane )/( fRight - fLeft );
	mat._22 = ( 2.0f*fNearPlane )/( fTop - fBottom );
	mat._31 = ( fRight + fLeft )/ (fRight - fLeft );
	mat._32 = ( fTop + fBottom )/ (fTop - fBottom );
	mat._33 = Q;
	mat._34 = 1.0f;
	mat._43 = -Q*fNearPlane;
}

// Desc: Apply solid color factor.
//////////////////////////////////////////////////////////////////////////
void C3DShadowGen::SetColorFactorShader(LPDIRECT3DDEVICE Device, D3DCOLOR Color)
{
	Device->SetRenderState(D3DRS_TEXTUREFACTOR, Color);

	C3DUtils::SetTextureStageState(Device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	C3DUtils::SetTextureStageState(Device, 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );
	C3DUtils::SetTextureStageState(Device, 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
	C3DUtils::SetTextureStageState(Device, 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	C3DUtils::SetTextureStageState(Device, 0, D3DTSS_TEXCOORDINDEX, 0 );
	C3DUtils::SetTextureStageState(Device, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowGen::SetTextureMatrixStage0Stage1(D3DXMATRIX* MatLightProj, D3DXMATRIX* MatLightView)
{
	D3DXMATRIX V, InvV;

	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
	LPDIRECT3DDEVICE m_Device = m_Renderer->m_Device;

	if(!m_Renderer->m_Camera) return E_FAIL;
	m_Renderer->m_Camera->GetViewMatrix(&V);

	float det = 0.0f;
	D3DXMatrixInverse(&InvV, &det, &V);

	D3DXMATRIX matTexScale;

	// Create texture scaling matrix:
	// | 0.5      0        0        0 |
	// | 0        -0.5     0        0 |
	// | 0        0        0        0 |
	// | 0.5      0.5      1        1 |

	// This will scale and offset -1 to 1 range of x, y
	// coords output by projection matrix to 0-1 texture
	// coord range.
	D3DXMatrixIdentity(&matTexScale);
	matTexScale._11 = 0.5f;
	matTexScale._22 = -0.5f;
	matTexScale._33 = 0.0f;
	matTexScale._41 = 0.5f; 
	matTexScale._42 = 0.5f;
	matTexScale._43 = 1.0f; 
	matTexScale._44 = 1.0f;


	D3DXMATRIX mat, mat2, m_matTex2;
	D3DXMatrixMultiply(&mat, MatLightProj, &matTexScale );
	D3DXMatrixMultiply(&mat2, MatLightView, &mat ); 
	D3DXMatrixMultiply(&m_matTex2, &InvV, &mat2); 

	HRESULT hr = m_Device->SetTransform( D3DTS_TEXTURE0, &m_matTex2 );
	if( FAILED( hr ) )
		return E_FAIL;
		
	return S_OK;
}
