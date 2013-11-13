// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DJoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
C3DJoint::C3DJoint()
{
	m_LocalRotation = m_LocalTranslation = D3DXVECTOR3(0, 0, 0);

	D3DXMatrixIdentity(&m_Absolute);
	D3DXMatrixIdentity(&m_Relative);
	D3DXMatrixIdentity(&m_Final);

	//m_NumRotationKeyframes = m_NumTranslationKeyframes = 0;
	
	//m_TranslationKeyframes = m_RotationKeyframes = NULL;

	for(int i=0; i<NUM_SKEL_ANIM_CHANNELS; i++)
		m_CurrentTranslationKeyframes[i] = m_CurrentRotationKeyframes[i] = -1;

	m_Parent = -1;

	m_Name = NULL;
}


//////////////////////////////////////////////////////////////////////////
C3DJoint::~C3DJoint()
{
	//SAFE_DELETE_ARRAY(m_TranslationKeyframes);
	//SAFE_DELETE_ARRAY(m_RotationKeyframes);

	SAFE_DELETE_ARRAY(m_Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DJoint::Persist(CBPersistMgr *PersistMgr)
{
	
	PersistMgr->Transfer(TMEMBER(m_LocalRotation));
	PersistMgr->Transfer(TMEMBER(m_LocalTranslation));
	PersistMgr->Transfer(TMEMBER(m_Absolute));
	PersistMgr->Transfer(TMEMBER(m_Relative));
	
	PersistMgr->Transfer(TMEMBER(m_Final));

	
	for(int i=0; i<NUM_SKEL_ANIM_CHANNELS; i++)
	{
		PersistMgr->Transfer(TMEMBER(m_CurrentTranslationKeyframes[i]));
		PersistMgr->Transfer(TMEMBER(m_CurrentRotationKeyframes[i]));
	}

	PersistMgr->Transfer(TMEMBER(m_Parent));
	
	return S_OK;
}