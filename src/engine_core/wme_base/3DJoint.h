// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DJOINT_H__2D42CAFA_C0F0_4950_991F_C239A1499681__INCLUDED_)
#define AFX_3DJOINT_H__2D42CAFA_C0F0_4950_991F_C239A1499681__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C3DJoint  
{
public:
	HRESULT Persist(CBPersistMgr *PersistMgr);

	C3DJoint();
	virtual ~C3DJoint();

	D3DXVECTOR3 m_LocalRotation;
	D3DXVECTOR3 m_LocalTranslation;
	D3DXMATRIX m_Absolute;
	D3DXMATRIX m_Relative;

	//int m_NumRotationKeyframes;
	//int m_NumTranslationKeyframes;
	
	//C3DKeyframe* m_TranslationKeyframes;
	//C3DKeyframe* m_RotationKeyframes;

	int m_CurrentTranslationKeyframes[NUM_SKEL_ANIM_CHANNELS];
	int m_CurrentRotationKeyframes[NUM_SKEL_ANIM_CHANNELS];

	D3DXMATRIX m_Final;

	int m_Parent;

	char* m_Name;
};

#endif // !defined(AFX_3DJOINT_H__2D42CAFA_C0F0_4950_991F_C239A1499681__INCLUDED_)
