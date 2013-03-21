// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\3DUtils.h"


//////////////////////////////////////////////////////////////////////////
BOOL C3DUtils::IntersectTriangle(const D3DXVECTOR3& orig,
					   const D3DXVECTOR3& dir,
					   D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
					   FLOAT* t, FLOAT* u, FLOAT* v)
{

	// Find vectors for two edges sharing vert0
	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	D3DXVECTOR3 pvec;
	D3DXVec3Cross( &pvec, &dir, &edge2 );

	// If determinant is near zero, ray lies in plane of triangle
	FLOAT det = D3DXVec3Dot( &edge1, &pvec );
	if( det < 0.0001f )
		return FALSE;

	// Calculate distance from vert0 to ray origin
	D3DXVECTOR3 tvec = orig - v0;

	// Calculate U parameter and test bounds
	*u = D3DXVec3Dot( &tvec, &pvec );
	if( *u < 0.0f || *u > det )
		return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross( &qvec, &tvec, &edge1 );

	// Calculate V parameter and test bounds
	*v = D3DXVec3Dot( &dir, &qvec );
	if( *v < 0.0f || *u + *v > det )
		return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot( &edge2, &qvec );

	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	D3DXVECTOR3 intersection;
	D3DXVECTOR3 dest = orig + dir;
	D3DXPLANE plane;
	D3DXPlaneFromPoints(&plane, &v0, &v1, &v2);
	D3DXPlaneIntersectLine(&intersection, &plane, &orig, &dest);
	*t = intersection.x;
	*u = intersection.y;
	*v = intersection.z;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
D3DXMATRIX* C3DUtils::MatrixSetTranslation(D3DXMATRIX* mat, D3DXVECTOR3* vec)
{
	mat->_41 = vec->x;
	mat->_42 = vec->y;
	mat->_43 = vec->z;

	return mat;
}


//////////////////////////////////////////////////////////////////////////
D3DXMATRIX* C3DUtils::MatrixSetRotation(D3DXMATRIX* mat, D3DXVECTOR3* vec)
{
	double cr = cos(vec->x);
	double sr = sin(vec->x);
	double cp = cos(vec->y);
	double sp = sin(vec->y);
	double cy = cos(vec->z);
	double sy = sin(vec->z);

	mat->_11 = (float)(cp*cy);
	mat->_12 = (float)(cp*sy);
	mat->_13 = (float)(-sp);

	double srsp = sr*sp;
	double crsp = cr*sp;

	mat->_21 = (float)(srsp*cy-cr*sy);
	mat->_22 = (float)(srsp*sy+cr*cy);
	mat->_23 = (float)(sr*cp);

	mat->_31 = (float)(crsp*cy+sr*sy);
	mat->_32 = (float)(crsp*sy-sr*cy);
	mat->_33 = (float)(cr*cp);


	return mat;
}


//////////////////////////////////////////////////////////////////////////
// object picking
//////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 C3DUtils::PickMakeRay(float FOV, LPDIRECT3DDEVICE Device, int MouseX, int MouseY, float Range)
{
	D3DXVECTOR3 LineEnd,CameraSpacePos;
	float NMouseX, NMouseY, det;
	D3DXMATRIX matView;

	D3DVIEWPORT viewport;
	Device->GetViewport(&viewport);


	NMouseX = 1.0 - 2.0*MouseX/viewport.Width;
	NMouseY = 1.0 - 2.0*MouseY/viewport.Height;

	float AspectRatio = viewport.Width / viewport.Height;

	//is FOV in radians
	CameraSpacePos.y = (NMouseY*tan(FOV/2.0));
	CameraSpacePos.x = ((-NMouseX/AspectRatio)*tan(FOV/2.0));

	LineEnd.x = Range*CameraSpacePos.x;
	LineEnd.y = Range*CameraSpacePos.y;
	LineEnd.z = Range;

	Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView,&det,&matView);
	D3DXVec3TransformCoord(&LineEnd,&LineEnd,&matView);


	return LineEnd;
}

//////////////////////////////////////////////////////////////////////////
bool C3DUtils::PickGetIntersect (D3DXVECTOR3 linestart, D3DXVECTOR3 lineend, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2,D3DXVECTOR3* intersection, float* distance)
{
	// compute plane's normal
	D3DXVECTOR3 vertex;
	D3DXVECTOR3 normal;

	D3DXVECTOR3 edge1 = v1 - v0;
	D3DXVECTOR3 edge2 = v2 - v1;

	D3DXVec3Cross(&normal, &edge1, &edge2);
	D3DXVec3Normalize(&normal, &normal);

	vertex = v0;


	D3DXVECTOR3 direction,L1;
	float linelength,dist_from_plane,percentage;

	direction.x=lineend.x-linestart.x;// calculate the lines direction vector
	direction.y=lineend.y-linestart.y;
	direction.z=lineend.z-linestart.z;

	linelength=D3DXVec3Dot(&direction,&normal); // This gives us the line length (the blue dot L3 + L4 in figure d)

	if (fabsf(linelength) < 0.00001) return false;


	L1.x=vertex.x-linestart.x;// calculate vector L1 (the PINK line in figure d)
	L1.y=vertex.y-linestart.y;
	L1.z=vertex.z-linestart.z;

	dist_from_plane=D3DXVec3Dot(&L1,&normal); // gives the distance from the plane (ORANGE Line L3 in figure d)
	percentage=dist_from_plane/linelength; // How far from Linestart , intersection is as a percentage of 0 to 1 
	if (percentage< 0.0) return false;
	else if (percentage > 1.0) return false;

	*distance=percentage;//record the distance from beginning of ray (0.0 -1.0)

	intersection->x=linestart.x+direction.x*percentage;// add the percentage of the line to line start
	intersection->y=linestart.y+direction.y*percentage;
	intersection->z=linestart.z+direction.z*percentage;

	return true;
}


//////////////////////////////////////////////////////////////////////////
void C3DUtils::DecomposeMatrixSimple(const D3DXMATRIX* Mat, D3DXVECTOR3* TransVec, D3DXVECTOR3* ScaleVec, D3DXQUATERNION* RotQ)
{
	*TransVec = D3DXVECTOR3(Mat->_41, Mat->_42, Mat->_43);
	*ScaleVec = D3DXVECTOR3(sqrtf(Mat->_11*Mat->_11+Mat->_21*Mat->_21+Mat->_31*Mat->_31),
		sqrtf(Mat->_12*Mat->_12+Mat->_22*Mat->_22+Mat->_32*Mat->_32),
		sqrtf(Mat->_13*Mat->_13+Mat->_23*Mat->_23+Mat->_33*Mat->_33));

	D3DXQUATERNION Q;
	D3DXQuaternionRotationMatrix(&Q, Mat);

	*RotQ = Q;
}


//////////////////////////////////////////////////////////////////////////
void C3DUtils::DecomposeMatrix(const D3DXMATRIX* Mat, D3DXVECTOR3* TransVec, D3DXVECTOR3* ScaleVec, D3DXQUATERNION* RotQ)
{
	enum unmatrix_indices
	{
		U_SCALEX,
		U_SCALEY,
		U_SCALEZ,
		U_SHEARXY,
		U_SHEARXZ,
		U_SHEARYZ,
		U_ROTATEX,
		U_ROTATEY,
		U_ROTATEZ,
		U_TRANSX,
		U_TRANSY,
		U_TRANSZ,
		U_PX,
		U_PY,
		U_PZ,
		U_PW
	};


	double unMatrixValues[16];

	register int i,j; 
	D3DXMATRIX locmat; 
	D3DXMATRIX pmat, invpmat, tinvpmat; 
	/* Vector4 type and functions need to be added to the common set. */ 
	D3DXVECTOR4 prhs, psol; 
	D3DXVECTOR3 row[3], pdum3; 
	locmat = *Mat; 

	/* Normalize the matrix. */ 
	if ( locmat.m[3][3] == 0 ) 
		return; 

	for ( i=0; i<4;i++ ) 
		for ( j=0; j<4; j++ ) 
			locmat.m[i][j] /= locmat.m[3][3]; 
	/* pmat is used to solve for perspective, but it also provides 
	* an easy way to test for singularity of the upper 3x3 component. 
	*/ 
	pmat = locmat; 
	for ( i=0; i<3; i++ ) 
		pmat.m[i][3] = 0; 
	pmat.m[3][3] = 1; 

	if ( D3DXMatrixDeterminant(&pmat) == 0.0 ) 
		return; 

	/* First, isolate perspective.  This is the messiest. */ 
	if ( locmat.m[0][3] != 0 || locmat.m[1][3] != 0 || locmat.m[2][3] != 0 )
	{ 
		/* prhs is the right hand side of the equation. */ 
		prhs.x = locmat.m[0][3]; 
		prhs.y = locmat.m[1][3]; 
		prhs.z = locmat.m[2][3]; 
		prhs.w = locmat.m[3][3]; 

		/* Solve the equation by inverting pmat and multiplying 
		* prhs by the inverse.  (This is the easiest way, not 
		* necessarily the best.) 
		* inverse function (and det4x4, above) from the Matrix 
		* Inversion gem in the first volume. 
		*/ 
		//inverse( &pmat, &invpmat ); 
		FLOAT det; 
		D3DXMatrixInverse(&invpmat,&det,&pmat); 
		//TransposeMatrix4( &invpmat, &tinvpmat ); 
		D3DXMatrixTranspose(&tinvpmat, &invpmat); 
		//V4MulPointByMatrix(&prhs, &tinvpmat, &psol); 
		D3DXVec4Transform(&psol,&prhs,&tinvpmat); 

		/* Stuff the answer away. */ 
		unMatrixValues[U_PX] = psol.x; 
		unMatrixValues[U_PY] = psol.y; 
		unMatrixValues[U_PZ] = psol.z; 
		unMatrixValues[U_PW] = psol.w; 
		/* Clear the perspective partition. */ 
		locmat.m[0][3] = locmat.m[1][3] = locmat.m[2][3] = 0; 
		locmat.m[3][3] = 1; 
	}
	else    /* No perspective. */ 
		unMatrixValues[U_PX] = unMatrixValues[U_PY] = unMatrixValues[U_PZ] = unMatrixValues[U_PW] = 0; 

	/* Next take care of translation (easy). */ 
	for ( i=0; i<3; i++ )
	{ 
		unMatrixValues[U_TRANSX + i] = locmat.m[3][i]; 
		locmat.m[3][i] = 0; 
	} 

	/* Now get scale and shear. */ 
	for ( i=0; i<3; i++ )
	{ 
		row[i].x = locmat.m[i][0]; 
		row[i].y = locmat.m[i][1]; 
		row[i].z = locmat.m[i][2]; 
	} 

	/* Compute X scale factor and normalize first row. */ 
	unMatrixValues[U_SCALEX] = D3DXVec3Length(&row[0]); 
	row[0] = *D3DXVec3Scale(&row[0],&row[0], 1.0); 

	/* Compute XY shear factor and make 2nd row orthogonal to 1st. */ 
	unMatrixValues[U_SHEARXY] = D3DXVec3Dot(&row[0], &row[1]); 
	(void)V3Combine(&row[1], &row[0], &row[1], 1.0, -unMatrixValues[U_SHEARXY]); 

	/* Now, compute Y scale and normalize 2nd row. */ 
	unMatrixValues[U_SCALEY] = D3DXVec3Length(&row[1]); 
	D3DXVec3Scale(&row[1], &row[1], 1.0); 
	unMatrixValues[U_SHEARXY] /= unMatrixValues[U_SCALEY]; 

	/* Compute XZ and YZ shears, orthogonalize 3rd row. */ 
	unMatrixValues[U_SHEARXZ] = D3DXVec3Dot(&row[0], &row[2]); 
	(void)V3Combine(&row[2], &row[0], &row[2], 1.0, -unMatrixValues[U_SHEARXZ]); 
	unMatrixValues[U_SHEARYZ] = D3DXVec3Dot(&row[1], &row[2]); 
	(void)V3Combine(&row[2], &row[1], &row[2], 1.0, -unMatrixValues[U_SHEARYZ]); 

	/* Next, get Z scale and normalize 3rd row. */ 
	unMatrixValues[U_SCALEZ] = D3DXVec3Length(&row[2]); 
	D3DXVec3Scale(&row[2], &row[2], 1.0); 
	unMatrixValues[U_SHEARXZ] /= unMatrixValues[U_SCALEZ]; 
	unMatrixValues[U_SHEARYZ] /= unMatrixValues[U_SCALEZ]; 


	/* At this point, the matrix (in rows[]) is orthonormal. 
	* Check for a coordinate system flip.  If the determinant 
	* is -1, then negate the matrix and the scaling factors. 
	*/ 
	if ( D3DXVec3Dot( &row[0], D3DXVec3Cross(&pdum3, &row[1], &row[2]) ) < 0 ) 
		for ( i = 0; i < 3; i++ ) 
		{ 
			unMatrixValues[U_SCALEX+i] *= -1; 
			row[i].x *= -1; 
			row[i].y *= -1; 
			row[i].z *= -1; 
		} 

	/* Now, get the rotations out, as described in the gem. */ 
	unMatrixValues[U_ROTATEY] = asin(-row[0].z); 
	if ( cos(unMatrixValues[U_ROTATEY]) != 0 )
	{ 
		unMatrixValues[U_ROTATEX] = atan2(row[1].z, row[2].z); 
		unMatrixValues[U_ROTATEZ] = atan2(row[0].y, row[0].x); 
	}
	else
	{ 
		unMatrixValues[U_ROTATEX] = atan2(-row[2].x, row[1].y); 
		unMatrixValues[U_ROTATEZ] = 0; 
	} 
	/* All done! */ 

	*TransVec = D3DXVECTOR3((float)unMatrixValues[U_TRANSX], (float)unMatrixValues[U_TRANSY], (float)unMatrixValues[U_TRANSZ]);
	*ScaleVec = D3DXVECTOR3((float)unMatrixValues[U_SCALEX], (float)unMatrixValues[U_SCALEY], (float)unMatrixValues[U_SCALEZ]);

	D3DXQUATERNION Qtemp;

	D3DXQuaternionRotationAxis(RotQ, &D3DXVECTOR3(1, 0, 0), (float)unMatrixValues[U_ROTATEX]);

	D3DXQuaternionRotationAxis(&Qtemp, &D3DXVECTOR3(0, 1, 0), (float)unMatrixValues[U_ROTATEY]);
	D3DXQuaternionMultiply(RotQ, RotQ, &Qtemp);

	D3DXQuaternionRotationAxis(&Qtemp, &D3DXVECTOR3(0, 0, 1), (float)unMatrixValues[U_ROTATEZ]);
	D3DXQuaternionMultiply(RotQ, RotQ, &Qtemp);
}

//////////////////////////////////////////////////////////////////////////
D3DXVECTOR3* C3DUtils::V3Combine (const D3DXVECTOR3* a, const D3DXVECTOR3* b, D3DXVECTOR3* result, double ascl, double bscl)
{ 
	result->x = (float)((ascl * a->x) + (bscl * b->x)); 
	result->y = (float)((ascl * a->y) + (bscl * b->y)); 
	result->z = (float)((ascl * a->z) + (bscl * b->z)); 
	return(result); 
} 

//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::SetFixedVertexShader(LPDIRECT3DDEVICE Device, DWORD FVF)
{
#ifdef WME_D3D9
	Device->SetVertexShader(NULL);
	return Device->SetFVF(FVF);
#else
	return Device->SetVertexShader(FVF);
#endif
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::SetStreamSource(LPDIRECT3DDEVICE Device, UINT StreamNumber, LPDIRECT3DVERTEXBUFFER pStreamData, UINT Stride)
{
#ifdef WME_D3D9
	return Device->SetStreamSource(StreamNumber, pStreamData, 0, Stride);
#else
	return Device->SetStreamSource(StreamNumber, pStreamData, Stride);
#endif
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::SetIndices(LPDIRECT3DDEVICE Device, LPDIRECT3DINDEXBUFFER pIndexData)
{
#ifdef WME_D3D9
	return Device->SetIndices(pIndexData);
#else
	return Device->SetIndices(pIndexData, 0);
#endif
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::CreateDepthStencilSurface(LPDIRECT3DDEVICE Device, UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, LPDIRECT3DSURFACE* ppSurface)
{
#ifdef WME_D3D9
	return Device->CreateDepthStencilSurface(Width, Height, Format, MultiSample, 0, FALSE, ppSurface, NULL);
#else
	return Device->CreateDepthStencilSurface(Width, Height, Format, MultiSample, ppSurface);
#endif
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::GetRenderTarget(LPDIRECT3DDEVICE Device, LPDIRECT3DSURFACE* ppRenderTarget, LPDIRECT3DSURFACE* ppZStencilSurface)
{
#ifdef WME_D3D9
	Device->GetRenderTarget(0, ppRenderTarget);
	return Device->GetDepthStencilSurface(ppZStencilSurface);
#else
	Device->GetRenderTarget(ppRenderTarget);
	return Device->GetDepthStencilSurface(ppZStencilSurface);
#endif

}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::SetRenderTarget(LPDIRECT3DDEVICE Device, LPDIRECT3DSURFACE pRenderTarget, LPDIRECT3DSURFACE pZStencilSurface)
{
#ifdef WME_D3D9
	Device->SetRenderTarget(0, pRenderTarget);
	return Device->SetDepthStencilSurface(pZStencilSurface);
#else
	return Device->SetRenderTarget(pRenderTarget, pZStencilSurface);
#endif
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::SetTextureStageState(LPDIRECT3DDEVICE Device, DWORD Stage, DWORD State, DWORD Value)
{
#ifdef WME_D3D9
	// some state which was here got pushed into sampler state in D3D9
	switch (State)
	{
		case D3DTSS_ADDRESSU:
			return Device->SetSamplerState(Stage, D3DSAMP_ADDRESSU, Value);
		case D3DTSS_ADDRESSV:
			return Device->SetSamplerState(Stage, D3DSAMP_ADDRESSV, Value);
		case D3DTSS_BORDERCOLOR:
			return Device->SetSamplerState(Stage, D3DSAMP_BORDERCOLOR, Value);
		case D3DTSS_MAGFILTER:
			return Device->SetSamplerState(Stage, D3DSAMP_MAGFILTER, Value);
		case D3DTSS_MINFILTER:
			return Device->SetSamplerState(Stage, D3DSAMP_MINFILTER, Value);
		case D3DTSS_MIPFILTER:
			return Device->SetSamplerState(Stage, D3DSAMP_MIPFILTER, Value);
		case D3DTSS_MIPMAPLODBIAS:
			return Device->SetSamplerState(Stage, D3DSAMP_MIPMAPLODBIAS, Value);
		case D3DTSS_MAXMIPLEVEL:
			return Device->SetSamplerState(Stage, D3DSAMP_MAXMIPLEVEL, Value);
		case D3DTSS_MAXANISOTROPY:
			return Device->SetSamplerState(Stage, D3DSAMP_MAXANISOTROPY, Value);
		default:
			return Device->SetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)State, Value);
	}
#else
	return Device->SetTextureStageState(Stage, (D3DTEXTURESTAGESTATETYPE)State, Value);
#endif
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::CreateImageSurface(LPDIRECT3DDEVICE Device, UINT Width, UINT Height, D3DFORMAT Format, LPDIRECT3DSURFACE* ppSurface)
{
#ifdef WME_D3D9
	return Device->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_SCRATCH, ppSurface, NULL); 
#else
	return Device->CreateImageSurface(Width, Height, Format, ppSurface);
#endif
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::GetFrontBuffer(LPDIRECT3DDEVICE Device, LPDIRECT3DSURFACE pDestSurface)
{
#ifdef WME_D3D9
	return Device->GetFrontBufferData(0, pDestSurface);
#else
	return Device->GetFrontBuffer(pDestSurface);
#endif
}

//////////////////////////////////////////////////////////////////////////
UINT C3DUtils::GetAdapterModeCount(LPDIRECT3D d3d, UINT Adapter, D3DFORMAT Format)
{
#ifdef WME_D3D9
	return d3d->GetAdapterModeCount(Adapter, Format);
#else
	return d3d->GetAdapterModeCount(Adapter);
#endif
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::EnumAdapterModes(LPDIRECT3D d3d, UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
#ifdef WME_D3D9
	return d3d->EnumAdapterModes(Adapter, Format, Mode, pMode);
#else
	return d3d->EnumAdapterModes(Adapter, Mode, pMode);
#endif
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DUtils::DrawIndexedPrimitive(LPDIRECT3DDEVICE Device, D3DPRIMITIVETYPE Type, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
{
#ifdef WME_D3D9
	return Device->DrawIndexedPrimitive(Type, 0, MinIndex, NumVertices, StartIndex, PrimitiveCount);
#else
	return Device->DrawIndexedPrimitive(Type, MinIndex, NumVertices, StartIndex, PrimitiveCount);
#endif
}
