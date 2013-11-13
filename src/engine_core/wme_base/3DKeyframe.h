// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DKEYFRAME_H__2748AD85_B731_4E9E_82EA_20D978160084__INCLUDED_)
#define AFX_3DKEYFRAME_H__2748AD85_B731_4E9E_82EA_20D978160084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C3DKeyframe  
{
public:
	C3DKeyframe();
	virtual ~C3DKeyframe();
	int m_JointIndex;
	float m_Time;	// in milliseconds
	D3DXVECTOR3 m_Parameter;
};

#endif // !defined(AFX_3DKEYFRAME_H__2748AD85_B731_4E9E_82EA_20D978160084__INCLUDED_)
