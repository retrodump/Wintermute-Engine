// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BRenderD3D.h"
#include "3DUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DEFAULT_NEAR_PLANE 90.0f
#define DEFAULT_FAR_PLANE  10000.0f

//////////////////////////////////////////////////////////////////////////
CBRenderD3D::CBRenderD3D(CBGame* inGame):CBRenderer(inGame)
{
	m_Direct3D = NULL;
	m_Device = NULL;
	m_SpriteVB = NULL;
	m_SpriteBatchVB = NULL;
	m_LineVB = NULL;
	m_ShaderVB = NULL;
	m_Camera = NULL;
	m_NumPolygons = 0;

	m_HardwareTL = false;
	m_MultisampleType = D3DMULTISAMPLE_NONE;

	m_State = RSTATE_NONE;
	m_FOV = D3DX_PI/4;

	m_NearClipPlane = DEFAULT_NEAR_PLANE;
	m_FarClipPlane = DEFAULT_FAR_PLANE;

	m_LastTexture = NULL;

	m_VBFlush = 4;
	m_VBDiscard = 2048;
	m_VBBase = m_VBDiscard;

	m_BatchVBFlush = 6;
	//m_BatchVBDiscard = 2048;
	m_BatchVBDiscard = 256; // revise later with new ForceWare/Vista
	m_BatchVBBase = m_VBDiscard;

	m_LineVBFlush = 4;
	m_LineVBDiscard = 2048;
	m_LineVBBase = m_LineVBDiscard;


	m_MaxTexWidth = m_MaxTexHeight = 2048;

	m_Force16bitTextures = false;

	m_UsingStencil = false;
	m_DepthStencilFormat = D3DFMT_D16;

	m_ShadowGen = NULL;

	m_BlendMode = BLEND_UNKNOWN;


	m_SpriteBatchMode = false;
	m_BatchBlendMode = BLEND_UNKNOWN;
	m_BatchAlphaDisable = false;
	m_BatchVertices = 0;
	m_BatchTexture = NULL;

	m_AmbientLightColor = 0x00000000;
	m_AmbientLightOverride = false;

}


//////////////////////////////////////////////////////////////////////////
CBRenderD3D::~CBRenderD3D()
{
	m_Camera = NULL; // ref only
	SAFE_DELETE(m_ShadowGen);
	InvalidateDeviceObjects();	
	RELEASE(m_Device);
	RELEASE(m_Direct3D);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::InitRenderer(CHWManager* hwManager)
{
	if(!hwManager->m_SelectedVideoDevice) return E_FAIL;

	HRESULT ret;

	m_Windowed = hwManager->m_SelectedWindowed;
	hwManager->GetResolution(&m_Width, &m_Height, &m_RealWidth, &m_RealHeight);

	m_DrawOffsetX = (m_RealWidth - m_Width) / 2;
	m_DrawOffsetY = (m_RealHeight - m_Height) / 2;

	m_BPP = hwManager->m_Selected16bitColors?16:32;
	m_Force16bitTextures = hwManager->m_Force16bitTextures;

	m_Width = max(10, m_Width);
	m_Height = max(10, m_Height);
	m_RealWidth = max(10, m_RealWidth);
	m_RealHeight = max(10, m_RealHeight);

	if((m_Direct3D = Game->m_DirectX.Direct3DCreate(D3D_SDK_VERSION)) == NULL){
		Game->LOG(0, "Error creating Direct3D object.");
		return E_FAIL;
	}

	// pick up correct video mode
	CHWVideoMode* mode = NULL;
	CHWVideoMode* BestMode = NULL;
	for(int i=0; i<hwManager->m_SelectedVideoDevice->m_Modes.GetSize(); i++){
		mode = hwManager->m_SelectedVideoDevice->m_Modes[i];

		if(mode->m_Windowed!=m_Windowed) continue;
		if(!m_Windowed && (mode->m_BPP != m_BPP || mode->m_Width != m_RealWidth || mode->m_Height != m_RealHeight)) continue;
		
		if(mode->m_Multisample[hwManager->m_SelectedMultisample] && mode->m_SupportsStencil){
			BestMode = mode;
			break;
		}
		else if(!BestMode) BestMode = mode;
	}

	if(!BestMode) return E_FAIL;

	// set actual info
	m_Format = BestMode->m_Format;
	if(BestMode->m_Multisample[hwManager->m_SelectedMultisample]) m_MultisampleType = hwManager->m_SelectedMultisample;
	else m_MultisampleType = D3DMULTISAMPLE_NONE;

	m_HardwareTL = hwManager->m_SelectedTL;

	if(Game->m_MaxShadowType >= SHADOW_STENCIL && BestMode->m_SupportsStencil)
	{
		m_DepthStencilFormat = BestMode->m_DepthStencilFormat;
		m_UsingStencil = true;
	}
	else
	{
		m_DepthStencilFormat = D3DFMT_D16;
		//m_DepthStencilFormat = D3DFMT_UNKNOWN;
		m_UsingStencil = false;
	}


	
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferWidth  = m_RealWidth;
	d3dpp.BackBufferHeight = m_RealHeight;
	d3dpp.BackBufferFormat = m_Format;
	d3dpp.hDeviceWindow    = m_Window;
	d3dpp.Windowed         = m_Windowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = m_DepthStencilFormat;
	d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)m_MultisampleType;
#ifdef WME_D3D9
	if(m_Windowed) d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif

	if(FAILED(ret = m_Direct3D->CreateDevice(
					hwManager->m_SelectedVideoDevice->m_AdapterID,
					D3DDEVTYPE_HAL,
					m_Window,
					m_HardwareTL?D3DCREATE_HARDWARE_VERTEXPROCESSING:D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp,
					&m_Device))){

		Game->LOG(ret, "Error creating 3D device.");
		return ret;
	}

	// get capabilities
	D3DCAPS DevCaps;
	memset(&DevCaps, 0, sizeof(D3DCAPS));
	m_Device->GetDeviceCaps(&DevCaps);
	m_MaxTexWidth = DevCaps.MaxTextureWidth;
	m_MaxTexHeight = DevCaps.MaxTextureHeight;

	if(Game->m_DEBUG_DebugMode && Game->m_Registry->ReadBool("Debug", "DumbVideoCard"))
		m_MaxTexWidth = m_MaxTexHeight = 256;

	if(hwManager->m_ForceTextureSubdivision){
		Game->LOG(0, "Forcing texture subdivision");
		m_MaxTexWidth = m_MaxTexHeight = 256;
	}

	Game->LOG(0, "Maximum texture size: %dx%d", m_MaxTexWidth, m_MaxTexHeight);


	// create vertex buffer for sprites
	if(FAILED(ret=RestoreDeviceObjects())) return ret;

	SetProjection();
	ClipCursor();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CBRenderD3D::InitLoop()
{
	CBRenderer::InitLoop();
	m_Device->BeginScene();
	Setup2D();
	m_NumPolygons = 0;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::Flip()
{
	m_LastTexture = NULL;

	HRESULT res;
    if(FAILED(res = m_Device->TestCooperativeLevel())){
        // If the device was lost, do not render until we get it back.
        if(res == D3DERR_DEVICELOST) return res;

        // Check if the device has come back and needs to be reset.
        if(res == D3DERR_DEVICENOTRESET){
			if(FAILED(res = ResetDevice())){
				Game->LOG(res, "Failed to restore the 3D device.");
                return res;
            }
        }        
        return res;
    }
			
	m_Device->EndScene();

	return m_Device->Present(NULL, NULL, NULL, NULL);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::Fill(BYTE r, BYTE g, BYTE b, RECT* rect)
{
	if(!Game->m_EditorMode)
	{
		D3DVIEWPORT vp;
		vp.X      = 0;
		vp.Y      = 0;
		vp.Width  = m_RealWidth;
		vp.Height = m_RealHeight;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;
		m_Device->SetViewport(&vp);
	}

	DWORD Flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	if(m_UsingStencil) Flags |= D3DCLEAR_STENCIL;

	return m_Device->Clear(0, NULL, Flags, D3DCOLOR_XRGB(r, g, b), 1.0f, 0);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::ResetDevice()
{
	//Game->LOG(0, "Resetting device...");

	m_State = RSTATE_NONE;
	InvalidateDeviceObjects();

	m_Width = max(10, m_Width);
	m_Height = max(10, m_Height);
	m_RealWidth = max(10, m_RealWidth);
	m_RealHeight = max(10, m_RealHeight);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferWidth  = m_RealWidth;
	d3dpp.BackBufferHeight = m_RealHeight;
	d3dpp.BackBufferFormat = m_Format;
	d3dpp.hDeviceWindow    = m_Window;
	d3dpp.Windowed         = m_Windowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = m_DepthStencilFormat;
	d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)m_MultisampleType;

	HRESULT res = m_Device->Reset(&d3dpp);
	if(FAILED(res))	Game->LOG(res, "Failed to reset 3D device.");

	if(FAILED(res=RestoreDeviceObjects())) return res;
	SetProjection();
	SetSpriteBlendMode(BLEND_UNKNOWN);
	ClipCursor();

	return res;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SwitchFullscreen()
{	
	return E_FAIL; // no longer supported!!!
	/*
	HRESULT ret = ResetDevice();
	CBRenderer::SwitchFullscreen();
	return ret;
	*/
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::Setup2D(bool Force)
{
	if(m_State!=RSTATE_2D || Force)
	{
		m_State = RSTATE_2D;

		
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		
		/*
		m_Device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_ANISOTROPIC);
		m_Device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_ANISOTROPIC);
		m_Device->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
		*/

		
		m_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);	
		m_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_Device->SetRenderState(D3DRS_AMBIENT, 0x00000000);
		
		m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		SetSpriteBlendMode(BLEND_NORMAL);

		m_Device->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE);
		//m_Device->SetRenderState(D3DRS_ALPHAREF,         0x08);
		m_Device->SetRenderState(D3DRS_ALPHAREF,         0x00);
		m_Device->SetRenderState(D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL);

		m_Device->SetRenderState(D3DRS_FILLMODE,   D3DFILL_SOLID);
		m_Device->SetRenderState(D3DRS_CULLMODE,   D3DCULL_CCW);
		m_Device->SetRenderState(D3DRS_STENCILENABLE,    FALSE);
		m_Device->SetRenderState(D3DRS_CLIPPING,         TRUE);
		m_Device->SetRenderState(D3DRS_CLIPPLANEENABLE,  FALSE);
		m_Device->SetRenderState(D3DRS_VERTEXBLEND,      FALSE);
		m_Device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_Device->SetRenderState(D3DRS_FOGENABLE,        FALSE);
		
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_TEXCOORDINDEX, 0);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		C3DUtils::SetTextureStageState(m_Device, 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		C3DUtils::SetTextureStageState(m_Device, 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

		if(m_MultisampleType!=0)
			m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::Setup3D(C3DCamera* Camera, bool Force)
{
	if(m_State!=RSTATE_3D || Force)
	{
		m_State = RSTATE_3D;

		// Set up other renderstates
		m_Device->SetRenderState(D3DRS_DITHERENABLE, TRUE);

		m_Device->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
		C3DUtils::SetTextureStageState(m_Device, 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
		m_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

		m_Device->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE);
		m_Device->SetRenderState(D3DRS_ALPHAREF,         0x08);

		SetAmbientLightRenderState();
		
		//m_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);

		m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		if(m_MultisampleType!=0)
			m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
		


		if(Camera) m_Camera = Camera;
		if(m_Camera)
		{
			D3DXMATRIX ViewMatrix;
			m_Camera->GetViewMatrix(&ViewMatrix);
			m_Device->SetTransform(D3DTS_VIEW, &ViewMatrix);

			m_FOV = m_Camera->m_FOV;

			m_NearClipPlane = m_Camera->m_NearClipPlane >= 0.0f ? m_Camera->m_NearClipPlane : DEFAULT_NEAR_PLANE;
			m_FarClipPlane  = m_Camera->m_FarClipPlane  >= 0.0f ? m_Camera->m_FarClipPlane  : DEFAULT_FAR_PLANE;
		}
		else
		{
			m_NearClipPlane = DEFAULT_NEAR_PLANE;
			m_FarClipPlane  = DEFAULT_FAR_PLANE;
		}

		// lighting
		m_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);




		// fog
		bool FogEnabled;
		DWORD FogColor;
		float FogStart, FogEnd;

		Game->GetFogParams(&FogEnabled, &FogColor, &FogStart, &FogEnd);
		if(FogEnabled)
		{
			m_Device->SetRenderState(D3DRS_FOGENABLE, TRUE);
			m_Device->SetRenderState(D3DRS_FOGCOLOR, FogColor);
			m_Device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			m_Device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&FogStart));
			m_Device->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&FogEnd));
		}
		else
		{
			m_Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
	
		SetProjection();
  }
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SetAmbientLightRenderState()
{
	if(m_AmbientLightOverride)
		return m_Device->SetRenderState(D3DRS_AMBIENT, m_AmbientLightColor);
	else
		return m_Device->SetRenderState(D3DRS_AMBIENT, Game->GetAmbientLightColor());

}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::Setup3DCustom(D3DXMATRIX* ViewMat, D3DXMATRIX* ProjMat)
{
	Setup3D();
	m_State = RSTATE_3D;
	if(ViewMat) m_Device->SetTransform(D3DTS_VIEW, ViewMat);
	if(ProjMat) m_Device->SetTransform(D3DTS_PROJECTION, ProjMat);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SetupLines()
{
	if(m_State!=RSTATE_LINES){
		m_State = RSTATE_LINES;

		m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_Device->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE);
		m_Device->SetRenderState(D3DRS_AMBIENT, 0x00000000);

		m_Device->SetTexture(0, NULL);
		m_LastTexture = NULL;

		if(m_MultisampleType!=0)
			m_Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::DrawSprite(LPDIRECT3DTEXTURE Texture, CONST RECT *Rect, float ZoomX, float ZoomY, CONST D3DXVECTOR2 *Pos, DWORD Color, bool AlphaDisable, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	D3DXVECTOR2 Scale(ZoomX / 100.0f, ZoomY / 100.f);
	return DrawSpriteEx(Texture, Rect, Pos, NULL, &Scale, 0.0f, Color, AlphaDisable, BlendMode, MirrorX, MirrorY);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::DrawSpriteEx(LPDIRECT3DTEXTURE Texture, CONST RECT* Rect, CONST D3DXVECTOR2* InPos, CONST D3DXVECTOR2 *Rot, CONST D3DXVECTOR2 *Scale, float Angle, DWORD Color, bool AlphaDisable, TSpriteBlendMode BlendMode, bool MirrorX, bool MirrorY)
{
	HRESULT res;
	D3DSURFACE_DESC sd;	

	D3DXVECTOR2 Pos = *InPos;

	if(FAILED(res = Texture->GetLevelDesc(0, &sd))) return res;

	if(m_ForceAlphaColor!=0) Color = m_ForceAlphaColor;


	if(m_SpriteBatchMode)
	{
		m_BatchTexture = Texture;
		m_BatchAlphaDisable = AlphaDisable;
		m_BatchBlendMode = BlendMode;
	}

	float left, right, bottom, top;
	float w, h;

	w = (Rect->right - Rect->left) * Scale->x;
	h = (Rect->bottom - Rect->top) * Scale->y;

	// texture coords
	left   = (float)Rect->left / (float)sd.Width;
	top    = (float)Rect->top / (float)sd.Height;
	right  = (float)(Rect->right) / (float)sd.Width;
	bottom = (float)(Rect->bottom) / (float)sd.Height;

	if(MirrorX)
	{
		float Temp = left;
		left = right;
		right = Temp;
	}
	if(MirrorY)
	{
		float Temp = top;
		top = bottom;
		bottom = Temp;
	}


	// lock vertex buffer
	SPRITEVERTEX* v;

	// batch mode
	if(m_SpriteBatchMode)
	{
		if(m_BatchVBBase + m_BatchVBFlush >= m_BatchVBDiscard)
		{
			CommitSpriteBatch();
			m_BatchVBBase = 0;
		}
		m_SpriteBatchVB->LockVB(m_BatchVBBase*sizeof(SPRITEVERTEX), m_BatchVBFlush * sizeof(SPRITEVERTEX), &v, m_BatchVBBase?D3DLOCK_NOOVERWRITE:D3DLOCK_DISCARD);
		m_BatchVBBase += m_BatchVBFlush;
	}
	// normal mode
	else
	{
		m_VBBase += m_VBFlush;

		if(m_VBBase >= m_VBDiscard)
			m_VBBase = 0;
		
		m_SpriteVB->LockVB(m_VBBase*sizeof(SPRITEVERTEX), m_VBFlush * sizeof(SPRITEVERTEX), &v, m_VBBase?D3DLOCK_NOOVERWRITE:D3DLOCK_DISCARD);
	}

	// position
	v[0].p = D3DXVECTOR4(Pos.x + 0 - 0.5f, Pos.y + h - 0.5f, 0.9f, 1.0f);
	v[1].p = D3DXVECTOR4(Pos.x + 0 - 0.5f, Pos.y + 0 - 0.5f, 0.9f, 1.0f);
	v[2].p = D3DXVECTOR4(Pos.x + w - 0.5f, Pos.y + h - 0.5f, 0.9f, 1.0f);
	v[3].p = D3DXVECTOR4(Pos.x + w - 0.5f, Pos.y + 0 - 0.5f, 0.9f, 1.0f);


	// color
	v[0].color = v[1].color = v[2].color = v[3].color = Color;

	// texture coordinates
	v[0].tu = left;	 v[0].tv = bottom; // lower left
	v[1].tu = left;	 v[1].tv = top;    // top left
	v[2].tu = right; v[2].tv = bottom; // bottom right
	v[3].tu = right; v[3].tv = top;    // top right

	if(Rot && Angle != 0.0f)
	{
		D3DXVECTOR2 sc(1.0f, 1.0f);
		TransformVertices(v, Rot, &sc, Angle);
	}


	for(int i=0; i<4; i++)
	{
		v[i].p.x += m_DrawOffsetX;
		v[i].p.y += m_DrawOffsetY;
	}


	if(m_SpriteBatchMode)
	{
		// make the second triangle
		v[4] = v[3];
		v[3] = v[1];
		v[5] = v[2];

		m_SpriteBatchVB->Unlock();
		m_BatchVertices += m_BatchVBFlush;
	}
	else
	{
		m_SpriteVB->Unlock();

		// render
		SetSpriteBlendMode(BlendMode);
		if(AlphaDisable)
		{
			m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		}

		if(m_LastTexture!=Texture)
		{
			m_LastTexture = Texture;
			m_Device->SetTexture(0, Texture);
		}

		C3DUtils::SetFixedVertexShader(m_Device, D3DFVF_SPRITEVERTEX);
		C3DUtils::SetStreamSource(m_Device, 0, m_SpriteVB, sizeof(SPRITEVERTEX));
		m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, m_VBBase, 2);

		if(AlphaDisable)
		{
			m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		}


		m_NumPolygons+=2;
	}
	


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::CommitSpriteBatch()
{
	// render
	SetSpriteBlendMode(m_BatchBlendMode);
	if(m_BatchAlphaDisable)
	{
		m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	if(m_LastTexture!=m_BatchTexture)
	{
		m_LastTexture = m_BatchTexture;
		m_Device->SetTexture(0, m_BatchTexture);
	}

	C3DUtils::SetFixedVertexShader(m_Device, D3DFVF_SPRITEVERTEX);
	C3DUtils::SetStreamSource(m_Device, 0, m_SpriteBatchVB, sizeof(SPRITEVERTEX));
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_BatchVertices / 3);

	if(m_BatchAlphaDisable)
	{
		m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}

	m_NumPolygons += m_BatchVertices / 3;

	m_BatchVBBase = 0;
	m_BatchVertices = 0;
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::StartSpriteBatch()
{
	m_SpriteBatchMode = true;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::EndSpriteBatch()
{
	if(!m_SpriteBatchMode) return E_FAIL;

	// ...
	m_SpriteBatchMode = false;
	return CommitSpriteBatch();
}

//////////////////////////////////////////////////////////////////////////
D3DMATRIX* CBRenderD3D::BuildMatrix(D3DMATRIX* pOut, const D3DXVECTOR2* centre, const D3DXVECTOR2* scaling, float angle)
{
	D3DXMATRIX matrices[5];

	D3DXMatrixTranslation(&matrices[0], -centre->x, -centre->y, 0);
	D3DXMatrixScaling(&matrices[1], scaling->x, scaling->y, 1);
	D3DXMatrixIdentity(&matrices[2]);
	D3DXMatrixIdentity(&matrices[3]);
	D3DXMatrixRotationZ(&matrices[2], angle);
	D3DXMatrixTranslation(&matrices[3], centre->x, centre->y, 0);

	matrices[4] = matrices[0] * matrices[1] * matrices[2] * matrices[3];
	*pOut = matrices[4];

	return pOut;
}


//////////////////////////////////////////////////////////////////////////
void CBRenderD3D::TransformVertices(struct SPRITEVERTEX* vertices, const D3DXVECTOR2* pCentre, const D3DXVECTOR2* pScaling, float angle)
{
	D3DXMATRIX matTransf, matVerts, matNew;

	BuildMatrix(&matTransf, pCentre, pScaling, angle);

	int cr;
	for (cr = 0; cr < 4; cr++)
	{
		// 4 vertices fit nicely into a 4x4 matrix --
		// Put each vertex point into a matrix row.
		matVerts(cr, 0) = vertices[cr].p.x;
		matVerts(cr, 1) = vertices[cr].p.y;
		matVerts(cr, 2) = vertices[cr].p.z;
		matVerts(cr, 3) = 1.0f;  // the scaling factor, w
	}

	// Reuse D3D matrix multiplication code to transform our 
	// vertices.
	matNew = matVerts * matTransf;

	for (cr = 0; cr < 4; cr++)
	{
		// Retrieve the newly transformed points.
		vertices[cr].p.x = matNew(cr, 0);
		vertices[cr].p.y = matNew(cr, 1);
		vertices[cr].p.z = matNew(cr, 2);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SetProjection()
{
	D3DXMATRIX matProj;

	float ResWidth, ResHeight;
	float LayerWidth, LayerHeight;
	float ModWidth, ModHeight;
	bool CustomViewport;
	GetProjectionParams(&ResWidth, &ResHeight, &LayerWidth, &LayerHeight, &ModWidth, &ModHeight, &CustomViewport);

	RECT rc;
	Game->GetCurrentViewportRect(&rc);
	float ViewportWidth = (float)rc.right - (float)rc.left;
	float ViewportHeight = (float)rc.bottom - (float)rc.top;

	// margins	
	int mleft = rc.left;
	int mright = ResWidth - ViewportWidth - rc.left;
	int mtop = rc.top;
	int mbottom = ResHeight - ViewportHeight - rc.top;


	D3DXMatrixPerspectiveFovLH(&matProj, m_FOV, ViewportWidth/ViewportHeight, m_NearClipPlane, m_FarClipPlane);

	float ScaleMod = ResHeight / ViewportHeight;
	float ScaleRatio = max(LayerWidth / ResWidth, LayerHeight / ResHeight) /** 1.05*/;

	float OffsetX = (float)Game->m_OffsetX;
	float OffsetY = (float)Game->m_OffsetY;

	if(!CustomViewport)
	{
		OffsetX -= m_DrawOffsetX;
		OffsetY -= m_DrawOffsetY;
	}


	matProj._11 *= ScaleRatio * ScaleMod;
	matProj._22 *= ScaleRatio * ScaleMod;
	matProj._31 = -(OffsetX + (mleft - mright)/2 - ModWidth) / ViewportWidth * 2.0f;	
	matProj._32 =  (OffsetY + (mtop - mbottom)/2 - ModHeight) / ViewportHeight * 2.0f;

	return m_Device->SetTransform(D3DTS_PROJECTION, &matProj);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::GetProjectionParams(float* ResWidth, float* ResHeight, float* LayerWidth, float* LayerHeight, float* ModWidth, float* ModHeight, bool* CustomViewport)
{
	*ResWidth = m_Width;
	*ResHeight = m_Height;
	if(Game->m_EditorResolutionWidth>0) *ResWidth = Game->m_EditorResolutionWidth;
	if(Game->m_EditorResolutionHeight>0) *ResHeight = Game->m_EditorResolutionHeight;

	int LWidth, LHeight;
	RECT SceneViewport;
	Game->GetLayerSize(&LWidth, &LHeight, &SceneViewport, CustomViewport);
	*LayerWidth = (float)LWidth;
	*LayerHeight = (float)LHeight;


	*ModWidth = 0.0f;
	*ModHeight = 0.0f;
	if(*LayerWidth > *ResWidth)   *ModWidth  = (*LayerWidth - *ResWidth) / 2.0f;
	if(*LayerHeight > *ResHeight) *ModHeight = (*LayerHeight - *ResHeight) / 2.0f;



	// new in 1.7.2.1
	// if layer height is smaller than resolution, we assume that we don't want to scroll
	// and that the camera overviews the entire resolution	
	if(*LayerHeight < *ResHeight)
	{
		*ModHeight -= (*ResHeight - *LayerHeight) / 2;
		*LayerHeight = *ResHeight;		
	}
	/*
	if(*LayerWidth < *ResWidth)
	{
	*ModWidth -= (*ResWidth - *LayerWidth) / 2;
	*LayerWidth = *ResWidth;		
	}
	*/

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::DrawLine(int X1, int Y1, int X2, int Y2, DWORD Color)
{
	m_LineVBBase += m_LineVBFlush;

	if(m_LineVBBase >= m_LineVBDiscard)
		m_LineVBBase = 0;

	X1 += m_DrawOffsetX;
	X2 += m_DrawOffsetX;
	Y1 += m_DrawOffsetY;
	Y2 += m_DrawOffsetY;

	LINEVERTEX* v;	
	m_LineVB->LockVB(m_LineVBBase*sizeof(LINEVERTEX), m_LineVBFlush * sizeof(LINEVERTEX), &v, m_LineVBBase?D3DLOCK_NOOVERWRITE:D3DLOCK_DISCARD);

	// position
	v[0].p = D3DXVECTOR4(X1, Y1, 0.9f, 1.0f);
	v[1].p = D3DXVECTOR4(X2, Y2, 0.9f, 1.0f);

	// color
	v[0].color = v[1].color = Color;

	m_LineVB->Unlock();

	// render
	C3DUtils::SetFixedVertexShader(m_Device, D3DFVF_LINEVERTEX);
	C3DUtils::SetStreamSource(m_Device, 0, m_LineVB, sizeof(LINEVERTEX));
    m_Device->DrawPrimitive(D3DPT_LINELIST, m_LineVBBase, 2);
		
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::WindowedBlt(){
	return Flip();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::Fade(WORD Alpha)
{
	DWORD dwAlpha = 255 - Alpha;
	return FadeToColor(dwAlpha<<24);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::FadeToColor(DWORD Color, RECT* rect)
{
	/*
	// solid color?
	if(D3DCOLGetA(Color)==0xFF)
		return Fill(D3DCOLGetR(Color), D3DCOLGetG(Color), D3DCOLGetB(Color), rect);
	*/


    LINEVERTEX* v;
/*
    D3DVIEWPORT8 vp;
    m_Device->GetViewport(&vp);
    FLOAT sx = (FLOAT)vp.Width;
    FLOAT sy = (FLOAT)vp.Height;
*/
	FLOAT left, right, bottom, top;
	if(rect){
		left = (FLOAT)rect->left;
		right = (FLOAT)rect->right;
		bottom = (FLOAT)rect->bottom;
		top = (FLOAT)rect->top;
	}
	else{
		RECT rc;
		Game->GetCurrentViewportRect(&rc);
		left = (FLOAT)rc.left;
		right = (FLOAT)rc.right;
		bottom = (FLOAT)rc.bottom;
		top = (FLOAT)rc.top;
	}

	m_LineVBBase += m_LineVBFlush;

	if(m_LineVBBase >= m_LineVBDiscard)
		m_LineVBBase = 0;

	m_LineVB->LockVB(m_LineVBBase*sizeof(LINEVERTEX), m_LineVBFlush * sizeof(LINEVERTEX), &v, m_LineVBBase?D3DLOCK_NOOVERWRITE:D3DLOCK_DISCARD);
    v[0].color = v[1].color = v[2].color = v[3].color = Color;

    v[0].p = D3DXVECTOR4(left,  bottom, 0.0f, 1.0f);
    v[1].p = D3DXVECTOR4(left,  top,    0.0f, 1.0f);
    v[2].p = D3DXVECTOR4(right, bottom, 0.0f, 1.0f);
    v[3].p = D3DXVECTOR4(right, top,    0.0f, 1.0f);

    m_LineVB->Unlock();

    // Set renderstates
    m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	SetSpriteBlendMode(BLEND_NORMAL);

	m_Device->SetRenderState(D3DRS_ZENABLE,   FALSE);
    m_Device->SetTexture(0, NULL);
	m_LastTexture = NULL;

    // Draw a big, colored square
	C3DUtils::SetFixedVertexShader(m_Device, D3DFVF_LINEVERTEX);
	C3DUtils::SetStreamSource(m_Device, 0, m_LineVB, sizeof(LINEVERTEX));
	
	m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, m_LineVBBase, 2);
	m_NumPolygons+=2;

	Setup2D(true);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::RestoreDeviceObjects()
{
	HRESULT ret;
	if(FAILED(ret = m_Device->CreateVertexBufferUni(m_VBDiscard*sizeof(SPRITEVERTEX),
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
													D3DFVF_SPRITEVERTEX,
													D3DPOOL_DEFAULT,
													&m_SpriteVB)))
	{
		Game->LOG(ret, "Error creating vertex buffer for sprite operations.");
		return ret;
	}

	if(FAILED(ret = m_Device->CreateVertexBufferUni(m_BatchVBDiscard*sizeof(SPRITEVERTEX),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFVF_SPRITEVERTEX,
		D3DPOOL_DEFAULT,
		&m_SpriteBatchVB)))
	{
		Game->LOG(ret, "Error creating vertex buffer for sprite batch operations.");
		return ret;
	}

	// create vertex buffer for lines
	if(FAILED(ret = m_Device->CreateVertexBufferUni(m_LineVBDiscard*sizeof(LINEVERTEX),
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
													D3DFVF_LINEVERTEX,
													D3DPOOL_DEFAULT,
													&m_LineVB)))
	{

		Game->LOG(ret, "Error creating vertex buffer for line operations.");
		return ret;
	}


	CreateShaderQuad();

	if(m_ShadowGen) m_ShadowGen->RestoreDeviceObjects();
	Game->RestoreDeviceObjects();
		
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::CreateShaderQuad()
{
	HRESULT ret;

	if(FAILED(ret = m_Device->CreateVertexBufferUni(4*sizeof(SPRITEVERTEX),
		D3DUSAGE_WRITEONLY,
		D3DFVF_SPRITEVERTEX,
		D3DPOOL_DEFAULT,
		&m_ShaderVB)))
	{

		Game->LOG(ret, "Error creating vertex buffer for shader quads.");
		return ret;
	}

	SPRITEVERTEX* v;
	if (SUCCEEDED(m_ShaderVB->LockVB(0, 0, &v, 0)))
	{
		v[0].p = D3DXVECTOR4(m_DrawOffsetX,           m_DrawOffsetY + m_Height, 0.0f, 1.0f); // bottom left
		v[1].p = D3DXVECTOR4(m_DrawOffsetX,           m_DrawOffsetY,            0.0f, 1.0f); // top left
		v[2].p = D3DXVECTOR4(m_DrawOffsetX + m_Width, m_DrawOffsetY + m_Height, 0.0f, 1.0f); // bottom right
		v[3].p = D3DXVECTOR4(m_DrawOffsetX + m_Width, m_DrawOffsetY,            0.0f, 1.0f); // top right

		v[0].tu = 0.0; v[0].tv = 1.0; // bottom left
		v[1].tu = 0.0; v[1].tv = 0.0; // top left
		v[2].tu = 1.0; v[2].tv = 1.0; // bottom right
		v[3].tu = 1.0; v[3].tv = 0.0; // top right


		v[0].color = 0xFFFFFFFF;
		v[1].color = 0xFFFFFFFF;
		v[2].color = 0xFFFFFFFF;
		v[3].color = 0xFFFFFFFF;

		m_ShaderVB->Unlock();
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::DrawShaderQuad()
{
	C3DUtils::SetFixedVertexShader(m_Device, D3DFVF_SPRITEVERTEX);
	C3DUtils::SetStreamSource(m_Device, 0, m_ShaderVB, sizeof(SPRITEVERTEX));
	return m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::InvalidateDeviceObjects()
{
	RELEASE(m_SpriteVB);
	RELEASE(m_SpriteBatchVB);
	RELEASE(m_LineVB);
	RELEASE(m_ShaderVB);
	if(m_ShadowGen) m_ShadowGen->InvalidateDeviceObjects();
	Game->InvalidateDeviceObjects();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SetViewport(int left, int top, int right, int bottom)
{
	/*
	left += m_DrawOffsetX;
	right += m_DrawOffsetX;

	bottom += m_DrawOffsetY;
	top += m_DrawOffsetY;
	*/

	D3DVIEWPORT vp;
	vp.X = left;
	vp.Y = top;
	vp.Width = right - left;
	vp.Height = bottom - top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	
	return m_Device->SetViewport(&vp);
}


//////////////////////////////////////////////////////////////////////////
CBImage* CBRenderD3D::TakeScreenshot()
{
	CBImage* Image = new CBImage(Game);
	if(!Image || FAILED(Image->LoadFromD3DDevice(m_Device, m_RealWidth, m_RealHeight))){
		delete Image;
		return NULL;
	}
	else return Image;
}

//////////////////////////////////////////////////////////////////////////
bool CBRenderD3D::UsingStencilBuffer()
{
	return m_UsingStencil;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::EnableShadows()
{
	if(!m_ShadowGen)
	{
		m_ShadowGen = new C3DShadowGen(Game);
	}
	return m_ShadowGen?S_OK:E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::DisableShadows()
{
	SAFE_DELETE(m_ShadowGen);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::DisplayShadow(CBObject* Object, D3DXVECTOR3* LightPos, D3DXVECTOR3* LightTarget, bool LightPosRelative)
{
	if(m_ShadowGen && (Game->m_MaxShadowType > SHADOW_NONE))
		return m_ShadowGen->Render(Object, LightPos, LightTarget, LightPosRelative);
	else return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CBRenderD3D::DumpData(char* Filename)
{
	FILE* f = fopen(Filename, "wt");
	if(!f) return;

	CBSurfaceStorage* Mgr = Game->m_SurfaceStorage;

	int TotalKB = 0;
	int TotalLoss = 0;
	fprintf(f, "Filename;Usage;Size;Real size;KBytes;Loss\n");
	for(int i=0; i<Mgr->m_Surfaces.GetSize(); i++)
	{
		CBSurfaceD3D* Surf = (CBSurfaceD3D*)Mgr->m_Surfaces[i];
		if(!Surf->m_Filename) continue;
		if(!Surf->m_Valid) continue;

		Surf->m_Texture->GetLevelDesc(0, &Surf->m_SurfaceDesc);
		
		fprintf(f, "%s;%d;", Surf->m_Filename, Surf->m_ReferenceCount);
		fprintf(f, "%dx%d;", Surf->GetWidth(), Surf->GetHeight());
		fprintf(f, "%dx%d;", Surf->m_SurfaceDesc.Width, Surf->m_SurfaceDesc.Height);
		
		int kb = Surf->m_SurfaceDesc.Width * Surf->m_SurfaceDesc.Height * 4 / 1024;
		int loss = (Surf->m_SurfaceDesc.Width * Surf->m_SurfaceDesc.Height - Surf->GetWidth() * Surf->GetHeight()) * 4 / 1024;
		TotalKB+=kb;
		TotalLoss+=loss;
		fprintf(f, "%d;", kb);
		fprintf(f, "%d;", loss);
		fprintf(f, "\n");
	}
	fprintf(f, "Total %d;;;;%d;%d\n", Mgr->m_Surfaces.GetSize(), TotalKB, TotalLoss);


	fclose(f);
	Game->LOG(0, "Texture Stats Dump completed.");
	Game->QuickMessage("Texture Stats Dump completed.");
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SetSpriteBlendMode(TSpriteBlendMode Mode, bool ForceChange)
{
	if(Mode==m_BlendMode && !ForceChange) return S_OK;


	m_BlendMode = Mode;
	if(m_BlendMode==BLEND_NORMAL)
	{
		m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else if(m_BlendMode==BLEND_ADDITIVE)
	{
		m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else if(m_BlendMode==BLEND_SUBTRACTIVE)
	{
		m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
		m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	}

	// mult?
	//m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
	//m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::DisplayDebugInfo()
{
	char str[100];
	sprintf(str, "PolyCount: %d", m_NumPolygons);
	Game->m_SystemFont->DrawText((BYTE*)str, 0, 20, m_Width, TAL_RIGHT);
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CBRenderD3D::StencilSupported()
{
	return m_UsingStencil;
}


//////////////////////////////////////////////////////////////////////////
const char* CBRenderD3D::GetName()
{
#ifdef WME_D3D9
	if(m_HardwareTL) return "Direct3D9, HW T&L";
	else return "Direct3D9";
#else
	if(m_HardwareTL) return "Direct3D8, HW T&L";
	else return "Direct3D8";
#endif
}

//////////////////////////////////////////////////////////////////////////
int CBRenderD3D::GetMaxActiveLights()
{
	int MaxLights = 0;

	D3DCAPS DevCaps;
	memset(&DevCaps, 0, sizeof(D3DCAPS));
	m_Device->GetDeviceCaps(&DevCaps);
	
	MaxLights = (int)DevCaps.MaxActiveLights;
	if(MaxLights==-1) MaxLights = 100;

	return MaxLights;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::InvalidateTexture(LPDIRECT3DTEXTURE Texture)
{
	if(m_LastTexture == Texture) m_LastTexture = NULL;
	if(m_BatchTexture == Texture) m_BatchTexture = NULL;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SetAmbientLightColor(DWORD Color)
{
	m_AmbientLightColor = Color;
	m_AmbientLightOverride = true;

	return SetAmbientLightRenderState();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderD3D::SetDefaultAmbientLightColor()
{
	m_AmbientLightColor = 0x00000000;
	m_AmbientLightOverride = false;

	return SetAmbientLightRenderState();
}
