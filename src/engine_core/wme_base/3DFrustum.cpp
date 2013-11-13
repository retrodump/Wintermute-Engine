// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DFrustum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
C3DFrustum::C3DFrustum(CBGame* inGame):CBBase(inGame)
{

}


//////////////////////////////////////////////////////////////////////////
C3DFrustum::~C3DFrustum()
{

}


//////////////////////////////////////////////////////////////////////////
void C3DFrustum::CalcualteFrustum()
{
	D3DXMATRIX matView, matProj, matClip;

	CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;

	rend->m_Device->GetTransform(D3DTS_VIEW, &matView);
	rend->m_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixMultiply(&matClip, &matView, &matProj);

	// right
	m_Planes[RIGHT].a = matClip._14 - matClip._11;
	m_Planes[RIGHT].b = matClip._24 - matClip._21;
	m_Planes[RIGHT].c = matClip._34 - matClip._31;
	m_Planes[RIGHT].d = matClip._44 - matClip._41;
	D3DXPlaneNormalize(&m_Planes[RIGHT], &m_Planes[RIGHT]);

	// left
	m_Planes[LEFT].a = matClip._14 + matClip._11;
	m_Planes[LEFT].b = matClip._24 + matClip._21;
	m_Planes[LEFT].c = matClip._34 + matClip._31;
	m_Planes[LEFT].d = matClip._44 + matClip._41;
	D3DXPlaneNormalize(&m_Planes[LEFT], &m_Planes[LEFT]);

	// bottom
	m_Planes[BOTTOM].a = matClip._14 + matClip._12;
	m_Planes[BOTTOM].b = matClip._24 + matClip._22;
	m_Planes[BOTTOM].c = matClip._34 + matClip._32;
	m_Planes[BOTTOM].d = matClip._44 + matClip._42;
	D3DXPlaneNormalize(&m_Planes[BOTTOM], &m_Planes[BOTTOM]);

	// top
	m_Planes[TOP].a = matClip._14 - matClip._12;
	m_Planes[TOP].b = matClip._24 - matClip._22;
	m_Planes[TOP].c = matClip._34 - matClip._32;
	m_Planes[TOP].d = matClip._44 - matClip._42;
	D3DXPlaneNormalize(&m_Planes[TOP], &m_Planes[TOP]);

	// back
	m_Planes[BACK].a = matClip._14 - matClip._13;
	m_Planes[BACK].b = matClip._24 - matClip._23;
	m_Planes[BACK].c = matClip._34 - matClip._33;
	m_Planes[BACK].d = matClip._44 - matClip._43;
	D3DXPlaneNormalize(&m_Planes[BACK], &m_Planes[BACK]);

	// front
	m_Planes[FRONT].a = matClip._14 + matClip._13;
	m_Planes[FRONT].b = matClip._24 + matClip._23;
	m_Planes[FRONT].c = matClip._34 + matClip._33;
	m_Planes[FRONT].d = matClip._44 + matClip._43;
	D3DXPlaneNormalize(&m_Planes[FRONT], &m_Planes[FRONT]);
}


//////////////////////////////////////////////////////////////////////////
bool C3DFrustum::PointInFrustum(float x, float y, float z)
{
	for(int i = 0; i < 6; i++ ){
		if(m_Planes[i].a * x + m_Planes[i].b * y + m_Planes[i].c * z + m_Planes[i].d <= 0){
			return false;
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool C3DFrustum::SphereInFrustum(float x, float y, float z, float radius)
{
	for(int i = 0; i < 6; i++){
		if(m_Planes[i].a * x + m_Planes[i].b * y + m_Planes[i].c * z + m_Planes[i].d <= -radius){
			return false;
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool C3DFrustum::CubeInFrustum(float x, float y, float z, float size)
{
	for(int i = 0; i < 6; i++){
		if(m_Planes[i].a * (x - size) + m_Planes[i].b * (y - size) + m_Planes[i].c * (z - size) + m_Planes[i].d > 0)
		   continue;
		if(m_Planes[i].a * (x + size) + m_Planes[i].b * (y - size) + m_Planes[i].c * (z - size) + m_Planes[i].d > 0)
		   continue;
		if(m_Planes[i].a * (x - size) + m_Planes[i].b * (y + size) + m_Planes[i].c * (z - size) + m_Planes[i].d > 0)
		   continue;
		if(m_Planes[i].a * (x + size) + m_Planes[i].b * (y + size) + m_Planes[i].c * (z - size) + m_Planes[i].d > 0)
		   continue;
		if(m_Planes[i].a * (x - size) + m_Planes[i].b * (y - size) + m_Planes[i].c * (z + size) + m_Planes[i].d > 0)
		   continue;
		if(m_Planes[i].a * (x + size) + m_Planes[i].b * (y - size) + m_Planes[i].c * (z + size) + m_Planes[i].d > 0)
		   continue;
		if(m_Planes[i].a * (x - size) + m_Planes[i].b * (y + size) + m_Planes[i].c * (z + size) + m_Planes[i].d > 0)
		   continue;
		if(m_Planes[i].a * (x + size) + m_Planes[i].b * (y + size) + m_Planes[i].c * (z + size) + m_Planes[i].d > 0)
		   continue;

		// If we get here, it isn't in the frustum
		return false;
	}
	return true;
}
