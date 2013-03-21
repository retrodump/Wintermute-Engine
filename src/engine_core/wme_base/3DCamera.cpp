// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DCamera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
C3DCamera::C3DCamera(CBGame* inGame):CBNamedObject(inGame)
{
	m_Position = D3DXVECTOR3(0,0,0);
	m_Target = D3DXVECTOR3(0,0,0);
	m_Bank = 0.0f;
	m_FOV = m_OrigFOV = DegToRad(45.0f);
	m_NearClipPlane = m_FarClipPlane = -1.0f;
}


//////////////////////////////////////////////////////////////////////////
C3DCamera::~C3DCamera()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DCamera::GetViewMatrix(D3DXMATRIX *ViewMatrix)
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	if(m_Bank!=0)
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationZ(&rot, DegToRad(m_Bank));
		D3DXVec3TransformCoord(&up, &up, &rot);
	}

	D3DXMatrixLookAtLH(ViewMatrix, &m_Position, &m_Target, &up);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void C3DCamera::SetupPos(D3DXVECTOR3 Pos, D3DXVECTOR3 Target, float Bank)
{
	m_Position = Pos;
	m_Target = Target;
	m_Bank = Bank;
}


//////////////////////////////////////////////////////////////////////////
void C3DCamera::RotateView(float X, float Y, float Z)
{
	D3DXVECTOR3 vVector;						// Vector for the position/view.

	// Get our view vector (The direciton we are facing)
	vVector = m_Target - m_Position;		// This gets the direction of the view
	
	// Rotate the view along the desired axis
	if(X) {
		// Rotate the view vector up or down, then add it to our position
		m_Target.z = (float)(m_Position.z + sin(X)*vVector.y + cos(X)*vVector.z);
		m_Target.y = (float)(m_Position.y + cos(X)*vVector.y - sin(X)*vVector.z);
	}
	if(Y) {
		// Rotate the view vector right or left, then add it to our position
		m_Target.z = (float)(m_Position.z + sin(Y)*vVector.x + cos(Y)*vVector.z);
		m_Target.x = (float)(m_Position.x + cos(Y)*vVector.x - sin(Y)*vVector.z);
	}
	if(Z) {
		// Rotate the view vector diagnally right or diagnally down, then add it to our position
		m_Target.x = (float)(m_Position.x + sin(Z)*vVector.y + cos(Z)*vVector.x);		
		m_Target.y = (float)(m_Position.y + cos(Z)*vVector.y - sin(Z)*vVector.x);
	}

}


//////////////////////////////////////////////////////////////////////////
void C3DCamera::Move(float Speed)
{
	D3DXVECTOR3 vVector;						// Init a vector for our view

	// Get our view vector (The direciton we are facing)
	vVector = m_Target - m_Position;		// This gets the direction of the view
	
	m_Position.x += vVector.x * Speed;		// Add our acceleration to our position's X
	m_Position.z += vVector.z * Speed;		// Add our acceleration to our position's Z
	m_Target.x += vVector.x * Speed;			// Add our acceleration to our view's X
	m_Target.z += vVector.z * Speed;			// Add our acceleration to our view's Z
}
