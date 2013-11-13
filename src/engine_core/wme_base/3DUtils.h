// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

class C3DUtils
{
public:
	static BOOL IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v);
	static D3DXMATRIX* MatrixSetTranslation(D3DXMATRIX* mat, D3DXVECTOR3* vec);
	static D3DXMATRIX* MatrixSetRotation(D3DXMATRIX* mat, D3DXVECTOR3* vec);

	static D3DXVECTOR3 PickMakeRay(float FOV, LPDIRECT3DDEVICE Device, int MouseX, int MouseY, float Range);
	static bool PickGetIntersect (D3DXVECTOR3 linestart, D3DXVECTOR3 lineend, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2,D3DXVECTOR3* intersection, float* distance);

	static void DecomposeMatrixSimple(const D3DXMATRIX* Mat, D3DXVECTOR3* TransVec, D3DXVECTOR3* ScaleVec, D3DXQUATERNION* RotQ);
	static void DecomposeMatrix(const D3DXMATRIX* Mat, D3DXVECTOR3* TransVec, D3DXVECTOR3* ScaleVec, D3DXQUATERNION* RotQ);

	// DX8 -> DX9 bridge
	static HRESULT SetFixedVertexShader(LPDIRECT3DDEVICE Device, DWORD FVF);
	static HRESULT SetStreamSource(LPDIRECT3DDEVICE Device, UINT StreamNumber, LPDIRECT3DVERTEXBUFFER pStreamData, UINT Stride);
	static HRESULT SetIndices(LPDIRECT3DDEVICE Device, LPDIRECT3DINDEXBUFFER pIndexData);
	static HRESULT CreateDepthStencilSurface(LPDIRECT3DDEVICE Device, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, LPDIRECT3DSURFACE* ppSurface);
	static HRESULT GetRenderTarget(LPDIRECT3DDEVICE Device, LPDIRECT3DSURFACE* ppRenderTarget, LPDIRECT3DSURFACE* ppZStencilSurface);
	static HRESULT SetRenderTarget(LPDIRECT3DDEVICE Device, LPDIRECT3DSURFACE pRenderTarget, LPDIRECT3DSURFACE pZStencilSurface);
	static HRESULT SetTextureStageState(LPDIRECT3DDEVICE Device, DWORD Stage, DWORD State, DWORD Value);
	static HRESULT CreateImageSurface(LPDIRECT3DDEVICE Device, UINT Width, UINT Height, D3DFORMAT Format, LPDIRECT3DSURFACE* ppSurface);
	static HRESULT GetFrontBuffer(LPDIRECT3DDEVICE Device, LPDIRECT3DSURFACE pDestSurface);
	static UINT GetAdapterModeCount(LPDIRECT3D d3d, UINT Adapter, D3DFORMAT Format);
	static HRESULT EnumAdapterModes(LPDIRECT3D d3d, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode);
	static HRESULT DrawIndexedPrimitive(LPDIRECT3DDEVICE Device, D3DPRIMITIVETYPE Type, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);

private:
	static D3DXVECTOR3* V3Combine (const D3DXVECTOR3* a, const D3DXVECTOR3* b, D3DXVECTOR3* result, double ascl, double bscl);
};
